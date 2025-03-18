#include "DS18B20.h"
#include "OneWire.h"

void DS18B20_matchROM(DS18B20_HandleTypeDef* hds18b20)
{
    oneWireTransmitByte(hds18b20->bus, DS18B20_MATCH_ROM);
    uint8_t* romCode = hds18b20->idNumber;
    for(uint8_t i = 0; i < 8; i++)
    {
      oneWireTransmitByte(hds18b20->bus, *romCode);
      romCode++;
    } 
}

void DS18B20_convertT(DS18B20_HandleTypeDef* hds18b20)
{
    oneWireReset(hds18b20->bus);
    //DS18B20_matchROM(hds18b20);
    oneWireTransmitByte(hds18b20->bus, DS18B20_SKIP_ROM);
    oneWireTransmitByte(hds18b20->bus, DS18B20_CONVERT_T);
}

void DS18B20_readSketchpad(DS18B20_HandleTypeDef* hds18b20)
{
  oneWireReset(hds18b20->bus);
  //DS18B20_matchROM(hds18b20);
  oneWireTransmitByte(hds18b20->bus, DS18B20_SKIP_ROM);
  oneWireTransmitByte(hds18b20->bus, DS18B20_READ_SKETCHPAD);
  uint8_t* sketchpad = hds18b20->sketchpad;
  for(uint8_t i = 0; i < 9; i++)
  {
    oneWireReceiveByte(hds18b20->bus, sketchpad);
    sketchpad++;
  }
}

void DS18B20_getResult(DS18B20_HandleTypeDef* hds18b20, float* result, enum temperatureUnit unit)
{
  DS18B20_readSketchpad(hds18b20);
  
  int8_t sign = (hds18b20->sketchpad[1] >> 7) ? -1 : 1;
  uint8_t lsb = hds18b20->sketchpad[0];
  uint8_t msb = (hds18b20->sketchpad[1] << 5) >> 5;
  
  int16_t temperature = sign * msb << 8 | lsb;
  *result = (float) temperature / 16.0;
  if(unit == FAHRENHEIT)
  {
    convertCelsiusToFahrenheit(result);
  }
}