#include "bsp_mcu.h"
#include "app.h"

#ifdef __cplusplus
extern "C"
{
#endif
int BSP_MCU_temperature(float *t)
{
   *t = 23.1;
    return APP_OK;
}

int BSP_MCU_reset(void)
{
   
    return APP_OK;
}

#ifdef __cplusplus
}
#endif