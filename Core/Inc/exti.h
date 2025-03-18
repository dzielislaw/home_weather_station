#ifndef STACJA_POGODOWA_EXTI_H
#define STACJA_POGODOWA_EXTI_H

#include <stdint.h>

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
inline void handleDataSaveInterrupt();
void handleMeasurementsResetInterrupt();
void handleGasAlarmActivationInterrupt();
void handleGasAlarmDeactivationInterrupt();
inline void handleGasAlarmInterrupt();

#endif