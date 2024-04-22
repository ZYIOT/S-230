#include "main.h"
#include "bsp_at24cm1.h"
#include "hardware_wrapper.h"

static GPIO_IIC_TypeDef gpio_iic = {
    .sda_gpio = IIC_SDA_GPIO_Port,
    .sda_pin = IIC_SDA_Pin,
    .scl_gpio = IIC_SCL_GPIO_Port,
    .scl_pin = IIC_SCL_Pin,
};

GPIO_AT24CXX_TypeDef gpio_at24cm1 = {
    .IIC = &gpio_iic,
    .address_type = ADDRESS_TYPE_16_BIT};

#define _BSP_AT24CM1_TRY_TIMES 3

int BSP_AT24CM1_read_bytes(uint16_t reg, uint8_t *data, uint16_t len)
{
    int rc = 0;
    for (int i = 0; i < _BSP_AT24CM1_TRY_TIMES; i++)
    {
        rc = GPIO_IIC_AT24CXX_read_bytes(&gpio_at24cm1, BSP_AT24CM1_DEVICE_ID, reg, data, len);
        if (rc == HAL_OK)
        {
            return rc;
        }
        HARDWARE_HAL_DELAY_MS(5);
    }
    return rc;
}

int BSP_AT24CM1_write_bytes(uint16_t reg, uint8_t *data, uint16_t len)
{
    int rc = 0;
    for (int i = 0; i < _BSP_AT24CM1_TRY_TIMES; i++)
    {
        rc = GPIO_IIC_AT24CXX_write_bytes(&gpio_at24cm1, BSP_AT24CM1_DEVICE_ID, reg, data, len, 3000);
        if (rc == HAL_OK)
        {
            return rc;
        }
        HARDWARE_HAL_DELAY_MS(5);
    }
    return rc;
}


int BSP_AT24CM1_read_id(uint8_t *data, uint16_t len)
{
    int rc = 0;
    for (int i = 0; i < _BSP_AT24CM1_TRY_TIMES; i++)
    {
        rc = GPIO_IIC_AT24CXX_read_id(&gpio_at24cm1, BSP_AT24CM1_DEVICE_ID, data, len);
        if (rc == HAL_OK)
        {
            return rc;
        }
        HARDWARE_HAL_DELAY_MS(5);
    }
    return rc;
}

