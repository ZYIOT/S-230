#include "app_environment.h"
#include "app_log.h"

static app_environment_t app_environment;

#ifdef SUPPORT_TEMPERATURE_AND_HUMIDITY
#include "bsp_aht20.h"
static void read_aht20()
{
    float temperature = 0.0;
    float humidity = 0.0;
    int rc = BSP_AHT20_trigger();
    if (rc != APP_OK)
    {
        app_environment.temperature_and_humidity_status = APP_ERROR;
    }
    HARDWARE_OS_DELAY_MS(100);
    // APP_LOG_debug("AHT20 read\r\n");
    rc = BSP_AHT20_read(&temperature, &humidity);
    if (rc != APP_OK)
    {
        app_environment.temperature_and_humidity_status = APP_ERROR;
    }
    else
    {
        app_environment.temperature_and_humidity_status = APP_OK;
        app_environment.temperature = temperature;
        app_environment.humidity = humidity;
    }
    APP_LOG_debug("AHT20: status: %d, temperature: %.2f humidity:%.2f\r\n", rc, temperature, humidity);
}
#endif

#if defined(SUPPORT_CHECK_POWER_VOL_IN) || defined(SUPPORT_CHECK_POWER_CUR_OUT)
#include "app_can.h"
#endif

#ifdef SUPPORT_CHECK_POWER_VOL_IN
static void read_POWER_IN(void)
{
    app_environment.phase_voltage_A = APP_CAN_get_phase_voltage_A;
    app_environment.phase_voltage_B = APP_CAN_get_phase_voltage_B;
    app_environment.phase_voltage_C = APP_CAN_get_phase_voltage_C;
}
#endif

#ifdef SUPPORT_CHECK_POWER_CUR_OUT
static void read_POWER_OUT(void)
{
    app_environment.channel_current_1 = update_can_relay_channel_current(0, 0);
    app_environment.channel_current_2 = update_can_relay_channel_current(0, 1);
    app_environment.channel_current_3 = update_can_relay_channel_current(0, 2);
    app_environment.channel_current_4 = update_can_relay_channel_current(0, 3);
    app_environment.channel_current_5 = update_can_relay_channel_current(0, 4);
}
#endif

int APP_ENVIRONMENT_init(void)
{
#ifdef SUPPORT_TEMPERATURE_AND_HUMIDITY
    int rc = BSP_AHT20_init();
    app_environment.temperature_and_humidity_status = APP_ERROR;
    app_environment.temperature = 0;
    app_environment.humidity = 0;
#endif
    return APP_OK; 
}

int APP_ENVIRONMENT_read(void)
{
#ifdef SUPPORT_TEMPERATURE_AND_HUMIDITY
    read_aht20();
#endif

#ifdef SUPPORT_CHECK_POWER_VOL_IN
    read_POWER_IN();
#endif

#ifdef SUPPORT_CHECK_POWER_CUR_OUT
    read_POWER_OUT();
#endif
    return APP_OK;
}

int APP_ENVIRONMENT_pack(g2_server_environment_message_pt message)
{
    message->config = 0;
#ifdef SUPPORT_TEMPERATURE_AND_HUMIDITY
    if (app_environment.temperature_and_humidity_status == APP_OK)
    {
        message->config = 0xC000;
        message->temperature = app_environment.temperature * 10;
        message->humidity = app_environment.humidity * 10;
    }
#endif

#ifdef SUPPORT_CHECK_POWER_VOL_IN
    message->config |= 0x1C00;
    message->phase_voltage_A = app_environment.phase_voltage_A;
    message->phase_voltage_B = app_environment.phase_voltage_B;
    message->phase_voltage_C = app_environment.phase_voltage_C;
#endif

#ifdef SUPPORT_CHECK_POWER_CUR_OUT
    message->config |= 0x3E0;
    message->channel_current_1 = app_environment.channel_current_1;
    message->channel_current_2 = app_environment.channel_current_2;
    message->channel_current_3 = app_environment.channel_current_3;
    message->channel_current_4 = app_environment.channel_current_4;
    message->channel_current_5 = app_environment.channel_current_5;
#endif

    return APP_OK;
}
