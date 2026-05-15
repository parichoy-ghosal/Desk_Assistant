#ifndef STORAGE_H
#define STORAGE_H

#include "FreeRTOS.h"
#include "task.h"

extern volatile int isSaving;

void storage_task(void *params);

#endif