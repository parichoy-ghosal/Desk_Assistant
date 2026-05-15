#include "storage.h"

#include "task_mngr.h"

#include "hardware/flash.h"
#include "hardware/sync.h"

#include <string.h>

#define FLASH_TARGET_OFFSET (1024 * 1024)

#define TASK_MAGIC 0x5441534B

typedef struct {

    uint32_t magic;
    uint32_t count;

    Task tasks[MAX_TASKS];

} SaveData;

volatile int isSaving = 0;

void save_tasks() {

    isSaving = 1;

    SaveData data;

    data.magic = TASK_MAGIC;

    Task *tasks = get_tasks();

    int count = get_task_count();

    data.count = count;

    memcpy(data.tasks,
           tasks,
           sizeof(Task) * count);

    uint32_t interrupts = save_and_disable_interrupts();

    flash_range_erase(
        FLASH_TARGET_OFFSET,
        FLASH_SECTOR_SIZE
    );

    flash_range_program(
        FLASH_TARGET_OFFSET,
        (const uint8_t *)&data,
        sizeof(SaveData)
    );

    restore_interrupts(interrupts);

    isSaving = 0;
}

void load_tasks() {

    const SaveData *data =
        (const SaveData *)(XIP_BASE + FLASH_TARGET_OFFSET);

    if (data->magic != TASK_MAGIC)
        return;

    Task *tasks = get_tasks();

    memcpy(tasks,
           data->tasks,
           sizeof(Task) * data->count);

    set_task_count(data->count);
}