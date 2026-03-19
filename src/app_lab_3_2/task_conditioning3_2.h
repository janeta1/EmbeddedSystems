#ifndef TASK_CONDITIONING_H
#define TASK_CONDITIONING_H
#include <Arduino_FreeRTOS.h>

#define TASK_COND_OFFSET_MS 5

extern float gNtcSat;
extern float gNtcMedian;
extern float gNtcFiltered;

// Function prototype
void taskConditioning3_2(void *pvParameters);

#endif // TASK_CONDITIONING_H