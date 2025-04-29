#include "utils.h"
#include "app.h"
#include "bsp_link.h"
#include "bsp_rtc.h"
#include "bsp_wdg.h"
#include "app_log.h"
#include "app_network.h"
#include "app_protocol.h"
#include "hardware_wrapper.h"

APP_NETWORK_t g_appNetwork = {0};

#define CAN_CONNECT_TO_SERVER()      \
    APP_NETWORK_CONNECTED() &&       \
        APP_SERVER_DISCONNECTED() && \
        (g_appNetwork.connectTick == 0 || (HARDWARE_GET_TICK() - g_appNetwork.connectTick > 15000))

static void AppNetworkReset(void)
{
    g_appNetwork.csq = CSQ_MIN_VALUE;
    g_appNetwork.connectedStatus = CONNECTED_STATUS_DISCONNECTED;
    g_appNetwork.networkStatus = CONNECTED_STATUS_DISCONNECTED;
    g_appNetwork.connectTick = 0;
    g_appNetwork.lastSendTick = 0;
    SET_APP_SERVER_ONCE_CONNECTED(CONNECTED_STATUS_DISCONNECTED);
}

static void OnConnect(void)
{
    g_appNetwork.networkStatus = CONNECTED_STATUS_CONNECTED;
}

static void OnDisconnect(void)
{
    if (APP_NETWORK_CONNECTED())
    {
        AppNetworkReset();
    }
}

void APP_NETWORK_AfterSendMessage(void)
{
    // g_appNetwork.lastSendTick = HARDWARE_GET_TICK();
}


void APP_NETWORK_OnConnectToServer(void)
{
    g_appNetwork.connectedStatus = CONNECTED_STATUS_CONNECTED;
    g_appNetwork.wasConnectedStatus = CONNECTED_STATUS_CONNECTED;
    g_appNetwork.connectTick = 0;
    if (g_appConfigDeviceConnect.reason != 1)
    {
        g_appConfigDeviceConnect.reason = 1;
        APP_CONFIG_DeviceConnectWrite(&g_appConfigDeviceConnect);
    }
    SET_APP_SERVER_ONCE_CONNECTED(CONNECTED_STATUS_CONNECTED);
}

static void RequestTimeFromServer(void)
{
    if (APP_SERVER_CONNECTED())
    {
        g2_server_packet_t packet;
        G2_SERVER_PACKET_init(&packet);
        BSP_PROTOCOL_send_read_get_time_message(&packet);
    }
}

static datetime_t s_checkRtcTime = {0};
static uint32_t s_refreshRtcDay = 0;
static int CheckRtcWorks(void)
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
            s_refreshRtcDay = 0xffff; //更新时间
        }
        if (datetime_equals(&s_checkRtcTime, &time))
        {
            rc = BSP_RTC_use_HSE();
            APP_CHECK_RC(rc);
            rc = BSP_RTC_set(time.year, time.month, time.day, time.hour, time.minute, time.second);
            APP_CHECK_RC(rc);
            use_hse = 1;
            use_hse_tick = tick_now;
            BSP_RTC_save_to_backup(&time);
            s_refreshRtcDay = 0xffff; //更新时间
        }
        else
        {
            datetime_copy(&s_checkRtcTime, &time);
        }
        uint32_t refresh_rtc = tick_now / 86400000; //24 hours
        if (refresh_rtc != s_refreshRtcDay && APP_SERVER_CONNECTED())
        {
            s_refreshRtcDay = refresh_rtc;
            RequestTimeFromServer();
        }
        if (use_hse == 1 && time.year >= 20 && use_hse_tick - tick_now > 5 * 60 * 1000)
        {
            use_hse_tick = tick_now;
            BSP_RTC_save_to_backup(&time);
        }
    }
    return APP_OK;
}

static void ConnectToServer(void)
{
    if (CAN_CONNECT_TO_SERVER())
    {
        g2_server_packet_t packet;
        G2_SERVER_PACKET_init(&packet);
        g2_server_connect_message_t message = {.reason = g_appConfigDeviceConnect.reason};
        if (message.reason == 0)
        {
            message.reason = 1;
        }
        if (g_appNetwork.wasConnectedStatus == CONNECTED_STATUS_CONNECTED)
        {
            message.reason = 2;
        }
        BSP_PROTOCOL_send_read_connect_message(&packet, &message);
        g_appNetwork.connectTick = HARDWARE_GET_TICK(); 
    }
}

