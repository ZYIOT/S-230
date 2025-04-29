#include "g2_server_slave.h"

static int _on_receive_write_device_id_message(g2_server_packet_pt packet)
{
    g2_server_device_id_message_t message;
    int rc = G2_SERVER_device_id_message_deserialize(packet->buffer, &message);
    G2_SERVER_CHECK_READ_RC_AND_RETURN
    packet->parsed = &message;
    return G2_SERVER_write_device_id_message_process(packet);
}

static int _on_receive_write_version_message(g2_server_packet_pt packet)
{
    g2_server_device_version_message_t message;
    int rc = G2_SERVER_device_version_message_deserialize(packet->buffer, &message);
    G2_SERVER_CHECK_READ_RC_AND_RETURN
    packet->parsed = &message;
    return G2_SERVER_write_version_message_process(packet);
}

static int _on_receive_read_sensor_config_message(g2_server_packet_pt packet)
{
    g2_server_probe_id_message_t message;
    int rc = G2_SERVER_probe_id_message_deserialize(packet->buffer, &message);
    G2_SERVER_CHECK_READ_RC_AND_RETURN
    packet->parsed = &message;
    return G2_SERVER_read_sensor_config_message_process(packet);
}

static int _on_receive_write_sensor_config_message(g2_server_packet_pt packet)
{
    g2_server_write_sensor_config_message_t message;
    int rc = G2_SERVER_write_sensor_config_message_deserialize(packet->buffer, &message);
    G2_SERVER_CHECK_READ_RC_AND_RETURN
    packet->parsed = &message;
    return G2_SERVER_write_sensor_config_message_process(packet);
}

static int _on_receive_delete_sensor_config_message(g2_server_packet_pt packet)
{
    g2_server_delete_sensor_config_message_t message;
    int rc = G2_SERVER_delete_sensor_config_message_deserialize(packet->buffer, &message);
    G2_SERVER_CHECK_READ_RC_AND_RETURN
    packet->parsed = &message;
    return G2_SERVER_delete_sensor_config_message_process(packet);
}

static int _on_receive_read_sensor_limit_message(g2_server_packet_pt packet)
{
    g2_server_probe_id_message_t message;
    int rc = G2_SERVER_probe_id_message_deserialize(packet->buffer, &message);
    G2_SERVER_CHECK_READ_RC_AND_RETURN
    packet->parsed = &message;
    return G2_SERVER_read_sensor_limit_message_process(packet);
}

static int _on_receive_write_sensor_limit_message(g2_server_packet_pt packet)
{
    g2_server_write_sensor_limit_message_t message;
    int rc = G2_SERVER_write_sensor_limit_message_deserialize(packet->buffer, &message);
    G2_SERVER_CHECK_READ_RC_AND_RETURN
    packet->parsed = &message;
    return G2_SERVER_write_sensor_limit_message_process(packet);
}

static int _on_receive_delete_sensor_limit_message(g2_server_packet_pt packet)
{
    g2_server_delete_sensor_limit_message_t message;
    int rc = G2_SERVER_delete_sensor_limit_message_deserialize(packet->buffer, &message);
    G2_SERVER_CHECK_READ_RC_AND_RETURN
    packet->parsed = &message;
    return G2_SERVER_delete_sensor_limit_message_process(packet);
}

static int _on_receive_read_sensor_alert_message(g2_server_packet_pt packet)
{
    g2_server_probe_id_message_t message;
    int rc = G2_SERVER_probe_id_message_deserialize(packet->buffer, &message);
    G2_SERVER_CHECK_READ_RC_AND_RETURN
    packet->parsed = &message;
    return G2_SERVER_read_sensor_alert_message_process(packet);
}

static int _on_receive_write_sensor_alert_message(g2_server_packet_pt packet)
{
    g2_server_write_sensor_alert_message_t message;
    int rc = G2_SERVER_write_sensor_alert_message_deserialize(packet->buffer, &message);
    G2_SERVER_CHECK_READ_RC_AND_RETURN
    packet->parsed = &message;
    return G2_SERVER_write_sensor_alert_message_process(packet);
}

static int _on_receive_delete_sensor_alert_message(g2_server_packet_pt packet)
{
    g2_server_delete_sensor_alert_message_t message;
    int rc = G2_SERVER_delete_sensor_alert_message_deserialize(packet->buffer, &message);
    G2_SERVER_CHECK_READ_RC_AND_RETURN
    packet->parsed = &message;
    return G2_SERVER_delete_sensor_alert_message_process(packet);
}

static int _on_receive_read_relay_jobs_message(g2_server_packet_pt packet)
{
    g2_server_relay_id_message_t message;
    int rc = G2_SERVER_relay_id_message_deserialize(packet->buffer, &message);
    G2_SERVER_CHECK_READ_RC_AND_RETURN
    packet->parsed = &message;
    return G2_SERVER_read_relay_jobs_message_process(packet);
}

static int _on_receive_write_relay_jobs_message(g2_server_packet_pt packet)
{
    g2_server_write_relay_jobs_message_t message;
    int rc = G2_SERVER_write_relay_jobs_message_deserialize(packet->buffer, &message);
    G2_SERVER_CHECK_READ_RC_AND_RETURN
    packet->parsed = &message;
    return G2_SERVER_write_relay_jobs_message_process(packet);
}

static int _on_receive_delete_relay_jobs_message(g2_server_packet_pt packet)
{
    g2_server_delete_relay_jobs_message_t message;
    int rc = G2_SERVER_delete_relay_jobs_message_deserialize(packet->buffer, &message);
    G2_SERVER_CHECK_READ_RC_AND_RETURN
    packet->parsed = &message;
    return G2_SERVER_delete_relay_jobs_message_process(packet);
}

static int _on_receive_read_salinity_message(g2_server_packet_pt packet)
{
    g2_server_probe_id_message_t message;
    int rc = G2_SERVER_probe_id_message_deserialize(packet->buffer, &message);
    G2_SERVER_CHECK_READ_RC_AND_RETURN
    packet->parsed = &message;
    return G2_SERVER_read_salinity_message_process(packet);
}

static int _on_receive_write_salinity_message(g2_server_packet_pt packet)
{
    g2_server_salinity_message_t message;
    int rc = G2_SERVER_salinity_message_deserialize(packet->buffer, &message);
    G2_SERVER_CHECK_READ_RC_AND_RETURN
    packet->parsed = &message;
    return G2_SERVER_write_salinity_message_process(packet);
}

static int _on_receive_write_calibrate_message(g2_server_packet_pt packet)
{
    g2_server_calibrate_message_t message;
    int rc = G2_SERVER_calibrate_message_deserialize(packet->buffer, &message);
    G2_SERVER_CHECK_READ_RC_AND_RETURN
    packet->parsed = &message;
    return G2_SERVER_write_calibrate_message_process(packet);
}

static int _on_receive_write_time_message(g2_server_packet_pt packet)
{
    g2_server_time_message_t message;
    int rc = G2_SERVER_time_message_deserialize(packet->buffer, &message);
    G2_SERVER_CHECK_READ_RC_AND_RETURN
    packet->parsed = &message;
    return G2_SERVER_write_time_message_process(packet);
}

static int _on_receive_write_relay_manual_message(g2_server_packet_pt packet)
{
    g2_server_relay_manual_message_t message;
    int rc = G2_SERVER_relay_manual_message_deserialize(packet->buffer, &message);
    G2_SERVER_CHECK_READ_RC_AND_RETURN
    packet->parsed = &message;
    return G2_SERVER_write_relay_manual_message_process(packet);
}

static int _on_receive_read_relay_manual_enable_message(g2_server_packet_pt packet)
{
    g2_server_relay_id_message_t message;
    int rc = G2_SERVER_relay_id_message_deserialize(packet->buffer, &message);
    G2_SERVER_CHECK_READ_RC_AND_RETURN
    packet->parsed = &message;
    return G2_SERVER_read_relay_manual_enable_message_process(packet);
}

