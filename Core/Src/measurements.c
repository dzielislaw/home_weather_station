#include "measurements.h"

void convertMeasurementsToCelsius(Measurements* measurements)
{
    measurements->tempUnit = CELSIUS;
    convertFahrenheitToCelsius(&measurements->outdoorTemp);
    convertFahrenheitToCelsius(&measurements->auxTemp);
    convertFahrenheitToCelsius(&measurements->temp);
    convertFahrenheitToCelsius(&measurements->minTemp);
    convertFahrenheitToCelsius(&measurements->maxTemp);
    convertFahrenheitToCelsius(&measurements->maxOutdoorTemp);
    convertFahrenheitToCelsius(&measurements->minOutdoorTemp);
}
void convertMeasurementsToFahrenheit(Measurements* measurements)
{
  measurements->tempUnit = FAHRENHEIT;
  convertCelsiusToFahrenheit(&measurements->outdoorTemp);
  convertCelsiusToFahrenheit(&measurements->auxTemp);
  convertCelsiusToFahrenheit(&measurements->temp);
  convertCelsiusToFahrenheit(&measurements->minTemp);
  convertCelsiusToFahrenheit(&measurements->maxTemp);
  convertCelsiusToFahrenheit(&measurements->maxOutdoorTemp);
  convertCelsiusToFahrenheit(&measurements->minOutdoorTemp);
}