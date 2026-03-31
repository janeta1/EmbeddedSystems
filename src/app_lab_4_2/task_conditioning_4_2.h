#ifndef TASK_CONDITIONING_4_2_H
#define TASK_CONDITIONING_4_2_H

#include <Arduino_FreeRTOS.h>
#include <semphr.h>
#include "dd_relay/dd_relay.h"
#include "dd_l298/dd_l298.h"

typedef struct {
    // Relay
    RelayState relayRequested;
    RelayState relayApplied;
    bool       relayPending;
    int        relayDebounce;
    // Motor
    int  motorRaw;
    int  motorSat;
    int  motorMedian;
    int  motorWeighted;
    int  motorRamped;
    int  motorApplied;
    bool motorSaturated;
    MotorDirection motorDirection;
    bool motorRunning;
} TaskConditioningState42;

extern TaskConditioningState42 s_state42;
extern SemaphoreHandle_t s_stateMutex42;

void taskConditioningInit42();
void taskConditioning42(void *pvParameters);

#endif // TASK_CONDITIONING_4_2_H