static int _on_receive_write_relay_manual_enable_message(g2_server_packet_pt packet)
{
    g2_server_relay_manual_enable_message_t message;
    int rc = G2_SERVER_relay_manual_enable_message_deserialize(packet->buffer, &message);
    G2_SERVER_CHECK_READ_RC_AND_RETURN
    packet->parsed = &message;
    return G2_SERVER_write_relay_manual_enable_message_process(packet);
}

static int _on_receive_write_system_message(g2_server_packet_pt packet)
{
    g2_server_system_message_t message;
    int rc = G2_SERVER_system_message_deserialize(packet->buffer, &message);
    G2_SERVER_CHECK_READ_RC_AND_RETURN
    packet->parsed = &message;
    return G2_SERVER_write_system_message_process(packet);
}

static int _on_receive_read_connect_message(g2_server_packet_pt packet)
{
    g2_server_connect_enable_message_t message;
    int rc = G2_SERVER_connect_enable_message_deserialize(packet->buffer, &message);
    G2_SERVER_CHECK_READ_RC_AND_RETURN
    packet->parsed = &message;
    return G2_SERVER_read_connect_message_process(packet);
}

static int _on_receive_read_sensor_limit_enable_message(g2_server_packet_pt packet)
{
    g2_server_relay_id_message_t message;
    int rc = G2_SERVER_relay_id_message_deserialize(packet->buffer, &message);
    G2_SERVER_CHECK_READ_RC_AND_RETURN
    packet->parsed = &message;
    return G2_SERVER_read_sensor_limit_enable_message_process(packet);
}

static int _on_receive_write_sensor_limit_enable_message(g2_server_packet_pt packet)
{
    g2_server_sensor_limit_enable_message_t message;
    int rc = G2_SERVER_sensor_limit_enable_message_deserialize(packet->buffer, &message);
    G2_SERVER_CHECK_READ_RC_AND_RETURN
    packet->parsed = &message;
    return G2_SERVER_write_sensor_limit_enable_message_process(packet);
}

static int _on_receive_read_device_enable_message(g2_server_packet_pt packet)
{
    g2_server_device_enable_message_t message;
    int rc = G2_SERVER_device_enable_message_deserialize(packet->buffer, &message);
    G2_SERVER_CHECK_READ_RC_AND_RETURN
    packet->parsed = &message;
    return G2_SERVER_read_device_enable_message_process(packet);
}

static int _on_receive_write_device_enable_message(g2_server_packet_pt packet)
{
    g2_server_device_enable_message_t message;
    int rc = G2_SERVER_device_enable_message_deserialize(packet->buffer, &message);
    G2_SERVER_CHECK_READ_RC_AND_RETURN
    packet->parsed = &message;
    return G2_SERVER_write_device_enable_message_process(packet);
}

static int _on_receive_write_device_capability_message(g2_server_packet_pt packet)
{
    g2_server_device_capability_message_t message;
    int rc = G2_SERVER_device_capability_message_deserialize(packet->buffer, &message);
    G2_SERVER_CHECK_READ_RC_AND_RETURN
    packet->parsed = &message;
    return G2_SERVER_write_device_capability_message_process(packet);
}

static int _on_receive_read_relay_data_message(g2_server_packet_pt packet)
{
    g2_server_relay_id_message_t message;
    int rc = G2_SERVER_relay_id_message_deserialize(packet->buffer, &message);
    G2_SERVER_CHECK_READ_RC_AND_RETURN
    packet->parsed = &message;
    return G2_SERVER_read_relay_data_message_process(packet);
}


static int _on_receive_read_sensor_data_message(g2_server_packet_pt packet)
{
    g2_server_probe_id_message_t message;
    int rc = G2_SERVER_probe_id_message_deserialize(packet->buffer, &message);
    G2_SERVER_CHECK_READ_RC_AND_RETURN
    packet->parsed = &message;
    return G2_SERVER_read_sensor_data_message_process(packet);
}

static int _on_receive_write_device_used_capability_message(g2_server_packet_pt packet)
{
    g2_server_device_used_capability_message_t message;
    int rc = G2_SERVER_device_used_capability_message_deserialize(packet->buffer, &message);
    G2_SERVER_CHECK_READ_RC_AND_RETURN
    packet->parsed = &message;
    return G2_SERVER_write_device_used_capability_message_process(packet);
}

static int _on_receive_read_relay_config_message(g2_server_packet_pt packet)
{
    g2_server_relay_id_message_t message;
    int rc = G2_SERVER_relay_id_message_deserialize(packet->buffer, &message);
    G2_SERVER_CHECK_READ_RC_AND_RETURN
    packet->parsed = &message;
    return G2_SERVER_read_relay_config_message_process(packet);
}

static int _on_receive_write_relay_config_message(g2_server_packet_pt packet)
{
    g2_server_write_relay_config_message_t message;
    int rc = G2_SERVER_write_relay_config_message_deserialize(packet->buffer, &message);
    G2_SERVER_CHECK_READ_RC_AND_RETURN
    packet->parsed = &message;
    return G2_SERVER_write_relay_config_message_process(packet);
}

static int _on_receive_delete_relay_config_message(g2_server_packet_pt packet)
{
    g2_server_delete_relay_config_message_t message;
    int rc = G2_SERVER_delete_relay_config_message_deserialize(packet->buffer, &message);
    G2_SERVER_CHECK_READ_RC_AND_RETURN
    packet->parsed = &message;
    return G2_SERVER_delete_relay_config_message_process(packet);
}

static int _on_receive_read_relay_jobs_enable_message(g2_server_packet_pt packet)
{
    g2_server_relay_id_message_t message;
    int rc = G2_SERVER_relay_id_message_deserialize(packet->buffer, &message);
    G2_SERVER_CHECK_READ_RC_AND_RETURN
    packet->parsed = &message;
    return G2_SERVER_read_relay_jobs_enable_message_process(packet);
}

static int _on_receive_write_relay_jobs_enable_message(g2_server_packet_pt packet)
{
    g2_server_relay_jobs_enable_message_t message;
    int rc = G2_SERVER_relay_jobs_enable_message_deserialize(packet->buffer, &message);
    G2_SERVER_CHECK_READ_RC_AND_RETURN
    packet->parsed = &message;
    return G2_SERVER_write_relay_jobs_enable_message_process(packet);
}

static int _on_receive_read_pump_message(g2_server_packet_pt packet)
{
    g2_server_relay_id_message_t message;
    int rc = G2_SERVER_relay_id_message_deserialize(packet->buffer, &message);
    G2_SERVER_CHECK_READ_RC_AND_RETURN
    packet->parsed = &message;
    return G2_SERVER_read_pump_message_process(packet);
}

static int _on_receive_write_pump_message(g2_server_packet_pt packet)
{
    g2_server_pump_message_t message;
    int rc = G2_SERVER_pump_message_deserialize(packet->buffer, &message);
    G2_SERVER_CHECK_READ_RC_AND_RETURN
    packet->parsed = &message;
    return G2_SERVER_write_pump_message_process(packet);
}

static int _on_receive_write_l6_start_message(g2_server_packet_pt packet)
{
    g2_server_probe_id_message_t message;
    int rc = G2_SERVER_probe_id_message_deserialize(packet->buffer, &message);
    G2_SERVER_CHECK_READ_RC_AND_RETURN
    packet->parsed = &message;
    return G2_SERVER_write_l6_start_message_process(packet);
}

static int _on_receive_write_l6_settings_message(g2_server_packet_pt packet)
{
    g2_server_l6_settings_message_t message;
    int rc = G2_SERVER_l6_settings_message_deserialize(packet->buffer, &message);
    G2_SERVER_CHECK_READ_RC_AND_RETURN
    packet->parsed = &message;
    return G2_SERVER_write_l6_settings_message_process(packet);
}

