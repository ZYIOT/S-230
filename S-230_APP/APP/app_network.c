#include "utils.h"
#include "app.h"
#include "bsp_link.h"
#include "bsp_rtc.h"
#include "bsp_wdg.h"
#include "app_log.h"
#include "app_network.h"
#include "app_protocol.h"
#include "hardware_wrapper.h"

app_network_t app_network = {0};

#define CAN_CONNECT_TO_SERVER()      \
    APP_NETWORK_CONNECTED() &&       \
        APP_SERVER_DISCONNECTED() && \
        (app_network.connect_tick == 0 || (HARDWARE_GET_TICK() - app_network.connect_tick > 15000))

static void _app_network_reset(void)
{
    app_network.csq = CSQ_MIN_VALUE;
    app_network.connected_status = CONNECTED_STATUS_DISCONNECTED;
    app_network.network_status = CONNECTED_STATUS_DISCONNECTED;
    app_network.connect_tick = 0;
    app_network.last_send_tick = 0;
    SET_APP_SERVER_ONCE_CONNECTED(CONNECTED_STATUS_DISCONNECTED);
}

static void _on_disconnect(void)
{
    if (APP_NETWORK_CONNECTED())
    {
        _app_network_reset();
    }
}

void APP_NETWORK_after_send_message(void)
{
    // app_network.last_send_tick = HARDWARE_GET_TICK();
}

static void _on_connect(void)
{
    app_network.network_status = CONNECTED_STATUS_CONNECTED;
}

void APP_NETWORK_on_connect_to_server(void)
{
    app_network.connected_status = CONNECTED_STATUS_CONNECTED;
    app_network.was_connected_status = CONNECTED_STATUS_CONNECTED;
    app_network.connect_tick = 0;
    if (app_config_device_connect.reason != 1)
    {
        app_config_device_connect.reason = 1;
        APP_CONFIG_device_connect_write(&app_config_device_connect);
    }
    SET_APP_SERVER_ONCE_CONNECTED(CONNECTED_STATUS_CONNECTED);
}

static void request_time_from_server(void)
{
    if (APP_SERVER_CONNECTED())
    {
        g2_server_packet_t packet;
        G2_SERVER_PACKET_init(&packet);
        BSP_PROTOCOL_send_read_get_time_message(&packet);
    }
}

static datetime_t check_rtc_time = {0};
static uint32_t refresh_rtc_day = 0;
static int check_rtc_works(void)
{
    static uint32_t tick = 0;
    static uint32_t use_hse = 0;
    static uint32_t use_hse_tick = 0;
    uint8_t week;
    datetime_t time = {0};
    uint32_t tick_now = HARDWARE_GET_TICK();
    int rc = 0;
    if (tick_now - tick > 1000 * 60)
    {
        tick = tick_now;
        BSP_RTC_get(&time.year, &time.month, &time.day, &time.hour, &time.minute, &time.second, &week);
        if (time.year < 20)
        {
            refresh_rtc_day = 0xffff; //更新时间
        }
        if (datetime_equals(&check_rtc_time, &time))
        {
            rc = BSP_RTC_use_HSE();
            APP_CHECK_RC(rc);
            rc = BSP_RTC_set(time.year, time.month, time.day, time.hour, time.minute, time.second);
            APP_CHECK_RC(rc);
            use_hse = 1;
            use_hse_tick = tick_now;
            BSP_RTC_save_to_backup(&time);
            refresh_rtc_day = 0xffff; //更新时间
        }
        else
        {
            datetime_copy(&check_rtc_time, &time);
        }
        uint32_t refresh_rtc = tick_now / 86400000; //24 hours
        if (refresh_rtc != refresh_rtc_day && APP_SERVER_CONNECTED())
        {
            refresh_rtc_day = refresh_rtc;
            request_time_from_server();
        }
        if (use_hse == 1 && time.year >= 20 && use_hse_tick - tick_now > 5 * 60 * 1000)
        {
            use_hse_tick = tick_now;
            BSP_RTC_save_to_backup(&time);
        }
    }
    return APP_OK;
}

