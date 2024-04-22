// #define RELAY_CHANNEL_SIZE 5
#define PN_CODE "S-230"
#define SN_CODE 1208

#define SUPPORT_PUMP                        // 气吹版本 
#define SUPPORT_TEMPERATURE_AND_HUMIDITY    // 温湿度监测 
#define SUPPORT_POWER_OFF_SENSOR            // 传感器电源(12V)关闭 
#define SUPPORT_POWER_OFF_DTU               // GPRS模块电源关闭 
#define SUPPROT_GPRS_USER_PROTOCOL          // 与GPRS模块间的user_protocol通讯 
#define SUPPORT_UART5_CONFIG_OPTION         // 使用UART5配置控制箱信息 

#define UART2_PRINT_LOG // 使用UART2打印log信息 
#define SUPPORT_TFTLCD  // 支持TFTLCD实时显示传感器数据 
#define SUPPORT_IRED    // 支持人体红外传感器 

#undef BOARD_HAS_RELAY  


#ifndef BOARD_HAS_RELAY
#undef RELAY_UART4_CAN 
#endif

#ifdef SUPPORT_TFTLCD
#define LCD_SHOW_DIR_HORIZONTAL 0   // 横屏 
#define LCD_SHOW_DIR_VERTICAL   1   // 竖屏 
#define LCD_SHOW_DIR    (LCD_SHOW_DIR_HORIZONTAL)

#define LCD_SHOW_MODE0  0   // 不叠加显示 
#define LCD_SHOW_MODE1  1   // 叠加显示 
#define LCD_SHOW_MODE   (LCD_SHOW_MODE0)

#define LCD_BACKGROUND_COLOR    WHITE
#define LCD_ERROR_COLOR         RED 
#endif
