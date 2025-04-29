#ifdef BOARD_HAS_RELAY
#define _COPY_TIME_FIELDS(t1, t2) \
    t1.year = t2.year;            \
    t1.month = t2.month;          \
    t1.day = t2.day;              \
    t1.hour = t2.hour;            \
    t1.minute = t2.minute;        \
    t1.second = t2.second;

int G2_SERVER_write_relay_manual_message_process(g2_server_packet_pt packet)
{
    CHECK_PROTOCOL_MESSAGE
    g2_server_relay_manual_message_pt pmsg = (g2_server_relay_manual_message_pt)packet->parsed;
    CHECK_PROTOCOL_RELAY_ID_MESSAGE(pmsg->relayID)

    uint8_t bits[16] = {0};
    uint8_t count = config_bit_1_bits(pmsg->relay_channel, bits);
    int i = 0;
    datetime_t start_datetime = {0};
    datetime_t end_datetime = {0};
    BSP_RTC_get_datetime(&start_datetime);
    _COPY_TIME_FIELDS(end_datetime, start_datetime)
    datetime_add(&end_datetime, pmsg->duration * 60);
    APP_CONFIG_Time_t start_time = {0};
    APP_CONFIG_Time_t end_time = {0};
    _COPY_TIME_FIELDS(start_time, start_datetime)
    _COPY_TIME_FIELDS(end_time, end_datetime)
    int rc = 0;
    for (i = 0; i < count; i++)
    {
        if (bits[i] <= RELAY_CHANNEL_SIZE)
        {
            rc = APP_CONFIG_ManualRelayWriteTime(pmsg->relayID - 1, bits[i] - 1, &g_appConfigManualRelay[pmsg->relayID - 1][bits[i] - 1], APP_CONFIG_HAS_CONFIG, APP_CONFIG_ENABLED, pmsg->type, start_time, end_time);
            CHECK_CONFIG_MESSAGE_RC(rc)
        }
    }
    CHECK_CONFIG_MESSAGE_RC(rc)
    BSP_PROTOCOL_send_ok_response(packet);
    return PROTOCOL_OK;
}

int G2_SERVER_write_relay_manual_enable_message_process(g2_server_packet_pt packet)
{
    CHECK_PROTOCOL_MESSAGE
    g2_server_relay_manual_enable_message_pt pmsg = (g2_server_relay_manual_enable_message_pt)packet->parsed;
    CHECK_PROTOCOL_RELAY_ID_MESSAGE(pmsg->relayID)

    uint8_t bits[16] = {0};
    uint8_t count = config_bit_1_bits(pmsg->relay_channel, bits);
    int i = 0;
    int rc = 0;
    uint8_t enable = 0;
    uint16_t mask = 0;
    for (i = 0; i < count; i++)
    {
        enable = 0;
        if (bits[i] <= RELAY_CHANNEL_SIZE)
        {
            mask = config_bit_from_index(bits[i]);
            if ((mask & pmsg->enable) == mask)
            {
                enable = APP_CONFIG_ENABLED;
            }
            rc = APP_CONFIG_ManualRelayWriteEnable(pmsg->relayID - 1, bits[i] - 1, &g_appConfigManualRelay[pmsg->relayID - 1][bits[i] - 1], enable);
            CHECK_CONFIG_MESSAGE_RC(rc)
        }
    }

    BSP_PROTOCOL_send_ok_response(packet);
    return PROTOCOL_OK;
}

int G2_SERVER_read_relay_manual_enable_message_process(g2_server_packet_pt packet)
{
    CHECK_PROTOCOL_MESSAGE
    g2_server_relay_id_message_pt pmsg = (g2_server_relay_id_message_pt)packet->parsed;
    CHECK_PROTOCOL_RELAY_ID_MESSAGE(pmsg->relayID)
    uint16_t enable = 0;
    uint16_t channel = 0;
    uint8_t channelID = 0;
    APP_CONFIG_ManualRelay_pt manual_relay = NULL;
    for (channelID = 0; channelID < RELAY_CHANNEL_SIZE; channelID++)
    {
        manual_relay = &g_appConfigManualRelay[pmsg->relayID - 1][channelID];
        if (HAS_CONFIG_IS_VALID(manual_relay))
        {
            channel |= (0x8000 >> channelID);
            if (ENABLE_IS_VALID(manual_relay))
                enable |= (0x8000 >> channelID);
        }
    }
    g2_server_relay_manual_enable_message_t message = {0};
    G2_SERVER_relay_manual_enable_message_init(&message);
    message.relayID = pmsg->relayID;
    message.relay_channel = channel;
    message.enable = enable;
    BSP_PROTOCOL_send_read_relay_manual_enable_message(packet, &message);
    return PROTOCOL_OK;
}

