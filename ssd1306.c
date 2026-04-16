#include "ssd1306.h"
#include <stdio.h>

void ssd1306_init(i2c_inst_t *i2c) {
    printf("OLED initialized\n");
}

void ssd1306_clear() {
    printf("OLED clear\n");
}

void ssd1306_draw_text(int x, int y, const char *text) {
    printf("OLED: %s\n", text);
}

void ssd1306_show() {
    // placeholder
}