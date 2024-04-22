#ifndef __APP_NETWORK_H
#define __APP_NETWORK_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "app_protocol.h"
#define CONNECTED_STATUS_DISCONNECTED 0
#define CONNECTED_STATUS_CONNECTED 1

#define CSQ_MIN_VALUE   (0) 
#define CSQ_MAX_VALUE   (32) 

typedef struct
{
    uint8_t network_status;
    uint8_t csq;
    uint8_t connected_status;
    uint8_t was_connected_status; //曾经连接到服务器
    uint32_t connect_tick;
    uint32_t last_send_tick;
    uint32_t last_send_data_tick;
    uint32_t once_connected_flag;
} app_network_t, *app_network_pt;

extern app_network_t app_network;

#define APP_NETWORK_CONNECTED() (app_network.network_status == CONNECTED_STATUS_CONNECTED)
#define APP_NETWORK_DISCONNECTED() (app_network.network_status == CONNECTED_STATUS_DISCONNECTED)
#define APP_SERVER_CONNECTED() (app_network.connected_status == CONNECTED_STATUS_CONNECTED)
#define APP_SERVER_DISCONNECTED() (app_network.connected_status == CONNECTED_STATUS_DISCONNECTED)

#define SET_APP_SERVER_ONCE_CONNECTED(state)    (app_network.once_connected_flag = state)
#define GET_APP_SERVER_ONCE_CONNECTED()         (app_network.once_connected_flag)

void APP_NETWORK_update_csq(uint8_t csq);
void APP_NETWORK_update_environment_to_server(g2_server_packet_pt packet);
int APP_NETWORK_init(void);
// void APP_NETWORK_on_disconnect(void);
// void APP_NETWORK_on_connect(void);
void APP_NETWORK_on_connect_to_server(void);
void APP_NETWORK_check_stataus(void);
void APP_NETWORK_task_run(void *argument);


#ifdef __cplusplus
}
#endif

#endif
