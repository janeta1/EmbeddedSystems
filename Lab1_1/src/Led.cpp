#include <Arduino.h>
#include "Led.h"

Led::Led(int pinNumber) {
    pin = pinNumber;
    pinMode(pin, OUTPUT); // Set the pin as an output
}

void Led::turnOn() {
    digitalWrite(pin, HIGH); // Turn the LED on
}

void Led::turnOff() {
    digitalWrite(pin, LOW); // Turn the LED off
}