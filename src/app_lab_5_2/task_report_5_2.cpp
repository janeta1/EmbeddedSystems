#include "task_report_5_2.h"
#include "task_conditioning_5_2.h"
#include "task_input_5_2.h"
#include "app_lab_5_2.h"
#include "dd_relay/dd_relay.h"
#include <semphr.h>
#include "srv_serial_stdio/Serial.h"
#include "srv_stdio_lcd/srv_stdio_lcd.h"

void taskReport52(void *pvParameters) {
    (void) pvParameters; // Unused parameter

    TickType_t lastWakeTime = xTaskGetTickCount();

    for (;;) {
        vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(TASK_REPORT52_MS));

        // Take a snapshot of the conditioning state
        TaskConditioningState52 snap;
        if (xSemaphoreTake(s_condMutex52, portMAX_DELAY) == pdTRUE) {
            snap = s_condState52;
            xSemaphoreGive(s_condMutex52);
        }

        // Serial Plotter (Arduino format: Label:Value,Label:Value)
        printf("SetPoint:%.2f Temp:%.2f Humidity:%.1f Output:%.2f\n",
            (double) snap.setPoint,
            (double) snap.temperature,
            (double) snap.humidity,
            (double) snap.pidOutput);

        // LCD
        printf("\x1B");
        printf("SP:%.1f  T:%.1f\n",
            (double) snap.setPoint,
            (double) snap.temperature);
        printf("Out:%.1f  [%s]\n",
            (double) snap.pidOutput,
            snap.relayApplied == RELAY_ON ? "HEAT" : (snap.fanRunning ? "COOL" : "IDLE"));

        // Serial only
        printf("==============================\n");
        printf(" Lab 5.2 - PID Temperature Control\n");
        printf("------------------------------\n");
        printf(" [Sensor]\n");
        printf("  Temp:      %.2f C\n", (double) snap.temperature);
        printf("  Humidity:  %.1f %%\n", (double) snap.humidity);
        printf(" [PID]\n");
        printf("  SetPoint:  %.2f C\n", (double) snap.setPoint);
        printf("  Error:    %.2f\n",    (double) snap.error);
        printf("  Kp=%.2f  Ki=%.2f  Kd=%.2f\n",
            (double) snap.kp, (double) snap.ki, (double) snap.kd);
        printf("  P=%.2f  I=%.2f  D=%.2f\n",
            (double) snap.pTerm, (double) snap.iTerm, (double) snap.dTerm);
        printf("  Output:   %.2f\n",    (double) snap.pidOutput);
        printf(" [Relay]\n");
        printf("  Requested: %s\n", snap.relayRequested == RELAY_ON ? "ON"  : "OFF");
        printf("  Applied:   %s\n", snap.relayApplied   == RELAY_ON ? "ON"  : "OFF");
        printf("  Debounce:  %s (%d/%d)\n",
            snap.relayPending ? "PENDING" : "STABLE",
            snap.relayDebounce, RELAY_DEBOUNCE_COUNT);
        printf("[Fan]\n");
        printf("  Running: %s %d\n", snap.fanRunning ? "ON " : "OFF ", snap.fanSpeed);
        printf("==============================\n");
        TaskInput52 snapCmd = taskInputGetLatest52();
        if (snapCmd.lastError[0] != '\0') {
            printf("[ERROR]: %s\n", snapCmd.lastError);
        }
        printf("==============================\n");
    }
}