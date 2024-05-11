// encoding = GBK 
#ifndef __BOARD_CONFIG_Y520_H__
#define __BOARD_CONFIG_Y520_H__
#ifdef __cplusplus
extern "C"
{
#endif

//*** <<< Use Configuration Wizard in Context Menu >>> ***

// <h> ������Ϣ 
// <s> PN 
#define PN_CODE "Y-520"
// <o> SN 
#define SN_CODE 1118
// </h>

// <h> ֧�ֵĹ��� 

// <c> ֧��L6 
#define SUPPORT_L6                          // �����汾 
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

// <c> ֧�ֽ��ߵ�ѹ��� 
#define SUPPORT_CHECK_POWER_VOL_IN          // �����ߵĵ�ѹ 
// </c>

// <c> ֧�ָ������������� 
#define SUPPORT_CHECK_POWER_CUR_OUT         // ��⸺��������� 
// </c>

// <c> ֧��UART2��ӡlog��Ϣ 
#define UART2_PRINT_LOG // ʹ��UART2��ӡlog��Ϣ 
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