static int _on_receive_read_l6_settings_message(g2_server_packet_pt packet)
{
    g2_server_probe_id_message_t message;
    int rc = G2_SERVER_probe_id_message_deserialize(packet->buffer, &message);
    G2_SERVER_CHECK_READ_RC_AND_RETURN
    packet->parsed = &message;
    return G2_SERVER_read_l6_settings_message_process(packet);
}

static int _on_receive_read_l6_status_message(g2_server_packet_pt packet)
{
    g2_server_probe_id_message_t message;
    int rc = G2_SERVER_probe_id_message_deserialize(packet->buffer, &message);
    G2_SERVER_CHECK_READ_RC_AND_RETURN
    packet->parsed = &message;
    return G2_SERVER_read_l6_status_message_process(packet);
}


static int _on_receive_write_firmware_start_message(g2_server_packet_pt packet)
{
    int rc = 0;
    g2_server_firmware_start_message_t message;

    rc = G2_SERVER_firmware_start_message_deserialize(packet->buffer, &message);
    G2_SERVER_CHECK_READ_RC_AND_RETURN
    packet->parsed = &message;

    return G2_SERVER_WriteFirmwareStartMessageProcess(packet);
}


static int _on_receive_write_firmware_data_message(g2_server_packet_pt packet)
{
    int rc = 0;	
    g2_server_firmware_data_message_t message;

    rc = G2_SERVER_firmware_data_message_deserialize(packet->buffer, &message);
    G2_SERVER_CHECK_READ_RC_AND_RETURN
    packet->parsed = &message;

    return G2_SERVER_WriteFirmwareDataMessageProcess(packet);
}


static int _on_receive_write_relay_power_config_message(g2_server_packet_pt packet)
{
    g2_server_relay_power_config_message_t message = {0};
    int rc = G2_SERVER_relay_power_config_message_deserialize(packet->buffer, &message);
    G2_SERVER_CHECK_READ_RC_AND_RETURN
    packet->parsed = &message;
    return G2_SERVER_write_relay_power_config_message_process(packet);
}


#ifdef BOARD_HAS_RELAY
static int _on_receive_read_relay_power_config_message(g2_server_packet_pt packet)
{
    g2_server_probe_id_message_t message = {0};
    int rc = G2_SERVER_probe_id_message_deserialize(packet->buffer, &message);
    G2_SERVER_CHECK_READ_RC_AND_RETURN
    packet->parsed = &message;
    return G2_SERVER_read_relay_power_config_message_process(packet);
}
#endif


static int _on_receive_write_loader_config_message(g2_server_packet_pt packet)
{
    g2_server_loader_config_message_t message = {0};
    int rc = G2_SERVER_loader_config_message_deserialize(packet->buffer, &message);
    G2_SERVER_CHECK_READ_RC_AND_RETURN
    packet->parsed = &message;
    return G2_SERVER_write_loader_config_message_process(packet);
}


static int _on_receive_read_loader_config_message(g2_server_packet_pt packet)
{
    g2_server_probe_id_message_t message = {0};
    int rc = G2_SERVER_probe_id_message_deserialize(packet->buffer, &message);
    G2_SERVER_CHECK_READ_RC_AND_RETURN
    packet->parsed = &message;
    return G2_SERVER_read_loader_config_message_process(packet);
}

WINAPI_EXPORT int WINAPI G2_SERVER_receive_packet(g2_server_packet_pt packet)
{
    if (packet->code == G2_SERVER_MESSAGE_OPERATION_READ) 
    {
        switch (packet->cmd)
        {
        case G2_SERVER_MESSAGE_DEVICE_ID:
            return G2_SERVER_read_device_id_message_process(packet);
        case G2_SERVER_MESSAGE_VERSION:
            return G2_SERVER_read_version_message_process(packet);
        case G2_SERVER_MESSAGE_SENSOR_CONFIG:
            return _on_receive_read_sensor_config_message(packet);
        case G2_SERVER_MESSAGE_SENSOR_LIMIT:
            return _on_receive_read_sensor_limit_message(packet);
        case G2_SERVER_MESSAGE_SENSOR_ALERT:
            return _on_receive_read_sensor_alert_message(packet);
        case G2_SERVER_MESSAGE_RELAY_JOBS:
            return _on_receive_read_relay_jobs_message(packet);
        case G2_SERVER_MESSAGE_SALINITY:
            return _on_receive_read_salinity_message(packet);
        case G2_SERVER_MESSAGE_TIME:
            return G2_SERVER_read_time_message_process(packet);
        case G2_SERVER_MESSAGE_RELAY_MANUAL_ENABLE:
            return _on_receive_read_relay_manual_enable_message(packet);
        case G2_SERVER_MESSAGE_SYSTEM:
            return G2_SERVER_read_system_message_process(packet);
        case G2_SERVER_MESSAGE_CONNECT:
            return _on_receive_read_connect_message(packet);
        case G2_SERVER_MESSAGE_SENSOR_LIMIT_ENABLE:
            return _on_receive_read_sensor_limit_enable_message(packet);
        case G2_SERVER_MESSAGE_DEVICE_ENABLE:
            return _on_receive_read_device_enable_message(packet);
        case G2_SERVER_MESSAGE_DEVICE_CAPABILITY:
            return G2_SERVER_read_device_capability_message_process(packet);
        case G2_SERVER_MESSAGE_RELAY_DATA:
            return _on_receive_read_relay_data_message(packet);
        case G2_SERVER_MESSAGE_SENSOR_DATA:
            return _on_receive_read_sensor_data_message(packet);
        case G2_SERVER_MESSAGE_DEVICE_USED_CAPABILITY:
            return G2_SERVER_read_device_used_capability_message_process(packet);
        case G2_SERVER_MESSAGE_DEVICE_SENSORS:
            return G2_SERVER_read_device_sensors_message_process(packet);
        case G2_SERVER_MESSAGE_RELAY_CONFIG:
            return _on_receive_read_relay_config_message(packet);
        case G2_SERVER_MESSAGE_RELAY_JOBS_ENABLE:
            return _on_receive_read_relay_jobs_enable_message(packet);
        case G2_SERVER_MESSAGE_PUMP:
            return _on_receive_read_pump_message(packet);
        case G2_SERVER_MESSAGE_GPRS_SIGNAL:
            return G2_SERVER_read_gprs_signal_message_process(packet);
        case G2_SERVER_MESSAGE_HEARTBEAT:
            return G2_SERVER_read_heartbeat_message_process(packet);
        case G2_SERVER_MESSAGE_GET_TIME:
            return G2_SERVER_read_get_time_message_process(packet);				
        case G2_SERVER_MESSAGE_L6_SETTINGS:
            return _on_receive_read_l6_settings_message(packet);
        case G2_SERVER_MESSAGE_L6_STATUS:
            return _on_receive_read_l6_status_message(packet);
        case G2_SERVER_MESSAGE_ENVIRONMENT:
            return G2_SERVER_read_environment_message_process(packet);
        case G2_SERVER_MESSAGE_READ_ICCID:
            return G2_SERVER_ReadIccidMessageProcess(packet);
#ifdef BOARD_HAS_RELAY
        case G2_SERVER_MESSAGE_RELAY_POWER_CONFIG:
            return _on_receive_read_relay_power_config_message(packet);  
#endif            
        case G2_SERVER_MESSAGE_LOADER_COONFIG:
            return _on_receive_read_loader_config_message(packet);     
        }
    }
    if (packet->code == G2_SERVER_MESSAGE_OPERATION_DELETE) 
    {
        switch (packet->cmd)
        {
        case G2_SERVER_MESSAGE_SENSOR_CONFIG:
            return _on_receive_delete_sensor_config_message(packet);
        case G2_SERVER_MESSAGE_SENSOR_LIMIT:
            return _on_receive_delete_sensor_limit_message(packet);
        case G2_SERVER_MESSAGE_SENSOR_ALERT:
            return _on_receive_delete_sensor_alert_message(packet);
        case G2_SERVER_MESSAGE_RELAY_JOBS:
            return _on_receive_delete_relay_jobs_message(packet);
        case G2_SERVER_MESSAGE_RELAY_CONFIG:
            return _on_receive_delete_relay_config_message(packet);
        }
    }
    if (packet->code == G2_SERVER_MESSAGE_OPERATION_WRITE) 
    {
        switch (packet->cmd)
        {
        case G2_SERVER_MESSAGE_DEVICE_ID:
            return _on_receive_write_device_id_message(packet);
        case G2_SERVER_MESSAGE_VERSION:
            return _on_receive_write_version_message(packet);
        case G2_SERVER_MESSAGE_SENSOR_CONFIG:
            return _on_receive_write_sensor_config_message(packet);
        case G2_SERVER_MESSAGE_SENSOR_LIMIT:
            return _on_receive_write_sensor_limit_message(packet);
        case G2_SERVER_MESSAGE_SENSOR_ALERT:
            return _on_receive_write_sensor_alert_message(packet);
        case G2_SERVER_MESSAGE_RELAY_JOBS:
            return _on_receive_write_relay_jobs_message(packet);
        case G2_SERVER_MESSAGE_SALINITY:
            return _on_receive_write_salinity_message(packet);
        case G2_SERVER_MESSAGE_CALIBRATE:
            return _on_receive_write_calibrate_message(packet);
        case G2_SERVER_MESSAGE_TIME:
            return _on_receive_write_time_message(packet);
        case G2_SERVER_MESSAGE_RELAY_MANUAL:
            return _on_receive_write_relay_manual_message(packet);
        case G2_SERVER_MESSAGE_RELAY_MANUAL_ENABLE:
            return _on_receive_write_relay_manual_enable_message(packet);
        case G2_SERVER_MESSAGE_SYSTEM:
            return _on_receive_write_system_message(packet);
        case G2_SERVER_MESSAGE_SENSOR_LIMIT_ENABLE:
            return _on_receive_write_sensor_limit_enable_message(packet);
        case G2_SERVER_MESSAGE_DEVICE_ENABLE:
            return _on_receive_write_device_enable_message(packet);
        case G2_SERVER_MESSAGE_DEVICE_CAPABILITY:
            return _on_receive_write_device_capability_message(packet);
        case G2_SERVER_MESSAGE_RECOVERY:
            return G2_SERVER_write_recovery_message_process(packet);
        case G2_SERVER_MESSAGE_DEVICE_USED_CAPABILITY:
            return _on_receive_write_device_used_capability_message(packet);
        case G2_SERVER_MESSAGE_RELAY_CONFIG:
            return _on_receive_write_relay_config_message(packet);
        case G2_SERVER_MESSAGE_RELAY_JOBS_ENABLE:
            return _on_receive_write_relay_jobs_enable_message(packet);
        case G2_SERVER_MESSAGE_PUMP:
            return _on_receive_write_pump_message(packet);
        case G2_SERVER_MESSAGE_RESTART:
            return G2_SERVER_write_restart_message_process(packet);
        case G2_SERVER_MESSAGE_L6_START:
            return _on_receive_write_l6_start_message(packet);
        case G2_SERVER_MESSAGE_L6_SETTINGS:
            return _on_receive_write_l6_settings_message(packet);
        case G2_SERVER_MESSAGE_FIREWARE_START:				
            return _on_receive_write_firmware_start_message(packet);
        case G2_SERVER_MESSAGE_FIREWARE_DATA:
            return _on_receive_write_firmware_data_message(packet);
        case G2_SERVER_MESSAGE_RELAY_POWER_CONFIG:
            return _on_receive_write_relay_power_config_message(packet);
        case G2_SERVER_MESSAGE_LOADER_COONFIG:
            return _on_receive_write_loader_config_message(packet);
        }
    }
    return PROTOCOL_NOT_SUPPORT;
}

