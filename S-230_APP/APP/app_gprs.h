#ifndef __APP_GPRS_H__
#define __APP_GPRS_H__


#ifdef __cplusplus
extern "C"
{
#endif

#include "user_protocol_packet.h"
#include "bsp_protocol.h"

enum
{
    UNKONW_MANUFACTURER = 0,    // 未知运营商 
    CMNET,                      // 移动 
    UNINET,                     // 联通 
    CTNET,                      // 电信 
};

enum 
{
    IDLE_FLOW = 0,              // 不动作 
    GET_SIM_ICCID_FLOW,         // 读取 ICCID 
    GET_SIM_MANUFACTURER_FLOW,  // 读取运营商 
};

typedef struct 
{
    int8_t status;      // 状态 
    uint8_t errcnt;     // 错误次数 
    uint8_t rstcnt;     // 复位次数 
}app_gprs_status_t, *app_gprs_status_pt;

void APP_GPRS_task_run(void *argument);

int USER_PROTOCOL_write_role_message_process(user_protocol_packet_pt packet);
int USER_PROTOCOL_read_sim_iccid_message_process(user_protocol_packet_pt packet);
int USER_PROTOCOL_read_sim_manufacturer_message_process(user_protocol_packet_pt packet);
int USER_PROTOCOL_write_device_id_message_process(user_protocol_packet_pt packet);

int G2_SERVER_read_iccid_message_process(g2_server_packet_pt packet);
int APP_GPRS_read_iccid_message_process(void);

#ifdef __cplusplus
}
#endif

#endif
