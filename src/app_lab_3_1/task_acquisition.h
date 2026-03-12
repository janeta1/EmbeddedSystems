#ifndef TASK_ACQUISITION_H
#define TASK_ACQUISITION_H

#include <Arduino.h>
#include <Arduino_FreeRTOS.h>

#define TASK_ACQ_OFFSET_MS 0
void taskAcquisition(void *pvParameters);

#endif // TASK_ACQUISITION_H