#ifndef STACJA_POGODOWA_UNITS_H
#define STACJA_POGODOWA_UNITS_H

enum temperatureUnit
{
    CELSIUS = 0,
    FAHRENHEIT = 1
};

const char getTemperatureUnitSymbol(enum temperatureUnit unit);
void convertCelsiusToFahrenheit(float* temp);
void convertFahrenheitToCelsius(float* temp);

#endif //STACJA_POGODOWA_UNITS_H