
/*
PH传感器使用文档为《PHG-206A-S在线PH传感器用户手册》
主要寄存器包括：
    测量值寄存器——0000 (前2Byte为数值，后2Byte为数值的小数位数) 
    地址寄存器——2002 (厂家默认为 06) 

设备默认波特率 9600 8N1 
厂家默认地址为06，我们需要修改为02 (相关设置命令为: 06 06 20 02 00 02 A3 BC)    
*/

#ifndef __RS485_DRIVER_SENSORS_PH260_H__
#define __RS485_DRIVER_SENSORS_PH260_H__
#ifdef __cplusplus
extern "C"
{
#endif

#include "drivers/sensors/sensors.h"

#define DEFAULT_BAUD            (9600)
#define DEFAULT_ADDR            (RS485_DRIVER_SENSOR_ID_PH)
#define DEFAULT_MV_MIN_VALUE    (0.00)
#define DEFAULT_MV_MAX_VALUE    (14.00)

// MSB格式 
#define PH_VAL_REG              (0x0000)
#define PH_ADDR_REG             (0x2002)
#define PH_RECOVERY_REG         (0x2020)


#define PH_VAL_REG_LEN          (0x2)
#define PH_ADDR_REG_LEN         (0x1)
#define PH_RECOVERY_REG_LEN     (0x1)

int RS485_DRIVER_chemins_ph260_init(void);

#ifdef __cplusplus
}
#endif
#endif
