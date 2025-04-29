#ifndef __HARDWARE_GPIO_IIC_H
#define __HARDWARE_GPIO_IIC_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "bsp_gpio.h"
#include "hardware_wrapper.h"

    typedef struct
    {
        GPIO_TypeDef *sda_gpio;
        GPIO_TypeDef *scl_gpio;
        uint16_t sda_pin;
        uint16_t scl_pin;
    } GPIO_IIC_TypeDef;

#define GPIO_IIC_NACK 0
#define GPIO_IIC_ACK 1

    void GPIO_IIC_start(GPIO_IIC_TypeDef *I2Cx);
    void GPIO_IIC_stop(GPIO_IIC_TypeDef *I2Cx);
    int GPIO_IIC_wait_ack(GPIO_IIC_TypeDef *I2Cx);
    void GPIO_IIC_ack(GPIO_IIC_TypeDef *I2Cx);
    void GPIO_IIC_nack(GPIO_IIC_TypeDef *I2Cx);
    void GPIO_IIC_send_byte(GPIO_IIC_TypeDef *I2Cx, uint8_t byte);
    int GPIO_IIC_read_byte(GPIO_IIC_TypeDef *I2Cx, uint8_t ack);
#ifdef __cplusplus
}
#endif

#endif
