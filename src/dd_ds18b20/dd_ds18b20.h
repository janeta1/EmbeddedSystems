#ifndef DD_ANALOG_H
#define DD_ANALOG_H

#include <OneWire.h>
#include <DallasTemperature.h>

#define DS18B20_PIN             2
#define DS18B20_INTERVAL_MS     1000

void  ddDs18b20Setup();
void  ddDs18b20Loop();
int   ddDs18b20GetCelsius();

#endif // DD_ANALOG_H