WINAPI_EXPORT int WINAPI G2_SERVER_send_read_device_id_message(g2_server_port_pt port, g2_server_packet_pt packet, g2_server_device_id_message_pt message)
{
    packet->length = G2_SERVER_device_id_message_serialize(packet->buffer, message);
    packet->parsed = message;
    packet->cmd_status = 0xd0;
    packet->cmd = G2_SERVER_MESSAGE_DEVICE_ID;
    packet->code = G2_SERVER_MESSAGE_OPERATION_READ;
    return G2_SERVER_PACKET_write(port, packet);
}

WINAPI_EXPORT int WINAPI G2_SERVER_send_write_device_id_message(g2_server_port_pt port, g2_server_packet_pt packet)
{
    packet->length = 0;
    packet->parsed = NULL;
    packet->cmd_status = 0xd0;
    packet->cmd = G2_SERVER_MESSAGE_DEVICE_ID;
    packet->code = G2_SERVER_MESSAGE_OPERATION_WRITE;
    return G2_SERVER_PACKET_write(port, packet);
}

WINAPI_EXPORT int WINAPI G2_SERVER_send_read_version_message(g2_server_port_pt port, g2_server_packet_pt packet, g2_server_device_version_message_pt message)
{
    packet->length = G2_SERVER_device_version_message_serialize(packet->buffer, message);
    packet->parsed = message;
    packet->cmd_status = 0xd0;
    packet->cmd = G2_SERVER_MESSAGE_VERSION;
    packet->code = G2_SERVER_MESSAGE_OPERATION_READ;
    return G2_SERVER_PACKET_write(port, packet);
}

WINAPI_EXPORT int WINAPI G2_SERVER_send_write_version_message(g2_server_port_pt port, g2_server_packet_pt packet)
{
    packet->length = 0;
    packet->parsed = NULL;
    packet->cmd_status = 0xd0;
    packet->cmd = G2_SERVER_MESSAGE_VERSION;
    packet->code = G2_SERVER_MESSAGE_OPERATION_WRITE;
    return G2_SERVER_PACKET_write(port, packet);
}

WINAPI_EXPORT int WINAPI G2_SERVER_send_read_sensor_config_message(g2_server_port_pt port, g2_server_packet_pt packet, g2_server_sensor_config_message_pt message)
{
    packet->length = G2_SERVER_sensor_config_message_serialize(packet->buffer, message);
    packet->parsed = message;
    packet->cmd_status = 0xd0;
    packet->cmd = G2_SERVER_MESSAGE_SENSOR_CONFIG;
    packet->code = G2_SERVER_MESSAGE_OPERATION_READ;
    return G2_SERVER_PACKET_write(port, packet);
}

WINAPI_EXPORT int WINAPI G2_SERVER_send_write_sensor_config_message(g2_server_port_pt port, g2_server_packet_pt packet)
{
    packet->length = 0;
    packet->parsed = NULL;
    packet->cmd_status = 0xd0;
    packet->cmd = G2_SERVER_MESSAGE_SENSOR_CONFIG;
    packet->code = G2_SERVER_MESSAGE_OPERATION_WRITE;
    return G2_SERVER_PACKET_write(port, packet);
}

WINAPI_EXPORT int WINAPI G2_SERVER_send_delete_sensor_config_message(g2_server_port_pt port, g2_server_packet_pt packet)
{
    packet->length = 0;
    packet->parsed = NULL;
    packet->cmd_status = 0xd0;
    packet->cmd = G2_SERVER_MESSAGE_SENSOR_CONFIG;
    packet->code = G2_SERVER_MESSAGE_OPERATION_DELETE;
    return G2_SERVER_PACKET_write(port, packet);
}

