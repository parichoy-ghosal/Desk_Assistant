#ifndef SSD1306_H
#define SSD1306_H

#include "hardware/i2c.h"

void ssd1306_init(i2c_inst_t *i2c);
void ssd1306_clear();
void ssd1306_draw_text(int x, int y, const char *text);
void draw_fire(int base_x, int base_y, int width, int max_height, int phase);
void ssd1306_show();


#endif