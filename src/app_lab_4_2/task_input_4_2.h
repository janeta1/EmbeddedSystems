#ifndef TASK_INPUT_4_2_H
#define TASK_INPUT_4_2_H

#include <Arduino_FreeRTOS.h>
#include "dd_l298/dd_l298.h"

typedef struct {
    bool relayRequested; // true = ON, false = OFF
    int motorSpeed;     // 0-255
    MotorDirection motorDirection; // MOTOR_STOP, MOTOR_FORWARD, MOTOR_BACKWARD
    bool motorDirectionSet; // true if direction was set by user, false if not
    bool motorStop;
    bool motorStart;
    char lastError[64];   // For storing error messages (if needed)
} TaskInput42;

void taskInputInit42();
TaskInput42 taskInputGetLatest42();
void taskInput42(void *pvParameters);

#endif // TASK_INPUT_4_2_H