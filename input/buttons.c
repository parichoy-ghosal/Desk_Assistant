#include "buttons.h"
#include "hardware/gpio.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "events.h"
#include "pico/time.h"

extern QueueHandle_t inputQueue;

#define BTN_UP     14
#define BTN_DOWN   15
#define BTN_LEFT   16
#define BTN_RIGHT  17
#define BTN_SELECT 18

#define LongPress 800
static uint32_t press_time = 0;
static int pressed = 0;
static int long_sent = 0;

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

            if (!pressed) {
                press_time = to_ms_since_boot(get_absolute_time());
                pressed = 1;
                long_sent = 0;
            }

            uint32_t now = to_ms_since_boot(get_absolute_time());

            if (!long_sent && (now - press_time > LongPress)) {
                InputEvent e = EVENT_SAVE;
                xQueueSend(inputQueue, &e, 0);

                long_sent = 1;
            }

        } 
        else {

            if (pressed && !long_sent) {
                InputEvent e = EVENT_SELECT;
                xQueueSend(inputQueue, &e, 0);
            }

            pressed = 0;
        }

        vTaskDelay(pdMS_TO_TICKS(50)); // debounce
    }
}