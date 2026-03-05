#include "app_lab_2_1.h"
#include "dd_button/dd_button.h"
#include "dd_led/Led.h"
#include "srv_os_seq/srv_os_seq.h"
#include "srv_serial_stdio/Serial.h"
#include <Arduino.h>
#include <avr/interrupt.h>

#define BUTTON_PIN 2
#define LED_GREEN_PIN 3
#define LED_RED_PIN 4
#define YELLOW_LED_PIN 5

#define LONG_PRESS_MS 500
#define SHORT_PRESS_BLINKS 5
#define LONG_PRESS_BLINKS 10

// Task 1 globals
static bool globalNewPressDetected = false;
static uint32_t globalLastPressDuration = 0;
static bool globalIsLongPress = false;

// Task 2 globals
static uint32_t globalTotalPresses = 0;
static uint32_t globalShortPresses = 0;
static uint32_t globalLongPresses = 0;
static uint32_t globalSumShortPressDuration = 0;
static uint32_t globalSumLongPressDuration = 0;

// Task 1 state
typedef enum {
    BTN_STATE_IDLE,
    BTN_STATE_PRESSED,
} BtnDetState_e;

static BtnDetState_e btnState = BTN_STATE_IDLE;
static uint32_t btnPressStart = 0;

// typedef enum
// {
//     MON_STATE_IDLE,
//     MON_STATE_BLINK,
// } MonState_e;

// static MonState_e monState = MON_STATE_IDLE;
static int blinkCount = 0;
static int blinkTarget = 0;

void appLab21Setup()
{
    serialInit(9600);

    ddButtonInit(BUTTON_PIN);
    ledInit(LED_GREEN_PIN);
    ledInit(LED_RED_PIN);
    ledInit(YELLOW_LED_PIN);
    ledTurnOff(LED_GREEN_PIN);
    ledTurnOff(LED_RED_PIN);
    ledTurnOff(YELLOW_LED_PIN);

    srvOsSeqInit();

    printf("App Lab 2.1 setup complete.\n");
}

void appLab21Loop()
{
}

// Task 1: button detection (20ms)
void task1BtnDetLoop()
{
    bool pressed = ddButtonIsPressed(BUTTON_PIN);

    switch (btnState) {
        // Button was not pressed, now is pressed -> start timing
        case BTN_STATE_IDLE:
            if (pressed)
            {
                btnPressStart = millis();
                btnState = BTN_STATE_PRESSED;
                ledTurnOff(LED_GREEN_PIN);
                ledTurnOff(LED_RED_PIN);
            }
            break;
        // Button was pressed, now is released -> calculate duration and type
        case BTN_STATE_PRESSED:
            if (!pressed)
            {
                uint32_t duration = millis() - btnPressStart;
                globalLastPressDuration = duration;
                globalIsLongPress = (duration >= LONG_PRESS_MS);
                // Set the flag to indicate a new press has been detected
                // for task 2 to process in its loop
                globalNewPressDetected = true;

                if (globalIsLongPress) {
                    ledTurnOn(LED_RED_PIN);
                }
                else {
                    ledTurnOn(LED_GREEN_PIN);
                }

                // Reset state to idle to wait for the next press
                btnState = BTN_STATE_IDLE;
            }
            break;

        default:
            // Safety fallback
            btnState = BTN_STATE_IDLE;
            break;
        }
}

// Task 2: statistics + yellow blink (100ms)
void task2BtnMonLoop()
{
    if (globalNewPressDetected) {
        globalTotalPresses++;

        if (globalIsLongPress) {
            globalLongPresses++;
            globalSumLongPressDuration += globalLastPressDuration;
            // The led will blink 10 times (10 * 2 because on+off) for long presses
            blinkTarget = LONG_PRESS_BLINKS * 2;
        }
        else {
            globalShortPresses++;
            globalSumShortPressDuration += globalLastPressDuration;
            // The led will blink 5 times (5 * 2 because on+off) for short presses
            blinkTarget = SHORT_PRESS_BLINKS * 2;
        }
        
        blinkCount = 0; // Reset blink count to start blinking for the new press
        globalNewPressDetected = false; // Reset the flag after processing
    }

    if (blinkCount < blinkTarget) {
        if (blinkCount % 2 == 0) {
            ledTurnOn(YELLOW_LED_PIN);
        }
        else {
            ledTurnOff(YELLOW_LED_PIN);
        }
        blinkCount++;
    }
    else {
        // Ensure the yellow LED is off after finishing the blinking sequence
        ledTurnOff(YELLOW_LED_PIN);
    }
}

// Task 3: periodic report (10s)
void task3RepLoop() {
    int totalPresses = globalTotalPresses;
    int shortPresses = globalShortPresses;
    int longPresses = globalLongPresses;

    printf("=== Button Press Report (10s) ===\n");
    printf("Total presses: %d\n", totalPresses);
    printf("Short presses (<500ms): %d\n", shortPresses);
    printf("Long presses (>=500ms): %d\n", longPresses);

    if (totalPresses > 0) {
        uint32_t avgDuration = (globalSumShortPressDuration + globalSumLongPressDuration) / totalPresses;
        printf("Average duration: %lu ms\n", avgDuration);
    }
    else {
        printf("Average duration: N/A\n");
    }

    printf("===========================\n");

    globalTotalPresses = 0;
    globalShortPresses = 0;
    globalLongPresses = 0;
    globalSumShortPressDuration = 0;
    globalSumLongPressDuration = 0;
}