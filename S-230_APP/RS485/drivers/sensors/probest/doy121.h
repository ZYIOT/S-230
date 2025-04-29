#ifndef __RS485_DRIVER_SENSORS_PROBEST_DOY121_H
#define __RS485_DRIVER_SENSORS_PROBEST_DOY121_H
#ifdef __cplusplus
extern "C"
{
#endif

#include "drivers/sensors/sensors.h"

#define DOY121_VALUE_REG        (0x0)
#define DOY121_CALIBRATE_REG    (0x7)
#define DOY121_SENSOR_ID_REG    (0x9)
#define DOY121_COMPENSATION_REG (0xA)
#define DOY121_BAUDRATE_REG_RO  (0xF)   // 0:4800 , 1:9600 , 2:19200 , 3:38400 
#define DOY121_BAUDRATE_REG_WO  (0x10)  // 0:4800 , 1:9600 , 2:19200 , 3:38400 

int RS485_DRIVER_probest_doy121_init(void);

#ifdef __cplusplus
}
#endif
#endif