WINAPI_EXPORT int WINAPI G2_SERVER_send_read_sensor_limit_message(g2_server_port_pt port, g2_server_packet_pt packet, g2_server_sensor_limit_message_pt message)
{
    packet->length = G2_SERVER_sensor_limit_message_serialize(packet->buffer, message);
    packet->parsed = message;
    packet->cmd_status = 0xd0;
    packet->cmd = G2_SERVER_MESSAGE_SENSOR_LIMIT;
    packet->code = G2_SERVER_MESSAGE_OPERATION_READ;
    return G2_SERVER_PACKET_write(port, packet);
}

WINAPI_EXPORT int WINAPI G2_SERVER_send_write_sensor_limit_message(g2_server_port_pt port, g2_server_packet_pt packet)
{
    packet->length = 0;
    packet->parsed = NULL;
    packet->cmd_status = 0xd0;
    packet->cmd = G2_SERVER_MESSAGE_SENSOR_LIMIT;
    packet->code = G2_SERVER_MESSAGE_OPERATION_WRITE;
    return G2_SERVER_PACKET_write(port, packet);
}

WINAPI_EXPORT int WINAPI G2_SERVER_send_delete_sensor_limit_message(g2_server_port_pt port, g2_server_packet_pt packet)
{
    packet->length = 0;
    packet->parsed = NULL;
    packet->cmd_status = 0xd0;
    packet->cmd = G2_SERVER_MESSAGE_SENSOR_LIMIT;
    packet->code = G2_SERVER_MESSAGE_OPERATION_DELETE;
    return G2_SERVER_PACKET_write(port, packet);
}

WINAPI_EXPORT int WINAPI G2_SERVER_send_read_sensor_alert_message(g2_server_port_pt port, g2_server_packet_pt packet, g2_server_sensor_alert_message_pt message)
{
    packet->length = G2_SERVER_sensor_alert_message_serialize(packet->buffer, message);
    packet->parsed = message;
    packet->cmd_status = 0xd0;
    packet->cmd = G2_SERVER_MESSAGE_SENSOR_ALERT;
    packet->code = G2_SERVER_MESSAGE_OPERATION_READ;
    return G2_SERVER_PACKET_write(port, packet);
}

WINAPI_EXPORT int WINAPI G2_SERVER_send_write_sensor_alert_message(g2_server_port_pt port, g2_server_packet_pt packet)
{
    packet->length = 0;
    packet->parsed = NULL;
    packet->cmd_status = 0xd0;
    packet->cmd = G2_SERVER_MESSAGE_SENSOR_ALERT;
    packet->code = G2_SERVER_MESSAGE_OPERATION_WRITE;
    return G2_SERVER_PACKET_write(port, packet);
}

WINAPI_EXPORT int WINAPI G2_SERVER_send_delete_sensor_alert_message(g2_server_port_pt port, g2_server_packet_pt packet)
{
    packet->length = 0;
    packet->parsed = NULL;
    packet->cmd_status = 0xd0;
    packet->cmd = G2_SERVER_MESSAGE_SENSOR_ALERT;
    packet->code = G2_SERVER_MESSAGE_OPERATION_DELETE;
    return G2_SERVER_PACKET_write(port, packet);
}

WINAPI_EXPORT int WINAPI G2_SERVER_send_read_relay_jobs_message(g2_server_port_pt port, g2_server_packet_pt packet, g2_server_relay_jobs_message_pt message)
{
    packet->length = G2_SERVER_relay_jobs_message_serialize(packet->buffer, message);
    packet->parsed = message;
    packet->cmd_status = 0xd0;
    packet->cmd = G2_SERVER_MESSAGE_RELAY_JOBS;
    packet->code = G2_SERVER_MESSAGE_OPERATION_READ;
    return G2_SERVER_PACKET_write(port, packet);
}

WINAPI_EXPORT int WINAPI G2_SERVER_send_write_relay_jobs_message(g2_server_port_pt port, g2_server_packet_pt packet)
{
    packet->length = 0;
    packet->parsed = NULL;
    packet->cmd_status = 0xd0;
    packet->cmd = G2_SERVER_MESSAGE_RELAY_JOBS;
    packet->code = G2_SERVER_MESSAGE_OPERATION_WRITE;
    return G2_SERVER_PACKET_write(port, packet);
}

WINAPI_EXPORT int WINAPI G2_SERVER_send_delete_relay_jobs_message(g2_server_port_pt port, g2_server_packet_pt packet)
{
    packet->length = 0;
    packet->parsed = NULL;
    packet->cmd_status = 0xd0;
    packet->cmd = G2_SERVER_MESSAGE_RELAY_JOBS;
    packet->code = G2_SERVER_MESSAGE_OPERATION_DELETE;
    return G2_SERVER_PACKET_write(port, packet);
}

WINAPI_EXPORT int WINAPI G2_SERVER_send_read_salinity_message(g2_server_port_pt port, g2_server_packet_pt packet, g2_server_salinity_message_pt message)
{
    packet->length = G2_SERVER_salinity_message_serialize(packet->buffer, message);
    packet->parsed = message;
    packet->cmd_status = 0xd0;
    packet->cmd = G2_SERVER_MESSAGE_SALINITY;
    packet->code = G2_SERVER_MESSAGE_OPERATION_READ;
    return G2_SERVER_PACKET_write(port, packet);
}

WINAPI_EXPORT int WINAPI G2_SERVER_send_write_salinity_message(g2_server_port_pt port, g2_server_packet_pt packet)
{
    packet->length = 0;
    packet->parsed = NULL;
    packet->cmd_status = 0xd0;
    packet->cmd = G2_SERVER_MESSAGE_SALINITY;
    packet->code = G2_SERVER_MESSAGE_OPERATION_WRITE;
    return G2_SERVER_PACKET_write(port, packet);
}

WINAPI_EXPORT int WINAPI G2_SERVER_send_write_calibrate_message(g2_server_port_pt port, g2_server_packet_pt packet)
{
    packet->length = 0;
    packet->parsed = NULL;
    packet->cmd_status = 0xd0;
    packet->cmd = G2_SERVER_MESSAGE_CALIBRATE;
    packet->code = G2_SERVER_MESSAGE_OPERATION_WRITE;
    return G2_SERVER_PACKET_write(port, packet);
}

WINAPI_EXPORT int WINAPI G2_SERVER_send_write_time_message(g2_server_port_pt port, g2_server_packet_pt packet)
{
    packet->length = 0;
    packet->parsed = NULL;
    packet->cmd_status = 0xd0;
    packet->cmd = G2_SERVER_MESSAGE_TIME;
    packet->code = G2_SERVER_MESSAGE_OPERATION_WRITE;
    return G2_SERVER_PACKET_write(port, packet);
}

WINAPI_EXPORT int WINAPI G2_SERVER_send_read_time_message(g2_server_port_pt port, g2_server_packet_pt packet, g2_server_time_message_pt message)
{
    packet->length = G2_SERVER_time_message_serialize(packet->buffer, message);
    packet->parsed = message;
    packet->cmd_status = 0xd0;
    packet->cmd = G2_SERVER_MESSAGE_TIME;
    packet->code = G2_SERVER_MESSAGE_OPERATION_READ;
    return G2_SERVER_PACKET_write(port, packet);
}

WINAPI_EXPORT int WINAPI G2_SERVER_send_write_relay_manual_message(g2_server_port_pt port, g2_server_packet_pt packet)
{
    packet->length = 0;
    packet->parsed = NULL;
    packet->cmd_status = 0xd0;
    packet->cmd = G2_SERVER_MESSAGE_RELAY_MANUAL;
    packet->code = G2_SERVER_MESSAGE_OPERATION_WRITE;
    return G2_SERVER_PACKET_write(port, packet);
}

