#include <stdio.h>
#include <stdbool.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"
#include "./src/light_sensor/bht1750.h"
#include "./src/display/display.h"

int main() {
    stdio_init_all();
    i2c_init(I2C_PORT_BH1750, 100 * 1000); // 100kHz
    gpio_set_function(I2C_SDA_PIN_BH1750, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_PIN_BH1750, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA_PIN_BH1750);
    gpio_pull_up(I2C_SCL_PIN_BH1750);
    init_display();
    bh1750_init();
    sleep_ms(100);

    while (true) {
        float lux = bh1750_read();
        show_lux_level(lux);
        if (lux >= 0) {
            printf("Luminosidade: %.2f lux\n", lux);
        }
        sleep_ms(100);
    }

    return 0;
}