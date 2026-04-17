#include "ssd1306.h"
#include "pico/stdlib.h"
#include <string.h>
#include "font5x7.h"
#include <stdlib.h>

#define WIDTH 128
#define HEIGHT 64
#define ADDR 0x3C

static uint8_t buffer[WIDTH * HEIGHT / 8];
static i2c_inst_t *i2c_port;

void ssd1306_command(uint8_t cmd) {
    uint8_t buf[2] = {0x00, cmd};
    i2c_write_blocking(i2c_port, ADDR, buf, 2, false);
}

void ssd1306_data(uint8_t *data, size_t len) {
    uint8_t temp[len + 1];
    temp[0] = 0x40;
    memcpy(&temp[1], data, len);
    i2c_write_blocking(i2c_port, ADDR, temp, len + 1, false);
}

void ssd1306_init(i2c_inst_t *i2c) {
    i2c_port = i2c;

    sleep_ms(100);

    ssd1306_command(0xAE); // display off
    ssd1306_command(0xA8); // multiplex
    ssd1306_command(0x3F);

    ssd1306_command(0xD3); // offset
    ssd1306_command(0x00);

    ssd1306_command(0x40); // start line
    ssd1306_command(0xA1); // segment remap
    ssd1306_command(0xC8); // scan direction

    ssd1306_command(0xDA);
    ssd1306_command(0x12);

    ssd1306_command(0x81);
    ssd1306_command(0x7F);

    ssd1306_command(0xA4);
    ssd1306_command(0xA6);

    ssd1306_command(0xD5);
    ssd1306_command(0x80);

    ssd1306_command(0x8D);
    ssd1306_command(0x14);

    ssd1306_command(0xAF); // display ON

    memset(buffer, 0, sizeof(buffer));
}

void ssd1306_clear() {
    memset(buffer, 0, sizeof(buffer));
}

void ssd1306_pixel(int x, int y) {
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) return;
    buffer[x + (y / 8) * WIDTH] |= (1 << (y % 8));
}

void ssd1306_show() {
    for (int page = 0; page < 8; page++) {
        ssd1306_command(0xB0 + page);
        ssd1306_command(0x00);
        ssd1306_command(0x10);

        ssd1306_data(&buffer[WIDTH * page], WIDTH);
    }
}

void ssd1306_char(int x, int y, char c) {
    if (c < 32 || c > 127) return;

    const uint8_t *bitmap = font5x7[c - 32];

    for (int i = 0; i < 5; i++) {
        uint8_t line = bitmap[i];

        for (int j = 0; j < 8; j++) {
            if (line & (1 << j)) {
                ssd1306_pixel(x + i, y + j);
            }
        }
    }
}

void ssd1306_text(int x, int y, const char *str) {
    int start_x = x;

    while (*str) {
        if (*str == '\n') {
            y += 8;        // move down one line
            x = start_x;   // reset to starting column
        } else {
            ssd1306_char(x, y, *str);
            x += 6;        // character spacing
        }
        str++;
    }
}


void draw_fire(int base_x, int base_y, int width, int max_height, int phase) {

    for (int i = 0; i < width; i++) {

        // random flicker
        int flicker = rand() % 3;

        // breathing effect using phase
        int height = (max_height / 2) + (phase % (max_height / 2));

        height = height - flicker;

        if (height < 0) height = 0;

        for (int j = 0; j < height; j++) {
            ssd1306_pixel(base_x + i, base_y - j);
        }
    }
}