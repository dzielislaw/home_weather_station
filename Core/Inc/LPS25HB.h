//
// Created by HP on 09.02.2024.
//
#ifndef LPS25HB_H
#define LPS25HB_H

#include "i2c.h"
#include "units.h"

#define LPS25HB_I2C_ADDRESS 0xBA
#define LPS25HB_WHO_AM_I_VALUE 0xBD

typedef enum{
    LPS25HB_WHO_AM_I = 0x0F,
    LPS25HB_CTRL_REG1 = 0x20,
    LPS25HB_CTRL_REG2 = 0x21,
    LPS25HB_STATUS_REG = 0x27,
    LPS25HB_PRESS_OUT_XL = 0x28,
    LPS25HB_PRESS_OUT_L = 0x29,
    LPS25HB_PRESS_OUT_H = 0x2A,
    LPS25HB_TEMP_OUT_L = 0x2B,
    LPS25HB_TEMP_OUT_H = 0x2C
}LPS25HB_Register;

typedef struct{
  char name[30];
  I2C_HandleTypeDef* bus;
  float sensorAltitude;
} LPS25HB_HandleTypeDef;

int LPS25HB_init(LPS25HB_HandleTypeDef* hlps25hb);
void LPS25HB_performMeasurement(LPS25HB_HandleTypeDef* hlps25hb);
float LPS25HB_getTemp(LPS25HB_HandleTypeDef* hlps25hb, enum temperatureUnit unit);
float LPS25HB_getPressureIn_hPa(LPS25HB_HandleTypeDef* hlps25hb);
#endif