#ifndef __BSP_HAL_H
#define __BSP_HAL_H

#ifdef __cplusplus
extern "C"
{
#endif
#include "main.h"
#include "board_config.h"

int BSP_HAL_init(void);
extern void APP_TASK_RunPerSecondIsr(void);
#ifdef __cplusplus
}
#endif

#endif
