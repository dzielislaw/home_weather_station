#include <stdbool.h>
#include <string.h>
#include "exti.h"
#include "usart.h"
#include "tim.h"
#include "main.h"

extern UART_HandleTypeDef huart4;
extern bool shouldUpdateEeprom;
extern bool shouldLightUpAlarmLED;
extern Measurements measurements;
extern bool shouldEnbleNormBrightness;
extern bool shoulEnable_maxBrightness;
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(GPIO_Pin == GPIO_PIN_3) handleDataSaveInterrupt();
  else if(GPIO_Pin == GPIO_PIN_4) handleGasAlarmInterrupt();
  else if(GPIO_Pin == GPIO_PIN_13) handleMeasurementsResetInterrupt();
}

inline void handleDataSaveInterrupt()
{
  shouldUpdateEeprom = true;
  const char* communicate = "DEBUG: Save data button push recorded\r\n";
  HAL_UART_Transmit(&huart4, communicate, strlen(communicate), HAL_MAX_DELAY);
}

void handleMeasurementsResetInterrupt()
{
  measurements.minHumidity = 100.0f;
  measurements.maxHumidity = 0.0f;
  measurements.minOutdoorTemp = measurements.minTemp = 100.0f;
  measurements.maxOutdoorTemp = measurements.maxTemp -150.0f;
  measurements.minPm1_0 = measurements.minPm2_5 = measurements.minPm10 = 0xFFFF;
  measurements.maxPm1_0 = measurements.maxPm2_5 = measurements.maxPm10 = 0;
  measurements.minPressure = 2000.0f;
  measurements.maxPressure = 0.0f;
  const char* communicate = "INFO: Measurements reset\r\n";
  HAL_UART_Transmit(&huart4, communicate, strlen(communicate), HAL_MAX_DELAY);
}

void handleGasAlarmActivationInterrupt()
{
  HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
  shouldLightUpAlarmLED = false;
  HAL_TIM_Base_Start_IT(&htim7);
  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
  HAL_GPIO_WritePin(LCD_BRIGHT_NORM_GPIO_Port, LCD_BRIGHT_NORM_Pin, GPIO_PIN_SET);
  shouldEnbleNormBrightness = false;
  shoulEnable_maxBrightness = true;
  //HAL_Delay(5);
  //HAL_GPIO_WritePin(LCD_BRIGHT_MAX_GPIO_Port, LCD_BRIGHT_MAX_Pin, GPIO_PIN_RESET);
      
  const char* communicate = "CRITICAL: -----ALARM! FLAMMABLE GASES DETECTED!--------\r\n";
  HAL_UART_Transmit(&huart4, communicate, strlen(communicate), HAL_MAX_DELAY);
  
}
void handleGasAlarmDeactivationInterrupt()
{
   HAL_TIM_Base_Stop_IT(&htim7);
   HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_2);
   HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
   HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_1); //buzzer
   HAL_GPIO_WritePin(LCD_BRIGHT_MAX_GPIO_Port, LCD_BRIGHT_MAX_Pin, GPIO_PIN_SET);
   shoulEnable_maxBrightness = false;
   shouldEnbleNormBrightness = true;
   //HAL_Delay(5);
   //HAL_GPIO_WritePin(LCD_BRIGHT_NORM_GPIO_Port, LCD_BRIGHT_NORM_Pin, GPIO_PIN_RESET);
   const char* communicate = "INFO: poziom gazow palnych w normie\r\n";
   HAL_UART_Transmit(&huart4, communicate, strlen(communicate), HAL_MAX_DELAY);
}

inline void handleGasAlarmInterrupt()
{
  if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_4) == GPIO_PIN_RESET) handleGasAlarmActivationInterrupt();
  else handleGasAlarmDeactivationInterrupt();
}