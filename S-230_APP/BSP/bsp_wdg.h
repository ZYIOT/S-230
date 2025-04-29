#ifndef __BSP_WDG_H
#define __BSP_WDG_H
#ifdef __cplusplus
extern "C"
{
#endif
#include "iwdg.h"
    void BSP_WDG_init(void);
    void BSP_WDG_feed(void);
    void BSP_WDG_stop(void);
#ifdef __cplusplus
}
#endif

#endif
