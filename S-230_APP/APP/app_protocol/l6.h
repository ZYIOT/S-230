#ifdef SUPPORT_L6
#include "app_l6.h"

int G2_SERVER_write_l6_start_message_process(g2_server_packet_pt packet)
{
    CHECK_PROTOCOL_MESSAGE
    g2_server_probe_id_message_pt pmsg = (g2_server_probe_id_message_pt)packet->parsed;
    CHECK_PROTOCOL_PROBE_ID_MESSAGE(pmsg->probe_id)
#ifdef SUPPORT_L6_ENABLE
	int rc =APP_L6_init();
#else
    int rc = APP_L6_start();	
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
    CHECK_PROTOCOL_PROBE_ID_MESSAGE(pmsg->probe_id)
    g2_server_l6_status_message_t message;
    G2_SERVER_l6_status_message_init(&message);
    message.probe_id = pmsg->probe_id;
    APP_L6_status_pack(&message);
    BSP_PROTOCOL_send_read_l6_status_message(packet, &message);
    return PROTOCOL_OK;
}

int G2_SERVER_write_l6_settings_message_process(g2_server_packet_pt packet)
{
    CHECK_PROTOCOL_MESSAGE
    g2_server_l6_settings_message_pt pmsg = (g2_server_l6_settings_message_pt)packet->parsed;
    CHECK_PROTOCOL_PROBE_ID_MESSAGE(pmsg->probe_id)
#ifdef SUPPORT_L6_ENABLE
	int rc = APP_L6_settings_write(pmsg->probe_id,pmsg);
#else
    int rc = APP_L6_set_register(pmsg->release, pmsg->read_wait, pmsg->read, pmsg->charge, pmsg->charge_wait * 60);
    APP_L6_read();
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
    CHECK_PROTOCOL_PROBE_ID_MESSAGE(pmsg->probe_id)

    g2_server_l6_settings_message_t message;
    G2_SERVER_l6_settings_message_init(&message);
    message.probe_id = pmsg->probe_id;
#ifdef SUPPORT_L6_ENABLE
    message.config = app_config_l6.has_config << 8;
    message.release = app_config_l6.release;
    message.read_wait = app_config_l6.read_wait;
    message.read = app_config_l6.read;
    message.charge = app_config_l6.charge;
    message.charge_wait = app_config_l6.charge_wait;
#else
    if (rs485_l6_status.status == RS485_OK)
    {
        message.config = 0xf800;
        message.release = rs485_l6_status.release;
        message.read_wait = rs485_l6_status.read_wait;
        message.read = rs485_l6_status.read;
        message.charge = rs485_l6_status.charge;
        message.charge_wait = rs485_l6_status.charge_wait / 60;
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
    APP_ENVIRONMENT_pack(&message);
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
