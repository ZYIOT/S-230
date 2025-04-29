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
    uint8_t networkStatus;
    uint8_t csq;
    uint8_t connectedStatus;
    uint8_t wasConnectedStatus; //曾经连接到服务器
    uint32_t connectTick;
    uint32_t lastSendTick;
    uint32_t lastSendDataTick;
    uint32_t onceConnectedFlag;
} APP_NETWORK_t, *APP_NETWORK_pt;

extern APP_NETWORK_t g_appNetwork;

#define APP_NETWORK_CONNECTED() (g_appNetwork.networkStatus == CONNECTED_STATUS_CONNECTED)
#define APP_NETWORK_DISCONNECTED() (g_appNetwork.networkStatus == CONNECTED_STATUS_DISCONNECTED)
#define APP_SERVER_CONNECTED() (g_appNetwork.connectedStatus == CONNECTED_STATUS_CONNECTED)
#define APP_SERVER_DISCONNECTED() (g_appNetwork.connectedStatus == CONNECTED_STATUS_DISCONNECTED)

#define SET_APP_SERVER_ONCE_CONNECTED(state)    (g_appNetwork.onceConnectedFlag = state)
#define GET_APP_SERVER_ONCE_CONNECTED()         (g_appNetwork.onceConnectedFlag)

void APP_NETWORK_UpdateCSQ(uint8_t csq);
void APP_NETWORK_UpdateEnvironmentToServer(g2_server_packet_pt packet);
int APP_NETWORK_Init(void);
// void APP_NETWORK_on_disconnect(void);
// void APP_NETWORK_on_connect(void);
void APP_NETWORK_OnConnectToServer(void);
void APP_NETWORK_CheckStataus(void);
void APP_NETWORK_task_run(void *argument);


#ifdef __cplusplus
}
#endif

#endif
