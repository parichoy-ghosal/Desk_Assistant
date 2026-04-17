#include <stdio.h>
#include "pico/stdlib.h"

#include "FreeRTOS.h"
#include "task.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"
#include "queue.h"

#define I2C_PORT i2c0
#define SDA_PIN 4
#define SCL_PIN 5

#define BTN_UP     14
#define BTN_DOWN   15
#define BTN_LEFT   16
#define BTN_RIGHT  17
#define BTN_SELECT 18

QueueSetHandle_t inputQueue;

void i2c_init_display() {
    i2c_init(I2C_PORT, 400000);

    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);

    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);
}

void buttons_init() {
    int pins[] = {BTN_UP, BTN_DOWN, BTN_LEFT, BTN_RIGHT, BTN_SELECT};

    for (int i = 0; i < 5; i++) {
        gpio_init(pins[i]);
        gpio_set_dir(pins[i], GPIO_IN);
        gpio_pull_up(pins[i]); // active LOW
    }
}

typedef enum {
    EVENT_NONE,
    EVENT_UP,
    EVENT_DOWN,
    EVENT_LEFT,
    EVENT_RIGHT,
    EVENT_SELECT
} InputEvent;

typedef enum {
    SCREEN_IDLE,
    SCREEN_MENU
} ScreenState;

ScreenState currentScreen = SCREEN_IDLE;

const char *menuItems[] = {
    "Tasks",
    "Timer",
    "Settings"
};

int menuIndex = 0;
int menuSize = 3;

void ui_task(void *params) {

    InputEvent event;
    int phase = 0;

    while (1) {

        if (xQueueReceive(inputQueue, &event, 0)) {

            if (currentScreen == SCREEN_IDLE) {
                if (event == EVENT_SELECT) {
                    currentScreen = SCREEN_MENU;
                }
            }

            else if (currentScreen == SCREEN_MENU) {

                if (event == EVENT_UP) {
                    menuIndex--;
                    if (menuIndex < 0) menuIndex = menuSize - 1;
                }

                if (event == EVENT_DOWN) {
                    menuIndex++;
                    if (menuIndex >= menuSize) menuIndex = 0;
                }

                if (event == EVENT_SELECT) {
                    printf("Selected: %s\n", menuItems[menuIndex]);
                }

                if (event == EVENT_LEFT) {
                    currentScreen = SCREEN_IDLE;
                }
            }
        }

        ssd1306_clear();

        if (currentScreen == SCREEN_IDLE) {

            ssd1306_text(0, 0, "TuiTui");
            ssd1306_text(0, 16, "RTOS Desk Assistant");
            draw_fire(30, 55, 50, 20, phase);
        }

        else if (currentScreen == SCREEN_MENU) {

            ssd1306_text(0, 0, "Menu");

            for (int i = 0; i < menuSize; i++) {

                if (i == menuIndex) {
                    ssd1306_text(0, 16 + i * 8, ">");
                }

                ssd1306_text(10, 16 + i * 8, menuItems[i]);
            }
        }

        ssd1306_show();

        phase++;
        vTaskDelay(pdMS_TO_TICKS(80));
    }
}

void input_task(void *params) {

    while (1) {

        if (!gpio_get(BTN_UP)) {
            InputEvent e = EVENT_UP;
            xQueueSend(inputQueue, &e, 0);
        }

        if (!gpio_get(BTN_DOWN)) {
            InputEvent e = EVENT_DOWN;
            xQueueSend(inputQueue, &e, 0);
        }

        if (!gpio_get(BTN_LEFT)) {
            InputEvent e = EVENT_LEFT;
            xQueueSend(inputQueue, &e, 0);
        }

        if (!gpio_get(BTN_RIGHT)) {
            InputEvent e = EVENT_RIGHT;
            xQueueSend(inputQueue, &e, 0);
        }

        if (!gpio_get(BTN_SELECT)) {
            InputEvent e = EVENT_SELECT;
            xQueueSend(inputQueue, &e, 0);
        }

        vTaskDelay(pdMS_TO_TICKS(150)); // debounce
    }
}

int main() {
    stdio_init_all();
    i2c_init_display();
    ssd1306_init(I2C_PORT);

    sleep_ms(2000);
    buttons_init();
    sleep_ms(1000);

    inputQueue = xQueueCreate(10, sizeof(InputEvent));

    xTaskCreate(input_task, "Input", 256, NULL, 2, NULL);
    xTaskCreate(ui_task, "UI", 512, NULL, 1, NULL);

    vTaskStartScheduler();

    while (1);
}
