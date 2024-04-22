
#include "bsp_rtc.h"
#include "bsp_protocol.h"
#include "app_log.h"
#include "app_relay.h"
#include "app_config.h"
#include "app_sensors.h"

#ifdef BOARD_HAS_RELAY
#include "app_can.h"

#define APP_RELAY_STATUS_UNSET 0
#define APP_RELAY_STATUS_SET_ON 1  // 按下
#define APP_RELAY_STATUS_SET_OFF 2 // 释放
#define APP_RELAY_STATUS_ESTOP 3   // 急停

#define APP_RELAY_STATUS_UNCHANGED 0
#define APP_RELAY_STATUS_CHANGED 1
#define APP_RELAY_STATUS_STILL_CHANGED 2

#define APP_RELAY_COMMAND_TRY_MAX       (5) // 主控板下发继电器控制命令尝试的最大次数 
static uint8_t broken_protect_channel_mask = 0; // 发生过告警的通道的标识
static app_relay_channel_t app_relay_channels[RELAY_SIZE][RELAY_CHANNEL_SIZE] = {0};
static uint8_t refresh_power_config = 1;
static uint8_t command_try_cnt[RELAY_CHANNEL_SIZE] = {0};

static uint8_t device_warnning_content = APP_DEVICE_WARNNING_OK;    // 设备告警信息内容 
static uint8_t last_device_warnning_content = APP_DEVICE_WARNNING_OK;   // 前一次的设备告警信息内容 

extern int8_t relay_board_error;
extern uint8_t has_recv_power_cfg_msg;
extern uint8_t relay_load_error;

static void _app_relay_channel_init(app_relay_channel_pt channel)
{
    channel->calculate_status = APP_RELAY_STATUS_UNSET;
    channel->output_status = APP_RELAY_STATUS_SET_OFF;
    channel->limit_status = APP_RELAY_STATUS_UNSET;
    channel->limit_status_is_on = 0;
    channel->times_status = APP_RELAY_STATUS_UNSET;
    channel->manual_status = APP_RELAY_STATUS_UNSET;
    channel->remote_manual_status = APP_RELAY_STATUS_UNSET;
    channel->local_manual_status = APP_RELAY_STATUS_UNSET;
    channel->all_off_manual_status = APP_RELAY_STATUS_UNSET;
    channel->relay_output_status = APP_RELAY_STATUS_UNSET;
    channel->relay_broken_status = APP_RELAY_STATUS_UNSET;
    channel->relay_leakage_status = APP_RELAY_STATUS_UNSET;
    channel->relay_loss_phase_status = APP_RELAY_STATUS_UNSET;
    channel->relay_overload_status = APP_RELAY_STATUS_UNSET;
    channel->relay_ouv_status = APP_RELAY_STATUS_UNSET;
    channel->relay_loss_phase_protect_status = APP_RELAY_STATUS_UNSET;
    channel->relay_overload_protect_status = APP_RELAY_STATUS_UNSET;
    channel->relay_ouv_protect_status = APP_RELAY_STATUS_UNSET;
    channel->status_change_tick = 0;
    channel->changed = APP_RELAY_STATUS_UNCHANGED;
}

int APP_RELAY_init(void)
{
    uint8_t relay_id = 0;
    uint8_t channel_id = 0;
    for (; relay_id < RELAY_SIZE; relay_id++)
    {
        for (; channel_id < RELAY_CHANNEL_SIZE; channel_id++)
        {
            _app_relay_channel_init(&app_relay_channels[relay_id][channel_id]);
        }
    }
    return APP_OK;
}

#define _SET_CALCULATE_STATUS(_type)        \
    if (status != APP_RELAY_STATUS_UNSET)   \
    {                                       \
        channel->calculate_status = status; \
        channel->_type = status;            \
        return;                             \
    }
#define _CHECK_CALCULATE_STATUS()                            \
    if (channel->calculate_status != APP_RELAY_STATUS_UNSET) \
    {                                                        \
        return;                                              \
    }

#define _COMPARE_TIME_FIELD(field)   \
    if (time1->field < time2->field) \
    {                                \
        return -1;                   \
    }                                \
    if (time1->field > time2->field) \
    {                                \
        return 1;                    \
    }

static int datetime_compare(app_config_time_pt time1, app_config_time_pt time2)
{
    _COMPARE_TIME_FIELD(year)
    _COMPARE_TIME_FIELD(month)
    _COMPARE_TIME_FIELD(day)
    _COMPARE_TIME_FIELD(hour)
    _COMPARE_TIME_FIELD(minute)
    _COMPARE_TIME_FIELD(second)
    return 0;
}

