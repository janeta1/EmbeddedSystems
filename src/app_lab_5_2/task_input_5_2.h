#ifndef TASK_INPUT_5_2_H
#define TASK_INPUT_5_2_H

#include <Arduino_FreeRTOS.h>

typedef struct {
    float setPoint; 
    float kp;
    float ki;
    float kd;
    char  lastError[64];
} TaskInput52;

void taskInputInit52();
TaskInput52 taskInputGetLatest52();
void taskInput52(void *pvParameters);

#endif // TASK_INPUT_5_2_H