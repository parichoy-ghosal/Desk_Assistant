#include "task_mngr.h"
#include <string.h>

static Task tasks[MAX_TASKS] = {
    {"Finish Project", 0},
    {"Workout", 0},
    {"Read Book", 1},
};

static int taskCount = 3;

void task_manager_init() {}

Task* get_tasks() {
    return tasks;
}

int get_task_count() {
    return taskCount;
}

void toggle_task(int index) {
    if (index < 0 || index >= taskCount) return;
    tasks[index].completed = !tasks[index].completed;
}

void add_task(const char *title) {
    if (taskCount >= MAX_TASKS) return;

    strncpy(tasks[taskCount].title, title, 31);
    tasks[taskCount].title[31] = '\0';
    tasks[taskCount].completed = 0;

    taskCount++;
}