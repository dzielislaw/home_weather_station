//
// Created by HP on 04.09.2024.
//
#include "HD44780.h"
#include <stdbool.h>

bool isBusy(HD44780_HandleTypeDef* hHD44780);
void sendByte(HD44780_HandleTypeDef* hHD44780, uint8_t byte);
void HD44780_SetDDRAMAdress(HD44780_HandleTypeDef* hHD44780, uint8_t address);

void writeHD44780_Pin(HD44780_Pin Pin, GPIO_PinState state)
{
    HAL_GPIO_WritePin(Pin.Port, Pin.Pin, state);
}

inline GPIO_PinState readHD44780_Pin(HD44780_Pin Pin)
{
    return HAL_GPIO_ReadPin(Pin.Port, Pin.Pin);
}

void enable(HD44780_HandleTypeDef* hHD44780)
{
    writeHD44780_Pin(hHD44780->Enable, SET);
}

void disable(HD44780_HandleTypeDef* hHD44780)
{
    writeHD44780_Pin(hHD44780->Enable, RESET);
}

inline void selectDataRegister(HD44780_HandleTypeDef* hHD44780)
{
    writeHD44780_Pin(hHD44780->RS, SET);
}

void selectInstructionRegister(HD44780_HandleTypeDef* hHD44780)
{
    writeHD44780_Pin(hHD44780->RS, RESET);
}


void sendByte(HD44780_HandleTypeDef* hHD44780, uint8_t byte)
{
  //(byte >> 5) ? selectInstructionRegister(hHD44780) : selectDataRegister(hHD44780);
  //(byte & 0x10) ? selectRead(hHD44780) : selectWrite(hHD44780);
  
  //pierwsze 4 bity
  (byte >> 7) ? writeHD44780_Pin(hHD44780->D7, SET) : writeHD44780_Pin(hHD44780->D7, RESET);
  (byte & 0x40) ? writeHD44780_Pin(hHD44780->D6, SET) : writeHD44780_Pin(hHD44780->D6, RESET);
  (byte & 0x20) ? writeHD44780_Pin(hHD44780->D5, SET) : writeHD44780_Pin(hHD44780->D5, RESET);
  (byte & 0x10) ? writeHD44780_Pin(hHD44780->D4, SET) : writeHD44780_Pin(hHD44780->D4, RESET);
  enable(hHD44780);
  while(isBusy(hHD44780)){}
  disable(hHD44780);
  
  (byte & 0x08) ? writeHD44780_Pin((hHD44780->D7), SET) : writeHD44780_Pin(hHD44780->D7, RESET);
  (byte & 0x04) ? writeHD44780_Pin(hHD44780->D6, SET) : writeHD44780_Pin(hHD44780->D6, RESET);
  (byte & 0x02) ? writeHD44780_Pin(hHD44780->D5, SET) : writeHD44780_Pin(hHD44780->D5, RESET);
  (byte & 0x01) ? writeHD44780_Pin(hHD44780->D4, SET) : writeHD44780_Pin(hHD44780->D4, RESET);
  enable(hHD44780);
  while(isBusy(hHD44780)){}
  disable(hHD44780);
}

bool isBusy(HD44780_HandleTypeDef* hHD44780)
{
  //return readHD44780_Pin(hHD44780->D7) == GPIO_PIN_SET;  // D7 - busy flag
  HAL_Delay(1);
  return false;
}

void clearDisplay(HD44780_HandleTypeDef* hHD44780)
{
  selectInstructionRegister(hHD44780);
  sendByte(hHD44780, 0x01);
}

void returnHome(HD44780_HandleTypeDef* hHD44780)
{
  selectInstructionRegister(hHD44780);
  sendByte(hHD44780, 0x02);
}

void displayPowerOn(HD44780_HandleTypeDef* hHD44780, bool shouldCoursorBeDisplayed, bool shouldCoursorFlicker)
{
  uint8_t byte = 0x0C | ((uint8_t) shouldCoursorBeDisplayed) << 1 | (uint8_t) shouldCoursorFlicker;
  selectInstructionRegister(hHD44780);
  sendByte(hHD44780, byte);
}

void displayPowerOff(HD44780_HandleTypeDef* hHD44780)
{
  selectInstructionRegister(hHD44780);
  sendByte(hHD44780, 0x08);
}

void HD44780_DisplayCharacter(HD44780_HandleTypeDef* hHD44780, uint8_t character)
{
    selectDataRegister(hHD44780);
    sendByte(hHD44780, character);
}

void HD44780_Init(HD44780_HandleTypeDef* hHD44780)
{ 
  writeHD44780_Pin(hHD44780->D4, RESET);
  writeHD44780_Pin(hHD44780->D5, RESET);
  writeHD44780_Pin(hHD44780->D6, RESET);
  writeHD44780_Pin(hHD44780->D7, RESET);
  writeHD44780_Pin(hHD44780->RS, RESET);
  writeHD44780_Pin(hHD44780->Enable, RESET);
  HAL_Delay(1);
  
  // set 4 bit mode
  writeHD44780_Pin(hHD44780->D5, SET);
  enable(hHD44780);
  while(isBusy(hHD44780)){}
  disable(hHD44780);

   clearDisplay(hHD44780);
  returnHome(hHD44780);

   selectInstructionRegister(hHD44780);
  sendByte(hHD44780, 0x28); // 4 line
  enable(hHD44780);
  while(isBusy(hHD44780)){}
  disable(hHD44780);

 selectInstructionRegister(hHD44780);
  sendByte(hHD44780, 0x06); // increment 
  enable(hHD44780);
  while(isBusy(hHD44780)){}
  disable(hHD44780);
 
HD44780_SetDDRAMAdress(hHD44780, 0x00);
    
  displayPowerOn(hHD44780, false, false);
}

void HD44780_SetDDRAMAdress(HD44780_HandleTypeDef* hHD44780, uint8_t address)
{
  address |= 0x80;
  selectInstructionRegister(hHD44780);
  sendByte(hHD44780, address);
}

void HD44780_DisplayString(HD44780_HandleTypeDef* hHD44780, const char* str)
{
  uint8_t i = 0;
  while(*str != '\0')
  {
    HD44780_DisplayCharacter(hHD44780, *str);
    str++;
    i++;
  }
  while(i < 19) {
    HD44780_DisplayCharacter(hHD44780, 0x20);
    i++;
  }
  
}