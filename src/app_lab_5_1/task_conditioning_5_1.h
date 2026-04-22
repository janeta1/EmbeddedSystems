#ifndef TASK_CONDITIONING_5_1_H
#define TASK_CONDITIONING_5_1_H

#include <Arduino_FreeRTOS.h>
#include <semphr.h>
#include "dd_relay/dd_relay.h"

typedef struct {
    float      setPoint;       
    float      hysteresis;     
    float      temperature;    // Last measured temperature (°C)
    float      humidity;
    float      von;            // Lower hysteresis threshold
    float      voff;           // Upper hysteresis threshold
    RelayState relayRequested; 
    RelayState relayApplied;  
    bool       relayPending;   
    int        relayDebounce; 
    bool       fanRunning; 
} TaskConditioningState51;

extern TaskConditioningState51 s_condState51;
extern SemaphoreHandle_t       s_condMutex51;

void taskConditioningInit51();
void taskConditioning51(void *pvParameters);

#endif // TASK_CONDITIONING_5_1_H