#ifndef __MODBUS_MASTER_H
#define __MODBUS_MASTER_H
#include "modbus_common.h"
#include "modbus_packet.h"
#include "modbus_message.h"

#ifdef __cplusplus
extern "C"
{
#endif

  WINAPI_EXPORT int WINAPI MODBUS_receive_packet(modbus_packet_pt packet);
  WINAPI_EXPORT int WINAPI MODBUS_send_read_holding_registers_message(modbus_port_pt port, modbus_packet_pt packet, modbus_read_holding_registers_request_message_pt message);
  WINAPI_EXPORT int WINAPI MODBUS_send_write_single_register_message(modbus_port_pt port, modbus_packet_pt packet, modbus_write_single_register_message_pt message);
  WINAPI_EXPORT int WINAPI MODBUS_send_write_multiple_registers_message(modbus_port_pt port, modbus_packet_pt packet, modbus_write_multiple_registers_request_message_pt message);

#define MODBUS_MESSAGES_MAP(F)                                             \
  F(MODBUS_MESSAGE_READ_HOLDING_REGISTERS, , , read_holding_registers)     \
  F(MODBUS_MESSAGE_WRITE_SINGLE_REGISTER, , , write_single_register)       \
  F(MODBUS_MESSAGE_WRITE_MULTIPLE_REGISTERS, , , write_multiple_registers) 

#define MODBUS_MESSAGES_PROCESS_DECLARE(_id, oper_name, oper_name_upcase, _name) \
  int MODBUS##oper_name##_##_name##_message_process(modbus_packet_pt packet);

  MODBUS_MESSAGES_MAP(MODBUS_MESSAGES_PROCESS_DECLARE)
  int MODBUS_error_message_process(modbus_packet_pt packet);

#if defined(WIN32) || defined(_WIN32)
  typedef int (*modbus_message_callback_t)(modbus_packet_pt packet);

  WINAPI_EXPORT void WINAPI MODBUS_set_message_callback(modbus_message_callback_t callback);
  WINAPI_EXPORT int WINAPI MODBUS_message_process(modbus_packet_pt packet);
#endif

#ifdef __cplusplus
}
#endif
#endif
