#ifndef STACJA_POGODOWA_MEASUREMENTS_H
#define STACJA_POGODOWA_MEASUREMENTS_H

#include <stdint.h>
#include "units.h"

typedef struct
{
  float temp;
  float minTemp;
  float maxTemp;
  
  float auxTemp;
  
  float outdoorTemp;
  float minOutdoorTemp;
  float maxOutdoorTemp;
  
  float pressure;
  float minPressure;
  float maxPressure;
  
  float humidity;
  float minHumidity;
  float maxHumidity;
  
  uint16_t pm1_0;
  uint16_t minPm1_0;
  uint16_t maxPm1_0;
  
  uint16_t pm2_5;
  uint16_t minPm2_5;
  uint16_t maxPm2_5;

  uint16_t pm10;
  uint16_t minPm10;
  uint16_t maxPm10;
  
  enum temperatureUnit tempUnit;
}Measurements;

void convertMeasurementsToCelsius(Measurements* m);
void convertMeasurementsToFahrenheit(Measurements* m);

#endif