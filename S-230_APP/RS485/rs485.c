#include "rs485.h"

int RS485_init(rs485_port_pt port)
{
    MODBUS_init();
    port->sem = HARDWARE_CREATE_SEMAPHORE();
    return 0;
}

#define RS485_CHECK_READ_RC_AND_RETURN \
    if (rc <= 0)                       \
    {                                  \
        return rc;                     \
    }

#define RS485_CHECK_RC_AND_RETURN \
    if (rc != 0)                  \
    {                             \
        return rc;                \
    }

#define RS485_CHECK_ERROR                                       \
    if (packet.exception != 0)                                  \
    {                                                           \
        if (error_callback != NULL)                             \
        {                                                       \
            return error_callback(packet.buffer[0], ret, args); \
        }                                                       \
        return RS485_ERROR;                                     \
    }

#define RS485_OPERATION_DEFINE(modbus_read_message)       \
    modbus_packet_t packet;                               \
    packet.id = id;                                       \
    MODBUS_PACKET_init(&packet);                          \
    int rc = modbus_read_message(port, &packet, message); \
    RS485_CHECK_RC_AND_RETURN                             \
    rc = MODBUS_PACKET_read(port, &packet);               \
    RS485_CHECK_RC_AND_RETURN                             \
    RS485_CHECK_ERROR

static int _read_holding_registers(modbus_port_pt port, uint8_t id, modbus_read_holding_registers_request_message_pt message, void *ret, rs485_success_callback callback, rs485_error_callback error_callback, void *args)
{
    RS485_OPERATION_DEFINE(MODBUS_send_read_holding_registers_message)
    modbus_read_holding_registers_response_message_t pmsg;
    MODBUS_read_holding_registers_response_message_init(&pmsg);
    rc = MODBUS_read_holding_registers_response_message_deserialize(packet.buffer, &pmsg);
    MODBUS_CHECK_READ_RC_AND_RETURN
    packet.parsed = &pmsg;
    if (callback != NULL)
    {
        return callback(pmsg.buffer, pmsg.count, ret, args);
    }
    return RS485_OK;
}

static int _write_single_register(modbus_port_pt port, uint8_t id, modbus_write_single_register_message_pt message, void *ret, rs485_success_callback callback, rs485_error_callback error_callback, void *args)
{
    RS485_OPERATION_DEFINE(MODBUS_send_write_single_register_message)
    modbus_write_single_register_message_t pmsg;
    rc = MODBUS_write_single_register_message_deserialize(packet.buffer, &pmsg);
    MODBUS_CHECK_READ_RC_AND_RETURN
    packet.parsed = &pmsg;
    if (pmsg.address != message->address || pmsg.value != message->value)
    {
        return RS485_ERROR;
    }
    if (callback != NULL)
    {
        return callback(packet.buffer, packet.length, ret, args);
    }
    return RS485_OK;
}

static int _write_multiple_registers(modbus_port_pt port, uint8_t id, modbus_write_multiple_registers_request_message_pt message, void *ret, rs485_success_callback callback, rs485_error_callback error_callback, void *args)
{
    RS485_OPERATION_DEFINE(MODBUS_send_write_multiple_registers_message)
    modbus_write_multiple_registers_response_message_t pmsg;
    rc = MODBUS_write_multiple_registers_response_message_deserialize(packet.buffer, &pmsg);
    MODBUS_CHECK_READ_RC_AND_RETURN
    packet.parsed = &pmsg;
    if (pmsg.address != message->address || pmsg.quantity != message->quantity)
    {
        return RS485_ERROR;
    }
    if (callback != NULL)
    {
        return callback(packet.buffer, packet.length, ret, args);
    }
    return RS485_OK;
}

int RS485_read_register(rs485_port_pt rs485, uint8_t id, uint16_t address, uint16_t quantity, void *ret, rs485_success_callback callback, rs485_error_callback error_callback, void *args)
{
    modbus_read_holding_registers_request_message_t message;
    message.address = address;
    message.quantity = quantity;
    HARDWARE_TAKE_SEMAPHORE(rs485->sem);
    int rc = _read_holding_registers(rs485->port, id, &message, ret, callback, error_callback, args);
    HARDWARE_GIVE_SEMAPHORE(rs485->sem);
    return rc;
}

int RS485_write_register(rs485_port_pt rs485, uint8_t id, uint16_t addr, uint16_t data, void *ret, rs485_success_callback callback, rs485_error_callback error_callback, void *args)
{
    modbus_write_single_register_message_t message;
    message.address = addr;
    message.value = data;
    HARDWARE_TAKE_SEMAPHORE(rs485->sem);
    int rc = _write_single_register(rs485->port, id, &message, ret, callback, error_callback, args);
    HARDWARE_GIVE_SEMAPHORE(rs485->sem);
    return rc;
}

int RS485_write_multiple_registers(rs485_port_pt rs485, uint8_t id, uint16_t addr, uint16_t quantity, uint8_t *value, void *ret, rs485_success_callback callback, rs485_error_callback error_callback, void *args)
{
    modbus_write_multiple_registers_request_message_t message;
    MODBUS_write_multiple_registers_request_message_init(&message);
    message.address = addr;
    message.quantity = quantity;
    message.count = quantity * 2;
    memcpy(message.value, value, message.count);
    HARDWARE_TAKE_SEMAPHORE(rs485->sem);
    int rc = _write_multiple_registers(rs485->port, id, &message, ret, callback, error_callback, args);
    HARDWARE_GIVE_SEMAPHORE(rs485->sem);
    return rc;
}
