#include "app.h"
#include "app_log.h"
#include "app_pump.h"
#include "hardware_wrapper.h"
#include "utils.h"
#include "app_sensors.h"

app_pump_t app_pump;
static uint8_t check_count = 0;
void _increment_error_count(void);

int APP_PUMP_init(void)
{
    BSP_PUMP_init();
    app_pump.status = APP_PUMP_STATUS_OFF;
    app_pump.last_status = APP_PUMP_STATUS_OFF;
    app_pump.changed = 0;
    app_pump.status_on_checking = 0;
    app_pump.check = APP_PUMP_CHECK_NORMAL;
    app_pump.error_count = 0;
    for (int i = 0; i < _APP_PUMP_FREQUENCY_ARRAY_LEN; i++)
    {
        app_pump.frequency[i] = 0;
    }
    return APP_OK;
}

int APP_PUMP_on(void)
{
    if (app_pump.status != APP_PUMP_STATUS_ON)
    {
        check_count = 0;
        app_pump.status = APP_PUMP_STATUS_ON;
    }
    BSP_PUMP_on();
    return APP_OK;
}

int APP_PUMP_off(void)
{
    if (app_pump.status != APP_PUMP_STATUS_OFF)
    {
        check_count = 0;
        app_pump.status = APP_PUMP_STATUS_OFF;
    }

    BSP_PUMP_off();
    return APP_OK;
}

#include "app_config/defines.h"
volatile uint32_t frequency = 0;
volatile uint8_t pump_status = 0;

int APP_PUMP_refresh(uint8_t refresh_status)
{
    if (refresh_status > 0)
    {
        app_pump.status = APP_PUMP_STATUS_ON;
        APP_PUMP_on();
    }
    else
    {
        app_pump.status = APP_PUMP_STATUS_OFF;
        app_pump.check = APP_PUMP_CHECK_NORMAL;
        APP_PUMP_off();
    }
    return APP_OK;
}

void APP_PUMP_task_run(void *argument)
{
    uint8_t last_check_status = 0;
    uint8_t check_status = 0;
    app_config_pump_status_pt cmsg = &app_config_pump_status[0];
    for (;;)
    {
        APP_PUMP_refresh(cmsg->status[0]);
        check_status = BSP_PUMP_read_status();
        if (last_check_status != check_status)
        {
            last_check_status = check_status;
            frequency++;
        }
    }
}

void APP_PUMP_set_check_frequency(uint16_t fq)
{
    static int i = 0;
    APP_LOG_debug("\r\npump frequency: %u\r\n", frequency);
    app_pump.frequency[i] = frequency;
    i++;
    frequency = 0;
    i %= _APP_PUMP_FREQUENCY_ARRAY_LEN;
    APP_PUMP_check();
}

int APP_PUMP_check(void)
{
    uint8_t c = APP_PUMP_CHECK_ERROR;
    int changed = 0;

    if (app_pump.status == APP_PUMP_STATUS_ON)
    {
        // 刚开启水泵时，有可能导致故障的状态；等多检测几次报确实有故障再报
        if (check_count < _APP_PUMP_FREQUENCY_ARRAY_LEN)
        {
            check_count++;
            app_pump.status_on_checking = 1;
            return APP_OK;
        }
        for (int i = 0; i < _APP_PUMP_FREQUENCY_ARRAY_LEN; i++)
        {
            if (app_pump.frequency[i] >= 10)
            {
                c = APP_PUMP_CHECK_NORMAL;
            }
        }
    }
    else
    {
        c = APP_PUMP_CHECK_NORMAL;
        check_count = 0;
    }
    app_pump.status_on_checking = 0;
    if ((app_pump.last_status != app_pump.status))
    {
        changed = 1;
    }
    if (c != app_pump.check)
    {
        app_pump.check = c;
        changed = 1;
        if (c == APP_PUMP_CHECK_ERROR)
        {
            _increment_error_count();
        }
    }

    app_pump.check = c;
    if (app_pump.changed == 0)
    {
        app_pump.changed = changed;
    }
    return APP_OK;
}

int APP_PUMP_changed(int *changed)
{
    *changed = 0;
    if (app_pump.changed == 1)
    {
        if (app_pump.status == APP_PUMP_STATUS_ON && app_pump.status_on_checking == 1)
        {
            return APP_ERROR;
        }
        *changed = 1;
        app_pump.changed = 0;
        app_pump.last_status = app_pump.status;
        return APP_OK;
    }
    return APP_ERROR;
}

static void _increment_error_count(void)
{
    app_pump.error_count++;
}
