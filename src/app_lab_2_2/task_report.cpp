#include "task_report.h"
#include "app_lab_2_2.h"
#include "srv_serial_stdio/Serial.h"
#include <Arduino.h>

void taskReport(void *pvParameters)
{
    (void) pvParameters;  // Unused parameter
    
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = pdMS_TO_TICKS(REPORT_REC);  // 10 seconds
    
    // Initial offset delay
    if (REPORT_OFFSET_MS > 0) {
        vTaskDelay(pdMS_TO_TICKS(REPORT_OFFSET_MS));
    }
    
    while (1) {
        // Wait for 10 seconds
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
        
        uint32_t totalPresses, shortPresses, longPresses;
        uint32_t sumShortDuration, sumLongDuration;
        
        if (xSemaphoreTake(xStatsMutex, portMAX_DELAY) == pdTRUE) {
            totalPresses = globalTotalPresses;
            shortPresses = globalShortPresses;
            longPresses = globalLongPresses;
            sumShortDuration = globalSumShortPressDuration;
            sumLongDuration = globalSumLongPressDuration;
            
            // Reset for next period
            globalTotalPresses = 0;
            globalShortPresses = 0;
            globalLongPresses = 0;
            globalSumShortPressDuration = 0;
            globalSumLongPressDuration = 0;
            
            xSemaphoreGive(xStatsMutex);  // Release mutex
        } else {
            printf("ERROR: Failed to acquire mutex\n");
            continue;
        }
        
        // Print report (outside critical section)
        printf("\n=== Button Press Report (10s) ===\n");
        printf("Total presses: %lu\n", totalPresses);
        printf("Short presses (<500ms): %lu\n", shortPresses);
        printf("Long presses (>=500ms): %lu\n", longPresses);
        
        if (totalPresses > 0) {
            uint32_t avgDuration = (sumShortDuration + sumLongDuration) / totalPresses;
            printf("Average duration: %lu ms\n", avgDuration);
        } else {
            printf("Average duration: N/A\n");
        }
        
        printf("=================================\n\n");
    }
}
