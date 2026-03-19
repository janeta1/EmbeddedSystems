#ifndef APP_LAB_3_2_H
#define APP_LAB_3_2_H

#include <Arduino_FreeRTOS.h>

#define LED_GREEN_PIN_LAB32 7
#define LED_RED_PIN_LAB32 8

#define TASK_ACQ_MS 50
#define TASK_COND_MS 20
#define TASK_REPORT_MS 500

// Function prototypes
void appLab32Setup();
void appLab32Loop();

#endif // APP_LAB_3_2_H