#ifndef __APP_TASK_H
#define __APP_TASK_H

#ifdef __cplusplus
extern "C"
{
#endif
#include "app.h"
    int APP_TASK_init(void);
    void APP_TASK_run(void);
    void APP_TASK_RunPerSecondIsr(void);
    osThreadId_t APP_TASK_get_status_handle(void);
    
    void APP_TASK_CreateUpdateFinishTask(void);
    void APP_TASK_create_update_continue_task(void);
    void APP_TASK_create_update_communication_module_task(void);

#ifdef __cplusplus
}
#endif

#endif
