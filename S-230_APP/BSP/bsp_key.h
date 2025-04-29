#ifndef __BSP_KEY_H
#define __BSP_KEY_H

#ifdef __cplusplus
extern "C"
{
#endif
#include "main.h"

    int BSP_KEY_init(void);
    int BSP_KEY_check_keys(void);
    int BSP_KEY_read(uint8_t idx, uint8_t *status, uint32_t *time);
    int BSP_KEY_press(uint8_t key_mask, uint32_t ms);
#ifdef __cplusplus
}
#endif

#endif
