#include "app_network.h"

int G2_SERVER_read_connect_message_process(g2_server_packet_pt packet)
{
    APP_NETWORK_on_connect_to_server();
    return PROTOCOL_OK;
}
