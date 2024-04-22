
#include "bsp_relay.h"
#ifdef BOARD_HAS_RELAY
typedef struct
{
    GPIO_TypeDef *gpio;
    uint16_t pin;
} BSP_RELAY_TypeDef;

static BSP_RELAY_TypeDef channels[] = {

    {.gpio = RELAY_1_GPIO_Port, .pin = RELAY_1_Pin}
#if (RELAY_CHANNEL_SIZE > 1)
    ,
    {.gpio = RELAY_2_GPIO_Port, .pin = RELAY_2_Pin}
#if (RELAY_CHANNEL_SIZE > 2)
    ,
    {.gpio = RELAY_3_GPIO_Port, .pin = RELAY_3_Pin}
#if (RELAY_CHANNEL_SIZE > 3)
    ,
    {.gpio = RELAY_4_GPIO_Port, .pin = RELAY_4_Pin}
#if (RELAY_CHANNEL_SIZE > 4)
    ,
    {.gpio = RELAY_5_GPIO_Port, .pin = RELAY_5_Pin}
#endif
#endif
#endif
#endif

};

static BSP_RELAY_TypeDef checks[] = {
    {.gpio = RELAY_OUT_1_GPIO_Port, .pin = RELAY_OUT_1_Pin}
#if (RELAY_CHANNEL_SIZE > 1)
    ,
    {.gpio = RELAY_OUT_2_GPIO_Port, .pin = RELAY_OUT_2_Pin}
#if (RELAY_CHANNEL_SIZE > 2)
    ,
    {.gpio = RELAY_OUT_3_GPIO_Port, .pin = RELAY_OUT_3_Pin}
#if (RELAY_CHANNEL_SIZE > 3)
    ,
    {.gpio = RELAY_OUT_4_GPIO_Port, .pin = RELAY_OUT_4_Pin}
#if (RELAY_CHANNEL_SIZE > 4)
    ,
    {.gpio = RELAY_OUT_5_GPIO_Port, .pin = RELAY_OUT_5_Pin}
#endif
#endif
#endif
#endif
};

#define __BSP_RELAY_OUT_read(RELAYx) HARDWARE_GPIO_READ_PIN(RELAYx->gpio, RELAYx->pin)
#define __BSP_RELAY_WRITE(RELAYx, status) HARDWARE_GPIO_WRITE_PIN(RELAYx->gpio, RELAYx->pin, status)

int BSP_RELAY_init(void)
{
    return APP_OK;
}

int BSP_RELAY_read_channel(uint8_t relay_id, uint8_t channel_id)
{
    if (relay_id >= RELAY_SIZE || channel_id >= RELAY_CHANNEL_SIZE)
    {
        return -1;
    }
    BSP_RELAY_TypeDef *check = &(checks[channel_id]);
    uint8_t s = __BSP_RELAY_OUT_read(check);
    if (s == GPIO_PIN_RESET)
    {
        return GPIO_PIN_SET;
    }
    else
    {
        return GPIO_PIN_RESET;
    }
}

int BSP_RELAY_set_channel(uint8_t relay_id, uint8_t channel_id, uint8_t status)
{
    if (relay_id >= RELAY_SIZE || channel_id >= RELAY_CHANNEL_SIZE)
    {
        return APP_ERROR;
    }
    BSP_RELAY_TypeDef *channel = &(channels[channel_id]);
    if (status == 0)
    {
        __BSP_RELAY_WRITE(channel, 1);
    }
    else
    {
        __BSP_RELAY_WRITE(channel, 0);
    }
    return APP_OK;
}

#endif
