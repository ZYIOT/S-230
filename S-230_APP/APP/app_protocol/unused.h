int G2_SERVER_write_system_message_process(g2_server_packet_pt packet)
{
    BSP_PROTOCOL_send_error_response(packet);
    return PROTOCOL_OK;
}

int G2_SERVER_read_system_message_process(g2_server_packet_pt packet)
{
    g2_server_system_message_t message = {0};
    message.config = 0xfe00;
    message.display = 1;
    message.display_refresh = 1;
    message.push_data_interval = 1;
    message.push_signal_interval = 1;
    message.sensor_wireless_interval = 1;
    message.relay_wireless_interval = 1;
    message.relay_release_time = 1;
    BSP_PROTOCOL_send_read_system_message(packet, &message);
    return PROTOCOL_OK;
}

int G2_SERVER_read_device_capability_message_process(g2_server_packet_pt packet)
{
    g2_server_device_capability_message_t message = {0};
    G2_SERVER_device_capability_message_init(&message);

    message.config = 0xff00;
    message.sensor_size = PROBE_SIZE;
    message.probe_size_per_sensor = PROBE_SENSOR_SIZE;
    message.gradient_size_per_probe = APP_CONFIG_MAX_LIMIT_TASK;
    message.calibrate_jobs_per_sensor = 0;
    message.relay_size = RELAY_SIZE;
    message.channel_size_per_relay = RELAY_CHANNEL_SIZE;
    message.job_size_per_channel = APP_CONFIG_MAX_TIMES_TASK;
    message.time_size_per_job = 4;
    BSP_PROTOCOL_send_read_device_capability_message(packet, &message);
    return PROTOCOL_OK;
}

int G2_SERVER_write_device_capability_message_process(g2_server_packet_pt packet)
{
    BSP_PROTOCOL_send_error_response(packet);
    return PROTOCOL_OK;
}

int G2_SERVER_read_device_used_capability_message_process(g2_server_packet_pt packet)
{
    g2_server_device_used_capability_message_t message = {0};
    G2_SERVER_device_used_capability_message_init(&message);
    message.sensor_count = PROBE_SIZE;
    message.relay_count = RELAY_SIZE;
    BSP_PROTOCOL_send_read_device_used_capability_message(packet, &message);
    return PROTOCOL_OK;
}

int G2_SERVER_write_device_used_capability_message_process(g2_server_packet_pt packet)
{
    BSP_PROTOCOL_send_error_response(packet);
    return PROTOCOL_OK;
}

int G2_SERVER_read_device_enable_message_process(g2_server_packet_pt packet)
{
    g2_server_device_enable_message_t message = {0};
    message.type = 0x11;
    message.node_id = 1;
    message.enable = 1;
    BSP_PROTOCOL_send_read_device_enable_message(packet, &message);
    return PROTOCOL_OK;
}

int G2_SERVER_write_device_enable_message_process(g2_server_packet_pt packet)
{
    BSP_PROTOCOL_send_error_response(packet);
    return PROTOCOL_OK;
}

int G2_SERVER_read_relay_config_message_process(g2_server_packet_pt packet)
{
    CHECK_PROTOCOL_MESSAGE
    g2_server_relay_id_message_pt pmsg = (g2_server_relay_id_message_pt)packet->parsed;
    CHECK_PROTOCOL_RELAY_ID_MESSAGE(pmsg->relayID)

    g2_server_relay_config_message_t message = {0};
    G2_SERVER_relay_config_message_init(&message);
    message.relayID = pmsg->relayID;
    message.channel_count = RELAY_CHANNEL_SIZE;
    BSP_PROTOCOL_send_read_relay_config_message(packet, &message);
    return PROTOCOL_OK;
}

int G2_SERVER_write_relay_config_message_process(g2_server_packet_pt packet)
{
    BSP_PROTOCOL_send_error_response(packet);
    return PROTOCOL_OK;
}

int G2_SERVER_delete_relay_config_message_process(g2_server_packet_pt packet)
{
    BSP_PROTOCOL_send_error_response(packet);
    return PROTOCOL_OK;
}

int G2_SERVER_read_sensor_limit_enable_message_process(g2_server_packet_pt packet)
{
    BSP_PROTOCOL_send_error_response(packet);
    return PROTOCOL_OK;
}

int G2_SERVER_write_sensor_limit_enable_message_process(g2_server_packet_pt packet)
{
    BSP_PROTOCOL_send_error_response(packet);
    return PROTOCOL_OK;
}