static void connect_to_server(void)
{
    if (CAN_CONNECT_TO_SERVER())
    {
        g2_server_packet_t packet;
        G2_SERVER_PACKET_init(&packet);
        g2_server_connect_message_t message = {.reason = app_config_device_connect.reason};
        if (message.reason == 0)
        {
            message.reason = 1;
        }
        if (app_network.was_connected_status == CONNECTED_STATUS_CONNECTED)
        {
            message.reason = 2;
        }
        BSP_PROTOCOL_send_read_connect_message(&packet, &message);
        app_network.connect_tick = HARDWARE_GET_TICK(); 
    }
}

static void send_heartbeat_to_server(void)
{
    if (APP_SERVER_CONNECTED() && HARDWARE_GET_TICK() - app_network.last_send_tick > 120 * 1000)
    {
        app_network.last_send_tick = HARDWARE_GET_TICK();
        g2_server_packet_t packet;
        G2_SERVER_PACKET_init(&packet);
        BSP_PROTOCOL_send_read_heartbeat_message(&packet);
    }
}


#if defined(SUPPORT_TEMPERATURE_AND_HUMIDITY) || defined(SUPPORT_CHECK_POWER_VOL_IN) || defined(SUPPORT_CHECK_POWER_CUR_OUT)
#include "app_environment.h"
static void send_environment_to_server(void)
{
    g2_server_packet_t packet;
    G2_SERVER_PACKET_init(&packet);
    g2_server_environment_message_t message;
    G2_SERVER_environment_message_init(&message);
    APP_ENVIRONMENT_pack(&message);
    BSP_PROTOCOL_send_read_environment_message(&packet, &message);
}
#endif

#ifdef SUPPORT_L6
#include "app_l6.h"
int send_l6_status_to_server(void)
{
    g2_server_packet_t packet;
    G2_SERVER_PACKET_init(&packet);
    g2_server_l6_status_message_t message;
    G2_SERVER_l6_status_message_init(&message);
    message.probe_id = 1;
    APP_L6_status_pack(&message);
    BSP_PROTOCOL_send_read_l6_status_message(&packet, &message);
    return PROTOCOL_OK;
}
#endif


void APP_NETWORK_update_environment_to_server(g2_server_packet_pt packet)
{
#if defined(SUPPORT_TEMPERATURE_AND_HUMIDITY) || defined(SUPPORT_CHECK_POWER_VOL_IN) || defined(SUPPORT_CHECK_POWER_CUR_OUT)
        APP_ENVIRONMENT_read();
        HARDWARE_OS_DELAY_MS(100);
        G2_SERVER_PACKET_init(packet);
        g2_server_environment_message_t message;
        G2_SERVER_environment_message_init(&message);
        APP_ENVIRONMENT_pack(&message);
        BSP_PROTOCOL_send_read_environment_message(packet, &message);
#endif
}

static int send_gprs_signal_to_server(void)
{
    if((CSQ_MIN_VALUE == app_network.csq) || (CSQ_MAX_VALUE < app_network.csq))
    {
        return APP_ERROR;
    }
    g2_server_packet_t packet;
    G2_SERVER_PACKET_init(&packet);
    g2_server_gprs_signal_message_t pmsg = {0};
    pmsg.rssi = app_network.csq;
    BSP_PROTOCOL_send_read_gprs_signal_message(&packet, &pmsg);
    return APP_OK;
}

#define SEND_CSQ_INTERVAL   (30)
static void send_csq_to_server(void)
{
    static size_t send_csq_tick = 0;
    static uint8_t csq_connected_server_flag = 0;
    if(CONNECTED_STATUS_CONNECTED == GET_APP_SERVER_ONCE_CONNECTED())
    {
        if(0x0 == csq_connected_server_flag)
        {
            send_csq_tick = HARDWARE_GET_TICK();
        }
        csq_connected_server_flag |= 0x1;
    }
    else
    {
        csq_connected_server_flag = 0x0;
    }
    if((APP_SERVER_CONNECTED() && (HARDWARE_GET_TICK() - send_csq_tick) >= SEND_CSQ_INTERVAL * 60 * 1000)
     || ((0x1 == csq_connected_server_flag) && (HARDWARE_GET_TICK() - send_csq_tick >= 10 * 1000)))
    {
        if(APP_OK == send_gprs_signal_to_server())
        {
            csq_connected_server_flag |= 0x2;
            send_csq_tick = HARDWARE_GET_TICK();
        }
    }
}

