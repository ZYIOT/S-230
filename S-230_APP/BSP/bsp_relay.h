#ifndef __BSP_RELAY_H
#define __BSP_RELAY_H

#ifdef __cplusplus
extern "C"
{
#endif
#include "main.h"
#include "hardware_wrapper.h"

#ifdef BOARD_HAS_RELAY
    int BSP_RELAY_init(void);
    int BSP_RELAY_read_channel(uint8_t relay_id, uint8_t channel_id);
    int BSP_RELAY_set_channel(uint8_t relay_id, uint8_t channel_id, uint8_t status);
#endif

#ifdef __cplusplus
}
#endif

#endif
