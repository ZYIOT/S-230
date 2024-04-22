#ifndef __APP_PROTOCOL_H
#define __APP_PROTOCOL_H

#ifdef __cplusplus
extern "C"
{
#endif
#include "app.h"
#include "bsp_protocol.h"
    int APP_PROTOCOL_init(void);
    void APP_PROTOCOL_task_run(void *argument);
#ifdef __cplusplus
}
#endif

#endif
