cmake_minimum_required(VERSION 3.13)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(pico_rtos_project C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

# =========================================
# FatFs SPI Library
# =========================================
add_subdirectory(no-OS-FatFS-SD-SPI-RPi-Pico/FatFs_SPI)

# =========================================
# Main Executable
# =========================================
add_executable(pico_rtos
    main.c
)

# =========================================
# Include Directories
# =========================================
target_include_directories(pico_rtos PRIVATE

    ${CMAKE_CURRENT_LIST_DIR}

    # drivers
    ${CMAKE_CURRENT_LIST_DIR}/drivers

    # input
    ${CMAKE_CURRENT_LIST_DIR}/input

    # system
    ${CMAKE_CURRENT_LIST_DIR}/system

    # tasks
    ${CMAKE_CURRENT_LIST_DIR}/tasks

    # ui
    ${CMAKE_CURRENT_LIST_DIR}/ui

    # storage
    ${CMAKE_CURRENT_LIST_DIR}/storage

    # FreeRTOS
    ${CMAKE_CURRENT_LIST_DIR}/FreeRTOS-Kernel/include
    ${CMAKE_CURRENT_LIST_DIR}/FreeRTOS-Kernel/portable/ThirdParty/GCC/RP2040
    ${CMAKE_CURRENT_LIST_DIR}/FreeRTOS-Kernel/portable/ThirdParty/GCC/RP2040/include
)

# =========================================
# Source Files
# =========================================
target_sources(pico_rtos PRIVATE

    # main
    main.c

    # drivers
    drivers/ssd1306.c
    drivers/font5x7.c

    # input
    input/buttons.c

    # tasks
    tasks/task_mngr.c

    # ui
    ui/ui.c

    # storage
    storage/storage.c
    storage/fs_tasks.c
    storage/hw_config.c

    # FreeRTOS
    FreeRTOS-Kernel/tasks.c
    FreeRTOS-Kernel/queue.c
    FreeRTOS-Kernel/list.c
    FreeRTOS-Kernel/timers.c
    FreeRTOS-Kernel/event_groups.c

    # heap
    FreeRTOS-Kernel/portable/MemMang/heap_4.c

    # RP2040 port
    FreeRTOS-Kernel/portable/ThirdParty/GCC/RP2040/port.c
)

# =========================================
# Libraries
# =========================================
target_link_libraries(pico_rtos PRIVATE

    # Pico
    pico_stdlib

    # Hardware
    hardware_irq
    hardware_exception
    hardware_structs
    hardware_i2c
    hardware_spi

    # FatFs SPI
    FatFs_SPI
)

# =========================================
# USB / UART
# =========================================
pico_enable_stdio_usb(pico_rtos 1)
pico_enable_stdio_uart(pico_rtos 0)

# =========================================
# Outputs
# =========================================
pico_add_extra_outputs(pico_rtos)