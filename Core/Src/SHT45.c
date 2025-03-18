#include "SHT45.h"
#include "main.h"
#include <stdbool.h>
#include <inttypes.h>
#include <stdint.h>

void sendCommand(SHT45_HandleTypeDef* hsht45, SHT45_COMMAND command)
{
    uint8_t cmd = command;
    HAL_I2C_Master_Transmit(hsht45->bus, SHT45_I2C_ADDRESS << 1, &command, sizeof(command), HAL_MAX_DELAY);
}

void performMeasurement(SHT45_HandleTypeDef* hsht45)
{
    sendCommand(hsht45, SHT45_MEASURE_HIGH_PRECISION);
    HAL_Delay(9);
}

bool verifyCRC(uint16_t measurement, uint8_t crc)
{
    uint8_t calculatedCrc = 0;
    for(uint8_t i = 0; i < 8; i++){
        calculatedCrc ^= (measurement << i) >> 8;
    }
    return calculatedCrc == crc;
}

void SHT45_getResults(SHT45_HandleTypeDef* hsht45, float* temp, enum temperatureUnit tempUnit, float* humidity)
{
    performMeasurement(hsht45);

    static uint8_t results[6];
    HAL_I2C_Master_Receive(hsht45->bus, SHT45_I2C_ADDRESS << 1, results, 6, HAL_MAX_DELAY);

    uint16_t tempMeasurement = results[0] << 8 | results[1];
    uint16_t humidityMeasurement = results[3] << 8 | results[4];
    /*if(!verifyCRC(tempMeasurement, results[2]) || !verifyCRC(humidityMeasurement, results[5])){
        // TODO error handling
    }
*/
    if(tempUnit == CELSIUS)
    {
        *temp = -45.0f + 175.0f * tempMeasurement / 65545.0f;
    }
    else if(tempUnit == FAHRENHEIT)
    {
        *temp = -49.0f + 315.0f * tempMeasurement / 65545.0f;
    }
    *humidity = -6.0f + 125.0f * humidityMeasurement / 65545.0f;
}

void softReset(SHT45_HandleTypeDef* hsht45)
{
  sendCommand(hsht45, SHT45_SOFT_RESET);
  HAL_Delay(1);
}

void SHT45_init(SHT45_HandleTypeDef* hsht45)
{
  softReset(hsht45);
  //sendCommand(hsht45, SHT45_CLEAR_STATUS_REGISTER);
  HAL_Delay(1);
}