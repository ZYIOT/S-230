#include "gpio_at24cxx.h"

#define I2Cx AT24Cx->IIC
#define __CHECK_RC_AND_RETURN \
    if (rc != 0)              \
    {                         \
        GPIO_IIC_stop(I2Cx);  \
        return rc;            \
    }

int GPIO_IIC_AT24CXX_read(GPIO_AT24CXX_TypeDef *AT24Cx, uint8_t device_id, uint16_t addr, uint8_t *data)
{
    return GPIO_IIC_AT24CXX_read_bytes(AT24Cx, device_id, addr, data, 1);
}

int GPIO_IIC_AT24CXX_write(GPIO_AT24CXX_TypeDef *AT24Cx, uint8_t device_id, uint16_t addr, uint8_t *data, size_t timeout)
{
    return GPIO_IIC_AT24CXX_write_bytes(AT24Cx, device_id, addr, data, 1, timeout);
}

static int _GPIO_IIC_AT24CXX_send_address(GPIO_AT24CXX_TypeDef *AT24Cx, uint16_t addr)
{
    int rc = 0;
    if (AT24Cx->address_type == ADDRESS_TYPE_8_BIT)
    {
        GPIO_IIC_send_byte(I2Cx, addr % 256);
        rc = GPIO_IIC_wait_ack(I2Cx);
        __CHECK_RC_AND_RETURN
    }
    else
    {
        GPIO_IIC_send_byte(I2Cx, addr / 256);
        rc = GPIO_IIC_wait_ack(I2Cx);
        __CHECK_RC_AND_RETURN
        GPIO_IIC_send_byte(I2Cx, addr % 256);
        rc = GPIO_IIC_wait_ack(I2Cx);
        __CHECK_RC_AND_RETURN
    }
    return 0;
}

int GPIO_IIC_AT24CXX_read_bytes(GPIO_AT24CXX_TypeDef *AT24Cx, uint8_t device_id, uint16_t addr, uint8_t *data, size_t len)
{
    int rc = 0;
    GPIO_IIC_start(I2Cx);
    GPIO_IIC_send_byte(I2Cx, (EEPROM_DATA_ADDRESS << 4) | ((device_id & 0x7) << 1) | EEPROM_WRITE_OPT);
    rc = GPIO_IIC_wait_ack(I2Cx);
    __CHECK_RC_AND_RETURN
    rc = _GPIO_IIC_AT24CXX_send_address(AT24Cx, addr);
    __CHECK_RC_AND_RETURN
    GPIO_IIC_start(I2Cx);
    GPIO_IIC_send_byte(I2Cx, (EEPROM_DATA_ADDRESS << 4) | ((device_id & 0x7) << 1) | EEPROM_READ_OPT);
    rc = GPIO_IIC_wait_ack(I2Cx);
    __CHECK_RC_AND_RETURN
    for (int i = 0; i < len; i++)
    {
        data[i] = GPIO_IIC_read_byte(I2Cx, i == (len - 1) ? GPIO_IIC_NACK : GPIO_IIC_ACK);
    }
    GPIO_IIC_stop(I2Cx);
    return 0;
}

static int _check_busy(GPIO_AT24CXX_TypeDef *AT24Cx, uint8_t device_id, size_t timeout)
{
    int rc = 0;
    size_t start = HARDWARE_GET_TICK();
    do
    {
        if (HARDWARE_GET_TICK() - start > timeout)
        {
            GPIO_IIC_stop(I2Cx);
            return -1;
        }
        HARDWARE_HAL_DELAY_US(20);
        // current address read
        GPIO_IIC_start(I2Cx);
        GPIO_IIC_send_byte(I2Cx, (EEPROM_DATA_ADDRESS << 4) | ((device_id & 0x7) << 1) | EEPROM_READ_OPT);
        rc = GPIO_IIC_wait_ack(I2Cx);
        GPIO_IIC_read_byte(I2Cx, GPIO_IIC_NACK);
        GPIO_IIC_stop(I2Cx);
    } while (rc != 0);
    return 0;
}

int GPIO_IIC_AT24CXX_write_bytes(GPIO_AT24CXX_TypeDef *AT24Cx, uint8_t device_id, uint16_t addr, uint8_t *data, size_t len, size_t timeout)
{
    int rc = 0;
    GPIO_IIC_start(I2Cx);
    GPIO_IIC_send_byte(I2Cx, (EEPROM_DATA_ADDRESS << 4) | ((device_id & 0x7) << 1) | EEPROM_WRITE_OPT);
    rc = GPIO_IIC_wait_ack(I2Cx);
    __CHECK_RC_AND_RETURN
    rc = _GPIO_IIC_AT24CXX_send_address(AT24Cx, addr);
    __CHECK_RC_AND_RETURN
    for (int i = 0; i < len; i++)
    {
        GPIO_IIC_send_byte(I2Cx, data[i]);
        rc = GPIO_IIC_wait_ack(I2Cx);
        __CHECK_RC_AND_RETURN
    }
    GPIO_IIC_stop(I2Cx);
    return _check_busy(AT24Cx, device_id, timeout);
    // HARDWARE_HAL_DELAY_US(1000); // should check IC busy
    // return 0;
}


int GPIO_IIC_AT24CXX_read_id(GPIO_AT24CXX_TypeDef *AT24Cx, uint8_t device_id, uint8_t *data, size_t len)
{
    int rc = 0;
    GPIO_IIC_start(I2Cx);
    GPIO_IIC_send_byte(I2Cx, (EEPROM_ID_ADDRESS << 4) | ((device_id & 0x7) << 1) | EEPROM_WRITE_OPT);
    rc = GPIO_IIC_wait_ack(I2Cx);
    __CHECK_RC_AND_RETURN
    rc = _GPIO_IIC_AT24CXX_send_address(AT24Cx, EEPROM_ID_REG);
    __CHECK_RC_AND_RETURN
    GPIO_IIC_start(I2Cx);
    GPIO_IIC_send_byte(I2Cx, (EEPROM_ID_ADDRESS << 4) | ((device_id & 0x7) << 1) | EEPROM_READ_OPT);
    rc = GPIO_IIC_wait_ack(I2Cx);
    __CHECK_RC_AND_RETURN
    for (int i = 0; i < len; i++)
    {
        data[i] = GPIO_IIC_read_byte(I2Cx, i == (len - 1) ? GPIO_IIC_NACK : GPIO_IIC_ACK);
    }
    GPIO_IIC_stop(I2Cx);
    return 0;
}