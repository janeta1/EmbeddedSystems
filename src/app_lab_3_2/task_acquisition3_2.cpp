#include "task_acquisition3_2.h"
#include "app_lab_3_2.h"
#include "srv_sns_temp/srv_sns_temp.h"
#include <Arduino_FreeRTOS.h>
#include <Arduino.h>

void taskAcquisition3_2(void *pvParameters) {
    (void) pvParameters;  // Unused parameter

    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = pdMS_TO_TICKS(TASK_ACQ_MS); // 20ms period

    if (TASK_ACQ_OFFSET_MS > 0) {
        vTaskDelay(pdMS_TO_TICKS(TASK_ACQ_OFFSET_MS));
    }

    while (1) {
        // Read sensors and update shared data in srv_sns_temp
        srvSnsTempAcquire();
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}