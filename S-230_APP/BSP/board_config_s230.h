// encoding = GBK 
#ifndef __BOARD_CONFIG_S230_H__
#define __BOARD_CONFIG_S230_H__

#ifdef __cplusplus
extern "C"
{
#endif

//*** <<< Use Configuration Wizard in Context Menu >>> ***
// <h> 板载信息 
// <s> PN 
#define PN_CODE "S-230"
// <o> SN 
#define SN_CODE 1208 
// </h>

// <h> 支持的功能 

// <c> 支持 PUMP 
#define SUPPORT_PUMP // 水泵版本 
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

// <c> 支持UART2打印log信息 
#define UART2_PRINT_LOG // 使用UART2打印log信息 
// </c>

// <c> 支持TFTLCD实时显示传感器数据 
// #define SUPPORT_TFTLCD  // 支持TFTLCD屏实时显示传感器数据 
// </c>

// <c> 支持人体红外控制LCD屏亮灭 
// #define SUPPORT_IRED    // 支持人体红外传感器 
// </c>

// </h>
    


#undef BOARD_HAS_RELAY  


#ifndef BOARD_HAS_RELAY
#undef RELAY_UART4_CAN 
#endif

#define LCD_SHOW_DIR_HORIZONTAL 0   // 横屏 
#define LCD_SHOW_DIR_VERTICAL   1   // 竖屏 
#define LCD_SHOW_MODE0  0   // 不叠加显示 
#define LCD_SHOW_MODE1  1   // 叠加显示 
#define LCD_BACKGROUND_COLOR    WHITE
#define LCD_ERROR_COLOR         RED 

#define LCD_SHOW_DIR    (LCD_SHOW_DIR_HORIZONTAL)
#define LCD_SHOW_MODE   (LCD_SHOW_MODE0)

//*** <<< end of configuration section >>> ***

#ifdef __cplusplus
}
#endif
#endif
