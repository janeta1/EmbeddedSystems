#ifndef APP_LAB_3_1_H
#define APP_LAB_3_1_H

#include <Arduino_FreeRTOS.h>

#define LED_GREEN_PIN_LAB31 7
#define LED_RED_PIN_LAB31 8


#define TASK_ACQ_MS 50
#define TASK_COND_MS 20
#define TASK_REPORT_MS 500

// Function prototypes
void appLab31Setup();
void appLab31Loop();

#endif // APP_LAB_3_1_H