#define RELAY_TIMES_ACTION(type) (((type)&0x0f))
#define RELAY_TIMES_HAS_CONFIG(type) (((type)&0x80) == 0x80)

static void copy_time_config_to_protocol(uint8_t relayID, g2_server_relay_jobs_message_pt message)
{
    int taskID = 0;
    int time_jobs_id = 0;
    uint8_t type = 0;
    APP_CONFIG_ConfigEnable_pt times_enable;
    APP_CONFIG_Times_pt times;
    for (taskID = 0; taskID < APP_CONFIG_MAX_TIMES_TASK; taskID++)
    {
        times_enable = &g_appConfigTimesEnable[relayID - 1].task[taskID];
        times = &g_appConfigTimes[relayID - 1][taskID];
        if (!ENABLE_IS_VALID(times_enable))
            continue;

        message->job_config |= (0x8000 >> taskID);
        message->tasks[taskID].hasLimit = times->hasLimit;
        message->tasks[taskID].relay_channel = times->channel;
        message->tasks[taskID].probeID = times->probeID;
        message->tasks[taskID].sensor_config = config_bit_from_index(times->indicatorID);
        message->tasks[taskID].value = times->value;
        message->tasks[taskID].threshold = times->threshold;
        message->tasks[taskID].executeType = times->executeType;
        message->tasks[taskID].time_config = 0;
        for (time_jobs_id = 0; time_jobs_id < 4; time_jobs_id++)
        {
            type = RELAY_TIMES_ACTION(times->times[time_jobs_id].type);
            if (RELAY_TIMES_HAS_CONFIG(times->times[time_jobs_id].type) && (type == 1 || type == 2))
            {
                message->tasks[taskID].times[time_jobs_id].type = times->times[time_jobs_id].type & 0x0f;
                message->tasks[taskID].times[time_jobs_id].startHour = times->times[time_jobs_id].startHour;
                message->tasks[taskID].times[time_jobs_id].startMinute = times->times[time_jobs_id].startMinute;
                message->tasks[taskID].times[time_jobs_id].endHour = times->times[time_jobs_id].endHour;
                message->tasks[taskID].times[time_jobs_id].endMinute = times->times[time_jobs_id].endMinute;
                message->tasks[taskID].time_config |= (0x80 >> time_jobs_id);
            }
        }
    }
}

int G2_SERVER_read_relay_jobs_message_process(g2_server_packet_pt packet)
{
    CHECK_PROTOCOL_MESSAGE
    g2_server_relay_id_message_pt pmsg = (g2_server_relay_id_message_pt)packet->parsed;
    CHECK_PROTOCOL_RELAY_ID_MESSAGE(pmsg->relayID)

    g2_server_relay_jobs_message_t message;
    G2_SERVER_relay_jobs_message_init(&message);
    message.relayID = pmsg->relayID;
    message.job_config = 0;
    copy_time_config_to_protocol(pmsg->relayID, &message);
    BSP_PROTOCOL_send_read_relay_jobs_message(packet, &message);
    return PROTOCOL_OK;
}

