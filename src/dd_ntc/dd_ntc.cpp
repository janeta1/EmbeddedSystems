#include "dd_ntc.h"
#include <Arduino.h>
#include <math.h>

static int sRawValue = 0;
static int sVoltage = 0;
static float sCelsius = 0;

void ddNtcSetup() {
    // Analog pins need no pinMode setup
}

void ddNtcLoop() {
    // Step 1: Read raw ADC value from NTC
    sRawValue = analogRead(NTC_PIN);
    
    // Step 2: Map raw value to voltage (0-5000 mV)
    sVoltage = map(sRawValue, 0, DD_ADC_MAX, 0, 5000);
    
    // Step 3: RAW -> resistance (NTC on top of voltage divider)
    sCelsius = ddNtcConvertToCelsius(sRawValue);

}

float ddNtcConvertToCelsius(int raw) {
    if (raw == 0) {
        return sCelsius -273.0f;
    }
    float resistance = DD_NTC_SERIES_R / (DD_ADC_MAX / (float)raw - 1.0);
    // Step 4: Resistance -> temperature using Beta formula
    float steinhart = log(resistance / DD_NTC_NOMINAL_R) / DD_NTC_BETA; // ln(R/R0) / B
    steinhart += 1.0 / DD_NTC_NOMINAL_TEMP; // + (1/To)
    return (1 / steinhart) - 273.15;
}

int ddNtcGetRaw() {
    return sRawValue;
}

int ddNtcGetVoltage() {
    return sVoltage;
}

float ddNtcGetCelsius() {
    return sCelsius;
}