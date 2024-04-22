#ifndef __APP_ENVIRONMENT_H
#define __APP_ENVIRONMENT_H

#ifdef __cplusplus
extern "C"
{
#endif
#include "app.h"
#include "bsp_protocol.h"
    typedef struct
    {
    int a;
#ifdef SUPPORT_TEMPERATURE_AND_HUMIDITY
        float temperature;
        float humidity;
        int temperature_and_humidity_status;
#endif
#ifdef SUPPORT_CHECK_POWER_VOL_IN
        uint16_t phase_voltage_A;
        uint16_t phase_voltage_B;
        uint16_t phase_voltage_C;
#endif
#ifdef SUPPORT_CHECK_POWER_CUR_OUT
        uint16_t channel_current_1;
        uint16_t channel_current_2;
        uint16_t channel_current_3;
        uint16_t channel_current_4;
        uint16_t channel_current_5;
#endif
    } app_environment_t, *app_environment_pt;

    int APP_ENVIRONMENT_init(void);
    int APP_ENVIRONMENT_read(void);
    int APP_ENVIRONMENT_pack(g2_server_environment_message_pt message);
#ifdef __cplusplus
}
#endif

#endif
