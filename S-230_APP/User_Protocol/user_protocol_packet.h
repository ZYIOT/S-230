#ifndef __USER_PROTOCOL_PACKET_H__
#define __USER_PROTOCOL_PACKET_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "user_protocol_common.h"


typedef int (*user_protocol_read_fun)(uint8_t buffer[USER_PROTOCOL_PROTOCOL_BUFFER_SIZE], size_t len, size_t tick);
typedef int (*user_protocol_write_fun)(uint8_t buffer[USER_PROTOCOL_PROTOCOL_BUFFER_SIZE], size_t len, size_t tick);
typedef void (*user_protocol_clear_fun)(void);
typedef size_t (*user_protocol_length_fun)(void);

typedef struct
{
    user_protocol_read_fun read;
    user_protocol_write_fun write;
    user_protocol_clear_fun clear;
    user_protocol_length_fun length;
} user_protocol_port_t, *user_protocol_port_pt;

typedef struct 
{
    uint16_t prefix;  /* 前缀 */ 
    uint16_t cmd_id;  /* 指令ID */ 
    uint8_t cmd;  /* 命令字 */ 
    uint16_t length;  /* 数据长度 */ 
    uint16_t length_r;  /* 数据长度反码 */ 
    uint8_t buffer[USER_PROTOCOL_MAX_BUFFER_SIZE];  /* 数据 */ 
    uint16_t crc;  /* CRC */ 
    void *parsed;
} user_protocol_packet_t, *user_protocol_packet_pt;


int USER_PROTOCOL_init(void);
int USER_PROTOCOL_PACKET_init(user_protocol_packet_pt packet);
int USER_PROTOCOL_PACKET_serialize(user_protocol_packet_pt packet, uint8_t bytes[USER_PROTOCOL_PROTOCOL_BUFFER_SIZE]);
int USER_PROTOCOL_PACKET_read(user_protocol_port_pt port, user_protocol_packet_pt packet);
int USER_PROTOCOL_PACKET_write(user_protocol_port_pt port, user_protocol_packet_pt packet);

#ifdef __cplusplus
}
#endif

#endif
