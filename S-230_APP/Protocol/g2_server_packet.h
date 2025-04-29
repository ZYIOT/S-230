#ifndef __G2_SERVER_PACKET_H
#define __G2_SERVER_PACKET_H
#ifdef __cplusplus
extern "C"
{
#endif

#include "g2_server_common.h"

typedef int (*g2_server_read_fun)(uint8_t buffer[G2_SERVER_PROTOCOL_BUFFER_SIZE], size_t len, size_t tick);
typedef int (*g2_server_write_fun)(uint8_t buffer[G2_SERVER_PROTOCOL_BUFFER_SIZE], size_t len, size_t tick);
typedef void (*g2_server_clear_fun)(void);
typedef size_t (*g2_server_length_fun)(void);

typedef struct
{
    g2_server_read_fun read;
    g2_server_write_fun write;
    g2_server_clear_fun clear;
    g2_server_length_fun length;
} g2_server_port_t, *g2_server_port_pt;

typedef struct 
{
    uint32_t deviceID;  	/* 设备ID */
    uint8_t code;  			/* 功能码 */ 
    uint8_t header;  		/* 帧头 */ 
    uint16_t cmd_id;  		/* 指令ID */ 
    uint8_t cmd;  			/* 命令字 */ 
    uint8_t cmd_status;  	/* 命令字状态 */ 
    uint8_t reversed;  		/* 预留 */ 
    uint8_t version;  		/* 版本 */ 
    uint8_t length;  		/* 数据长度 */ 
    uint8_t buffer[G2_SERVER_MAX_BUFFER_SIZE];  /* 数据 */ 
    uint16_t crc;  			/* CRC */ 
    uint8_t exception;  	/* 异常 */ 
    void *parsed;
} g2_server_packet_t, *g2_server_packet_pt;

WINAPI_EXPORT int WINAPI G2_SERVER_init(void);
WINAPI_EXPORT int WINAPI G2_SERVER_PACKET_init(g2_server_packet_pt packet);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_PACKET_serialize(g2_server_packet_pt packet, uint8_t bytes[G2_SERVER_PROTOCOL_BUFFER_SIZE]);
WINAPI_EXPORT int WINAPI G2_SERVER_PACKET_read(g2_server_port_pt port, g2_server_packet_pt packet);
WINAPI_EXPORT int WINAPI G2_SERVER_PACKET_write(g2_server_port_pt port, g2_server_packet_pt packet);


uint16_t getPacketLen(g2_server_packet_pt pt);
void setPacketLen(g2_server_packet_pt pt, uint16_t len);

#ifdef __cplusplus
}
#endif
#endif
