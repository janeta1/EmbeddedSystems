#include "task_button_detection.h"
#include "app_lab_2_2.h"
#include "dd_button/dd_button.h"
#include "dd_led/Led.h"
#include <Arduino.h>

typedef enum
{
    BTN_STATE_IDLE,
    BTN_STATE_PRESSED,
} BtnDetState_e;

void taskButtonDetection(void *pvParameters)
{
    (void) pvParameters;  // Unused parameter
    
    BtnDetState_e btnState = BTN_STATE_IDLE;
    uint32_t btnPressStart = 0;
    
    // Initialize timing for periodic execution
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = pdMS_TO_TICKS(BTN_DET_REC);  // 20ms period
    
    // Initial offset delay if needed
    if (BTN_DET_OFFSET_MS > 0) {
        vTaskDelay(pdMS_TO_TICKS(BTN_DET_OFFSET_MS));
    }
    
    while (1) {
        // Check button state
        bool currentPressed = ddButtonIsPressed(BUTTON_PIN);
        
        switch (btnState) {
            // IDLE: Waiting for button press
            case BTN_STATE_IDLE:
                if (currentPressed) {
                    // Button just pressed - start timing
                    btnPressStart = millis();
                    btnState = BTN_STATE_PRESSED;
                    
                    ledTurnOff(LED_GREEN_PIN);
                    ledTurnOff(LED_RED_PIN);
                }
                break;
                
            // PRESSED: Button is held, waiting for release
            case BTN_STATE_PRESSED:
                if (!currentPressed) {
                    // Button released - calculate duration
                    uint32_t duration = millis() - btnPressStart;
                    
                    // Validate duration (prevent overflow/stuck button)
                    if (duration > MAX_PRESS_DURATION_MS) {
                        duration = MAX_PRESS_DURATION_MS;  // Cap at max
                    }
                    
                    bool isLongPress = (duration >= LONG_PRESS_MS);
                    
                    // Update shared data (protected by mutex)
                    if (xSemaphoreTake(xStatsMutex, portMAX_DELAY) == pdTRUE) {
                        globalLastPressDuration = duration;
                        globalIsLongPress = isLongPress;
                        globalNewPressDetected = true;
                        xSemaphoreGive(xStatsMutex);
                    }
                    
                    if (isLongPress) {
                        ledTurnOn(LED_RED_PIN);  
                    } else {
                        ledTurnOn(LED_GREEN_PIN); 
                    }

                    printf("\n- Button was pressed for %lu ms -\n\n", duration);
                    
                    // Signal Task 2 via semaphore (event notification)
                    xSemaphoreGive(xButtonPressSemaphore);
                    
                    // Return to idle state
                    btnState = BTN_STATE_IDLE;
                }
                break;
                
            default:
                // Safety fallback
                btnState = BTN_STATE_IDLE;
                break;
        }
        
        // Wait for next period (20ms)
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}