//
// Created by HP on 12.08.2024.
//

#include <inttypes.h>
#include "usart.h"
#include "OneWire.h"
#include <stdio.h>

void setBaudRate(UART_HandleTypeDef* huart, uint32_t baudrate)
{
  huart3.Instance = USART3;
  huart3.Init.BaudRate = baudrate;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_RXOVERRUNDISABLE_INIT;
  huart3.AdvancedInit.OverrunDisable = UART_ADVFEATURE_OVERRUN_DISABLE;
  if (HAL_HalfDuplex_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart3, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart3, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
}

HAL_StatusTypeDef oneWireReset(oneWire_HandleTypeDef* hOneWire)
{
    uint8_t data = 0xF0;
    setBaudRate(hOneWire, 9600);
    HAL_UART_Transmit(hOneWire, &data, sizeof(data), HAL_MAX_DELAY);
    HAL_UART_Receive(hOneWire, &data, sizeof(data), HAL_MAX_DELAY);
    setBaudRate(hOneWire, 115200);
    if (data != 0xF0)
    {
        return HAL_OK;
    }
    return HAL_ERROR;
}

uint8_t oneWireReceiveBit(UART_HandleTypeDef* huart)
{
    uint8_t data = 0xFF;
    HAL_UART_Transmit(huart, &data, sizeof(data), HAL_MAX_DELAY);
    HAL_UART_Receive(huart, &data, sizeof(data), HAL_MAX_DELAY);
    return data & 0x01;
}

void oneWireTransmitBit(UART_HandleTypeDef* huart, uint8_t bit)
{
    if(bit != 0x00)
    {
        bit = 0xFF;
    }
    HAL_UART_Transmit(huart, &bit, sizeof(bit), HAL_MAX_DELAY);
}

void oneWireReceiveByte(oneWire_HandleTypeDef* hOneWire, uint8_t* byte)
{
    *byte = 0x00;
    for(uint8_t i = 0; i < 8; i++)
    {
        *byte >>= 1;
        if(oneWireReceiveBit(hOneWire))
        {
            *byte |= 0x80;
        }
    }
}

void oneWireTransmitByte(oneWire_HandleTypeDef* hOneWire, uint8_t byte)
{
    for(uint8_t i = 0; i < 8; i++)
    {
        oneWireTransmitBit(hOneWire, byte & 0x01);
        byte >>= 1;
    }
}
