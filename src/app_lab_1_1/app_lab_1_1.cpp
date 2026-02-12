#include "app_lab_1_1.h"
#include <Arduino.h>
#include "stdio.h"
#include <string.h>
#include "srv_serial_stdio/Serial.h"
#include "dd_led/Led.h"

#define LED_PIN LED_BUILTIN
#define BAUD_RATE 9600
#define BUFFER_SIZE 20

void lab_1_1_setup() {
    // Initialize serial communication
    serial_init(BAUD_RATE);

    // Initialize the LED
    led_init(LED_PIN);
    led_turn_off(LED_PIN); // Start with the LED off

    printf("--- app_lab_1_1: Started ---\r\n");
    printf("Available commands:\r\n");
    printf(" led on - Turn the LED ON\r\n");
    printf(" led off - Turn the LED OFF\r\n");
}

void lab_1_1_loop() {
    char command[BUFFER_SIZE];

    printf("\r\nEnter command: ");
    printf("\r\n> ");

    // Read a line of input into the buffer up to BUFFER_SIZE-1 characters
    scanf(" %19[^\r\n]", command);

    // Debug: show what was read
    printf("[DEBUG] Received: '%s' (length: %d)\n", command, strlen(command));

    // Process the command
    if (strcmp(command, "led on") == 0) {
        led_turn_on(LED_PIN);
        printf("LED is turned ON\r\n");
    } else if (strcmp(command, "led off") == 0) {
        led_turn_off(LED_PIN);
        printf("LED is turned OFF\r\n");
    } else {
        printf("! WARNING ! Unknown command. Use 'led on' or 'led off'\r\n");
    }

    delay(100); // Small delay to avoid overwhelming the serial output
}
