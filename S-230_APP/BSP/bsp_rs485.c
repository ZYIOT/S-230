#include "bsp_rs485.h"
#include "bsp_log.h"
#include "bsp_board_debug.h"

#define RS485_UART_HANDLE   (&huart3)
#define RS485_RECV_BUFFER   (uart3_recv_buffer)
#define RS485_OUTPUT    (HARDWARE_GPIO_WRITE_PIN_L(RS485_WRITE_GPIO_Port, RS485_WRITE_Pin))
#define RS485_READIN    (HARDWARE_GPIO_WRITE_PIN_H(RS485_WRITE_GPIO_Port, RS485_WRITE_Pin))

#ifdef SHOW_RS485_DATA
    #define BSP_LOG_RS485(...)      BSP_LOG_debug(##__VA_ARGS__)
    #define BSP_LOG_RS485_HEX(...)  BSP_LOG_debug_hex(##__VA_ARGS__)
#else
    #define BSP_LOG_RS485(...)
    #define BSP_LOG_RS485_HEX(...)
#endif

static void _rdir_init(void)
{
    RS485_READIN;
}


#define __RS485_CHECK_READ_RC_AND_RETURN \
    if (rc <= 0)                         \
    {                                    \
        return RS485_READ_ERROR;         \
    }


static int _rread(uint8_t buffer[MODBUS_MODBUS_BUFFER_SIZE], size_t len, size_t tick)
{
    int index = 0, rc = 0;
    BSP_LOG_RS485("Rr:len=%d, data=", len);
    do
    {
        rc = xStreamBufferReceive(RS485_RECV_BUFFER, &buffer[index], len - index, tick);
        __RS485_CHECK_READ_RC_AND_RETURN
        BSP_LOG_RS485_HEX(&buffer[index], rc);
        index += rc;
    } while (index < len);
    BSP_LOG_RS485("\r\n");
    return rc;
}

static int _rwrite(uint8_t buffer[MODBUS_MODBUS_BUFFER_SIZE], size_t len, size_t tick)
{
    BSP_LOG_RS485("Rw:len=%d, data=", len);
    BSP_LOG_RS485_HEX(buffer, len);
    BSP_LOG_RS485("\r\n");
    RS485_OUTPUT;
    HAL_UART_Transmit(RS485_UART_HANDLE, buffer, len, tick);
    HARDWARE_HAL_DELAY_US(20);
    RS485_READIN;
    return len;
}

static void _clear(void)
{
    xStreamBufferReset(RS485_RECV_BUFFER);
}

static size_t _length(void)
{
    return xStreamBufferBytesAvailable(RS485_RECV_BUFFER);
}

static modbus_port_t modbus_port = {
    .read = _rread,
    .write = _rwrite,
    .clear = _clear,
    .length = _length};

rs485_port_t rs485_port;

int BSP_RS485_init(void)
{
    rs485_port.port = &modbus_port;
    _rdir_init();
    RS485_init(&rs485_port);
    RS485_DRIVER_init();
    rs485_port.port->clear();
    return 0;
}
