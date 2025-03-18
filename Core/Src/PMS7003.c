//
// Created by HP on 20.02.2024.
//
#include "PMS7003.h"
#include "main.h"
#include "usart.h"

void PMS7003_getResults(PMS7003_HandleTypeDef* hpms7003, uint16_t* pm1_0, uint16_t* pm2_5, uint16_t* pm10){
    uint8_t byte = 0x00;
    int n = 0;
    do {
        n++;
        HAL_UART_Receive(hpms7003->bus, &byte, 1, 150);
        if(n > 100) return;
    } while (byte != PMS7003_START_CHAR_1);
    HAL_UART_Receive(hpms7003->bus, &byte, 1, HAL_MAX_DELAY);
    if(byte != PMS7003_START_CHAR_2){
        return;
    }

    static uint8_t frame[30];
    HAL_UART_Receive(hpms7003->bus, frame, 10, HAL_MAX_DELAY);

    *pm1_0 = frame[2] << 8 | frame[3];
    *pm2_5 = frame[4] << 8 | frame[5];
    *pm10 = frame[6] << 8 | frame[7];
}