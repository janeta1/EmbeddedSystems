#include "app_lab_4_2.h"
#include "task_input_4_2.h"
#include "task_conditioning_4_2.h"
#include "task_report_4_2.h"
#include "dd_relay/dd_relay.h"
#include "dd_l298/dd_l298.h"
#include "srv_stdio_lcd/srv_stdio_lcd.h"
#include "srv_serial_stdio/Serial.h"
#include "dd_led/Led.h"
#include "srv_stdio_keypad/srv_stdio_keypad.h"
#include "dd_button/dd_button.h"
#include <Arduino.h>
#include <Arduino_FreeRTOS.h>

void appLab42Setup() {
    // Initialize hardware
    serialInit(9600);
    srvStdioLCDSetup();

    ddRelayInit(RELAY_PIN42);
    ddL298Init(L298_ENA_PIN42, L298_IN1_PIN42, L298_IN2_PIN42);

    // Initialize tasks
    taskInputInit42();
    taskConditioningInit42();

    // Create tasks
    xTaskCreate(taskInput42, "TaskInput42", 512, NULL, 1, NULL);
    xTaskCreate(taskConditioning42, "TaskCond42", 512, NULL, 2, NULL);
    xTaskCreate(taskReport42, "TaskReport42", 768, NULL, 1, NULL);

    vTaskStartScheduler();
}

void appLab42Loop() {
    // Empty. All work is done in tasks.
}