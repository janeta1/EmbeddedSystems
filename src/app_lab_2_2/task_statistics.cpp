#include "task_statistics.h"
#include "app_lab_2_2.h"
#include "dd_led/Led.h"
#include <Arduino.h>

void taskStatistics(void *pvParameters)
{
    (void) pvParameters;  // Unused parameter
    
    int blinkCount = 0;
    int blinkTarget = 0;
    
    // Initial offset delay
    if (STATS_OFFSET_MS > 0) {
        vTaskDelay(pdMS_TO_TICKS(STATS_OFFSET_MS));
    }
    
    while (1) {
        // Check for new button press 
        if (xSemaphoreTake(xButtonPressSemaphore, pdMS_TO_TICKS(100)) == pdTRUE) {
            // New press detected - read press data with mutex protection
            uint32_t duration;
            bool isLongPress;
            
            if (xSemaphoreTake(xStatsMutex, portMAX_DELAY) == pdTRUE) {
                duration = globalLastPressDuration;
                isLongPress = globalIsLongPress;
                globalNewPressDetected = false;  
                
                globalTotalPresses++;
                
                if (isLongPress) {
                    globalLongPresses++;
                    globalSumLongPressDuration += duration;
                    blinkTarget = LONG_PRESS_BLINKS * 2;  // 10 blinks = 20 toggles
                } else {
                    globalShortPresses++;
                    globalSumShortPressDuration += duration;
                    blinkTarget = SHORT_PRESS_BLINKS * 2;  // 5 blinks = 10 toggles
                }
                
                xSemaphoreGive(xStatsMutex);  // Release mutex
            }
            
            // Reset blink counter for new sequence
            blinkCount = 0;
        }
        
        // Handle yellow LED blinking 
        if (blinkCount < blinkTarget) {
            if (blinkCount % 2 == 0) {
                ledTurnOn(YELLOW_LED_PIN);
            } else {
                ledTurnOff(YELLOW_LED_PIN);
            }
            blinkCount++;
        } else {
            // Ensure LED is off when not blinking
            ledTurnOff(YELLOW_LED_PIN);
        }
    }
}