static int save_protocol_to_time_config(g2_server_relay_jobs_message_pt config)
{
    uint8_t taskID = config_bit_to_index(config->job_config);
    if (taskID == 0 || taskID > APP_CONFIG_MAX_TIMES_TASK)
    {
        return APP_ERROR;
    }
    int rc = 0;
    taskID = taskID - 1;
    uint8_t time_jobs_id = 0;
    APP_CONFIG_TimeEnable_pt times_enable = &g_appConfigTimesEnable[config->relayID - 1];
    APP_CONFIG_Times_t times = {0};
    g2_server_relay_jobs_task_message_pt task = &config->tasks[taskID];
    APP_CONFIG_TimesInit(&times);

    times.hasLimit = task->hasLimit;
    times.channel = task->relay_channel;
    times.probeID = task->probeID;
    times.indicatorID = config_bit_to_index(task->sensor_config);
    times.value = task->value;
    times.threshold = task->threshold;
    times.executeType = task->executeType;
    uint8_t mask = 0;
    for (time_jobs_id = 0; time_jobs_id < 4; time_jobs_id++)
    {
        mask = 0x80 >> time_jobs_id;
        uint8_t job_type = task->times[time_jobs_id].type;
        if ((task->time_config & mask) == mask && (job_type == 1 || job_type == 2))
        {
            times.times[time_jobs_id].type = job_type | 0x80;
            times.times[time_jobs_id].startHour = task->times[time_jobs_id].startHour;
            times.times[time_jobs_id].startMinute = task->times[time_jobs_id].startMinute;
            times.times[time_jobs_id].endHour = task->times[time_jobs_id].endHour;
            times.times[time_jobs_id].endMinute = task->times[time_jobs_id].endMinute;
        }
    }
    rc = APP_CONFIG_TimesWrite(config->relayID - 1, taskID, &times);
    APP_CHECK_RC(rc)
    rc = APP_CONFIG_TimesRead(config->relayID - 1, taskID, &g_appConfigTimes[config->relayID - 1][taskID]);
    APP_CHECK_RC(rc)
    APP_CONFIG_ConfigEnable_t config_enable = {.hasConfig = APP_CONFIG_HAS_CONFIG, .enable = APP_CONFIG_ENABLED};
    return APP_CONFIG_TimesEnableWriteConfigEnable(config->relayID - 1, times_enable, taskID, config_enable);
}

int G2_SERVER_write_relay_jobs_message_process(g2_server_packet_pt packet)
{
    CHECK_PROTOCOL_MESSAGE
    g2_server_write_relay_jobs_message_pt pmsg = (g2_server_write_relay_jobs_message_pt)packet->parsed;
    CHECK_PROTOCOL_CHECK_WRITE_MESSAGE(pmsg->oper)
    g2_server_relay_jobs_message_pt config = &pmsg->relay;
    CHECK_PROTOCOL_RELAY_ID_MESSAGE(config->relayID)

    int rc = save_protocol_to_time_config(config);
    CHECK_CONFIG_MESSAGE_RC(rc)
    BSP_PROTOCOL_send_ok_response(packet);
    return PROTOCOL_OK;
}

static int delete_protocol_to_time_config(g2_server_delete_relay_jobs_message_pt config)
{
    uint8_t taskID = config_bit_to_index(config->config);
    if (taskID == 0 || taskID > APP_CONFIG_MAX_TIMES_TASK)
    {
        return APP_ERROR;
    }
    APP_CONFIG_ConfigEnable_t config_enable = {.hasConfig = 0, .enable = 0};
    APP_CONFIG_TimeEnable_pt times_enable = &g_appConfigTimesEnable[config->relayID - 1];
    return APP_CONFIG_TimesEnableWriteConfigEnable(config->relayID - 1, times_enable, taskID - 1, config_enable);
}

int G2_SERVER_delete_relay_jobs_message_process(g2_server_packet_pt packet)
{
    CHECK_PROTOCOL_MESSAGE
    g2_server_delete_relay_jobs_message_pt pmsg = (g2_server_delete_relay_jobs_message_pt)packet->parsed;
    CHECK_PROTOCOL_CHECK_DELETE_MESSAGE(pmsg->oper)
    CHECK_PROTOCOL_RELAY_ID_MESSAGE(pmsg->relayID)
    int rc = delete_protocol_to_time_config(pmsg);
    CHECK_CONFIG_MESSAGE_RC(rc)
    BSP_PROTOCOL_send_ok_response(packet);
    return PROTOCOL_OK;
}

int G2_SERVER_read_relay_jobs_enable_message_process(g2_server_packet_pt packet)
{
    CHECK_PROTOCOL_MESSAGE
    g2_server_relay_id_message_pt pmsg = (g2_server_relay_id_message_pt)packet->parsed;
    CHECK_PROTOCOL_RELAY_ID_MESSAGE(pmsg->relayID)

    g2_server_relay_jobs_enable_message_t message;
    G2_SERVER_relay_jobs_enable_message_init(&message);
    message.relayID = pmsg->relayID;
    APP_CONFIG_TimeEnable_pt times_enable = &g_appConfigTimesEnable[pmsg->relayID - 1];
    for (int taskID = 0; taskID < APP_CONFIG_MAX_TIMES_TASK; taskID++)
    {
        if (HAS_CONFIG_IS_VALID(&times_enable->task[taskID]))
        {
            message.config |= 0x8000 >> taskID;
            if (ENABLE_IS_VALID(&times_enable->task[taskID]))
            {
                message.enable |= 0x8000 >> taskID;
            }
        }
    }
    BSP_PROTOCOL_send_read_relay_jobs_enable_message(packet, &message);
    return PROTOCOL_OK;
}