static void SendHeartbeatToServer(void)
{
    if (APP_SERVER_CONNECTED() && HARDWARE_GET_TICK() - g_appNetwork.lastSendTick > 120 * 1000)
    {
        g_appNetwork.lastSendTick = HARDWARE_GET_TICK();
        g2_server_packet_t packet;
        G2_SERVER_PACKET_init(&packet);
        BSP_PROTOCOL_send_read_heartbeat_message(&packet);
    }
}


#if defined(SUPPORT_TEMPERATURE_AND_HUMIDITY) || defined(SUPPORT_CHECK_POWER_VOL_IN) || defined(SUPPORT_CHECK_POWER_CUR_OUT)
#include "app_environment.h"
static void SendEnvironmentToServer(void)
{
    g2_server_packet_t packet;
    G2_SERVER_PACKET_init(&packet);
    g2_server_environment_message_t message;
    G2_SERVER_environment_message_init(&message);
    APP_ENVIRONMENT_Pack(&message);
    BSP_PROTOCOL_send_read_environment_message(&packet, &message);
}
#endif

#ifdef SUPPORT_L6
#include "app_l6.h"
int SendL6StatusToServer(void)
{
    g2_server_packet_t packet;
    G2_SERVER_PACKET_init(&packet);
    g2_server_l6_status_message_t message;
    G2_SERVER_l6_status_message_init(&message);
    message.probeID = 1;
    APP_L6_StatusPack(&message);
    BSP_PROTOCOL_send_read_l6_status_message(&packet, &message);
    return PROTOCOL_OK;
}
#endif


void APP_NETWORK_UpdateEnvironmentToServer(g2_server_packet_pt packet)
{
#if defined(SUPPORT_TEMPERATURE_AND_HUMIDITY) || defined(SUPPORT_CHECK_POWER_VOL_IN) || defined(SUPPORT_CHECK_POWER_CUR_OUT)
        APP_ENVIRONMENT_Read();
        HARDWARE_OS_DELAY_MS(100);
        G2_SERVER_PACKET_init(packet);
        g2_server_environment_message_t message;
        G2_SERVER_environment_message_init(&message);
        APP_ENVIRONMENT_Pack(&message);
        BSP_PROTOCOL_send_read_environment_message(packet, &message);
#endif
}

static int SendGprsSignalToServer(void)
{
    if((CSQ_MIN_VALUE == g_appNetwork.csq) || (CSQ_MAX_VALUE < g_appNetwork.csq))
    {
        return APP_ERROR;
    }
    g2_server_packet_t packet;
    G2_SERVER_PACKET_init(&packet);
    g2_server_gprs_signal_message_t pmsg = {0};
    pmsg.rssi = g_appNetwork.csq;
    BSP_PROTOCOL_send_read_gprs_signal_message(&packet, &pmsg);
    return APP_OK;
}

#define SEND_CSQ_INTERVAL   (30)
static void SendCsqToServer(void)
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
        if(APP_OK == SendGprsSignalToServer())
        {
            csq_connected_server_flag |= 0x2;
            send_csq_tick = HARDWARE_GET_TICK();
        }
    }
}

extern APP_CONFIG_DiagnosisMessage_t g_diagnosisMsg;
static void CheckAndSendDiagnosisMessage(void)
{
    APP_CONFIG_Time_t time;
    uint8_t week = 0;
    APP_CONFIG_Time_pt msgTime = &(g_diagnosisMsg.msgTime);
    BSP_RTC_get(&time.year, &time.month, &time.day, &time.hour, &time.minute, &time.second, &week);
    if(msgTime->year > 0)  // 检查自诊断信息是否有效 
    {
        // 若不在同一天，则清除自诊断信息 
        if(time.day != msgTime->day) 
        {
            APP_CONFIG_DiagnosisMessageRecovery();
        }
        else if((time.year == msgTime->year) && (time.month == msgTime->month) && (time.day == msgTime->day))
        {
            // 同一天的12:00之后，则清除自诊断信息 
            if(time.hour >= 12)
            {
                APP_CONFIG_DiagnosisMessageRecovery();
            }
            // 当天7:00之后,则上报自诊断信息，然后清除自诊断信息 
            else if(time.hour >= 7)
            {
                g2_server_packet_t packet;
                G2_SERVER_PACKET_init(&packet);
                g2_server_self_diagnosis_message_t message;
                G2_SERVER_self_diagnosis_message_init(&message);
                message.type = g_diagnosisMsg.msgType;
                message.status = g_diagnosisMsg.msgStatus;
                BSP_PROTOCOL_send_read_self_diagnosis_message(&packet, &message);
                APP_CONFIG_DiagnosisMessageRecovery();
            }
        }
    }
}

