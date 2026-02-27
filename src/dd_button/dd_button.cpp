#include "dd_button.h"
#include <Arduino.h>

void ddButtonInit(int pin) {
    pinMode(pin, INPUT_PULLUP); 
}

int ddButtonRead(int pin) {
    return digitalRead(pin);
}

bool ddButtonIsPressed(int pin) {
    return digitalRead(pin) == LOW; 
}