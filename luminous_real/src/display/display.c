#include "pico/stdlib.h"
#include "src/display/ssd1306.h"
#include "src/display/display.h"

//==============================================================================
// Exibição no Display
//==============================================================================

ssd1306_t ssd;

// Inicializa o display
void init_display()
{
    ssd1306_init_config_clean(&ssd, I2C_SCL, I2C_SDA, I2C_PORT, I2C_LINK);
}

// Função para desenhar o texto fixo do display
void draw_lux_static_text()
{
    ssd1306_clear_screen(&ssd);
    ssd1306_draw_string(&ssd, "Nivel de lux", 20, 10);
    ssd1306_draw_string(&ssd, "--------", 35, 20);
    ssd1306_send_data(&ssd);
}

// Exibe nível de lux no display, atualizando apenas se mudar
void show_lux_level(float lux_level)
{
    static float last_lux_level = -1.0f;
    if (last_lux_level < 0.0f) {
        // Primeira vez: desenha texto fixo
        draw_lux_static_text();
    }
    // Só atualiza se o valor mudou significativamente
    if (fabsf(lux_level - last_lux_level) > 0.01f) {
        // Apaga apenas a área do valor anterior
        ssd1306_draw_filled_rect(&ssd, 35, 30, 60, 10, 0); // 0 = cor preta
        char *lux_str = float_to_string(lux_level);
        ssd1306_draw_string(&ssd, lux_str, 35, 30);
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