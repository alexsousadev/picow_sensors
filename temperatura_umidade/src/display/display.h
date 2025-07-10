#include <string.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "src/display/ssd1306.h"

// Definições de pinos
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define I2C_LINK 0x3C

// Declarações de variáveis globais
extern ssd1306_t ssd;

// Protótipos de funções
void init_display(void);
void show_temp_humi(void);
char *float_to_string(float num);
void clear_display_line(int y_position, int x_start, int width);
void define_warning();