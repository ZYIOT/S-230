#ifndef __BSP_LINK_H
#define __BSP_LINK_H

#ifdef __cplusplus
extern "C"
{
#endif
#include "app.h"
#include "main.h"
#include "hardware_wrapper.h"

#define BSP_LINK_connect_read() HARDWARE_GPIO_READ_PIN(LINK_GPIO_Port, LINK_Pin)
#define BSP_LINK_csq_read() HARDWARE_GPIO_READ_PIN(LINK1_GPIO_Port, LINK1_Pin)

#ifdef __cplusplus
}
#endif
#endif
