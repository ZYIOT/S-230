#ifndef __USER_PROTOCOL_MASTER_H__
#define __USER_PROTOCOL_MASTER_H__


#ifdef __cplusplus
extern "C"
{
#endif
#include "user_protocol_common.h"
#include "user_protocol_packet.h"
#include "user_protocol_message.h"



int USER_PROTOCOL_receive_packet(user_protocol_packet_pt packet);
// int USER_PROTOCOL_send_write_server_message(user_protocol_port_pt port, user_protocol_packet_pt packet, user_protocol_server_message_pt message);
// int USER_PROTOCOL_send_read_server_message(user_protocol_port_pt port,user_protocol_packet_pt packet);


// #define USER_PROTOCOL_MESSAGES_MAP(F) \
//     F(USER_PROTOCOL_MESSAGE_WRITE_SERVER, , , write_server) \
//     F(USER_PROTOCOL_MESSAGE_READ_SERVER, , , read_server) \

    // #define USER_PROTOCOL_MESSAGES_PROCESS_DECLARE(_id, oper_name, oper_name_upcase,_name) \
    // int USER_PROTOCOL##oper_name##_##_name##_message_process(user_protocol_packet_pt packet);

    // USER_PROTOCOL_MESSAGES_MAP(USER_PROTOCOL_MESSAGES_PROCESS_DECLARE)

int USER_PROTOCOL_send_write_role_message(user_protocol_port_pt port, user_protocol_packet_pt packet, uint8_t *role);
int USER_PROTOCOL_send_read_sim_iccid_message(user_protocol_port_pt port, user_protocol_packet_pt packet);
int USER_PROTOCOL_send_read_sim_manufaturer_message(user_protocol_port_pt port, user_protocol_packet_pt packet);
int USER_PROTOCOL_send_write_device_id_message(user_protocol_port_pt port, user_protocol_packet_pt packet, uint8_t *device_id);

#ifdef __cplusplus
}
#endif

#endif

