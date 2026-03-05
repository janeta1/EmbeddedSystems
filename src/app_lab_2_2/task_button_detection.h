#ifndef TASK_BUTTON_DETECTION_H
#define TASK_BUTTON_DETECTION_H

#include <Arduino_FreeRTOS.h>

// Task configuration
#define BTN_DET_OFFSET_MS 0    // Initial offset
#define BTN_DET_REC 20         // 20ms recurrence period
#define MAX_PRESS_DURATION_MS 10000  // Max duration to prevent overflow

void taskButtonDetection(void *pvParameters);

#endif // TASK_BUTTON_DETECTION_H