#include "fs_tasks.h"

#include "task_mngr.h"

#include "ff.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

FATFS fs;

void save_tasks_to_sd() {

    FIL file;

    if (f_open(&file,
               "tasks.txt",
               FA_WRITE | FA_CREATE_ALWAYS) != FR_OK) {
        return;
    }

    Task *tasks = get_tasks();
    int count = get_task_count();

    for (int i = 0; i < count; i++) {

        char line[64];

        sprintf(line,
                "%s,%d\n",
                tasks[i].title,
                tasks[i].completed);

        UINT written;

        f_write(&file,
                line,
                strlen(line),
                &written);
    }

    f_close(&file);
}

void load_tasks_from_sd() {

    FIL file;

    if (f_open(&file,
               "tasks.txt",
               FA_READ) != FR_OK) {
        return;
    }

    char line[64];

    int index = 0;

    Task *tasks = get_tasks();

    while (f_gets(line,
                  sizeof(line),
                  &file)) {

        char *comma = strchr(line, ',');

        if (!comma)
            continue;

        *comma = '\0';

        strcpy(tasks[index].title, line);

        tasks[index].completed = atoi(comma + 1);

        index++;
    }

    set_task_count(index);

    f_close(&file);
}