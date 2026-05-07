#include "task_fsm_6_1.h"
#include "app_lab_6_1.h"
#include "srv_fsm/srv_fsm.h"
#include "dd_button/dd_button.h"
#include "dd_led/Led.h"
#include <Arduino_FreeRTOS.h>

void taskFsmInit61() {
    ddButtonInit(BUTTON_PIN_61);
    ledInit(LED_PIN_61);
}

void taskFsm61(void *pvParameters) {
    (void) pvParameters;

    bool lastButtonState = false;
    TickType_t lastWakeTime = xTaskGetTickCount();

    for (;;) {
        // Step 1: Apply output based on current state (Moore: output = f(state))
        unsigned long out = srvFsmGetOutput(&g_fsm61);
        if (out) {
            ledTurnOn(LED_PIN_61);
        } else {
            ledTurnOff(LED_PIN_61);
        }

        // Step 2: Wait for the delay defined in the current state
        unsigned long delayMs = srvFsmGetDelay(&g_fsm61);
        vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(delayMs));

        // Step 3: Read input — detect rising edge only (press, not hold)
        bool pressed = ddButtonIsPressed(BUTTON_PIN_61);
        int input = (pressed && !lastButtonState) ? 1 : 0;
        lastButtonState = pressed;

        // Step 4: Transition to next state
        srvFsmStep(&g_fsm61, input);
    }
}