WINAPI_EXPORT int WINAPI G2_SERVER_send_read_relay_manual_enable_message(g2_server_port_pt port, g2_server_packet_pt packet, g2_server_relay_manual_enable_message_pt message)
{
    packet->length = G2_SERVER_relay_manual_enable_message_serialize(packet->buffer, message);
    packet->parsed = message;
    packet->cmd_status = 0xd0;
    packet->cmd = G2_SERVER_MESSAGE_RELAY_MANUAL_ENABLE;
    packet->code = G2_SERVER_MESSAGE_OPERATION_READ;
    return G2_SERVER_PACKET_write(port, packet);
}

WINAPI_EXPORT int WINAPI G2_SERVER_send_write_relay_manual_enable_message(g2_server_port_pt port, g2_server_packet_pt packet)
{
    packet->length = 0;
    packet->parsed = NULL;
    packet->cmd_status = 0xd0;
    packet->cmd = G2_SERVER_MESSAGE_RELAY_MANUAL_ENABLE;
    packet->code = G2_SERVER_MESSAGE_OPERATION_WRITE;
    return G2_SERVER_PACKET_write(port, packet);
}

WINAPI_EXPORT int WINAPI G2_SERVER_send_read_system_message(g2_server_port_pt port, g2_server_packet_pt packet, g2_server_system_message_pt message)
{
    packet->length = G2_SERVER_system_message_serialize(packet->buffer, message);
    packet->parsed = message;
    packet->cmd_status = 0xd0;
    packet->cmd = G2_SERVER_MESSAGE_SYSTEM;
    packet->code = G2_SERVER_MESSAGE_OPERATION_READ;
    return G2_SERVER_PACKET_write(port, packet);
}

WINAPI_EXPORT int WINAPI G2_SERVER_send_write_system_message(g2_server_port_pt port, g2_server_packet_pt packet)
{
    packet->length = 0;
    packet->parsed = NULL;
    packet->cmd_status = 0xd0;
    packet->cmd = G2_SERVER_MESSAGE_SYSTEM;
    packet->code = G2_SERVER_MESSAGE_OPERATION_WRITE;
    return G2_SERVER_PACKET_write(port, packet);
}

WINAPI_EXPORT int WINAPI G2_SERVER_send_read_connect_message(g2_server_port_pt port, g2_server_packet_pt packet, g2_server_connect_message_pt message)
{
    packet->length = G2_SERVER_connect_message_serialize(packet->buffer, message);
    packet->parsed = message;
    packet->cmd_status = 0xd0;
    packet->cmd = G2_SERVER_MESSAGE_CONNECT;
    packet->code = G2_SERVER_MESSAGE_OPERATION_READ;
    return G2_SERVER_PACKET_write(port, packet);
}

WINAPI_EXPORT int WINAPI G2_SERVER_send_read_sensor_limit_enable_message(g2_server_port_pt port, g2_server_packet_pt packet, g2_server_sensor_limit_enable_message_pt message)
{
    packet->length = G2_SERVER_sensor_limit_enable_message_serialize(packet->buffer, message);
    packet->parsed = message;
    packet->cmd_status = 0xd0;
    packet->cmd = G2_SERVER_MESSAGE_SENSOR_LIMIT_ENABLE;
    packet->code = G2_SERVER_MESSAGE_OPERATION_READ;
    return G2_SERVER_PACKET_write(port, packet);
}

WINAPI_EXPORT int WINAPI G2_SERVER_send_write_sensor_limit_enable_message(g2_server_port_pt port, g2_server_packet_pt packet)
{
    packet->length = 0;
    packet->parsed = NULL;
    packet->cmd_status = 0xd0;
    packet->cmd = G2_SERVER_MESSAGE_SENSOR_LIMIT_ENABLE;
    packet->code = G2_SERVER_MESSAGE_OPERATION_WRITE;
    return G2_SERVER_PACKET_write(port, packet);
}

WINAPI_EXPORT int WINAPI G2_SERVER_send_read_device_enable_message(g2_server_port_pt port, g2_server_packet_pt packet, g2_server_device_enable_message_pt message)
{
    packet->length = G2_SERVER_device_enable_message_serialize(packet->buffer, message);
    packet->parsed = message;
    packet->cmd_status = 0xd0;
    packet->cmd = G2_SERVER_MESSAGE_DEVICE_ENABLE;
    packet->code = G2_SERVER_MESSAGE_OPERATION_READ;
    return G2_SERVER_PACKET_write(port, packet);
}

WINAPI_EXPORT int WINAPI G2_SERVER_send_write_device_enable_message(g2_server_port_pt port, g2_server_packet_pt packet)
{
    packet->length = 0;
    packet->parsed = NULL;
    packet->cmd_status = 0xd0;
    packet->cmd = G2_SERVER_MESSAGE_DEVICE_ENABLE;
    packet->code = G2_SERVER_MESSAGE_OPERATION_WRITE;
    return G2_SERVER_PACKET_write(port, packet);
}

WINAPI_EXPORT int WINAPI G2_SERVER_send_read_device_capability_message(g2_server_port_pt port, g2_server_packet_pt packet, g2_server_device_capability_message_pt message)
{
    packet->length = G2_SERVER_device_capability_message_serialize(packet->buffer, message);
    packet->parsed = message;
    packet->cmd_status = 0xd0;
    packet->cmd = G2_SERVER_MESSAGE_DEVICE_CAPABILITY;
    packet->code = G2_SERVER_MESSAGE_OPERATION_READ;
    return G2_SERVER_PACKET_write(port, packet);
}

WINAPI_EXPORT int WINAPI G2_SERVER_send_write_device_capability_message(g2_server_port_pt port, g2_server_packet_pt packet)
{
    packet->length = 0;
    packet->parsed = NULL;
    packet->cmd_status = 0xd0;
    packet->cmd = G2_SERVER_MESSAGE_DEVICE_CAPABILITY;
    packet->code = G2_SERVER_MESSAGE_OPERATION_WRITE;
    return G2_SERVER_PACKET_write(port, packet);
}

WINAPI_EXPORT int WINAPI G2_SERVER_send_read_relay_data_message(g2_server_port_pt port, g2_server_packet_pt packet, g2_server_relay_data_message_pt message)
{
    packet->length = G2_SERVER_relay_data_message_serialize(packet->buffer, message);
    packet->parsed = message;
    packet->cmd_status = 0xd0;
    packet->cmd = G2_SERVER_MESSAGE_RELAY_DATA;
    packet->code = G2_SERVER_MESSAGE_OPERATION_READ;
    return G2_SERVER_PACKET_write(port, packet);
}


WINAPI_EXPORT int WINAPI G2_SERVER_send_read_sensor_data_message(g2_server_port_pt port, g2_server_packet_pt packet, g2_server_sensor_data_message_pt message)
{
    packet->length = G2_SERVER_sensor_data_message_serialize(packet->buffer, message);
    packet->parsed = message;
    packet->cmd_status = 0xd0;
    packet->cmd = G2_SERVER_MESSAGE_SENSOR_DATA;
    packet->code = G2_SERVER_MESSAGE_OPERATION_READ;
    return G2_SERVER_PACKET_write(port, packet);
}

WINAPI_EXPORT int WINAPI G2_SERVER_send_write_recovery_message(g2_server_port_pt port, g2_server_packet_pt packet)
{
    packet->length = 0;
    packet->parsed = NULL;
    packet->cmd_status = 0xd0;
    packet->cmd = G2_SERVER_MESSAGE_RECOVERY;
    packet->code = G2_SERVER_MESSAGE_OPERATION_WRITE;
    return G2_SERVER_PACKET_write(port, packet);
}

