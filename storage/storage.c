#include "storage.h"

volatile int isSaving = 0;

void storage_task(void *params) {

    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}