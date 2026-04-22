#ifndef APP_LAB_5_2_H
#define APP_LAB_5_2_H

#include <Arduino_FreeRTOS.h>

#define RELAY_PIN52 7

#define L298_ENA_PIN52 6
#define L298_IN1_PIN52 4
#define L298_IN2_PIN52 5

// Hysteresis thresholds around SetPoint
#define HYSTERESIS_DELTA 1.0f   // Von = SP - delta, Voff = SP + delta

// Default SetPoint (degrees C)
#define DEFAULT_SETPOINT 25.0f

#define DEFAULT_KP 20.0f
#define DEFAULT_KI 0.5f
#define DEFAULT_KD 1.0f

#define PID_INTEGRAL_MAX 255.0f
#define PID_INTEGRAL_MIN -255.0f

// Task periods
#define TASK_ACQ52_MS 2000
#define TASK_COND52_MS 50
#define TASK_REPORT52_MS 1000

void appLab52Setup();
void appLab52Loop();

#endif // APP_LAB_5_2_H