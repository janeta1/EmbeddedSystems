#include <Arduino.h>
#include "app_lab_1_1/app_lab_1_1.h"
#include "app_lab_1_2/app_lab_1_2.h"
#include "app_lab_2_1/app_lab_2_1.h"
#include "app_lab_2_2/app_lab_2_2.h"
#include "app_lab_3_1/app_lab_3_1.h"
#include "app_lab_3_2/app_lab_3_2.h"
#include "app_lab_4_1/app_lab_4_1.h"
#include "app_lab_4_2/app_lab_4_2.h"

#define LAB1_2 12
#define LAB1_1 11
#define LAB2_1 21
#define LAB2_2 22
#define LAB3_1 31
#define LAB3_2 32
#define LAB4_1 41
#define LAB4_2 42
#define APP_USED LAB4_1

void setup() {
    #if APP_USED == LAB1_1
        lab11Setup();
    #elif APP_USED == LAB1_2
        appLab12Setup();
    #elif APP_USED == LAB2_1
        appLab21Setup();
    #elif APP_USED == LAB2_2
        appLab22Setup();
    #elif APP_USED == LAB3_1
        appLab31Setup();
    #elif APP_USED == LAB3_2
        appLab32Setup();
    #elif APP_USED == LAB4_1
        appLab41Setup();
    #elif APP_USED == LAB4_2
        appLab42Setup();
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
    #elif APP_USED == LAB3_1
        appLab31Loop();
    #elif APP_USED == LAB3_2
        appLab32Loop();
    #elif APP_USED == LAB4_1
        appLab41Loop();
    #elif APP_USED == LAB4_2
        appLab42Loop();
    #endif
}
