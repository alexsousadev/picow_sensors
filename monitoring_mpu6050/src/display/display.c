#include "pico/stdlib.h"
#include "src/display/ssd1306.h"
#include "src/display/display.h"
#include "src/mpu6050/mpu6050.h"

//==============================================================================
// Exibição no Display
//==============================================================================

ssd1306_t ssd;
extern mpu6050_data_t current_mpu6050_data;

char pitch_str[32];
char roll_str[32];

// Inicializa o display
void init_display()
{
    ssd1306_init_config_clean(&ssd, I2C_SCL, I2C_SDA, I2C_PORT, I2C_LINK);
}

// Converte float para string
char *float_to_string(float num)
{
    char *str = (char *)malloc(12 * sizeof(char));
    if (str == NULL)
        return NULL;
    // Usa snprintf com formato fixo e força o ponto como separador decimal
    snprintf(str, 12, "%0.2f", num);
    // Garante que o separador decimal é ponto
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == ',') {
            str[i] = '.';
        }
    }
    return str;
}

// Função para limpar uma linha específica do display
void clear_display_line(int y_position, int x_start, int width)
{
    ssd1306_draw_string(&ssd, "                    ", x_start, y_position);
}

// Exibe os valores do MPU6050 no display
void show_mpu6050_values()
{
    ssd1306_clear_screen(&ssd);
    snprintf(pitch_str, sizeof(pitch_str), "Pitch: %.1f", current_mpu6050_data.pitch);
    snprintf(roll_str, sizeof(roll_str), "Roll: %.1f", current_mpu6050_data.roll);
    ssd1306_draw_string(&ssd, pitch_str, 0, 0);
    ssd1306_draw_string(&ssd, roll_str, 0, 10);
    ssd1306_send_data(&ssd);

    if (current_mpu6050_data.pitch > LIMIT_ANGLE || current_mpu6050_data.roll > LIMIT_ANGLE){

        if(current_mpu6050_data.pitch > LIMIT_ANGLE){
            ssd1306_draw_string(&ssd, "Pitch no limite\n", 0, 30);
            printf("Pitch no limite\n");    
            ssd1306_send_data(&ssd);
        }

        if(current_mpu6050_data.roll > LIMIT_ANGLE){
            ssd1306_draw_string(&ssd, "Roll no limite\n", 0, 40);
            printf("Roll no limite\n");
            ssd1306_send_data(&ssd);
        }

    }

}