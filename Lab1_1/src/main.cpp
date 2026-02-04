#include <Arduino.h>
#include "Led.h"
#include "SerialIO.h"
#include <string.h>

#define LED_PIN LED_BUILTIN

Led led(LED_PIN);

void setup() {
  serialInit(9600);
}

void loop() {
  char command[20];

  printf("Enter command (led on/led off): ");
  serialReadCommand(command, sizeof(command));
  
  // Debug: show what was read
  printf("[DEBUG] Received: '%s' (length: %d)\n", command, strlen(command));

  if (strcmp(command, "led on") == 0) {
    led.turnOn();
    serialPrint("LED is turned ON");
  } else if (strcmp(command, "led off") == 0) {
    led.turnOff();
    serialPrint("LED is turned OFF");
  } else {
    serialPrint("! WARNING ! Unknown command. Use 'led on' or 'led off'");
  }
}
