#include "bsp_protocol.h"
#include "bsp_log.h"
#include "app.h"
#include "hardware_wrapper.h"

static HARDWARE_SEMAPHORE_TYPE_T sem;
int BSP_PROTOCOL_init(void)
{
    sem = HARDWARE_CREATE_SEMAPHORE();
    G2_SERVER_init();
    return APP_OK;
}

extern void APP_NETWORK_after_send_message(void);

int _write(uint8_t buffer[G2_SERVER_PROTOCOL_BUFFER_SIZE], size_t len, size_t tick)
{
    BSP_LOG_trace("P write:  len=%d, data=", len);
    BSP_LOG_trace_hex(buffer, len);
    BSP_LOG_trace("\n");
    HARDWARE_TAKE_SEMAPHORE(sem);
    int rc = HAL_UART_Transmit(&huart1, buffer, len, tick) == HAL_OK ? PROTOCOL_OK : PROTOCOL_ERROR;
    HARDWARE_GIVE_SEMAPHORE(sem);
    if (rc == PROTOCOL_OK)
    {
        APP_NETWORK_after_send_message();
    }
    return rc;
}

int _read(uint8_t buffer[G2_SERVER_PROTOCOL_BUFFER_SIZE], size_t len, size_t tick)
{
    BSP_LOG_trace("P read:  len=%d, data=", len);
    int index = 0, rc = 0;
    do
    {
        rc = xStreamBufferReceive(uart1_recv_buffer, &buffer[index], len - index, tick);
        G2_SERVER_CHECK_READ_RC_AND_RETURN
        BSP_LOG_trace_hex(&buffer[index], rc);
        index += rc;
    } while (index < len);
    BSP_LOG_trace("\n");
    return index;
}

void _clear(void)
{
    xStreamBufferReset(uart1_recv_buffer);
}

static size_t _length(void)
{
    return xStreamBufferBytesAvailable(uart1_recv_buffer);
}

g2_server_port_t g2_server_port = {
    .read = _read,
    .write = _write,
    .clear = _clear,
    .length = _length};

int BSP_PROTOCOL_send_response(g2_server_packet_pt packet, uint8_t cmd_status)
{
    packet->cmd_status = cmd_status;
    packet->length = 0;
    return G2_SERVER_PACKET_write(&g2_server_port, packet);
}

int BSP_PROTOCOL_send_rc_response(g2_server_packet_pt packet, int rc)
{
    if (rc == APP_OK)
    {
        BSP_PROTOCOL_send_ok_response(packet);
    }
    else
    {
        BSP_PROTOCOL_send_error_response(packet);
    }
    return PROTOCOL_OK;
}

int BSP_PROTOCOL_send_ok_response(g2_server_packet_pt packet)
{
    return BSP_PROTOCOL_send_response(packet, BSP_PROTOCOL_GPRS_SUCCESS_ACK);
}

int BSP_PROTOCOL_send_error_response(g2_server_packet_pt packet)
{
    return BSP_PROTOCOL_send_response(packet, BSP_PROTOCOL_GPRS_ERROR_ACK);
}
