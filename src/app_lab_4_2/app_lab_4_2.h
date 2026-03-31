#ifndef APP_LAB_4_2_H
#define APP_LAB_4_2_H

#include <Arduino_FreeRTOS.h>

#define RELAY_PIN42 7
// #define SERVO_PIN42 6
#define L298_ENA_PIN42  6
#define L298_IN1_PIN42  4
#define L298_IN2_PIN42  5

// Task periods
#define TASK_INPUT42_MS 100
#define TASK_COND42_MS 100
#define TASK_REPORT42_MS 1000

void appLab42Setup();
void appLab42Loop();

#endif // APP_LAB_4_2_H