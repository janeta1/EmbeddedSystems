#ifndef LED_H
#define LED_H

void ledInit(int pin);
void ledTurnOn(int pin);
void ledTurnOff(int pin);
int isLedOn(int pin);
void toggleLed(int pin);

#endif