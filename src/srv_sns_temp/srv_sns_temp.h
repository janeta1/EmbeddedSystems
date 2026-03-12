#ifndef SRV_SNS_TEMP_H
#define SRV_SNS_TEMP_H
#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <semphr.h>

// ----------Alert thresholds----------
// // Sensor 1 - Potentiometer
// #define POT_THRESHOLD_HIGH 26
// #define POT_THRESHOLD_LOW 24
// #define POT_DEBOUNCE 5

// // Sensor 2 - DS18B20
// #define DS18B20_THRESHOLD_HIGH  26
// #define DS18B20_THRESHOLD_LOW   24
// #define DS18B20_DEBOUNCE        5

// Sensor 1 - NTC
#define NTC_THRESHOLD_HIGH 26
#define NTC_THRESHOLD_LOW 24
#define NTC_DEBOUNCE 5

// Sensor 2 - DHT22
#define DHT_THRESHOLD_HIGH 26
#define DHT_THRESHOLD_LOW 24
#define DHT_DEBOUNCE 5

// ----------Alert states----------
typedef struct {
    bool alertActive; // confirmed debounced alert state
    bool alertPending; // new alert detected, pending debounce confirmation
    int debounceCounter; // counts consecutive readings above/below threshold
} AlertState;

// Function prototypes
void srvSnsTempSetup();

// Called from acquisition task every 20ms (writes sensor data)
void srvSnsTempAcquire();

// Called from conditioning task every 20ms (processes alert logic)
void srvSnsTempProcess();

// Called from task_report 
int srvSnsTempGetNtcRaw();
int srvSnsTempGetNtcVoltage();
float srvSnsTempGetNtcCelsius();
float srvSnsTempGetDhtTemperature();
float srvSnsTempGetDhtHumidity();
AlertState srvSnsTempGetAlert1();
AlertState srvSnsTempGetAlert2();

#endif // SRV_SNS_TEMP_H