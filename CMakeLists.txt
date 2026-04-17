cmake_minimum_required(VERSION 3.13)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(pico_rtos_project C CXX ASM)

pico_sdk_init()

add_executable(pico_rtos
    main.c
)

target_include_directories(pico_rtos PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/drivers
    ${CMAKE_CURRENT_LIST_DIR}/input
    ${CMAKE_CURRENT_LIST_DIR}/system
    ${CMAKE_CURRENT_LIST_DIR}/tasks
    ${CMAKE_CURRENT_LIST_DIR}/ui
    ${CMAKE_CURRENT_LIST_DIR}/FreeRTOS-Kernel/include
    ${CMAKE_CURRENT_LIST_DIR}/FreeRTOS-Kernel/portable/ThirdParty/GCC/RP2040
    ${CMAKE_CURRENT_LIST_DIR}/FreeRTOS-Kernel/portable/ThirdParty/GCC/RP2040/include
)

target_sources(pico_rtos PRIVATE
    main.c 
    drivers/ssd1306.c
    drivers/font5x7.c
    FreeRTOS-Kernel/tasks.c
    FreeRTOS-Kernel/queue.c
    FreeRTOS-Kernel/list.c
    FreeRTOS-Kernel/timers.c
    FreeRTOS-Kernel/event_groups.c
    FreeRTOS-Kernel/portable/MemMang/heap_4.c
    FreeRTOS-Kernel/portable/ThirdParty/GCC/RP2040/port.c
)

target_link_libraries(pico_rtos PRIVATE pico_stdlib hardware_irq hardware_exception hardware_structs hardware_i2c)

pico_add_extra_outputs(pico_rtos)
pico_enable_stdio_usb(pico_rtos 1)
pico_enable_stdio_uart(pico_rtos 0)
