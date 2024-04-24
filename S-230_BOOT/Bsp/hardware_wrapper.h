#ifndef __BSP_WRAPPER_H
#define __BSP_WRAPPER_H

#ifdef __cplusplus
extern "C"
{
#endif
#include "dwt_delay.h"
#include "main.h"
#include "bsp_gpio.h"

#define HARDWARE_GPIO_WRITE_PIN(port, pin, state) HAL_GPIO_WritePin(port, pin, (state) > 0 ? GPIO_PIN_SET : GPIO_PIN_RESET)
#define HARDWARE_GPIO_WRITE_PIN_H(port, pin) HAL_GPIO_WritePin(port, pin, GPIO_PIN_SET)
#define HARDWARE_GPIO_WRITE_PIN_L(port, pin) HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET)
#define HARDWARE_GPIO_READ_PIN(port, pin) HAL_GPIO_ReadPin(port, pin)
#define HARDWARE_GPIO_SET_INPUT(port, pin) BSP_GPIO_set_pin_as_input(port, pin)
#define HARDWARE_GPIO_SET_OUTPUT(port, pin) BSP_GPIO_set_pin_as_output(port, pin)


#define HARDWARE_HAL_DELAY_MS(ms) HAL_Delay(ms)
#define HARDWARE_HAL_DELAY_US(us) DWT_delay(us)
#define HARDWARE_GET_TICK() HAL_GetTick()


#ifdef __cplusplus
}
#endif
#endif
