#include <Arduino.h>
#include "app_lab_1_1/app_lab_1_1.h"
#include "app_lab_1_2/app_lab_1_2.h"
#include "app_lab_2_1/app_lab_2_1.h"
#include "app_lab_2_2/app_lab_2_2.h"

#define LAB1_2 12
#define LAB1_1 11
#define LAB2_1 21
#define LAB2_2 22
#define APP_USED LAB2_2

void setup() {
  #if APP_USED == LAB1_1
      lab11Setup();
  #elif APP_USED == LAB1_2
      appLab12Setup();
  #elif APP_USED == LAB2_1
      appLab21Setup();
  #elif APP_USED == LAB2_2
      appLab22Setup();
  #endif
}

void loop() {
  #if APP_USED == LAB1_1
      lab11Loop();
  #elif APP_USED == LAB1_2
      appLab12Loop();
  #elif APP_USED == LAB2_1
      appLab21Loop();
  #elif APP_USED == LAB2_2
      appLab22Loop();  
  #endif
}
