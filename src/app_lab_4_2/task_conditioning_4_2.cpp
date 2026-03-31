#include "task_conditioning_4_2.h"
#include "task_input_4_2.h"
#include "app_lab_4_2.h"
#include "dd_relay/dd_relay.h" 
#include "dd_l298/dd_l298.h"
#include "dd_led/Led.h"
#include "srv_sig_cond/srv_sig_cond.h"
#include <Arduino_FreeRTOS.h>
#include <semphr.h>

// Global snapshot, read by task_report
TaskConditioningState42 s_state42 = {RELAY_OFF, RELAY_OFF, false, 0, 0, 0, 0, 0, 0, 0, false, MOTOR_STOP, false};
SemaphoreHandle_t s_stateMutex42 = NULL;

static float sMedianBuf[MEDIAN_SIZE];
static int sMedianHead  = 0;
static int sMedianCount = 0;

static float sIIR      = 0.0f;
static bool  sIIRReady = false;

static float sRampedSpeed = 0.0f;

void taskConditioningInit42() {
    s_stateMutex42 = xSemaphoreCreateMutex();
}

void taskConditioning42(void *pvParameters) {
    (void) pvParameters; // Unused parameter

    TickType_t lastWakeTime = xTaskGetTickCount();

    for (;;) {
        // 1. Get latest user command
        TaskInput42 input = taskInputGetLatest42();

        // 2. Relay: pass command to driver, let it debounce
        ddRelaySetRequested(input.relayRequested ? RELAY_ON : RELAY_OFF);
        ddRelayStep(); // Call the step function to update relay state
        int rawSpeed = input.motorSpeed; // default: use whatever user last set

        if (input.motorStop) {
            ddL298Stop();
            ddL298SetSpeed(0);
            rawSpeed = 0;
        } else if (input.motorStart) {
            ddL298Start();
        } else if (input.motorDirectionSet) {
            ddL298SetDirection(input.motorDirection);
        }
        // 3. Motor: apply saturation and pass command to driver
        // int rawSpeed = input.motorSpeed;

        // Saturate to physical limits
        float saturatedVal = srvSigCondSaturate((float) rawSpeed, (float) L298_SPEED_MIN, (float) L298_SPEED_MAX);
        bool saturated = (saturatedVal != (float) rawSpeed);

        // Median filter
        float medianVal = srvSigCondMedian(saturatedVal, sMedianBuf, &sMedianHead, &sMedianCount, MEDIAN_SIZE);

        // IIR filter
        float weightedVal = srvSigCondIIR(medianVal, &sIIR, &sIIRReady, IIR_ALPHA);

        // Ramp (soft start/stop)
        sRampedSpeed = srvSigCondRamp(sRampedSpeed, weightedVal, RAMP_STEP);

        // Only apply to motor if it's running
        if (ddL298IsRunning()) {
            ddL298SetSpeed((int) sRampedSpeed);
        }

        // 5. Update snapshot for reporting
        if (xSemaphoreTake(s_stateMutex42, portMAX_DELAY) == pdTRUE) {
            s_state42.relayRequested = ddRelayGetRequested();
            s_state42.relayPending = ddRelayGetPending() != ddRelayGetApplied();
            s_state42.relayApplied = ddRelayGetApplied();
            s_state42.relayDebounce = ddRelayGetDebounceCounter();
            s_state42.motorRaw = rawSpeed;
            s_state42.motorSat = (int) saturatedVal;
            s_state42.motorMedian = (int) medianVal;
            s_state42.motorWeighted = (int) weightedVal;
            s_state42.motorRamped = (int) sRampedSpeed;
            s_state42.motorApplied = ddL298GetSpeed();
            s_state42.motorSaturated = saturated;
            s_state42.motorDirection = ddL298GetDirection();
            s_state42.motorRunning = ddL298IsRunning();
            xSemaphoreGive(s_stateMutex42);
        }

        vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(TASK_COND42_MS)); // Run every 100 ms
    }
}