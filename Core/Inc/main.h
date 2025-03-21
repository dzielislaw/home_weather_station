/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "units.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
typedef struct
{
  float temp;
  float minTemp;
  float maxTemp;
  
  float outdoorTemp;
  float minOutdoorTemp;
  float maxOutdoorTemp;
  
  float pressure;
  float minPressure;
  float maxPressure;
  
  float humidity;
  float minHumidity;
  float maxHumidity;
  
  uint16_t pm1_0;
  uint16_t minPm1_0;
  uint16_t maxPm1_0;
  
  uint16_t pm2_5;
  uint16_t minPm2_5;
  uint16_t maxPm2_5;

  uint16_t pm10;
  uint16_t minPm10;
  uint16_t maxPm10;
  
  enum temperatureUnit tempUnit;
}Measurements;
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define MAX_INPUT_Pin GPIO_PIN_14
#define MAX_INPUT_GPIO_Port GPIOC
#define MAX_INPU_Pin GPIO_PIN_15
#define MAX_INPU_GPIO_Port GPIOC
#define MIN_INPUT_Pin GPIO_PIN_4
#define MIN_INPUT_GPIO_Port GPIOA
#define LCD_ENABLE_Pin GPIO_PIN_5
#define LCD_ENABLE_GPIO_Port GPIOA
#define LCD_D4_Pin GPIO_PIN_7
#define LCD_D4_GPIO_Port GPIOA
#define LCD_D6_Pin GPIO_PIN_7
#define LCD_D6_GPIO_Port GPIOC
#define LCD_D7_Pin GPIO_PIN_9
#define LCD_D7_GPIO_Port GPIOA
#define LCD_BRIGHT_MAX_Pin GPIO_PIN_10
#define LCD_BRIGHT_MAX_GPIO_Port GPIOA
#define LCD_BRIGHT_NORM_Pin GPIO_PIN_12
#define LCD_BRIGHT_NORM_GPIO_Port GPIOA
#define TEMP_UNIT_INPUT_Pin GPIO_PIN_15
#define TEMP_UNIT_INPUT_GPIO_Port GPIOA
#define LCD_D5_Pin GPIO_PIN_6
#define LCD_D5_GPIO_Port GPIOB
#define MAKS_INPUT_Pin GPIO_PIN_7
#define MAKS_INPUT_GPIO_Port GPIOB
#define LCD_RS_Pin GPIO_PIN_9
#define LCD_RS_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
