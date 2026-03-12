#include "task_conditioning.h"
#include "app_lab_3_1.h"
#include "srv_sns_temp/srv_sns_temp.h"
#include "dd_led/Led.h"
#include <Arduino_FreeRTOS.h>

void taskConditioning(void *pvParameters) {
    (void) pvParameters;  // Unused parameter

    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = pdMS_TO_TICKS(TASK_COND_MS); // 20ms period

    if (TASK_COND_OFFSET_MS > 0) {
        vTaskDelay(pdMS_TO_TICKS(TASK_COND_OFFSET_MS));
    }

    while (1) {
        // Process threshold + hysteresis + debounce for both sensors
        srvSnsTempProcess();

        // Update LEDs based on alert states
        AlertState alert1 = srvSnsTempGetAlert1();
        AlertState alert2 = srvSnsTempGetAlert2();

        if (alert1.alertActive) {
            ledTurnOn(LED_GREEN_PIN_LAB31);
        } else {
            ledTurnOff(LED_GREEN_PIN_LAB31);
        }

        if (alert2.alertActive) {
            ledTurnOn(LED_RED_PIN_LAB31);
        } else {
            ledTurnOff(LED_RED_PIN_LAB31);
        }

        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}
