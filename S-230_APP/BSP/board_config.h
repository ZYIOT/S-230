/* encoding = GBK */
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

//*** <<< Use Configuration Wizard in Context Menu >>> ***

// <o> 设备型号 <1=>S200 <2=>S210 <3=>S220 <4=>S221 <5=>S230 <10=>Y210 <11=>Y211 <12=>Y212 <20=>Y510 <21=>Y511 <22=>Y513 <23=>Y515 <24=>Y516 <25=>Y520 <30=>X210 <31=>X510
#define BOARD_TYPE  5

#define BOARD_HAS_RELAY     1
#define RELAY_SIZE          1
#define RELAY_CHANNEL_SIZE  5
#define PROBE_SIZE          1
#define PROBE_SENSOR_SIZE   3
#define INDICATOR_SIZE      8


#define BOARD_TYPE_S200 1
#define BOARD_TYPE_S210 2
#define BOARD_TYPE_S220 3
#define BOARD_TYPE_S221 4
#define BOARD_TYPE_S230 5
#define BOARD_TYPE_Y210 10
#define BOARD_TYPE_Y211 11
#define BOARD_TYPE_Y212 12
#define BOARD_TYPE_Y510 20
#define BOARD_TYPE_Y511 21
#define BOARD_TYPE_Y513 22
#define BOARD_TYPE_Y515 23
#define BOARD_TYPE_Y516 24
#define BOARD_TYPE_Y520 25
#define BOARD_TYPE_X210 30
#define BOARD_TYPE_X510 31

#if (BOARD_TYPE == BOARD_TYPE_S200)
#include "board_config_s200.h"
#elif (BOARD_TYPE == BOARD_TYPE_S210)
#include "board_config_s210.h"
#elif (BOARD_TYPE == BOARD_TYPE_S220)
#include "board_config_s220.h"
#elif (BOARD_TYPE == BOARD_TYPE_S221)
#include "board_config_s221.h"
#elif (BOARD_TYPE == BOARD_TYPE_S230)
#include "board_config_s230.h"
#elif (BOARD_TYPE == BOARD_TYPE_Y210)
#include "board_config_y210.h"
#elif (BOARD_TYPE == BOARD_TYPE_Y211)
#include "board_config_y211.h"
#elif (BOARD_TYPE == BOARD_TYPE_Y212)
#include "board_config_y212.h"
#elif (BOARD_TYPE == BOARD_TYPE_Y510)
#include "board_config_y510.h"
#elif (BOARD_TYPE == BOARD_TYPE_Y511)
#include "board_config_y511.h"
#elif (BOARD_TYPE == BOARD_TYPE_Y513)
#include "board_config_y513.h"
#elif (BOARD_TYPE == BOARD_TYPE_Y515)
#include "board_config_y515.h"
#elif (BOARD_TYPE == BOARD_TYPE_Y516)
#include "board_config_y516.h"
#elif (BOARD_TYPE == BOARD_TYPE_Y520)
#include "board_config_y520.h"
#elif (BOARD_TYPE == BOARD_TYPE_X210)
#include "board_config_x210.h"
#elif (BOARD_TYPE == BOARD_TYPE_X510)
#include "board_config_x510.h"
#else
#error "Please set correct BOARD_TYPE"
#endif

// <h> 主板定义 

// <e4> APP 版本信息 
// <o> MAJOR
// <o1> MINOR 
// <o2> REVISION 
// <o3> BUILD 
#define APP_FIRMWARE_MAJOR 3
#define APP_FIRMWARE_MINOR 0
#define APP_FIRMWARE_REVISION 0
#define APP_FIRMWARE_BUILD 0
// </e>
#define ENABLE_EDIT_APP_FIRMWARE_VERSION  0

// <e4> APP 默认版本信息 
// <o> MAJOR
// <o1> MINOR 
// <o2> REVISION 
// <o3> BUILD 
#define APP_FIRMWARE_MAJOR_DEFAULT 3
#define APP_FIRMWARE_MINOR_DEFAULT 0
#define APP_FIRMWARE_REVISION_DEFAULT 0
#define APP_FIRMWARE_BUILD_DEFAULT 0
// </e>
#define ENABLE_EDIT_APP_FIRMWARE_DEFAULT_VERSION  0

// <e4> FORMATTER 版本信息 
// <o> MAJOR
// <o1> MINOR 
// <o2> REVISION 
// <o3> BUILD 
#define FORMATTER_FIRMWARE_MAJOR 3
#define FORMATTER_FIRMWARE_MINOR 0
#define FORMATTER_FIRMWARE_REVISION 0
#define FORMATTER_FIRMWARE_BUILD 0
// </e>
#define ENABLE_EDIT_FORMATTER_FIRMWARE_VERSION  0

// <e4> 硬件信息 
// <o> MAJOR
// <o1> MINOR 
// <o2> REVISION 
// <o3> BUILD 
#define APP_HARDWARE_MAJOR 1
#define APP_HARDWARE_MINOR 0
#define APP_HARDWARE_REVISION 0
#define APP_HARDWARE_BUILD 0
// </e>
#define ENABLE_EDIT_APP_HARDWARE_VERSION  0

// </h>



//*** <<< end of configuration section >>> ***

#ifdef __cplusplus
}
#endif

#endif
