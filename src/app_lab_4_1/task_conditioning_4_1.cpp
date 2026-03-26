#include "task_conditioning_4_1.h"
#include "task_input_4_1.h"
#include "app_lab_4_1.h"
#include "dd_relay/dd_relay.h" 
#include "dd_servo/dd_servo.h"
#include "dd_led/Led.h"
#include <Arduino_FreeRTOS.h>
#include <semphr.h>

// Global snapshot, read by task_report
TaskConditioningState s_state = {RELAY_OFF, RELAY_OFF, false, 0, 0, 0, false, ""};
SemaphoreHandle_t s_stateMutex = NULL;

void taskConditioningInit41() {
    s_stateMutex = xSemaphoreCreateMutex();
}

void taskConditioning41(void *pvParameters) {
    (void) pvParameters; // Unused parameter

    TickType_t lastWakeTime = xTaskGetTickCount();

    for (;;) {
        // 1. Get latest user command
        TaskInput input = taskInputGetLatest41();

        // 2. Relay: pass command to driver, let it debounce
        ddRelaySetRequested(input.relayRequested ? RELAY_ON : RELAY_OFF);
        ddRelayStep(); // Call the step function to update relay state

        // 3. Servo: apply saturation and pass command to driver
        int rawAngle = input.servoAngle;
        bool saturated = (rawAngle < SERVO_ANGLE_MIN || rawAngle > SERVO_ANGLE_MAX);
        ddServoSetAngle(rawAngle); // ddServoSetAngle will handle saturation internally

        // 5. Update snapshot for reporting
        if (xSemaphoreTake(s_stateMutex, portMAX_DELAY) == pdTRUE) {
            s_state.relayRequested = ddRelayGetRequested();
            s_state.relayPending = ddRelayGetPending() != ddRelayGetApplied();
            s_state.relayApplied = ddRelayGetApplied();
            s_state.relayDebounce = ddRelayGetDebounceCounter();
            s_state.servoRaw = rawAngle;
            s_state.servoApplied = ddServoGetAngle();
            s_state.servoSaturated = saturated;
            xSemaphoreGive(s_stateMutex);
        }

        vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(TASK_COND41_MS)); // Run every 100 ms
    }
}