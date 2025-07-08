#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "src/temp-umi/aht10.h"
#include "src/display/display.h"

int main() {
    stdio_init_all();
    aht10_init();
    init_display();

    while (true) {
        if (aht10_read_data(&aht10_data)) {
            printf("Umidade: %.2f %%\nTemperatura: %.2f C\n\n", aht10_data.humidity, aht10_data.temperature);
            show_temp_humi();
        } else {
            printf("Falha na leitura do AHT10. Tentando resetar...\n");
            aht10_reset();
            sleep_ms(100);
        }
        sleep_ms(200);
    }

    return 0;
}