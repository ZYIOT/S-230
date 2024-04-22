#include "main.h"
#include "bsp_aht20.h"
#include "hardware_wrapper.h"

static GPIO_IIC_TypeDef gpio_iic = {
    .sda_gpio = IIC_SDA_GPIO_Port,
    .sda_pin = IIC_SDA_Pin,
    .scl_gpio = IIC_SCL_GPIO_Port,
    .scl_pin = IIC_SCL_Pin,
};

GPIO_AHT20_TypeDef gpio_aht20 = {
    .IIC = &gpio_iic};

