#include <Arduino.h>
#include "Serial.h"
#include <stdio.h>

// Write character to Serial
int serialPutChar(char c, FILE *stream) {
    Serial.write(c);
    return 0;
}

// Read character from Serial
int serialGetChar(FILE *stream) {
    while (!Serial.available()) {
        ; // Wait for data
    }
    return Serial.read();
}


void serialInit(long baudRate) {
    Serial.begin(baudRate);
    while (!Serial) {
        ; // Wait for Serial to initialize
    }

    // Setup stdin and stdout to use serial port

    FILE *myStream = fdevopen(serialPutChar, serialGetChar);
    if (myStream == NULL) {
        // Handle error
        Serial.println("Failed to open serial stream");
    } else {
        stdout = stdin = stderr = myStream; // Set all standard streams to our serial stream
    }
}