WINAPI_EXPORT int WINAPI G2_SERVER_send_read_device_used_capability_message(g2_server_port_pt port, g2_server_packet_pt packet, g2_server_device_used_capability_message_pt message)
{
    packet->length = G2_SERVER_device_used_capability_message_serialize(packet->buffer, message);
    packet->parsed = message;
    packet->cmd_status = 0xd0;
    packet->cmd = G2_SERVER_MESSAGE_DEVICE_USED_CAPABILITY;
    packet->code = G2_SERVER_MESSAGE_OPERATION_READ;
    return G2_SERVER_PACKET_write(port, packet);
}

WINAPI_EXPORT int WINAPI G2_SERVER_send_write_device_used_capability_message(g2_server_port_pt port, g2_server_packet_pt packet)
{
    packet->length = 0;
    packet->parsed = NULL;
    packet->cmd_status = 0xd0;
    packet->cmd = G2_SERVER_MESSAGE_DEVICE_USED_CAPABILITY;
    packet->code = G2_SERVER_MESSAGE_OPERATION_WRITE;
    return G2_SERVER_PACKET_write(port, packet);
}

WINAPI_EXPORT int WINAPI G2_SERVER_send_read_relay_config_message(g2_server_port_pt port, g2_server_packet_pt packet, g2_server_relay_config_message_pt message)
{
    packet->length = G2_SERVER_relay_config_message_serialize(packet->buffer, message);
    packet->parsed = message;
    packet->cmd_status = 0xd0;
    packet->cmd = G2_SERVER_MESSAGE_RELAY_CONFIG;
    packet->code = G2_SERVER_MESSAGE_OPERATION_READ;
    return G2_SERVER_PACKET_write(port, packet);
}

WINAPI_EXPORT int WINAPI G2_SERVER_send_write_relay_config_message(g2_server_port_pt port, g2_server_packet_pt packet)
{
    packet->length = 0;
    packet->parsed = NULL;
    packet->cmd_status = 0xd0;
    packet->cmd = G2_SERVER_MESSAGE_RELAY_CONFIG;
    packet->code = G2_SERVER_MESSAGE_OPERATION_WRITE;
    return G2_SERVER_PACKET_write(port, packet);
}

WINAPI_EXPORT int WINAPI G2_SERVER_send_delete_relay_config_message(g2_server_port_pt port, g2_server_packet_pt packet)
{
    packet->length = 0;
    packet->parsed = NULL;
    packet->cmd_status = 0xd0;
    packet->cmd = G2_SERVER_MESSAGE_RELAY_CONFIG;
    packet->code = G2_SERVER_MESSAGE_OPERATION_DELETE;
    return G2_SERVER_PACKET_write(port, packet);
}

WINAPI_EXPORT int WINAPI G2_SERVER_send_read_relay_jobs_enable_message(g2_server_port_pt port, g2_server_packet_pt packet, g2_server_relay_jobs_enable_message_pt message)
{
    packet->length = G2_SERVER_relay_jobs_enable_message_serialize(packet->buffer, message);
    packet->parsed = message;
    packet->cmd_status = 0xd0;
    packet->cmd = G2_SERVER_MESSAGE_RELAY_JOBS_ENABLE;
    packet->code = G2_SERVER_MESSAGE_OPERATION_READ;
    return G2_SERVER_PACKET_write(port, packet);
}

WINAPI_EXPORT int WINAPI G2_SERVER_send_write_relay_jobs_enable_message(g2_server_port_pt port, g2_server_packet_pt packet)
{
    packet->length = 0;
    packet->parsed = NULL;
    packet->cmd_status = 0xd0;
    packet->cmd = G2_SERVER_MESSAGE_RELAY_JOBS_ENABLE;
    packet->code = G2_SERVER_MESSAGE_OPERATION_WRITE;
    return G2_SERVER_PACKET_write(port, packet);
}

WINAPI_EXPORT int WINAPI G2_SERVER_send_read_pump_message(g2_server_port_pt port, g2_server_packet_pt packet, g2_server_pump_message_pt message)
{
    packet->length = G2_SERVER_pump_message_serialize(packet->buffer, message);
    packet->parsed = message;
    packet->cmd_status = 0xd0;
    packet->cmd = G2_SERVER_MESSAGE_PUMP;
    packet->code = G2_SERVER_MESSAGE_OPERATION_READ;
    return G2_SERVER_PACKET_write(port, packet);
}

WINAPI_EXPORT int WINAPI G2_SERVER_send_write_pump_message(g2_server_port_pt port, g2_server_packet_pt packet)
{
    packet->length = 0;
    packet->parsed = NULL;
    packet->cmd_status = 0xd0;
    packet->cmd = G2_SERVER_MESSAGE_PUMP;
    packet->code = G2_SERVER_MESSAGE_OPERATION_WRITE;
    return G2_SERVER_PACKET_write(port, packet);
}

WINAPI_EXPORT int WINAPI G2_SERVER_send_write_restart_message(g2_server_port_pt port, g2_server_packet_pt packet)
{
    packet->length = 0;
    packet->parsed = NULL;
    packet->cmd_status = 0xd0;
    packet->cmd = G2_SERVER_MESSAGE_RESTART;
    packet->code = G2_SERVER_MESSAGE_OPERATION_WRITE;
    return G2_SERVER_PACKET_write(port, packet);
}

WINAPI_EXPORT int WINAPI G2_SERVER_send_read_gprs_signal_message(g2_server_port_pt port, g2_server_packet_pt packet, g2_server_gprs_signal_message_pt message)
{
    packet->length = G2_SERVER_gprs_signal_message_serialize(packet->buffer, message);
    packet->parsed = message;
    packet->cmd_status = 0xd0;
    packet->cmd = G2_SERVER_MESSAGE_GPRS_SIGNAL;
    packet->code = G2_SERVER_MESSAGE_OPERATION_READ;
    return G2_SERVER_PACKET_write(port, packet);
}

WINAPI_EXPORT int WINAPI G2_SERVER_send_read_heartbeat_message(g2_server_port_pt port, g2_server_packet_pt packet)
{
    packet->length = 0;
    packet->parsed = NULL;
    packet->cmd_status = 0xd0;
    packet->cmd = G2_SERVER_MESSAGE_HEARTBEAT;
    packet->code = G2_SERVER_MESSAGE_OPERATION_READ;
    return G2_SERVER_PACKET_write(port, packet);
}

WINAPI_EXPORT int WINAPI G2_SERVER_send_read_get_time_message(g2_server_port_pt port, g2_server_packet_pt packet)
{
    packet->length = 0;
    packet->parsed = NULL;
    packet->cmd_status = 0xd0;
    packet->cmd = G2_SERVER_MESSAGE_GET_TIME;
    packet->code = G2_SERVER_MESSAGE_OPERATION_READ;
    return G2_SERVER_PACKET_write(port, packet);
}

WINAPI_EXPORT int WINAPI G2_SERVER_send_write_l6_start_message(g2_server_port_pt port, g2_server_packet_pt packet)
{
    packet->length = 0;
    packet->parsed = NULL;
    packet->cmd_status = 0xd0;
    packet->cmd = G2_SERVER_MESSAGE_L6_START;
    packet->code = G2_SERVER_MESSAGE_OPERATION_WRITE;
    return G2_SERVER_PACKET_write(port, packet);
}

WINAPI_EXPORT int WINAPI G2_SERVER_send_write_l6_settings_message(g2_server_port_pt port, g2_server_packet_pt packet)
{
    packet->length = 0;
    packet->parsed = NULL;
    packet->cmd_status = 0xd0;
    packet->cmd = G2_SERVER_MESSAGE_L6_SETTINGS;
    packet->code = G2_SERVER_MESSAGE_OPERATION_WRITE;
    return G2_SERVER_PACKET_write(port, packet);
}

