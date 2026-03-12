#include "dd_potentiometer.h"
#include <Arduino.h>

static int sRawValue = 0;
static int sVoltage = 0;
static float sCelsius = 0;

void ddPotentiometerSetup() {
    // Analog pins need no pinMode setup
}

void ddPotentiometerLoop() {
    // Step 1: Read raw ADC value from potentiometer
    sRawValue = analogRead(POTENTIOMETER_PIN);
    
    // Step 2: Map raw value to voltage (0-5000 mV)
    sVoltage = map(sRawValue, DD_POTENTIOMETER_RAW_MIN, DD_POTENTIOMETER_RAW_MAX, 
                   DD_POTENTIOMETER_VOLTAGE_MIN, DD_POTENTIOMETER_VOLTAGE_MAX);
    
    // Step 3: Map raw value to temperature (0-100 °C)
    sCelsius = map(sRawValue, DD_POTENTIOMETER_RAW_MIN, DD_POTENTIOMETER_RAW_MAX, 
                   DD_POTENTIOMETER_TEMP_MIN, DD_POTENTIOMETER_TEMP_MAX);
}

int ddPotentiometerGetRaw() {
    return sRawValue;
}

int ddPotentiometerGetVoltage() {
    return sVoltage;
}

float ddPotentiometerGetTemperature() {
    return sCelsius;
}