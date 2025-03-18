//
// Created by HP on 17.10.2024.
//
#ifndef STACJA_POGODOWA_SHT45_H
#define STACJA_POGODOWA_SHT45_H

#include "i2c.h"
#include "units.h"

#define SHT45_I2C_ADDRESS 0x44

typedef enum {
    SHT45_MEASURE_HIGH_PRECISION = 0xFD,
    SHT45_MEASURE_MEDIUM_PRECISION = 0xF6,
    SHT45_MEASURE_LOW_PRECISION = 0xE0,
    SHT45_READ_SERIAL_NUMBER = 0x89,
    SHT45_SOFT_RESET = 0x94,
    
}SHT45_COMMAND;

typedef struct {
  char name[30];
  I2C_HandleTypeDef* bus;
} SHT45_HandleTypeDef;

void SHT45_init(SHT45_HandleTypeDef* hsht45);
void SHT45_getResults(SHT45_HandleTypeDef* hsht45, float* temp, enum temperatureUnit tempUnit, float* humidity);

#endif //STACJA_POGODOWA_SHT35_H
