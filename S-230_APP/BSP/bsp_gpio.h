#ifndef __BSP_GPIO_H
#define __BSP_GPIO_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "gpio.h"

    void BSP_GPIO_set_pin_as_input(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
    void BSP_GPIO_set_pin_as_output(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
    void BSP_GPIO_set_pin_as_analog(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
    void BSP_GPIO_set_pin_as_alternate(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);

#ifdef __cplusplus
}
#endif

#endif
