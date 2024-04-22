#ifndef __BSP_MCU_H
#define __BSP_MCU_H

#ifdef __cplusplus
extern "C"
{
#endif
#include "main.h"

#define BSP_MCU_init()

int BSP_MCU_temperature(float * t);
int BSP_MCU_reset(void);
#define MCU_LED_ON()    HAL_GPIO_WritePin(MCU_STATE_GPIO_Port, MCU_STATE_Pin, GPIO_PIN_SET);
#define MCU_LED_OFF()   HAL_GPIO_WritePin(MCU_STATE_GPIO_Port, MCU_STATE_Pin, GPIO_PIN_RESET);

#ifdef __cplusplus
}
#endif

#endif
