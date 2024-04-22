#ifndef __BSP_RNG_H
#define __BSP_RNG_H

#ifdef __cplusplus
extern "C"
{
#endif
#include "hardware_wrapper.h"

void BSP_RNG_init(void);
void BSP_RNG_deinit(void);
uint32_t BSP_RNG_get(void);


#ifdef __cplusplus
}
#endif

#endif
