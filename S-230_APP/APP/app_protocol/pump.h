#include "app_pump.h"


#ifdef SUPPORT_PUMP
int G2_SERVER_read_pump_message_process(g2_server_packet_pt packet)
{
    CHECK_PROTOCOL_MESSAGE
    g2_server_probe_id_message_pt pmsg = (g2_server_probe_id_message_pt)packet->parsed;
    CHECK_PROTOCOL_PROBE_ID_MESSAGE(pmsg->probeID)

    g2_server_pump_message_t msg = {0};
    msg.probeID = pmsg->probeID;
#ifndef ENABLE_INPUT_RAND_MAX_MIN    
    msg.channel = 0x6000;
    msg.status = 0;
    if (g_appPump.status > 0)
    {
        msg.status = 0x6000;
    }
#else
    msg.channel = g_appRandOpt.max;
    msg.status = g_appRandOpt.min;
    APP_LOG_Debug("Read Rand Value, Max:%d, Min:%d\r\n", g_appRandOpt.max, g_appRandOpt.min);
#endif    
    BSP_PROTOCOL_send_read_pump_message(packet, &msg);
    return PROTOCOL_OK;
}

int G2_SERVER_write_pump_message_process(g2_server_packet_pt packet)
{
    CHECK_PROTOCOL_MESSAGE
    g2_server_pump_message_pt pmsg = (g2_server_pump_message_pt)packet->parsed;
    CHECK_PROTOCOL_PROBE_ID_MESSAGE(pmsg->probeID)
#ifndef ENABLE_INPUT_RAND_MAX_MIN 
    uint16_t mask = 0x6000;
    if ((pmsg->channel & mask) == mask)
    {
        uint8_t status = 0;
        if ((pmsg->status & mask) == mask)
        {
            status = 1;
        }
        int rc = APP_CONFIG_PumpStatusWriteStatus(pmsg->probeID - 1, &g_appConfigPumpStatus[pmsg->probeID - 1], 0, status);
        CHECK_CONFIG_MESSAGE_RC(rc)
        // APP_PUMP_Refresh(g_appConfigPumpStatus[pmsg->probeID - 1].status[0]);
    }
#else
    g_appRandOpt.max = pmsg->channel;
    g_appRandOpt.min = pmsg->status;
    APP_CONFIG_RandValueWrite(&g_appRandOpt);
    APP_LOG_Debug("Update Rand Value, Max:%d, Min:%d\r\n", g_appRandOpt.max, g_appRandOpt.min);
#endif    
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


