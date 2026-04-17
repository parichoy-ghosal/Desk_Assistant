#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H

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

#endif