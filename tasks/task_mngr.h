#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H

#include "pico/time.h"

#define MAX_TASKS 10

typedef struct {
    char title[32];
    int completed;
} Task;

void task_manager_init();
Task* get_tasks();
int get_task_count();
void toggle_task(int index);
void add_task(const char *title);
void set_task_count(int count);
void set_isDirty (int flag);
int get_isDirty (void);
void clearDirty();
absolute_time_t get_lastChangeTime();



#endif