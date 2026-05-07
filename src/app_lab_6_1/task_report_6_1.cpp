#include "task_report_6_1.h"
#include "app_lab_6_1.h"
#include "srv_fsm/srv_fsm.h"
#include "srv_serial_stdio/Serial.h"
#include "srv_stdio_lcd/srv_stdio_lcd.h"

void taskReport61(void *pvParameters) {
    (void) pvParameters;

    TickType_t lastWakeTime = xTaskGetTickCount();

    for (;;) {
        vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(TASK_REPORT_61_MS));

        // Snapshot state atomically
        int state = srvFsmGetState(&g_fsm61);
        const char *stateName = (state == FSM_LED_ON) ? "LED_ON" : "LED_OFF";

        // LCD output (2 lines, clears first)
        printf("\x1B");
        printf("FSM Button-LED\n");
        printf("State: %s\n", stateName);

        // Serial detailed output
        printf("==============================\n");
        printf(" Lab 6.1 - Button-LED FSM\n");
        printf("------------------------------\n");
        printf(" State ID : %d\n", state);
        printf(" State    : %s\n", stateName);
        printf(" LED      : %s\n", (state == FSM_LED_ON) ? "ON" : "OFF");
        printf("==============================\n");
    }
}