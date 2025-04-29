#ifndef __G2_SERVER_SLAVE_H
#define __G2_SERVER_SLAVE_H
#ifdef __cplusplus
extern "C"
{
#endif
#include "g2_server_common.h"
#include "g2_server_packet.h"
#include "g2_server_message.h"
#include "app_update_firmware.h"

WINAPI_EXPORT int WINAPI G2_SERVER_receive_packet(g2_server_packet_pt packet);
WINAPI_EXPORT int WINAPI G2_SERVER_send_read_device_id_message(g2_server_port_pt port, g2_server_packet_pt packet, g2_server_device_id_message_pt message);
WINAPI_EXPORT int WINAPI G2_SERVER_send_write_device_id_message(g2_server_port_pt port, g2_server_packet_pt packet);
WINAPI_EXPORT int WINAPI G2_SERVER_send_read_version_message(g2_server_port_pt port, g2_server_packet_pt packet, g2_server_device_version_message_pt message);
WINAPI_EXPORT int WINAPI G2_SERVER_send_write_version_message(g2_server_port_pt port, g2_server_packet_pt packet);
WINAPI_EXPORT int WINAPI G2_SERVER_send_read_sensor_config_message(g2_server_port_pt port, g2_server_packet_pt packet, g2_server_sensor_config_message_pt message);
WINAPI_EXPORT int WINAPI G2_SERVER_send_write_sensor_config_message(g2_server_port_pt port, g2_server_packet_pt packet);
WINAPI_EXPORT int WINAPI G2_SERVER_send_delete_sensor_config_message(g2_server_port_pt port, g2_server_packet_pt packet);
WINAPI_EXPORT int WINAPI G2_SERVER_send_read_sensor_limit_message(g2_server_port_pt port, g2_server_packet_pt packet, g2_server_sensor_limit_message_pt message);
WINAPI_EXPORT int WINAPI G2_SERVER_send_write_sensor_limit_message(g2_server_port_pt port, g2_server_packet_pt packet);
WINAPI_EXPORT int WINAPI G2_SERVER_send_delete_sensor_limit_message(g2_server_port_pt port, g2_server_packet_pt packet);
WINAPI_EXPORT int WINAPI G2_SERVER_send_read_sensor_alert_message(g2_server_port_pt port, g2_server_packet_pt packet, g2_server_sensor_alert_message_pt message);
WINAPI_EXPORT int WINAPI G2_SERVER_send_write_sensor_alert_message(g2_server_port_pt port, g2_server_packet_pt packet);
WINAPI_EXPORT int WINAPI G2_SERVER_send_delete_sensor_alert_message(g2_server_port_pt port, g2_server_packet_pt packet);
WINAPI_EXPORT int WINAPI G2_SERVER_send_read_relay_jobs_message(g2_server_port_pt port, g2_server_packet_pt packet, g2_server_relay_jobs_message_pt message);
WINAPI_EXPORT int WINAPI G2_SERVER_send_write_relay_jobs_message(g2_server_port_pt port, g2_server_packet_pt packet);
WINAPI_EXPORT int WINAPI G2_SERVER_send_delete_relay_jobs_message(g2_server_port_pt port, g2_server_packet_pt packet);
WINAPI_EXPORT int WINAPI G2_SERVER_send_read_salinity_message(g2_server_port_pt port, g2_server_packet_pt packet, g2_server_salinity_message_pt message);
WINAPI_EXPORT int WINAPI G2_SERVER_send_write_salinity_message(g2_server_port_pt port, g2_server_packet_pt packet);
WINAPI_EXPORT int WINAPI G2_SERVER_send_write_calibrate_message(g2_server_port_pt port, g2_server_packet_pt packet);
WINAPI_EXPORT int WINAPI G2_SERVER_send_write_time_message(g2_server_port_pt port, g2_server_packet_pt packet);
WINAPI_EXPORT int WINAPI G2_SERVER_send_read_time_message(g2_server_port_pt port, g2_server_packet_pt packet, g2_server_time_message_pt message);
WINAPI_EXPORT int WINAPI G2_SERVER_send_write_relay_manual_message(g2_server_port_pt port, g2_server_packet_pt packet);
WINAPI_EXPORT int WINAPI G2_SERVER_send_read_relay_manual_enable_message(g2_server_port_pt port, g2_server_packet_pt packet, g2_server_relay_manual_enable_message_pt message);
WINAPI_EXPORT int WINAPI G2_SERVER_send_write_relay_manual_enable_message(g2_server_port_pt port, g2_server_packet_pt packet);
WINAPI_EXPORT int WINAPI G2_SERVER_send_read_system_message(g2_server_port_pt port, g2_server_packet_pt packet, g2_server_system_message_pt message);
WINAPI_EXPORT int WINAPI G2_SERVER_send_write_system_message(g2_server_port_pt port, g2_server_packet_pt packet);
WINAPI_EXPORT int WINAPI G2_SERVER_send_read_connect_message(g2_server_port_pt port, g2_server_packet_pt packet, g2_server_connect_message_pt message);
WINAPI_EXPORT int WINAPI G2_SERVER_send_read_sensor_limit_enable_message(g2_server_port_pt port, g2_server_packet_pt packet, g2_server_sensor_limit_enable_message_pt message);
WINAPI_EXPORT int WINAPI G2_SERVER_send_write_sensor_limit_enable_message(g2_server_port_pt port, g2_server_packet_pt packet);
WINAPI_EXPORT int WINAPI G2_SERVER_send_read_device_enable_message(g2_server_port_pt port, g2_server_packet_pt packet, g2_server_device_enable_message_pt message);
WINAPI_EXPORT int WINAPI G2_SERVER_send_write_device_enable_message(g2_server_port_pt port, g2_server_packet_pt packet);
WINAPI_EXPORT int WINAPI G2_SERVER_send_read_device_capability_message(g2_server_port_pt port, g2_server_packet_pt packet, g2_server_device_capability_message_pt message);
WINAPI_EXPORT int WINAPI G2_SERVER_send_write_device_capability_message(g2_server_port_pt port, g2_server_packet_pt packet);
WINAPI_EXPORT int WINAPI G2_SERVER_send_read_relay_data_message(g2_server_port_pt port, g2_server_packet_pt packet, g2_server_relay_data_message_pt message);
WINAPI_EXPORT int WINAPI G2_SERVER_send_read_sensor_data_message(g2_server_port_pt port, g2_server_packet_pt packet, g2_server_sensor_data_message_pt message);
WINAPI_EXPORT int WINAPI G2_SERVER_send_write_recovery_message(g2_server_port_pt port, g2_server_packet_pt packet);
WINAPI_EXPORT int WINAPI G2_SERVER_send_read_device_used_capability_message(g2_server_port_pt port, g2_server_packet_pt packet, g2_server_device_used_capability_message_pt message);
WINAPI_EXPORT int WINAPI G2_SERVER_send_write_device_used_capability_message(g2_server_port_pt port, g2_server_packet_pt packet);
WINAPI_EXPORT int WINAPI G2_SERVER_send_read_relay_config_message(g2_server_port_pt port, g2_server_packet_pt packet, g2_server_relay_config_message_pt message);
WINAPI_EXPORT int WINAPI G2_SERVER_send_write_relay_config_message(g2_server_port_pt port, g2_server_packet_pt packet);
WINAPI_EXPORT int WINAPI G2_SERVER_send_delete_relay_config_message(g2_server_port_pt port, g2_server_packet_pt packet);
WINAPI_EXPORT int WINAPI G2_SERVER_send_read_relay_jobs_enable_message(g2_server_port_pt port, g2_server_packet_pt packet, g2_server_relay_jobs_enable_message_pt message);
WINAPI_EXPORT int WINAPI G2_SERVER_send_write_relay_jobs_enable_message(g2_server_port_pt port, g2_server_packet_pt packet);
WINAPI_EXPORT int WINAPI G2_SERVER_send_read_pump_message(g2_server_port_pt port, g2_server_packet_pt packet, g2_server_pump_message_pt message);
WINAPI_EXPORT int WINAPI G2_SERVER_send_write_pump_message(g2_server_port_pt port, g2_server_packet_pt packet);
WINAPI_EXPORT int WINAPI G2_SERVER_send_write_restart_message(g2_server_port_pt port, g2_server_packet_pt packet);
WINAPI_EXPORT int WINAPI G2_SERVER_send_read_gprs_signal_message(g2_server_port_pt port, g2_server_packet_pt packet, g2_server_gprs_signal_message_pt message);
WINAPI_EXPORT int WINAPI G2_SERVER_send_read_heartbeat_message(g2_server_port_pt port, g2_server_packet_pt packet);
WINAPI_EXPORT int WINAPI G2_SERVER_send_read_get_time_message(g2_server_port_pt port, g2_server_packet_pt packet);
WINAPI_EXPORT int WINAPI G2_SERVER_send_write_l6_start_message(g2_server_port_pt port, g2_server_packet_pt packet);
WINAPI_EXPORT int WINAPI G2_SERVER_send_write_l6_settings_message(g2_server_port_pt port, g2_server_packet_pt packet);
WINAPI_EXPORT int WINAPI G2_SERVER_send_read_l6_settings_message(g2_server_port_pt port, g2_server_packet_pt packet, g2_server_l6_settings_message_pt message);
WINAPI_EXPORT int WINAPI G2_SERVER_send_read_l6_status_message(g2_server_port_pt port, g2_server_packet_pt packet, g2_server_l6_status_message_pt message);
WINAPI_EXPORT int WINAPI G2_SERVER_send_read_environment_message(g2_server_port_pt port, g2_server_packet_pt packet, g2_server_environment_message_pt message);
WINAPI_EXPORT int WINAPI G2_SERVER_send_read_self_diagnosis_message(g2_server_port_pt port, g2_server_packet_pt packet, g2_server_self_diagnosis_message_pt message);
WINAPI_EXPORT int WINAPI G2_SERVER_send_write_firmware_data_ask_message(g2_server_port_pt port, g2_server_packet_pt packet, g2_server_firmware_data_ask_pt message);
WINAPI_EXPORT int WINAPI G2_SERVER_send_write_firmware_update_ack_message(g2_server_port_pt port, g2_server_packet_pt packet, g2_server_firmware_update_ack_pt message);
WINAPI_EXPORT int WINAPI G2_SERVER_send_read_device_sensors_message(g2_server_port_pt port, g2_server_packet_pt packet, g2_server_device_sensors_message_pt message);
WINAPI_EXPORT int WINAPI G2_SERVER_send_write_relay_power_config_message(g2_server_port_pt port, g2_server_packet_pt packet);


#define G2_SERVER_MESSAGES_MAP(F) \
    F(G2_SERVER_MESSAGE_DEVICE_ID, _read, _READ, deviceID) \
    F(G2_SERVER_MESSAGE_DEVICE_ID, _write, _WRITE, deviceID) \
    F(G2_SERVER_MESSAGE_VERSION, _read, _READ, version) \
    F(G2_SERVER_MESSAGE_VERSION, _write, _WRITE, version) \
    F(G2_SERVER_MESSAGE_SENSOR_CONFIG, _read, _READ, sensor_config) \
    F(G2_SERVER_MESSAGE_SENSOR_CONFIG, _write, _WRITE, sensor_config) \
    F(G2_SERVER_MESSAGE_SENSOR_CONFIG, _delete, _DELETE, sensor_config) \
    F(G2_SERVER_MESSAGE_SENSOR_LIMIT, _read, _READ, sensor_limit) \
    F(G2_SERVER_MESSAGE_SENSOR_LIMIT, _write, _WRITE, sensor_limit) \
    F(G2_SERVER_MESSAGE_SENSOR_LIMIT, _delete, _DELETE, sensor_limit) \
    F(G2_SERVER_MESSAGE_SENSOR_ALERT, _read, _READ, sensor_alert) \
    F(G2_SERVER_MESSAGE_SENSOR_ALERT, _write, _WRITE, sensor_alert) \
    F(G2_SERVER_MESSAGE_SENSOR_ALERT, _delete, _DELETE, sensor_alert) \
    F(G2_SERVER_MESSAGE_RELAY_JOBS, _read, _READ, relay_jobs) \
    F(G2_SERVER_MESSAGE_RELAY_JOBS, _write, _WRITE, relay_jobs) \
    F(G2_SERVER_MESSAGE_RELAY_JOBS, _delete, _DELETE, relay_jobs) \
    F(G2_SERVER_MESSAGE_SALINITY, _read, _READ, salinity) \
    F(G2_SERVER_MESSAGE_SALINITY, _write, _WRITE, salinity) \
    F(G2_SERVER_MESSAGE_CALIBRATE, _write, _WRITE, calibrate) \
    F(G2_SERVER_MESSAGE_TIME, _write, _WRITE, time) \
    F(G2_SERVER_MESSAGE_TIME, _read, _READ, time) \
    F(G2_SERVER_MESSAGE_RELAY_MANUAL, _write, _WRITE, relay_manual) \
    F(G2_SERVER_MESSAGE_RELAY_MANUAL_ENABLE, _read, _READ, relay_manual_enable) \
    F(G2_SERVER_MESSAGE_RELAY_MANUAL_ENABLE, _write, _WRITE, relay_manual_enable) \
    F(G2_SERVER_MESSAGE_SYSTEM, _read, _READ, system) \
    F(G2_SERVER_MESSAGE_SYSTEM, _write, _WRITE, system) \
    F(G2_SERVER_MESSAGE_CONNECT, _read, _READ, connect) \
    F(G2_SERVER_MESSAGE_SENSOR_LIMIT_ENABLE, _read, _READ, sensor_limit_enable) \
    F(G2_SERVER_MESSAGE_SENSOR_LIMIT_ENABLE, _write, _WRITE, sensor_limit_enable) \
    F(G2_SERVER_MESSAGE_DEVICE_ENABLE, _read, _READ, device_enable) \
    F(G2_SERVER_MESSAGE_DEVICE_ENABLE, _write, _WRITE, device_enable) \
    F(G2_SERVER_MESSAGE_DEVICE_CAPABILITY, _read, _READ, device_capability) \
    F(G2_SERVER_MESSAGE_DEVICE_CAPABILITY, _write, _WRITE, device_capability) \
    F(G2_SERVER_MESSAGE_RELAY_DATA, _read, _READ, relay_data) \
    F(G2_SERVER_MESSAGE_SENSOR_DATA, _read, _READ, sensor_data) \
    F(G2_SERVER_MESSAGE_RECOVERY, _write, _WRITE, recovery) \
    F(G2_SERVER_MESSAGE_DEVICE_USED_CAPABILITY, _read, _READ, device_used_capability) \
    F(G2_SERVER_MESSAGE_DEVICE_USED_CAPABILITY, _write, _WRITE, device_used_capability) \
    F(G2_SERVER_MESSAGE_DEVICE_SENSORS, _read, _READ, device_sensors) \
    F(G2_SERVER_MESSAGE_RELAY_CONFIG, _read, _READ, relay_config) \
    F(G2_SERVER_MESSAGE_RELAY_CONFIG, _write, _WRITE, relay_config) \
    F(G2_SERVER_MESSAGE_RELAY_CONFIG, _delete, _DELETE, relay_config) \
    F(G2_SERVER_MESSAGE_RELAY_JOBS_ENABLE, _read, _READ, relay_jobs_enable) \
    F(G2_SERVER_MESSAGE_RELAY_JOBS_ENABLE, _write, _WRITE, relay_jobs_enable) \
    F(G2_SERVER_MESSAGE_PUMP, _read, _READ, pump) \
    F(G2_SERVER_MESSAGE_PUMP, _write, _WRITE, pump) \
    F(G2_SERVER_MESSAGE_RESTART, _write, _WRITE, restart) \
    F(G2_SERVER_MESSAGE_GPRS_SIGNAL, _read, _READ, gprs_signal) \
    F(G2_SERVER_MESSAGE_HEARTBEAT, _read, _READ, heartbeat) \
    F(G2_SERVER_MESSAGE_GET_TIME, _read, _READ, get_time) \
    F(G2_SERVER_MESSAGE_L6_START, _write, _WRITE, l6_start) \
    F(G2_SERVER_MESSAGE_L6_SETTINGS, _write, _WRITE, l6_settings) \
    F(G2_SERVER_MESSAGE_L6_SETTINGS, _read, _READ, l6_settings) \
    F(G2_SERVER_MESSAGE_L6_STATUS, _read, _READ, l6_status) \
    F(G2_SERVER_MESSAGE_ENVIRONMENT, _read, _READ, environment) \
    F(G2_SERVER_MESSAGE_ENVIRONMENT, _read, _READ, self_diagnosis) \
    F(G2_SERVER_MESSAGE_FIREWARE_START, _write, _WRITE, firmware_start) \
    F(G2_SERVER_MESSAGE_FIREWARE_DATA, _write, _WRITE, firmware_data) \
    F(G2_SERVER_MESSAGE_RELAY_POWER_CONFIG, _write, _WRITE, relay_power_config) \
    F(G2_SERVER_MESSAGE_DEVICE_WARNNING, _read, _READ, device_warnning) \
    F(G2_SERVER_MESSAGE_LOADER_COONFIG, _write, _WRITE, loader_config) \
    F(G2_SERVER_MESSAGE_LOADER_COONFIG, _read, _READ, loader_config) \
  
  
#define G2_SERVER_MESSAGES_PROCESS_DECLARE(_id, oper_name, oper_name_upcase,_name) \
int G2_SERVER##oper_name##_##_name##_message_process(g2_server_packet_pt packet);

G2_SERVER_MESSAGES_MAP(G2_SERVER_MESSAGES_PROCESS_DECLARE)

#ifdef __cplusplus
}
#endif
#endif
