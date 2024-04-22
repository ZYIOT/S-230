#ifndef __RS485_DRIVER_SENSORS_FIVE_PHASE_DOY170_H
#define __RS485_DRIVER_SENSORS_FIVE_PHASE_DOY170_H
#ifdef __cplusplus
extern "C"
{
#endif

#include "drivers/sensors/sensors.h"

#define DOY170_CALIBRATE_REG    (0x7)
#define DOY170_COMPENSATION_REG (0xA)
#define DOY170_VALUE_REG        (0x28)
#define DOY170_SENSOR_ID_REG    (0x140)

int RS485_DRIVER_five_phase_doy170_init(void);

#ifdef __cplusplus
}
#endif
#endif
