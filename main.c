#include <stdio.h>

#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/i2c.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "task_mngr.h"
#include "buttons.h"
#include "ui.h"
#include "events.h"
#include "ssd1306.h"
#include "storage.h"

#define I2C_PORT i2c0
#define SDA_PIN 4
#define SCL_PIN 5

QueueHandle_t inputQueue;

void i2c_init_display() {

    i2c_init(I2C_PORT, 400000);

    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);

    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);
}

int main() {

    stdio_init_all();

    sleep_ms(2000);

    i2c_init_display();

    ssd1306_init(I2C_PORT);

    ssd1306_clear();
    ssd1306_text(0, 0, "Desk Assistant");
    ssd1306_text(0, 16, "Booting...");
    ssd1306_show();

    sleep_ms(1000);

    buttons_init();

    task_manager_init();

    load_tasks();

    inputQueue = xQueueCreate(
        10,
        sizeof(InputEvent)
    );

    xTaskCreate(
        input_task,
        "Input",
        256,
        NULL,
        1,
        NULL
    );

    xTaskCreate(
        ui_task,
        "UI",
        512,
        NULL,
        2,
        NULL
    );

    vTaskStartScheduler();

    while (1) {
    }

    return 0;
}