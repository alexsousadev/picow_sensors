#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"

// Definições de endereço e comandos
#define AHT10_ADDR 0x38
#define AHT10_CMD_INITIALIZE 0xE1
#define AHT10_CMD_MEASURE 0xAC
#define AHT10_CMD_SOFT_RESET 0xBA
#define AHT10_STATUS_BUSY_MASK 0x80
#define AHT10_STATUS_CAL_MASK 0x08

// Definições de pinos e baudrate
#define AHT10_I2C_PORT i2c0
#define AHT10_SDA_PIN 0
#define AHT10_SCL_PIN 1
#define AHT10_I2C_BAUDRATE 100000

typedef struct aht10_data {
    float humidity;
    float temperature;
} aht10_data_t;

extern aht10_data_t aht10_data;

// Funções principais
void aht10_init();
void aht10_reset();
bool aht10_read_data(aht10_data_t *data);

// Funções auxiliares para modularização
void aht10_init_pins();
bool aht10_send_command(uint8_t cmd, uint8_t *data, size_t data_len);
bool aht10_read_bytes(uint8_t *buffer, size_t len);
bool aht10_wait_for_ready();
bool aht10_check_calibration();
void aht10_convert_raw_data(uint8_t *raw_data, aht10_data_t *data);

