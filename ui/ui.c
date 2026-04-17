#include <stdio.h>
#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "events.h"
#include "queue.h"
#include "ssd1306.h"
#include "task_mngr.h"

extern QueueHandle_t inputQueue;

typedef enum {
    SCREEN_IDLE,
    SCREEN_MENU,
    SCREEN_TASKS,
    SCREEN_ADD_TASK
} ScreenState;

const char *menuItems[] = {
    "Tasks",
    "Add Task",
    "Timer",
    "Settings"
};

int taskIndex = 0;
int menuIndex = 0;
int menuSize = 3;

char newTask[32] = "AAAAAAAAAA";
int cursorPos = 0;

ScreenState currentScreen = SCREEN_IDLE;

void ui_task(void *params) {

    InputEvent event;
    int phase = 0;

    while (1) {

        if (xQueueReceive(inputQueue, &event, 0)) {

            // GLOBAL BACK
            if (event == EVENT_LEFT) {
                if (currentScreen == SCREEN_TASKS) currentScreen = SCREEN_MENU;
                else if (currentScreen == SCREEN_MENU) currentScreen = SCREEN_IDLE;
                else if (currentScreen == SCREEN_ADD_TASK) currentScreen = SCREEN_MENU;
                continue;
            }

            if (currentScreen == SCREEN_IDLE) {
                if (event == EVENT_SELECT) currentScreen = SCREEN_MENU;
            }

            else if (currentScreen == SCREEN_MENU) {

                if (event == EVENT_UP) {
                    menuIndex = (menuIndex - 1 + menuSize) % menuSize;
                }

                if (event == EVENT_DOWN) {
                    menuIndex = (menuIndex + 1) % menuSize;
                }

                if (event == EVENT_SELECT) {
                    if (menuIndex == 0) currentScreen = SCREEN_TASKS;
                    else if (menuIndex == 1) currentScreen = SCREEN_ADD_TASK;
                }
            }

            else if (currentScreen == SCREEN_TASKS) {

                int count = get_task_count();

                if (event == EVENT_UP) {
                    taskIndex = (taskIndex - 1 + count) % count;
                }

                if (event == EVENT_DOWN) {
                    taskIndex = (taskIndex + 1) % count;
                }

                if (event == EVENT_SELECT) {
                    toggle_task(taskIndex);
                }
            }

            else if (currentScreen == SCREEN_ADD_TASK) {

                if (event == EVENT_UP) {
                    newTask[cursorPos]++;
                    if (newTask[cursorPos] > 'Z') newTask[cursorPos] = 'A';
                }

                if (event == EVENT_DOWN) {
                    newTask[cursorPos]--;
                    if (newTask[cursorPos] < 'A') newTask[cursorPos] = 'Z';
                }

                if (event == EVENT_RIGHT) {
                    cursorPos = (cursorPos + 1) % 10;
                }

                if (event == EVENT_SELECT) {
                    add_task(newTask);
                    currentScreen = SCREEN_TASKS;
                }
            }
        }

        ssd1306_clear();

        if (currentScreen == SCREEN_IDLE) {
            ssd1306_text(0, 0, "TuiTui");
            ssd1306_text(0, 16, "RTOS Desk Assistant");
            draw_fire(30, 55, 50, 20, phase);
        }

        else if (currentScreen == SCREEN_MENU) {
            ssd1306_text(0, 0, "Menu");

            for (int i = 0; i < menuSize; i++) {
                int y = 16 + i * 8;

                if (i == menuIndex) {
                    ssd1306_text(0, y, ">");
                }

                ssd1306_text(10, y, menuItems[i]);
            }
        }

        else if (currentScreen == SCREEN_TASKS) {

            Task *tasks = get_tasks();
            int count = get_task_count();

            ssd1306_text(0, 0, "Tasks");

            for (int i = 0; i < count; i++) {

                int y = 16 + i * 8;

                if (i == taskIndex) {
                    ssd1306_text(0, y, ">");
                }

                if (tasks[i].completed)
                    ssd1306_text(10, y, "[x]");
                else
                    ssd1306_text(10, y, "[ ]");

                ssd1306_text(30, y, tasks[i].title);
            }
        }

        else if (currentScreen == SCREEN_ADD_TASK) {
            ssd1306_text(0, 0, "Add Task:");
            ssd1306_text(0, 16, newTask);

            // cursor
            ssd1306_pixel(cursorPos * 6, 30);
        }

        ssd1306_show();

        phase++;
        vTaskDelay(pdMS_TO_TICKS(80));
    }
}