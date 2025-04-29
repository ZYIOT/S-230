
/*
1、液位传感器使用的文档为《毫米 V1.5版 IP68超声波水位计 MODBUS通讯协议2018.11.26 .doc》。
2、主要的寄存器包括：
	距离/物位值寄存器——00
	参考零点寄存器——2A
	测量模式、单位选择寄存器——5E
	波特率、工作方式寄存器——62
	表型字、仪表地址寄存器——6B
	
设备默认波特率为 9600 。
首先厂家默认的仪表地址为 01，根据我们实际需求，需要修改为 07 （相关设置命令为：01 06 00 6B 00 07 B9 D4）
根据厂家说明，设备测量值的单位固定为 mm ，修改寄存器无效
然后修改设备的测量模式(5E寄存器的前一个字节buf[0])，修改为测量物位值（相关设置命令为：07 06 00 5E 01 02 68 2F）
再设置参考零点（单位为 cm，高字节在前），譬如修改为 500cm （相关设置命令为：07 06 00 2A 01 F4 A8 73）

最后读取物位值寄存器，获得物位值（物位值 = 参考零点 - 设备到液面的距离，高字节在前，相关读取命令为：07 03 00 00 00 01 84 6C）
*/
#ifndef __RS485_DRIVER_SENSORS_ANBULEILA_WL991_H__
#define __RS485_DRIVER_SENSORS_ANBULEILA_WL991_H__
#ifdef __cplusplus
extern "C"
{
#endif

#include "drivers/sensors/sensors.h"

#define REFERENCE_ZERO_DEPTH    (500)  // 设置默认参考零点，即传感器距离底部的距离(cm)

#define DEFAULT_WL991_DEV       (0x7)   // 设置的默认设备地址
#define DEFAULT_WL991_BAUD      (9600)  // 默认波特率

// LSB格式
#define WL991_DISTANCE_REG      (0x0)   // 距离寄存器(buf[0:1])
#define WL991_VOLTAGE_REG       (0x1)   // 模拟输出寄存器
#define WL991_TEM_REG           (0x2)   // 温度寄存器
#define WL991_REF_REG           (0x2A)  // 参考零点寄存器(buf[0])
#define WL991_MES_MAX_REG       (0x2B)  // 量程高点寄存器(buf[0:1])
#define WL991_MODE_REG          (0x5E)  // 测量模式寄存器(buf[0],0:距离, 1:物位)
#define WL991_BAUD_REG          (0x62)  // 波特率寄存器(buf[0],0:2400, 1:4800, 2:9600, 3:19200)
#define WL991_DEV_REG           (0x6B)  // 设备地址寄存器(buf[1])

int RS485_DRIVER_level_anbuleila_init(void);

#ifdef __cplusplus
}
#endif
#endif
