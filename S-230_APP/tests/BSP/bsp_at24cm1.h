#ifndef __BSP_AT24CM1_H
#define __BSP_AT24CM1_H

#ifdef __cplusplus
extern "C"
{
#endif
#include "hardware_wrapper.h"
#define BSP_AT24CM1_DEVICE_ID 1
#define BSP_AT24CM1_init()
#define BSP_AT24CM1_read(reg, data) BSP_AT24CM1_read_bytes(reg, data, 1)
#define BSP_AT24CM1_write(reg, data) BSP_AT24CM1_write_bytes(reg, data, 1)
    int BSP_AT24CM1_write_bytes(uint16_t reg, uint8_t *data, uint16_t len);
    int BSP_AT24CM1_read_bytes(uint16_t reg, uint8_t *data, uint16_t len);

#ifdef __cplusplus
}
#endif

#endif
