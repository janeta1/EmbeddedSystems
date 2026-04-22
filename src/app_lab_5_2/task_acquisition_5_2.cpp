#include "task_acquisition_5_2.h"
#include "app_lab_5_2.h"
#include "dd_dht/dd_dht.h"
#include <Arduino_FreeRTOS.h>
#include <semphr.h>

static TaskAcquisitionState52 s_acqState = {0.0f, false};
static SemaphoreHandle_t s_acqMutex = NULL;

void taskAcquisitionInit52() {
    s_acqMutex = xSemaphoreCreateMutex();
}

TaskAcquisitionState52 taskAcquisitionGetLatest52() {
    TaskAcquisitionState52 copy = {0.0f, false};
    if (xSemaphoreTake(s_acqMutex, portMAX_DELAY) == pdTRUE) {
        copy = s_acqState;
        xSemaphoreGive(s_acqMutex);
    }
    return copy;
}

void taskAcquisition52(void *pvParameters) {
    (void) pvParameters; // Unused parameter

    TickType_t lastWakeTime = xTaskGetTickCount();

    for (;;) {
        // 1. Trigger DHT driver to read (throttled internally every 2000ms)
        ddDhtLoop();

        // 2. Fetch latest valid reading
        float t = ddDhtGetTemperature();
        float h = ddDhtGetHumidity();

        // 3. Update shared state
        if (xSemaphoreTake(s_acqMutex, portMAX_DELAY) == pdTRUE) {
            s_acqState.temperature = t;
            s_acqState.humidity    = h;
            s_acqState.valid       = true;
            xSemaphoreGive(s_acqMutex);
        }

        vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(TASK_ACQ52_MS));
    }
}