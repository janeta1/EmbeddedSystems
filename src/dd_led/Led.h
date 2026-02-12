#ifndef LED_H
#define LED_H

void led_init(int pin);
void led_turn_on(int pin);
void led_turn_off(int pin);
int is_led_on(int pin);
void toggle_led(int pin);

#endif