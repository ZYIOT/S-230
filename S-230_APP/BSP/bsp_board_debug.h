// encoding = GBK 

#ifndef __BSP_BOARD_DEBUG_H__
#define __BSP_BOARD_DEBUG_H__

#ifdef __cplusplus
extern "C"
{
#endif

//*** <<< Use Configuration Wizard in Context Menu >>> ***

// <h> 调试相关宏定义 

// <h> Log打印相关配置 
// <e1> 使能Log功能  
    // <o> Log等级  <0=> NONE    <1=> ERROR  <2=> WARN  <3=> INFO  <4=> DEBUG  <5=> TRACE 
    #define BSP_LOG_DEFAULT_LEVEL 4
    // <c> 显示RS485通讯数据 
    #define SHOW_RS485_DATA 
    // </c>
    // <c> 显示PROTOCOL通讯数据 
    #define SHOW_PROTOCOL_DATA 
    // </c>
    // <c> 显示GPRS通讯数据 
    #define SHOW_GPRS_DATA 
    // </c>
    // <c> 显示设备time 
    #define SHOW_DEVICE_TIME 
    // </c>
    // <c> 显示LCD的传感器状态 
    //#define SHOW_LCD_SENSOR_STATUS 
    // </c>
    // <c> 显示pump状态 
    // #define SHOW_PUMP_STATUS
    // </c>
// </e>
#define ENABLE_LOG_TRACE 1
// </h>

#if (0 == ENABLE_LOG_TRACE)
    #undef ENABLE_LOG_TRACE 
    #define BSP_LOG_DEFAULT_LEVEL 5 
    #ifdef SHOW_RS485_DATA 
    #undef SHOW_RS485_DATA 
    #endif
    #ifdef SHOW_PROTOCOL_DATA 
    #undef SHOW_PROTOCOL_DATA 
    #endif 
    #ifdef SHOW_GPRS_DATA 
    #undef SHOW_GPRS_DATA 
    #endif
    #ifdef SHOW_DEVICE_TIME
    #undef SHOW_DEVICE_TIME
    #endif
    #ifdef SHOW_LCD_SENSOR_STATUS
    #undef SHOW_LCD_SENSOR_STATUS
    #endif
    #ifdef SHOW_PUMP_STATUS 
    #undef SHOW_PUMP_STATUS
    #endif
#else    
#endif 

// <c> 关闭看门狗 
 //#define DISABLE_WDG
// </c>

//<c> 调试 PH202 的 ADC 值 
//#define PH202_ADC_DEBUG 
// </c>

//<c> 使能 DO 传感器有默认值  
#define ENABLE_DO_SENSOR_DEFAULT 
// </c>
// #define USE_NEW_SENSOR_SCAN_DRIVER

// </h>


//*** <<< end of configuration section >>> ***

#ifdef __cplusplus
}
#endif
#endif
