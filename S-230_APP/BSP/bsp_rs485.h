#ifndef __BSP_RS485_H
#define __BSP_RS485_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "main.h"
#include "usart.h"
#include "bsp_usart.h"
#include "rs485_sensor.h"
#include "drivers/sensors/sensors.h"

    extern rs485_port_t rs485_port;
    int BSP_RS485_init(void);

#ifdef __cplusplus
}
#endif

#endif
