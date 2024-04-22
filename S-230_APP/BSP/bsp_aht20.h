#ifndef __BSP_AHT20_H
#define __BSP_AHT20_H

#ifdef __cplusplus
extern "C"
{
#endif
#include "gpio_aht20.h"

    extern GPIO_AHT20_TypeDef gpio_aht20;

#define BSP_AHT20_init() GPIO_IIC_AHT20_init(&gpio_aht20)
#define BSP_AHT20_trigger() GPIO_IIC_AHT20_trigger(&gpio_aht20)
#define BSP_AHT20_reset() GPIO_IIC_AHT20_reset(&gpio_aht20)
#define BSP_AHT20_read(temperature, humidity) GPIO_IIC_AHT20_read(&gpio_aht20, temperature, humidity)

#ifdef __cplusplus
}
#endif

#endif
