#ifndef __APP_H
#define __APP_H

#ifdef __cplusplus
extern "C"
{
#endif
#define APP_ENABLE 1
#define APP_DISABLE 0
#define APP_ENABLED(config) ((config).enable == APP_ENABLE)

#define APP_OK 0
#define APP_ERROR -1
#define APP_MALLOC_ERROR -2

#define APP_TRUE    1 
#define APP_FALSE   0 

#ifndef ARRAY_LEN
#define ARRAY_LEN(array) (sizeof(array) / sizeof(array[0]))
#endif

#ifndef UNUSED
#define UNUSED(X) (void)X
#endif

#define APP_CHECK_RC(rc) \
    if (rc != APP_OK)    \
    {                    \
        return rc;       \
    }

#define APP_CHECK_RC_AND_RESET(rc) \
    if (rc != APP_OK)              \
    {                              \
        BSP_MCU_reset();           \
    }

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "hardware_wrapper.h"

typedef  void (*pFunction)(void);
int APP_MAIN(void);

#ifdef __cplusplus
}
#endif

#endif
