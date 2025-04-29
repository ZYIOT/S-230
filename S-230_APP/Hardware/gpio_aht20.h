#ifndef __HARDWARE_GPIO_AHT20_H
#define __HARDWARE_GPIO_AHT20_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "gpio_iic.h"
#include "hardware_wrapper.h"

    typedef struct
    {
        GPIO_IIC_TypeDef *IIC;
    } GPIO_AHT20_TypeDef;

    int GPIO_IIC_AHT20_init(GPIO_AHT20_TypeDef *AHT20x);
    int GPIO_IIC_AHT20_trigger(GPIO_AHT20_TypeDef *AHT20x);
    int GPIO_IIC_AHT20_reset(GPIO_AHT20_TypeDef *AHT20x);
    int GPIO_IIC_AHT20_read(GPIO_AHT20_TypeDef *AHT20x, float *temperature, float *humidity);

#ifdef __cplusplus
}
#endif

#endif
