#include <Arduino.h>
#include "Led.h"

void ledInit(int pin) {
    pinMode(pin, OUTPUT); // Set the pin as an output
}

void ledTurnOn(int pin) {
    digitalWrite(pin, HIGH); // Turn the LED on
}

void ledTurnOff(int pin) {
    digitalWrite(pin, LOW); // Turn the LED off
}

int isLedOn(int pin) {
    return digitalRead(pin) == HIGH ? 1 : 0; // Return 1 if LED is on, 0 if off
}

void toggleLed(int pin) {
    if (isLedOn(pin)) {
        ledTurnOff(pin);
    } else {
        ledTurnOn(pin);
    }
}
