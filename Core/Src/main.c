/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "comp.h"
#include "dac.h"
#include "i2c.h"
#include "rtc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "exti.h"
#include "LPS25HB.h"
#include "SHT45.h"
#include "PMS7003.h"
#include "ILI9341.h"
#include "HD44780.h"
#include "RGB565.h"
#include "EPaper.h"
#include "OneWire.h"
#include "DS18B20.h"
#include "EEPROM_24AA01.h"
#include "units.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef union
{
  int16_t altitude;
  uint8_t memoryImage[2];
} altitudeUnion;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
 Measurements measurements;
 float tempFromSHT35;
  
bool isWarmingUp = true;
bool isFirstInterrupt = true;
bool shouldLightUpAlarmLED = false;
bool shouldUpdateEeprom = false;
bool shouldEnbleNormBrightness = false;
bool shoulEnable_maxBrightness = false;
bool shouldDelay = true;
bool isPM10Last = true;
altitudeUnion altitude;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void updateMinMaxF(float* value, float* min, float* max);
void updateMinMax(uint16_t* value, uint16_t* min, uint16_t* max);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void handleTimer3PeriodElapsed()
{
  if(isFirstInterrupt == true)
   {
     isFirstInterrupt = false;
   }
   else
   {
     HAL_TIM_Base_Stop_IT(&htim3);
     const char* communicate = "DEBUG: Gas sensor warmed up\r\n";
     HAL_UART_Transmit(&huart4, communicate, strlen(communicate), HAL_MAX_DELAY);
     HAL_GPIO_EXTI_Callback(GPIO_PIN_4);
   }
}

void handleTimer7PeriodElapsed()
{
  if(shouldLightUpAlarmLED == true)
   {
     HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
     shouldLightUpAlarmLED = false;
   }
   else
   {
     HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_2);
     shouldLightUpAlarmLED = true;
   }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
{
  if(htim == &htim3) handleTimer3PeriodElapsed();
  else if(htim == &htim7) handleTimer7PeriodElapsed();
}

void updateMinMaxF(float* value, float* min, float* max)
{
    if(*min > *value) *min = *value;
    if(*max < *value) *max = *value;
}

void updateMinMax(uint16_t* value, uint16_t* min, uint16_t* max)
{
    if(*min > *value) *min = *value;
    if(*max < *value) *max = *value;
}

void convertMeasurementsToCelsius()
{
    measurements.tempUnit = CELSIUS;
    convertFahrenheitToCelsius(&measurements.outdoorTemp);
    convertFahrenheitToCelsius(&tempFromSHT35);
    convertFahrenheitToCelsius(&measurements.temp);
    convertFahrenheitToCelsius(&measurements.minTemp);
    convertFahrenheitToCelsius(&measurements.maxTemp);
    convertFahrenheitToCelsius(&measurements.maxOutdoorTemp);
    convertFahrenheitToCelsius(&measurements.minOutdoorTemp);
}