#define NETWORK_MIN_INTERVAL    (5)
#define SEND_ENV_INTERVAL_CNT   (10 / NETWORK_MIN_INTERVAL)
#define SEND_L6_STATUS_INTERVAL_CNT (5 / NETWORK_MIN_INTERVAL)
static void SendDataToServer(void)
{
    static uint8_t send_env_cnt = 0;
    static uint8_t send_l6_status_cnt = 0;
    if (APP_SERVER_CONNECTED() && (HARDWARE_GET_TICK() - g_appNetwork.lastSendDataTick) > NETWORK_MIN_INTERVAL * 60 * 1000)
    {
        g_appNetwork.lastSendDataTick = HARDWARE_GET_TICK();
        send_env_cnt += 1;
        send_l6_status_cnt += 1;
        if(send_env_cnt == SEND_ENV_INTERVAL_CNT)
        {
#if defined(SUPPORT_TEMPERATURE_AND_HUMIDITY) || defined(SUPPORT_CHECK_POWER_VOL_IN) || defined(SUPPORT_CHECK_POWER_CUR_OUT)
        APP_ENVIRONMENT_Read();
        HARDWARE_OS_DELAY_MS(100);
        SendEnvironmentToServer();
#else        
        send_env_cnt = 0;
#endif
        }
        if(send_l6_status_cnt == SEND_L6_STATUS_INTERVAL_CNT)
        {
#ifdef SUPPORT_L6
            HARDWARE_OS_DELAY_MS(100);
            SendL6StatusToServer();
#endif
            send_l6_status_cnt = 0;
        }
        CheckAndSendDiagnosisMessage();
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
static int ParseCsq(uint32_t sample, uint8_t *csq)
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

int APP_NETWORK_Init(void)
{
    AppNetworkReset();
    g_appNetwork.wasConnectedStatus = 0;
    g_appNetwork.lastSendDataTick = HARDWARE_GET_TICK();
    uint8_t week;
    BSP_RTC_get(&s_checkRtcTime.year, &s_checkRtcTime.month, &s_checkRtcTime.day, &s_checkRtcTime.hour, &s_checkRtcTime.minute, &s_checkRtcTime.second, &week);
    return APP_OK;
}

void APP_NETWORK_CheckStataus(void)
{
    if (BSP_LINK_connect_read() == 1)
    {
        OnConnect();
    }
    else
    {
        OnDisconnect();
    }
}

static int LoadRtcTime(void)
{
    datetime_t time = {0};
    uint8_t week = 0;
    int rc = BSP_RTC_get(&time.year, &time.month, &time.day, &time.hour, &time.minute, &time.second, &week);
    APP_CHECK_RC(rc)
    APP_LOG_Debug("load: 20%02d-%02d-%02d %02d:%02d:%02d\r\n", time.year, time.month, time.day, time.hour, time.minute, time.second);
    if (time.year == 0)
    {
        return BSP_RTC_load_from_backup();
    }
    return APP_OK;
}

void APP_NETWORK_UpdateCSQ(uint8_t csq)
{
    g_appNetwork.csq = csq;
    APP_LOG_Debug("CSQ: %d\r\n", g_appNetwork.csq);
}

void APP_NETWORK_task_run(void *argument)
{
    uint32_t sample = 0;
    uint8_t csq = 0;
    int rc = 0;
    HARDWARE_OS_DELAY_MS(200);
    BSP_RTC_init();
    HARDWARE_OS_DELAY_MS(1500);
    LoadRtcTime();
#if defined(SUPPORT_TEMPERATURE_AND_HUMIDITY) || defined(SUPPORT_CHECK_POWER_VOL_IN) || defined(SUPPORT_CHECK_POWER_CUR_OUT)
    APP_ENVIRONMENT_Init();
    HARDWARE_OS_DELAY_MS(100);
    APP_ENVIRONMENT_Read();
#endif
    for (;;)
    {
        sample = sample << 1;
        sample |= BSP_LINK_csq_read();
        if ((sample & 0xFF000000) == 0xFC000000) // check sample header
        {
            rc = ParseCsq(sample, &csq);
            if (rc == APP_OK)
            {
                g_appNetwork.csq = csq;
                APP_LOG_Trace("CSQ: %d\r\n", g_appNetwork.csq);
            }
        }
        APP_NETWORK_CheckStataus();
        ConnectToServer();
        SendHeartbeatToServer();
        SendCsqToServer();
        SendDataToServer();
        CheckRtcWorks();
        APP_UPDATE_FIRMWARE_ResendDataAsk();
        APP_GPRS_ReadIccidMessageProcess();
        HARDWARE_OS_DELAY_MS(250);
    }
}


