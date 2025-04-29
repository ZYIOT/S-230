
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

int g_eepromInitTip = 0;
uint32_t APP_CONFIG_DeviceID(void)
{
    return g_appConfigSystem.deviceID;
}

static int FormatEeprom(void)
{
    int rc = APP_OK;
    return rc;
    rc = APP_CONFIG_SystemLoad();
    BSP_WDG_feed();
    if (rc != APP_OK)
    {
        rc = APP_CONFIG_SystemRecovery();
        APP_LOG_Trace("system recovery rc: %d\r\n", rc);
        APP_CHECK_RC_AND_RESET(rc);
        g_eepromInitTip = EEPROM_INIT_TIP_FORMAT;
    }
    else
    {
        g_eepromInitTip = EEPROM_INIT_TIP_RECOVERY;
    }
    BSP_WDG_feed();
    rc = APP_CONFIG_EepromRecoveryAll();
    BSP_WDG_feed();
    APP_LOG_Debug("config recovery rc: %d\r\n", rc);
    APP_CHECK_RC_AND_RESET(rc);
    return APP_OK;
}

static int RecoveryEeprom(void)
{
    if (g_eepromInitTip != 0)
    {
        return APP_OK;
    }
    int rc = APP_OK;
    return rc;
    BSP_WDG_feed();
    rc = APP_CONFIG_EepromRecoveryAll();
    BSP_WDG_feed();
    APP_LOG_Debug("config recovery rc: %d\r\n", rc);
    APP_CHECK_RC_AND_RESET(rc);
    g_eepromInitTip = EEPROM_INIT_TIP_RECOVERY;
    return APP_OK;
}

int APP_CONFIG_Init(void)
{
    APP_CONFIG_SystemInit(&g_appConfigSystem);
    HARDWARE_HAL_DELAY_MS(10);
    BSP_WDG_feed();
    FormatEeprom();
    BSP_WDG_feed();
    RecoveryEeprom();
    BSP_WDG_feed();
    int rc = APP_CONFIG_EepromLoadAll();
    BSP_WDG_feed();
    APP_LOG_Debug("config init rc: %d\r\n", rc);
    APP_CHECK_RC_AND_RESET(rc)
    return APP_OK;
}
