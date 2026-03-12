#ifndef DD_DTH_H
#define DD_DTH_H

#include <DHT.h>

#define DHT_PIN 18
#define DHT_TYPE DHT11

#define DHT_READ_INTERVAL_MS 2000

// Function prototypes
void ddDhtSetup();
void ddDhtLoop();
float ddDhtGetTemperature();
float ddDhtGetHumidity();

#endif // DD_DTH_H