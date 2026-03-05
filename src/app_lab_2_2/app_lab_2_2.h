#ifndef APP_LAB_2_2_H
#define APP_LAB_2_2_H

#include <Arduino_FreeRTOS.h>
#include <semphr.h>

#define BUTTON_PIN 2
#define LED_GREEN_PIN 12
#define LED_RED_PIN 11
#define YELLOW_LED_PIN 10

// Press detection thresholds
#define LONG_PRESS_MS 500
#define SHORT_PRESS_BLINKS 5
#define LONG_PRESS_BLINKS 10

extern SemaphoreHandle_t xButtonPressSemaphore;  // Signal new press
extern SemaphoreHandle_t xStatsMutex;            // Protect statistics

// Task 1 globals (same as Lab 2.1)
extern bool globalNewPressDetected;
extern uint32_t globalLastPressDuration;
extern bool globalIsLongPress;

// Task 2 globals (same as Lab 2.1)
extern uint32_t globalTotalPresses;
extern uint32_t globalShortPresses;
extern uint32_t globalLongPresses;
extern uint32_t globalSumShortPressDuration;
extern uint32_t globalSumLongPressDuration;

void appLab22Setup();
void appLab22Loop();

#endif // APP_LAB_2_2_H