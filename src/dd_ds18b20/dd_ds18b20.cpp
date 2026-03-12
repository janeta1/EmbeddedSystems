#include "dd_ds18b20.h"
#include <Arduino.h>
#include <Arduino_FreeRTOS.h>

static OneWire           s_oneWire(DS18B20_PIN);
static DallasTemperature s_sensors(&s_oneWire);
static int               s_celsius  = 0;
static TickType_t        s_lastTick = 0;

#define DS18B20_INTERVAL_TICKS pdMS_TO_TICKS(DS18B20_INTERVAL_MS)

void ddDs18b20Setup() {
    s_sensors.begin();
    s_lastTick = xTaskGetTickCount();
}

void ddDs18b20Loop() {
    TickType_t now = xTaskGetTickCount();
    if ((now - s_lastTick) < DS18B20_INTERVAL_TICKS) return;
    s_lastTick = now;

    s_sensors.requestTemperatures();
    float t = s_sensors.getTempCByIndex(0);

    if (t != DEVICE_DISCONNECTED_C) {
        s_celsius = (int)t;
    }
}

int ddDs18b20GetCelsius() {
    return s_celsius;
}