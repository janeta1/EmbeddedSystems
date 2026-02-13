#include <Arduino.h>
#include "app_lab_1_1/app_lab_1_1.h"

#define APP_USED LAB1_1

void setup() {
  #if APP_USED == LAB1_1
      lab11Setup();
  #endif
}

void loop() {
  #if APP_USED == LAB1_1
      lab11Loop();
  #endif
}
