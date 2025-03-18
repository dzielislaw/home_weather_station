//
// Created by HP on 21.08.2024.
//
#include "units.h"
void convertCelsiusToFahrenheit(float* temp)
{
    *temp = *temp * 1.8f + 32.0f;
}

void convertFahrenheitToCelsius(float* temp)
{
    *temp = (*temp - 32.0f) / 1.8f;
}

const char getTemperatureUnitSymbol(enum temperatureUnit unit)
{
    if(unit == CELSIUS)
    {
        return 'C';
    }
    if(unit == FAHRENHEIT)
    {
        return 'F';
    }
}