extern app_config_diagnosis_message_t diagnosis_msg;
static void check_and_send_diagnosis_message(void)
{
    app_config_time_t time;
    uint8_t week = 0;
    app_config_time_pt msg_time = &(diagnosis_msg.msg_time);
    BSP_RTC_get(&time.year, &time.month, &time.day, &time.hour, &time.minute, &time.second, &week);
    if(msg_time->year > 0)  // 检查自诊断信息是否有效 
    {
        // 若不在同一天，则清除自诊断信息 
        if(time.day != msg_time->day) 
        {
            APP_CONFIG_diagnosis_message_recovery();
        }
        else if((time.year == msg_time->year) && (time.month == msg_time->month) && (time.day == msg_time->day))
        {
            // 同一天的12:00之后，则清除自诊断信息 
            if(time.hour >= 12)
            {
                APP_CONFIG_diagnosis_message_recovery();
            }
            // 当天7:00之后,则上报自诊断信息，然后清除自诊断信息 
            else if(time.hour >= 7)
            {
                g2_server_packet_t packet;
                G2_SERVER_PACKET_init(&packet);
                g2_server_self_diagnosis_message_t message;
                G2_SERVER_self_diagnosis_message_init(&message);
                message.type = diagnosis_msg.msg_type;
                message.status = diagnosis_msg.msg_status;
                BSP_PROTOCOL_send_read_self_diagnosis_message(&packet, &message);
                APP_CONFIG_diagnosis_message_recovery();
            }
        }
    }
}

#define NETWORK_MIN_INTERVAL    (5)
#define SEND_ENV_INTERVAL_CNT   (10 / NETWORK_MIN_INTERVAL)
#define SEND_L6_STATUS_INTERVAL_CNT (5 / NETWORK_MIN_INTERVAL)
static void send_data_to_server(void)
{
    static uint8_t send_env_cnt = 0;
    static uint8_t send_l6_status_cnt = 0;
    if (APP_SERVER_CONNECTED() && (HARDWARE_GET_TICK() - app_network.last_send_data_tick) > NETWORK_MIN_INTERVAL * 60 * 1000)
    {
        app_network.last_send_data_tick = HARDWARE_GET_TICK();
        send_env_cnt += 1;
        send_l6_status_cnt += 1;
        if(send_env_cnt == SEND_ENV_INTERVAL_CNT)
        {
// #if defined(SUPPORT_TEMPERATURE_AND_HUMIDITY) || defined(SUPPORT_CHECK_POWER_VOL_IN) || defined(SUPPORT_CHECK_POWER_CUR_OUT)
//         APP_ENVIRONMENT_read();
//         HARDWARE_OS_DELAY_MS(100);
//         send_environment_to_server();
// #endif
            send_env_cnt = 0;
        }
        if(send_l6_status_cnt == SEND_L6_STATUS_INTERVAL_CNT)
        {
#ifdef SUPPORT_L6
            HARDWARE_OS_DELAY_MS(100);
            send_l6_status_to_server();
#endif
            send_l6_status_cnt = 0;
        }
        check_and_send_diagnosis_message();
    }
}

