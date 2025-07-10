#include <stdio.h>
#include <stdbool.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"
#include "./src/light_sensor/bht1750.h"
#include "./src/display/display.h"

int main() {
    stdio_init_all();
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