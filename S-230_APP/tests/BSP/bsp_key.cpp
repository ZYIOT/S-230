#include "hardware_wrapper.h"
#include "app.h"
#include "bsp_key.h"
#include "bsp_wdg.h"
#include "bsp_log.h"

#define BSP_KEY_MAX_INDEX 4

int BSP_KEY_init(void)
{
    return APP_OK;
}

int BSP_KEY_check_keys(void)
{
    return 0;
}
int BSP_KEY_read(uint8_t idx, uint8_t *status, uint32_t *time)
{
    *status = 0;
    *time = 0;
    return APP_OK;
}

int BSP_KEY_press(uint8_t key_mask, uint32_t ms)
{
    return APP_OK;
}
