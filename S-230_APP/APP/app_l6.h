#ifndef __APP_L6_H
#define __APP_L6_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "rs485_l6.h"
#include "bsp_rs485.h"
#include "bsp_protocol.h"

extern rs485_l6_status_t rs485_l6_status;

#define APP_L6_Initialized()        (rs485_l6_status.initialized == 1)
#define APP_L6_DataReady()          (rs485_l6_status.data_status == 0)
#define APP_L6_SendDataReady()      (rs485_l6_status.send_status == 1) 

#define APP_L6_ForceSaturabilityReady()     (rs485_l6_status.saturability_status == 1) 

#define APP_L6_IsBroken()   (rs485_l6_status.reset_count >= 3 && rs485_l6_status.errorCount >= 10)
#define APP_L6_ForceRead()  (APP_L6_IsBroken() || rs485_l6_status.force_status == 1)
#define APP_L6_CanReset()   (rs485_l6_status.reset_count < 3 && rs485_l6_status.errorCount >= 10)

#define APP_L6_Read()       RS485_L6_read(&rs485_port, &rs485_l6_status) 
#define APP_L6_Start()      RS485_L6_start(&rs485_port) 
#define APP_L6_SetRegister(release, readWait, read, charge, chargeWait) RS485_L6_set_register(&rs485_port, release, readWait, read, charge, chargeWait) 

#define APP_L6_ReleaseTime()        (g_appConfigL6.release)
#define APP_L6_ReadWaitTime()       (g_appConfigL6.readWait + APP_L6_ReleaseTime())
#define APP_L6_ReadTime()           (g_appConfigL6.read + APP_L6_ReadWaitTime())
#define APP_L6_ChargeTime()         (g_appConfigL6.charge + APP_L6_ReadTime())	
#define APP_L6_ChargeWaitTime()     (g_appConfigL6.chargeWait * 60 + APP_L6_ChargeTime())

int APP_L6_Init(void);
int APP_L6_StatusPack(g2_server_l6_status_message_pt message);
int APP_L6_SettingsWrite(uint8_t probeID,g2_server_l6_settings_message_pt message);
void APP_L6_task_run(void *argument);


#ifdef __cplusplus
}
#endif

#endif
