
#define PN_CODE "Y-520"
#define SN_CODE 1118

#define SUPPORT_L6                          // 气吹版本 
#define SUPPORT_TEMPERATURE_AND_HUMIDITY    // 温湿度监测 
#define SUPPORT_POWER_OFF_SENSOR            // 传感器电源(12V)关闭 
#define SUPPORT_POWER_OFF_DTU               // GPRS模块电源关闭 
#define SUPPROT_GPRS_USER_PROTOCOL          // 与GPRS模块间的user_protocol通讯 
#define SUPPORT_UART5_CONFIG_OPTION         // 使用UART5配置控制箱信息 
#define SUPPORT_CHECK_POWER_VOL_IN          // 输入电压监测 
#define SUPPORT_CHECK_POWER_CUR_OUT         // 输出电流监测 


#define UART2_PRINT_LOG // 使用UART2打印log信息 
#undef LCD12864_ENABLE

#ifdef BOARD_HAS_RELAY
#define RELAY_UART4_CAN
#endif
