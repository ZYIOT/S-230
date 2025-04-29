#include "app_network.h"

char *get_network_status()
{
    if (APP_NETWORK_CONNECTED())
    {
        switch (g_appNetwork.connectedStatus)
        {
        case CONNECTED_STATUS_CONNECTED:
            snprintf(app_screen_msg, APP_SCREEN_MSG_BUFFER_SIZE - 1, "������");
            break;
        // case CONNECTED_STATUS_CONNECTING:
        //     snprintf(app_screen_msg, APP_SCREEN_MSG_BUFFER_SIZE - 1, "�������ӷ���");
        //     break;
        case CONNECTED_STATUS_DISCONNECTED:
            snprintf(app_screen_msg, APP_SCREEN_MSG_BUFFER_SIZE - 1, "δ���ӷ���");
            break;
        default:
            snprintf(app_screen_msg, APP_SCREEN_MSG_BUFFER_SIZE - 1, "δ֪����");
            break;
        }
    }
    else
    {
        snprintf(app_screen_msg, APP_SCREEN_MSG_BUFFER_SIZE - 1, "����δ����");
    }

    return app_screen_msg;
}

char *get_network_csq()
{
    if (g_appNetwork.csq > 31)
    {
        snprintf(app_screen_msg, APP_SCREEN_MSG_BUFFER_SIZE - 1, "�����źŲ�");
    }
    else
    {
        snprintf(app_screen_msg, APP_SCREEN_MSG_BUFFER_SIZE - 1, "%d", g_appNetwork.csq);
    }
    return app_screen_msg;
}

app_screen_message_node_t network_info_messages[] = {
    {.name = "����״̬:", .handler = NULL},
    {.name = "  ", .handler = &get_network_status},
    {.name = "�ź�ǿ��:", .handler = NULL},
    {.name = "  ", .handler = &get_network_csq},
    APP_SCREEN_MESSAGE_NODE_END};

NEW_MESSAGE_PAGE(network_info_page, network_info_messages, NULL, NULL, NULL)
