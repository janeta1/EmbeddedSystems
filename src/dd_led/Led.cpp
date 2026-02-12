#include <Arduino.h>
#include "Led.h"

void led_init(int pin) {
    pinMode(pin, OUTPUT); // Set the pin as an output
}

void led_turn_on(int pin) {
    digitalWrite(pin, HIGH); // Turn the LED on
}

void led_turn_off(int pin) {
    digitalWrite(pin, LOW); // Turn the LED off
}

int is_led_on(int pin) {
    return digitalRead(pin) == HIGH ? 1 : 0; // Return 1 if LED is on, 0 if off
}

void toggle_led(int pin) {
    if (is_led_on(pin)) {
        led_turn_off(pin);
    } else {
        led_turn_on(pin);
    }
}
