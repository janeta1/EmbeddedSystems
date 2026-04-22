#include "task_report_5_1.h"
#include "task_conditioning_5_1.h"
#include "task_input_5_1.h"
#include "app_lab_5_1.h"
#include "dd_relay/dd_relay.h"
#include <semphr.h>
#include "srv_serial_stdio/Serial.h"
#include "srv_stdio_lcd/srv_stdio_lcd.h"

void taskReport51(void *pvParameters)
{
    (void)pvParameters; // Unused parameter

    TickType_t lastWakeTime = xTaskGetTickCount();

    for (;;)
    {
        vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(TASK_REPORT51_MS));

        // Take a snapshot of the conditioning state
        TaskConditioningState51 snap;
        if (xSemaphoreTake(s_condMutex51, portMAX_DELAY) == pdTRUE)
        {
            snap = s_condState51;
            xSemaphoreGive(s_condMutex51);
        }

        // Serial Plotter (Arduino format: Label:Value,Label:Value)
        printf("SetPoint:%.2f Temp:%.2f Humidity:%.1f Relay:%d Fan:%d\n",
               (double)snap.setPoint,
               (double)snap.temperature,
               (double)snap.humidity,
               snap.relayApplied == RELAY_ON ? 1 : 0,
               snap.fanRunning ? 1 : 0);

        // LCD
        // Determine mode
        const char *mode;
        if (snap.temperature < snap.von)
        {
            mode = "HEAT";
        }
        else if (snap.temperature > snap.voff)
        {
            mode = "FAN";
        }
        else
        {
            mode = "IDLE";
        }

        printf("\x1B");
        printf("SP:%.1f  T:%.1f\n", 
               (double)snap.setPoint,
               (double)snap.temperature);
        printf("HYS:%.1f  [%s]\n",
               (double)snap.hysteresis,
               mode);

        // Serial only
        printf("==============================\n");
        printf(" Lab 5.1 - ON-OFF Hysteresis Control\n");
        printf("------------------------------\n");
        printf(" [Control]\n");
        printf("  SetPoint:  %.2f C\n", (double)snap.setPoint);
        printf("  Hysteresis: %.2f C\n", (double)snap.hysteresis);
        printf("  Von:       %.2f C\n", (double)snap.von);
        printf("  Voff:      %.2f C\n", (double)snap.voff);
        printf("  Temp:      %.2f C\n", (double)snap.temperature);
        printf("  Humidity:  %.1f %%\n", (double)snap.humidity);
        printf(" [Relay]\n");
        printf("  Requested: %s\n", snap.relayRequested == RELAY_ON ? "ON" : "OFF");
        printf("  Applied:   %s\n", snap.relayApplied == RELAY_ON ? "ON" : "OFF");
        printf("  Debounce:  %s (%d/%d)\n",
               snap.relayPending ? "PENDING" : "STABLE",
               snap.relayDebounce, RELAY_DEBOUNCE_COUNT);
        printf("[Fan]\n");
        printf("  Running: %s\n", snap.fanRunning ? "ON" : "OFF");
        printf("==============================\n");
        TaskInput51 snapCmd = taskInputGetLatest51();
        if (snapCmd.lastError[0] != '\0')
        {
            printf("[ERROR]: %s\n", snapCmd.lastError);
        }
        printf("==============================\n");
    }
}