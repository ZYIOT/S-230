#include "main.h"
#include "FreeRTOS.h"
#include "stream_Buffer.h"
#include "cmsis_os.h"
#include "bsp_usart.h"
#include "bsp_protocol.h"
#include "bsp_log.h"
#include "bsp_board_debug.h"
#include "app.h"
#include "hardware_wrapper.h"

#define GPRS_COMM_UART_HANDLE   (&huart1)
#define GPRS_COMM_RECV_BUFFER   (uart1_recv_buffer)

#define GPRS_CONFIG_UART_HANDLE (&huart6)
#define GPRS_CONFIG_RECV_BUFFER (uart6_recv_buffer)

#define BOARD_CONFIG_UART_HANDLE    (&huart5)
#define BOARD_CONFOG_RECV_BUFFER    (NULL)

#ifdef SHOW_PROTOCOL_DATA
    #define BSP_LOG_PROTOCOL(...)      BSP_LOG_debug(##__VA_ARGS__)
    #define BSP_LOG_PROTOCOL_HEX(...)  BSP_LOG_debug_hex(##__VA_ARGS__)
#else
    #define BSP_LOG_PROTOCOL(...)
    #define BSP_LOG_PROTOCOL_HEX(...)
#endif

#ifdef SHOW_GPRS_DATA
    #define BSP_LOG_GPRS(...)      BSP_LOG_debug(##__VA_ARGS__)
    #define BSP_LOG_GPRS_HEX(...)  BSP_LOG_debug_hex(##__VA_ARGS__)
#else
    #define BSP_LOG_GPRS(...)
    #define BSP_LOG_GPRS_HEX(...)
#endif



static HARDWARE_SEMAPHORE_TYPE_T protocol_sem;
static HARDWARE_SEMAPHORE_TYPE_T board_sem;

int BSP_PROTOCOL_init(void)
{
    protocol_sem = HARDWARE_CREATE_SEMAPHORE();
    board_sem = HARDWARE_CREATE_SEMAPHORE();
    G2_SERVER_init();
    return APP_OK;
}

extern void APP_NETWORK_AfterSendMessage(void);

int _pwrite(uint8_t buffer[G2_SERVER_PROTOCOL_BUFFER_SIZE], size_t len, size_t tick)
{
    BSP_LOG_PROTOCOL("Pw:len=%d, data=", len);
    BSP_LOG_PROTOCOL_HEX(buffer, len);
    BSP_LOG_PROTOCOL("\r\n");
    HARDWARE_TAKE_SEMAPHORE(protocol_sem);
    int rc = HAL_UART_Transmit(GPRS_COMM_UART_HANDLE, buffer, len, tick) == HAL_OK ? PROTOCOL_OK : PROTOCOL_ERROR;
    HARDWARE_GIVE_SEMAPHORE(protocol_sem);
    if (rc == PROTOCOL_OK)
    {
        APP_NETWORK_AfterSendMessage();
    }	
    return rc;
}

int _pread(uint8_t buffer[G2_SERVER_PROTOCOL_BUFFER_SIZE], size_t len, size_t tick)
{
    BSP_LOG_PROTOCOL("Pr:len=%d, data=", len);
    int index = 0, rc = 0;
	do
    {
        rc = xStreamBufferReceive(GPRS_COMM_RECV_BUFFER, &buffer[index], len - index, tick);
        G2_SERVER_CHECK_READ_RC_AND_RETURN
        BSP_LOG_PROTOCOL_HEX(&buffer[index], rc);
        index += rc;
    } while (index < len);
	BSP_LOG_PROTOCOL("\r\n");
    return index;
}

void _pclear(void)
{
    xStreamBufferReset(GPRS_COMM_RECV_BUFFER);
}

static size_t _plength(void)
{
    return xStreamBufferBytesAvailable(GPRS_COMM_RECV_BUFFER);
}

g2_server_port_t g2_server_port = {
    .read = _pread,
    .write = _pwrite,
    .clear = _pclear,
    .length = _plength
};

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


int _bwrite(uint8_t buffer[G2_SERVER_PROTOCOL_BUFFER_SIZE], size_t len, size_t tick)
{
    HARDWARE_TAKE_SEMAPHORE(board_sem);
    int rc = HAL_UART_Transmit(BOARD_CONFIG_UART_HANDLE, buffer, len, tick) == HAL_OK ? PROTOCOL_OK : PROTOCOL_ERROR;
    HARDWARE_GIVE_SEMAPHORE(board_sem);
    return rc;
}

// uart5 的 rx streambuffer 与 uart1 的共用 
g2_server_port_t pc_config_port = {
    .read = NULL,
    .write = _bwrite,
    .clear = NULL,
    .length = NULL
};

int BSP_PROTOCOL_pc_send_message(uint8_t buffer[G2_SERVER_PROTOCOL_BUFFER_SIZE], size_t len)
{
    pc_config_port.write(buffer, len, G2_SERVER_WRITE_TIMEOUT);
}


static HARDWARE_SEMAPHORE_TYPE_T gprs_config_sem;
int BSP_USER_PROTOCOL_init(void)
{
    gprs_config_sem = HARDWARE_CREATE_SEMAPHORE();
    USER_PROTOCOL_init();
    return APP_OK;
}

static int _gwrite(uint8_t buffer[USER_PROTOCOL_PROTOCOL_BUFFER_SIZE], size_t len, size_t tick)
{
    BSP_LOG_GPRS("Gw:len=%d, data=", len);
    BSP_LOG_GPRS_HEX(buffer, len);
    BSP_LOG_GPRS("\r\n");
    HARDWARE_TAKE_SEMAPHORE(gprs_config_sem);
    int rc = HAL_UART_Transmit(GPRS_CONFIG_UART_HANDLE, buffer, len, tick) == HAL_OK ? PROTOCOL_OK : PROTOCOL_ERROR;
    HARDWARE_GIVE_SEMAPHORE(gprs_config_sem);	
    return rc;
}

static int _gread(uint8_t buffer[USER_PROTOCOL_PROTOCOL_BUFFER_SIZE], size_t len, size_t tick)
{
    BSP_LOG_GPRS("Gr:len=%d, data=", len);
    int index = 0, rc = 0;
    do
    {
        rc = xStreamBufferReceive(GPRS_CONFIG_RECV_BUFFER, &buffer[index], len - index, tick);
        USER_PROTOCOL_CHECK_READ_RC_AND_RETURN
        BSP_LOG_GPRS_HEX(&buffer[index], rc);
        index += rc;
    } while (index < len);
	BSP_LOG_GPRS("\r\n");
    return index;
}

static void _gclear(void)
{
    xStreamBufferReset(GPRS_CONFIG_RECV_BUFFER);
}

static size_t _glength(void)
{
    return xStreamBufferBytesAvailable(GPRS_CONFIG_RECV_BUFFER);
}


user_protocol_port_t user_protocol_port = {
    .read = _gread,
    .write = _gwrite,
    .clear = _gclear,
    .length = _glength
};

