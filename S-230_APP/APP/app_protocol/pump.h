#include "app_pump.h"


#ifdef SUPPORT_PUMP
int G2_SERVER_read_pump_message_process(g2_server_packet_pt packet)
{
    CHECK_PROTOCOL_MESSAGE
    g2_server_probe_id_message_pt pmsg = (g2_server_probe_id_message_pt)packet->parsed;
    CHECK_PROTOCOL_PROBE_ID_MESSAGE(pmsg->probe_id)

    g2_server_pump_message_t msg = {0};
    msg.probe_id = pmsg->probe_id;
    msg.channel = 0x6000;
    msg.status = 0;
    if (app_pump.status > 0)
    {
        msg.status = 0x6000;
    }
    BSP_PROTOCOL_send_read_pump_message(packet, &msg);
    return PROTOCOL_OK;
}

int G2_SERVER_write_pump_message_process(g2_server_packet_pt packet)
{
    CHECK_PROTOCOL_MESSAGE
    g2_server_pump_message_pt pmsg = (g2_server_pump_message_pt)packet->parsed;
    CHECK_PROTOCOL_PROBE_ID_MESSAGE(pmsg->probe_id)
    uint16_t mask = 0x6000;
    if ((pmsg->channel & mask) == mask)
    {
        uint8_t status = 0;
        if ((pmsg->status & mask) == mask)
        {
            status = 1;
        }
        int rc = APP_CONFIG_pump_status_write_status(pmsg->probe_id - 1, &app_config_pump_status[pmsg->probe_id - 1], 0, status);
        CHECK_CONFIG_MESSAGE_RC(rc)
        // APP_PUMP_refresh(app_config_pump_status[pmsg->probe_id - 1].status[0]);
    }
    BSP_PROTOCOL_send_ok_response(packet);
    return PROTOCOL_OK;
}
#else
int G2_SERVER_read_pump_message_process(g2_server_packet_pt packet)
{
    BSP_PROTOCOL_send_error_response(packet);
    return PROTOCOL_OK;
}

int G2_SERVER_write_pump_message_process(g2_server_packet_pt packet)
{
    BSP_PROTOCOL_send_error_response(packet);
    return PROTOCOL_OK;
}
#endif


