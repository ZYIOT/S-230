#include "bsp_hal.h"
#include "bsp_usart.h"
#include "bsp_at24cm1.h"
#include "bsp_rtc.h"
#include "bsp_mcu.h"
#include "bsp_wdg.h"
#include "bsp_power_key.h"

#include "app.h"
#include "app_log.h"
#include "app_config.h"
#include "app_task.h"
#ifdef SUPPORT_PUMP
#include "app_pump.h"
#endif
#ifdef BOARD_HAS_RELAY
#include "app_relay.h"
#endif
#include "app_logo.h"
#include "app_key.h"
#include "app_update_firmware.h"

// 版本更改时，修改board_config.h里对应的版本号的宏的值即可
#define NUM2STR(num)	#num
#define VER2STR(major,minor,revision,build)	NUM2STR(major)"."NUM2STR(minor)"."NUM2STR(revision)"."NUM2STR(build)
#define VERSION_STR		VER2STR(APP_FIRMWARE_MAJOR, APP_FIRMWARE_MINOR, APP_FIRMWARE_REVISION, APP_FIRMWARE_BUILD)
#define APP_FIREWARE_NAME_STR		PN_CODE"_APP_V"VERSION_STR
const uint8_t app_file_name_str[64] __attribute__((at(FLASH_APP_START_ADDR + 0x400))) = APP_FIREWARE_NAME_STR;
const uint8_t compiler_time[64] __attribute__((at(FLASH_APP_START_ADDR + 0x440))) = __DATE__"-"__TIME__;

// 显示程序版本信息 
static void APP_FILEINFO_SHOW(void)
{
    uint8_t *app_version_str = (uint8_t *)&app_file_name_str;
    uint32_t infoAddr = (uint32_t)&app_file_name_str;
	APP_LOG_show("device <%s> at %08x\r\n", app_version_str, infoAddr);
    APP_LOG_show("compiler at <%s>\r\n", compiler_time);
}

static void APP_LOGINFO_SHOW(void)
{
    APP_LOG_show("APP LOG CONFIG INFO\r\n");
    APP_LOG_show("*********************\r\n");
#ifdef SHOW_RS485_DATA
    APP_LOG_show("* show [RS485 DATA]\r\n");
#endif
#ifdef SHOW_PROTOCOL_DATA
    APP_LOG_show("* show [PROTOCOL DATA]\r\n");
#endif
#ifdef SHOW_GPRS_DATA
    APP_LOG_show("* show [GPRS DATA]\r\n");
#endif
#ifdef SHOW_DEVICE_TIME
    APP_LOG_show("* show [DEVICE TIME]\r\n");
#endif
#ifdef SHOW_LCD_SENSOR_STATUS
    APP_LOG_show("* show [LCD SENSOR STATUS]\r\n");
#endif
#ifdef SHOW_PUMP_STATUS
    APP_LOG_show("* show [PUMP STATUS]\r\n");
#endif
    APP_LOG_show("*********************\r\n");
}

static int _app_init(void)
{
    MCU_STATE_ON();
    POWER_GPRS_ON();
    POWER_SENSOR_ON();
    BSP_AT24CM1_init();
    APP_CONFIG_init();
#ifdef SUPPORT_PUMP
    BSP_PUMP_off();
    APP_PUMP_init();
#endif
#ifdef BOARD_HAS_RELAY
    APP_RELAY_init();
#endif
    APP_UPDATE_FIRMWARE_compare_app_firmware_setting_info();
    return APP_OK;
}

int main(void)
{
    // __HAL_RCC_HSI_ENABLE();
    // __HAL_RCC_SYSCLK_CONFIG(RCC_SYSCLKSOURCE_HSI);
    BSP_HAL_init();
    BSP_UART_init();
    APP_LOG_init();
    APP_LOG_level_show();
    APP_LOGO_show();
	APP_FILEINFO_SHOW();
    APP_LOGINFO_SHOW();
    _app_init();
    APP_TASK_init();
    BSP_WDG_feed();
    APP_TASK_run();
    return 0;
}
