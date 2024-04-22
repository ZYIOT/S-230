#ifndef __MODBUS_MESSAGE_H
#define __MODBUS_MESSAGE_H
#include "modbus_common.h"
#include "modbus_packet.h"

#ifdef __cplusplus
extern "C"
{
#endif

  typedef struct
  {
    uint8_t status; 
  } modbus_error_response_message_t, *modbus_error_response_message_pt;

  WINAPI_EXPORT int WINAPI MODBUS_error_response_message_init(modbus_error_response_message_pt message);
  WINAPI_EXPORT uint16_t WINAPI MODBUS_error_response_message_serialize(uint8_t *bytes, modbus_error_response_message_pt message);
  WINAPI_EXPORT uint16_t WINAPI MODBUS_error_response_message_deserialize(uint8_t *bytes, modbus_error_response_message_pt message);

  typedef struct
  {
    uint16_t address; 
    uint16_t quantity; 
  } modbus_read_coils_request_message_t, *modbus_read_coils_request_message_pt;

  WINAPI_EXPORT int WINAPI MODBUS_read_coils_request_message_init(modbus_read_coils_request_message_pt message);
  WINAPI_EXPORT uint16_t WINAPI MODBUS_read_coils_request_message_serialize(uint8_t *bytes, modbus_read_coils_request_message_pt message);
  WINAPI_EXPORT uint16_t WINAPI MODBUS_read_coils_request_message_deserialize(uint8_t *bytes, modbus_read_coils_request_message_pt message);

  typedef struct
  {
    uint8_t count;                         
    uint8_t buffer[MODBUS_MAX_BUFFER_SIZE];
  } modbus_read_coils_response_message_t, *modbus_read_coils_response_message_pt;

  WINAPI_EXPORT int WINAPI MODBUS_read_coils_response_message_init(modbus_read_coils_response_message_pt message);
  WINAPI_EXPORT uint16_t WINAPI MODBUS_read_coils_response_message_serialize(uint8_t *bytes, modbus_read_coils_response_message_pt message);
  WINAPI_EXPORT uint16_t WINAPI MODBUS_read_coils_response_message_deserialize(uint8_t *bytes, modbus_read_coils_response_message_pt message);

  typedef struct
  {
    uint16_t address;  
    uint16_t quantity; 
  } modbus_read_discrete_inputs_request_message_t, *modbus_read_discrete_inputs_request_message_pt;

  WINAPI_EXPORT int WINAPI MODBUS_read_discrete_inputs_request_message_init(modbus_read_discrete_inputs_request_message_pt message);
  WINAPI_EXPORT uint16_t WINAPI MODBUS_read_discrete_inputs_request_message_serialize(uint8_t *bytes, modbus_read_discrete_inputs_request_message_pt message);
  WINAPI_EXPORT uint16_t WINAPI MODBUS_read_discrete_inputs_request_message_deserialize(uint8_t *bytes, modbus_read_discrete_inputs_request_message_pt message);

  typedef struct
  {
    uint8_t count;                          
    uint8_t buffer[MODBUS_MAX_BUFFER_SIZE]; 
  } modbus_read_discrete_inputs_response_message_t, *modbus_read_discrete_inputs_response_message_pt;

  WINAPI_EXPORT int WINAPI MODBUS_read_discrete_inputs_response_message_init(modbus_read_discrete_inputs_response_message_pt message);
  WINAPI_EXPORT uint16_t WINAPI MODBUS_read_discrete_inputs_response_message_serialize(uint8_t *bytes, modbus_read_discrete_inputs_response_message_pt message);
  WINAPI_EXPORT uint16_t WINAPI MODBUS_read_discrete_inputs_response_message_deserialize(uint8_t *bytes, modbus_read_discrete_inputs_response_message_pt message);

  typedef struct
  {
    uint16_t address;  
    uint16_t quantity; 
  } modbus_read_holding_registers_request_message_t, *modbus_read_holding_registers_request_message_pt;

  WINAPI_EXPORT int WINAPI MODBUS_read_holding_registers_request_message_init(modbus_read_holding_registers_request_message_pt message);
  WINAPI_EXPORT uint16_t WINAPI MODBUS_read_holding_registers_request_message_serialize(uint8_t *bytes, modbus_read_holding_registers_request_message_pt message);
  WINAPI_EXPORT uint16_t WINAPI MODBUS_read_holding_registers_request_message_deserialize(uint8_t *bytes, modbus_read_holding_registers_request_message_pt message);

  typedef struct
  {
    uint8_t count;                          
    uint8_t buffer[MODBUS_MAX_BUFFER_SIZE]; 
  } modbus_read_holding_registers_response_message_t, *modbus_read_holding_registers_response_message_pt;

  WINAPI_EXPORT int WINAPI MODBUS_read_holding_registers_response_message_init(modbus_read_holding_registers_response_message_pt message);
  WINAPI_EXPORT uint16_t WINAPI MODBUS_read_holding_registers_response_message_serialize(uint8_t *bytes, modbus_read_holding_registers_response_message_pt message);
  WINAPI_EXPORT uint16_t WINAPI MODBUS_read_holding_registers_response_message_deserialize(uint8_t *bytes, modbus_read_holding_registers_response_message_pt message);

  typedef struct
  {
    uint16_t address;  
    uint16_t quantity; 
  } modbus_read_input_registers_request_message_t, *modbus_read_input_registers_request_message_pt;

  WINAPI_EXPORT int WINAPI MODBUS_read_input_registers_request_message_init(modbus_read_input_registers_request_message_pt message);
  WINAPI_EXPORT uint16_t WINAPI MODBUS_read_input_registers_request_message_serialize(uint8_t *bytes, modbus_read_input_registers_request_message_pt message);
  WINAPI_EXPORT uint16_t WINAPI MODBUS_read_input_registers_request_message_deserialize(uint8_t *bytes, modbus_read_input_registers_request_message_pt message);

  typedef struct
  {
    uint8_t count;                          
    uint8_t buffer[MODBUS_MAX_BUFFER_SIZE]; 
  } modbus_read_input_registers_response_message_t, *modbus_read_input_registers_response_message_pt;

  WINAPI_EXPORT int WINAPI MODBUS_read_input_registers_response_message_init(modbus_read_input_registers_response_message_pt message);
  WINAPI_EXPORT uint16_t WINAPI MODBUS_read_input_registers_response_message_serialize(uint8_t *bytes, modbus_read_input_registers_response_message_pt message);
  WINAPI_EXPORT uint16_t WINAPI MODBUS_read_input_registers_response_message_deserialize(uint8_t *bytes, modbus_read_input_registers_response_message_pt message);

  typedef struct
  {
    uint16_t address; 
    uint16_t value;   
  } modbus_write_single_coil_message_t, *modbus_write_single_coil_message_pt;

  WINAPI_EXPORT int WINAPI MODBUS_write_single_coil_message_init(modbus_write_single_coil_message_pt message);
  WINAPI_EXPORT uint16_t WINAPI MODBUS_write_single_coil_message_serialize(uint8_t *bytes, modbus_write_single_coil_message_pt message);
  WINAPI_EXPORT uint16_t WINAPI MODBUS_write_single_coil_message_deserialize(uint8_t *bytes, modbus_write_single_coil_message_pt message);

  typedef struct
  {
    uint16_t address; 
    uint16_t value;  
  } modbus_write_single_register_message_t, *modbus_write_single_register_message_pt;

  WINAPI_EXPORT int WINAPI MODBUS_write_single_register_message_init(modbus_write_single_register_message_pt message);
  WINAPI_EXPORT uint16_t WINAPI MODBUS_write_single_register_message_serialize(uint8_t *bytes, modbus_write_single_register_message_pt message);
  WINAPI_EXPORT uint16_t WINAPI MODBUS_write_single_register_message_deserialize(uint8_t *bytes, modbus_write_single_register_message_pt message);

  typedef struct
  {
    uint8_t status; 
  } modbus_read_exception_status_response_message_t, *modbus_read_exception_status_response_message_pt;

  WINAPI_EXPORT int WINAPI MODBUS_read_exception_status_response_message_init(modbus_read_exception_status_response_message_pt message);
  WINAPI_EXPORT uint16_t WINAPI MODBUS_read_exception_status_response_message_serialize(uint8_t *bytes, modbus_read_exception_status_response_message_pt message);
  WINAPI_EXPORT uint16_t WINAPI MODBUS_read_exception_status_response_message_deserialize(uint8_t *bytes, modbus_read_exception_status_response_message_pt message);

  typedef struct
  {
    uint16_t sub_function; 
    uint16_t data;        
  } modbus_diagnostics_message_t, *modbus_diagnostics_message_pt;

  WINAPI_EXPORT int WINAPI MODBUS_diagnostics_message_init(modbus_diagnostics_message_pt message);
  WINAPI_EXPORT uint16_t WINAPI MODBUS_diagnostics_message_serialize(uint8_t *bytes, modbus_diagnostics_message_pt message);
  WINAPI_EXPORT uint16_t WINAPI MODBUS_diagnostics_message_deserialize(uint8_t *bytes, modbus_diagnostics_message_pt message);

  typedef struct
  {
    uint16_t status; 
    uint16_t event_count; 
  } modbus_get_common_event_counter_response_message_t, *modbus_get_common_event_counter_response_message_pt;

  WINAPI_EXPORT int WINAPI MODBUS_get_common_event_counter_response_message_init(modbus_get_common_event_counter_response_message_pt message);
  WINAPI_EXPORT uint16_t WINAPI MODBUS_get_common_event_counter_response_message_serialize(uint8_t *bytes, modbus_get_common_event_counter_response_message_pt message);
  WINAPI_EXPORT uint16_t WINAPI MODBUS_get_common_event_counter_response_message_deserialize(uint8_t *bytes, modbus_get_common_event_counter_response_message_pt message);

  typedef struct
  {
    uint8_t count;                             
    uint16_t status;                          
    uint16_t event_count;                   
    uint16_t message_count;                 
    uint8_t events[MODBUS_MAX_BUFFER_SIZE - 7];
  } modbus_get_common_event_log_response_message_t, *modbus_get_common_event_log_response_message_pt;

  WINAPI_EXPORT int WINAPI MODBUS_get_common_event_log_response_message_init(modbus_get_common_event_log_response_message_pt message);
  WINAPI_EXPORT uint16_t WINAPI MODBUS_get_common_event_log_response_message_serialize(uint8_t *bytes, modbus_get_common_event_log_response_message_pt message);
  WINAPI_EXPORT uint16_t WINAPI MODBUS_get_common_event_log_response_message_deserialize(uint8_t *bytes, modbus_get_common_event_log_response_message_pt message);

  typedef struct
  {
    uint16_t address;                       
    uint16_t quantity;                  
    uint8_t count;                       
    uint8_t value[MODBUS_MAX_BUFFER_SIZE - 5]; 
  } modbus_write_multiple_coils_request_message_t, *modbus_write_multiple_coils_request_message_pt;

  WINAPI_EXPORT int WINAPI MODBUS_write_multiple_coils_request_message_init(modbus_write_multiple_coils_request_message_pt message);
  WINAPI_EXPORT uint16_t WINAPI MODBUS_write_multiple_coils_request_message_serialize(uint8_t *bytes, modbus_write_multiple_coils_request_message_pt message);
  WINAPI_EXPORT uint16_t WINAPI MODBUS_write_multiple_coils_request_message_deserialize(uint8_t *bytes, modbus_write_multiple_coils_request_message_pt message);

  typedef struct
  {
    uint8_t address;  
    uint16_t quantity; 
  } modbus_write_multiple_coils_response_message_t, *modbus_write_multiple_coils_response_message_pt;

  WINAPI_EXPORT int WINAPI MODBUS_write_multiple_coils_response_message_init(modbus_write_multiple_coils_response_message_pt message);
  WINAPI_EXPORT uint16_t WINAPI MODBUS_write_multiple_coils_response_message_serialize(uint8_t *bytes, modbus_write_multiple_coils_response_message_pt message);
  WINAPI_EXPORT uint16_t WINAPI MODBUS_write_multiple_coils_response_message_deserialize(uint8_t *bytes, modbus_write_multiple_coils_response_message_pt message);

  typedef struct
  {
    uint16_t address;                         
    uint16_t quantity;                   
    uint8_t count;                         
    uint8_t value[MODBUS_MAX_BUFFER_SIZE - 5]; 
  } modbus_write_multiple_registers_request_message_t, *modbus_write_multiple_registers_request_message_pt;

  WINAPI_EXPORT int WINAPI MODBUS_write_multiple_registers_request_message_init(modbus_write_multiple_registers_request_message_pt message);
  WINAPI_EXPORT uint16_t WINAPI MODBUS_write_multiple_registers_request_message_serialize(uint8_t *bytes, modbus_write_multiple_registers_request_message_pt message);
  WINAPI_EXPORT uint16_t WINAPI MODBUS_write_multiple_registers_request_message_deserialize(uint8_t *bytes, modbus_write_multiple_registers_request_message_pt message);

  typedef struct
  {
    uint16_t address; 
    uint16_t quantity; 
  } modbus_write_multiple_registers_response_message_t, *modbus_write_multiple_registers_response_message_pt;

  WINAPI_EXPORT int WINAPI MODBUS_write_multiple_registers_response_message_init(modbus_write_multiple_registers_response_message_pt message);
  WINAPI_EXPORT uint16_t WINAPI MODBUS_write_multiple_registers_response_message_serialize(uint8_t *bytes, modbus_write_multiple_registers_response_message_pt message);
  WINAPI_EXPORT uint16_t WINAPI MODBUS_write_multiple_registers_response_message_deserialize(uint8_t *bytes, modbus_write_multiple_registers_response_message_pt message);

  typedef struct
  {
    uint8_t count; 
    uint8_t server_id; 
    uint8_t status; 
  } modbus_report_server_id_response_message_t, *modbus_report_server_id_response_message_pt;

  WINAPI_EXPORT int WINAPI MODBUS_report_server_id_response_message_init(modbus_report_server_id_response_message_pt message);
  WINAPI_EXPORT uint16_t WINAPI MODBUS_report_server_id_response_message_serialize(uint8_t *bytes, modbus_report_server_id_response_message_pt message);
  WINAPI_EXPORT uint16_t WINAPI MODBUS_report_server_id_response_message_deserialize(uint8_t *bytes, modbus_report_server_id_response_message_pt message);

  typedef struct
  {
    uint32_t count;
    uint32_t crc32; 
  } modbus_IAP_check_message_t, *modbus_IAP_check_message_pt;

  WINAPI_EXPORT int WINAPI MODBUS_IAP_check_message_init(modbus_IAP_check_message_pt message);
  WINAPI_EXPORT uint16_t WINAPI MODBUS_IAP_check_message_serialize(uint8_t *bytes, modbus_IAP_check_message_pt message);
  WINAPI_EXPORT uint16_t WINAPI MODBUS_IAP_check_message_deserialize(uint8_t *bytes, modbus_IAP_check_message_pt message);

  typedef struct
  {
    uint16_t page;    
    uint8_t data[256];
  } modbus_IAP_request_message_t, *modbus_IAP_request_message_pt;

  WINAPI_EXPORT int WINAPI MODBUS_IAP_request_message_init(modbus_IAP_request_message_pt message);
  WINAPI_EXPORT uint16_t WINAPI MODBUS_IAP_request_message_serialize(uint8_t *bytes, modbus_IAP_request_message_pt message);
  WINAPI_EXPORT uint16_t WINAPI MODBUS_IAP_request_message_deserialize(uint8_t *bytes, modbus_IAP_request_message_pt message);

  typedef struct
  {
    uint16_t page;
  } modbus_IAP_response_message_t, *modbus_IAP_response_message_pt;

  WINAPI_EXPORT int WINAPI MODBUS_IAP_response_message_init(modbus_IAP_response_message_pt message);
  WINAPI_EXPORT uint16_t WINAPI MODBUS_IAP_response_message_serialize(uint8_t *bytes, modbus_IAP_response_message_pt message);
  WINAPI_EXPORT uint16_t WINAPI MODBUS_IAP_response_message_deserialize(uint8_t *bytes, modbus_IAP_response_message_pt message);

#ifdef __cplusplus
}
#endif
#endif
