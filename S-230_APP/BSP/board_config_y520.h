// encoding = GBK 
#ifndef __BOARD_CONFIG_Y520_H__
#define __BOARD_CONFIG_Y520_H__
#ifdef __cplusplus
extern "C"
{
#endif

//*** <<< Use Configuration Wizard in Context Menu >>> ***

// <h> 板载信息 
// <s> PN 
#define PN_CODE "Y-520"
// <o> SN 
#define SN_CODE 1118
// </h>

// <h> 支持的功能 

// <c> 支持L6 
#define SUPPORT_L6                          // 气吹版本 
// </c>

// <c> 支持温湿度监测 
#define SUPPORT_TEMPERATURE_AND_HUMIDITY    // 温湿度监测 
// </c>

// <c> 支持开关传感器12V电源 
#define SUPPORT_POWER_OFF_SENSOR            // 传感器电源(12V)关闭 
// </c>

// <c> 支持开关GPRS模块电源 
#define SUPPORT_POWER_OFF_DTU               // GPRS模块电源关闭 
// </c>

// <c> 支持USER_PROTOCOL通讯 
#define SUPPROT_GPRS_USER_PROTOCOL          // 与GPRS模块间的user_protocol通讯 
// </c>

// <c> 支持UART5配置板载信息 
#define SUPPORT_UART5_CONFIG_OPTION         // 使用UART5配置控制箱信息 
// </c>

// <c> 支持进线电压检测 
#define SUPPORT_CHECK_POWER_VOL_IN          // 检测进线的电压 
// </c>

// <c> 支持负载输出电流检测 
#define SUPPORT_CHECK_POWER_CUR_OUT         // 检测负载输出电流 
// </c>

// <c> 支持UART2打印log信息 
#define UART2_PRINT_LOG // 使用UART2打印log信息 
// </c>

// </h>

#undef LCD12864_ENABLE

#ifdef BOARD_HAS_RELAY
#define RELAY_UART4_CAN
#endif

//*** <<< end of configuration section >>> ***

#ifdef __cplusplus
}
#endif
#endif
