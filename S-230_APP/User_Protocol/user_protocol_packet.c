
#include "user_protocol_packet.h"


static int left_shift_buffer(uint8_t *buf, size_t len)
{
    for (size_t i = 1; i < len; i++)
    {
        buf[i - 1] = buf[i];
    }
    return PROTOCOL_OK;
}



typedef int (*user_protocol_parser_fun)(user_protocol_port_pt port, user_protocol_packet_pt packet);

#pragma pack(1)
typedef struct
{
    uint16_t prefix;  /* 前缀 */ 
    uint16_t cmd_id;  /* 指令ID */ 
    uint8_t cmd;  /* 命令字 */ 
    uint16_t length;  /* 数据长度 */ 
    uint16_t length_r;  /* 数据长度反码 */ 
} user_protocol_packet_header_t, *user_protocol_packet_header_pt;
#pragma pack()

typedef union {
    user_protocol_packet_header_t header;
    uint8_t buffer[USER_PROTOCOL_HEADER_SIZE];
} user_protocol_packet_union_t, *user_protocol_packet_union_pt;

static int _check_header(user_protocol_packet_union_pt union_header, user_protocol_packet_pt packet)
{
    user_protocol_packet_header_t header = union_header->header;
    if((header.prefix == (0x5aa5)) &&
    (header.length_r == ((uint16_t)~header.length))){
        return PROTOCOL_OK;
    }
    return PROTOCOL_ERROR;
}


static int _copy_header(user_protocol_packet_union_pt union_header, user_protocol_packet_pt packet)
{
    user_protocol_packet_header_t header = union_header->header;
    packet->prefix = header.prefix;
    packet->cmd_id = header.cmd_id;
    packet->cmd = header.cmd;
    packet->length = header.length;
    packet->length_r = header.length_r;
    return PROTOCOL_OK;
}

static int _find_header(user_protocol_port_pt port, user_protocol_packet_pt packet)
{
    static user_protocol_packet_union_t packet_header;
    static uint8_t new_header = 0;
    int rc = 0;
    if(new_header == 0){
        memset(packet_header.buffer, 0, USER_PROTOCOL_HEADER_SIZE);
    }
    int len = port->length();
    if (len < 1 || (new_header == 0 && (len < USER_PROTOCOL_HEADER_SIZE)))
    {
        return PROTOCOL_READ_ERROR;
    }
    while (1)
    {
        if (new_header == 0)
        {
            rc = port->read(packet_header.buffer, USER_PROTOCOL_HEADER_SIZE, USER_PROTOCOL_READ_TIMEOUT);
        }
        else
        {
            rc = port->read(&packet_header.buffer[USER_PROTOCOL_HEADER_SIZE - 1], 1, USER_PROTOCOL_READ_TIMEOUT);
        }
        new_header = 1;
        USER_PROTOCOL_CHECK_READ_RC_AND_RETURN
        if (_check_header(&packet_header, packet) == PROTOCOL_OK)
        {
            _copy_header(&packet_header, packet);
            new_header = 0;
            return PROTOCOL_OK;
        }
        left_shift_buffer(packet_header.buffer, USER_PROTOCOL_HEADER_SIZE);
    }
}

static int _parse_extra(user_protocol_port_pt port, user_protocol_packet_pt packet)
{
    uint8_t buf[8] = {0};
    int rc = 0;
    if (packet->length > 0)
    {
        rc = port->read(packet->buffer, packet->length, USER_PROTOCOL_READ_TIMEOUT);
        USER_PROTOCOL_CHECK_READ_RC_AND_RETURN
    }
    rc = port->read(buf, 2, USER_PROTOCOL_READ_TIMEOUT);
    USER_PROTOCOL_CHECK_READ_RC_AND_RETURN
    packet->crc = read_uint16_t(buf); 
    return PROTOCOL_OK;
}

static int _parse_verify_crc(user_protocol_port_pt port, user_protocol_packet_pt packet)
{
    uint8_t buf[USER_PROTOCOL_PROTOCOL_BUFFER_SIZE] = {0};
    int len = USER_PROTOCOL_PACKET_serialize(packet, buf);
    if (user_protocol_crc16(buf, len - 2) != packet->crc)
    {
        return PROTOCOL_CRC_ERROR;
    }
    return PROTOCOL_OK;
}


int USER_PROTOCOL_init(void)
{
    return PROTOCOL_OK;
}

int USER_PROTOCOL_PACKET_init(user_protocol_packet_pt packet)
{
    packet->prefix = 0x5aa5; 
    packet->cmd_id = 0; 
    packet->cmd = 0; 
    packet->length = 0; 
    packet->length_r = 0; 
    memset(packet->buffer, 0, USER_PROTOCOL_MAX_BUFFER_SIZE); 
    packet->crc = 0; 
    packet->parsed = NULL;
    return PROTOCOL_OK;
}

int USER_PROTOCOL_PACKET_serialize(user_protocol_packet_pt packet, uint8_t bytes[USER_PROTOCOL_PROTOCOL_BUFFER_SIZE])
{
    uint16_t pos = 0;
    write_uint16_t(packet->prefix, &bytes[pos]); 
    pos += 2;
    write_uint16_t(packet->cmd_id, &bytes[pos]); 
    pos += 2;
    write_uint8_t(packet->cmd, &bytes[pos]); 
    pos += 1;
    write_uint16_t(packet->length, &bytes[pos]); 
    pos += 2;
    packet->length_r = ~packet->length;;
    write_uint16_t(packet->length_r, &bytes[pos]); 
    pos += 2;
    if(packet->length)
    {
        memcpy(&bytes[pos], packet->buffer, packet->length); 
        pos += packet->length;
    }
    write_uint16_t(packet->crc, &bytes[pos]); 
    pos += 2;
    return pos;
}

int USER_PROTOCOL_PACKET_read(user_protocol_port_pt port, user_protocol_packet_pt packet)
{
    int rc = 0;
    rc = _find_header(port, packet);
    USER_PROTOCOL_CHECK_RC
    rc = _parse_extra(port, packet);
    USER_PROTOCOL_CHECK_RC
    return _parse_verify_crc(port, packet);
}

int USER_PROTOCOL_PACKET_write(user_protocol_port_pt port, user_protocol_packet_pt packet)
{
    uint8_t buf[USER_PROTOCOL_PROTOCOL_BUFFER_SIZE] = {0};
    int len = USER_PROTOCOL_PACKET_serialize(packet, buf);
    uint16_t crc = user_protocol_crc16(buf, len - 2);
    write_uint16_t(crc, &buf[len - 2]);
    port->write(buf, len, USER_PROTOCOL_WRITE_TIMEOUT);
    return PROTOCOL_OK;
}
