#ifndef __RS485_DRIVER_SENSORS_SZKLT_PH230_H
#define __RS485_DRIVER_SENSORS_SZKLT_PH230_H
#ifdef __cplusplus
extern "C"
{
#endif

#include "drivers/sensors/sensors.h"

#define PH230_VALUE_REG     (0x0)
#define PH230_CALIBRATE_REG (0x4)
#define PH230_SENSOR_ID_REG (0xA)

int RS485_DRIVER_szklt_ph230_init(void);
#ifdef __cplusplus
}
#endif
#endif