void convertMeasurementsToFahrenheit()
{
  measurements.tempUnit = FAHRENHEIT;
  convertCelsiusToFahrenheit(&measurements.outdoorTemp);
  convertCelsiusToFahrenheit(&tempFromSHT35);
  convertCelsiusToFahrenheit(&measurements.temp);
  convertCelsiusToFahrenheit(&measurements.minTemp);
  convertCelsiusToFahrenheit(&measurements.maxTemp);
  convertCelsiusToFahrenheit(&measurements.maxOutdoorTemp);
  convertCelsiusToFahrenheit(&measurements.minOutdoorTemp);
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
  altitudeUnion tmp;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_UART4_Init();
  MX_I2C1_Init();
  MX_I2C3_Init();
  MX_RTC_Init();
  MX_UART5_Init();
  MX_DAC1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_USART3_UART_Init();
  MX_TIM4_Init();
  MX_TIM7_Init();
  MX_COMP4_Init();
  MX_TIM1_Init();
  MX_TIM20_Init();
  /* USER CODE BEGIN 2 */
  oneWire_HandleTypeDef* hOneWire = &huart3;
  oneWireReset(hOneWire);
  HAL_GPIO_EXTI_Callback(GPIO_PIN_13);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  
  //HAL_GPIO_WritePin(LCD_BRIGHT_NORM_GPIO_Port, LCD_BRIGHT_NORM_Pin, GPIO_PIN_RESET);
  
  char communicate[256];
  char unit[5];

  uint32_t brightnessState;
  measurements.tempUnit = CELSIUS;
  
  RTC_DateTypeDef date;
  RTC_TimeTypeDef time;
  
  EEPROM_24AA01_HandleTypeDef eeprom;
  strcpy(eeprom.name, "i2c eeprom");
  eeprom.bus = &hi2c3;
  
  SHT45_HandleTypeDef sht45_Sensor;
  strcpy(sht45_Sensor.name, "indoor humidity sensor");
  sht45_Sensor.bus = &hi2c3;
  
  LPS25HB_HandleTypeDef lps25hb_Sensor;
  strcpy(lps25hb_Sensor.name, "indoor pressure sensor");
  lps25hb_Sensor.bus = &hi2c1;
  lps25hb_Sensor.sensorAltitude = altitude.altitude;
  
  PMS7003_HandleTypeDef pms7003_Sensor;
  strcpy(pms7003_Sensor.name, "indoor air pollution sensor");
  pms7003_Sensor.bus = &huart5;

  DS18B20_HandleTypeDef ds18b20_Sensor;
  strcpy(ds18b20_Sensor.name, "outdoor temperature sensor");
  ds18b20_Sensor.bus = hOneWire;
  uint8_t romCode[] = {0x28, 0x7e, 0x4c, 0x14, 0x00, 0x00, 0x00, 0x6b};
  for(uint8_t i = 0 ; i < 8; i++)
  {
    ds18b20_Sensor.idNumber[i] = romCode[i];
  }
  
  
  HAL_TIM_Base_Start(&htim2);
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
  HAL_TIM_Base_Start_IT(&htim3);
  
  HAL_TIM_Encoder_Start(&htim1, TIM_CHANNEL_ALL);
  EEPROM_24AA01_ReadByte(&eeprom, 0x40, &altitude.memoryImage[0]);
  EEPROM_24AA01_ReadByte(&eeprom, 0x41, &altitude.memoryImage[1]);
  __HAL_TIM_SET_COUNTER(&htim1, altitude.altitude * 2);
   
   
  sprintf(communicate, "Weather station hello!\r\n");
  HAL_UART_Transmit(&huart4, communicate, strlen(communicate), HAL_MAX_DELAY);
  SHT45_init(&sht45_Sensor);
  sprintf(communicate, "SHT45 initialization... OK\r\n");
  HAL_UART_Transmit(&huart4, communicate, strlen(communicate), HAL_MAX_DELAY);
  
  if(LPS25HB_init(&lps25hb_Sensor) == -1)
  {
    sprintf(communicate, "LPS25HB pressure sensor initliazation... FAILED\r\n");
    HAL_UART_Transmit(&huart4, communicate, strlen(communicate), HAL_MAX_DELAY);
  }
  else
  {
    sprintf(communicate, "LPS25HB pressure sesnsor initialization... OK\r\n");
    HAL_UART_Transmit(&huart4, communicate, strlen(communicate), HAL_MAX_DELAY);
  }
  
  HD44780_Pin d4;
  d4.Port = LCD_D4_GPIO_Port;
  d4.Pin = LCD_D4_Pin;
  
  HD44780_Pin d5;
  d5.Port = LCD_D5_GPIO_Port;
  d5.Pin = LCD_D5_Pin;
  
  HD44780_Pin d6;
  d6.Port = LCD_D6_GPIO_Port;
  d6.Pin = LCD_D6_Pin;
  
  HD44780_Pin d7;
  d7.Port = LCD_D7_GPIO_Port;
  d7.Pin = LCD_D7_Pin;
  
  HD44780_Pin rs;
  rs.Port = LCD_RS_GPIO_Port;
  rs.Pin = LCD_RS_Pin;
  
  HD44780_Pin enable;
  enable.Port = LCD_ENABLE_GPIO_Port;
  enable.Pin = LCD_ENABLE_Pin;
  
  HD44780_HandleTypeDef hHD44780;
  hHD44780.D4 = d4;
  hHD44780.D5 = d5;
  hHD44780.D6 = d6;
  hHD44780.D7 = d7;
  hHD44780.RS = rs;
  hHD44780.Enable = enable;
  
  HD44780_Init(&hHD44780);
  
  sprintf(communicate, "HD44780 initialization (master only) DONE\r\n");
  HAL_UART_Transmit(&huart4, communicate, strlen(communicate), HAL_MAX_DELAY);
  HAL_GPIO_WritePin(LCD_BRIGHT_NORM_GPIO_Port, LCD_BRIGHT_NORM_Pin, GPIO_PIN_RESET);
  
  char lcdString[21];
  HAL_COMP_Start(&hcomp4);
  HAL_DAC_Start(&hdac1, DAC_CHANNEL_1);
   HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, 02570); // dobrane empirycznie
  while (1)
  {         
    brightnessState = HAL_COMP_GetOutputLevel(&hcomp4);
    //HAL_COMP_Stop(&hcomp4);
    //HAL_DAC_Stop(&hdac1, DAC_CHANNEL_1);
    PMS7003_getResults(&pms7003_Sensor, &measurements.pm1_0, &measurements.pm2_5, &measurements.pm10);
    updateMinMax(&measurements.pm1_0, &measurements.minPm1_0, &measurements.maxPm1_0);
      updateMinMax(&measurements.pm2_5, &measurements.minPm2_5, &measurements.maxPm2_5);
      updateMinMax(&measurements.pm10, &measurements.minPm10, &measurements.maxPm10);
      
      LPS25HB_performMeasurement(&lps25hb_Sensor);
      measurements.temp = LPS25HB_getTemp(&lps25hb_Sensor, measurements.tempUnit);
      updateMinMaxF(&measurements.temp, &measurements.minTemp, &measurements.maxTemp);

      altitude.altitude = (int16_t) (__HAL_TIM_GET_COUNTER(&htim1) / 2);
      if(altitude.altitude > 30000) 
      {
        altitude.altitude -= 32768;
      }
      lps25hb_Sensor.sensorAltitude = altitude.altitude;
      measurements.pressure = LPS25HB_getPressureIn_hPa(&lps25hb_Sensor);
      updateMinMaxF(&measurements.pressure, &measurements.minPressure, &measurements.maxPressure);
      
      DS18B20_convertT(&ds18b20_Sensor);
      HAL_Delay(750);
      DS18B20_getResult(&ds18b20_Sensor, &measurements.outdoorTemp, measurements.tempUnit);
      HAL_Delay(10);
      updateMinMaxF(&measurements.outdoorTemp, &measurements.minOutdoorTemp, &measurements.maxOutdoorTemp);
      
      HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);
      HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);

      SHT45_getResults(&sht45_Sensor, &tempFromSHT35, measurements.tempUnit, &measurements.humidity);
      updateMinMaxF(&measurements.humidity, &measurements.minHumidity, &measurements.maxHumidity);
      

      if(HAL_GPIO_ReadPin(GPIOA, TEMP_UNIT_INPUT_Pin) == GPIO_PIN_RESET && measurements.tempUnit == CELSIUS)
      {
        convertMeasurementsToFahrenheit();
      }
      else if (HAL_GPIO_ReadPin(GPIOA, TEMP_UNIT_INPUT_Pin) == GPIO_PIN_SET && measurements.tempUnit == FAHRENHEIT)
      {
        convertMeasurementsToCelsius();
      }
      sprintf(unit, "%c", getTemperatureUnitSymbol(measurements.tempUnit));
      sprintf(communicate, "Raport na: %02d-%02d-%4d %02d:%02d:%02d\r\n",  date.Date, date.Month, date.Year + 2000, time.Hours, time.Minutes, time.Seconds);
      HAL_UART_Transmit(&huart4, communicate, strlen(communicate), HAL_MAX_DELAY);
      if(brightnessState == COMP_OUTPUT_LEVEL_HIGH)
      {
        sprintf(communicate, "Jest jasno\r\n");
      }
      else
      {
        sprintf(communicate, "Jest ciemno\r\n");
      }
      
      HAL_UART_Transmit(&huart4, communicate, strlen(communicate), HAL_MAX_DELAY);
      sprintf(communicate, "Temperatura wynosi (LPS25HB) %.2f \xC2\xB0%s / (SHT35) %.2f \xC2\xB0%s\tmin: %.2f \xC2\xB0%s\tmax: %.2f \xC2\xB0%s\r\n", measurements.temp, unit, tempFromSHT35, unit, measurements.minTemp, unit, measurements.maxTemp, unit);
      HAL_UART_Transmit(&huart4, communicate, strlen(communicate), HAL_MAX_DELAY);
      sprintf(communicate, "Temperatura na zewnatrz wynosi %.2f \xC2\xB0%s\tmin: %.2f \xC2\xB0%s\tmax: %.2f \xC2\xB0%s\r\n", measurements.outdoorTemp, unit, measurements.minOutdoorTemp, unit, measurements.maxOutdoorTemp, unit);
      HAL_UART_Transmit(&huart4, communicate, strlen(communicate), HAL_MAX_DELAY);
      sprintf(communicate, "Cisnienie wynosi: %.2f hPa\tmin: %.2f hPa\tmax: %.2f hPa\r\n", measurements.pressure, measurements.minPressure, measurements.maxPressure);
      HAL_UART_Transmit(&huart4, communicate, strlen(communicate), HAL_MAX_DELAY);
      sprintf(communicate, "Wilgotnosc wzgledna wynosi %.1f RH\tmin: %.1f RH\tmax: %.1f RH\r\n", measurements.humidity, measurements.minHumidity, measurements.maxHumidity);
      HAL_UART_Transmit(&huart4, communicate, strlen(communicate), HAL_MAX_DELAY);
      sprintf(communicate, "Stezenie pylu zawieszonego PM 1.0 wynosi %hu ug/m3\tmin: %hu ug/m3\tmax: %hu ug/m3\r\nStezenie pylu zawieszonego PM 2.5 wynosi %hu ug/m3\tmin: %hu ug/m3\tmax: %hu ug/m3\r\nStezenie pylu zawieszonego PM 10 wynosi %hu ug/m3\tmin: %hu ug/m3\tmax: %hu ug/m3\r\n\r\n",
               measurements.pm1_0, measurements.minPm1_0, measurements.maxPm1_0, measurements.pm2_5, measurements.minPm2_5, measurements.maxPm2_5, measurements.pm10, measurements.minPm10, measurements.maxPm10);
      HAL_UART_Transmit(&huart4, communicate, strlen(communicate), HAL_MAX_DELAY);
      
      sprintf(communicate, "%.2f %.2f %.1f %hu %hu %hu \r\n", measurements.temp, measurements.pressure, measurements.humidity, measurements.pm1_0, measurements.pm2_5, measurements.pm10);
      HAL_UART_Transmit(&huart5, communicate, strlen(communicate), HAL_MAX_DELAY);
      
      if(shouldEnbleNormBrightness || shoulEnable_maxBrightness){
        clearDisplay(&hHD44780);
        HAL_Delay(5);
        if(shoulEnable_maxBrightness){
          HAL_GPIO_WritePin(LCD_BRIGHT_MAX_GPIO_Port, LCD_BRIGHT_MAX_Pin, GPIO_PIN_RESET);
          HD44780_SetDDRAMAdress(&hHD44780, 0);
          sprintf(lcdString, "ALARM, GAZY PALNE!");
          HD44780_DisplayString(&hHD44780, lcdString);
          shoulEnable_maxBrightness = false;
          while (shouldEnbleNormBrightness == false){};
          HAL_Delay(5);
        } 
         HAL_GPIO_WritePin(LCD_BRIGHT_NORM_GPIO_Port, LCD_BRIGHT_NORM_Pin, GPIO_PIN_RESET);
         shouldEnbleNormBrightness = false;
         clearDisplay(&hHD44780);
      }
      
      if(HAL_GPIO_ReadPin(MIN_INPUT_GPIO_Port, MIN_INPUT_Pin) == GPIO_PIN_RESET){
        measurements.pressure = measurements.minPressure;
        measurements.humidity = measurements.minHumidity;
        measurements.pm10 = measurements.minPm10;
        measurements.pm2_5 = measurements.minPm2_5;
        measurements.pm1_0 = measurements.minPm1_0;
        measurements.outdoorTemp = measurements.minOutdoorTemp;
        measurements.temp = measurements.minTemp;
      }
        else if(HAL_GPIO_ReadPin(MAKS_INPUT_GPIO_Port, MAKS_INPUT_Pin) == GPIO_PIN_RESET){
        measurements.pressure = measurements.maxPressure;
        measurements.humidity = measurements.maxHumidity;
        measurements.pm10 = measurements.maxPm10;
        measurements.pm2_5 = measurements.maxPm2_5;
        measurements.pm1_0 = measurements.maxPm1_0;
        measurements.outdoorTemp = measurements.maxOutdoorTemp;
        measurements.temp = measurements.maxTemp;
      }
      
      HD44780_SetDDRAMAdress(&hHD44780, 0);
      sprintf(lcdString, "%c%.1f\xDF%c %.1f\xDF%c", (tempFromSHT35 >= 0)? ' ' : '-', tempFromSHT35, getTemperatureUnitSymbol(measurements.tempUnit), measurements.outdoorTemp, getTemperatureUnitSymbol(measurements.tempUnit));
      HD44780_DisplayString(&hHD44780, lcdString);
      
      HD44780_SetDDRAMAdress(&hHD44780, 0x40);
      if(measurements.pressure < 1000){
        sprintf(lcdString, " %d hPa %d m.n.p.m.", (uint16_t) measurements.pressure, altitude.altitude);
      }
      else{
        sprintf(lcdString, "%d hPa %d m.n.p.m.", (uint16_t) measurements.pressure,  altitude.altitude);
      }
      HD44780_DisplayString(&hHD44780, lcdString);
      
      HD44780_SetDDRAMAdress(&hHD44780, 0x14);
      sprintf(lcdString, "PM1 %d ug/m3 %d%% RH", measurements.pm1_0, (uint16_t) measurements.humidity);
      HD44780_DisplayString(&hHD44780, lcdString);
      
      HD44780_SetDDRAMAdress(&hHD44780, 0x54);
      if(isPM10Last){
        sprintf(lcdString, "PM2.5 %d ug/m3", measurements.pm2_5);
      }
      else{
        sprintf(lcdString, "PM10 %d ug/m3", measurements.pm10);
      }
      HD44780_DisplayString(&hHD44780, lcdString); 
      isPM10Last = !isPM10Last;
      
      if(shouldUpdateEeprom)
      {
        shouldUpdateEeprom = false;
        EEPROM_24AA01_ReadByte(&eeprom, 0x40, &tmp.memoryImage[0]);
        EEPROM_24AA01_ReadByte(&eeprom, 0x41, &tmp.memoryImage[1]);
        if(tmp.altitude != altitude.altitude)
        {
          EEPROM_24AA01_WriteByte(&eeprom, 0x40, &altitude.memoryImage[0]);
          EEPROM_24AA01_WriteByte(&eeprom, 0x41, &altitude.memoryImage[1]);
          sprintf(communicate, "Altitude %d saved to eeprom\r\n", altitude.altitude);
          HAL_UART_Transmit(&huart4, communicate, strlen(communicate), HAL_MAX_DELAY);
        }
      }
      
      HAL_Delay(250);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV1;
  RCC_OscInitStruct.PLL.PLLN = 21;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
