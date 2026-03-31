#ifndef APP_LAB_4_1_H
#define APP_LAB_4_1_H

#include <Arduino_FreeRTOS.h>

#define RELAY_PIN41 7
#define SERVO_PIN41 6
#define BUTTON_RELAY41 23

// Task periods
#define TASK_INPUT41_MS 50
#define TASK_COND41_MS 100
#define TASK_REPORT41_MS 1000

void appLab41Setup();
void appLab41Loop();

#endif // APP_LAB_4_1_H