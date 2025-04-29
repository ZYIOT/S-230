#include "modbus_message.h"

  WINAPI_EXPORT int WINAPI MODBUS_error_response_message_init(modbus_error_response_message_pt message)
  {
    message->status = 0;
    return MODBUS_OK;
  }

  WINAPI_EXPORT uint16_t WINAPI MODBUS_error_response_message_serialize(uint8_t *bytes, modbus_error_response_message_pt message)
  {
    uint16_t pos = 0;
    write_uint8_t_BE(message->status, &bytes[pos]);
    pos += 1;
    return pos;
  }

  WINAPI_EXPORT uint16_t WINAPI MODBUS_error_response_message_deserialize(uint8_t *bytes, modbus_error_response_message_pt message)
  {
    uint16_t pos = 0;
    message->status = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    return pos;
  }

  WINAPI_EXPORT int WINAPI MODBUS_read_coils_request_message_init(modbus_read_coils_request_message_pt message)
  {
    message->address = 0;
    message->quantity = 0;
    return MODBUS_OK;
  }

  WINAPI_EXPORT uint16_t WINAPI MODBUS_read_coils_request_message_serialize(uint8_t *bytes, modbus_read_coils_request_message_pt message)
  {
    uint16_t pos = 0;
    write_uint16_t_BE(message->address, &bytes[pos]);
    pos += 2;
    write_uint16_t_BE(message->quantity, &bytes[pos]);
    pos += 2;
    return pos;
  }

  WINAPI_EXPORT uint16_t WINAPI MODBUS_read_coils_request_message_deserialize(uint8_t *bytes, modbus_read_coils_request_message_pt message)
  {
    uint16_t pos = 0;
    message->address = read_uint16_t_BE(&bytes[pos]);
    pos += 2;
    message->quantity = read_uint16_t_BE(&bytes[pos]);
    pos += 2;
    return pos;
  }

  WINAPI_EXPORT int WINAPI MODBUS_read_coils_response_message_init(modbus_read_coils_response_message_pt message)
  {
    message->count = 0;
    memset(message->buffer, 0, MODBUS_MAX_BUFFER_SIZE);
    return MODBUS_OK;
  }

  WINAPI_EXPORT uint16_t WINAPI MODBUS_read_coils_response_message_serialize(uint8_t *bytes, modbus_read_coils_response_message_pt message)
  {
    uint16_t pos = 0;
    write_uint8_t_BE(message->count, &bytes[pos]);
    pos += 1;
    memcpy(&bytes[pos], message->buffer, message->count);
    pos += message->count;
    return pos;
  }

  WINAPI_EXPORT uint16_t WINAPI MODBUS_read_coils_response_message_deserialize(uint8_t *bytes, modbus_read_coils_response_message_pt message)
  {
    uint16_t pos = 0;
    message->count = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    memcpy(message->buffer, &bytes[pos], message->count);
    pos += message->count;
    return pos;
  }

  WINAPI_EXPORT int WINAPI MODBUS_read_discrete_inputs_request_message_init(modbus_read_discrete_inputs_request_message_pt message)
  {
    message->address = 0;
    message->quantity = 0;
    return MODBUS_OK;
  }

  WINAPI_EXPORT uint16_t WINAPI MODBUS_read_discrete_inputs_request_message_serialize(uint8_t *bytes, modbus_read_discrete_inputs_request_message_pt message)
  {
    uint16_t pos = 0;
    write_uint16_t_BE(message->address, &bytes[pos]);
    pos += 2;
    write_uint16_t_BE(message->quantity, &bytes[pos]);
    pos += 2;
    return pos;
  }

  WINAPI_EXPORT uint16_t WINAPI MODBUS_read_discrete_inputs_request_message_deserialize(uint8_t *bytes, modbus_read_discrete_inputs_request_message_pt message)
  {
    uint16_t pos = 0;
    message->address = read_uint16_t_BE(&bytes[pos]);
    pos += 2;
    message->quantity = read_uint16_t_BE(&bytes[pos]);
    pos += 2;
    return pos;
  }

  WINAPI_EXPORT int WINAPI MODBUS_read_discrete_inputs_response_message_init(modbus_read_discrete_inputs_response_message_pt message)
  {
    message->count = 0;
    memset(message->buffer, 0, MODBUS_MAX_BUFFER_SIZE);
    return MODBUS_OK;
  }

  WINAPI_EXPORT uint16_t WINAPI MODBUS_read_discrete_inputs_response_message_serialize(uint8_t *bytes, modbus_read_discrete_inputs_response_message_pt message)
  {
    uint16_t pos = 0;
    write_uint8_t_BE(message->count, &bytes[pos]);
    pos += 1;
    memcpy(&bytes[pos], message->buffer, message->count);
    pos += message->count;
    return pos;
  }

  WINAPI_EXPORT uint16_t WINAPI MODBUS_read_discrete_inputs_response_message_deserialize(uint8_t *bytes, modbus_read_discrete_inputs_response_message_pt message)
  {
    uint16_t pos = 0;
    message->count = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    memcpy(message->buffer, &bytes[pos], message->count);
    pos += message->count;
    return pos;
  }

  WINAPI_EXPORT int WINAPI MODBUS_read_holding_registers_request_message_init(modbus_read_holding_registers_request_message_pt message)
  {
    message->address = 0;
    message->quantity = 0;
    return MODBUS_OK;
  }

  WINAPI_EXPORT uint16_t WINAPI MODBUS_read_holding_registers_request_message_serialize(uint8_t *bytes, modbus_read_holding_registers_request_message_pt message)
  {
    uint16_t pos = 0;
    write_uint16_t_BE(message->address, &bytes[pos]);
    pos += 2;
    write_uint16_t_BE(message->quantity, &bytes[pos]);
    pos += 2;
    return pos;
  }

  WINAPI_EXPORT uint16_t WINAPI MODBUS_read_holding_registers_request_message_deserialize(uint8_t *bytes, modbus_read_holding_registers_request_message_pt message)
  {
    uint16_t pos = 0;
    message->address = read_uint16_t_BE(&bytes[pos]);
    pos += 2;
    message->quantity = read_uint16_t_BE(&bytes[pos]);
    pos += 2;
    return pos;
  }

  WINAPI_EXPORT int WINAPI MODBUS_read_holding_registers_response_message_init(modbus_read_holding_registers_response_message_pt message)
  {
    message->count = 0;
    memset(message->buffer, 0, MODBUS_MAX_BUFFER_SIZE);
    return MODBUS_OK;
  }

  WINAPI_EXPORT uint16_t WINAPI MODBUS_read_holding_registers_response_message_serialize(uint8_t *bytes, modbus_read_holding_registers_response_message_pt message)
  {
    uint16_t pos = 0;
    write_uint8_t_BE(message->count, &bytes[pos]);
    pos += 1;
    memcpy(&bytes[pos], message->buffer, message->count);
    pos += message->count;
    return pos;
  }

  WINAPI_EXPORT uint16_t WINAPI MODBUS_read_holding_registers_response_message_deserialize(uint8_t *bytes, modbus_read_holding_registers_response_message_pt message)
  {
    uint16_t pos = 0;
    message->count = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    memcpy(message->buffer, &bytes[pos], message->count);
    pos += message->count ;
    return pos;
  }

  WINAPI_EXPORT int WINAPI MODBUS_read_input_registers_request_message_init(modbus_read_input_registers_request_message_pt message)
  {
    message->address = 0;
    message->quantity = 0;
    return MODBUS_OK;
  }

  WINAPI_EXPORT uint16_t WINAPI MODBUS_read_input_registers_request_message_serialize(uint8_t *bytes, modbus_read_input_registers_request_message_pt message)
  {
    uint16_t pos = 0;
    write_uint16_t_BE(message->address, &bytes[pos]);
    pos += 2;
    write_uint16_t_BE(message->quantity, &bytes[pos]);
    pos += 2;
    return pos;
  }

  WINAPI_EXPORT uint16_t WINAPI MODBUS_read_input_registers_request_message_deserialize(uint8_t *bytes, modbus_read_input_registers_request_message_pt message)
  {
    uint16_t pos = 0;
    message->address = read_uint16_t_BE(&bytes[pos]);
    pos += 2;
    message->quantity = read_uint16_t_BE(&bytes[pos]);
    pos += 2;
    return pos;
  }

  WINAPI_EXPORT int WINAPI MODBUS_read_input_registers_response_message_init(modbus_read_input_registers_response_message_pt message)
  {
    message->count = 0;
    memset(message->buffer, 0, MODBUS_MAX_BUFFER_SIZE);
    return MODBUS_OK;
  }

  WINAPI_EXPORT uint16_t WINAPI MODBUS_read_input_registers_response_message_serialize(uint8_t *bytes, modbus_read_input_registers_response_message_pt message)
  {
    uint16_t pos = 0;
    write_uint8_t_BE(message->count, &bytes[pos]);
    pos += 1;
    memcpy(&bytes[pos], message->buffer, message->count);
    pos += message->count;
    return pos;
  }

  WINAPI_EXPORT uint16_t WINAPI MODBUS_read_input_registers_response_message_deserialize(uint8_t *bytes, modbus_read_input_registers_response_message_pt message)
  {
    uint16_t pos = 0;
    message->count = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    memcpy(message->buffer, &bytes[pos], message->count);
    pos += message->count;
    return pos;
  }

  WINAPI_EXPORT int WINAPI MODBUS_write_single_coil_message_init(modbus_write_single_coil_message_pt message)
  {
    message->address = 0;
    message->value = 0;
    return MODBUS_OK;
  }

  WINAPI_EXPORT uint16_t WINAPI MODBUS_write_single_coil_message_serialize(uint8_t *bytes, modbus_write_single_coil_message_pt message)
  {
    uint16_t pos = 0;
    write_uint16_t_BE(message->address, &bytes[pos]);
    pos += 2;
    write_uint16_t_BE(message->value, &bytes[pos]);
    pos += 2;
    return pos;
  }

  WINAPI_EXPORT uint16_t WINAPI MODBUS_write_single_coil_message_deserialize(uint8_t *bytes, modbus_write_single_coil_message_pt message)
  {
    uint16_t pos = 0;
    message->address = read_uint16_t_BE(&bytes[pos]);
    pos += 2;
    message->value = read_uint16_t_BE(&bytes[pos]);
    pos += 2;
    return pos;
  }

  WINAPI_EXPORT int WINAPI MODBUS_write_single_register_message_init(modbus_write_single_register_message_pt message)
  {
    message->address = 0;
    message->value = 0;
    return MODBUS_OK;
  }

  WINAPI_EXPORT uint16_t WINAPI MODBUS_write_single_register_message_serialize(uint8_t *bytes, modbus_write_single_register_message_pt message)
  {
    uint16_t pos = 0;
    write_uint16_t_BE(message->address, &bytes[pos]);
    pos += 2;
    write_uint16_t_BE(message->value, &bytes[pos]);
    pos += 2;
    return pos;
  }

  WINAPI_EXPORT uint16_t WINAPI MODBUS_write_single_register_message_deserialize(uint8_t *bytes, modbus_write_single_register_message_pt message)
  {
    uint16_t pos = 0;
    message->address = read_uint16_t_BE(&bytes[pos]);
    pos += 2;
    message->value = read_uint16_t_BE(&bytes[pos]);
    pos += 2;
    return pos;
  }

  WINAPI_EXPORT int WINAPI MODBUS_read_exception_status_response_message_init(modbus_read_exception_status_response_message_pt message)
  {
    message->status = 0;
    return MODBUS_OK;
  }

  WINAPI_EXPORT uint16_t WINAPI MODBUS_read_exception_status_response_message_serialize(uint8_t *bytes, modbus_read_exception_status_response_message_pt message)
  {
    uint16_t pos = 0;
    write_uint8_t_BE(message->status, &bytes[pos]);
    pos += 1;
    return pos;
  }

  WINAPI_EXPORT uint16_t WINAPI MODBUS_read_exception_status_response_message_deserialize(uint8_t *bytes, modbus_read_exception_status_response_message_pt message)
  {
    uint16_t pos = 0;
    message->status = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    return pos;
  }

  WINAPI_EXPORT int WINAPI MODBUS_diagnostics_message_init(modbus_diagnostics_message_pt message)
  {
    message->sub_function = 0;
    message->data = 0;
    return MODBUS_OK;
  }

  WINAPI_EXPORT uint16_t WINAPI MODBUS_diagnostics_message_serialize(uint8_t *bytes, modbus_diagnostics_message_pt message)
  {
    uint16_t pos = 0;
    write_uint16_t_BE(message->sub_function, &bytes[pos]);
    pos += 2;
    write_uint16_t_BE(message->data, &bytes[pos]);
    pos += 2;
    return pos;
  }

  WINAPI_EXPORT uint16_t WINAPI MODBUS_diagnostics_message_deserialize(uint8_t *bytes, modbus_diagnostics_message_pt message)
  {
    uint16_t pos = 0;
    message->sub_function = read_uint16_t_BE(&bytes[pos]);
    pos += 2;
    message->data = read_uint16_t_BE(&bytes[pos]);
    pos += 2;
    return pos;
  }

  WINAPI_EXPORT int WINAPI MODBUS_get_common_event_counter_response_message_init(modbus_get_common_event_counter_response_message_pt message)
  {
    message->status = 0xffff;
    message->event_count = 0;
    return MODBUS_OK;
  }

  WINAPI_EXPORT uint16_t WINAPI MODBUS_get_common_event_counter_response_message_serialize(uint8_t *bytes, modbus_get_common_event_counter_response_message_pt message)
  {
    uint16_t pos = 0;
    write_uint16_t_BE(message->status, &bytes[pos]);
    pos += 2;
    write_uint16_t_BE(message->event_count, &bytes[pos]);
    pos += 2;
    return pos;
  }

  WINAPI_EXPORT uint16_t WINAPI MODBUS_get_common_event_counter_response_message_deserialize(uint8_t *bytes, modbus_get_common_event_counter_response_message_pt message)
  {
    uint16_t pos = 0;
    message->status = read_uint16_t_BE(&bytes[pos]);
    pos += 2;
    message->event_count = read_uint16_t_BE(&bytes[pos]);
    pos += 2;
    return pos;
  }

  WINAPI_EXPORT int WINAPI MODBUS_get_common_event_log_response_message_init(modbus_get_common_event_log_response_message_pt message)
  {
    message->count = 0;
    message->status = 0xffff;
    message->event_count = 0;
    message->message_count = 0;
    memset(message->events, 0, MODBUS_MAX_BUFFER_SIZE - 7);
    return MODBUS_OK;
  }

  WINAPI_EXPORT uint16_t WINAPI MODBUS_get_common_event_log_response_message_serialize(uint8_t *bytes, modbus_get_common_event_log_response_message_pt message)
  {
    uint16_t pos = 0;
    write_uint8_t_BE(message->count, &bytes[pos]);
    pos += 1;
    write_uint16_t_BE(message->status, &bytes[pos]);
    pos += 2;
    write_uint16_t_BE(message->event_count, &bytes[pos]);
    pos += 2;
    write_uint16_t_BE(message->message_count, &bytes[pos]);
    pos += 2;
    memcpy(&bytes[pos], message->events, message->count - 6);
    pos += message->count - 6;
    return pos;
  }

  WINAPI_EXPORT uint16_t WINAPI MODBUS_get_common_event_log_response_message_deserialize(uint8_t *bytes, modbus_get_common_event_log_response_message_pt message)
  {
    uint16_t pos = 0;
    message->count = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    message->status = read_uint16_t_BE(&bytes[pos]);
    pos += 2;
    message->event_count = read_uint16_t_BE(&bytes[pos]);
    pos += 2;
    message->message_count = read_uint16_t_BE(&bytes[pos]);
    pos += 2;
    memcpy(message->events, &bytes[pos], message->count - 6);
    pos += message->count - 6;
    return pos;
  }

  WINAPI_EXPORT int WINAPI MODBUS_write_multiple_coils_request_message_init(modbus_write_multiple_coils_request_message_pt message)
  {
    message->address = 0;
    message->quantity = 0;
    message->count = 0;
    memset(message->value, 0, MODBUS_MAX_BUFFER_SIZE - 5);
    return MODBUS_OK;
  }

  WINAPI_EXPORT uint16_t WINAPI MODBUS_write_multiple_coils_request_message_serialize(uint8_t *bytes, modbus_write_multiple_coils_request_message_pt message)
  {
    uint16_t pos = 0;
    write_uint16_t_BE(message->address, &bytes[pos]);
    pos += 2;
    write_uint16_t_BE(message->quantity, &bytes[pos]);
    pos += 2;
    write_uint8_t_BE(message->count, &bytes[pos]);
    pos += 1;
    memcpy(&bytes[pos], message->value, message->count);
    pos += message->count;
    return pos;
  }

  WINAPI_EXPORT uint16_t WINAPI MODBUS_write_multiple_coils_request_message_deserialize(uint8_t *bytes, modbus_write_multiple_coils_request_message_pt message)
  {
    uint16_t pos = 0;
    message->address = read_uint16_t_BE(&bytes[pos]);
    pos += 2;
    message->quantity = read_uint16_t_BE(&bytes[pos]);
    pos += 2;
    message->count = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    memcpy(message->value, &bytes[pos], message->count);
    pos += message->count;
    return pos;
  }

  WINAPI_EXPORT int WINAPI MODBUS_write_multiple_coils_response_message_init(modbus_write_multiple_coils_response_message_pt message)
  {
    message->address = 0;
    message->quantity = 0;
    return MODBUS_OK;
  }

  WINAPI_EXPORT uint16_t WINAPI MODBUS_write_multiple_coils_response_message_serialize(uint8_t *bytes, modbus_write_multiple_coils_response_message_pt message)
  {
    uint16_t pos = 0;
    write_uint8_t_BE(message->address, &bytes[pos]);
    pos += 1;
    write_uint16_t_BE(message->quantity, &bytes[pos]);
    pos += 2;
    return pos;
  }

  WINAPI_EXPORT uint16_t WINAPI MODBUS_write_multiple_coils_response_message_deserialize(uint8_t *bytes, modbus_write_multiple_coils_response_message_pt message)
  {
    uint16_t pos = 0;
    message->address = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    message->quantity = read_uint16_t_BE(&bytes[pos]);
    pos += 2;
    return pos;
  }

  WINAPI_EXPORT int WINAPI MODBUS_write_multiple_registers_request_message_init(modbus_write_multiple_registers_request_message_pt message)
  {
    message->address = 0;
    message->quantity = 0;
    message->count = 0;
    memset(message->value, 0, MODBUS_MAX_BUFFER_SIZE - 5);
    return MODBUS_OK;
  }

  WINAPI_EXPORT uint16_t WINAPI MODBUS_write_multiple_registers_request_message_serialize(uint8_t *bytes, modbus_write_multiple_registers_request_message_pt message)
  {
    uint16_t pos = 0;
    write_uint16_t_BE(message->address, &bytes[pos]);
    pos += 2;
    write_uint16_t_BE(message->quantity, &bytes[pos]);
    pos += 2;
    write_uint8_t_BE(message->count, &bytes[pos]);
    pos += 1;
    memcpy(&bytes[pos], message->value, message->count);
    pos += message->count;
    return pos;
  }

  WINAPI_EXPORT uint16_t WINAPI MODBUS_write_multiple_registers_request_message_deserialize(uint8_t *bytes, modbus_write_multiple_registers_request_message_pt message)
  {
    uint16_t pos = 0;
    message->address = read_uint16_t_BE(&bytes[pos]);
    pos += 2;
    message->quantity = read_uint16_t_BE(&bytes[pos]);
    pos += 2;
    message->count = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    memcpy(message->value, &bytes[pos], message->count);
    pos += message->count;
    return pos;
  }

  WINAPI_EXPORT int WINAPI MODBUS_write_multiple_registers_response_message_init(modbus_write_multiple_registers_response_message_pt message)
  {
    message->address = 0;
    message->quantity = 0;
    return MODBUS_OK;
  }

  WINAPI_EXPORT uint16_t WINAPI MODBUS_write_multiple_registers_response_message_serialize(uint8_t *bytes, modbus_write_multiple_registers_response_message_pt message)
  {
    uint16_t pos = 0;
    write_uint16_t_BE(message->address, &bytes[pos]);
    pos += 2;
    write_uint16_t_BE(message->quantity, &bytes[pos]);
    pos += 2;
    return pos;
  }

  WINAPI_EXPORT uint16_t WINAPI MODBUS_write_multiple_registers_response_message_deserialize(uint8_t *bytes, modbus_write_multiple_registers_response_message_pt message)
  {
    uint16_t pos = 0;
    message->address = read_uint16_t_BE(&bytes[pos]);
    pos += 2;
    message->quantity = read_uint16_t_BE(&bytes[pos]);
    pos += 2;
    return pos;
  }

  WINAPI_EXPORT int WINAPI MODBUS_report_server_id_response_message_init(modbus_report_server_id_response_message_pt message)
  {
    message->count = 0;
    message->server_id = 0;
    message->status = 0xFF;
    return MODBUS_OK;
  }

  WINAPI_EXPORT uint16_t WINAPI MODBUS_report_server_id_response_message_serialize(uint8_t *bytes, modbus_report_server_id_response_message_pt message)
  {
    uint16_t pos = 0;
    write_uint8_t_BE(message->count, &bytes[pos]);
    pos += 1;
    write_uint8_t_BE(message->server_id, &bytes[pos]);
    pos += 1;
    write_uint8_t_BE(message->status, &bytes[pos]);
    pos += 1;
    return pos;
  }

  WINAPI_EXPORT uint16_t WINAPI MODBUS_report_server_id_response_message_deserialize(uint8_t *bytes, modbus_report_server_id_response_message_pt message)
  {
    uint16_t pos = 0;
    message->count = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    message->server_id = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    message->status = read_uint8_t_BE(&bytes[pos]);
    pos += 1;
    return pos;
  }

  WINAPI_EXPORT int WINAPI MODBUS_IAP_check_message_init(modbus_IAP_check_message_pt message)
  {
    message->count = 0;
    message->crc32 = 0;
    return MODBUS_OK;
  }

  WINAPI_EXPORT uint16_t WINAPI MODBUS_IAP_check_message_serialize(uint8_t *bytes, modbus_IAP_check_message_pt message)
  {
    uint16_t pos = 0;
    write_uint32_t_BE(message->count, &bytes[pos]);
    pos += 4;
    write_uint32_t_BE(message->crc32, &bytes[pos]);
    pos += 4;
    return pos;
  }

  WINAPI_EXPORT uint16_t WINAPI MODBUS_IAP_check_message_deserialize(uint8_t *bytes, modbus_IAP_check_message_pt message)
  {
    uint16_t pos = 0;
    message->count = read_uint32_t_BE(&bytes[pos]);
    pos += 4;
    message->crc32 = read_uint32_t_BE(&bytes[pos]);
    pos += 4;
    return pos;
  }

  WINAPI_EXPORT int WINAPI MODBUS_IAP_request_message_init(modbus_IAP_request_message_pt message)
  {
    message->page = 0;
    memset(message->data, 0, 256);
    return MODBUS_OK;
  }

  WINAPI_EXPORT uint16_t WINAPI MODBUS_IAP_request_message_serialize(uint8_t *bytes, modbus_IAP_request_message_pt message)
  {
    uint16_t pos = 0;
    write_uint16_t_BE(message->page, &bytes[pos]);
    pos += 2;
    memcpy(&bytes[pos], message->data, 256);
    pos += 256;
    return pos;
  }

  WINAPI_EXPORT uint16_t WINAPI MODBUS_IAP_request_message_deserialize(uint8_t *bytes, modbus_IAP_request_message_pt message)
  {
    uint16_t pos = 0;
    message->page = read_uint16_t_BE(&bytes[pos]);
    pos += 2;
    memcpy(message->data, &bytes[pos], 256);
    pos += 256;
    return pos;
  }

  WINAPI_EXPORT int WINAPI MODBUS_IAP_response_message_init(modbus_IAP_response_message_pt message)
  {
    message->page = 0;
    return MODBUS_OK;
  }

  WINAPI_EXPORT uint16_t WINAPI MODBUS_IAP_response_message_serialize(uint8_t *bytes, modbus_IAP_response_message_pt message)
  {
    uint16_t pos = 0;
    write_uint16_t_BE(message->page, &bytes[pos]);
    pos += 2;
    return pos;
  }

  WINAPI_EXPORT uint16_t WINAPI MODBUS_IAP_response_message_deserialize(uint8_t *bytes, modbus_IAP_response_message_pt message)
  {
    uint16_t pos = 0;
    message->page = read_uint16_t_BE(&bytes[pos]);
    pos += 2;
    return pos;
  }
