#include "task_conditioning3_2.h"
#include "app_lab_3_2.h"
#include "srv_sns_temp/srv_sns_temp.h"
#include "srv_sig_cond/srv_sig_cond.h"
#include "dd_led/Led.h"
#include "dd_ntc/dd_ntc.h"
#include <Arduino_FreeRTOS.h>
#include <stdlib.h>

// NTC conditioning state - owned here
static float sNtcMedianBuf[MEDIAN_SIZE] = {0};
static int sNtcMedianHead = 0;
static int sNtcMedianCount = 0;
static float sNtcWma = 0.0f;
static bool sNtcWmaReady = false;

float gNtcSat = 0.0f;
float gNtcMedian = 0.0f;
float gNtcFiltered = 0.0f; 

void taskConditioning3_2(void *pvParameters) {
    (void) pvParameters;  // Unused parameter

    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = pdMS_TO_TICKS(TASK_COND_MS); // 20ms period

    if (TASK_COND_OFFSET_MS > 0) {
        vTaskDelay(pdMS_TO_TICKS(TASK_COND_OFFSET_MS));
    }

    while (1) {
        // Read raw NTC value for conditioning
        float ntcRaw = srvSnsTempGetNtcRaw();
        // Apply signal conditioning steps
        gNtcSat = srvSigCondSaturate(ntcRaw, SAT_MIN, SAT_MAX);
        gNtcMedian = srvSigCondMedian(gNtcSat, sNtcMedianBuf, &sNtcMedianHead, &sNtcMedianCount, MEDIAN_SIZE);
        gNtcFiltered = srvSigCondIIR(gNtcMedian, &sNtcWma, &sNtcWmaReady, 20);
        float ntcCelsius = ddNtcConvertToCelsius(gNtcFiltered);
        srvSnsTempSetNtcFiltered(ntcCelsius);

        // Process threshold + hysteresis + debounce for both sensors
        srvSnsTempProcess();

        // Update LEDs based on alert states
        AlertState alert1 = srvSnsTempGetAlert1();
        AlertState alert2 = srvSnsTempGetAlert2();

        if (alert1.alertActive) {
            ledTurnOn(LED_GREEN_PIN_LAB32);
        } else {
            ledTurnOff(LED_GREEN_PIN_LAB32);
        }

        if (alert2.alertActive) {
            ledTurnOn(LED_RED_PIN_LAB32);
        } else {
            ledTurnOff(LED_RED_PIN_LAB32);
        }

        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}
