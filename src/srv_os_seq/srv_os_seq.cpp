#include "srv_os_seq.h"
#include "timer-api.h"
#include "app_lab_2_1/app_lab_2_1.h"
#include <Arduino.h>

static Task_t tasks[] = {
    { task1BtnDetLoop, TASK1_PERIOD_MS,    0, 0 },
    { task2BtnMonLoop, TASK2_PERIOD_MS,   20, 0 },
    { task3RepLoop,    TASK3_PERIOD_MS,  100, 0 }
};

#define NUM_TASKS (sizeof(tasks)/sizeof(Task_t))

void srvOsSeqInit(void)
{
    for (size_t i = 0; i < NUM_TASKS; i++) {
        tasks[i].counter = tasks[i].offset;
    }

    timer_init_ISR_1KHz(TIMER_DEFAULT);
}

void timer_handle_interrupts(int timer) {
    for (size_t i = 0; i < NUM_TASKS; i++) {
        if (tasks[i].counter > 0) {
            tasks[i].counter--;
        }
        if (tasks[i].counter == 0) {
            tasks[i].counter = tasks[i].period;
            tasks[i].taskFunc();
            break; 
        }
    }
}