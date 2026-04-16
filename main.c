#include <stdio.h>
#include "pico/stdlib.h"

#include "FreeRTOS.h"
#include "task.h"
#include "hardware/i2c.h"

#define I2C_PORT i2c0
#define SDA_PIN 4
#define SCL_PIN 5

void i2c_init_display() {
    i2c_init(I2C_PORT, 400000);

    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);

    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);
}

void task1(void *params) {
    while (1) {
        printf("Task 1 running\n");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void task2(void *params) {
    while (1) {
        printf("Task 2 running\n");
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void ui_task(void *params) {
    while (1) {
        ssd1306_clear();
        ssd1306_draw_text(0, 0, "Desk Assistant");
        ssd1306_draw_text(0, 16, "RTOS Running");
        ssd1306_show();

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

int main() {
    stdio_init_all();
    i2c_init_display();
    ssd1306_init(I2C_PORT);

    sleep_ms(2000);

    xTaskCreate(ui_task, "UI_Task", 256, NULL, 1, NULL);

    vTaskStartScheduler();

    while (1);
}
