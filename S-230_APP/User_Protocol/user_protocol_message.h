#ifndef __USER_PROTOCOL_MESSAGE_H__
#define __USER_PROTOCOL_MESSAGE_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "app.h"
#include "user_protocol_packet.h"
#include "user_protocol_common.h"

int USER_PROTOCOL_role_message_init(uint8_t *role);
uint16_t USER_PROTOCOL_role_message_serialize(uint8_t *buf, uint8_t *role);
uint16_t USER_PROTOCOL_role_message_deserialize(uint8_t *buf, uint8_t *role);

int  USER_PROTOCOL_sim_iccid_message_init(uint8_t *sim_iccid);
uint16_t USER_PROTOCOL_sim_iccid_message_serialize(uint8_t *buf, uint8_t *sim_iccid);
uint16_t USER_PROTOCOL_sim_iccid_message_deserialize(uint8_t *buf, uint8_t *sim_iccid);

int  USER_PROTOCOL_sim_manufacturer_message_init(uint8_t *manufacturer);
uint16_t USER_PROTOCOL_sim_manufacturer_message_serialize(uint8_t *buf, uint8_t *manufacturer);
uint16_t USER_PROTOCOL_sim_manufacturer_message_deserialize(uint8_t *buf, uint8_t *manufacturer);

int USER_PROTOCOL_device_id_message_init(uint8_t *deviceID);
uint16_t USER_PROTOCOL_device_id_message_serialize(uint8_t *buf, uint8_t *deviceID);
uint16_t USER_PROTOCOL_device_id_message_deserialize(uint8_t *buf, uint8_t *deviceID);

typedef struct 
{
    uint8_t code;  /* 命令回复 */ 
} user_protocol_status_message_t, *user_protocol_status_message_pt;

typedef struct 
{
    uint16_t port;  /* 服务器端口 */ 
    uint8_t host_length;  /* 服务器地址长度 */ 
    uint8_t host[USER_PROTOCOL_MAX_BUFFER_SIZE];  /* 数据 */ 
} user_protocol_server_message_t, *user_protocol_server_message_pt;

#ifdef __cplusplus
}
#endif

#endif
