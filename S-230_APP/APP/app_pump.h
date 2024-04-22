#ifndef __APP_PUMP_H
#define __APP_PUMP_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "bsp_pump.h"

#define _APP_PUMP_FREQUENCY_ARRAY_LEN 4
    typedef struct
    {
        uint8_t status;
        uint8_t last_status;       //上一次上传的状态
        uint8_t changed;           //是否需要上传状态
        uint8_t status_on_checking; //还在检查状态
        uint8_t check;
        uint16_t frequency[_APP_PUMP_FREQUENCY_ARRAY_LEN];
        uint16_t error_count;
    } app_pump_t;
    extern app_pump_t app_pump;

#define APP_PUMP_STATUS_ON 1
#define APP_PUMP_STATUS_OFF 0
#define APP_PUMP_CHECK_NORMAL 0
#define APP_PUMP_CHECK_ERROR 1

    int APP_PUMP_init(void);
    int APP_PUMP_on(void);
    int APP_PUMP_off(void);
    int APP_PUMP_refresh(uint8_t refresh_status);
    int APP_PUMP_check(void);
    int APP_PUMP_changed(int *changed);
    int APP_PUMP_clear_error_count(void);
    void APP_PUMP_set_check_frequency(uint16_t fq);
    void APP_PUMP_task_run(void *argument);
#ifdef __cplusplus
}
#endif

#endif
