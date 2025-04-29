#ifndef __RS485_DRIVER_SENSORS_ZYIOT_DO_DEFAULT_H
#define __RS485_DRIVER_SENSORS_ZYIOT_DO_DEFAULT_H
#ifdef __cplusplus
extern "C"
{
#endif

#include "drivers/sensors/sensors.h"

#define DO_DEFAULT_VALUE_REG        (0x0)
#define DO_DEFAULT_CALIBRATE_REG    (0x7)
#define DO_DEFAULT_SENSOR_ID_REG    (0x9)
#define DO_DEFAULT_COMPENSATION_REG (0xA)

#define DO_DEFAULT_DO_VALUE_MIN     (0.00f) 
#define DO_DEFAULT_DO_VALUE_MAX     (21.00f)
#define DO_DEFAULT_TMP_VALUE_MIN    (0.0f)
#define DO_DEFAULT_TMP_VALUE_MAX    (51.0f)

int RS485_DRIVER_zyiot_do_default_init(void);

#ifdef __cplusplus
}
#endif
#endif
