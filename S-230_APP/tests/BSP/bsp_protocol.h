#ifndef __BSP_PROTOCOL_H
#define __BSP_PROTOCOL_H
#ifdef __cplusplus
extern "C"
{
#endif
#include <stdio.h>
#include <stdint.h>

#define BSP_PROTOCOL_GPRS_SUCCESS_ACK 0xD0
#define BSP_PROTOCOL_GPRS_ERROR_ACK 0xD1
#define BSP_PROTOCOL_GPRS_ERROR_CMD 0xD2

    extern g2_server_port_t g2_server_port;
    int BSP_PROTOCOL_init(void);
    int BSP_PROTOCOL_send_response(g2_server_packet_pt packet, uint8_t cmd_status);
    int BSP_PROTOCOL_send_rc_response(g2_server_packet_pt packet, int rc);
    int BSP_PROTOCOL_send_ok_response(g2_server_packet_pt packet);
    int BSP_PROTOCOL_send_error_response(g2_server_packet_pt packet);

#define BSP_PROTOCOL_PACKET_read(packet) G2_SERVER_PACKET_read(&g2_server_port, packet)
#define BSP_PROTOCOL_PACKET_write(packet) G2_SERVER_PACKET_write(&g2_server_port, packet)
#define BSP_PROTOCOL_receive_packet(packet) G2_SERVER_receive_packet(packet)

#define BSP_PROTOCOL_send_read_device_id_message(packet, message) G2_SERVER_send_read_device_id_message(&g2_server_port, packet, message)
#define BSP_PROTOCOL_send_write_device_id_message(packet) G2_SERVER_send_write_device_id_message(&g2_server_port, packet)
#define BSP_PROTOCOL_send_read_version_message(packet, message) G2_SERVER_send_read_version_message(&g2_server_port, packet, message)
#define BSP_PROTOCOL_send_write_version_message(packet) G2_SERVER_send_write_version_message(&g2_server_port, packet)
#define BSP_PROTOCOL_send_read_sensor_config_message(packet, message) G2_SERVER_send_read_sensor_config_message(&g2_server_port, packet, message)
#define BSP_PROTOCOL_send_write_sensor_config_message(packet) G2_SERVER_send_write_sensor_config_message(&g2_server_port, packet)
#define BSP_PROTOCOL_send_delete_sensor_config_message(packet) G2_SERVER_send_delete_sensor_config_message(&g2_server_port, packet)
#define BSP_PROTOCOL_send_read_sensor_limit_message(packet, message) G2_SERVER_send_read_sensor_limit_message(&g2_server_port, packet, message)
#define BSP_PROTOCOL_send_write_sensor_limit_message(packet) G2_SERVER_send_write_sensor_limit_message(&g2_server_port, packet)
#define BSP_PROTOCOL_send_delete_sensor_limit_message(packet) G2_SERVER_send_delete_sensor_limit_message(&g2_server_port, packet)
#define BSP_PROTOCOL_send_read_sensor_alert_message(packet, message) G2_SERVER_send_read_sensor_alert_message(&g2_server_port, packet, message)
#define BSP_PROTOCOL_send_write_sensor_alert_message(packet) G2_SERVER_send_write_sensor_alert_message(&g2_server_port, packet)
#define BSP_PROTOCOL_send_delete_sensor_alert_message(packet) G2_SERVER_send_delete_sensor_alert_message(&g2_server_port, packet)
#define BSP_PROTOCOL_send_read_relay_jobs_message(packet, message) G2_SERVER_send_read_relay_jobs_message(&g2_server_port, packet, message)
#define BSP_PROTOCOL_send_write_relay_jobs_message(packet) G2_SERVER_send_write_relay_jobs_message(&g2_server_port, packet)
#define BSP_PROTOCOL_send_delete_relay_jobs_message(packet) G2_SERVER_send_delete_relay_jobs_message(&g2_server_port, packet)
#define BSP_PROTOCOL_send_read_salinity_message(packet, message) G2_SERVER_send_read_salinity_message(&g2_server_port, packet, message)
#define BSP_PROTOCOL_send_write_salinity_message(packet) G2_SERVER_send_write_salinity_message(&g2_server_port, packet)
#define BSP_PROTOCOL_send_write_calibrate_message(packet) G2_SERVER_send_write_calibrate_message(&g2_server_port, packet)
#define BSP_PROTOCOL_send_write_time_message(packet) G2_SERVER_send_write_time_message(&g2_server_port, packet)
#define BSP_PROTOCOL_send_read_time_message(packet, message) G2_SERVER_send_read_time_message(&g2_server_port, packet, message)
#define BSP_PROTOCOL_send_write_relay_manual_message(packet) G2_SERVER_send_write_relay_manual_message(&g2_server_port, packet)
#define BSP_PROTOCOL_send_read_relay_manual_enable_message(packet, message) G2_SERVER_send_read_relay_manual_enable_message(&g2_server_port, packet, message)
#define BSP_PROTOCOL_send_write_relay_manual_enable_message(packet) G2_SERVER_send_write_relay_manual_enable_message(&g2_server_port, packet)
#define BSP_PROTOCOL_send_read_system_message(packet, message) G2_SERVER_send_read_system_message(&g2_server_port, packet, message)
#define BSP_PROTOCOL_send_write_system_message(packet) G2_SERVER_send_write_system_message(&g2_server_port, packet)
#define BSP_PROTOCOL_send_read_connect_message(packet, message) G2_SERVER_send_read_connect_message(&g2_server_port, packet, message)
#define BSP_PROTOCOL_send_read_sensor_limit_enable_message(packet, message) G2_SERVER_send_read_sensor_limit_enable_message(&g2_server_port, packet, message)
#define BSP_PROTOCOL_send_write_sensor_limit_enable_message(packet) G2_SERVER_send_write_sensor_limit_enable_message(&g2_server_port, packet)
#define BSP_PROTOCOL_send_read_device_enable_message(packet, message) G2_SERVER_send_read_device_enable_message(&g2_server_port, packet, message)
#define BSP_PROTOCOL_send_write_device_enable_message(packet) G2_SERVER_send_write_device_enable_message(&g2_server_port, packet)
#define BSP_PROTOCOL_send_read_device_capability_message(packet, message) G2_SERVER_send_read_device_capability_message(&g2_server_port, packet, message)
#define BSP_PROTOCOL_send_write_device_capability_message(packet) G2_SERVER_send_write_device_capability_message(&g2_server_port, packet)
#define BSP_PROTOCOL_send_read_relay_data_message(packet, message) G2_SERVER_send_read_relay_data_message(&g2_server_port, packet, message)
#define BSP_PROTOCOL_send_read_sensor_data_message(packet, message) G2_SERVER_send_read_sensor_data_message(&g2_server_port, packet, message)
#define BSP_PROTOCOL_send_write_recovery_message(packet) G2_SERVER_send_write_recovery_message(&g2_server_port, packet)
#define BSP_PROTOCOL_send_read_device_used_capability_message(packet, message) G2_SERVER_send_read_device_used_capability_message(&g2_server_port, packet, message)
#define BSP_PROTOCOL_send_write_device_used_capability_message(packet) G2_SERVER_send_write_device_used_capability_message(&g2_server_port, packet)
#define BSP_PROTOCOL_send_read_relay_config_message(packet, message) G2_SERVER_send_read_relay_config_message(&g2_server_port, packet, message)
#define BSP_PROTOCOL_send_write_relay_config_message(packet) G2_SERVER_send_write_relay_config_message(&g2_server_port, packet)
#define BSP_PROTOCOL_send_delete_relay_config_message(packet) G2_SERVER_send_delete_relay_config_message(&g2_server_port, packet)
#define BSP_PROTOCOL_send_read_relay_jobs_enable_message(packet, message) G2_SERVER_send_read_relay_jobs_enable_message(&g2_server_port, packet, message)
#define BSP_PROTOCOL_send_write_relay_jobs_enable_message(packet) G2_SERVER_send_write_relay_jobs_enable_message(&g2_server_port, packet)
#define BSP_PROTOCOL_send_read_pump_message(packet, message) G2_SERVER_send_read_pump_message(&g2_server_port, packet, message)
#define BSP_PROTOCOL_send_write_pump_message(packet) G2_SERVER_send_write_pump_message(&g2_server_port, packet)
#define BSP_PROTOCOL_send_write_restart_message(packet) G2_SERVER_send_write_restart_message(&g2_server_port, packet)
#define BSP_PROTOCOL_send_read_gprs_signal_message(packet, message) G2_SERVER_send_read_gprs_signal_message(&g2_server_port, packet, message)
#define BSP_PROTOCOL_send_read_heartbeat_message(packet) G2_SERVER_send_read_heartbeat_message(&g2_server_port, packet)
#define BSP_PROTOCOL_send_read_get_time_message(packet) G2_SERVER_send_read_get_time_message(&g2_server_port, packet)
#define BSP_PROTOCOL_send_write_l6_start_message(packet) G2_SERVER_send_write_l6_start_message(&g2_server_port, packet)
#define BSP_PROTOCOL_send_write_l6_settings_message(packet) G2_SERVER_send_write_l6_settings_message(&g2_server_port, packet)
#define BSP_PROTOCOL_send_read_l6_settings_message(packet, message) G2_SERVER_send_read_l6_settings_message(&g2_server_port, packet, message)
#define BSP_PROTOCOL_send_read_l6_status_message(packet, message) G2_SERVER_send_read_l6_status_message(&g2_server_port, packet, message)
#define BSP_PROTOCOL_send_read_environment_message(packet, message) G2_SERVER_send_read_environment_message(&g2_server_port, packet, message)

#ifdef __cplusplus
}
#endif
#endif
