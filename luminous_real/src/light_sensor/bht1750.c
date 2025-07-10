#include "bht1750.h"

void bh1750_init_i2c() {
    i2c_init(I2C_PORT_BH1750, 100 * 1000); // 100kHz
    gpio_set_function(I2C_SDA_PIN_BH1750, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_PIN_BH1750, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA_PIN_BH1750);
    gpio_pull_up(I2C_SCL_PIN_BH1750);
}

void bh1750_init() {
    bh1750_init_i2c();
    uint8_t buf[1] = {BH1750_CONTINUOUS_HIGH_RES_MODE};
    i2c_write_blocking(I2C_PORT_BH1750, BH1750_ADDR, buf, 1, false);
}

float bh1750_read() {
    uint8_t data[2];
    int result = i2c_read_blocking(I2C_PORT_BH1750, BH1750_ADDR, data, 2, false);
    if (result != 2) {
        printf("Erro ao ler o sensor\n");
        return -1;
    }
    uint16_t raw = (data[0] << 8) | data[1];
    return raw / 1.2; 
}