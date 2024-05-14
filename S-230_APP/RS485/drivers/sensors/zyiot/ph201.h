

#ifndef __RS485_DRIVER_SENSORS_ZYIOT_PH201_H
#define __RS485_DRIVER_SENSORS_ZYIOT_PH201_H
#ifdef __cplusplus
extern "C"
{
#endif

#include "drivers/sensors/sensors.h"


#define PH201_VALUE_REG     (0x0)
#define PH201_CALIBRATE_REG (0x4)
#define PH201_SENSOR_ID_REG (0xA)

int RS485_DRIVER_zyiot_ph201_init(void);

#ifdef __cplusplus
}
#endif
#endif
