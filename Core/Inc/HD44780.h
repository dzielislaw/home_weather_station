#include "gpio.h"

#ifndef STACJA_POGODOWA_HD44780_H
#define STACJA_POGODOWA_HD44780_H

#define SET GPIO_PIN_SET
#define RESET GPIO_PIN_RESET

#define HD44780_FIRST_LINE_ADDR 0x00;
#define HD44780_SECOND_LINE_ADDR 0x40;
#define HD44780_THIRD_LINE_ADDR 0x14;
#define HD44780_FOURTH_LINE_ADDR 0x54;

typedef struct
{
  GPIO_TypeDef* Port;
  uint16_t Pin;
}HD44780_Pin;


typedef struct
{
  HD44780_Pin D4;
  HD44780_Pin D5;
  HD44780_Pin D6;
  HD44780_Pin D7;
  HD44780_Pin Enable;
  HD44780_Pin RS;
}HD44780_HandleTypeDef;

void HD44780_Init(HD44780_HandleTypeDef* hHD44780);
void HD44780_DisplayCharacter(HD44780_HandleTypeDef* hHD44780, uint8_t character);
void HD44780_DisplayString(HD44780_HandleTypeDef* hHD44780, const char* str);
void HD44780_SetDDRAMAdress(HD44780_HandleTypeDef* hHD44780, uint8_t address);
void clearDisplay(HD44780_HandleTypeDef* hHD44780);

#endif