/*
 * @Description: 
 * @FilePath: \Y-520_main_control_boot\Bsp\board_config.h
 * @Version: 1.0
 * @Author: ZYIOT_Z&L
 * @Date: 2022-11-29 10:21:29
 * @LastEditTime: 2024-02-18 14:20:27
 * @LastEditors: ZYIOT_Z&L
 * @Company: zytdwl @copyright 
 */
#ifndef __BSP_CONFIG_H
#define __BSP_CONFIG_H

#ifdef __cplusplus
extern "C"
{
#endif

#define BSP_LOG_DEFAULT_LEVEL 5
#define ENABLE_LOG_TRACE
#define DISABLE_WDG


// DEBUG 时使用的变量宏定义 
#define DEBUG_FIRMWARE_SETTING_INFO_TEST	(0)

#if (0 == DEBUG_FIRMWARE_SETTING_INFO_TEST)
#define DEBUG_JUMP_TO_APP_ENABLE		(1)
#else
#define DEBUG_JUMP_TO_APP_ENABLE		(0)
#endif

#define PN_CODE "S-230"

// BOOT 版本号相关宏定义 
#define BOOT_VERSION_H	3
#define BOOT_VERSION_M	0
#define BOOT_VERSION_L	0

#define BOOT_HARDWARE_MAJOR 	1
#define BOOT_HARDWARE_MINOR 	0
#define BOOT_HARDWARE_REVISION 	0

#define APP_VERSION_H_DEFAULT	3
#define APP_VERSION_M_DEFAULT	0
#define APP_VERSION_L_DEFAULT	0


#ifdef __cplusplus
}
#endif

#endif
