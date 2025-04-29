#ifndef __RS485_H
#define __RS485_H
#ifdef __cplusplus
extern "C"
{
#endif

#include "stdint.h"
#include "utils.h"
#include "rs485_config.h"
#include "modbus_master.h"
#include "hardware_wrapper.h"

#define RS485_BUFFER_MAX_SIZE 50
#define RS485_READ_TIMEOUT 1000
#define RS485_READ_IDLE 500
#define RS485_WRITE_IDLE 1000

#define RS485_OK 0
#define RS485_ERROR -1
#define RS485_READ_ERROR -2
#define RS485_PARAM_ERROR -3
#define RS485_CRC_ERROR -4

#define RS485_CODE_READ_REGISTER 0x03
#define RS485_CODE_READ_INPUT_REGISTER 0x04
#define RS485_CODE_WRITE_REGISTER 0x06

#define RS485_SENSOR_DO_CALIBRATE_SATURATED 2

#define RS485_SENSOR_PH_CALIBRATE_7 1
#define RS485_SENSOR_PH_CALIBRATE_4 2
#define RS485_SENSOR_PH_CALIBRATE_10 3
    // #include "rs485_wrapper.h"
#define RS485_CHECK_RC(rc) \
    if (rc != RS485_OK)    \
    {                      \
        return rc;         \
    }
    typedef int (*rs485_read_func)(uint8_t buffer[RS485_BUFFER_MAX_SIZE], size_t len, size_t tick);
    typedef int (*rs485_write_func)(uint8_t buffer[RS485_BUFFER_MAX_SIZE], size_t len, size_t tick);
    typedef int (*rs485_clear_func)(void);
    typedef int (*rs485_success_callback)(uint8_t buffer[RS485_BUFFER_MAX_SIZE], size_t len, void *ret, void *args);
    typedef int (*rs485_error_callback)(uint8_t error_code, void *ret, void *args);

    typedef struct
    {
        HARDWARE_SEMAPHORE_TYPE_T sem;
        modbus_port_pt port;
    } rs485_port_t, *rs485_port_pt;

    int RS485_init(rs485_port_pt rs485);
    int RS485_read_register(rs485_port_pt rs485, uint8_t id, uint16_t address, uint16_t quantity, void *ret, rs485_success_callback callback, rs485_error_callback error_callback, void *args);
    int RS485_write_register(rs485_port_pt rs485, uint8_t id, uint16_t address, uint16_t data, void *ret, rs485_success_callback callback, rs485_error_callback error_callback, void *args);
    int RS485_write_multiple_registers(rs485_port_pt rs485, uint8_t id, uint16_t addr, uint16_t quantity, uint8_t *value, void *ret, rs485_success_callback callback, rs485_error_callback error_callback, void *args);

#ifdef __cplusplus
}
#endif
#endif
