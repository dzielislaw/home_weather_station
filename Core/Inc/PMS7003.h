//
// Created by HP on 19.02.2024.
//

#ifndef STACJA_POGODOWA_PMS7003_H
#define STACJA_POGODOWA_PMS7003_H

#include "usart.h"
#include <inttypes.h>

#define PMS7003_BAUDRATE 9600
#define PMS7003_START_CHAR_1 0x42
#define PMS7003_START_CHAR_2 0x4D

typedef enum{
    PMS7003_READ_PASSIVE_MODE = 0xE2,
    PMS7003_CHANGE_MODE = 0xE1,
    PMS7003_SET_SLEEP_MODE = 0xE4
}PMS7003_COMMAND;

typedef enum{
    PMS7003_ENTER_SLEEP = 0,
    PMS7003_WAKEUP = 1
}PMS7003_SLEEP_MODE;

typedef enum{
    PMS7003_PASSIVE_MODE = 0,
    PMS7003_ACTIVE_MODE = 1
}PMS7003_MODE;

typedef struct{
  char name[30];
  UART_HandleTypeDef* bus;
} PMS7003_HandleTypeDef;

void PMS7003_getResults(PMS7003_HandleTypeDef* hpms7003, uint16_t* pm1_0, uint16_t* pm2_5, uint16_t* pm10);
#endif //STACJA_POGODOWA_PMS7003_H
