#include <Arduino.h>
#include "SerialIO.h"
#include <stdio.h>

// Write character to Serial
static int serialPutChar(char c, FILE *stream) {
    Serial.write(c);
    return 0;
}

// Read character from Serial
static int serialGetChar(FILE *stream) {
    while (Serial.available() == 0) {
        ; // Wait for data
    }
    return Serial.read();
}

// Streams for stdin and stdout
static FILE serialOut;
static FILE serialIn;

void serialInit(long baudRate) {
    Serial.begin(baudRate);
    while (!Serial) {
        ; // Wait for Serial to initialize
    }

    // Setup stdin and stdout to use serial port
    fdev_setup_stream(&serialOut, serialPutChar, NULL, _FDEV_SETUP_WRITE);
    fdev_setup_stream(&serialIn, NULL, serialGetChar, _FDEV_SETUP_READ);
    stdout = &serialOut;
    stdin = &serialIn;
}

void serialReadCommand(char *buffer, size_t length) {
    size_t index = 0;
    int c;
    
    // Read characters until newline or buffer full
    while (index < length - 1) {
        c = getchar();
        if (c == '\n' || c == '\r' || c == EOF) {
            break;
        }
        buffer[index++] = c;
    }
    buffer[index] = '\0'; // Null terminate
    
    // Flush any remaining characters until newline
    if (c != '\n' && c != EOF) {
        while ((c = getchar()) != '\n' && c != EOF);
    }
}

void serialPrint(const char *message) {
    printf("%s\n", message); // Print the message
}