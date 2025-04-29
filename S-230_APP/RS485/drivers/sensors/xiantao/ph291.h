
/*
PH������ʹ���ĵ�Ϊ������������485��˵����.pdf��
��Ҫ�Ĵ���������
    ����Ũ��ֵ�Ĵ��� ���� 0000 (����10��100���豸Ϊʵ��ֵ��100��������1000���豸Ϊʵ��ֵ��10��) 
    PHֵ�Ĵ��� ���� 0001 (ʵ��ֵ��100��) 
    �¶�ֵ�Ĵ��� ���� 0002 (ʵ��ֵ��10��) 
    ��ַ�Ĵ�������2002 (����Ĭ��Ϊ 01) 

�豸Ĭ�ϲ����� 4800 8N1 ��Ĭ�ϵ�ַΪ06��
������Ҫ�޸� ������Ϊ9600���豸��ַΪ 02 
*/
#ifndef __RS485_DRIVER_SENSORS_XT_PH291_H__
#define __RS485_DRIVER_SENSORS_XT_PH291_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "drivers/sensors/sensors.h"


#define PH291_VALUE_REG         (0x0000) 
#define PH291_SENSOR_ID_REG     (0x07D0) 
#define PH291_BAUDRATE_REG      (0x07D1) 

#define PH291_VALUE_REG_LEN         (0x03) 
#define PH291_SENSOR_ID_REG_LEN     (0x01) 
#define PH291_BAUDRATE_REG_LEN      (0x01) 

#define DEFAULT_PH_MIN_VALUE    (0.00f)
#define DEFAULT_PH_MAX_VALUE    (14.00f)

#define PH291_BAUDRATE_2400         (0)
#define PH291_BAUDRATE_4800         (1)
#define PH291_BAUDRATE_9600         (2)
#define PH291_BAUDRATE_19200        (3)
#define PH291_BAUDRATE_38400        (4)
#define PH291_BAUDRATE_57600        (5)
#define PH291_BAUDRATE_115200       (6)
#define PH291_BAUDRATE_1200         (7)

int RS485_DRIVER_xt_ph291_init(void);

#ifdef __cplusplus
}
#endif

#endif
