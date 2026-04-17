#include "buttons.h"
#include "hardware/gpio.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "events.h"

extern QueueHandle_t inputQueue;

#define BTN_UP     14
#define BTN_DOWN   15
#define BTN_LEFT   16
#define BTN_RIGHT  17
#define BTN_SELECT 18

void buttons_init() {
    int pins[] = {BTN_UP, BTN_DOWN, BTN_LEFT, BTN_RIGHT, BTN_SELECT};

    for (int i = 0; i < 5; i++) {
        gpio_init(pins[i]);
        gpio_set_dir(pins[i], GPIO_IN);
        gpio_pull_up(pins[i]);
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