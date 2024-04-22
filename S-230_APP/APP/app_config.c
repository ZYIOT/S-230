
#include "utils.h"
#include "bsp_at24cm1.h"
#include "bsp_mcu.h"
#include "bsp_wdg.h"
#include "app.h"
#include "app_log.h"
#include "app_config.h"
#include "app_config/map.h"
#include "app_config/common.h"
#include "app_config/message.h"

int eeprom_init_tip = 0;
uint32_t APP_CONFIG_device_id(void)
{
    return app_config_system.device_id;
}

static int _format_eeprom(void)
{
    int rc = APP_OK;
    return rc;
    rc = APP_CONFIG_system_load();
    BSP_WDG_feed();
    if (rc != APP_OK)
    {
        rc = APP_CONFIG_system_recovery();
        APP_LOG_trace("system recovery rc: %d\r\n", rc);
        APP_CHECK_RC_AND_RESET(rc);
        eeprom_init_tip = EEPROM_INIT_TIP_FORMAT;
    }
    else
    {
        eeprom_init_tip = EEPROM_INIT_TIP_RECOVERY;
    }
    BSP_WDG_feed();
    rc = APP_CONFIG_eeprom_recovery_all();
    BSP_WDG_feed();
    APP_LOG_debug("config recovery rc: %d\r\n", rc);
    APP_CHECK_RC_AND_RESET(rc);
    return APP_OK;
}

static int _recovery_eeprom(void)
{
    if (eeprom_init_tip != 0)
    {
        return APP_OK;
    }
    int rc = APP_OK;
    return rc;
    BSP_WDG_feed();
    rc = APP_CONFIG_eeprom_recovery_all();
    BSP_WDG_feed();
    APP_LOG_debug("config recovery rc: %d\r\n", rc);
    APP_CHECK_RC_AND_RESET(rc);
    eeprom_init_tip = EEPROM_INIT_TIP_RECOVERY;
    return APP_OK;
}

int APP_CONFIG_init(void)
{
    APP_CONFIG_system_init(&app_config_system);
    HARDWARE_HAL_DELAY_MS(10);
    BSP_WDG_feed();
    _format_eeprom();
    BSP_WDG_feed();
    _recovery_eeprom();
    BSP_WDG_feed();
    int rc = APP_CONFIG_eeprom_load_all();
    BSP_WDG_feed();
    APP_LOG_debug("config init rc: %d\r\n", rc);
    APP_CHECK_RC_AND_RESET(rc)
    return APP_OK;
}
