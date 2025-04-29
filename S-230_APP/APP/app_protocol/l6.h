#ifdef SUPPORT_L6
#include "app_l6.h"

int G2_SERVER_write_l6_start_message_process(g2_server_packet_pt packet)
{
    CHECK_PROTOCOL_MESSAGE
    g2_server_probe_id_message_pt pmsg = (g2_server_probe_id_message_pt)packet->parsed;
    CHECK_PROTOCOL_PROBE_ID_MESSAGE(pmsg->probeID)
#ifdef SUPPORT_L6_ENABLE
	int rc =APP_L6_Init();
#else
    int rc = APP_L6_Start();	
#endif
    if (rc != APP_OK)
    {
        BSP_PROTOCOL_send_response(packet, 0xe2);
        return PROTOCOL_OK;
    }
    BSP_PROTOCOL_send_write_l6_start_message(packet);
    return PROTOCOL_OK;
}

int G2_SERVER_read_l6_status_message_process(g2_server_packet_pt packet)
{
    CHECK_PROTOCOL_MESSAGE
    g2_server_probe_id_message_pt pmsg = (g2_server_probe_id_message_pt)packet->parsed;
    CHECK_PROTOCOL_PROBE_ID_MESSAGE(pmsg->probeID)
    g2_server_l6_status_message_t message;
    G2_SERVER_l6_status_message_init(&message);
    message.probeID = pmsg->probeID;
    APP_L6_StatusPack(&message);
    BSP_PROTOCOL_send_read_l6_status_message(packet, &message);
    return PROTOCOL_OK;
}

int G2_SERVER_write_l6_settings_message_process(g2_server_packet_pt packet)
{
    CHECK_PROTOCOL_MESSAGE
    g2_server_l6_settings_message_pt pmsg = (g2_server_l6_settings_message_pt)packet->parsed;
    CHECK_PROTOCOL_PROBE_ID_MESSAGE(pmsg->probeID)
#ifdef SUPPORT_L6_ENABLE
	int rc = APP_L6_SettingsWrite(pmsg->probeID,pmsg);
#else
    int rc = APP_L6_SetRegister(pmsg->release, pmsg->readWait, pmsg->read, pmsg->charge, pmsg->chargeWait * 60);
    APP_L6_Read();
#endif
    if (rc != APP_OK)
    {
        BSP_PROTOCOL_send_response(packet, 0xd7);
        return PROTOCOL_OK;
    }
    BSP_PROTOCOL_send_write_l6_settings_message(packet);
    return PROTOCOL_OK;
}

int G2_SERVER_read_l6_settings_message_process(g2_server_packet_pt packet)
{
    CHECK_PROTOCOL_MESSAGE
    g2_server_probe_id_message_pt pmsg = (g2_server_probe_id_message_pt)packet->parsed;
    CHECK_PROTOCOL_PROBE_ID_MESSAGE(pmsg->probeID)

    g2_server_l6_settings_message_t message;
    G2_SERVER_l6_settings_message_init(&message);
    message.probeID = pmsg->probeID;
#ifdef SUPPORT_L6_ENABLE
    message.config = g_appConfigL6.hasConfig << 8;
    message.release = g_appConfigL6.release;
    message.readWait = g_appConfigL6.readWait;
    message.read = g_appConfigL6.read;
    message.charge = g_appConfigL6.charge;
    message.chargeWait = g_appConfigL6.chargeWait;
#else
    if (rs485_l6_status.status == RS485_OK)
    {
        message.config = 0xf800;
        message.release = rs485_l6_status.release;
        message.readWait = rs485_l6_status.readWait;
        message.read = rs485_l6_status.read;
        message.charge = rs485_l6_status.charge;
        message.chargeWait = rs485_l6_status.chargeWait / 60;
    }
#endif
    BSP_PROTOCOL_send_read_l6_settings_message(packet, &message);
    return PROTOCOL_OK;
}

#else
int G2_SERVER_write_l6_start_message_process(g2_server_packet_pt packet)
{
    BSP_PROTOCOL_send_error_response(packet);
    return PROTOCOL_OK;
}

int G2_SERVER_read_l6_status_message_process(g2_server_packet_pt packet)
{
    BSP_PROTOCOL_send_error_response(packet);
    return PROTOCOL_OK;
}

int G2_SERVER_write_l6_settings_message_process(g2_server_packet_pt packet)
{
    BSP_PROTOCOL_send_error_response(packet);
    return PROTOCOL_OK;
}
int G2_SERVER_read_l6_settings_message_process(g2_server_packet_pt packet)
{
    BSP_PROTOCOL_send_error_response(packet);
    return PROTOCOL_OK;
}
#endif

#if defined(SUPPORT_TEMPERATURE_AND_HUMIDITY) || defined(SUPPORT_CHECK_POWER_VOL_IN) || defined(SUPPORT_CHECK_POWER_CUR_OUT)
#include "app_environment.h"
int G2_SERVER_read_environment_message_process(g2_server_packet_pt packet)
{
    g2_server_environment_message_t message;
    G2_SERVER_environment_message_init(&message);
    APP_ENVIRONMENT_Pack(&message);
    BSP_PROTOCOL_send_read_environment_message(packet, &message);
    return PROTOCOL_OK;
}
#else
int G2_SERVER_read_environment_message_process(g2_server_packet_pt packet)
{
    BSP_PROTOCOL_send_error_response(packet);
    return PROTOCOL_OK;
}
#endif
