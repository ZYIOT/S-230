#
#include "bsp_hal.h"
#include "bsp_usart.h"
#include "bsp_at24cm1.h"
#include "bsp_pump.h"
#include "bsp_mcu.h"
#include "bsp_wdg.h"
#include "bsp_power_key.h"
#include "utils.h"
#include "app.h"
#include "app_log.h"
#include "app_config/defines.h"
#include "app_config/map.h"
#include "app_config/common.h"
#include "app_config/message.h"


// 版本更改时，修改board_config.h里对应的版本号的宏的值即可
#define NUM2STR(num)	#num
#define VER2STR(major,minor,revision)	NUM2STR(major)"."NUM2STR(minor)"."NUM2STR(revision)
#define VERSION_STR		VER2STR(APP_FIRMWARE_MAJOR, APP_FIRMWARE_MINOR, APP_FIRMWARE_REVISION)
#define APP_FIREWARE_NAME_STR		PN_CODE"_FORMATTER_V"VERSION_STR
uint8_t app_file_name_str[64] __attribute__((at(0x8000000 + 0x400))) = APP_FIREWARE_NAME_STR;
uint8_t compiler_time[64] __attribute__((at(0x8000000 + 0x440))) = __DATE__"-"__TIME__;

// 显示程序版本信息 
static void APP_FILEINFO_SHOW(void)
{
    uint8_t *app_version_str = (uint8_t *)&app_file_name_str;
    uint32_t infoAddr = (uint32_t)&app_file_name_str;
	BSP_LOG_show("3G online device <%s> at %08x\r\n", app_version_str, infoAddr);
    BSP_LOG_show("compiler at <%s>\r\n", compiler_time);
}

static int _app_init(void)
{
    BSP_AT24CM1_init();
    BSP_PUMP_init();
    return APP_OK;
}

static int format_eeprom(void)
{
    int rc = APP_CONFIG_system_load();
    BSP_WDG_feed();
    if (rc != APP_OK)
    {
        rc = APP_CONFIG_system_recovery();
        BSP_LOG_trace("system recovery rc: %d\r\n", rc);
        APP_CHECK_RC_AND_RESET(rc);
    }
    rc = APP_CONFIG_eeprom_recovery_all();
    rc = APP_CONFIG_eeprom_recovery_free_page();
    BSP_WDG_feed();
    BSP_LOG_debug("config recovery rc: %d\r\n", rc);
    APP_CHECK_RC_AND_RESET(rc);
    return APP_OK;
}

__weak app_relay_power_config_t app_relay_power_config[RELAY_SIZE][RELAY_CHANNEL_SIZE];
__weak void APP_NETWORK_check_stataus(void)
{
}

__weak void APP_TASK_run_per_second_ISR(void)
{
    // BSP_WDG_feed();
}


#define SUPPORT_ALL_BOARD_FORMATTER     // 支持 Y-511/Y-515/S220 共用一个格式化程序 

#ifdef SUPPORT_ALL_BOARD_FORMATTER

#define PUMP_PWRKEY_Pin GPIO_PIN_7
#define PUMP_PWRKEY_GPIO_Port GPIOB

#define SENSOR_PWRKEY_Pin GPIO_PIN_8
#define SENSOR_PWRKEY_GPIO_Port GPIOC

#ifndef SUPPORT_PUMP
#define BSP_PUMP_on() HARDWARE_GPIO_WRITE_PIN_L(PUMP_PWRKEY_GPIO_Port, PUMP_PWRKEY_Pin)
#define BSP_PUMP_off() HARDWARE_GPIO_WRITE_PIN_H(PUMP_PWRKEY_GPIO_Port, PUMP_PWRKEY_Pin)
#endif
#endif

#ifndef SUPPORT_ALL_BOARD_FORMATTER
#if     defined(SUPPORT_PUMP)
#define  BOARD_RELAY_on() BSP_PUMP_on()
#define  BOARD_RELAY_off() BSP_PUMP_off()
#elif   defined(SUPPORT_L6_ENABLE)
#define  BOARD_RELAY_on() BSP_BUZZ_on()
#define  BOARD_RELAY_off() BSP_BUZZ_off()
#else
#define  BOARD_RELAY_on() POWER_SENSOR_ON()
#define  BOARD_RELAY_off() POWER_SENSOR_OFF()
#endif
#else
#define  BOARD_RELAY_on()   \
    BSP_PUMP_on();          \
    POWER_SENSOR_ON()

#define BOARD_RELAY_off()   \
    BSP_PUMP_off();         \
    POWER_SENSOR_OFF()
#endif


#ifdef SUPPORT_ALL_BOARD_FORMATTER
void BUZZ_PWRKEY_MX_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    
    GPIO_InitStruct.Pin = PUMP_PWRKEY_Pin ;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOB, PUMP_PWRKEY_Pin , GPIO_PIN_SET);

    GPIO_InitStruct.Pin = SENSOR_PWRKEY_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(SENSOR_PWRKEY_GPIO_Port, &GPIO_InitStruct);
	HAL_GPIO_WritePin(SENSOR_PWRKEY_GPIO_Port, SENSOR_PWRKEY_Pin, GPIO_PIN_SET);
}
#endif


int main(void)
{
    BSP_HAL_init();
    #ifdef SUPPORT_ALL_BOARD_FORMATTER
        BUZZ_PWRKEY_MX_Init();
    #endif
    BSP_UART_init();
    BSP_LOG_init();
	APP_FILEINFO_SHOW();
    _app_init();
    int rc = format_eeprom();
    if (rc == APP_OK)
    {
        while (1)
        {
            BOARD_RELAY_on();
            MCU_STATE_ON();
            BSP_WDG_feed();
            HARDWARE_HAL_DELAY_MS(1000);
            MCU_STATE_OFF();
            BOARD_RELAY_off();
            BSP_WDG_feed();
            HARDWARE_HAL_DELAY_MS(1000);
            BSP_LOG_debug("eeprom formatter run\r\n");
        }
    }
    else
    {
        while (1)
        {
            MCU_STATE_ON();
            BOARD_RELAY_on();
            BSP_WDG_feed();
            HARDWARE_HAL_DELAY_MS(200);
            MCU_STATE_OFF();
            BOARD_RELAY_off();
            BSP_WDG_feed();
            HARDWARE_HAL_DELAY_MS(1800);
            BSP_LOG_debug("eeprom formatter error!!!\r\n");
        }
    }
}
