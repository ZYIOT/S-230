#include "bsp_hal.h"
#include "bsp_at24cm1.h"
#include "bsp_mcu.h"
#include "bsp_wdg.h"
#include "board_config.h"
#include "bsp_board_debug.h"
#include "app.h"
#include "app_log.h"
#include "app_update_firmware.h"
#include "app_config/map.h"



// 版本更改时，修改board_config.h里对应的版本号的宏的值即可
#define NUM2STR(num)	#num
#define VER2STR(major,minor,revision)	NUM2STR(major)"."NUM2STR(minor)"."NUM2STR(revision)
#define VERSION_STR		VER2STR(BOOT_VERSION_H, BOOT_VERSION_M, BOOT_VERSION_L)
#define BOOT_FIREWARE_NAME_STR		"BOOT_V"VERSION_STR

uint8_t app_info_erro_str[] = "app info error !!!\r\nplease check app file\r\n\r\n";
const uint8_t boot_file_name_str[64] __attribute__((at(FLASH_BOOTLOADER_START_ADDR+0x400))) = BOOT_FIREWARE_NAME_STR;
const uint8_t compiler_time[64] __attribute__((at(FLASH_BOOTLOADER_START_ADDR + 0x440))) = __DATE__"-"__TIME__;




pFunction jump_to_application;
/*****************************************************************************
 * 函 数 名  : _iap_load_app
 * 负 责 人  : Z&L
 * 创建日期  : 2021年10月9日
 * 函数功能  : 跳转到APP程序
 * 输入参数  : uint32_t appxaddr  APP的起始地址
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它    : 

*****************************************************************************/
#if 1
void _iap_load_app(uint32_t appxaddr)
{
    APP_LOG_trace("Ready Jump to App,AppAddress:%x\r\n",appxaddr);
    HARDWARE_HAL_DELAY_MS(20);
	BSP_WDG_stop();
    __disable_irq(); // 在APP 应用的 SystemInit函数的最后加上 __enable_irq();
    __set_PRIMASK(1); // 关闭STM32总中断
    __set_FAULTMASK(1);
#ifdef USE_HAL_DRIVER
    HAL_RCC_DeInit();
    HAL_DeInit();
#endif
#if (1 == DEBUG_JUMP_TO_APP_ENABLE)
    /* Jump to user application */
    jump_to_application = (pFunction) *(__IO uint32_t*) (appxaddr + 4);
    /* Initialize user application's Stack Pointer */
    __set_PSP(*(__IO uint32_t *)appxaddr);
    __set_MSP(*(__IO uint32_t*) appxaddr);
    __set_CONTROL(0);
    jump_to_application();							//跳转到APP.
#else
	firmware_setting_info_test_recv();
#endif
}
#else
void _iap_load_app(uint32_t appxaddr)
{
    APP_LOG_trace("Ready Jump to App,AppAddress:%x\r\n",appxaddr);
    HARDWARE_HAL_DELAY_MS(20);
	BSP_WDG_stop();
#if (1 == DEBUG_JUMP_TO_APP_ENABLE)
    /* Jump to user application */
    jump_to_application = (pFunction) *(__IO uint32_t*) (appxaddr + 4);
    /* Initialize user application's Stack Pointer */
    __set_MSP(*(__IO uint32_t*) appxaddr);
    jump_to_application();							//跳转到APP.
#else
	firmware_setting_info_test_recv();
#endif
}
#endif

static void APP_FILEINFO_SHOW(void)
{
    uint8_t *app_version_str = (uint8_t *)(FLASH_BOOTLOADER_START_ADDR + 0x400);
    uint32_t infoAddr = FLASH_BOOTLOADER_START_ADDR + 0x400;
	APP_LOG_show("boot file <%s> at %08x\r\n", app_version_str, infoAddr);
    APP_LOG_show("compiler at <%s>\r\n", compiler_time);
}

int APP_MAIN(void)
{
    int rc = APP_OK;
	UPDATE_ERROR_TYPE_E check = UPDATE_NO_ERROR;
    BSP_LOG_init();
    BSP_LOG_level_show();
    BSP_WDG_init();
    BSP_WDG_feed();
    APP_FILEINFO_SHOW();
	// 检查 Factory 区并设置数据
    check = APP_UPDATE_FIRMWARE_factory_check();
	APP_LOG_trace("%s\r\n", boot_file_name_str);
    if(UPDATE_NO_ERROR == check)
    {
        check = APP_UPDATE_FIRMWARE_check_file();
        if((UPDATE_NO_ERROR == check) 
        || (UPDATE_ERROR_FILE_EXIST == check) 
        || (UPDATE_ERROR_DOWNLOAD_UNFINISH == check) 
        || (UPDATE_ERROR_FILETYPE == check) 
        || (UPDATE_ERROR_BINTYPE == check))
        {
            _iap_load_app(FLASH_APP_START_ADDR);
        }
    }
    while(1)
    {
        APP_UPDATE_FIRMWARE_showErrorTypeContent(check);
        APP_LOG_trace("%s", app_info_erro_str);
        HARDWARE_HAL_DELAY_MS(2000);
    }
    return 0;
}
