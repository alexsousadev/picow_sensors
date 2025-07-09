#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "pico/stdlib.h"
#include "./src/mpu6050/mpu6050.h"
#include "./src/display/display.h"

int main() {

    stdio_init_all(); 
    init_display();
    mpu6050_init(); 

    while (true) {
        mpu6050_read_values();

        // 2. Exibe os dados no console
        printf("=== DADOS DO MPU6050 ===\n");
        printf("  Acelerometro - X: %d, Y: %d, Z: %d\n", 
               current_mpu6050_data.accel[0], current_mpu6050_data.accel[1], current_mpu6050_data.accel[2]);
        printf("  Giroscopio   - X: %d, Y: %d, Z: %d\n", 
               current_mpu6050_data.gyro[0], current_mpu6050_data.gyro[1], current_mpu6050_data.gyro[2]);
        printf("========================\n\n");

        show_mpu6050_values();
        mpu6050_check_alert(&current_mpu6050_data);

        sleep_ms(100); 
    }

    return 0; 
}