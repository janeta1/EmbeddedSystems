#ifndef TASK_ACQUISITION_5_1_H
#define TASK_ACQUISITION_5_1_H

#include <Arduino_FreeRTOS.h>

typedef struct {
    float temperature;  // Last valid temperature reading (°C)
    float humidity;     // Last valid humidity reading (%)
    bool  valid;        // false until first successful read
} TaskAcquisitionState51;

void taskAcquisitionInit51();
TaskAcquisitionState51 taskAcquisitionGetLatest51();
void taskAcquisition51(void *pvParameters);

#endif // TASK_ACQUISITION_5_1_H