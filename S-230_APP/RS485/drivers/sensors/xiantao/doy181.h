/*
DO传感器使用文档为《溶解氧变速器说明书.pdf》
主要寄存器包括：
    饱和度寄存器 —— 0000 (1为100%，浮点数，大端) 
    DO值寄存器——0002 (mg/L，浮点数，大端) 
    地址寄存器——2000 (厂家默认为 01) 

设备默认波特率 4800 8N1 ，我们需要修改波特率为 9600 
厂家默认地址为01 
*/


#ifndef __RS485_DRIVER_SENSORS_XT_DOY181_H__
#define __RS485_DRIVER_SENSORS_XT_DOY181_H__
#ifdef __cplusplus
extern "C"
{
#endif

#include "drivers/sensors/sensors.h"

#define DOY181_SATURATION_VALUE_REG (0x0000)
#define DOY181_DO_VALUE_REG         (0x0002)
#define DOY181_TMP_VALUE_REG        (0x0004)
#define DOY181_SENSOR_ID_REG        (0x07D0)
#define DOY181_BAUDRATE_REG         (0x07D1)
#define DOY181_CALIBRATE_REG        (0x1010)
#define DOY181_COMPENSATION_REG     (0x1020)

#define DOY181_SATURATION_REG_LEN   (0x02) 
#define DOY181_DO_REG_LEN           (0x02) 
#define DOY181_SENSOR_ID_REG_LEN    (0x01) 
#define DOY181_BAUDRATE_REG_LEN     (0x01) 
#define DOY181_CALIBRATE_REG_LEN    (0x01) 
#define DOY181_COMPENSATION_REG_LEN (0x01) 

#define SATURATION_MAX_VALUE        (2.00f) 
#define SATURATION_MIN_VALUE        (0) 

#define DOY181_BAUDRATE_2400        (0)
#define DOY181_BAUDRATE_4800        (1)
#define DOY181_BAUDRATE_9600        (2)
#define DOY181_BAUDRATE_19200       (3)
#define DOY181_BAUDRATE_38400       (4)
#define DOY181_BAUDRATE_57600       (5)
#define DOY181_BAUDRATE_115200      (6)

int RS485_DRIVER_xiantao_doy181_init(void);

#ifdef __cplusplus
}
#endif
#endif