static int datetime_in_range(app_config_time_pt now, app_config_time_pt start, app_config_time_pt end)
{
    // APP_LOG_trace("start: 20%d-%d-%d %d:%d:%d\r\n", start->year, start->month, start->day, start->hour, start->minute, start->second);
    // APP_LOG_trace("now: 20%d-%d-%d %d:%d:%d\r\n", now->year, now->month, now->day, now->hour, now->minute, now->second);
    // APP_LOG_trace("end: 20%d-%d-%d %d:%d:%d\r\n", end->year, end->month, end->day, end->hour, end->minute, end->second);
    int rc = datetime_compare(now, start);
    int rc1 = datetime_compare(now, end);
    if (rc == -1 || rc1 == 1)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

static int time_job_compare(app_config_time_pt now, uint8_t hour, uint8_t minute)
{
    if (now->hour < hour)
    {
        return -1;
    }
    if (now->hour > hour)
    {
        return 1;
    }

    if (now->minute < minute)
    {
        return -1;
    }
    if (now->minute > minute)
    {
        return 1;
    }
    return 0;
}

static int time_job_in_range(app_config_time_pt now, app_config_times_job_pt job)
{
    int over_day = 0;
    if (job->end_hour == job->start_hour && job->end_minute == job->start_minute)
        return 0;
    uint8_t start_hour = job->start_hour;
    uint8_t start_minute = job->start_minute;
    uint8_t end_hour = job->end_hour;
    uint8_t end_minute = job->end_minute;
    if (job->end_hour < job->start_hour)
        over_day = 1;
    if (job->end_hour == job->start_hour && job->end_minute < job->start_minute)
        over_day = 1;
    if (over_day == 1)
    {
        start_hour = job->end_hour;
        start_minute = job->end_minute;
        end_hour = job->start_hour;
        end_minute = job->start_minute;
    }
    int rc1 = time_job_compare(now, start_hour, start_minute);
    int rc2 = time_job_compare(now, end_hour, end_minute);

    // 跨开要以时间范围外，非跨天在时间范围内

    if (rc1 != -1 && rc2 == -1)
    {
        return over_day ? 0 : 1;
    }
    return over_day ? 1 : 0;
}

#define RELAY_TIMES_ACTION(type) (((type)&0x0f))
#define RELAY_TIMES_HAS_CONFIG(type) (((type)&0x80) == 0x80)

static int valid_time_job(app_config_time_pt now, app_config_times_job_pt job)
{
    uint8_t type = RELAY_TIMES_ACTION(job->type);
    if (!RELAY_TIMES_HAS_CONFIG(job->type))
        return 0;
    if (type != 1 && type != 2)
        return 0;
    return time_job_in_range(now, job);
}

static int _check_times_limit_valid(app_config_times_pt times)
{
    if (times->has_limit != 1) // 无门限直接返回成功
    {
        return 1;
    }
    app_water_indicator_value_pt water_indicator;
    int rc = APP_SENSORS_water_indicator_get(times->indicator_id - 1, &water_indicator);
    if (rc != APP_OK || water_indicator->sensor == NULL)
    {
        return 0;
    }
    double value;
    uint16_t u_value = 0;
    int rc1 = mean_filter_get(&water_indicator->filter, &value);
    int rc2 = water_indicator_to_uint16_t(times->indicator_id - 1, value, &u_value);
    if (rc1 != APP_OK || rc2 != APP_OK || water_indicator->error_count > 3 || mean_filter_count(&water_indicator->filter) < 5)
    {
        return 0;
    }
    if (times->execute_type == 1 && u_value < times->value)
    {
        return 1;
    }
    if (times->execute_type == 2 && u_value > times->value)
    {
        return 1;
    }
    return 0;
}

// 计算手动操作结果
static void _calculate_channel_status_by_manual_relay(uint8_t relay_id, uint8_t channel_id, app_config_time_pt now)
{
    app_relay_channel_pt channel = &app_relay_channels[relay_id][channel_id];
    app_config_manual_relay_pt manual_relay = &app_config_manual_relay[relay_id][channel_id];
    uint8_t status = APP_RELAY_STATUS_UNSET;
    channel->remote_manual_status = APP_RELAY_STATUS_UNSET;
    if (ENABLE_IS_VALID(manual_relay) && datetime_in_range(now, &manual_relay->start, &manual_relay->end))
    {
        if ((manual_relay->type) != 0) // 控制类型为:开
        {
            status = APP_RELAY_STATUS_SET_ON;
        }
        else // 控制类型为:关
        {
            status = APP_RELAY_STATUS_SET_OFF;
        }
        _SET_CALCULATE_STATUS(remote_manual_status)
    }
}

// 计算定时操作结果
static void _calculate_channel_status_by_relay_jobs(uint8_t relay_id, uint8_t channel_id, app_config_time_pt now)
{
    app_relay_channel_pt channel = &app_relay_channels[relay_id][channel_id];
    app_config_config_enable_pt times_enable;
    app_config_times_pt times;
    uint16_t mask = 0;
    uint8_t status = APP_RELAY_STATUS_UNSET;
    uint8_t time_jobs_id = 0;
    uint8_t task_id = 0;
    uint8_t type = 0;
    channel->times_status = APP_RELAY_STATUS_UNSET;
    for (task_id = 0; task_id < APP_CONFIG_MAX_TIMES_TASK; task_id++)
    {
        times_enable = &app_config_times_enable[relay_id].task[task_id];
        times = &app_config_times[relay_id][task_id];
        if (!ENABLE_IS_VALID(times_enable))
        {
            continue;
        }
        if (!_check_times_limit_valid(times))
        {
            continue;
        }
        mask = 0x8000 >> channel_id;
        if ((times->channel & mask) != mask)
        {
            continue;
        }
        for (time_jobs_id = 0; time_jobs_id < 4; time_jobs_id++)
        {
            if (valid_time_job(now, &times->times[time_jobs_id]))
            {
                type = RELAY_TIMES_ACTION(times->times[time_jobs_id].type);
                if (type == 1)
                {
                    status = APP_RELAY_STATUS_SET_ON;
                }
                if (type == 2)
                {
                    status = APP_RELAY_STATUS_SET_OFF;
                }
                _SET_CALCULATE_STATUS(times_status)
            }
        }
    }
}

#define CALCULATE_LIMIT_STATUS_IS_ON(indicator_id, task_id) (((indicator_id + 1) << 8) | (task_id + 1))
int _check_indicator_limit(int indicator_id, int task_id, app_relay_channel_pt channel, app_water_indicator_value_pt water_indicator, app_config_limit_pt limit)
{
    int rc1 = 0, rc2 = 0;
    double value = 0;
    uint16_t u_value = 0;
    rc1 = mean_filter_get(&water_indicator->filter, &value);
    rc2 = water_indicator_to_uint16_t(indicator_id, value, &u_value);
    if (rc1 != APP_OK || rc2 != APP_OK || water_indicator->error_count > 3 || mean_filter_count(&water_indicator->filter) < 5)
    {
        return 0;
    }
    if (limit->type == 1)
    {
        if (u_value < limit->down_limit)
        {
            channel->limit_status_is_on = CALCULATE_LIMIT_STATUS_IS_ON(indicator_id, task_id);
        }

        if ((channel->limit_status_is_on == CALCULATE_LIMIT_STATUS_IS_ON(indicator_id, task_id)))
        {
            if (u_value > limit->up_limit)
            {
                channel->limit_status_is_on = 0;
            }
            return channel->limit_status_is_on;
        }
    }

    if (limit->type == 2)
    {
        if (u_value > limit->up_limit)
        {
            channel->limit_status_is_on = CALCULATE_LIMIT_STATUS_IS_ON(indicator_id, task_id);
        }
        if ((channel->limit_status_is_on == CALCULATE_LIMIT_STATUS_IS_ON(indicator_id, task_id)))
        {
            if (u_value < limit->down_limit)
            {
                channel->limit_status_is_on = 0;
            }
            return channel->limit_status_is_on;
        }
    }

    return 0;
}

// 计算智能操作结果
static void _calculate_channel_status_by_sensor_limits(uint8_t relay_id, uint8_t channel_id)
{
    app_relay_channel_pt channel = &app_relay_channels[relay_id][channel_id];

    app_config_limit_pt limit = &app_config_limit[0][0][0];
    uint16_t mask = 0;
    app_water_indicator_value_pt water_indicator;
    uint8_t status = APP_RELAY_STATUS_UNSET;
    channel->limit_status = APP_RELAY_STATUS_UNSET;
    int rc = 0;
    int indicator_id = 0;
    int task_id = 0;
    for (indicator_id = 0; indicator_id < INDICATOR_SIZE; indicator_id++)
    {
        if (indicator_id >= PROBE_SENSOR_SIZE)
        {
            break;
        }
        for (task_id = 0; task_id < APP_CONFIG_MAX_LIMIT_TASK; task_id++)
        {
            limit = &app_config_limit[0][indicator_id][task_id];
            mask = 0x8000 >> channel_id;
            if ((!ENABLE_IS_VALID(limit)) || ((limit->channel & mask) != mask))
            {
                continue;
            }
            rc = APP_SENSORS_water_indicator_get(indicator_id, &water_indicator);
            if (rc == APP_OK && water_indicator->sensor != NULL && _check_indicator_limit(indicator_id, task_id, channel, water_indicator, limit))
            {
                status = APP_RELAY_STATUS_SET_ON;
                _SET_CALCULATE_STATUS(limit_status)
            }
        }
    }
}

static void _set_timer_task_control_enable(uint8_t relay_id, uint8_t channel_id, uint8_t enable)
{
    app_config_manual_relay_pt manual_relay = &app_config_manual_relay[relay_id][channel_id];
    if (ENABLE_IS_VALID(manual_relay) ^ enable)
    {
        manual_relay->enable = enable;
        APP_CONFIG_manual_relay_write_enable(relay_id, channel_id, manual_relay, enable);
    }
}

static uint8_t _get_protect_enable(uint8_t channel_id, uint8_t broken_type)
{
    app_relay_power_config_pt pconfig = &app_relay_power_config[0][channel_id];
    uint8_t enable = APP_RELAY_PROTECT_DISABLE;
    if (APP_RELAY_STATUS_LOSS_PHASE == broken_type)
    {
        enable = pconfig->loss_phase_protect_enable;
    }
    else if (APP_RELAY_STATUS_OVERLOAD == broken_type)
    {
        enable = pconfig->overload_protect_enable;
    }
    else if (APP_RELAY_STATUS_OUV == broken_type)
    {
        enable = pconfig->ouv_protect_enable;
    }

    return enable;
}


uint8_t local_key_status[RELAY_CHANNEL_SIZE] = {APP_RELAY_STATUS_UNSET};
static uint8_t key_status_changed[RELAY_CHANNEL_SIZE] = {APP_RELAY_STATUS_UNCHANGED};

static void _calculate_remote_status(uint8_t relay_id, uint8_t channel_id, app_config_time_pt now)
{
    app_relay_channel_pt channel = &app_relay_channels[relay_id][channel_id];
    channel->calculate_status = APP_RELAY_STATUS_UNSET;
    channel->remote_manual_status = APP_RELAY_STATUS_UNSET;
    channel->times_status = APP_RELAY_STATUS_UNSET;
    channel->limit_status = APP_RELAY_STATUS_UNSET;
    _calculate_channel_status_by_manual_relay(relay_id, channel_id, now);
    _CHECK_CALCULATE_STATUS()
    _calculate_channel_status_by_relay_jobs(relay_id, channel_id, now);
    _CHECK_CALCULATE_STATUS()
    _calculate_channel_status_by_sensor_limits(relay_id, channel_id);
    _CHECK_CALCULATE_STATUS()
    channel->calculate_status = APP_RELAY_STATUS_SET_OFF; // 没有控制任务时，默认为"设置通道关"
}

void _update_broken_protect_channel_mask(uint8_t channel_id)
{
    uint8_t error_type = APP_CAN_read_relay_channel_broken(0, channel_id);
    uint8_t enable = _get_protect_enable(channel_id, error_type);

    if (APP_RELAY_PROTECT_ENABLE == enable)
    {
        broken_protect_channel_mask |= (0x1 << channel_id);
    }
}

uint8_t _get_broken_protect_channel_status(uint8_t channel_id)
{
    return (0x1 & (broken_protect_channel_mask >> channel_id));
}


void _app_relay_check_load_conflict(void)
{
    uint8_t channel_id = 0;
    app_relay_channel_pt channel = NULL;
    // 根据通道故障过的状态重新计算通道输出 
    for(channel_id = 0; channel_id < RELAY_CHANNEL_SIZE; channel_id++)
    {
        // 当发现进线类型与负载不匹配故障时，设置通道输出为 OFF 
        if(0x1 & (relay_load_error >> channel_id))
        {
            channel = &app_relay_channels[0][channel_id];
            channel->calculate_status = APP_RELAY_STATUS_SET_OFF;
        }
    }
}

static void _calculate_channel_status(uint8_t relay_id, uint8_t channel_id, app_config_time_pt now)
{
    app_relay_channel_pt channel = &app_relay_channels[relay_id][channel_id];
    channel->local_manual_status = APP_RELAY_STATUS_UNSET;

    _calculate_remote_status(relay_id, channel_id, now);
    if(APP_OK == APP_CAN_update_relay_command(channel_id))
    {
        command_try_cnt[channel_id] = 0;
    }
    else
    {
        if((command_try_cnt[channel_id] >= APP_RELAY_COMMAND_TRY_MAX) || (IF_RELAY_BOARD_CHECK_ERROR()))
        {
            uint8_t output = (APP_RELAY_STATUS_SET_ON == channel->calculate_status ? 1 : 0);
            APP_CAN_set_relay_message_error(channel_id, output);
        }
    }
    uint8_t key_status = APP_CAN_read_key_status_channel(relay_id, channel_id);
    uint8_t output_status = (APP_CAN_read_output_channel(relay_id, channel_id) == 1 ? APP_RELAY_STATUS_SET_ON : APP_RELAY_STATUS_SET_OFF);
    // key_status_changed[channel_id] = APP_RELAY_STATUS_UNCHANGED;    // 默认按键没有动作
    // 按键按下--ON
    if ((APP_RELAY_STATUS_SET_ON == key_status) && (APP_RELAY_STATUS_SET_ON != local_key_status[channel_id]))
    {
        key_status_changed[channel_id] = APP_RELAY_STATUS_CHANGED;
        // _set_timer_task_control_enable(relay_id, channel_id, (!APP_CONFIG_ENABLED));
        APP_LOG_debug("key<%d> press\r\n", channel_id + 1);
    }
    // 按键释放--OFF
    else if ((APP_RELAY_STATUS_SET_OFF == key_status) && (APP_RELAY_STATUS_SET_OFF != local_key_status[channel_id]))
    {
        key_status_changed[channel_id] = APP_RELAY_STATUS_CHANGED;
        // _set_timer_task_control_enable(relay_id, channel_id, (!APP_CONFIG_ENABLED));
        APP_LOG_debug("key<%d> release\r\n", channel_id + 1);
    }
    local_key_status[channel_id] = key_status;

    _update_broken_protect_channel_mask(channel_id);

    if (APP_RELAY_STATUS_CHANGED == key_status_changed[channel_id]) // 本地按键状态发生变化
    {
        if ((APP_RELAY_STATUS_SET_ON != key_status) && ((APP_RELAY_STATUS_UNSET != channel->remote_manual_status) || (APP_RELAY_STATUS_UNSET != channel->times_status) || (APP_RELAY_STATUS_UNSET != channel->limit_status)))
        {
            // 以远程手动/定时/智能的计算结果为准
            channel->local_manual_status = APP_RELAY_STATUS_UNSET;
            channel->output_status = output_status;
        }
        else
        { // 以本地按键的计算结果为准
            channel->calculate_status = output_status;
            channel->output_status = output_status;
            channel->local_manual_status = output_status;
            channel->remote_manual_status = APP_RELAY_STATUS_UNSET;
            channel->times_status = APP_RELAY_STATUS_UNSET;
            channel->limit_status = APP_RELAY_STATUS_UNSET;;
        }
        key_status_changed[channel_id] = APP_RELAY_STATUS_STILL_CHANGED;
        channel->changed = APP_RELAY_STATUS_CHANGED;
    }
    else if (APP_RELAY_STATUS_STILL_CHANGED == key_status_changed[channel_id]) // 本地按键状态保持
    {
        if (APP_RELAY_STATUS_SET_ON == key_status)
        {
            // 以本地按键的计算结果为准
            channel->calculate_status = APP_RELAY_STATUS_SET_ON;
            channel->output_status = output_status;
            channel->local_manual_status = output_status;
            channel->remote_manual_status = APP_RELAY_STATUS_UNSET;
            channel->times_status = APP_RELAY_STATUS_UNSET;
            channel->limit_status = APP_RELAY_STATUS_UNSET;;
        }
        else if ((APP_RELAY_STATUS_SET_OFF == key_status) && ((APP_RELAY_STATUS_UNSET == channel->remote_manual_status) && (APP_RELAY_STATUS_UNSET == channel->times_status) && (APP_RELAY_STATUS_UNSET == channel->limit_status)))
        {
            // 以默认输出OFF为准
            channel->calculate_status = APP_RELAY_STATUS_SET_OFF;
            channel->output_status = output_status;
            channel->local_manual_status = output_status;
            channel->remote_manual_status = APP_RELAY_STATUS_UNSET;
            channel->times_status = APP_RELAY_STATUS_UNSET;
            channel->limit_status = APP_RELAY_STATUS_UNSET;;
        }
        else
        {
            // 以远程手动/定时/智能的计算结果为准
            channel->local_manual_status = APP_RELAY_STATUS_UNSET;
            channel->output_status = output_status;
        }
    }

    if (_get_broken_protect_channel_status(channel_id))
    {
        // 检测到发送(过)不可恢复故障后，设置输出为 OFF
        channel->calculate_status = APP_RELAY_STATUS_SET_OFF;
        channel->output_status = APP_RELAY_STATUS_SET_OFF;
    }
}


int G2_SERVER_write_relay_power_config_message_process(g2_server_packet_pt packet)
{
    g2_server_relay_power_config_message_pt message = (g2_server_relay_power_config_message_pt)(packet->parsed);
    app_relay_power_config_pt pconfig = NULL, mpower = NULL;
    // 更新 app_relay_power
    mpower = &(message->power[0][0]);
    for (uint8_t relay_id = 0; relay_id < RELAY_SIZE; relay_id++)
    {
        for (uint8_t channel_id = 0; channel_id < RELAY_CHANNEL_SIZE; channel_id++)
        {
            if (config_bit_is_set(message->config, channel_id + 1))
            {
                pconfig = &app_relay_power_config[relay_id][channel_id];
                if (0 == mpower->power)
                {
                    mpower->power = (RELAY_CHANNEL_SIZE - 1 == channel_id ? 10 : 30);
                }
                memcpy((uint8_t *)(pconfig), (uint8_t *)(mpower), sizeof(app_relay_power_config_t));
                // 刷新 EEPROM
                pconfig->enable = ENABLE;
                APP_CONFIG_relay_power_config_write(channel_id, pconfig);
            }
            mpower++;
        }
    }
    // 下发新的配置到检测控制板
    refresh_power_config = 1;
    return BSP_PROTOCOL_send_write_relay_power_config_message(packet);
}

int G2_SERVER_read_relay_power_config_message_process(g2_server_packet_pt packet)
{
    g2_server_relay_power_config_message_t message;
    app_relay_power_config_pt pconfig = NULL, mpower = NULL;
    message.probe_id = packet->buffer[0];
    mpower = &(message.power[0][0]);
    for (uint8_t relay_id = 0; relay_id < RELAY_SIZE; relay_id++)
    {
        for (uint8_t channel_id = 0; channel_id < RELAY_CHANNEL_SIZE; channel_id++)
        {
            APP_CONFIG_relay_power_config_init(channel_id, mpower);
            pconfig = &app_relay_power_config[relay_id][channel_id];
            if (ENABLE == pconfig->enable)
            {
                message.config |= (1 << (15 - channel_id));
            }
            memcpy((uint8_t *)(mpower), (uint8_t *)(pconfig), sizeof(app_relay_power_config_t));
            mpower++;
        }
    }
    return BSP_PROTOCOL_send_read_relay_power_config_message(packet, &message);
}

int APP_RELAY_parse_power_config(uint8_t relay_id, uint8_t channel_id, uint8_t *buffer)
{
    app_relay_power_config_pt pconfig = &app_relay_power_config[relay_id][channel_id];
    buffer[0] = (APP_RELAY_PHASE_1V1 == pconfig->phase_config ? 1 : 3);
    buffer[1] = pconfig->power;
    buffer[2] = pconfig->loss_phase_protect_enable;
    buffer[3] = pconfig->overload_protect_enable;
    buffer[4] = pconfig->ouv_protect_enable;
    return APP_OK;
}

#ifdef APP_REPLACE_RELAY_DEBUG
int APP_RELAY_refresh_power_config(void)
{
    if (refresh_power_config)
    {
        APP_CAN_send_load_type_config();
        refresh_power_config = 0;
    }
    return APP_OK;
}
#else
int APP_RELAY_refresh_power_config(void)
{
    int rc = APP_OK;
    if (refresh_power_config)
    {
        // 发送新的 power config 到检测控制板
        for (uint8_t relay_id = 0; relay_id < RELAY_SIZE; relay_id++)
        {
            for (uint8_t channel_id = 0; channel_id < RELAY_CHANNEL_SIZE; channel_id++)
            {
                rc = APP_CAN_send_power_config(relay_id, channel_id);
                if (APP_OK != rc)
                {
                    return APP_ERROR;
                }
            }
        }
        APP_CAN_send_load_type_config();
        refresh_power_config = 0;
    }
    return APP_OK;
}
#endif
int G2_SERVER_write_loader_config_message_process(g2_server_packet_pt packet)
{
    g2_server_loader_config_message_pt message = (g2_server_loader_config_message_pt)(packet->parsed);
    app_relay_power_config_pt pconfig = NULL, mpower = NULL;
    uint8_t mloader = APP_RELAY_LOADER_TYPE_AERATOR;
    for(uint8_t relay_id=0; relay_id<RELAY_SIZE; relay_id++)
    {
        mpower = &(message->power[0][0]);
        for(uint8_t channel_id=0; channel_id<RELAY_CHANNEL_SIZE; channel_id++)
        {
            if(config_bit_is_set(message->config, channel_id + 1))
            {
                pconfig = &app_relay_power_config[relay_id][channel_id];
                if (0 == mpower->power)
                {
                    mpower->power = (RELAY_CHANNEL_SIZE - 1 == channel_id ? 10 : 30);
                }
                mloader = mpower->loader_type;
                mpower->loader_type = (APP_RELAY_LOADER_TYPE_VVVF_AERATOR == mloader ? APP_RELAY_LOADER_TYPE_VVVF_AERATOR : APP_RELAY_LOADER_TYPE_AERATOR);
                memcpy((uint8_t *)(pconfig), (uint8_t *)(mpower), sizeof(app_relay_power_config_t));
            }
            pconfig->enable = ENABLE;
            APP_CONFIG_relay_power_config_write(channel_id, pconfig);
            mpower++;
        }
    }
    refresh_power_config = 1;
    return BSP_PROTOCOL_send_write_loader_config_message(packet);
}
int G2_SERVER_read_loader_config_message_process(g2_server_packet_pt packet)
{
    g2_server_loader_config_message_t message;
    app_relay_power_config_pt pconfig = NULL, mpower = NULL;
    message.probe_id = packet->buffer[0];
    mpower = &(message.power[0][0]);
    for(uint8_t relay_id=0; relay_id<RELAY_SIZE; relay_id++)
    {
        for(uint8_t channel_id=0; channel_id<RELAY_CHANNEL_SIZE; channel_id++)
        {
            APP_CONFIG_relay_power_config_init(channel_id, mpower);
            pconfig = &app_relay_power_config[relay_id][channel_id];
            if(ENABLE == pconfig->enable)
            {
                message.config |= (1 << (15 - channel_id));
            }
            memcpy((uint8_t *)(mpower), (uint8_t *)(pconfig), sizeof(app_relay_power_config_t));
            mpower++;
        }
    }
    return BSP_PROTOCOL_send_read_loader_config_message(packet, &message);
}


int APP_RELAY_parse_load_type(uint8_t *laod_type)
{
    app_relay_power_config_pt pconfig = NULL;
    for(uint8_t relay_id=0; relay_id<RELAY_SIZE; relay_id++)
    {
        for(uint8_t channel_id=0; channel_id<RELAY_CHANNEL_SIZE; channel_id++)
        {
            pconfig = &app_relay_power_config[relay_id][channel_id];
            laod_type[channel_id] = pconfig->loader_type;
        }
    }
    return APP_OK;
}

int APP_RELAY_refresh(void)
{
    uint8_t relay_id = 0;
    uint8_t channel_id = 0;
    app_config_time_t time;
    uint8_t week = 0;
    static size_t time_tick = 0;
    BSP_RTC_get(&time.year, &time.month, &time.day, &time.hour, &time.minute, &time.second, &week);
    if (HARDWARE_GET_TICK() - time_tick >= 3000)
    {
        APP_LOG_debug("now: 20%02d-%02d-%02d %02d:%02d:%02d\r\n", time.year, time.month, time.day, time.hour, time.minute, time.second);
        time_tick = HARDWARE_GET_TICK();
    }
    for (relay_id = 0; relay_id < RELAY_SIZE; relay_id++)
    {
        for (channel_id = 0; channel_id < RELAY_CHANNEL_SIZE; channel_id++)
        {
            _calculate_channel_status(relay_id, channel_id, &time);
        }
    }
    _app_relay_check_load_conflict();
    return APP_OK;
}

int APP_RELAY_check_refresh(void)
{
    uint8_t relay_id = 0;
    uint8_t channel_id = 0;
    app_relay_channel_pt channel = &app_relay_channels[0][0];
    for (relay_id = 0; relay_id < RELAY_SIZE; relay_id++)
    {
        for (channel_id = 0; channel_id < RELAY_CHANNEL_SIZE; channel_id++)
        {
            channel = &app_relay_channels[relay_id][channel_id];

            if (channel->calculate_status != channel->output_status)
            {
                if (0 == _get_broken_protect_channel_status(channel_id))
                {
                    return APP_ERROR;
                }
            }
        }
    }
    return APP_OK;
}

static int _set_channel(uint8_t relay_id, uint8_t channel_id, uint8_t *value)
{
    app_relay_channel_pt channel = &app_relay_channels[relay_id][channel_id];
    uint8_t output_status = (APP_CAN_read_output_channel(relay_id, channel_id) == 1 ? APP_RELAY_STATUS_SET_ON : APP_RELAY_STATUS_SET_OFF);

    if (APP_RELAY_STATUS_UNSET != channel->calculate_status)
    {
        // 本地手动操作导致输出改变
        if (channel->output_status != output_status)
        {
            if (APP_RELAY_STATUS_SET_ON == output_status)
            {
                APP_LOG_debug("get_ch <%d> = ON\r\n", (channel_id + 1));
            }
            else
            {
                APP_LOG_debug("get_ch <%d> = OFF\r\n", (channel_id + 1));
            }
            channel->changed = APP_RELAY_STATUS_CHANGED;
            channel->output_status = output_status;
            channel->status_change_tick = HARDWARE_GET_TICK();
            return APP_ERROR;
        }
        // 通道任务计算结果发生变化
        else if ((channel->output_status != channel->calculate_status) && (0 == _get_broken_protect_channel_status(channel_id)))
        {
            if (APP_RELAY_STATUS_SET_ON == channel->calculate_status)
            {
                APP_LOG_debug("set_ch <%d> = ON\r\n", (channel_id + 1));
            }
            else
            {
                APP_LOG_debug("set_ch <%d> = OFF\r\n", (channel_id + 1));
            }
            *value = channel->calculate_status == APP_RELAY_STATUS_SET_ON ? 1 : 0;
            channel->changed = APP_RELAY_STATUS_CHANGED;
            channel->output_status = channel->calculate_status;
            channel->status_change_tick = HARDWARE_GET_TICK();
            return APP_OK;
        }
    }
    return APP_ERROR;
}



uint8_t APP_RELAY_set_channels(void)
{
    int rc = 0;
    uint8_t relay_id = 0;
    uint8_t channel_id = 0;
    uint8_t change = 0;
    static uint8_t set_value[RELAY_SIZE][RELAY_CHANNEL_SIZE] = {0};

    if(GET_RELAY_BOARD_CHECK_RESULT() >= 0)
    {
        for (relay_id = 0; relay_id < RELAY_SIZE; relay_id++)
        {
            for (channel_id = 0; channel_id < RELAY_CHANNEL_SIZE; channel_id++)
            {
                rc = _set_channel(relay_id, channel_id, &set_value[relay_id][channel_id]);
                if (rc == APP_OK)
                {
                    command_try_cnt[channel_id]++;
                    if(command_try_cnt[channel_id] >= APP_RELAY_COMMAND_TRY_MAX)
                    {
                        command_try_cnt[channel_id] = APP_RELAY_COMMAND_TRY_MAX;
                        continue;
                    }
                    change++;
                    rc = APP_CAN_set_relay_channel(relay_id, channel_id, set_value[relay_id][channel_id]);
                    if (APP_OK != rc)
                    {
                        APP_LOG_debug("remote opreate <%d-%d> result unkonw\r\n", relay_id, channel_id);
                    }
                }
            }
        }
    }
    return change;
}

int APP_RELAY_read_channels(void)
{
    uint8_t relay_id = 0;
    uint8_t channel_id = 0;
    uint8_t key_status = 0;
    uint8_t device_load_status = 0; 
    int rc = 0;
    // uint8_t key_status = APP_RELAY_STATUS_UNSET;
    app_relay_channel_pt channel;

    for (relay_id = 0; relay_id < RELAY_SIZE; relay_id++)
    {
        for (channel_id = 0; channel_id < RELAY_CHANNEL_SIZE; channel_id++)
        {
            channel = &app_relay_channels[relay_id][channel_id];

            channel->relay_broken_status = APP_RELAY_STATUS_UNSET;
            channel->relay_leakage_status = APP_RELAY_STATUS_UNSET;
            channel->relay_loss_phase_status = APP_RELAY_STATUS_UNSET;
            channel->relay_overload_status = APP_RELAY_STATUS_UNSET;
            channel->relay_ouv_status = APP_RELAY_STATUS_UNSET;

            rc = APP_CAN_read_relay_channel(relay_id, channel_id);
            if (rc == 1)
            {
                if (channel->relay_output_status == APP_RELAY_STATUS_SET_OFF)
                {
                    channel->changed = APP_RELAY_STATUS_CHANGED;
                    APP_LOG_debug("relay_output_status ON\r\n");
                }
                channel->relay_output_status = APP_RELAY_STATUS_SET_ON;
            }
            if (rc == 0)
            {
                if (channel->relay_output_status == APP_RELAY_STATUS_SET_ON)
                {
                    channel->changed = APP_RELAY_STATUS_CHANGED;
                    APP_LOG_debug("relay_output_status OFF\r\n");
                }
                channel->relay_output_status = APP_RELAY_STATUS_SET_OFF;
            }
            rc = APP_CAN_read_relay_channel_broken(relay_id, channel_id);
            switch (rc)
            {
            case APP_RELAY_STATUS_OK:
                break;
            case APP_RELAY_STATUS_BROKEN:
                channel->relay_broken_status = APP_RELAY_STATUS_SET_ON;
                break;
            case APP_RELAY_STATUS_LOSS_PHASE:
                channel->relay_loss_phase_status = APP_RELAY_STATUS_SET_ON;
                break;
            case APP_RELAY_STATUS_OVERLOAD:
                channel->relay_overload_status = APP_RELAY_STATUS_SET_ON;
                break;
            case APP_RELAY_STATUS_OUV:
                channel->relay_ouv_status = APP_RELAY_STATUS_SET_ON;
                break;
            // case APP_RELAY_STATUS_LEAKAGE:
            //     channel->relay_leakage_status = APP_RELAY_STATUS_SET_ON;
            //     break;
            case APP_RELAY_STATUS_LOAD_ERROR:
                // channel->relay_broken_status = APP_RELAY_STATUS_SET_ON;
                device_load_status |= (0x1 << channel_id);
                // APP_LOG_debug("APP_RELAY_STATUS_LOAD_ERROR\r\n");
                break;
            case APP_RELAY_STATUS_PHASE_ERROR:
                // channel->relay_broken_status = APP_RELAY_STATUS_SET_ON;
                device_load_status |= (0x1 << channel_id);
                // APP_LOG_debug("APP_RELAY_STATUS_PHASE_ERROR\r\n");
                break;
            default:
                break;
            }
        }
    }
    if(device_load_status)
    {
        device_warnning_content = APP_DEVICE_WARNNING_LOADER;
    }
    else
    {
        device_warnning_content = APP_DEVICE_WARNNING_OK;
    }
    return APP_OK;
}

#define CHECK_CHANNEL_CHANGED_STATUS(field)          \
    if (channel->field != status)                    \
    {                                                \
        channel->changed = APP_RELAY_STATUS_CHANGED; \
        channel->field = status;                     \
        APP_LOG_debug(#field "changed\r\n");         \
    }                                                \
    status = APP_RELAY_STATUS_UNSET;

static uint32_t _update_broken_status(uint8_t channel_id, uint8_t broken_type, uint8_t broken_index, uint8_t protect_enable, uint8_t output, uint8_t *protect_status)
{
    static uint8_t broken_status[RELAY_CHANNEL_SIZE][APP_RELAY_STATUS_MAX - 1] = {APP_RELAY_STATUS_UNSET};
    uint32_t change = 0;
    if (broken_index >= APP_RELAY_STATUS_MAX - 1)
    {
        return 0;
    }
    if(broken_status[channel_id][broken_index] != broken_type)  // 故障状态发生变化 
    {
        change = 1 << broken_index;
        if(1 == broken_type)    // 当前是该故障发生 
        {
            APP_LOG_debug("ch[%d] broken_<%d> = true, output=%d\r\n", channel_id + 1, broken_index, output);
            if (broken_index >= 2)
            {
                if((APP_RELAY_PROTECT_ENABLE == protect_enable))    // 刚刚保护 
                {
                    if ((APP_RELAY_STATUS_SET_OFF == output) && (NULL != protect_status))
                    {
                        *protect_status = 1;
                    }
                }
                else if(_get_broken_protect_channel_status(channel_id) && (NULL != protect_status)) // 已经保护 
                {
                    *protect_status = 1;
                }
            }
        }
        else    // 当前是该故障解除 
        {
            APP_LOG_debug("ch[%d] broken_<%d> = null\r\n", channel_id + 1, broken_index);
        }
    }
    else
    {
        if((1 == broken_type) && (broken_index >= 2))
        {
            if ((APP_RELAY_PROTECT_ENABLE == protect_enable) && (APP_RELAY_STATUS_SET_OFF == output) && (1 != *protect_status) && (NULL != protect_status))
            {
                change = 1 << broken_index;
                *protect_status = 1;
            }
            else if (_get_broken_protect_channel_status(channel_id) && (NULL != protect_status))
            {
                *protect_status = 1;
            }
        }
    }
    broken_status[channel_id][broken_index] = broken_type;
    return change;
}

static void calculate_channel_manual_status(uint8_t channel_id, app_relay_channel_pt channel)
{
    uint8_t status = APP_RELAY_STATUS_UNSET;
    if ((APP_RELAY_STATUS_SET_ON == channel->remote_manual_status ||
         APP_RELAY_STATUS_SET_ON == channel->local_manual_status) ||
        (APP_RELAY_STATUS_SET_OFF == channel->remote_manual_status ||
         APP_RELAY_STATUS_SET_OFF == channel->local_manual_status))
    {
        status = APP_RELAY_STATUS_SET_ON;
    }
    CHECK_CHANNEL_CHANGED_STATUS(manual_status);
    uint8_t key_status = 0;
    key_status = APP_CAN_read_key_status_channel(0, channel_id);
    if (APP_RELAY_STATUS_ESTOP == key_status)
    {
        status = APP_RELAY_STATUS_SET_ON;
    }
    CHECK_CHANNEL_CHANGED_STATUS(all_off_manual_status);
    /*
        if (channel->output_status == APP_RELAY_STATUS_SET_ON &&
            channel->relay_output_status != APP_RELAY_STATUS_SET_ON &&
            (HARDWARE_GET_TICK() - channel->status_change_tick > 50000))
        {
            status = APP_RELAY_STATUS_SET_ON;
        }
        CHECK_CHANNEL_CHANGED_STATUS(relay_broken_status);
    */
}

static void calculate_channel_auto_status(uint8_t channel_id, app_relay_channel_pt channel)
{
    static uint8_t time_status_last[RELAY_CHANNEL_SIZE] = {APP_RELAY_STATUS_UNSET};
    static uint8_t limit_status_last[RELAY_CHANNEL_SIZE] = {APP_RELAY_STATUS_UNSET};

    if(time_status_last[channel_id] != channel->times_status)
    {
        channel->changed = APP_RELAY_STATUS_CHANGED;
        time_status_last[channel_id] = channel->times_status;
    }
    if(limit_status_last[channel_id] != channel->limit_status)
    {
        channel->changed = APP_RELAY_STATUS_CHANGED;
        limit_status_last[channel_id] = channel->limit_status;
    }
}

static void calculate_channel_broken_status(uint8_t channel_id, app_relay_channel_pt channel)
{
    uint32_t cnt = 0;
    app_relay_power_config_pt pconfig = &app_relay_power_config[0][channel_id];
    cnt |= _update_broken_status(channel_id, channel->relay_broken_status, 0, APP_RELAY_PROTECT_DISABLE, channel->output_status, NULL);
    cnt |= _update_broken_status(channel_id, channel->relay_leakage_status, 1, APP_RELAY_PROTECT_DISABLE, channel->output_status, NULL);
    cnt |= _update_broken_status(channel_id, channel->relay_loss_phase_status, 2, pconfig->loss_phase_protect_enable, channel->output_status, &(channel->relay_loss_phase_protect_status));
    cnt |= _update_broken_status(channel_id, channel->relay_overload_status, 3, pconfig->overload_protect_enable, channel->output_status, &(channel->relay_overload_protect_status));
    cnt |= _update_broken_status(channel_id, channel->relay_ouv_status, 5, pconfig->ouv_protect_enable, channel->output_status, &(channel->relay_ouv_protect_status));

    channel->changed |= (cnt ? APP_RELAY_STATUS_CHANGED : APP_RELAY_STATUS_UNCHANGED);
}

#define SET_RELAY_CHANNEL_STATUS(field)            \
    if (channel->field == APP_RELAY_STATUS_SET_ON) \
        message->field |= mask;

#define SET_RELAY_CHANNEL_CONTROL_STATUS(field)                                                  \
    if (channel->field == APP_RELAY_STATUS_SET_ON || channel->field == APP_RELAY_STATUS_SET_OFF) \
        message->field |= mask;

#define SET_RELAY_CHANNEL_CONTROL1_STATUS(field1, field2)                                         \
    if (channel->field2 == APP_RELAY_STATUS_SET_ON && channel->field1 != APP_RELAY_STATUS_SET_ON) \
        message->field2 |= mask;

void SET_RELAY_CHANNEL_BROKEN_STATUS(uint8_t field, uint8_t enable, uint8_t protect, uint8_t index, uint16_t *mfield, uint16_t *mprotect, g2_server_relay_data_message_pt message, uint16_t mask)
{
    if (field == APP_RELAY_STATUS_SET_ON)
    {
        uint8_t channel_id = config_bit_to_index(mask);
        channel_id = (0 == channel_id ? RELAY_CHANNEL_SIZE : channel_id - 1);
        if (((APP_RELAY_PROTECT_ENABLE == enable) || _get_broken_protect_channel_status(channel_id)) && ((2 == index) || (3 == index) || (5 == index)))
        {
            if ((0 != protect) && (NULL != mprotect)) // 使能保护且保护动作了
            {
                *mfield &= (~mask);
                *mprotect |= mask;
                message->limit_status &= (~mask);
                message->times_status &= (~mask);
                message->manual_status &= (~mask);
                message->remote_manual_status &= (~mask);
                message->local_manual_status &= (~mask);
                message->all_off_manual_status &= (~mask);
            }
            else if(NULL != mprotect)
            {
                *mfield |= mask;
                *mprotect &= (~mask);
            }
        }
        else
        {
            *mfield |= mask;
            if ((0 != protect) && (NULL != mprotect))
            {
                *mprotect &= (~mask);
            }
        }
        message->relay_output_status &= (~mask);
    }
    else
    {
        *mfield &= (~mask);
        if ((0 != protect) && (NULL != mprotect))
        {
            *mprotect &= (~mask);
        }
    }
}

/*
故障/保护 判定规则：
1、判定是否存在保护。若无保护，则判定2 
2、判定输出是否正常(relay_output_status)。若不正常（故障），则判定3 
3、判定通道的故障类型(relay_broken_status、relay_loss_phase_status、relay_overload_status、relay_ouv_status) 
*/ 
int APP_RELAY_check_server_packet(g2_server_relay_data_message_pt message)
{
    // 判定 3 种远程任务（本地手动任务、定时任务、智能任务 ）的相同 bit 位处是否同时存在 bit = 1 的情况 
    if((message->remote_manual_status + message->times_status + message->limit_status) != 
       (message->remote_manual_status | message->times_status | message->limit_status))
    {return APP_ERROR;}
    // 判定 2 种手动任务（本地手动、远程手动）的相同 bit 位处是否同时存在 bit = 1 的情况 
    if(message->local_manual_status & message->remote_manual_status)
    {return APP_ERROR;}
    // 判定 手动状态 = 本地手动 + 远程手动 
    if(message->manual_status != message->local_manual_status + message->remote_manual_status)
    {return APP_ERROR;}
    // 判定 4 种故障类型的相同 bit 位处是否同时存在 bit = 1 的情况 
    if((message->relay_broken_status + message->relay_loss_phase_status + message->relay_overload_status + message->relay_ouv_status) != 
       (message->relay_broken_status | message->relay_loss_phase_status | message->relay_overload_status | message->relay_ouv_status))
    {return APP_ERROR;}
    // 开关状态 = 正常输出 + (各种)故障 
    if(message->output_status != (message->relay_output_status  + 
                                 (message->relay_broken_status + message->relay_loss_phase_status  + message->relay_overload_status  + message->relay_ouv_status)))
    {return APP_ERROR;}
    return APP_OK;
}

int APP_RELAY_read_status_protocol(uint8_t relay_id, g2_server_relay_data_message_pt message, int *changed)
{
    if (relay_id > RELAY_SIZE || relay_id == 0)
    {
        return APP_ERROR;
    }
    uint8_t week;
    APP_RELAY_read_channels();
    G2_SERVER_relay_data_message_init(message);
    message->relay_id = relay_id;
    message->config = 0xffff;
    BSP_RTC_get(&message->time.year, &message->time.month, &message->time.day, &message->time.hour, &message->time.minute, &message->time.second, &week);
    app_relay_channel_pt channel;
    uint16_t mask = 0;
    *changed = 0;
    app_relay_power_config_pt pconfig = NULL;
    for (uint8_t channel_id = 0; channel_id < RELAY_CHANNEL_SIZE; channel_id++)
    {
        channel = &app_relay_channels[relay_id - 1][channel_id];
        mask = 0x8000 >> channel_id;
        calculate_channel_manual_status(channel_id, channel);
        calculate_channel_auto_status(channel_id, channel);
        calculate_channel_broken_status(channel_id, channel);
        if (channel->changed == APP_RELAY_STATUS_CHANGED)
        {
            *changed = 1;
        }
        channel->changed = APP_RELAY_STATUS_UNCHANGED;
        SET_RELAY_CHANNEL_CONTROL_STATUS(limit_status)
        SET_RELAY_CHANNEL_CONTROL_STATUS(times_status)
        SET_RELAY_CHANNEL_STATUS(manual_status)
        SET_RELAY_CHANNEL_CONTROL_STATUS(remote_manual_status)
        SET_RELAY_CHANNEL_CONTROL_STATUS(local_manual_status)
        SET_RELAY_CHANNEL_STATUS(all_off_manual_status)
        SET_RELAY_CHANNEL_STATUS(output_status)
        SET_RELAY_CHANNEL_STATUS(relay_output_status)
        pconfig = &app_relay_power_config[0][channel_id];
        SET_RELAY_CHANNEL_BROKEN_STATUS(channel->relay_broken_status, APP_RELAY_PROTECT_DISABLE, 0, 0, &(message->relay_broken_status), NULL, message, mask);
        SET_RELAY_CHANNEL_BROKEN_STATUS(channel->relay_leakage_status, APP_RELAY_PROTECT_DISABLE, 0, 1, &(message->relay_leakage_status), NULL, message, mask);
        SET_RELAY_CHANNEL_BROKEN_STATUS(channel->relay_loss_phase_status, pconfig->loss_phase_protect_enable, channel->relay_loss_phase_protect_status, 2, &(message->relay_loss_phase_status), &(message->relay_loss_phase_protect_status), message, mask);
        SET_RELAY_CHANNEL_BROKEN_STATUS(channel->relay_overload_status, pconfig->overload_protect_enable, channel->relay_overload_protect_status, 3, &(message->relay_overload_status), &(message->relay_overload_protect_status), message, mask);
        SET_RELAY_CHANNEL_BROKEN_STATUS(channel->relay_ouv_status, pconfig->ouv_protect_enable, channel->relay_ouv_protect_status, 5, &(message->relay_ouv_status), &(message->relay_ouv_protect_status), message, mask);
    }
    if(APP_OK != APP_RELAY_check_server_packet(message))
    {
        *changed = 0;
    }
    return APP_OK;
}

static int _read_channel_broken_status(void)
{
    static uint8_t last_status = 0;
    uint8_t current_status = 0;
    uint8_t relay_id = 1;
    app_relay_channel_pt channel;
    for (uint8_t channel_id = 0; channel_id < RELAY_CHANNEL_SIZE; channel_id++)
    {
        if(_get_broken_protect_channel_status(channel_id))
        {
            current_status |= (0x1 << channel_id);
        }
    }
    if(current_status != last_status)
    {
        last_status = current_status;
        return APP_ERROR;
    }
    else
    {
        last_status = current_status;
        return APP_OK;
    }
}

#include "app_network.h"
static uint8_t delay_resend_enviroment_data_flag = 0;
static uint32_t send_data_tick = 0;
int APP_RELAY_send_data_to_server(void)
{
    g2_server_relay_data_message_t message = {0};
    int changed = 0;
    int rc = APP_RELAY_read_status_protocol(RELAY_SIZE, &message, &changed);
    APP_CHECK_RC(rc)
    if (APP_SERVER_CONNECTED() && (changed || (HARDWARE_GET_TICK() - send_data_tick >= 10 * 60 * 1000)))
    {
        g2_server_packet_t packet;
        G2_SERVER_PACKET_init(&packet);
        send_data_tick = HARDWARE_GET_TICK();
        BSP_PROTOCOL_send_read_relay_data_message(&packet, &message);
        if(0 != has_recv_power_cfg_msg)
        {
            APP_NETWORK_update_environment_to_server(&packet);
        }
        // 判定是否发生保护 
        if(APP_ERROR == _read_channel_broken_status())
        {
            delay_resend_enviroment_data_flag = 1;
        }
    }

    if(APP_SERVER_CONNECTED() && (1 == delay_resend_enviroment_data_flag) && (HARDWARE_GET_TICK() - send_data_tick >= 3 * 1000))
    {
        g2_server_packet_t packet;
        G2_SERVER_PACKET_init(&packet);
        APP_NETWORK_update_environment_to_server(&packet);
        delay_resend_enviroment_data_flag = 0;
    }
    return PROTOCOL_OK;
}

int APP_RELAY_read_device_warnning(g2_server_device_warnning_message_pt message)
{
    if(APP_DEVICE_WARNNING_OK != device_warnning_content)
    {
        message->type = APP_DEVICE_WARNNING_RELAY;
        message->content = device_warnning_content;
    }
    return APP_OK;
}

int APP_RELAY_send_relay_error_to_server(void)
{
    if(IF_RELAY_BOARD_CHECK_ERROR())
    {
        device_warnning_content = APP_DEVICE_WARNNING_DISCONNECTED;
    }
    else
    {
        // 与检测控制板的通讯从 断开连接->恢复连接 
        if(APP_DEVICE_WARNNING_DISCONNECTED == device_warnning_content)
        {
            device_warnning_content = APP_DEVICE_WARNNING_OK;
            last_device_warnning_content = APP_DEVICE_WARNNING_OK; // 恢复连接时，不上报消息到服务器 
        }
    }
    
            
    if(APP_SERVER_CONNECTED())
    {
        if((device_warnning_content != last_device_warnning_content) && (APP_DEVICE_WARNNING_OK != device_warnning_content))
        {
            g2_server_packet_t packet;
            G2_SERVER_write_device_warnning_message_process(&packet);
            if(APP_DEVICE_WARNNING_DISCONNECTED == device_warnning_content)
            {
                APP_LOG_debug("relay board lost\r\n");
            }
            else if(APP_DEVICE_WARNNING_LOADER == device_warnning_content)
            {
                APP_LOG_debug("load conflict with phase\r\n");
            }
            last_device_warnning_content = device_warnning_content;
            return APP_OK;
        }
    }
    return APP_OK;
}

void APP_RELAY_task_run(void *argument)
{
    HARDWARE_OS_DELAY_MS(3000); // 防止时钟还没有运行起来
    APP_RELAY_refresh_power_config();
    HARDWARE_OS_DELAY_MS(9000); // 防止时钟还没有运行起来
    uint8_t set_channel_cnt = 0;
    for (;;)
    {
        APP_RELAY_refresh_power_config();
        APP_RELAY_refresh();
        do
        {
            set_channel_cnt = APP_RELAY_set_channels();
            if (set_channel_cnt)
            {
                HARDWARE_OS_DELAY_MS(2500 + set_channel_cnt * 200);
            }
            else
            {
                HARDWARE_OS_DELAY_MS(200);
            }
            APP_RELAY_refresh_power_config();
            APP_RELAY_refresh();
        } while (APP_RELAY_check_refresh() != APP_OK); // 3秒内没有新的操作变动，再上传服务器
        APP_RELAY_send_data_to_server();
        APP_RELAY_send_relay_error_to_server();
    }
}

#endif
