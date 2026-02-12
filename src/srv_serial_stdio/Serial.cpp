#include <Arduino.h>
#include "Serial.h"
#include <stdio.h>

// Write character to Serial
int serial_put_char(char c, FILE *stream) {
    Serial.write(c);
    return 0;
}

// Read character from Serial
int serial_get_char(FILE *stream) {
    while (!Serial.available()) {
        ; // Wait for data
    }
    return Serial.read();
}

// Streams for stdin and stdout
// static FILE serialOut;
// static FILE serialIn;

void serial_init(long baudRate) {
    Serial.begin(baudRate);
    while (!Serial) {
        ; // Wait for Serial to initialize
    }

    // Setup stdin and stdout to use serial port
    // fdev_setup_stream(&serialOut, serial_put_char, NULL, _FDEV_SETUP_WRITE);
    // fdev_setup_stream(&serialIn, NULL, serial_get_char, _FDEV_SETUP_READ);
    // stdout = &serialOut;
    // stdin = &serialIn;

    FILE *mystream = fdevopen(serial_put_char, serial_get_char);
    if (mystream == NULL) {
        // Handle error
        Serial.println("Failed to open serial stream");
    } else {
        stdout = stdin = stderr = mystream; // Set all standard streams to our serial stream
    }
}

// void serial_read_command(char *buffer, size_t length) {
//     size_t index = 0;
//     int c;

//     // Read characters until newline or buffer full
//     while (index < length - 1) {
//         while (!Serial.available()) {
//             ; // Wait for data
//         }
//         c = Serial.read();
//         if (c == '\n' || c == '\r' || c == EOF) {
//             break; // End of input
//         }
//         buffer[index++] = c;
//     }
//     buffer[index] = '\0'; // Null terminate

//     // Flush any remaining characters until newline
//     if (c != '\n' && c != EOF) {
//         while ((c = Serial.read()) != '\n' && c != EOF);
//     }
// }

// void serial_print(const char *message) {
//     printf("%s\n", message); // Print the message
// }
