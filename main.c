#include <stdio.h>
#include "pico/stdlib.h"

#include "FreeRTOS.h"
#include "task.h"

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

int main() {
    stdio_init_all();

    sleep_ms(2000);

    xTaskCreate(task1, "Task1", 256, NULL, 1, NULL);
    xTaskCreate(task2, "Task2", 256, NULL, 1, NULL);

    vTaskStartScheduler();

    while (1);
}
