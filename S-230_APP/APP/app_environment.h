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
        int tempAndHumiStatus;
#endif
#ifdef SUPPORT_CHECK_POWER_VOL_IN
        uint16_t phaseVoltageA;
        uint16_t phaseVoltageB;
        uint16_t phaseVoltageC;
#endif
#ifdef SUPPORT_CHECK_POWER_CUR_OUT
        uint16_t channelCurrent1;
        uint16_t channelCurrent2;
        uint16_t channelCurrent3;
        uint16_t channelCurrent4;
        uint16_t channelCurrent5;
#endif
    } APP_ENVIRONMENT_t, *APP_ENVIRONMENT_pt;

    int APP_ENVIRONMENT_Init(void);
    int APP_ENVIRONMENT_Read(void);
    int APP_ENVIRONMENT_Pack(g2_server_environment_message_pt message);
#ifdef __cplusplus
}
#endif

#endif
