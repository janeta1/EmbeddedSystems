#include "task_conditioning_4_2.h"
#include "task_input_4_2.h"
#include "app_lab_4_2.h"
#include "dd_relay/dd_relay.h" 
#include "dd_servo/dd_servo.h"
#include "dd_led/Led.h"
#include "srv_sig_cond/srv_sig_cond.h"
#include <Arduino_FreeRTOS.h>
#include <semphr.h>

// Global snapshot, read by task_report
TaskConditioningState42 s_state42 = {RELAY_OFF, RELAY_OFF, false, 0, 0, 0, 0, 0, 0, 0, false, ""};
SemaphoreHandle_t s_stateMutex42 = NULL;

static float sMedianBuf[MEDIAN_SIZE];
static int sMedianHead  = 0;
static int sMedianCount = 0;

static float sIIR      = 0.0f;
static bool  sIIRReady = false;

static float sRampedAngle = 0.0f;

void taskConditioningInit42() {
    s_stateMutex42 = xSemaphoreCreateMutex();
}

void taskConditioning42(void *pvParameters) {
    (void) pvParameters; // Unused parameter

    TickType_t lastWakeTime = xTaskGetTickCount();

    for (;;) {
        // 1. Get latest user command
        TaskInput input = taskInputGetLatest42();

        // 2. Relay: pass command to driver, let it debounce
        ddRelaySetRequested(input.relayRequested ? RELAY_ON : RELAY_OFF);
        ddRelayStep(); // Call the step function to update relay state

        // 3. Servo: apply saturation and pass command to driver
        int rawAngle = input.servoAngle;

        // Saturate to physical limits
        float saturatedVal = srvSigCondSaturate((float) rawAngle, (float) SERVO_ANGLE_MIN, (float) SERVO_ANGLE_MAX);
        bool saturated = (saturatedVal != (float) rawAngle);

        // Median filter
        float medianVal = srvSigCondMedian(saturatedVal, sMedianBuf, &sMedianHead, &sMedianCount, MEDIAN_SIZE);

        // IIR filter
        float weightedVal = srvSigCondIIR(medianVal, &sIIR, &sIIRReady, IIR_ALPHA);

        // Ramp (soft start/stop)
        sRampedAngle = srvSigCondRamp(sRampedAngle, weightedVal, RAMP_STEP);

        // Apply final value to servo
        ddServoSetAngle((int) sRampedAngle);

        // 5. Update snapshot for reporting
        if (xSemaphoreTake(s_stateMutex42, portMAX_DELAY) == pdTRUE) {
            s_state42.relayRequested = ddRelayGetRequested();
            s_state42.relayPending = ddRelayGetPending() != ddRelayGetApplied();
            s_state42.relayApplied = ddRelayGetApplied();
            s_state42.relayDebounce = ddRelayGetDebounceCounter();
            s_state42.servoRaw = rawAngle;
            s_state42.servoSat = (int) saturatedVal;
            s_state42.servoMedian = (int) medianVal;
            s_state42.servoWeighted = (int) weightedVal;
            s_state42.servoRamped = (int) sRampedAngle;
            s_state42.servoApplied = ddServoGetAngle();
            s_state42.servoSaturated = saturated;
            xSemaphoreGive(s_stateMutex42);
        }

        vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(TASK_COND42_MS)); // Run every 100 ms
    }
}