#include "modbus_master.h"

static int _on_receive_read_holding_registers_message(modbus_packet_pt packet)
{
  modbus_read_holding_registers_response_message_t message;
  int rc = MODBUS_read_holding_registers_response_message_deserialize(packet->buffer, &message);
  MODBUS_CHECK_READ_RC_AND_RETURN
  packet->parsed = &message;
  return MODBUS_read_holding_registers_message_process(packet);
}

static int _on_receive_write_single_register_message(modbus_packet_pt packet)
{
  modbus_write_single_register_message_t message;
  int rc = MODBUS_write_single_register_message_deserialize(packet->buffer, &message);
  MODBUS_CHECK_READ_RC_AND_RETURN
  packet->parsed = &message;
  return MODBUS_write_single_register_message_process(packet);
}

static int _on_receive_write_multiple_registers_message(modbus_packet_pt packet)
{
  modbus_write_multiple_registers_response_message_t message;
  int rc = MODBUS_write_multiple_registers_response_message_deserialize(packet->buffer, &message);
  MODBUS_CHECK_READ_RC_AND_RETURN
  packet->parsed = &message;
  return MODBUS_write_multiple_registers_message_process(packet);
}

static int _on_receive_error_message(modbus_packet_pt packet)
{
  modbus_error_response_message_t message;
  int rc = MODBUS_error_response_message_deserialize(packet->buffer, &message);
  MODBUS_CHECK_READ_RC_AND_RETURN
  packet->parsed = &message;
  return MODBUS_error_message_process(packet);
}

WINAPI_EXPORT int WINAPI MODBUS_receive_packet(modbus_packet_pt packet)
{
  if (packet->exception == 1)
  {
    return _on_receive_error_message(packet);
  }
  switch (packet->code)
  {
  case MODBUS_MESSAGE_READ_HOLDING_REGISTERS:
    return _on_receive_read_holding_registers_message(packet);
  case MODBUS_MESSAGE_WRITE_SINGLE_REGISTER:
    return _on_receive_write_single_register_message(packet);
  case MODBUS_MESSAGE_WRITE_MULTIPLE_REGISTERS:
    return _on_receive_write_multiple_registers_message(packet);
  }
  return MODBUS_NOT_SUPPORT;
}

WINAPI_EXPORT int WINAPI MODBUS_send_read_holding_registers_message(modbus_port_pt port, modbus_packet_pt packet, modbus_read_holding_registers_request_message_pt message)
{
  packet->length = MODBUS_read_holding_registers_request_message_serialize(packet->buffer, message);
  packet->parsed = message;
  packet->code = MODBUS_MESSAGE_READ_HOLDING_REGISTERS;
  return MODBUS_PACKET_write(port, packet);
}


WINAPI_EXPORT int WINAPI MODBUS_send_write_single_register_message(modbus_port_pt port, modbus_packet_pt packet, modbus_write_single_register_message_pt message)
{
  packet->length = MODBUS_write_single_register_message_serialize(packet->buffer, message);
  packet->parsed = message;
  packet->code = MODBUS_MESSAGE_WRITE_SINGLE_REGISTER;
  return MODBUS_PACKET_write(port, packet);
}


WINAPI_EXPORT int WINAPI MODBUS_send_write_multiple_registers_message(modbus_port_pt port, modbus_packet_pt packet, modbus_write_multiple_registers_request_message_pt message)
{
  packet->length = MODBUS_write_multiple_registers_request_message_serialize(packet->buffer, message);
  packet->parsed = message;
  packet->code = MODBUS_MESSAGE_WRITE_MULTIPLE_REGISTERS;
  return MODBUS_PACKET_write(port, packet);
}

#if defined(WIN32) || defined(_WIN32)
static modbus_message_callback_t modbus_message_callback = NULL;
WINAPI_EXPORT void WINAPI MODBUS_set_message_callback(modbus_message_callback_t callback)
{
  modbus_message_callback = callback;
}

WINAPI_EXPORT int WINAPI MODBUS_message_process(modbus_packet_pt packet)
{
  if (modbus_message_callback != NULL)
  {
    modbus_message_callback(packet);
  }
  else
  {
    return MODBUS_OK;
  }
}

int MODBUS_error_message_process(modbus_packet_pt packet)
{
  return MODBUS_message_process(packet);
}

#define MODBUS_MESSAGES_PROCESS(_id, oper_name, oper_name_upcase, _name)     \
  int MODBUS##oper_name##_##_name##_message_process(modbus_packet_pt packet) \
  {                                                                          \
    return MODBUS_message_process(packet);                                   \
  }

#else
#define MODBUS_MESSAGES_PROCESS(_id, oper_name, oper_name_upcase, _name)            \
  __weak int MODBUS##oper_name##_##_name##_message_process(modbus_packet_pt packet) \
  {                                                                                 \
    return MODBUS_OK;                                                               \
  }

__weak int MODBUS_error_message_process(modbus_packet_pt packet)
{
  return MODBUS_OK;
}
#endif
MODBUS_MESSAGES_MAP(MODBUS_MESSAGES_PROCESS)
