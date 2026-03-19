#ifndef DD_NTC_H
#define DD_NTC_H
#include <Arduino.h>

#define NTC_PIN A0

// Constants
#define DD_NTC_SERIES_R 10000
#define DD_NTC_NOMINAL_R 10000
#define DD_NTC_NOMINAL_TEMP 298.15 // 25°C in Kelvin
#define DD_NTC_BETA 3950
#define DD_ADC_MAX 1023

// Function prototypes
void ddNtcSetup();
void ddNtcLoop();
int ddNtcGetRaw();
int ddNtcGetVoltage();
float ddNtcGetCelsius();
float ddNtcConvertToCelsius(int raw);

#endif // DD_NTC_H