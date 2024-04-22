#ifndef __APP_CONFIG_H
#define __APP_CONFIG_H

#ifdef __cplusplus
extern "C"
{
#endif
#include <stdint.h>
#include "app.h"
#include "config_bit.h"

    extern int eeprom_init_tip;

#define EEPROM_INIT_TIP_FORMAT 1
#define EEPROM_INIT_TIP_RECOVERY 2

#define HAS_CONFIG_IS_VALID(_var) ((_var)->has_config == APP_CONFIG_HAS_CONFIG)
#define ENABLE_IS_VALID(_var) (HAS_CONFIG_IS_VALID(_var) && ((_var)->enable == APP_CONFIG_ENABLED))

#define CHECK_ENABLE_IS_VALID(_var)               \
    if (!ENABLE_IS_VALID(_var))                   \
    {                                             \
        BSP_PROTOCOL_send_error_response(packet); \
        return PROTOCOL_ERROR;                    \
    }
#define CHECK_HAS_CONFIG_IS_VALID(_var)           \
    if (!HAS_CONFIG_IS_VALID(_var))               \
    {                                             \
        BSP_PROTOCOL_send_error_response(packet); \
        return PROTOCOL_ERROR;                    \
    }

#include "app_config/defines.h"

    int APP_CONFIG_init(void);
    uint32_t APP_CONFIG_device_id(void);
#ifdef __cplusplus
}
#endif

#endif
