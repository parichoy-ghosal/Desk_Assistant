#ifndef STORAGE_H
#define STORAGE_H

#include "FreeRTOS.h"
#include "task.h"

void save_tasks();
void load_tasks();

extern volatile int isSaving;

#endif