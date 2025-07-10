#ifndef MPU6050_H
#define MPU6050_H

#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"
#include <math.h>

// Endereço I2C e registradores do MPU6050
#define MPU6050_ADDR           0x68
#define MPU6050_PWR_MGMT_1     0x6B
#define MPU6050_ACCEL_XOUT_H   0x3B

// Configuração do acelerômetro
#define ACCEL_SCALE_FACTOR     16384.0f

// Configuração I2C
#define MPU6050_I2C_PORT       i2c0
#define MPU6050_SDA_PIN        0
#define MPU6050_SCL_PIN        1
#define MPU6050_I2C_BAUDRATE   100000

// Limite de inclinação para alerta (graus)
#define LIMIT_ANGLE    60.0f

// Estrutura de dados do MPU6050
typedef struct {
    float pitch;
    float roll;
    int16_t accel[3];
    int16_t gyro[3];
} mpu6050_data_t;

extern mpu6050_data_t current_mpu6050_data;

// Protótipos de funções
void mpu6050_init();
void mpu6050_read_raw(int16_t accel[3], int16_t gyro[3]);
void calculate_angles(int16_t accel[3], float *pitch, float *roll);
void mpu6050_read_values();

#endif // MPU6050_H 