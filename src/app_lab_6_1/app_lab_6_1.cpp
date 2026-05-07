#include "app_lab_6_1.h"
#include "task_fsm_6_1.h"
#include "task_report_6_1.h"
#include "srv_fsm/srv_fsm.h"
#include "srv_serial_stdio/Serial.h"
#include "srv_stdio_lcd/srv_stdio_lcd.h"
#include <Arduino_FreeRTOS.h>

// Shared FSM instance — tasks access this via g_fsm61
SrvFsm g_fsm61;

void appLab61Setup() {
    // Init output interfaces
    serialInit(9600);
    srvStdioLCDSetup();

    // Init FSM engine with Button-LED table, start in LED_OFF
    srvFsmInit(&g_fsm61, buttonLedFsmTable, FSM_TABLE_SIZE, FSM_LED_OFF);

    // Init hardware peripherals
    taskFsmInit61();

    // Create tasks
    xTaskCreate(taskFsm61,    "TaskFSM61",    512, NULL, 2, NULL);
    xTaskCreate(taskReport61, "TaskReport61", 512, NULL, 1, NULL);

    vTaskStartScheduler();
}

void appLab61Loop() {
    // Empty — all work is done in FreeRTOS tasks
}