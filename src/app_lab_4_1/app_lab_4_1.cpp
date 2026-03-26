#include "app_lab_4_1.h"
#include "task_input_4_1.h"
#include "task_conditioning_4_1.h"
#include "task_report_4_1.h"
#include "dd_relay/dd_relay.h"
#include "dd_servo/dd_servo.h"
#include "srv_stdio_lcd/srv_stdio_lcd.h"
#include "srv_serial_stdio/Serial.h"
#include "dd_led/Led.h"
#include "srv_stdio_keypad/srv_stdio_keypad.h"
#include "dd_button/dd_button.h"
#include <Arduino.h>
#include <Arduino_FreeRTOS.h>

void appLab41Setup() {
    // Initialize hardware
    serialInit(9600);
    srvStdioLCDSetup();

    ddRelayInit(RELAY_PIN41);
    ddServoInit(SERVO_PIN41);

    // Initialize tasks
    taskInputInit41();
    taskConditioningInit41();

    // Create tasks
    xTaskCreate(taskInput41, "TaskInput41", 512, NULL, 1, NULL);
    xTaskCreate(taskConditioning41, "TaskCond41", 512, NULL, 2, NULL);
    xTaskCreate(taskReport41, "TaskReport41", 768, NULL, 1, NULL);

    vTaskStartScheduler();
}

void appLab41Loop() {
    // Empty. All work is done in tasks.
}