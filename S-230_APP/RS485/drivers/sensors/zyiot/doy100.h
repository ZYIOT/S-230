#ifndef __RS485_DRIVER_SENSORS_ZYIOT_DOY100_H
#define __RS485_DRIVER_SENSORS_ZYIOT_DOY100_H
#ifdef __cplusplus
extern "C"
{
#endif

#include "drivers/sensors/sensors.h"

#define DOY100_VALUE_REG        (0x0)
#define DOY100_CALIBRATE_REG    (0x7)
#define DOY100_SENSOR_ID_REG    (0x9)
#define DOY100_COMPENSATION_REG (0xA)

int RS485_DRIVER_zyiot_doy100_init(void);

#ifdef __cplusplus
}
#endif
#endif
