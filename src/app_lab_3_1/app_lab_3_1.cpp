#include "app_lab_3_1.h"
#include "task_acquisition.h"
#include "task_conditioning.h"
#include "task_report31.h"
#include "srv_sns_temp/srv_sns_temp.h"
#include "srv_stdio_lcd/srv_stdio_lcd.h"
#include "dd_led/Led.h"
#include "srv_serial_stdio/Serial.h"
#include <Arduino.h>
#include <Arduino_FreeRTOS.h>

void appLab31Setup() {
    // Initialize services
    serialInit(9600);
    srvStdioLCDSetup();

    ledInit(LED_GREEN_PIN_LAB31);
    ledInit(LED_RED_PIN_LAB31);
    ledTurnOff(LED_GREEN_PIN_LAB31);
    ledTurnOff(LED_RED_PIN_LAB31);

    srvSnsTempSetup();

    // Create tasks
    xTaskCreate(taskAcquisition, "Acquisition", 512, NULL, 3, NULL);
    xTaskCreate(taskConditioning, "Conditioning", 384, NULL, 2, NULL);
    xTaskCreate(taskReport31, "Report31", 768, NULL, 1, NULL);

    vTaskStartScheduler();
}

void appLab31Loop() {
    // Empty - everything is handled by FreeRTOS
}