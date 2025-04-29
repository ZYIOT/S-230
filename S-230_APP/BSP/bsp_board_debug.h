// encoding = GBK 

#ifndef __BSP_BOARD_DEBUG_H__
#define __BSP_BOARD_DEBUG_H__

#ifdef __cplusplus
extern "C"
{
#endif

//*** <<< Use Configuration Wizard in Context Menu >>> ***

// <h> ������غ궨�� 

// <h> Log��ӡ������� 
// <e1> ʹ��Log����  
    // <o> Log�ȼ�  <0=> NONE    <1=> ERROR  <2=> WARN  <3=> INFO  <4=> DEBUG  <5=> TRACE 
    #define BSP_LOG_DEFAULT_LEVEL 4
    // <c> ��ʾRS485ͨѶ���� 
    #define SHOW_RS485_DATA 
    // </c>
    // <c> ��ʾPROTOCOLͨѶ���� 
    #define SHOW_PROTOCOL_DATA 
    // </c>
    // <c> ��ʾGPRSͨѶ���� 
    #define SHOW_GPRS_DATA 
    // </c>
    // <c> ��ʾ�豸time 
    #define SHOW_DEVICE_TIME 
    // </c>
    // <c> ��ʾLCD�Ĵ�����״̬ 
    //#define SHOW_LCD_SENSOR_STATUS 
    // </c>
    // <c> ��ʾpump״̬ 
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

// <c> �رտ��Ź� 
 //#define DISABLE_WDG
// </c>

//<c> ���� PH202 �� ADC ֵ 
//#define PH202_ADC_DEBUG 
// </c>

//<c> ʹ�� DO ��������Ĭ��ֵ  
#define ENABLE_DO_SENSOR_DEFAULT 
// </c>
// #define USE_NEW_SENSOR_SCAN_DRIVER

// </h>


//*** <<< end of configuration section >>> ***

#ifdef __cplusplus
}
#endif
#endif
