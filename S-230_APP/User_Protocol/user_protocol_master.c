#include "user_protocol_master.h"
#include "app_gprs.h"

static int _on_receive_write_role_message(user_protocol_packet_pt packet)
{
    uint8_t role = 0;
    int rc = USER_PROTOCOL_role_message_deserialize(packet->buffer, &role);
    USER_PROTOCOL_CHECK_READ_RC_AND_RETURN
    packet->parsed = &role;
    return USER_PROTOCOL_WriteRoleMessageProcess(packet);
}

static int _on_receive_read_sim_iccid_message(user_protocol_packet_pt packet)
{
    uint8_t sim_ccid[20] = {0};
    int rc = USER_PROTOCOL_sim_iccid_message_deserialize(packet->buffer, sim_ccid);
    USER_PROTOCOL_CHECK_READ_RC_AND_RETURN
    packet->parsed = sim_ccid;
    return USER_PROTOCOL_ReadSimIccidMessageProcess(packet);
}

static int _on_receive_read_sim_manufacturer_message(user_protocol_packet_pt packet)
{
    uint8_t manufacturer;
    int rc = USER_PROTOCOL_sim_manufacturer_message_deserialize(packet->buffer, &manufacturer);
    USER_PROTOCOL_CHECK_READ_RC_AND_RETURN
    packet->parsed = &manufacturer;
    return USER_PROTOCOL_ReadSimManufacturerMessageProcess(packet);
}

static int _on_receive_write_device_id_message(user_protocol_packet_pt packet)
{
    uint32_t deviceID = APP_CONFIG_DeviceID();
    uint8_t id[4] = {0};
    write_uint32_t_BE(deviceID, id);
    return BSP_USER_PROTOCOL_send_write_device_id_message(packet, id);
}


int USER_PROTOCOL_receive_packet(user_protocol_packet_pt packet)
{
    switch (packet->cmd)
    {
    case USER_PROTOCOL_MESSAGE_WRITE_ROLE:
        return _on_receive_write_role_message(packet);
    
    case USER_PROTOCOL_MESSAGE_READ_SIM_ICCID:
        return _on_receive_read_sim_iccid_message(packet);

    case USER_PROTOCOL_MESSAGE_READ_MANUFACTURER:
        return _on_receive_read_sim_manufacturer_message(packet);

    case USER_PROTOCOL_MESSAGE_WRITE_DEVICE_ID:
        return _on_receive_write_device_id_message(packet);

    default:
        return PROTOCOL_NOT_SUPPORT;
    }
}


int USER_PROTOCOL_send_write_role_message(user_protocol_port_pt port, user_protocol_packet_pt packet, uint8_t *role)
{
    packet->length = USER_PROTOCOL_role_message_serialize(packet->buffer, role);
    packet->parsed = role;
    packet->cmd = USER_PROTOCOL_MESSAGE_WRITE_ROLE;
    return USER_PROTOCOL_PACKET_write(port, packet);
}

int USER_PROTOCOL_send_read_sim_iccid_message(user_protocol_port_pt port, user_protocol_packet_pt packet)
{
    packet->length = 0;
    packet->parsed = NULL;
    packet->cmd = USER_PROTOCOL_MESSAGE_READ_SIM_ICCID;
    return USER_PROTOCOL_PACKET_write(port, packet);
}

int USER_PROTOCOL_send_read_sim_manufaturer_message(user_protocol_port_pt port, user_protocol_packet_pt packet)
{
    packet->length = 0;
    packet->parsed = NULL;
    packet->cmd = USER_PROTOCOL_MESSAGE_READ_MANUFACTURER;
    return USER_PROTOCOL_PACKET_write(port, packet);
}

int USER_PROTOCOL_send_write_device_id_message(user_protocol_port_pt port, user_protocol_packet_pt packet, uint8_t *deviceID)
{
    packet->length = USER_PROTOCOL_device_id_message_serialize(packet->buffer, deviceID);
    packet->parsed = packet->buffer;
    packet->cmd = USER_PROTOCOL_MESSAGE_WRITE_DEVICE_ID;
    return USER_PROTOCOL_PACKET_write(port, packet);
}


// #define USER_PROTOCOL_MESSAGES_PROCESS(_id, oper_name, oper_name_upcase,_name) \
//     __weak int USER_PROTOCOL##oper_name##_##_name##_message_process(user_protocol_packet_pt packet) \
//     { \
//         return PROTOCOL_OK;\
//     }

// USER_PROTOCOL_MESSAGES_MAP(USER_PROTOCOL_MESSAGES_PROCESS)
