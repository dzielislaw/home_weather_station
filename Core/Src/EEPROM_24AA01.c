//
// Created by HP on 23.10.2024.
//
#include "EEPROM_24AA01.h"
#include "i2c.h"

uint8_t EEPROM_24AA01_ReadByte(EEPROM_24AA01_HandleTypeDef* h24aa01, uint8_t address, uint8_t* result)
{
    HAL_I2C_Mem_Read(h24aa01->bus, EEPROM_24AA01_ADDRESS, address, 1, result, sizeof(result), HAL_MAX_DELAY);
}

uint8_t EEPROM_24AA01_WriteByte(EEPROM_24AA01_HandleTypeDef* h24aa01, uint8_t address, uint8_t* value)
{
  HAL_I2C_Mem_Write(h24aa01->bus, EEPROM_24AA01_ADDRESS, address, 1, value, sizeof(value), HAL_MAX_DELAY);
  HAL_Delay(5);
}