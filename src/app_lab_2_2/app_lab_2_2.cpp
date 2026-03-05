#include "app_lab_2_2.h"
#include "task_button_detection.h"
#include "task_statistics.h"
#include "task_report.h"
#include "dd_button/dd_button.h"
#include "dd_led/Led.h"
#include "srv_serial_stdio/Serial.h"
#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <task.h>
#include <semphr.h>

// Global synchronization objects
SemaphoreHandle_t xButtonPressSemaphore = NULL;
SemaphoreHandle_t xStatsMutex = NULL;

// Task 1 globals (same as Lab 2.1)
bool globalNewPressDetected = false;
uint32_t globalLastPressDuration = 0;
bool globalIsLongPress = false;

// Task 2 globals (same as Lab 2.1)
uint32_t globalTotalPresses = 0;
uint32_t globalShortPresses = 0;
uint32_t globalLongPresses = 0;
uint32_t globalSumShortPressDuration = 0;
uint32_t globalSumLongPressDuration = 0;

void appLab22Setup()
{
    serialInit(9600);
    ddButtonInit(BUTTON_PIN);
    ledInit(LED_GREEN_PIN);
    ledInit(LED_RED_PIN);
    ledInit(YELLOW_LED_PIN);
    ledTurnOff(LED_GREEN_PIN);
    ledTurnOff(LED_RED_PIN);
    ledTurnOff(YELLOW_LED_PIN);

    printf("\n=== Lab 2.2 - FreeRTOS Preemptive ===\n");
    printf("Initializing synchronization...\n");

    // Create binary semaphore for button press signaling
    xButtonPressSemaphore = xSemaphoreCreateBinary();
    if (xButtonPressSemaphore == NULL)
    {
        printf("ERROR: Failed to create semaphore!\n");
        while (1)
            ; // Halt
    }

    // Create mutex for statistics protection
    xStatsMutex = xSemaphoreCreateMutex();
    if (xStatsMutex == NULL)
    {
        printf("ERROR: Failed to create mutex!\n");
        while (1)
            ; // Halt
    }

    printf("Creating FreeRTOS tasks...\n");

    // Task 1: Button Detection (20ms, Priority 3 - Highest)
    xTaskCreate(taskButtonDetection, "BtnDet", 128, NULL, 3, NULL);

    // Task 2: Statistics & Blink (100ms, Priority 2 - Normal)
    xTaskCreate(taskStatistics, "Stats", 160, NULL, 2, NULL);

    // Task 3: Periodic Report (10s, Priority 1 - Lowest)
    xTaskCreate(taskReport, "Report", 192, NULL, 1, NULL);

    printf("Tasks created! Starting scheduler...\n");
    printf("Press button (<500ms=short, >=500ms=long)\n\n");
    vTaskStartScheduler();
}

void appLab22Loop()
{
    // Empty - FreeRTOS handles everything
}