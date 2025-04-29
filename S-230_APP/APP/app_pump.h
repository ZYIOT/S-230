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
    uint8_t lastStatus;       //上一次上传的状态
    uint8_t changed;           //是否需要上传状态
    uint8_t statusOnChecking; //还在检查状态
    uint8_t check;
    uint16_t frequency[_APP_PUMP_FREQUENCY_ARRAY_LEN];
    uint16_t errorCount;
} APP_PUMP_t;
extern APP_PUMP_t g_appPump;

#define APP_PUMP_STATUS_ON      1
#define APP_PUMP_STATUS_OFF     0
#define APP_PUMP_CHECK_NORMAL   0
#define APP_PUMP_CHECK_ERROR    1

int APP_PUMP_Init(void);
int APP_PUMP_On(void);
int APP_PUMP_Off(void);
int APP_PUMP_Refresh(uint8_t refresh_status);
int APP_PUMP_Check(void);
int APP_PUMP_Changed(int *changed);
int APP_PUMP_clear_error_count(void);
void APP_PUMP_SetCheckFrequency(uint16_t fq);
void APP_PUMP_task_run(void *argument);

#ifdef __cplusplus
}
#endif

#endif
