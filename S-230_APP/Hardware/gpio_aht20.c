#include "gpio_aht20.h"
#include "utils.h"

#define I2Cx AHT20x->IIC
#define __CHECK_RC_AND_RETURN \
    if (rc != 0)              \
    {                         \
        GPIO_IIC_stop(I2Cx);  \
        return rc;            \
    }

#define AHT20_CMD_INIT 0xBE
#define AHT20_CMD_RESET 0xBA
#define AHT20_CMD_TRIGGER 0xAC
#define AHT20_DEVICE_ID 0x38

static uint8_t CRC8(uint8_t *buf, uint8_t len)
{
    uint8_t i;
    uint8_t byte;
    uint8_t crc = 0xFF;
    for (byte = 0; byte < len; byte++)
    {
        crc ^= (buf[byte]);
        for (i = 8; i > 0; --i)
        {
            if (crc & 0x80)
                crc = (crc << 1) ^ 0x31;
            else
                crc = (crc << 1);
        }
    }
    return crc;
}

static int _read_bytes(GPIO_AHT20_TypeDef *AHT20x, uint8_t *data, size_t len)
{
    int rc = 0;
    GPIO_IIC_start(I2Cx);
    GPIO_IIC_send_byte(I2Cx, (AHT20_DEVICE_ID << 1) | 1);
    rc = GPIO_IIC_wait_ack(I2Cx);
    __CHECK_RC_AND_RETURN
    for (int i = 0; i < len; i++)
    {
        data[i] = GPIO_IIC_read_byte(I2Cx, i == (len - 1) ? GPIO_IIC_NACK : GPIO_IIC_ACK);
    }
    GPIO_IIC_stop(I2Cx);
    return 0;
}

static int _check_busy(GPIO_AHT20_TypeDef *AHT20x, uint8_t device_id, size_t timeout)
{
    int rc = 0;
    size_t start = HARDWARE_GET_TICK();
    do
    {
        if (HARDWARE_GET_TICK() - start > timeout)
        {
            return -1;
        }
        HARDWARE_HAL_DELAY_US(20);
        GPIO_IIC_start(I2Cx);
        GPIO_IIC_send_byte(I2Cx, (AHT20_DEVICE_ID << 1) | 1);
        rc = GPIO_IIC_wait_ack(I2Cx);
        GPIO_IIC_stop(I2Cx);
    } while (rc != 0);
    return 0;
}

int _write_bytes(GPIO_AHT20_TypeDef *AHT20x, uint8_t cmd, uint8_t *data, size_t len, size_t timeout)
{
    int rc = 0;
    GPIO_IIC_start(I2Cx);
    GPIO_IIC_send_byte(I2Cx, AHT20_DEVICE_ID << 1);
    rc = GPIO_IIC_wait_ack(I2Cx);
    __CHECK_RC_AND_RETURN
    GPIO_IIC_send_byte(I2Cx, cmd);
    rc = GPIO_IIC_wait_ack(I2Cx);
    __CHECK_RC_AND_RETURN
    for (int i = 0; i < len; i++)
    {
        GPIO_IIC_send_byte(I2Cx, data[i]);
        rc = GPIO_IIC_wait_ack(I2Cx);
        __CHECK_RC_AND_RETURN
    }
    GPIO_IIC_stop(I2Cx);
    return _check_busy(AHT20x, AHT20_DEVICE_ID, timeout);
}

int GPIO_IIC_AHT20_init(GPIO_AHT20_TypeDef *AHT20x)
{
    uint8_t buf[] = {0x08, 0x00};
    return _write_bytes(AHT20x, AHT20_CMD_INIT, buf, 2, 120);
}

int GPIO_IIC_AHT20_reset(GPIO_AHT20_TypeDef *AHT20x)
{
    uint8_t buf[] = {0x0};
    return _write_bytes(AHT20x, AHT20_CMD_RESET, buf, 0, 120);
}

int GPIO_IIC_AHT20_trigger(GPIO_AHT20_TypeDef *AHT20x)
{
    uint8_t buf[] = {0x33, 0x00};
    return _write_bytes(AHT20x, AHT20_CMD_TRIGGER, buf, 2, 120);
}

int GPIO_IIC_AHT20_read(GPIO_AHT20_TypeDef *AHT20x, float *temperature, float *humidity)
{
    uint8_t buf[7] = {0};
    int rc = _read_bytes(AHT20x, buf, 7);
    __CHECK_RC_AND_RETURN

    if ((buf[0] & 0x88) != 0x8)
    {
        return -1;
    }
    if (CRC8(buf, 6) != buf[6])
    {
        return -1;
    }

    uint32_t t = read_uint32_t_BE(&buf[1]);
    t = (t >> 12) & 0xFFFFF;
    *humidity = t * 100.0 / 1024 / 1024;
    t = read_uint32_t_BE(&buf[2]);
    t = t & 0xFFFFF;
    *temperature = t * 200.0 / 1024 / 1024 - 50;
    return 0;
}
