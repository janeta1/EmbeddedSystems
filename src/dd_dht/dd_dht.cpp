#include "dd_dht.h"
#include <Arduino.h>

static DHT dht(DHT_PIN, DHT_TYPE);
static float sTemperature = 0.0;
static float sHumidity = 0.0;
static uint32_t sLastReadTime = 0;

void ddDhtSetup() {
    dht.begin();
    sLastReadTime = millis();
}


// Throtlled: only read every DHT_READ_INTERVAL_MS milliseconds
// Safe to call every 20ms from acquisition task
void ddDhtLoop() {
    uint32_t currentTime = millis();
    if (currentTime - sLastReadTime < DHT_READ_INTERVAL_MS) return;

    sLastReadTime = currentTime;
    float t = dht.readTemperature();
    float h = dht.readHumidity();

    // Only update if both readings are valid
    if (!isnan(t)) {
        sTemperature = t;
    }

    if (!isnan(h)) {
        sHumidity = h;
    }
}

float ddDhtGetTemperature() {
    return sTemperature;
}

float ddDhtGetHumidity() {
    return sHumidity;
}