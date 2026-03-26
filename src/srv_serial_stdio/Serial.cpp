#include <Arduino.h>
#include "Serial.h"
#include <stdio.h>
#include <Arduino_FreeRTOS.h>

// Write character to Serial
int serialPutChar(char c, FILE *stream) {
    Serial.write(c);
    return 0;
}

// Read character from Serial
int serialGetChar(FILE *stream) {
    while (!Serial.available()) {
        vTaskDelay(pdMS_TO_TICKS(50));
    }
    return Serial.read();
}

void serialInit(long baudRate) {
    Serial.begin(baudRate);
    while (!Serial) {
        ; // Wait for Serial to initialize
    }

    // Setup stdin and stdout to use serial port

    static FILE myStream;
    fdev_setup_stream(&myStream, serialPutChar, serialGetChar, _FDEV_SETUP_RW);
    stdout = &myStream;
    stdin  = &myStream;
    stderr = &myStream;
}
