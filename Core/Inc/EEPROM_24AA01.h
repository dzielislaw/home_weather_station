//
// Created by HP on 23.10.2024.
//

#ifndef STACJA_POGODOWA_24AA01_H
#define STACJA_POGODOWA_24AA01_H

#include "i2c.h"

#define EEPROM_24AA01_ADDRESS 0xA0

typedef struct {
    char name[30];
    I2C_HandleTypeDef* bus;
} EEPROM_24AA01_HandleTypeDef;

uint8_t EEPROM_24AA01_ReadByte(EEPROM_24AA01_HandleTypeDef* h24aa01, uint8_t address, uint8_t* result);
uint8_t EEPROM_24AA01_WriteByte(EEPROM_24AA01_HandleTypeDef* h24aa01, uint8_t address, uint8_t* value);

#endif //STACJA_POGODOWA_24AA01_H
