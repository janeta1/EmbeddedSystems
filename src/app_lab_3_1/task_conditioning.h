#ifndef TASK_CONDITIONING_H
#define TASK_CONDITIONING_H
#include <Arduino_FreeRTOS.h>

#define TASK_COND_OFFSET_MS 5

// Function prototype
void taskConditioning(void *pvParameters);

#endif // TASK_CONDITIONING_H