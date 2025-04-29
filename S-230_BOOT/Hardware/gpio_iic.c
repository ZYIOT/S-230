
#include "gpio_iic.h"

#define __SDA_INPUT HARDWARE_GPIO_SET_INPUT(I2Cx->sda_gpio, I2Cx->sda_pin);
#define __SDA_OUTPUT HARDWARE_GPIO_SET_OUTPUT(I2Cx->sda_gpio, I2Cx->sda_pin);
#define __SCL_INPUT HARDWARE_GPIO_SET_INPUT(I2Cx->scl_gpio, I2Cx->scl_pin);
#define __SCL_OUTPUT HARDWARE_GPIO_SET_OUTPUT(I2Cx->scl_gpio, I2Cx->scl_pin);
#define __SCL_H HARDWARE_GPIO_WRITE_PIN(I2Cx->scl_gpio, I2Cx->scl_pin, GPIO_PIN_SET);
#define __SCL_L HARDWARE_GPIO_WRITE_PIN(I2Cx->scl_gpio, I2Cx->scl_pin, GPIO_PIN_RESET);
#define __SDA_H HARDWARE_GPIO_WRITE_PIN(I2Cx->sda_gpio, I2Cx->sda_pin, GPIO_PIN_SET);
#define __SDA_L HARDWARE_GPIO_WRITE_PIN(I2Cx->sda_gpio, I2Cx->sda_pin, GPIO_PIN_RESET);
#define __SCL(state) HARDWARE_GPIO_WRITE_PIN(I2Cx->sda_gpio, I2Cx->sda_pin, state);
#define __SDA(state) HARDWARE_GPIO_WRITE_PIN(I2Cx->sda_gpio, I2Cx->sda_pin, state);
#define __SDA_READ HARDWARE_GPIO_READ_PIN(I2Cx->sda_gpio, I2Cx->sda_pin)
#define __DELAY_US HARDWARE_HAL_DELAY_US(2)
#define __CHECK_RC_AND_RETURN \
    if (rc != 0)              \
    {                         \
        GPIO_IIC_stop(I2Cx);  \
        return rc;            \
    }
void GPIO_IIC_start(GPIO_IIC_TypeDef *I2Cx)
{
    __SDA_OUTPUT
    __SDA_H
    DWT_delay(4);
    __SCL_H
    HARDWARE_HAL_DELAY_US(4);
    __SDA_L
    HARDWARE_HAL_DELAY_US(4);
    // __SCL_L
    // HARDWARE_HAL_DELAY_US(4);
}
void GPIO_IIC_stop(GPIO_IIC_TypeDef *I2Cx)
{
    __SDA_OUTPUT
    __SDA_L
    HARDWARE_HAL_DELAY_US(4);
    __SCL_H
    HARDWARE_HAL_DELAY_US(4);
    __SDA_H
    HARDWARE_HAL_DELAY_US(4);
}

int GPIO_IIC_wait_ack(GPIO_IIC_TypeDef *I2Cx)
{
    uint8_t timeout = 0;
    __SDA_H
    __SDA_INPUT
    HARDWARE_HAL_DELAY_US(2);
    __SCL_H
    HARDWARE_HAL_DELAY_US(2);
    while (__SDA_READ)
    {
        timeout++;
        if (timeout > 250)
        {
            return -1;
        }
    }
    __SCL_L
    return 0;
}

void GPIO_IIC_ack(GPIO_IIC_TypeDef *I2Cx)
{

    __SDA_OUTPUT
    __SCL_L
    __SDA_L
    HARDWARE_HAL_DELAY_US(2);
    __SCL_H
    HARDWARE_HAL_DELAY_US(2);
    __SCL_L
}

void GPIO_IIC_nack(GPIO_IIC_TypeDef *I2Cx)
{
    __SDA_OUTPUT
    __SCL_L
    __SDA_H
    HARDWARE_HAL_DELAY_US(2);
    __SCL_H
    HARDWARE_HAL_DELAY_US(2);
    __SCL_L
}

void GPIO_IIC_send_byte(GPIO_IIC_TypeDef *I2Cx, uint8_t byte)
{
    uint8_t t;
    __SDA_OUTPUT
    HARDWARE_HAL_DELAY_US(2);
    for (t = 0; t < 8; t++)
    {
        __SCL_L
        __SDA((byte & 0x80) > 0 ? 1 : 0);
        byte <<= 1;
        HARDWARE_HAL_DELAY_US(2);
        __SCL_H
        HARDWARE_HAL_DELAY_US(2);
    }
    __SCL_L
}

int GPIO_IIC_read_byte(GPIO_IIC_TypeDef *I2Cx, uint8_t ack)
{
    uint8_t i, receive = 0;
    __SDA_H
    __SDA_INPUT
    for (i = 0; i < 8; i++)
    {
        __SCL_L
        HARDWARE_HAL_DELAY_US(2);
        __SCL_H
        HARDWARE_HAL_DELAY_US(2);
        receive <<= 1;
        if (__SDA_READ)
            receive++;
        HARDWARE_HAL_DELAY_US(2);
    }
    __SCL_L
    if (ack == GPIO_IIC_ACK)
    {
        GPIO_IIC_ack(I2Cx);
    }
    else
    {
        GPIO_IIC_nack(I2Cx);
    }
    return receive;
}
