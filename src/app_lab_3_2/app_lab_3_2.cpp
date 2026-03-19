#include "app_lab_3_2.h"
#include "task_acquisition3_2.h"
#include "task_conditioning3_2.h"
#include "task_report3_2.h"
#include "srv_sns_temp/srv_sns_temp.h"
#include "srv_stdio_lcd/srv_stdio_lcd.h"
#include "dd_led/Led.h"
#include "srv_serial_stdio/Serial.h"
#include <Arduino.h>
#include <Arduino_FreeRTOS.h>

void appLab32Setup() {
    // Initialize services
    serialInit(9600);
    srvStdioLCDSetup();

    ledInit(LED_GREEN_PIN_LAB32);
    ledInit(LED_RED_PIN_LAB32);
    ledTurnOff(LED_GREEN_PIN_LAB32);
    ledTurnOff(LED_RED_PIN_LAB32);

    srvSnsTempSetup();

    // Create tasks
    xTaskCreate(taskAcquisition3_2, "Acquisition", 512, NULL, 3, NULL);
    xTaskCreate(taskConditioning3_2, "Conditioning", 384, NULL, 2, NULL);
    xTaskCreate(taskReport3_2, "Report3_2", 768, NULL, 1, NULL);

    vTaskStartScheduler();
}

void appLab32Loop() {
    // Empty - everything is handled by FreeRTOS
}