#ifndef TASK_INPUT_4_2_H
#define TASK_INPUT_4_2_H

#include <Arduino_FreeRTOS.h>

typedef struct {
    bool relayRequested; // true = ON, false = OFF
    int servoAngle;      // 0 to 180 degrees
    char lastError[64];   // For storing error messages (if needed)
} TaskInput;

void taskInputInit42();
TaskInput taskInputGetLatest42();
void taskInput42(void *pvParameters);

#endif // TASK_INPUT_4_2_H