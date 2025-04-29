#include "app.h"
#include "bsp_rtc.h"
#include "app_log.h"
#include "app_protocol.h"
#include "app_config.h"
#include "hardware_wrapper.h"
#include "utils.h"
#include "datetime_utils.h"
#include "config_bit.h"
#include "app_config/defines.h"

#define CHECK_PROTOCOL_MESSAGE                    \
    if (packet->parsed == NULL)                   \
    {                                             \
        BSP_PROTOCOL_send_error_response(packet); \
        return PROTOCOL_ERROR;                    \
    }

#define CHECK_PROTOCOL_PROBE_ID_MESSAGE(probe_id) \
    if (probe_id > PROBE_SIZE || probe_id < 1)    \
    {                                             \
        BSP_PROTOCOL_send_error_response(packet); \
        return PROTOCOL_ERROR;                    \
    }

#define CHECK_PROTOCOL_RELAY_ID_MESSAGE(relay_id) \
    if (relay_id > RELAY_SIZE || relay_id < 1)    \
    {                                             \
        BSP_PROTOCOL_send_error_response(packet); \
        return PROTOCOL_ERROR;                    \
    }

#define CHECK_PROTOCOL_CHECK_WRITE_MESSAGE(oper)  \
    if (oper != 0x51)                             \
    {                                             \
        BSP_PROTOCOL_send_error_response(packet); \
        return PROTOCOL_ERROR;                    \
    }

#define CHECK_PROTOCOL_CHECK_DELETE_MESSAGE(oper) \
    if (oper != 0x52)                             \
    {                                             \
        BSP_PROTOCOL_send_error_response(packet); \
        return PROTOCOL_ERROR;                    \
    }

#define CHECK_CONFIG_MESSAGE_RC(rc)               \
    if (rc != APP_OK)                             \
    {                                             \
        BSP_PROTOCOL_send_error_response(packet); \
        return PROTOCOL_ERROR;                    \
    }

#define CHECK_PROTOCOL_PROBE_SENSOR_MESSAGE(indicator_id)     \
    if (indicator_id > PROBE_SENSOR_SIZE || indicator_id < 1) \
    {                                                         \
        BSP_PROTOCOL_send_error_response(packet);             \
        return PROTOCOL_ERROR;                                \
    }


#define CHECK_PROTOCOL_INDICATOR_ID_MESSAGE(indicator_id)     \
    if (indicator_id > INDICATOR_SIZE || indicator_id < 1)    \
    {                                                         \
        BSP_PROTOCOL_send_error_response(packet);             \
        return PROTOCOL_ERROR;                                \
    }

#define CHECK_PROTOCOL_ID_RANGE_MESSAGE(min, max) \
    if (indicator_id > max || min < 1)            \
    {                                             \
        BSP_PROTOCOL_send_error_response(packet); \
        return PROTOCOL_ERROR;                    \
    }

#define SEND_ERROR_MESSAGE()                      \
    do                                            \
    {                                             \
        BSP_PROTOCOL_send_error_response(packet); \
        return PROTOCOL_ERROR;                    \
    } while (0)

#include "app_protocol/network.h"
#include "app_protocol/device.h"
#include "app_protocol/sensor.h"
#include "app_protocol/relay.h"
#include "app_protocol/pump.h"
#include "app_protocol/unused.h"
#include "app_protocol/l6.h"

int APP_PROTOCOL_init(void)
{
    return APP_OK;
}

void APP_PROTOCOL_task_run(void *argument)
{
    g2_server_packet_t packet;
    int rc = 0;
    uint32_t recvCnt = 0;
    BSP_PROTOCOL_init();
    for (;;)
    {
        G2_SERVER_PACKET_init(&packet);
        rc = BSP_PROTOCOL_PACKET_read(&packet);
        if (rc == PROTOCOL_OK)
        {
            recvCnt += 1;
            APP_LOG_debug("PROTOCOL recv cnt = %d\r\n", recvCnt);
            if (G2_SERVER_receive_packet(&packet) == PROTOCOL_NOT_SUPPORT)
            {
                BSP_PROTOCOL_send_error_response(&packet);
            }
        }
        else
        {
            HARDWARE_OS_DELAY_MS(100);
        }
    }
}
