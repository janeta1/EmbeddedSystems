#ifndef DD_POTENTIOMETER_H
#define DD_POTENTIOMETER_H
#include <Arduino.h>


#define POTENTIOMETER_PIN A0

// ADC constants
#define DD_POTENTIOMETER_RAW_MIN 0
#define DD_POTENTIOMETER_RAW_MAX 1023

// Voltage range for the potentiometer
#define DD_POTENTIOMETER_VOLTAGE_MIN 0
#define DD_POTENTIOMETER_VOLTAGE_MAX 5000

// Simulated temperature range in degrees Celsius
#define DD_POTENTIOMETER_TEMP_MIN 0
#define DD_POTENTIOMETER_TEMP_MAX 100

// Function prototypes
void ddPotentiometerSetup();
void ddPotentiometerLoop();
int ddPotentiometerGetRaw();
int ddPotentiometerGetVoltage();
float ddPotentiometerGetTemperature();

#endif // DD_POTENTIOMETER_H