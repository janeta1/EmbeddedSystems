#include "srv_sns_temp.h"
#include "dd_potentiometer/dd_potentiometer.h"
#include "dd_ds18b20/dd_ds18b20.h"
#include "dd_ntc/dd_ntc.h"
#include "dd_dht/dd_dht.h"
#include <Arduino_FreeRTOS.h>
#include <Arduino.h>
#include <semphr.h>

static SemaphoreHandle_t s_mutex = NULL;

// // Sensor 1 - Potentiometer
// static int sPotRaw = 0;
// static int sPotVoltage = 0;
// static float sPotTemperature = 0.0;
// static AlertState sAlert1 = {false, false, 0};

// Sensor 1 - NTC
static int sNtcRaw = 0;
static int sNtcVoltage = 0;
static float sNtcTemperature = 0.0;
static AlertState sAlert1 = {false, false, 0};

// // Sensor 2 - DS18B20
// static float sDsTemperature = 0.0;
// static AlertState sAlert2 = {false, false, 0};

// Sensor 2 - DHT22
static float sDhtTemperature = 0.0;
static float sDhtHumidity = 0.0;
static AlertState sAlert2 = {false, false, 0};

// Helper: hysteresis + debounce
static void updateAlert(AlertState *alert, float value, int thresholdHigh, int thresholdLow, int debounceLimit) {
    bool newPending;
    if (value >= thresholdHigh) {
        newPending = true;
    } else if (value <= thresholdLow) {
        newPending = false;
    } else {
        newPending = alert->alertPending;
    }

    if (newPending == alert->alertPending) {
        if (alert->debounceCounter < debounceLimit) {
            alert->debounceCounter++;
        }
    } else {
        alert->alertPending    = newPending;
        alert->debounceCounter = 1;
    }

    if (alert->debounceCounter >= debounceLimit) {
        alert->alertActive = alert->alertPending;
    }
}

void srvSnsTempSetup() {
    s_mutex = xSemaphoreCreateMutex();
    // ddPotentiometerSetup();
    // ddDs18b20Setup();
    ddNtcSetup();
    ddDhtSetup();
}

// For acquisition task: read sensors and update shared data
void srvSnsTempAcquire() {
    ddNtcLoop();
    ddDhtLoop();

    int ntcRaw = ddNtcGetRaw();
    int ntcVoltage = ddNtcGetVoltage();
    float ntcTemp = ddNtcGetCelsius();
    float dhtTemp = ddDhtGetTemperature();
    float dhtHumidity = ddDhtGetHumidity();

    // Update shared data with mutex protection
    if (xSemaphoreTake(s_mutex, portMAX_DELAY) == pdTRUE) {
        sNtcRaw = ntcRaw;
        sNtcVoltage = ntcVoltage;
        sNtcTemperature = ntcTemp;
        sDhtTemperature = dhtTemp;
        sDhtHumidity = dhtHumidity;
        xSemaphoreGive(s_mutex);
    }
}

// For conditioning task: process alert logic based on latest sensor data
void srvSnsTempProcess() {
    if (xSemaphoreTake(s_mutex, portMAX_DELAY) == pdTRUE) {
        updateAlert(&sAlert1, sNtcTemperature,
                    NTC_THRESHOLD_HIGH, NTC_THRESHOLD_LOW, NTC_DEBOUNCE);
        updateAlert(&sAlert2, sDhtTemperature,
                    DHT_THRESHOLD_HIGH, DHT_THRESHOLD_LOW, DHT_DEBOUNCE);
        xSemaphoreGive(s_mutex);
    }
}

// Getters
// int srvSnsTempGetPotRaw() {
//     int value = 0;
//     if (xSemaphoreTake(s_mutex, portMAX_DELAY) == pdTRUE) {
//         value = sPotRaw;
//         xSemaphoreGive(s_mutex);
//     }
//     return value;
// }

// int srvSnsTempGetPotVoltage() {
//     int value = 0;
//     if (xSemaphoreTake(s_mutex, portMAX_DELAY) == pdTRUE) {
//         value = sPotVoltage;
//         xSemaphoreGive(s_mutex);
//     }
//     return value;
// }

// float srvSnsTempGetPotCelsius() {
//     float value = 0.0;
//     if (xSemaphoreTake(s_mutex, portMAX_DELAY) == pdTRUE) {
//         value = sPotTemperature;
//         xSemaphoreGive(s_mutex);
//     }
//     return value;
// }

int srvSnsTempGetNtcRaw() {
    int value = 0;
    if (xSemaphoreTake(s_mutex, portMAX_DELAY) == pdTRUE) {
        value = sNtcRaw;
        xSemaphoreGive(s_mutex);
    }
    return value;
}

int srvSnsTempGetNtcVoltage() {
    int value = 0;
    if (xSemaphoreTake(s_mutex, portMAX_DELAY) == pdTRUE) {
        value = sNtcVoltage;
        xSemaphoreGive(s_mutex);
    }
    return value;
}

float srvSnsTempGetNtcCelsius() {
    float value = 0.0;
    if (xSemaphoreTake(s_mutex, portMAX_DELAY) == pdTRUE) {
        value = sNtcTemperature;
        xSemaphoreGive(s_mutex);
    }
    return value;
}

float srvSnsTempGetDhtTemperature() {
    float v = 0.0;
    if (xSemaphoreTake(s_mutex, portMAX_DELAY) == pdTRUE) {
        v = sDhtTemperature;
        xSemaphoreGive(s_mutex);
    }
    return v;
}

float srvSnsTempGetDhtHumidity() {
    float v = 0.0;
    if (xSemaphoreTake(s_mutex, portMAX_DELAY) == pdTRUE) {
        v = sDhtHumidity;
        xSemaphoreGive(s_mutex);
    }
    return v;
}

AlertState srvSnsTempGetAlert1() {
    AlertState alert = {false, false, 0};
    if (xSemaphoreTake(s_mutex, portMAX_DELAY) == pdTRUE) {
        alert = sAlert1;
        xSemaphoreGive(s_mutex);
    }
    return alert;
}

AlertState srvSnsTempGetAlert2() {
    AlertState alert = {false, false, 0};
    if (xSemaphoreTake(s_mutex, portMAX_DELAY) == pdTRUE) {
        alert = sAlert2;
        xSemaphoreGive(s_mutex);
    }
    return alert;
}