WINAPI_EXPORT int WINAPI G2_SERVER_send_read_l6_settings_message(g2_server_port_pt port, g2_server_packet_pt packet, g2_server_l6_settings_message_pt message)
{
    packet->length = G2_SERVER_l6_settings_message_serialize(packet->buffer, message);
    packet->parsed = message;
    packet->cmd_status = 0xd0;
    packet->cmd = G2_SERVER_MESSAGE_L6_SETTINGS;
    packet->code = G2_SERVER_MESSAGE_OPERATION_READ;
    return G2_SERVER_PACKET_write(port, packet);
}

WINAPI_EXPORT int WINAPI G2_SERVER_send_read_l6_status_message(g2_server_port_pt port, g2_server_packet_pt packet, g2_server_l6_status_message_pt message)
{
    packet->length = G2_SERVER_l6_status_message_serialize(packet->buffer, message);
    packet->parsed = message;
    packet->cmd_status = 0xd0;
    packet->cmd = G2_SERVER_MESSAGE_L6_STATUS;
    packet->code = G2_SERVER_MESSAGE_OPERATION_READ;
    return G2_SERVER_PACKET_write(port, packet);
}

WINAPI_EXPORT int WINAPI G2_SERVER_send_read_environment_message(g2_server_port_pt port, g2_server_packet_pt packet, g2_server_environment_message_pt message)
{
    packet->length = G2_SERVER_environment_message_serialize(packet->buffer, message);
    packet->parsed = message;
    packet->cmd_status = 0xd0;
    packet->cmd = G2_SERVER_MESSAGE_ENVIRONMENT;
    packet->code = G2_SERVER_MESSAGE_OPERATION_READ;
    return G2_SERVER_PACKET_write(port, packet);
}

/* 固件升级相关 */
WINAPI_EXPORT int WINAPI G2_SERVER_send_write_firmware_data_ask_message(g2_server_port_pt port, g2_server_packet_pt packet, g2_server_firmware_data_ask_pt message)
{
    uint16_t len = 0;

    len = G2_SERVER_send_firmware_data_ask_message_serialize(packet->buffer, message);
    setPacketLen(packet, len);
    packet->parsed = NULL;
    packet->cmd_status = 0xd0;
    packet->cmd = G2_SERVER_MESSAGE_FIREWARE_DATA;
    packet->code = G2_SERVER_MESSAGE_OPERATION_WRITE;

    return G2_SERVER_PACKET_write(port, packet);
}


WINAPI_EXPORT int WINAPI G2_SERVER_send_write_firmware_update_ack_message(g2_server_port_pt port, g2_server_packet_pt packet, g2_server_firmware_update_ack_pt message)
{
    uint16_t len = 0;

    len = G2_SERVER_send_firmware_update_ack_message_serialize(packet->buffer, message);
    setPacketLen(packet, len);
    packet->parsed = NULL;
    packet->cmd_status = 0xd0;
    packet->cmd = G2_SERVER_MESSAGE_FIREWARE_STOP;
    packet->code = G2_SERVER_MESSAGE_OPERATION_READ;

    return G2_SERVER_PACKET_write(port, packet);
}


WINAPI_EXPORT int WINAPI G2_SERVER_send_read_self_diagnosis_message(g2_server_port_pt port, g2_server_packet_pt packet, g2_server_self_diagnosis_message_pt message)
{
    packet->length = G2_SERVER_self_diagnosis_message_serialize(packet->buffer, message);
    packet->parsed = message;
    packet->cmd_status = 0xd0;
    packet->cmd = G2_SERVER_MESSAGE_SELF_DIAGNOSIS;
    packet->code = G2_SERVER_MESSAGE_OPERATION_READ;
    return G2_SERVER_PACKET_write(port, packet);
}


WINAPI_EXPORT int WINAPI G2_SERVER_send_read_device_sensors_message(g2_server_port_pt port, g2_server_packet_pt packet, g2_server_device_sensors_message_pt message)
{
    uint16_t len = 0;

    len = G2_SERVER_device_sensors_message_serialize(packet->buffer, message);
    setPacketLen(packet, len);
    packet->parsed = NULL;
    packet->cmd_status = 0xd0;
    packet->cmd = G2_SERVER_MESSAGE_DEVICE_SENSORS;
    packet->code = G2_SERVER_MESSAGE_OPERATION_READ;

    return G2_SERVER_PACKET_write(port, packet);
}

WINAPI_EXPORT int WINAPI G2_SERVER_send_read_iccid_message(g2_server_port_pt port, g2_server_packet_pt packet, g2_server_sim_iccid_message_pt message)
{
    packet->length = G2_SERVER_iccid_message_serialize(packet->buffer, message);
    packet->parsed = message;
    packet->cmd_status = 0xd0;
    packet->cmd = G2_SERVER_MESSAGE_READ_ICCID;
    packet->code = G2_SERVER_MESSAGE_OPERATION_READ;
    return G2_SERVER_PACKET_write(port, packet);
}


WINAPI_EXPORT int WINAPI G2_SERVER_send_write_relay_power_config_message(g2_server_port_pt port, g2_server_packet_pt packet)
{
    packet->length = 0;
    packet->parsed = NULL;
    packet->cmd_status = 0xd0;
    packet->cmd = G2_SERVER_MESSAGE_RELAY_POWER_CONFIG;
    packet->code = G2_SERVER_MESSAGE_OPERATION_WRITE;
    return G2_SERVER_PACKET_write(port, packet);
}

WINAPI_EXPORT int WINAPI G2_SERVER_send_read_relay_power_config_message(g2_server_port_pt port, g2_server_packet_pt packet, g2_server_relay_power_config_message_pt message)
{
    packet->length = G2_SERVER_relay_power_config_message_serialize(packet->buffer, message);
    packet->parsed = NULL;
    packet->cmd_status = 0xd0;
    packet->cmd = G2_SERVER_MESSAGE_RELAY_POWER_CONFIG;
    packet->code = G2_SERVER_MESSAGE_OPERATION_READ;
    return G2_SERVER_PACKET_write(port, packet);
}



WINAPI_EXPORT int WINAPI G2_SERVER_send_write_loader_config_message(g2_server_port_pt port, g2_server_packet_pt packet)
{
    packet->length = 0;
    packet->parsed = NULL;
    packet->cmd_status = 0xd0;
    packet->cmd = G2_SERVER_MESSAGE_LOADER_COONFIG;
    packet->code = G2_SERVER_MESSAGE_OPERATION_WRITE;
    return G2_SERVER_PACKET_write(port, packet);
}

WINAPI_EXPORT int WINAPI G2_SERVER_send_read_loader_config_message(g2_server_port_pt port, g2_server_packet_pt packet, g2_server_loader_config_message_pt message)
{
    packet->length = G2_SERVER_loader_config_message_serialize(packet->buffer, message);
    packet->parsed = NULL;
    packet->cmd_status = 0xd0;
    packet->cmd = G2_SERVER_MESSAGE_LOADER_COONFIG;
    packet->code = G2_SERVER_MESSAGE_OPERATION_READ;
    return G2_SERVER_PACKET_write(port, packet);
}


WINAPI_EXPORT int WINAPI G2_SERVER_send_read_device_warnning_message(g2_server_port_pt port, g2_server_packet_pt packet, g2_server_device_warnning_message_pt message)
{
    packet->length = G2_SERVER_device_warnning_message_serialize(packet->buffer, message);
    packet->parsed = NULL;
    packet->cmd_status = 0xd0;
    packet->cmd = G2_SERVER_MESSAGE_DEVICE_WARNNING;
    packet->code = G2_SERVER_MESSAGE_OPERATION_READ;
    return G2_SERVER_PACKET_write(port, packet);
}

#define G2_SERVER_MESSAGES_PROCESS(_id, oper_name, oper_name_upcase,_name) \
__weak int G2_SERVER##oper_name##_##_name##_message_process(g2_server_packet_pt packet) \
{ \
    return PROTOCOL_OK;\
}

G2_SERVER_MESSAGES_MAP(G2_SERVER_MESSAGES_PROCESS)
