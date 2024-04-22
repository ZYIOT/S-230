#ifndef __G2_SERVER_COMMON_H
#define __G2_SERVER_COMMON_H
#ifdef __cplusplus
extern "C"
{
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "utils.h"

#if defined(WIN32) || defined(_WIN32)
#define WINAPI_EXPORT __declspec(dllexport)
#define __weak 
// #ifndef WINAPI
// #define WINAPI __stdcall
// #endif
#define WINAPI
#else
#define WINAPI_EXPORT
#define WINAPI
#endif

#include "app_config.h"

#define PROTOCOL_OK 0
#define PROTOCOL_ERROR -1
#define PROTOCOL_READ_ERROR -2
#define PROTOCOL_CRC_ERROR -3
#define PROTOCOL_NOT_SUPPORT -255
#define G2_SERVER_READ_TIMEOUT 1000
#define G2_SERVER_WRITE_TIMEOUT 0x2fff

#define G2_SERVER_CHECK_RC      \
    if (rc != PROTOCOL_OK)      \
    {                           \
        return rc;              \
    }

#define G2_SERVER_CHECK_READ_RC_AND_RETURN  \
    if (rc <= 0)                            \
    {                                       \
        return PROTOCOL_READ_ERROR;         \
    }

#define G2_SERVER_MAX_BUFFER_SIZE (USED_DATA_MAX_SIZE)
#define G2_SERVER_MESSAGE_OPERATION_READ 0x3
#define G2_SERVER_MESSAGE_OPERATION_WRITE 0x16
#define G2_SERVER_MESSAGE_OPERATION_DELETE 0x17
#define G2_SERVER_OPERATION_CODE_WRITE 0x51
#define G2_SERVER_OPERATION_CODE_DELETE 0x52
#define G2_SERVER_PROTOCOL_DEVICE_ID APP_CONFIG_device_id()
#define G2_SERVER_HEADER_SIZE 13
#define G2_SERVER_PROTOCOL_BUFFER_SIZE G2_SERVER_MAX_BUFFER_SIZE + G2_SERVER_HEADER_SIZE + 4

#define G2_SERVER_MESSAGE_DEVICE_ID                 1
#define G2_SERVER_MESSAGE_VERSION                   2
#define G2_SERVER_MESSAGE_SENSOR_CONFIG             3
#define G2_SERVER_MESSAGE_SENSOR_LIMIT              4
#define G2_SERVER_MESSAGE_SENSOR_ALERT              5
#define G2_SERVER_MESSAGE_RELAY_JOBS                6
#define G2_SERVER_MESSAGE_SALINITY                  7
#define G2_SERVER_MESSAGE_CALIBRATE                 8
#define G2_SERVER_MESSAGE_TIME                      10
#define G2_SERVER_MESSAGE_RELAY_MANUAL              11
#define G2_SERVER_MESSAGE_RELAY_MANUAL_ENABLE       12
#define G2_SERVER_MESSAGE_SYSTEM                    13
#define G2_SERVER_MESSAGE_CONNECT                   14
#define G2_SERVER_MESSAGE_SENSOR_LIMIT_ENABLE       15
#define G2_SERVER_MESSAGE_DEVICE_ENABLE             16
#define G2_SERVER_MESSAGE_DEVICE_CAPABILITY         17
#define G2_SERVER_MESSAGE_RELAY_DATA                18
#define G2_SERVER_MESSAGE_SENSOR_DATA               19
#define G2_SERVER_MESSAGE_RECOVERY                  20
#define G2_SERVER_MESSAGE_DEVICE_USED_CAPABILITY    21
#define G2_SERVER_MESSAGE_DEVICE_SENSORS            22
#define G2_SERVER_MESSAGE_RELAY_CONFIG              27
#define G2_SERVER_MESSAGE_RELAY_JOBS_ENABLE         28
#define G2_SERVER_MESSAGE_PUMP                      30
#define G2_SERVER_MESSAGE_RESTART                   32
#define G2_SERVER_MESSAGE_GPRS_SIGNAL               33
#define G2_SERVER_MESSAGE_HEARTBEAT                 34
#define G2_SERVER_MESSAGE_GET_TIME                  35
#define G2_SERVER_MESSAGE_L6_START                  36
#define G2_SERVER_MESSAGE_L6_SETTINGS               37
#define G2_SERVER_MESSAGE_L6_STATUS                 38
#define G2_SERVER_MESSAGE_ENVIRONMENT               39
#define G2_SERVER_MESSAGE_FIREWARE_START            40
#define G2_SERVER_MESSAGE_FIREWARE_DATA             41
#define G2_SERVER_MESSAGE_FIREWARE_STOP             42
#define G2_SERVER_MESSAGE_SELF_DIAGNOSIS            43
#define G2_SERVER_MESSAGE_READ_ICCID                44    // 0x2C 
#define G2_SERVER_MESSAGE_RELAY_POWER_CONFIG        46    // 0x2E 
#define G2_SERVER_MESSAGE_DEVICE_WARNNING           47    // 0x2F 
#define G2_SERVER_MESSAGE_LOADER_COONFIG            48    // 0x30

#define crc16(buf, size) get_modbus_crc16(buf, size)
#define crc16_add(buf, size, input)	add_modbus_crc16(buf, size, input)


#ifdef __cplusplus
}
#endif
#endif
