#include "task_conditioning_5_1.h"
#include "task_acquisition_5_1.h"
#include "task_input_5_1.h"
#include "app_lab_5_1.h"
#include "dd_relay/dd_relay.h"
#include <Arduino_FreeRTOS.h>
#include <semphr.h>

// Global snapshot, read by task_report
TaskConditioningState51 s_condState51 = {
    DEFAULT_SETPOINT,       // setPoint
    HYSTERESIS_DELTA,      // hysteresis
    0.0f,                   // temperature
    0.0f,                   // humidity
    DEFAULT_SETPOINT - HYSTERESIS_DELTA, // von
    DEFAULT_SETPOINT + HYSTERESIS_DELTA, // voff
    RELAY_OFF,              // relayRequested
    RELAY_OFF,              // relayApplied
    false,                  // relayPending
    0                       // relayDebounce
};
SemaphoreHandle_t s_condMutex51 = NULL;

// Hysteresis state — persists between cycles
static RelayState sControlOutput = RELAY_OFF;

void taskConditioningInit51() {
    s_condMutex51 = xSemaphoreCreateMutex();
}

void taskConditioning51(void *pvParameters) {
    (void) pvParameters; // Unused parameter

    TickType_t lastWakeTime = xTaskGetTickCount();

    for (;;) {
        // 1. Get latest sensor reading
        TaskAcquisitionState51 acq = taskAcquisitionGetLatest51();

        float sp   = taskInputGetLatest51().setPoint; // SetPoint set by user via serial
        float hys  = taskInputGetLatest51().hysteresis; // Hysteresis delta set by user via serial
        float von  = sp - hys;     // Lower threshold: turn relay ON below this
        float voff = sp + hys;     // Upper threshold: turn relay OFF above this
        float temp = acq.temperature;
        float humidity = acq.humidity;

        // 2. ON-OFF with hysteresis logic (formula 6.2 from lab manual)
        //    - if temp < Von  → relay ON
        //    - if temp > Voff → relay OFF
        //    - else           → keep previous state (sControlOutput unchanged)
        if (temp < von) {
            sControlOutput = RELAY_ON;
        } else if (temp > voff) {
            sControlOutput = RELAY_OFF;
        }
        // else: do nothing — maintain previous output

        // 3. Pass desired state to relay driver and step debounce
        ddRelaySetRequested(sControlOutput);
        ddRelayStep();

        // 4. Update shared snapshot for task_report
        if (xSemaphoreTake(s_condMutex51, portMAX_DELAY) == pdTRUE) {
            s_condState51.setPoint       = sp;
            s_condState51.hysteresis     = hys;
            s_condState51.temperature    = temp;
            s_condState51.humidity       = humidity;
            s_condState51.von            = von;
            s_condState51.voff           = voff;
            s_condState51.relayRequested = ddRelayGetRequested();
            s_condState51.relayApplied   = ddRelayGetApplied();
            s_condState51.relayPending   = ddRelayGetPending() != ddRelayGetApplied();
            s_condState51.relayDebounce  = ddRelayGetDebounceCounter();
            xSemaphoreGive(s_condMutex51);
        }

        vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(TASK_COND51_MS));
    }
}