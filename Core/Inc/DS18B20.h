//
// Created by HP on 12.08.2024.
//

#ifndef STACJA_POGODOWA_DS18B20_H
#define STACJA_POGODOWA_DS18B20_H

#include <inttypes.h>
#include "OneWire.h"
#include "units.h"

typedef enum {
    DS18B20_SEARCH_ROM = 0xF0,
    DS18B20_READ_ROM = 0x33,
    DS18B20_MATCH_ROM = 0x55,
    DS18B20_SKIP_ROM = 0xCC,
    DS18B20_ALARM_SEARCH = 0xEC,
    DS18B20_CONVERT_T = 0x44,
    DS18B20_WRITE_SKETCHPAD = 0x4E,
    DS18B20_READ_SKETCHPAD = 0xBE,
    DS18B20_COPY_SKETCHPAD = 0x48,
    DS18B20_RECALL_E2 = 0xB8,
    DS18B20_READ_POWER_SUPPLY = 0xB4
}DS18B20_COMMAND;

typedef struct{
    char name[30];
    oneWire_HandleTypeDef* bus;
    uint8_t sketchpad[9];
    uint8_t idNumber[8];
}DS18B20_HandleTypeDef;

void DS18B20_convertT(DS18B20_HandleTypeDef* hds18b20);
void DS18B20_getResult(DS18B20_HandleTypeDef* hds18b20, float* result, enum temperatureUnit unit);

#endif //STACJA_POGODOWA_DS18B20_H
