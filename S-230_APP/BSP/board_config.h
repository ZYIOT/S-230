/*
 * @Description: 
 * @FilePath: \S-230_app-LCD\BSP\board_config.h
 * @Version: 1.0
 * @Author: ZYIOT_Z&L
 * @Date: 2023-11-14 08:59:19
 * @LastEditTime: 2024-02-18 09:10:19
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
// 以下是调试时使用的变量
// #define DISABLE_WDG

#define BOARD_HAS_RELAY 1
#define RELAY_SIZE 1
#define RELAY_CHANNEL_SIZE 5
#define PROBE_SIZE 1
#define PROBE_SENSOR_SIZE 3
#define INDICATOR_SIZE 8


// APP 固件版本号
#define APP_FIRMWARE_MAJOR 3
#define APP_FIRMWARE_MINOR 0
#define APP_FIRMWARE_REVISION 0
#define APP_FIRMWARE_BUILD 0
// APP 默认固件版本号
#define APP_FIRMWARE_MAJOR_DEFAULT 3
#define APP_FIRMWARE_MINOR_DEFAULT 0
#define APP_FIRMWARE_REVISION_DEFAULT 0
#define APP_FIRMWARE_BUILD_DEFAULT 0
// 硬件版本号
#define APP_HARDWARE_MAJOR 1
#define APP_HARDWARE_MINOR 0
#define APP_HARDWARE_REVISION 0
#define APP_HARDWARE_BUILD 0
// FORMATTER 固件版本号
#define FORMATTER_FIRMWARE_MAJOR 3
#define FORMATTER_FIRMWARE_MINOR 0
#define FORMATTER_FIRMWARE_REVISION 0
#define FORMATTER_FIRMWARE_BUILD 0


#define BOARD_TYPE_S230 5
#define BOARD_TYPE_Y520 25

#ifndef BOARD_TYPE
#define BOARD_TYPE BOARD_TYPE_S230
#endif

#if (BOARD_TYPE == BOARD_TYPE_Y520)
#include "board_config_y520.h"
#elif (BOARD_TYPE == BOARD_TYPE_S230)
#include "board_config_s230.h"
#else
#error "Please set correct BOARD_TYPE"
#endif

#ifdef __cplusplus
}
#endif

#endif
