#include "src/mpu6050/mpu6050.h"
#include <stdio.h>
#include <string.h>

mpu6050_data_t current_mpu6050_data;

// Inicializa os pinos do MPU6050
void mpu6050_init_pins(void) {
    i2c_init(MPU6050_I2C_PORT, MPU6050_I2C_BAUDRATE);
    gpio_set_function(MPU6050_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(MPU6050_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(MPU6050_SDA_PIN);
    gpio_pull_up(MPU6050_SCL_PIN);
    sleep_ms(100);
}

// Inicializa o MPU6050
void mpu6050_init() {
    mpu6050_init_pins();
    
    uint8_t config_data[2];
    config_data[0] = MPU6050_PWR_MGMT_1; 
    config_data[1] = 0x00; 
    
    int result = i2c_write_blocking(MPU6050_I2C_PORT, MPU6050_ADDR, config_data, 2, false);
    if (result == PICO_ERROR_GENERIC) {
        printf("Erro ao acordar MPU6050! Verifique conexoes e endereco I2C.\n");
    } else {
        printf("MPU6050 acordado e inicializado com sucesso.\n");
    }
    sleep_ms(100);
}

// Função para ler dados brutos do MPU6050
void mpu6050_read_raw(int16_t accel[3], int16_t gyro[3]) {
    uint8_t sensor_data[14];

    uint8_t register_addr = MPU6050_ACCEL_XOUT_H;
    int result = i2c_write_blocking(MPU6050_I2C_PORT, MPU6050_ADDR, &register_addr, 1, true); 
    if (result == PICO_ERROR_GENERIC) {
        printf("Erro ao solicitar leitura de dados do MPU6050.\n");
        memset(accel, 0, sizeof(int16_t)*3);
        memset(gyro, 0, sizeof(int16_t)*3);
        return;
    }
    
    result = i2c_read_blocking(MPU6050_I2C_PORT, MPU6050_ADDR, sensor_data, 14, false);
    if (result == PICO_ERROR_GENERIC) {
        printf("Erro ao ler dados do MPU6050.\n");
        memset(accel, 0, sizeof(int16_t)*3);
        memset(gyro, 0, sizeof(int16_t)*3);
        return;
    }

    accel[0] = (int16_t)((sensor_data[0] << 8) | sensor_data[1]);
    accel[1] = (int16_t)((sensor_data[2] << 8) | sensor_data[3]);
    accel[2] = (int16_t)((sensor_data[4] << 8) | sensor_data[5]);

    gyro[0] = (int16_t)((sensor_data[8] << 8) | sensor_data[9]);
    gyro[1] = (int16_t)((sensor_data[10] << 8) | sensor_data[11]);
    gyro[2] = (int16_t)((sensor_data[12] << 8) | sensor_data[13]);
}

void convert_raw_to_g_units(int16_t accel[3], float accel_g[3]) {
    accel_g[0] = (float)accel[0] / ACCEL_SCALE_FACTOR;
    accel_g[1] = (float)accel[1] / ACCEL_SCALE_FACTOR;
    accel_g[2] = (float)accel[2] / ACCEL_SCALE_FACTOR;
}

// Calcula os ângulos Pitch e Roll a partir dos dados do acelerômetro
void calculate_angles(int16_t accel[3], float *pitch, float *roll) {
    float accel_g[3];
    convert_raw_to_g_units(accel, accel_g);

    *pitch = atan2f(accel_g[0], sqrtf(accel_g[1] * accel_g[1] + accel_g[2] * accel_g[2])) * 180.0f / M_PI;
    *roll = atan2f(accel_g[1], sqrtf(accel_g[0] * accel_g[0] + accel_g[2] * accel_g[2])) * 180.0f / M_PI;
}

// Função para ler todos os dados do MPU6050
void mpu6050_read_values() {
    mpu6050_read_raw(current_mpu6050_data.accel, current_mpu6050_data.gyro);
    calculate_angles(current_mpu6050_data.accel, &current_mpu6050_data.pitch, &current_mpu6050_data.roll);
}

// Função para verificar alertas de inclinação
void mpu6050_check_alert(mpu6050_data_t *data) {
    if (fabsf(data->pitch) > ANGULO_ALERTA_GRAUS || fabsf(data->roll) > ANGULO_ALERTA_GRAUS) {
        printf("\n🚨 ALERTA: Inclinacao excedida! 🚨\n");
        printf("   Pitch: %.2f° (limite: %.1f°)\n", data->pitch, ANGULO_ALERTA_GRAUS);
        printf("   Roll: %.2f° (limite: %.1f°)\n", data->roll, ANGULO_ALERTA_GRAUS);
        printf("   Ajuste a posicao do sensor!\n\n");
    }
}