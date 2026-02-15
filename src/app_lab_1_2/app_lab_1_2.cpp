#include "app_lab_1_2.h"
#include "srv_stdio_keypad/srv_stdio_keypad.h"
#include "srv_stdio_lcd/srv_stdio_lcd.h"
#include "dd_led/Led.h"
#include <stdio.h>
#include <Arduino.h>

#define GREEN_LED_PIN 10
#define RED_LED_PIN 11

#define CODE_LENGTH 4

// The correct code to unlock the system
const char correctCode[CODE_LENGTH + 1] = "2709"; 

void appLab12Setup() {
    // Initialize the LCD and keypad services
    srvStdioLCDSetup();
    srvStdioKeypadSetup();

    // Initialize the LEDs
    ledInit(GREEN_LED_PIN);
    ledInit(RED_LED_PIN);

    printf("- System Ready -\n");
    delay(1000);
}

void appLab12Loop() {
    char enteredCode[CODE_LENGTH + 1] = {0}; // Buffer to store the entered code

    // Clear the LCD
    printf("%c", 0x1B);
    printf("> Enter Code: ");

    // Read the code from the keypad
    for (int i = 0; i < CODE_LENGTH; i++) {
        scanf("%c", &enteredCode[i]);
        // printf("%c", enteredCode[i]);
        printf("*"); // mask input
    }

    // Null-terminate the entered code
    enteredCode[CODE_LENGTH] = '\0';
    delay(1000); // Simulate processing time
    printf("%c", 0x1B); // Clear the LCD

    if (strcmp(enteredCode, correctCode) == 0) {
        printf("> Access Granted\n");
        ledTurnOn(GREEN_LED_PIN);
        ledTurnOff(RED_LED_PIN);
    } else {
        printf("> Access Denied\n");
        ledTurnOn(RED_LED_PIN);
        ledTurnOff(GREEN_LED_PIN);
    }

    delay(3000); // Wait before allowing another attempt

    // Reset the system for the next attempt
    ledTurnOff(GREEN_LED_PIN);
    ledTurnOff(RED_LED_PIN);
}