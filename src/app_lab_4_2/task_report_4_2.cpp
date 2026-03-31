#include "task_report_4_2.h"
#include "app_lab_4_2.h"
#include "task_conditioning_4_2.h"
#include "dd_relay/dd_relay.h"
#include "semphr.h"
#include "srv_serial_stdio/Serial.h"
#include "srv_stdio_lcd/srv_stdio_lcd.h"
#include "task_input_4_2.h"

static const char* dirStr(MotorDirection d) {
    switch (d) {
        case MOTOR_FORWARD:  return "FWD";
        case MOTOR_BACKWARD: return "BWD";
        default:             return "STP";
    }
}

void taskReport42(void *pvParameters) {
    (void) pvParameters; // Unused parameter
    TickType_t lastWakeTime = xTaskGetTickCount();

    for (;;) {
        vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(TASK_REPORT42_MS)); 

        TaskConditioningState42 snap = {RELAY_OFF, RELAY_OFF, false, 0, 0, 0, 0, 0, 0, 0, false, MOTOR_STOP, false};
        if (xSemaphoreTake(s_stateMutex42, portMAX_DELAY) == pdTRUE) {
            snap = s_state42; // Copy the latest snapshot
            xSemaphoreGive(s_stateMutex42);
        }

        // LCD
        printf("\x1B");
        printf("RLY:%s DB:%d %s\n",
            snap.relayApplied == RELAY_ON ? "ON " : "OFF",
            snap.relayDebounce,
            snap.relayPending ? "PD" : "OK");
        printf("MTR:%3d %s SAT:%s\n",
            snap.motorApplied,
            dirStr(snap.motorDirection),
            snap.motorSaturated ? "YES" : "NO ");
 
        // --- Serial full report every 500ms ---
        printf("==============================\n");
        printf(" Lab 5 - Actuator Control\n");
        printf("------------------------------\n");
        printf(" [Relay]\n");
        printf("  Requested: %s\n", snap.relayRequested == RELAY_ON ? "ON"  : "OFF");
        printf("  Applied:   %s\n", snap.relayApplied   == RELAY_ON ? "ON"  : "OFF");
        printf("  Debounce:  %s (%d/%d)\n",
            snap.relayPending ? "PENDING" : "STABLE",
            snap.relayDebounce, RELAY_DEBOUNCE_COUNT);
        printf(" [Motor Pipeline]\n");
        printf("  Direction: %s\n", dirStr(snap.motorDirection));
        printf("  Running:   %s\n", snap.motorRunning ? "YES" : "NO");
        printf("  Raw:       %d\n",  snap.motorRaw);
        printf("  Saturated: %d %s\n", snap.motorSat, snap.motorSaturated ? "(!)" : "");
        printf("  Median:    %d\n",  snap.motorMedian);
        printf("  Weighted:  %d\n",  snap.motorWeighted);
        printf("  Ramped:    %d\n",  snap.motorRamped);
        printf("  Applied:   %d\n",  snap.motorApplied);
        TaskInput42 snapCmd = taskInputGetLatest42();
        if (snapCmd.lastError[0] != '\0') {
            printf("[ERROR]: %s\n", snapCmd.lastError);
        }
        printf("==============================\n");
    }
}