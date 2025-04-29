#ifndef __USER_PROTOCOL_COMMON_H__
#define __USER_PROTOCOL_COMMON_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "app.h"
#include "utils.h"

#define PROTOCOL_OK 0
#define PROTOCOL_ERROR -1
#define PROTOCOL_READ_ERROR -2
#define PROTOCOL_CRC_ERROR -3
#define PROTOCOL_NOT_SUPPORT -255
#define USER_PROTOCOL_READ_TIMEOUT 1000
#define USER_PROTOCOL_WRITE_TIMEOUT 0x2fff


#define USER_PROTOCOL_CHECK_RC  \
    if (rc != PROTOCOL_OK)      \
    {                           \
        return rc;              \
    }

#define USER_PROTOCOL_CHECK_READ_RC_AND_RETURN  \
    if (rc <= 0)                                \
    {                                           \
        return PROTOCOL_READ_ERROR;             \
    }

#define USER_PROTOCOL_MAX_BUFFER_SIZE   (100)
#define USER_PROTOCOL_HEADER_SIZE       (9)
#define USER_PROTOCOL_PROTOCOL_BUFFER_SIZE  (USER_PROTOCOL_MAX_BUFFER_SIZE + USER_PROTOCOL_HEADER_SIZE + 4)

#define USER_PROTOCOL_MESSAGE_WRITE_SERVER  1
#define USER_PROTOCOL_MESSAGE_READ_SERVER   2

#define user_protocol_crc16(buf, size) get_modbus_crc16(buf, size)

typedef enum
{
    USER_PROTOCOL_MESSAGE_WRITE_ROLE = 1,       // 设置设备的主从应答
    USER_PROTOCOL_MESSAGE_READ_SIM_ICCID,       // 获取 SIM 卡的 ICCID
    USER_PROTOCOL_MESSAGE_READ_MANUFACTURER,    // 获取 SIM 卡的运行商类型
    USER_PROTOCOL_MESSAGE_WRITE_DEVICE_ID,      // 下发控制箱设备 ID 
    USER_PROTOCOL_MESSAGE_WRITE_MQTT_DATA,      // 下发 MQTT 相关数据 
    USER_PROTOCOL_MESSAGE_WRITE_CSQ,            // 获取 4G模块的 CSQ 值 
    USER_PROTOCOL_UNKNOW_CMD,
}user_protocol_cmd_e;


#ifdef __cplusplus
}
#endif

#endif
