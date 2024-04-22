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
    CHECK_PROTOCOL_RELAY_ID_MESSAGE(pmsg->relay_id)

    uint8_t bits[16] = {0};
    uint8_t count = config_bit_1_bits(pmsg->relay_channel, bits);
    int i = 0;
    datetime_t start_datetime = {0};
    datetime_t end_datetime = {0};
    BSP_RTC_get_datetime(&start_datetime);
    _COPY_TIME_FIELDS(end_datetime, start_datetime)
    datetime_add(&end_datetime, pmsg->duration * 60);
    app_config_time_t start_time = {0};
    app_config_time_t end_time = {0};
    _COPY_TIME_FIELDS(start_time, start_datetime)
    _COPY_TIME_FIELDS(end_time, end_datetime)
    int rc = 0;
    for (i = 0; i < count; i++)
    {
        if (bits[i] <= RELAY_CHANNEL_SIZE)
        {
            rc = APP_CONFIG_manual_relay_write_time(pmsg->relay_id - 1, bits[i] - 1, &app_config_manual_relay[pmsg->relay_id - 1][bits[i] - 1], APP_CONFIG_HAS_CONFIG, APP_CONFIG_ENABLED, pmsg->type, start_time, end_time);
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
    CHECK_PROTOCOL_RELAY_ID_MESSAGE(pmsg->relay_id)

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
            rc = APP_CONFIG_manual_relay_write_enable(pmsg->relay_id - 1, bits[i] - 1, &app_config_manual_relay[pmsg->relay_id - 1][bits[i] - 1], enable);
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
    CHECK_PROTOCOL_RELAY_ID_MESSAGE(pmsg->relay_id)
    uint16_t enable = 0;
    uint16_t channel = 0;
    uint8_t channel_id = 0;
    app_config_manual_relay_pt manual_relay = NULL;
    for (channel_id = 0; channel_id < RELAY_CHANNEL_SIZE; channel_id++)
    {
        manual_relay = &app_config_manual_relay[pmsg->relay_id - 1][channel_id];
        if (HAS_CONFIG_IS_VALID(manual_relay))
        {
            channel |= (0x8000 >> channel_id);
            if (ENABLE_IS_VALID(manual_relay))
                enable |= (0x8000 >> channel_id);
        }
    }
    g2_server_relay_manual_enable_message_t message = {0};
    G2_SERVER_relay_manual_enable_message_init(&message);
    message.relay_id = pmsg->relay_id;
    message.relay_channel = channel;
    message.enable = enable;
    BSP_PROTOCOL_send_read_relay_manual_enable_message(packet, &message);
    return PROTOCOL_OK;
}

#define RELAY_TIMES_ACTION(type) (((type)&0x0f))
#define RELAY_TIMES_HAS_CONFIG(type) (((type)&0x80) == 0x80)

static void copy_time_config_to_protocol(uint8_t relay_id, g2_server_relay_jobs_message_pt message)
{
    int task_id = 0;
    int time_jobs_id = 0;
    uint8_t type = 0;
    app_config_config_enable_pt times_enable;
    app_config_times_pt times;
    for (task_id = 0; task_id < APP_CONFIG_MAX_TIMES_TASK; task_id++)
    {
        times_enable = &app_config_times_enable[relay_id - 1].task[task_id];
        times = &app_config_times[relay_id - 1][task_id];
        if (!ENABLE_IS_VALID(times_enable))
            continue;

        message->job_config |= (0x8000 >> task_id);
        message->tasks[task_id].has_limit = times->has_limit;
        message->tasks[task_id].relay_channel = times->channel;
        message->tasks[task_id].probe_id = times->probe_id;
        message->tasks[task_id].sensor_config = config_bit_from_index(times->indicator_id);
        message->tasks[task_id].value = times->value;
        message->tasks[task_id].threshold = times->threshold;
        message->tasks[task_id].execute_type = times->execute_type;
        message->tasks[task_id].time_config = 0;
        for (time_jobs_id = 0; time_jobs_id < 4; time_jobs_id++)
        {
            type = RELAY_TIMES_ACTION(times->times[time_jobs_id].type);
            if (RELAY_TIMES_HAS_CONFIG(times->times[time_jobs_id].type) && (type == 1 || type == 2))
            {
                message->tasks[task_id].times[time_jobs_id].type = times->times[time_jobs_id].type & 0x0f;
                message->tasks[task_id].times[time_jobs_id].start_hour = times->times[time_jobs_id].start_hour;
                message->tasks[task_id].times[time_jobs_id].start_minute = times->times[time_jobs_id].start_minute;
                message->tasks[task_id].times[time_jobs_id].end_hour = times->times[time_jobs_id].end_hour;
                message->tasks[task_id].times[time_jobs_id].end_minute = times->times[time_jobs_id].end_minute;
                message->tasks[task_id].time_config |= (0x80 >> time_jobs_id);
            }
        }
    }
}

int G2_SERVER_read_relay_jobs_message_process(g2_server_packet_pt packet)
{
    CHECK_PROTOCOL_MESSAGE
    g2_server_relay_id_message_pt pmsg = (g2_server_relay_id_message_pt)packet->parsed;
    CHECK_PROTOCOL_RELAY_ID_MESSAGE(pmsg->relay_id)

    g2_server_relay_jobs_message_t message;
    G2_SERVER_relay_jobs_message_init(&message);
    message.relay_id = pmsg->relay_id;
    message.job_config = 0;
    copy_time_config_to_protocol(pmsg->relay_id, &message);
    BSP_PROTOCOL_send_read_relay_jobs_message(packet, &message);
    return PROTOCOL_OK;
}

static int save_protocol_to_time_config(g2_server_relay_jobs_message_pt config)
{
    uint8_t task_id = config_bit_to_index(config->job_config);
    if (task_id == 0 || task_id > APP_CONFIG_MAX_TIMES_TASK)
    {
        return APP_ERROR;
    }
    int rc = 0;
    task_id = task_id - 1;
    uint8_t time_jobs_id = 0;
    app_config_times_enable_pt times_enable = &app_config_times_enable[config->relay_id - 1];
    app_config_times_t times = {0};
    g2_server_relay_jobs_task_message_pt task = &config->tasks[task_id];
    APP_CONFIG_times_init(&times);

    times.has_limit = task->has_limit;
    times.channel = task->relay_channel;
    times.probe_id = task->probe_id;
    times.indicator_id = config_bit_to_index(task->sensor_config);
    times.value = task->value;
    times.threshold = task->threshold;
    times.execute_type = task->execute_type;
    uint8_t mask = 0;
    for (time_jobs_id = 0; time_jobs_id < 4; time_jobs_id++)
    {
        mask = 0x80 >> time_jobs_id;
        uint8_t job_type = task->times[time_jobs_id].type;
        if ((task->time_config & mask) == mask && (job_type == 1 || job_type == 2))
        {
            times.times[time_jobs_id].type = job_type | 0x80;
            times.times[time_jobs_id].start_hour = task->times[time_jobs_id].start_hour;
            times.times[time_jobs_id].start_minute = task->times[time_jobs_id].start_minute;
            times.times[time_jobs_id].end_hour = task->times[time_jobs_id].end_hour;
            times.times[time_jobs_id].end_minute = task->times[time_jobs_id].end_minute;
        }
    }
    rc = APP_CONFIG_times_write(config->relay_id - 1, task_id, &times);
    APP_CHECK_RC(rc)
    rc = APP_CONFIG_times_read(config->relay_id - 1, task_id, &app_config_times[config->relay_id - 1][task_id]);
    APP_CHECK_RC(rc)
    app_config_config_enable_t config_enable = {.has_config = APP_CONFIG_HAS_CONFIG, .enable = APP_CONFIG_ENABLED};
    return APP_CONFIG_times_enable_write_config_enable(config->relay_id - 1, times_enable, task_id, config_enable);
}

int G2_SERVER_write_relay_jobs_message_process(g2_server_packet_pt packet)
{
    CHECK_PROTOCOL_MESSAGE
    g2_server_write_relay_jobs_message_pt pmsg = (g2_server_write_relay_jobs_message_pt)packet->parsed;
    CHECK_PROTOCOL_CHECK_WRITE_MESSAGE(pmsg->oper)
    g2_server_relay_jobs_message_pt config = &pmsg->relay;
    CHECK_PROTOCOL_RELAY_ID_MESSAGE(config->relay_id)

    int rc = save_protocol_to_time_config(config);
    CHECK_CONFIG_MESSAGE_RC(rc)
    BSP_PROTOCOL_send_ok_response(packet);
    return PROTOCOL_OK;
}

static int delete_protocol_to_time_config(g2_server_delete_relay_jobs_message_pt config)
{
    uint8_t task_id = config_bit_to_index(config->config);
    if (task_id == 0 || task_id > APP_CONFIG_MAX_TIMES_TASK)
    {
        return APP_ERROR;
    }
    app_config_config_enable_t config_enable = {.has_config = 0, .enable = 0};
    app_config_times_enable_pt times_enable = &app_config_times_enable[config->relay_id - 1];
    return APP_CONFIG_times_enable_write_config_enable(config->relay_id - 1, times_enable, task_id - 1, config_enable);
}

int G2_SERVER_delete_relay_jobs_message_process(g2_server_packet_pt packet)
{
    CHECK_PROTOCOL_MESSAGE
    g2_server_delete_relay_jobs_message_pt pmsg = (g2_server_delete_relay_jobs_message_pt)packet->parsed;
    CHECK_PROTOCOL_CHECK_DELETE_MESSAGE(pmsg->oper)
    CHECK_PROTOCOL_RELAY_ID_MESSAGE(pmsg->relay_id)
    int rc = delete_protocol_to_time_config(pmsg);
    CHECK_CONFIG_MESSAGE_RC(rc)
    BSP_PROTOCOL_send_ok_response(packet);
    return PROTOCOL_OK;
}

int G2_SERVER_read_relay_jobs_enable_message_process(g2_server_packet_pt packet)
{
    CHECK_PROTOCOL_MESSAGE
    g2_server_relay_id_message_pt pmsg = (g2_server_relay_id_message_pt)packet->parsed;
    CHECK_PROTOCOL_RELAY_ID_MESSAGE(pmsg->relay_id)

    g2_server_relay_jobs_enable_message_t message;
    G2_SERVER_relay_jobs_enable_message_init(&message);
    message.relay_id = pmsg->relay_id;
    app_config_times_enable_pt times_enable = &app_config_times_enable[pmsg->relay_id - 1];
    for (int task_id = 0; task_id < APP_CONFIG_MAX_TIMES_TASK; task_id++)
    {
        if (HAS_CONFIG_IS_VALID(&times_enable->task[task_id]))
        {
            message.config |= 0x8000 >> task_id;
            if (ENABLE_IS_VALID(&times_enable->task[task_id]))
            {
                message.enable |= 0x8000 >> task_id;
            }
        }
    }
    BSP_PROTOCOL_send_read_relay_jobs_enable_message(packet, &message);
    return PROTOCOL_OK;
}

static int save_protocol_to_time_config_enable(g2_server_relay_jobs_enable_message_pt pmsg)
{
    app_config_times_enable_pt times_enable = &app_config_times_enable[pmsg->relay_id - 1];
    app_config_config_enable_t config_enable = {.has_config = 0, .enable = 0};
    uint16_t mask = 0x8000;
    for (int task_id = 0; task_id < APP_CONFIG_MAX_TIMES_TASK; task_id++)
    {
        mask = 0x8000 >> task_id;
        if ((pmsg->config & mask) == mask)
        {
            config_enable.has_config = times_enable->task[task_id].has_config;
            config_enable.enable = times_enable->task[task_id].enable;
            if (HAS_CONFIG_IS_VALID(&times_enable->task[task_id]))
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
            APP_CONFIG_times_enable_write_config_enable(pmsg->relay_id - 1, times_enable, task_id, config_enable);
        }
    }
    return APP_OK;
}

int G2_SERVER_write_relay_jobs_enable_message_process(g2_server_packet_pt packet)
{
    CHECK_PROTOCOL_MESSAGE
    g2_server_relay_jobs_enable_message_pt pmsg = (g2_server_relay_jobs_enable_message_pt)packet->parsed;
    CHECK_PROTOCOL_RELAY_ID_MESSAGE(pmsg->relay_id)
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
    CHECK_PROTOCOL_RELAY_ID_MESSAGE(pmsg->relay_id)

    int changed = 0;
    g2_server_relay_data_message_t message;
    int rc = APP_RELAY_read_status_protocol(pmsg->relay_id, &message, &changed);
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
