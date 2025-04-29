// encoding = GBK 
#ifndef __BOARD_CONFIG_S230_H__
#define __BOARD_CONFIG_S230_H__

#ifdef __cplusplus
extern "C"
{
#endif

//*** <<< Use Configuration Wizard in Context Menu >>> ***
// <h> ������Ϣ 
// <s> PN 
#define PN_CODE "S-230"
// <o> SN 
#define SN_CODE 1208 
// </h>

// <h> ֧�ֵĹ��� 

// <c> ֧�� PUMP 
#define SUPPORT_PUMP // ˮ�ð汾 
// </c>

// <c> ֧����ʪ�ȼ�� 
#define SUPPORT_TEMPERATURE_AND_HUMIDITY    // ��ʪ�ȼ�� 
// </c>

// <c> ֧�ֿ��ش�����12V��Դ 
#define SUPPORT_POWER_OFF_SENSOR            // ��������Դ(12V)�ر� 
// </c>

// <c> ֧�ֿ���GPRSģ���Դ 
#define SUPPORT_POWER_OFF_DTU               // GPRSģ���Դ�ر� 
// </c>

// <c> ֧��USER_PROTOCOLͨѶ 
#define SUPPROT_GPRS_USER_PROTOCOL          // ��GPRSģ����user_protocolͨѶ 
// </c>

// <c> ֧��UART5���ð�����Ϣ 
#define SUPPORT_UART5_CONFIG_OPTION         // ʹ��UART5���ÿ�������Ϣ 
// </c>

// <c> ֧��UART2��ӡlog��Ϣ 
#define UART2_PRINT_LOG // ʹ��UART2��ӡlog��Ϣ 
// </c>

// <c> ֧��TFTLCDʵʱ��ʾ���������� 
// #define SUPPORT_TFTLCD  // ֧��TFTLCD��ʵʱ��ʾ���������� 
// </c>

// <c> ֧������������LCD������ 
// #define SUPPORT_IRED    // ֧��������⴫���� 
// </c>

// </h>
    


#undef BOARD_HAS_RELAY  


#ifndef BOARD_HAS_RELAY
#undef RELAY_UART4_CAN 
#endif

#define LCD_SHOW_DIR_HORIZONTAL 0   // ���� 
#define LCD_SHOW_DIR_VERTICAL   1   // ���� 
#define LCD_SHOW_MODE0  0   // ��������ʾ 
#define LCD_SHOW_MODE1  1   // ������ʾ 
#define LCD_BACKGROUND_COLOR    WHITE
#define LCD_ERROR_COLOR         RED 

#define LCD_SHOW_DIR    (LCD_SHOW_DIR_HORIZONTAL)
#define LCD_SHOW_MODE   (LCD_SHOW_MODE0)

//*** <<< end of configuration section >>> ***

#ifdef __cplusplus
}
#endif
#endif
