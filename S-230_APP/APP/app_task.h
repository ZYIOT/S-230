#ifndef __APP_TASK_H
#define __APP_TASK_H

#ifdef __cplusplus
extern "C"
{
#endif
#include "app.h"
    int APP_TASK_init(void);
    void APP_TASK_run(void);
    void APP_TASK_run_per_second_ISR(void);
    osThreadId_t APP_TASK_get_status_handle(void);
    
    void APP_TASK_create_update_finish_task(void);
    void APP_TASK_create_update_continue_task(void);
    void APP_TASK_create_update_communication_module_task(void);
    void APP_TASK_create_check_net_task(void);

#ifdef __cplusplus
}
#endif

#endif
