#include "app_lab_5_2.h"
#include "task_acquisition_5_2.h"
#include "task_conditioning_5_2.h"
#include "task_report_5_2.h"
#include "task_input_5_2.h"
#include "dd_relay/dd_relay.h"
#include "dd_dht/dd_dht.h"
#include "dd_l298/dd_l298.h"
#include "srv_stdio_lcd/srv_stdio_lcd.h"
#include "srv_serial_stdio/Serial.h"
#include <Arduino.h>
#include <Arduino_FreeRTOS.h>

void appLab52Setup() {
    // Initialize hardware
    serialInit(9600);
    srvStdioLCDSetup();

    ddRelayInit(RELAY_PIN52);
    ddDhtSetup();

    ddL298Init(L298_ENA_PIN52, L298_IN1_PIN52, L298_IN2_PIN52);

    // Initialize tasks
    taskInputInit52();
    taskAcquisitionInit52();
    taskConditioningInit52();

    // Create tasks
    xTaskCreate(taskInput52,        "TaskInput52",  512, NULL, 1, NULL);
    xTaskCreate(taskAcquisition52,  "TaskAcq52",    512, NULL, 3, NULL);
    xTaskCreate(taskConditioning52, "TaskCond52",   512, NULL, 2, NULL);
    xTaskCreate(taskReport52,       "TaskReport52", 768, NULL, 1, NULL);

    vTaskStartScheduler();
}

void appLab52Loop() {
    // Empty. All work is done in FreeRTOS tasks.
}