static int save_protocol_to_time_config_enable(g2_server_relay_jobs_enable_message_pt pmsg)
{
    APP_CONFIG_TimeEnable_pt times_enable = &g_appConfigTimesEnable[pmsg->relayID - 1];
    APP_CONFIG_ConfigEnable_t config_enable = {.hasConfig = 0, .enable = 0};
    uint16_t mask = 0x8000;
    for (int taskID = 0; taskID < APP_CONFIG_MAX_TIMES_TASK; taskID++)
    {
        mask = 0x8000 >> taskID;
        if ((pmsg->config & mask) == mask)
        {
            config_enable.hasConfig = times_enable->task[taskID].hasConfig;
            config_enable.enable = times_enable->task[taskID].enable;
            if (HAS_CONFIG_IS_VALID(&times_enable->task[taskID]))
            {
                if ((pmsg->enable & mask) == mask)
                {
                    config_enable.enable = APP_CONFIG_HAS_CONFIG;
                }
                else
                {
                    config_enable.enable = 0;
                }
            }
            APP_CONFIG_TimesEnableWriteConfigEnable(pmsg->relayID - 1, times_enable, taskID, config_enable);
        }
    }
    return APP_OK;
}

int G2_SERVER_write_relay_jobs_enable_message_process(g2_server_packet_pt packet)
{
    CHECK_PROTOCOL_MESSAGE
    g2_server_relay_jobs_enable_message_pt pmsg = (g2_server_relay_jobs_enable_message_pt)packet->parsed;
    CHECK_PROTOCOL_RELAY_ID_MESSAGE(pmsg->relayID)
    int rc = save_protocol_to_time_config_enable(pmsg);
    CHECK_CONFIG_MESSAGE_RC(rc)
    BSP_PROTOCOL_send_ok_response(packet);
    return PROTOCOL_OK;
}

#include "app_relay.h"

int G2_SERVER_read_relay_data_message_process(g2_server_packet_pt packet)
{
    CHECK_PROTOCOL_MESSAGE
    g2_server_relay_id_message_pt pmsg = (g2_server_relay_id_message_pt)packet->parsed;
    CHECK_PROTOCOL_RELAY_ID_MESSAGE(pmsg->relayID)

    int changed = 0;
    g2_server_relay_data_message_t message;
    int rc = APP_RELAY_read_status_protocol(pmsg->relayID, &message, &changed);
    CHECK_CONFIG_MESSAGE_RC(rc)
    BSP_PROTOCOL_send_read_relay_data_message(packet, &message);
    return PROTOCOL_OK;
}

#else

int G2_SERVER_write_relay_manual_message_process(g2_server_packet_pt packet)
{
    BSP_PROTOCOL_send_error_response(packet);
    return PROTOCOL_ERROR;
}

int G2_SERVER_write_relay_manual_enable_message_process(g2_server_packet_pt packet)
{
    BSP_PROTOCOL_send_error_response(packet);
    return PROTOCOL_ERROR;
}

int G2_SERVER_read_relay_manual_enable_message_process(g2_server_packet_pt packet)
{
    BSP_PROTOCOL_send_error_response(packet);
    return PROTOCOL_ERROR;
}

int G2_SERVER_read_relay_jobs_message_process(g2_server_packet_pt packet)
{
    BSP_PROTOCOL_send_error_response(packet);
    return PROTOCOL_ERROR;
}

int G2_SERVER_write_relay_jobs_message_process(g2_server_packet_pt packet)
{
    BSP_PROTOCOL_send_error_response(packet);
    return PROTOCOL_ERROR;
}

int G2_SERVER_delete_relay_jobs_message_process(g2_server_packet_pt packet)
{
    BSP_PROTOCOL_send_error_response(packet);
    return PROTOCOL_ERROR;
}

int G2_SERVER_read_relay_jobs_enable_message_process(g2_server_packet_pt packet)
{
    BSP_PROTOCOL_send_error_response(packet);
    return PROTOCOL_ERROR;
}

int G2_SERVER_write_relay_jobs_enable_message_process(g2_server_packet_pt packet)
{
    BSP_PROTOCOL_send_error_response(packet);
    return PROTOCOL_ERROR;
}

int G2_SERVER_read_relay_data_message_process(g2_server_packet_pt packet)
{
    BSP_PROTOCOL_send_error_response(packet);
    return PROTOCOL_ERROR;
}
#endif
