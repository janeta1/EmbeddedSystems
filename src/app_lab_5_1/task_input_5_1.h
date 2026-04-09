#ifndef TASK_INPUT_5_1_H
#define TASK_INPUT_5_1_H

#include <Arduino_FreeRTOS.h>

typedef struct {
    float setPoint; 
    float hysteresis;
    char  lastError[64];
} TaskInput51;

void taskInputInit51();
TaskInput51 taskInputGetLatest51();
void taskInput51(void *pvParameters);

#endif // TASK_INPUT_5_1_H