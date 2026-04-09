#include "app_lab_5_1.h"
#include "task_acquisition_5_1.h"
#include "task_conditioning_5_1.h"
#include "task_report_5_1.h"
#include "task_input_5_1.h"
#include "dd_relay/dd_relay.h"
#include "dd_dht/dd_dht.h"
#include "srv_stdio_lcd/srv_stdio_lcd.h"
#include "srv_serial_stdio/Serial.h"
#include <Arduino.h>
#include <Arduino_FreeRTOS.h>

void appLab51Setup() {
    // Initialize hardware
    serialInit(9600);
    srvStdioLCDSetup();

    ddRelayInit(RELAY_PIN51);
    ddDhtSetup();

    // Initialize tasks
    taskInputInit51();
    taskAcquisitionInit51();
    taskConditioningInit51();

    // Create tasks
    xTaskCreate(taskInput51,        "TaskInput51",  512, NULL, 1, NULL);
    xTaskCreate(taskAcquisition51,  "TaskAcq51",    512, NULL, 3, NULL);
    xTaskCreate(taskConditioning51, "TaskCond51",   512, NULL, 2, NULL);
    xTaskCreate(taskReport51,       "TaskReport51", 768, NULL, 1, NULL);

    vTaskStartScheduler();
}

void appLab51Loop() {
    // Empty. All work is done in FreeRTOS tasks.
}