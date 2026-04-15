#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#include <stdint.h>

/* CPU and timing */
#define configCPU_CLOCK_HZ              125000000
#define configTICK_RATE_HZ              1000

/* Task settings */
#define configMAX_PRIORITIES            5
#define configMINIMAL_STACK_SIZE        128
#define configTOTAL_HEAP_SIZE           (20 * 1024)

/* Kernel behavior */
#define configUSE_PREEMPTION            1
#define configUSE_TIME_SLICING          1

/* Tick configuration (IMPORTANT FIX) */
#define configUSE_16_BIT_TICKS          0

/* MPU (RP2040 DOES NOT USE MPU) */
#define configENABLE_MPU                0

/* Hooks (REQUIRED) */
#define configUSE_IDLE_HOOK             0
#define configUSE_TICK_HOOK             0

/* Sync features */
#define configUSE_MUTEXES               1
#define configUSE_COUNTING_SEMAPHORES   1

/* Optional but useful */
#define configSUPPORT_DYNAMIC_ALLOCATION 1
#define configSUPPORT_STATIC_ALLOCATION  0

/* API inclusion */
#define INCLUDE_vTaskDelay              1
#define INCLUDE_xTaskGetSchedulerState  1

#endif
