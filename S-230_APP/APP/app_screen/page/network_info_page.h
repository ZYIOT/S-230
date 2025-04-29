#include "app_network.h"

char *get_network_status()
{
    if (APP_NETWORK_CONNECTED())
    {
        switch (g_appNetwork.connectedStatus)
        {
        case CONNECTED_STATUS_CONNECTED:
            snprintf(app_screen_msg, APP_SCREEN_MSG_BUFFER_SIZE - 1, "已连接");
            break;
        // case CONNECTED_STATUS_CONNECTING:
        //     snprintf(app_screen_msg, APP_SCREEN_MSG_BUFFER_SIZE - 1, "正在连接服务");
        //     break;
        case CONNECTED_STATUS_DISCONNECTED:
            snprintf(app_screen_msg, APP_SCREEN_MSG_BUFFER_SIZE - 1, "未连接服务");
            break;
        default:
            snprintf(app_screen_msg, APP_SCREEN_MSG_BUFFER_SIZE - 1, "未知错误");
            break;
        }
    }
    else
    {
        snprintf(app_screen_msg, APP_SCREEN_MSG_BUFFER_SIZE - 1, "网络未连接");
    }

    return app_screen_msg;
}

char *get_network_csq()
{
    if (g_appNetwork.csq > 31)
    {
        snprintf(app_screen_msg, APP_SCREEN_MSG_BUFFER_SIZE - 1, "网络信号差");
    }
    else
    {
        snprintf(app_screen_msg, APP_SCREEN_MSG_BUFFER_SIZE - 1, "%d", g_appNetwork.csq);
    }
    return app_screen_msg;
}

app_screen_message_node_t network_info_messages[] = {
    {.name = "网络状态:", .handler = NULL},
    {.name = "  ", .handler = &get_network_status},
    {.name = "信号强度:", .handler = NULL},
    {.name = "  ", .handler = &get_network_csq},
    APP_SCREEN_MESSAGE_NODE_END};

NEW_MESSAGE_PAGE(network_info_page, network_info_messages, NULL, NULL, NULL)
