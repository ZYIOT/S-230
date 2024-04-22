/*
盐度传感器使用文档为《DDM-206A-S在线盐度传感器用户手册》
主要寄存器包括：
    测量值寄存器——0000 (前2Byte为数值，后2Byte为数值的小数位数) 
    温度寄存器——0002 (前2Byte为数值，后2Byte为数值的小数位数) 
    地址寄存器——2002 (厂家默认为 06) 

设备默认波特率 9600 8N1 
厂家默认地址为06，我们需要修改为08 (相关设置命令为: 06 06 20 02 00 08 23 BB)    
*/


#ifndef __RS485_DRIVER_SENSORS_CHEMINS_SAL610_H__
#define __RS485_DRIVER_SENSORS_CHEMINS_SAL610_H__
#ifdef __cplusplus
extern "C"
{
#endif

#include "drivers/sensors/sensors.h"

#define DEFAULT_BAUD        (9600)
#define DEFAULT_ADDR        (RS485_DRIVER_SENSOR_ID_SALINITY)
#define DEFAULT_MIN_VALUE   (0.0)
#define DEFAULT_MAX_VALUE   (70.0)
// MSB格式 
#define SALINITY_VALUE_REG      (0x0000)
#define SALINITY_TMP_REG        (0x0002)
#define SALINITY_ADDR_REG       (0x2002) 

#define SALINITY_VALUE_REG_LEN  (0x2)
#define SALINITY_TMP_REG_LEN    (0x2)
#define SALINITY_ADDR_REG_LEN   (0x1)

int RS485_DRIVER_chemins_sal610_init(void);


#ifdef __cplusplus
}
#endif
#endif
