#include "hardware_wrapper.h"
#include "app.h"
#include "bsp_key.h"
#include "bsp_wdg.h"
#include "bsp_log.h"

#define BSP_KEY_MAX_INDEX 4

#define __BSP_KEY_read(KEYx) HARDWARE_GPIO_READ_PIN(KEYx->gpio, KEYx->pin)

typedef struct
{
    GPIO_TypeDef *gpio;
    uint16_t pin;
    uint8_t status; //当前状态
    uint32_t time;  //连续N个周期为按下
} BSP_KEY_TypeDef;

static BSP_KEY_TypeDef keys[] = {
    {.gpio = KEY_MENU_GPIO_Port, .pin = KEY_MENU_Pin, .status = 0, .time = 0},
    {.gpio = KEY_OK_GPIO_Port, .pin = KEY_OK_Pin, .status = 0, .time = 0},
    {.gpio = KEY_BACK_GPIO_Port, .pin = KEY_BACK_Pin, .status = 0, .time = 0},
    {.gpio = KEY_NEXT_GPIO_Port, .pin = KEY_NEXT_Pin, .status = 0, .time = 0}};

int BSP_KEY_init(void)
{
    return APP_OK;
}

int BSP_KEY_check_keys(void)
{
    int count = 0;
    BSP_KEY_TypeDef *key = &(keys[0]);
    for (int i = 0; i < BSP_KEY_MAX_INDEX; i++)
    {
        key = &(keys[i]);
        uint8_t s = __BSP_KEY_read(key);
        if (s == GPIO_PIN_RESET)
        {
            key->status = GPIO_PIN_SET;
            key->time++;
            count++;
        }
        else
        {
            key->status = GPIO_PIN_RESET;
            key->time = 0;
        }
    }
    return count;
}
int BSP_KEY_read(uint8_t idx, uint8_t *status, uint32_t *time)
{
    if (idx > BSP_KEY_MAX_INDEX)
    {
        return -1;
    }
    BSP_KEY_TypeDef *key = &(keys[idx]);
    *status = key->status;
    *time = key->time;
    return APP_OK;
}

int BSP_KEY_press(uint8_t key_mask, uint32_t ms)
{
    uint8_t status = 0;
    uint32_t times = 0;
    uint32_t delay = HARDWARE_GET_TICK();
    int i = 0;
    uint8_t mask = 0;
    do
    {
        BSP_KEY_check_keys();
        for (i = 0; i < BSP_KEY_MAX_INDEX; i++)
        {
            BSP_KEY_read(i, &status, &times);
            mask = 0x80 >> i;
            if (status != GPIO_PIN_SET && (key_mask & mask) == mask)
            {
                return APP_ERROR;
            }
        }
        BSP_WDG_feed();
        HARDWARE_HAL_DELAY_MS(50);
        // BSP_LOG_debug("key: %u\r\n", HARDWARE_GET_TICK() - delay);
    } while (HARDWARE_GET_TICK() - delay < ms);

    return APP_OK;
}
