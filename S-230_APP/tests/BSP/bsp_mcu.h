#ifndef __BSP_MCU_H
#define __BSP_MCU_H

#ifdef __cplusplus
extern "C"
{
#endif
#include "hardware_wrapper.h"

#define BSP_MCU_init()

int BSP_MCU_temperature(float * t);
int BSP_MCU_reset(void);

#ifdef __cplusplus
}
#endif

#endif
