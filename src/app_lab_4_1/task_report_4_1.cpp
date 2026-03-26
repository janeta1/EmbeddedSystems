#include "task_report_4_1.h"
#include "app_lab_4_1.h"
#include "task_conditioning_4_1.h"
#include "dd_relay/dd_relay.h"
#include "semphr.h"
#include "srv_serial_stdio/Serial.h"
#include "srv_stdio_lcd/srv_stdio_lcd.h"
#include "task_input_4_1.h"

void taskReport41(void *pvParameters) {
    (void) pvParameters; // Unused parameter
    TickType_t lastWakeTime = xTaskGetTickCount();

    for (;;) {
        vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(TASK_REPORT41_MS)); 

        TaskConditioningState snap = { RELAY_OFF, RELAY_OFF, false, 0, 0, 0, false };
        if (xSemaphoreTake(s_stateMutex, portMAX_DELAY) == pdTRUE) {
            snap = s_state; // Copy the latest snapshot
            xSemaphoreGive(s_stateMutex);
        }

        // LCD
        printf("\x1B");
        printf("RLY:%s DB:%d %s\n",
            snap.relayApplied == RELAY_ON ? "ON " : "OFF",
            snap.relayDebounce,
            snap.relayPending ? "PD" : "OK");
        printf("SRV:%3d SAT:%s\n",
            snap.servoApplied,
            snap.servoSaturated ? "YES" : "NO ");
 
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
        printf(" [Servo]\n");
        printf("  Raw:       %d deg\n", snap.servoRaw);
        printf("  Applied:   %d deg\n", snap.servoApplied);
        printf("  Saturated: %s\n",     snap.servoSaturated ? "YES (!)" : "no");
        TaskInput snapCmd = taskInputGetLatest41();
        if (snapCmd.lastError[0] != '\0') {
            printf("[ERROR]: %s\n", snapCmd.lastError);
        }
        printf("==============================\n");
    }
}