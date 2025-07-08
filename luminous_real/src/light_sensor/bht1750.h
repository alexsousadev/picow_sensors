#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

// Configuração do i2c
#define I2C_PORT_BH1750 i2c0
#define I2C_SDA_PIN_BH1750 0
#define I2C_SCL_PIN_BH1750 1

// Endereço do sensor BH1750
#define BH1750_ADDR 0x23
#define BH1750_CONTINUOUS_HIGH_RES_MODE 0x10

void bh1750_init();
float bh1750_read();