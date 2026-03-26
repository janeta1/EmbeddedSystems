#ifndef TASK_CONDITIONING_4_2_H
#define TASK_CONDITIONING_4_2_H

#include <Arduino_FreeRTOS.h>
#include <semphr.h>
#include "dd_relay/dd_relay.h"

typedef struct {
    // Relay
    RelayState relayRequested;
    RelayState relayApplied;
    bool       relayPending;
    int        relayDebounce;
    // Servo
    int  servoRaw;
    int  servoSat;
    int  servoMedian;
    int  servoWeighted;
    int  servoRamped;
    int  servoApplied;
    bool servoSaturated;
    // Command acknowledgement — set by taskInput, printed+cleared by taskReport
    char cmdAck[32];
} TaskConditioningState42;

extern TaskConditioningState42 s_state42;
extern SemaphoreHandle_t s_stateMutex42;

void taskConditioningInit42();
void taskConditioning42(void *pvParameters);

#endif // TASK_CONDITIONING_4_2_H