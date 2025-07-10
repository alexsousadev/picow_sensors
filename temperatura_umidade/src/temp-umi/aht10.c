#include "aht10.h"

aht10_data_t aht10_data;

// Inicializa os pinos do AHT10
void aht10_init_pins() {
    i2c_init(AHT10_I2C_PORT, AHT10_I2C_BAUDRATE);
    gpio_set_function(AHT10_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(AHT10_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(AHT10_SDA_PIN);
    gpio_pull_up(AHT10_SCL_PIN);
}

// Envia um comando para o AHT10
bool aht10_send_command(uint8_t cmd, uint8_t *data, size_t data_len) {
    // Validação dos parâmetros
    if (data_len > 3) {
        return false; 
    }
    
    // Prepara o buffer de comando
    uint8_t command_buffer[4] = {0};
    command_buffer[0] = cmd;
    
    // Copia dados adicionais se fornecidos
    if (data != NULL && data_len > 0) {
        for (size_t i = 0; i < data_len; i++) {
            command_buffer[i + 1] = data[i];
        }
    }
    
    // Envia comando
    int bytes_written = i2c_write_blocking(
        AHT10_I2C_PORT, 
        AHT10_ADDR, 
        command_buffer, 
        data_len + 1, 
        false
    );
    
    // Verifica se a escrita foi bem-sucedida
    return (bytes_written == (int)(data_len + 1));
}

// Lê bytes do AHT10
bool aht10_read_bytes(uint8_t *buffer, size_t len) {
    int ret = i2c_read_blocking(AHT10_I2C_PORT, AHT10_ADDR, buffer, len, false);
    return (ret != PICO_ERROR_GENERIC);
}

// Espera o AHT10 estar pronto
bool aht10_wait_for_ready() {
    uint8_t status_byte;
    if (!aht10_read_bytes(&status_byte, 1)) {
        return false;
    }
    return !(status_byte & AHT10_STATUS_BUSY_MASK);
}

// Verifica se o AHT10 está calibrado
bool aht10_check_calibration() {
    uint8_t status;
    if (!aht10_read_bytes(&status, 1)) {
        return false;
    }
    return (status & AHT10_STATUS_CAL_MASK);
}

// Converte dados brutos para valores reais
void aht10_convert_raw_data(uint8_t *raw_data, aht10_data_t *data) {
    // Extrai umidade (20 bits)
    uint32_t raw_humidity = ((uint32_t)raw_data[1] << 16) | ((uint32_t)raw_data[2] << 8) | raw_data[3];
    raw_humidity = raw_humidity >> 4;

    // Extrai temperatura (20 bits)
    uint32_t raw_temperature = ((uint32_t)raw_data[3] & 0x0F) << 16 | ((uint32_t)raw_data[4] << 8) | raw_data[5];

    // Converte para valores reais
    data->humidity = (float)raw_humidity * 100.0f / 1048576.0f;
    data->temperature = (float)raw_temperature * 200.0f / 1048576.0f - 50.0f;
}

// Inicializa o AHT10
void aht10_init() {
    aht10_init_pins();

    printf("Configurando I2C para sensor AHT10...\n");
    aht10_reset();

    uint8_t init_data[2] = {0x08, 0x00};
    if (!aht10_send_command(AHT10_CMD_INITIALIZE, init_data, 2)) {
        printf("Falha ao inicializar sensor AHT10\n");
        return;
    }

    sleep_ms(300);

    if (!aht10_check_calibration()) {
        printf("Sensor AHT10 não calibrado - reinicie o sistema\n");
    } else {
        printf("Sensor AHT10 inicializado com sucesso\n");
    }
}

// Reseta o AHT10
void aht10_reset() {
    if (!aht10_send_command(AHT10_CMD_SOFT_RESET, NULL, 0)) {
        printf("Falha ao resetar sensor AHT10\n");
    }
    sleep_ms(20);
}

// Lê dados do AHT10
bool aht10_read_data(aht10_data_t *data) {
    uint8_t measure_data[2] = {0x33, 0x00};
    // Envia comando de medição
    if (!aht10_send_command(AHT10_CMD_MEASURE, measure_data, 2))
        return false;
    sleep_ms(80);

    // Espera o sensor estar pronto
    if (!aht10_wait_for_ready())
        return false;

    // Lê os dados brutos
    uint8_t raw_data[6];
    if (!aht10_read_bytes(raw_data, 6))
        return false;
    aht10_convert_raw_data(raw_data, data);
    return true;
}
