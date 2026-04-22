#ifndef TASK_ACQUISITION_5_2_H
#define TASK_ACQUISITION_5_2_H

#include <Arduino_FreeRTOS.h>

typedef struct {
    float temperature;  // Last valid temperature reading (°C)
    float humidity;     // Last valid humidity reading (%)
    bool  valid;        // false until first successful read
} TaskAcquisitionState52;

void taskAcquisitionInit52();
TaskAcquisitionState52 taskAcquisitionGetLatest52();
void taskAcquisition52(void *pvParameters);

#endif // TASK_ACQUISITION_5_2_H