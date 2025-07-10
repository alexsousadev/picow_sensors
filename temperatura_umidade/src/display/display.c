#include "pico/stdlib.h"
#include "src/display/ssd1306.h"
#include "src/display/display.h"
#include "src/temp-umi/aht10.h"

#define TEMP_WARNING_LIMIT 33.0f
#define HUMI_WARNING_LIMIT 50.0f

ssd1306_t ssd;
char temperature_str[32];
char humidity_str[32];

// Inicializa o display
void init_display()
{
    ssd1306_init_config_clean(&ssd, I2C_SCL, I2C_SDA, I2C_PORT, I2C_LINK);
}

// Exibe a temperatura e a umidade no display
void show_temp_humi()
{
    ssd1306_clear_screen(&ssd);
    snprintf(temperature_str, sizeof(temperature_str), "Temp: %.1f C", aht10_data.temperature);
    snprintf(humidity_str, sizeof(humidity_str), "Umid: %.1f %%", aht10_data.humidity);
    ssd1306_draw_string(&ssd, temperature_str, 0, 0);
    ssd1306_draw_string(&ssd, humidity_str, 0, 10);
    ssd1306_send_data(&ssd);
    define_warning();
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

// Define o aviso de temperatura ou umidade acima do limite
void define_warning(){
    bool alert = false;
    if (aht10_data.temperature > TEMP_WARNING_LIMIT || aht10_data.humidity > HUMI_WARNING_LIMIT){ 
        ssd1306_draw_string(&ssd, "-- ALERTA --", 20, 30);
        alert = true;
        if (aht10_data.temperature > TEMP_WARNING_LIMIT){
            ssd1306_draw_string(&ssd, "Temp acima", 0, 40);
        }
        if (aht10_data.humidity > HUMI_WARNING_LIMIT){
            ssd1306_draw_string(&ssd, "Umi acima", 0, 50);
        }
    }
    if (alert) {
        ssd1306_send_data(&ssd);
    }
}

