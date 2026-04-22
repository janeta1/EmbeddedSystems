#ifndef TASK_CONDITIONING_5_2_H
#define TASK_CONDITIONING_5_2_H

#include <Arduino_FreeRTOS.h>
#include <semphr.h>
#include "dd_relay/dd_relay.h"

typedef struct {
    float      setPoint;       
    float      kp;
    float      ki;
    float      kd;     
    float      temperature;    // Last measured temperature (°C)
    float      humidity;
    float      error;       
    float      pTerm;
    float      iTerm;
    float      dTerm;
    float      pidOutput;
    RelayState relayRequested; 
    RelayState relayApplied;  
    bool       relayPending;   
    int        relayDebounce; 
    int        fanSpeed;
    bool       fanRunning; 
} TaskConditioningState52;

extern TaskConditioningState52 s_condState52;
extern SemaphoreHandle_t       s_condMutex52;

void taskConditioningInit52();
void taskConditioning52(void *pvParameters);

#endif // TASK_CONDITIONING_5_2_H