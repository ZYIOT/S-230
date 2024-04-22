#ifndef __BSP_RS485_H
#define __BSP_RS485_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "rs485_sensor.h"
#include "drivers/sensors/sensors.h"

    extern rs485_port_t rs485_port;
    int BSP_RS485_init(void);
    int RS485_DRIVER_match_by_model(rs485_sensor_pt sensor, uint8_t model);
#ifdef __cplusplus
}
#endif

#endif