/* csq 的解析方法如下：
	1111 1100 | *_*_ *_*_ *_*_ *_*_ | ~#~# ~#~#
	最高字节为 FC
	中间2字节为 data1 和 data2
	最后一字节为 data1 与 data2 的 checksum (对应二进制数值中 bit=1 的个数)
	_ 表示 data1 的二进制数值
	* 表示 data2 的二进制数值
	# 表示 data1 的 checksum 的二进制数值
	~ 表示 data2 的 checksum 的二进制数值
*/
static int parse_csq(uint32_t sample, uint8_t *csq)
{
    uint16_t data1 = 0;
    uint32_t mask1 = 1;
    uint16_t data2 = 0;
    uint32_t mask2 = 2;
    uint8_t c = 0;
    uint8_t checksum = 0;
    int i = 0;

    for (i = 0; i < 16; i++)
    {
        if (sample & mask1)
        {
            data1 |= 1 << i;
        }
        if (sample & mask2)
        {
            data2 |= 1 << i;
        }
        mask1 <<= 2;
        mask2 <<= 2;
    }
    c = (uint8_t)(data1 >> 4);
    checksum = get_1_bits_count_uint8_t(c);
    if ((data1 & 0xF) == checksum)
    {
        *csq = c;
        return APP_OK;
    }
    c = (uint8_t)(data2 >> 4);
    checksum = get_1_bits_count_uint8_t(c);
    if ((data2 & 0xF) == checksum)
    {
        *csq = c;
        return APP_OK;
    }
    return APP_ERROR;
}

int APP_NETWORK_init(void)
{
    _app_network_reset();
    app_network.was_connected_status = 0;
    app_network.last_send_data_tick = HARDWARE_GET_TICK();
    uint8_t week;
    BSP_RTC_get(&check_rtc_time.year, &check_rtc_time.month, &check_rtc_time.day, &check_rtc_time.hour, &check_rtc_time.minute, &check_rtc_time.second, &week);
    return APP_OK;
}

void APP_NETWORK_check_stataus(void)
{
    if (BSP_LINK_connect_read() == 1)
    {
        _on_connect();
    }
    else
    {
        _on_disconnect();
    }
}

int load_rtc_time(void)
{
    datetime_t time = {0};
    uint8_t week = 0;
    int rc = BSP_RTC_get(&time.year, &time.month, &time.day, &time.hour, &time.minute, &time.second, &week);
    APP_CHECK_RC(rc)
    APP_LOG_debug("load: 20%02d-%02d-%02d %02d:%02d:%02d\r\n", time.year, time.month, time.day, time.hour, time.minute, time.second);
    if (time.year == 0)
    {
        return BSP_RTC_load_from_backup();
    }
    return APP_OK;
}

void APP_NETWORK_update_csq(uint8_t csq)
{
    app_network.csq = csq;
    APP_LOG_debug("CSQ: %d\r\n", app_network.csq);
}

void APP_NETWORK_task_run(void *argument)
{
    uint32_t sample = 0;
    uint8_t csq = 0;
    int rc = 0;
    HARDWARE_OS_DELAY_MS(200);
    BSP_RTC_init();
    HARDWARE_OS_DELAY_MS(1500);
    load_rtc_time();
#if defined(SUPPORT_TEMPERATURE_AND_HUMIDITY) || defined(SUPPORT_CHECK_POWER_VOL_IN) || defined(SUPPORT_CHECK_POWER_CUR_OUT)
    APP_ENVIRONMENT_init();
    HARDWARE_OS_DELAY_MS(100);
    APP_ENVIRONMENT_read();
#endif
    for (;;)
    {
        sample = sample << 1;
        sample |= BSP_LINK_csq_read();
        if ((sample & 0xFF000000) == 0xFC000000) // check sample header
        {
            rc = parse_csq(sample, &csq);
            if (rc == APP_OK)
            {
                app_network.csq = csq;
                APP_LOG_debug("CSQ: %d\r\n", app_network.csq);
            }
        }
        APP_NETWORK_check_stataus();
        connect_to_server();
        send_heartbeat_to_server();
        send_csq_to_server();
        send_data_to_server();
        check_rtc_works();
        APP_UPDATE_FIRMWARE_resend_data_ask();
        APP_GPRS_read_iccid_message_process();
        HARDWARE_OS_DELAY_MS(250);
    }
}


