#include "pico/stdlib.h"
#include <math.h>
#include "src/display/ssd1306.h"
#include "src/display/display.h"

ssd1306_t ssd;

// Inicializa o display
void init_display()
{
    ssd1306_init_config_clean(&ssd, I2C_SCL, I2C_SDA, I2C_PORT, I2C_LINK);
}

// Desenhar o texto fixo do display
void draw_lux_static_text()
{
    ssd1306_clear_screen(&ssd);
    ssd1306_draw_string(&ssd, "Nivel de luz", 20, 10);
    ssd1306_draw_string(&ssd, "--------", 35, 20);
    ssd1306_send_data(&ssd);
}

// Função para limpar uma linha específica do display
void clear_display_line(int y_position, int x_start, int width)
{
    ssd1306_draw_string(&ssd, "                    ", x_start, y_position);
}
// Exibe nível de lux no display, atualizando apenas se mudar
void show_lux_level(float lux_level)
{
    static float last_lux_level = -1.0f;
    if (last_lux_level < 0.0f) {
        draw_lux_static_text();
    }
    // Só atualiza se o valor mudou significativamente
    if (fabsf(lux_level - last_lux_level) > 0.01f) {
        // Limpa apenas a linha onde o valor é exibido (linha 30)
        clear_display_line(30, 35, 70);
        ssd1306_send_data(&ssd);
        
        char *lux_str = float_to_string(lux_level);
        ssd1306_draw_string(&ssd, lux_str, 40, 30);
        free(lux_str);
        ssd1306_send_data(&ssd);
        last_lux_level = lux_level;
    }
}

// Converte float para string
char *float_to_string(float num)
{
    char *str = (char *)malloc(12 * sizeof(char));
    if (str == NULL)
        return NULL;
    snprintf(str, 12, "%0.2f", num);
    return str;
}