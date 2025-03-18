//
// Created by HP on 09.02.2024.
//
#include <stdbool.h>
#include "LPS25HB.h"
#include "main.h"
#include "i2c.h"
#include <math.h>

uint8_t LPS25HB_ReadRegister(LPS25HB_HandleTypeDef* hlps25hb, LPS25HB_Register reg)
{
    uint8_t regValue;
    HAL_I2C_Mem_Read(hlps25hb->bus, LPS25HB_I2C_ADDRESS, reg, 1, &regValue, sizeof(regValue), HAL_MAX_DELAY);
    return regValue;
}

void LPS25HB_WriteRegister(LPS25HB_HandleTypeDef* hlps25hb, LPS25HB_Register reg, uint8_t value)
{
    HAL_I2C_Mem_Write(hlps25hb->bus, LPS25HB_I2C_ADDRESS, reg, 1, &value, sizeof(value), HAL_MAX_DELAY);
}

bool isConnected(LPS25HB_HandleTypeDef* hlps25hb)
{
    return (LPS25HB_ReadRegister(hlps25hb, LPS25HB_WHO_AM_I) == LPS25HB_WHO_AM_I_VALUE);
}

void LPS25HB_performMeasurement(LPS25HB_HandleTypeDef* hlps25hb)
{
    uint8_t regValue = LPS25HB_ReadRegister(hlps25hb, LPS25HB_CTRL_REG2);
    regValue |= 0x01; // trigger measurement
    LPS25HB_WriteRegister(hlps25hb, LPS25HB_CTRL_REG2, regValue);
    HAL_Delay(1000);
}

int16_t getTemperature(LPS25HB_HandleTypeDef* hlps25hb){
    int16_t temperature = LPS25HB_ReadRegister(hlps25hb, LPS25HB_TEMP_OUT_H) << 8;
    temperature |= LPS25HB_ReadRegister(hlps25hb, LPS25HB_TEMP_OUT_L);
    return temperature;
}

uint32_t getPressure(LPS25HB_HandleTypeDef* hlps25hb){
    uint32_t pressure = LPS25HB_ReadRegister(hlps25hb, LPS25HB_PRESS_OUT_H) << 16;
    pressure |= LPS25HB_ReadRegister(hlps25hb, LPS25HB_PRESS_OUT_L) << 8;
    pressure |= LPS25HB_ReadRegister(hlps25hb, LPS25HB_PRESS_OUT_XL);
    return pressure;
}

void wakeUp(LPS25HB_HandleTypeDef* hlps25hb)
{
    uint8_t regValue = LPS25HB_ReadRegister(hlps25hb, LPS25HB_CTRL_REG1);
    regValue |= 1 << 7; // Wake up sensor
    LPS25HB_WriteRegister(hlps25hb, LPS25HB_CTRL_REG1, regValue);
}

float LPS25HB_getTemp(LPS25HB_HandleTypeDef* hlps25hb, enum temperatureUnit unit)
{
    int16_t measurement = getTemperature(hlps25hb);
    if(unit == CELSIUS)
    {
        return measurement / 480.0f + 42.5f;
    }
    if(unit == FAHRENHEIT)
    {
        return (measurement / 480.0f + 42.5f) * 1.8f + 32.0f;
    }
}

float convertCelsiusToKelvin(float temp){
    return temp + 273.15f;
}

float LPS25HB_getPressureIn_hPa(LPS25HB_HandleTypeDef* hlps25hb){
    enum temperatureUnit unit = CELSIUS;
    float temperature = LPS25HB_getTemp(hlps25hb, unit);
    temperature = convertCelsiusToKelvin(temperature);

    uint32_t measurement = getPressure(hlps25hb);
    float pressure = (float) measurement;
    pressure *= exp(0.034162608734308 * hlps25hb->sensorAltitude / temperature);
    pressure /= 4096.0f; // 1 hPa = 4096 LSB
    return pressure;
}

int LPS25HB_init(LPS25HB_HandleTypeDef* hlps25hb)
{
    wakeUp(hlps25hb);
    if(!isConnected(hlps25hb))
    {
        return -1;
    }
    return 0;
}
