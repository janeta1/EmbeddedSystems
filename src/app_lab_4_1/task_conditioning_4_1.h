#ifndef TASK_CONDITIONING_4_1_H
#define TASK_CONDITIONING_4_1_H

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
    int  servoApplied;
    bool servoSaturated;
    // Command acknowledgement — set by taskInput, printed+cleared by taskReport
    char cmdAck[32];
} TaskConditioningState;

extern TaskConditioningState s_state;
extern SemaphoreHandle_t s_stateMutex;

void taskConditioningInit41();
void taskConditioning41(void *pvParameters);

#endif // TASK_CONDITIONING_4_1_H