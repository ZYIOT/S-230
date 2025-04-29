#include "app_environment.h"
#include "app_log.h"

static APP_ENVIRONMENT_t s_appEnvironment;

#ifdef SUPPORT_TEMPERATURE_AND_HUMIDITY
#include "bsp_aht20.h"
static void ReadAht20()
{
    float temperature = 0.0;
    float humidity = 0.0;
    int rc = BSP_AHT20_trigger();
    if (rc != APP_OK)
    {
        s_appEnvironment.tempAndHumiStatus = APP_ERROR;
    }
    HARDWARE_OS_DELAY_MS(100);
    // APP_LOG_Debug("AHT20 read\r\n");
    rc = BSP_AHT20_read(&temperature, &humidity);
    if (rc != APP_OK)
    {
        s_appEnvironment.tempAndHumiStatus = APP_ERROR;
    }
    else
    {
        s_appEnvironment.tempAndHumiStatus = APP_OK;
        s_appEnvironment.temperature = temperature;
        s_appEnvironment.humidity = humidity;
    }
    APP_LOG_Debug("AHT20: status: %d, temperature: %.2f humidity:%.2f\r\n", rc, temperature, humidity);
}
#endif

#if defined(SUPPORT_CHECK_POWER_VOL_IN) || defined(SUPPORT_CHECK_POWER_CUR_OUT)
#include "app_can.h"
#endif

#ifdef SUPPORT_CHECK_POWER_VOL_IN
static void ReadPowerIn(void)
{
    s_appEnvironment.phaseVoltageA = APP_CAN_get_phase_voltage_A;
    s_appEnvironment.phaseVoltageB = APP_CAN_get_phase_voltage_B;
    s_appEnvironment.phaseVoltageC = APP_CAN_get_phase_voltage_C;
}
#endif

#ifdef SUPPORT_CHECK_POWER_CUR_OUT
static void ReadPowerOut(void)
{
    s_appEnvironment.channelCurrent1 = update_can_relay_channel_current(0, 0);
    s_appEnvironment.channelCurrent2 = update_can_relay_channel_current(0, 1);
    s_appEnvironment.channelCurrent3 = update_can_relay_channel_current(0, 2);
    s_appEnvironment.channelCurrent4 = update_can_relay_channel_current(0, 3);
    s_appEnvironment.channelCurrent5 = update_can_relay_channel_current(0, 4);
}
#endif

int APP_ENVIRONMENT_Init(void)
{
#ifdef SUPPORT_TEMPERATURE_AND_HUMIDITY
    int rc = BSP_AHT20_init();
    s_appEnvironment.tempAndHumiStatus = APP_ERROR;
    s_appEnvironment.temperature = 0;
    s_appEnvironment.humidity = 0;
#endif
    return APP_OK; 
}

int APP_ENVIRONMENT_Read(void)
{
#ifdef SUPPORT_TEMPERATURE_AND_HUMIDITY
    ReadAht20();
#endif

#ifdef SUPPORT_CHECK_POWER_VOL_IN
    ReadPowerIn();
#endif

#ifdef SUPPORT_CHECK_POWER_CUR_OUT
    ReadPowerOut();
#endif
    return APP_OK;
}

int APP_ENVIRONMENT_Pack(g2_server_environment_message_pt message)
{
    message->config = 0;
#ifdef SUPPORT_TEMPERATURE_AND_HUMIDITY
    if (s_appEnvironment.tempAndHumiStatus == APP_OK)
    {
        message->config = 0xC000;
        message->temperature = s_appEnvironment.temperature * 10;
        message->humidity = s_appEnvironment.humidity * 10;
    }
#endif

#ifdef SUPPORT_CHECK_POWER_VOL_IN
    message->config |= 0x1C00;
    message->phaseVoltageA = s_appEnvironment.phaseVoltageA;
    message->phaseVoltageB = s_appEnvironment.phaseVoltageB;
    message->phaseVoltageC = s_appEnvironment.phaseVoltageC;
#endif

#ifdef SUPPORT_CHECK_POWER_CUR_OUT
    message->config |= 0x3E0;
    message->channelCurrent1 = s_appEnvironment.channelCurrent1;
    message->channelCurrent2 = s_appEnvironment.channelCurrent2;
    message->channelCurrent3 = s_appEnvironment.channelCurrent3;
    message->channelCurrent4 = s_appEnvironment.channelCurrent4;
    message->channelCurrent5 = s_appEnvironment.channelCurrent5;
#endif

    return APP_OK;
}
