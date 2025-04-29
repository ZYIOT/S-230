int G2_SERVER_read_device_id_message_process(g2_server_packet_pt packet)
{
    g2_server_device_id_message_t pmsg = {0};
    pmsg.deviceID = APP_CONFIG_DeviceID();
    BSP_PROTOCOL_send_read_device_id_message(packet, &pmsg);
    return PROTOCOL_OK;
}

int G2_SERVER_write_device_id_message_process(g2_server_packet_pt packet)
{
    CHECK_PROTOCOL_MESSAGE
    g2_server_device_id_message_pt pmsg = (g2_server_device_id_message_pt)packet->parsed;
    if (pmsg->deviceID < 1 || pmsg->deviceID >= 10000000)
    {
        BSP_PROTOCOL_send_error_response(packet);
        return PROTOCOL_ERROR;
    }
    int rc = APP_CONFIG_SystemWriteDeviceID(&g_appConfigSystem, pmsg->deviceID);
    CHECK_CONFIG_MESSAGE_RC(rc)
    BSP_PROTOCOL_send_ok_response(packet);
    return PROTOCOL_OK;
}

int G2_SERVER_read_version_message_process(g2_server_packet_pt packet)
{
    g2_server_device_version_message_t pmsg = {0};
    memcpy(pmsg.hardware, g_appConfigSystem.hardware, 3);
    pmsg.firmware[0] = APP_FIRMWARE_MAJOR;
    pmsg.firmware[1] = APP_FIRMWARE_MINOR;
    pmsg.firmware[2] = APP_FIRMWARE_REVISION;
    strncpy((char *)pmsg.PN, PN_CODE, 10);
    uint8_t sn[11] = {0};
    write_uint16_t_BE(SN_CODE, sn);
    write_uint32_t_BE(APP_CONFIG_DeviceID(), &sn[2]);
    memcpy(&(sn[6]), g_appConfigSystem.SN, 5);
    memcpy(pmsg.SN, sn, 11);
    BSP_PROTOCOL_send_read_version_message(packet, &pmsg);
    return PROTOCOL_OK;
}

int G2_SERVER_write_version_message_process(g2_server_packet_pt packet)
{
    CHECK_PROTOCOL_MESSAGE
    g2_server_device_version_message_pt pmsg = (g2_server_device_version_message_pt)packet->parsed;
    int rc = APP_CONFIG_SystemWriteSN(&g_appConfigSystem, &(pmsg->SN[6]), pmsg->hardware);
    CHECK_CONFIG_MESSAGE_RC(rc)
    BSP_PROTOCOL_send_ok_response(packet);
    return PROTOCOL_OK;
}

int G2_SERVER_write_time_message_process(g2_server_packet_pt packet)
{
    CHECK_PROTOCOL_MESSAGE
    g2_server_time_message_pt pmsg = (g2_server_time_message_pt)packet->parsed;
    int rc = check_datetime1(pmsg->year, pmsg->month, pmsg->day, pmsg->hour, pmsg->minute, pmsg->second, 0);
    CHECK_CONFIG_MESSAGE_RC(rc)
    rc = BSP_RTC_set(pmsg->year, pmsg->month, pmsg->day, pmsg->hour, pmsg->minute, pmsg->second);
    CHECK_CONFIG_MESSAGE_RC(rc)
    BSP_PROTOCOL_send_ok_response(packet);
    return PROTOCOL_OK;
}

int G2_SERVER_read_time_message_process(g2_server_packet_pt packet)
{
    g2_server_time_message_t message = {0};
    G2_SERVER_time_message_init(&message);
    int rc = BSP_RTC_get(&message.year, &message.month, &message.day, &message.hour, &message.minute, &message.second, &message.week);
    CHECK_CONFIG_MESSAGE_RC(rc)
    BSP_PROTOCOL_send_read_time_message(packet, &message);
    return PROTOCOL_OK;
}

int G2_SERVER_read_gprs_signal_message_process(g2_server_packet_pt packet)
{
    g2_server_gprs_signal_message_t pmsg = {0};
    pmsg.rssi = g_appNetwork.csq;
    BSP_PROTOCOL_send_read_gprs_signal_message(packet, &pmsg);
    return PROTOCOL_OK;
}

#include "bsp_mcu.h"
#include "bsp_power_key.h"

int G2_SERVER_write_restart_message_process(g2_server_packet_pt packet)
{
    BSP_PROTOCOL_send_ok_response(packet);
    g_appConfigDeviceConnect.reason = 3;
    APP_CONFIG_DeviceConnectWrite(&g_appConfigDeviceConnect);
    HARDWARE_OS_DELAY_MS(500);
#ifdef SUPPORT_POWER_OFF_SENSOR
    POWER_SENSOR_OFF();
#endif
#ifdef SUPPORT_POWER_OFF_DTU
    POWER_GPRS_OFF();
#endif
#ifdef BOARD_HAS_RELAY
    APP_CAN_reset_relay();
#endif
#if defined(SUPPORT_POWER_OFF_SENSOR) || defined(SUPPORT_POWER_OFF_DTU)
    HARDWARE_HAL_DELAY_MS(5000); 
#endif
    BSP_MCU_reset();
    return PROTOCOL_OK;
}

int G2_SERVER_write_recovery_message_process(g2_server_packet_pt packet)
{
    int rc = APP_CONFIG_EepromRecoveryAll();
    CHECK_CONFIG_MESSAGE_RC(rc)
    BSP_PROTOCOL_send_ok_response(packet);
    g_appConfigDeviceConnect.reason = 4;
    APP_CONFIG_DeviceConnectWrite(&g_appConfigDeviceConnect);
    HARDWARE_OS_DELAY_MS(3000);
#ifdef BOARD_HAS_RELAY
    APP_CAN_reset_relay();
    HARDWARE_OS_DELAY_MS(100);
#endif
    BSP_MCU_reset();
    return PROTOCOL_OK;
}

#ifdef BOARD_HAS_RELAY
int G2_SERVER_write_device_warnning_message_process(g2_server_packet_pt packet)
{
    G2_SERVER_PACKET_init(packet);
    g2_server_device_warnning_message_t message;
    G2_SERVER_device_warnning_message_init(&message);
    APP_RELAY_read_device_warnning(&message);
    BSP_PROTOCOL_send_read_device_warnning_message(packet, &message);
    return PROTOCOL_OK;
}
#endif
