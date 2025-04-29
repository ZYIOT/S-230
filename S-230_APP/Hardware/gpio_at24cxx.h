#ifndef __HARDWARE_GPIO_AT24CXX_H
#define __HARDWARE_GPIO_AT24CXX_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "gpio_iic.h"

#define ADDRESS_TYPE_8_BIT 1
#define ADDRESS_TYPE_16_BIT 2
#define EEPROM_WRITE_OPT        (0x0)
#define EEPROM_READ_OPT         (0x1)
#define EEPROM_DATA_ADDRESS     (0xA)
#define EEPROM_ID_ADDRESS       (0xB)
#define EEPROM_ID_REG           (0x200)
#define EEPROM_ID_LEN           (0x10)

    typedef struct
    {
        GPIO_IIC_TypeDef *IIC;
        uint8_t address_type;

    } GPIO_AT24CXX_TypeDef;

    int GPIO_IIC_AT24CXX_read(GPIO_AT24CXX_TypeDef *AT24Cx, uint8_t deviceID, uint16_t addr, uint8_t *data);
    int GPIO_IIC_AT24CXX_write(GPIO_AT24CXX_TypeDef *AT24Cx, uint8_t deviceID, uint16_t addr, uint8_t *data, size_t timeout);
    int GPIO_IIC_AT24CXX_read_bytes(GPIO_AT24CXX_TypeDef *AT24Cx, uint8_t deviceID, uint16_t addr, uint8_t *data, size_t len);
    int GPIO_IIC_AT24CXX_write_bytes(GPIO_AT24CXX_TypeDef *AT24Cx, uint8_t deviceID, uint16_t addr, uint8_t *data, size_t len, size_t timeout);
    int GPIO_IIC_AT24CXX_read_id(GPIO_AT24CXX_TypeDef *AT24Cx, uint8_t deviceID, uint8_t *data, size_t len);
#ifdef __cplusplus
}
#endif

#endif
