#ifndef APP_LAB_5_1_H
#define APP_LAB_5_1_H

#include <Arduino_FreeRTOS.h>

#define RELAY_PIN51 7

// Hysteresis thresholds around SetPoint
#define HYSTERESIS_DELTA 1.0f   // Von = SP - delta, Voff = SP + delta

// Default SetPoint (degrees C)
#define DEFAULT_SETPOINT 25.0f

// Task periods
#define TASK_ACQ51_MS 2000
#define TASK_COND51_MS 50
#define TASK_REPORT51_MS 1000

void appLab51Setup();
void appLab51Loop();

#endif // APP_LAB_5_1_H