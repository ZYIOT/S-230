#ifndef __BSP_MCU_H
#define __BSP_MCU_H

#ifdef __cplusplus
extern "C"
{
#endif
#include "main.h"

#define BSP_MCU_init()


int BSP_MCU_reset(void);
void BSP_MCU_run_led_flash(void);

#ifdef __cplusplus
}
#endif

#endif
