#ifndef __BSP_BL0910_H
#define __BSP_BL0910_H

#ifdef __cplusplus
extern "C"
{
#endif
#include "bl0910.h"

extern BL0910_TypeDef bl0910;

#ifdef SUPPORT_CHECK_VOLTAGE
#define BSP_BL0910_init() BL0910_init(&bl0910)
#define BSP_BL0910_reset() BL0910_reset(&bl0910);
#define BSP_BL0910_system_reset() BL0910_system_reset(&bl0910);
#define BSP_BL0910_register_reset() BL0910_register_reset(&bl0910);
#define BSP_BL0910_read(data) BL0910_read(&bl0910, data);
#else
#define BSP_BL0910_init()       (0)
#define BSP_BL0910_reset()
#define BSP_BL0910_system_reset()
#define BSP_BL0910_register_reset()
#define BSP_BL0910_read(data)   (0)
#endif
#ifdef __cplusplus
}

#endif

#endif
