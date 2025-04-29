#include "app.h"
#include "app_log.h"
#include "app_pump.h"
#include "hardware_wrapper.h"
#include "utils.h"
#include "app_sensors.h"

#ifdef SHOW_PUMP_STATUS
#define APP_LOG_PUMP_STATUS(...)    APP_LOG_Debug(##__VA_ARGS__)
#else
#define APP_LOG_PUMP_STATUS(...)
#endif

APP_PUMP_t g_appPump;
static uint8_t s_checkCount = 0;

static void IncrementErrorCount(void)
{
    g_appPump.errorCount++;
}

int APP_PUMP_Init(void)
{
    BSP_PUMP_init();
    g_appPump.status = APP_PUMP_STATUS_OFF;
    g_appPump.lastStatus = APP_PUMP_STATUS_OFF;
    g_appPump.changed = 0;
    g_appPump.statusOnChecking = 0;
    g_appPump.check = APP_PUMP_CHECK_NORMAL;
    g_appPump.errorCount = 0;
    for (int i = 0; i < _APP_PUMP_FREQUENCY_ARRAY_LEN; i++)
    {
        g_appPump.frequency[i] = 0;
    }
    return APP_OK;
}

int APP_PUMP_On(void)
{
    if (g_appPump.status != APP_PUMP_STATUS_ON)
    {
        s_checkCount = 0;
        g_appPump.status = APP_PUMP_STATUS_ON;
    }
    BSP_PUMP_on();
    return APP_OK;
}

int APP_PUMP_Off(void)
{
    if (g_appPump.status != APP_PUMP_STATUS_OFF)
    {
        s_checkCount = 0;
        g_appPump.status = APP_PUMP_STATUS_OFF;
    }

    BSP_PUMP_off();
    return APP_OK;
}

#include "app_config/defines.h"
volatile uint32_t vs_frequency = 0;
volatile uint8_t vs_pumpStatus = 0;

int APP_PUMP_Refresh(uint8_t refreshStatus)
{
    if (refreshStatus > 0)
    {
        g_appPump.status = APP_PUMP_STATUS_ON;
        APP_PUMP_On();
    }
    else
    {
        g_appPump.status = APP_PUMP_STATUS_OFF;
        g_appPump.check = APP_PUMP_CHECK_NORMAL;
        APP_PUMP_Off();
    }
    return APP_OK;
}

void APP_PUMP_task_run(void *argument)
{
    uint8_t last_check_status = 0;
    uint8_t check_status = 0;
    APP_CONFIG_PumpStatus_pt cmsg = &g_appConfigPumpStatus[0];
    for (;;)
    {
        APP_PUMP_Refresh(cmsg->status[0]);
        check_status = BSP_PUMP_read_status();
        if (last_check_status != check_status)
        {
            last_check_status = check_status;
            vs_frequency++;
        }
    }
}

void APP_PUMP_SetCheckFrequency(uint16_t fq)
{
    static int i = 0;
    APP_LOG_PUMP_STATUS("\r\npump frequency: %u\r\n", vs_frequency);
    g_appPump.frequency[i] = vs_frequency;
    i++;
    vs_frequency = 0;
    i %= _APP_PUMP_FREQUENCY_ARRAY_LEN;
    APP_PUMP_Check();
}

int APP_PUMP_Check(void)
{
    uint8_t c = APP_PUMP_CHECK_ERROR;
    int changed = 0;

    if (g_appPump.status == APP_PUMP_STATUS_ON)
    {
        // 刚开启水泵时，有可能导致故障的状态；等多检测几次报确实有故障再报
        if (s_checkCount < _APP_PUMP_FREQUENCY_ARRAY_LEN)
        {
            s_checkCount++;
            g_appPump.statusOnChecking = 1;
            return APP_OK;
        }
        for (int i = 0; i < _APP_PUMP_FREQUENCY_ARRAY_LEN; i++)
        {
            if (g_appPump.frequency[i] >= 10)
            {
                c = APP_PUMP_CHECK_NORMAL;
            }
        }
    }
    else
    {
        c = APP_PUMP_CHECK_NORMAL;
        s_checkCount = 0;
    }
    g_appPump.statusOnChecking = 0;
    if ((g_appPump.lastStatus != g_appPump.status))
    {
        changed = 1;
    }
    if (c != g_appPump.check)
    {
        g_appPump.check = c;
        changed = 1;
        if (c == APP_PUMP_CHECK_ERROR)
        {
            IncrementErrorCount();
        }
    }

    g_appPump.check = c;
    if (g_appPump.changed == 0)
    {
        g_appPump.changed = changed;
    }
    return APP_OK;
}

int APP_PUMP_Changed(int *changed)
{
    *changed = 0;
    if (g_appPump.changed == 1)
    {
        if (g_appPump.status == APP_PUMP_STATUS_ON && g_appPump.statusOnChecking == 1)
        {
            return APP_ERROR;
        }
        *changed = 1;
        g_appPump.changed = 0;
        g_appPump.lastStatus = g_appPump.status;
        return APP_OK;
    }
    return APP_ERROR;
}

