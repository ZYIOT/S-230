#ifndef __MODBUS_PACKET_H
#define __MODBUS_PACKET_H
#include "modbus_common.h"

#ifdef __cplusplus
extern "C"
{
#endif

  typedef int (*modbus_read_fun)(uint8_t buffer[MODBUS_MODBUS_BUFFER_SIZE], size_t len, size_t tick);
  typedef int (*modbus_write_fun)(uint8_t buffer[MODBUS_MODBUS_BUFFER_SIZE], size_t len, size_t tick);
  typedef void (*modbus_clear_fun)(void);
  typedef size_t (*modbus_length_fun)(void);

  typedef struct
  {
    modbus_read_fun read;
    modbus_write_fun write;
    modbus_clear_fun clear;
    modbus_length_fun length;
  } modbus_port_t, *modbus_port_pt;

  typedef struct
  {
    uint8_t id;   //目标地址
    uint8_t addr; //返回数据时的目标地址检查
    uint8_t code;
    uint8_t buffer[MODBUS_MAX_BUFFER_SIZE];
    uint16_t crc;
    uint16_t length;
    uint8_t exception;
    void *parsed;
  } modbus_packet_t, *modbus_packet_pt;

  WINAPI_EXPORT int WINAPI MODBUS_init(void);
  WINAPI_EXPORT int WINAPI MODBUS_PACKET_init(modbus_packet_pt packet);
  WINAPI_EXPORT uint16_t WINAPI MODBUS_PACKET_serialize(modbus_packet_pt packet, uint8_t bytes[MODBUS_MODBUS_BUFFER_SIZE]);
  WINAPI_EXPORT int WINAPI MODBUS_PACKET_read(modbus_port_pt port, modbus_packet_pt packet);
  WINAPI_EXPORT int WINAPI MODBUS_PACKET_write(modbus_port_pt port, modbus_packet_pt packet);

#ifdef __cplusplus
}
#endif
#endif
