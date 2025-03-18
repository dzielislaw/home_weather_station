//
// Created by HP on 12.08.2024.
//

#ifndef STACJA_POGODOWA_ONEWIRE_H
#define STACJA_POGODOWA_ONEWIRE_H

#include <inttypes.h>
#include "usart.h"

typedef UART_HandleTypeDef oneWire_HandleTypeDef;

HAL_StatusTypeDef oneWireReset(oneWire_HandleTypeDef* hOneWire);
void oneWireTransmitByte(oneWire_HandleTypeDef* hOneWire, uint8_t byte);
void oneWireReceiveByte(oneWire_HandleTypeDef* hOneWire, uint8_t* byte);

#endif //STACJA_POGODOWA_ONEWIRE_H
