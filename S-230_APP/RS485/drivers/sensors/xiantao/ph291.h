
/*
PH传感器使用文档为《氨氮变送器485型说明书.pdf》
主要寄存器包括：
    氨氮浓度值寄存器 —— 0000 (量程10和100的设备为实际值的100倍，量程1000的设备为实际值的10倍) 
    PH值寄存器 —— 0001 (实际值的100倍) 
    温度值寄存器 —— 0002 (实际值的10倍) 
    地址寄存器——2002 (厂家默认为 01) 

设备默认波特率 4800 8N1 ，默认地址为06，
我们需要修改 波特率为9600，设备地址为 02 
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
