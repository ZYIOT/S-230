#ifndef __G2_SERVER_MESSAGE_H
#define __G2_SERVER_MESSAGE_H
#ifdef __cplusplus
extern "C"
{
#endif
#include "g2_server_common.h"
#include "g2_server_packet.h"

typedef struct
{
    uint32_t device_id;  /* 设备ID */
} g2_server_device_id_message_t, *g2_server_device_id_message_pt;

WINAPI_EXPORT int WINAPI G2_SERVER_device_id_message_init(g2_server_device_id_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_device_id_message_serialize(uint8_t *bytes, g2_server_device_id_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_device_id_message_deserialize(uint8_t *bytes, g2_server_device_id_message_pt message);

typedef struct
{
    uint8_t hardware[3];  /* 硬件版本 */
    uint8_t firmware[3];  /* 固件版本 */
    uint8_t PN[10];  /* PN */
    uint8_t SN[11];  /* SN */
} g2_server_device_version_message_t, *g2_server_device_version_message_pt;

WINAPI_EXPORT int WINAPI G2_SERVER_device_version_message_init(g2_server_device_version_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_device_version_message_serialize(uint8_t *bytes, g2_server_device_version_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_device_version_message_deserialize(uint8_t *bytes, g2_server_device_version_message_pt message);

typedef struct
{
    uint8_t probe_id;  /* 变送器ID */
} g2_server_probe_id_message_t, *g2_server_probe_id_message_pt;

WINAPI_EXPORT int WINAPI G2_SERVER_probe_id_message_init(g2_server_probe_id_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_probe_id_message_serialize(uint8_t *bytes, g2_server_probe_id_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_probe_id_message_deserialize(uint8_t *bytes, g2_server_probe_id_message_pt message);

typedef struct
{
    uint8_t count;  /* 变送器ID个数 */
    uint8_t probe_id;  /* 变送器ID */
} g2_server_probe_ids_message_t, *g2_server_probe_ids_message_pt;

WINAPI_EXPORT int WINAPI G2_SERVER_probe_ids_message_init(g2_server_probe_ids_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_probe_ids_message_serialize(uint8_t *bytes, g2_server_probe_ids_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_probe_ids_message_deserialize(uint8_t *bytes, g2_server_probe_ids_message_pt message);

typedef struct
{
    uint8_t relay_id;  /* 继电器ID */
} g2_server_relay_id_message_t, *g2_server_relay_id_message_pt;

WINAPI_EXPORT int WINAPI G2_SERVER_relay_id_message_init(g2_server_relay_id_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_relay_id_message_serialize(uint8_t *bytes, g2_server_relay_id_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_relay_id_message_deserialize(uint8_t *bytes, g2_server_relay_id_message_pt message);

typedef struct
{
    uint8_t count;  /* 继电器ID个数 */
    uint8_t relay_id;  /* 继电器ID */
} g2_server_relay_ids_message_t, *g2_server_relay_ids_message_pt;

WINAPI_EXPORT int WINAPI G2_SERVER_relay_ids_message_init(g2_server_relay_ids_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_relay_ids_message_serialize(uint8_t *bytes, g2_server_relay_ids_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_relay_ids_message_deserialize(uint8_t *bytes, g2_server_relay_ids_message_pt message);

typedef struct
{
    uint8_t manufacturer;  /* 传感器厂商 */
    uint8_t type;  /* 传感器类型 */
} g2_server_sensor_config_data_message_t, *g2_server_sensor_config_data_message_pt;

WINAPI_EXPORT int WINAPI G2_SERVER_sensor_config_data_message_init(g2_server_sensor_config_data_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_sensor_config_data_message_serialize(uint8_t *bytes, g2_server_sensor_config_data_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_sensor_config_data_message_deserialize(uint8_t *bytes, g2_server_sensor_config_data_message_pt message);

typedef struct
{
    uint8_t probe_id;  /* 变送器ID */
    uint16_t config;  /* 变送器配置 */
    g2_server_sensor_config_data_message_t data[PROBE_SENSOR_SIZE];  /* 数据 */
} g2_server_sensor_config_message_t, *g2_server_sensor_config_message_pt;

WINAPI_EXPORT int WINAPI G2_SERVER_sensor_config_message_init(g2_server_sensor_config_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_sensor_config_message_serialize(uint8_t *bytes, g2_server_sensor_config_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_sensor_config_message_deserialize(uint8_t *bytes, g2_server_sensor_config_message_pt message);

typedef struct
{
    uint8_t oper;  /* 设置类型 */
    g2_server_sensor_config_message_t sensor;  /* 传感器配置 */
} g2_server_write_sensor_config_message_t, *g2_server_write_sensor_config_message_pt;

WINAPI_EXPORT int WINAPI G2_SERVER_write_sensor_config_message_init(g2_server_write_sensor_config_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_write_sensor_config_message_serialize(uint8_t *bytes, g2_server_write_sensor_config_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_write_sensor_config_message_deserialize(uint8_t *bytes, g2_server_write_sensor_config_message_pt message);

typedef struct
{
    uint8_t oper;  /* 设置类型 */
    uint8_t probe_id;  /* 变送器ID */
    uint16_t config;  /* 传感器配置 */
} g2_server_delete_sensor_config_message_t, *g2_server_delete_sensor_config_message_pt;

WINAPI_EXPORT int WINAPI G2_SERVER_delete_sensor_config_message_init(g2_server_delete_sensor_config_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_delete_sensor_config_message_serialize(uint8_t *bytes, g2_server_delete_sensor_config_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_delete_sensor_config_message_deserialize(uint8_t *bytes, g2_server_delete_sensor_config_message_pt message);

typedef struct
{
    uint16_t relay_channel;  /* 继电器通道 */
    uint16_t reversed_relay_channel;  /* 备用继电器通道 */
    uint8_t type;  /* 控制类型 */
    uint16_t max;  /* 门限最大值 */
    uint16_t min;  /* 门限最小值 */
} g2_server_sensor_limit_data_message_t, *g2_server_sensor_limit_data_message_pt;

WINAPI_EXPORT int WINAPI G2_SERVER_sensor_limit_data_message_init(g2_server_sensor_limit_data_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_sensor_limit_data_message_serialize(uint8_t *bytes, g2_server_sensor_limit_data_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_sensor_limit_data_message_deserialize(uint8_t *bytes, g2_server_sensor_limit_data_message_pt message);

typedef struct
{
    uint8_t relay_id;  /* 继电器ID */
    uint8_t gradient;  /* 梯度配置 */
    g2_server_sensor_limit_data_message_t data[8];  /* 数据 */
} g2_server_sensor_limit_gradient_message_t, *g2_server_sensor_limit_gradient_message_pt;

WINAPI_EXPORT int WINAPI G2_SERVER_sensor_limit_gradient_message_init(g2_server_sensor_limit_gradient_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_sensor_limit_gradient_message_serialize(uint8_t *bytes, g2_server_sensor_limit_gradient_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_sensor_limit_gradient_message_deserialize(uint8_t *bytes, g2_server_sensor_limit_gradient_message_pt message);

typedef struct
{
    uint8_t probe_id;  /* 变送器ID */
    uint16_t config;  /* 传感器配置 */
    g2_server_sensor_limit_gradient_message_t sensors[INDICATOR_SIZE];  /* 数据 */
} g2_server_sensor_limit_message_t, *g2_server_sensor_limit_message_pt;

WINAPI_EXPORT int WINAPI G2_SERVER_sensor_limit_message_init(g2_server_sensor_limit_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_sensor_limit_message_serialize(uint8_t *bytes, g2_server_sensor_limit_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_sensor_limit_message_deserialize(uint8_t *bytes, g2_server_sensor_limit_message_pt message);

typedef struct
{
    uint8_t oper;  /* 设置类型 */
    g2_server_sensor_limit_message_t sensor;  /* 变送器门限 */
} g2_server_write_sensor_limit_message_t, *g2_server_write_sensor_limit_message_pt;

WINAPI_EXPORT int WINAPI G2_SERVER_write_sensor_limit_message_init(g2_server_write_sensor_limit_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_write_sensor_limit_message_serialize(uint8_t *bytes, g2_server_write_sensor_limit_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_write_sensor_limit_message_deserialize(uint8_t *bytes, g2_server_write_sensor_limit_message_pt message);

typedef struct
{
    uint8_t oper;  /* 设置类型 */
    uint8_t probe_id;  /* 变送器ID */
    uint16_t config;  /* 传感器配置 */
    uint8_t relay_id;  /* 继电器ID */
    uint8_t gradient;  /* 梯度配置 */
} g2_server_delete_sensor_limit_message_t, *g2_server_delete_sensor_limit_message_pt;

WINAPI_EXPORT int WINAPI G2_SERVER_delete_sensor_limit_message_init(g2_server_delete_sensor_limit_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_delete_sensor_limit_message_serialize(uint8_t *bytes, g2_server_delete_sensor_limit_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_delete_sensor_limit_message_deserialize(uint8_t *bytes, g2_server_delete_sensor_limit_message_pt message);

typedef struct
{
    uint16_t max;  /* 门限最大值 */
    uint16_t min;  /* 门限最小值 */
    uint16_t threshold;  /* 门限窗口值 */
} g2_server_sensor_alert_data_message_t, *g2_server_sensor_alert_data_message_pt;

WINAPI_EXPORT int WINAPI G2_SERVER_sensor_alert_data_message_init(g2_server_sensor_alert_data_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_sensor_alert_data_message_serialize(uint8_t *bytes, g2_server_sensor_alert_data_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_sensor_alert_data_message_deserialize(uint8_t *bytes, g2_server_sensor_alert_data_message_pt message);

typedef struct
{
    uint8_t probe_id;  /* 变送器ID */
    uint16_t config;  /* 报警配置 */
    g2_server_sensor_alert_data_message_t data[INDICATOR_SIZE];  /* 数据 */
} g2_server_sensor_alert_message_t, *g2_server_sensor_alert_message_pt;

WINAPI_EXPORT int WINAPI G2_SERVER_sensor_alert_message_init(g2_server_sensor_alert_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_sensor_alert_message_serialize(uint8_t *bytes, g2_server_sensor_alert_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_sensor_alert_message_deserialize(uint8_t *bytes, g2_server_sensor_alert_message_pt message);

typedef struct
{
    uint8_t oper;  /* 设置类型 */
    g2_server_sensor_alert_message_t sensor;  /* 报警值 */
} g2_server_write_sensor_alert_message_t, *g2_server_write_sensor_alert_message_pt;

WINAPI_EXPORT int WINAPI G2_SERVER_write_sensor_alert_message_init(g2_server_write_sensor_alert_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_write_sensor_alert_message_serialize(uint8_t *bytes, g2_server_write_sensor_alert_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_write_sensor_alert_message_deserialize(uint8_t *bytes, g2_server_write_sensor_alert_message_pt message);

typedef struct
{
    uint8_t oper;  /* 设置类型 */
    uint8_t probe_id;  /* 变送器ID */
} g2_server_delete_sensor_alert_message_t, *g2_server_delete_sensor_alert_message_pt;

WINAPI_EXPORT int WINAPI G2_SERVER_delete_sensor_alert_message_init(g2_server_delete_sensor_alert_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_delete_sensor_alert_message_serialize(uint8_t *bytes, g2_server_delete_sensor_alert_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_delete_sensor_alert_message_deserialize(uint8_t *bytes, g2_server_delete_sensor_alert_message_pt message);

typedef struct
{
    uint8_t start_hour;  /* 开启时 */
    uint8_t start_minute;  /* 开启分 */
    uint8_t end_hour;  /* 关闭时 */
    uint8_t end_minute;  /* 关闭分 */
    uint8_t type;  /* 控制类型 */
} g2_server_relay_job_time_message_t, *g2_server_relay_job_time_message_pt;

WINAPI_EXPORT int WINAPI G2_SERVER_relay_job_time_message_init(g2_server_relay_job_time_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_relay_job_time_message_serialize(uint8_t *bytes, g2_server_relay_job_time_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_relay_job_time_message_deserialize(uint8_t *bytes, g2_server_relay_job_time_message_pt message);

typedef struct
{
    uint8_t has_limit;  /* 门限判断条件类型 */
    uint16_t relay_channel;  /* 继电器通道 */
    uint8_t probe_id;  /* 变送器ID */
    uint16_t sensor_config;  /* 变送器探头类型 */
    uint16_t value;  /* 相应类型门限值 */
    uint16_t threshold;  /* 窗口值 */
    uint8_t execute_type;  /* 执行类型 */
    uint8_t time_config;  /* 时间配置 */
    g2_server_relay_job_time_message_t times[4];  /* 数据 */
} g2_server_relay_jobs_task_message_t, *g2_server_relay_jobs_task_message_pt;

WINAPI_EXPORT int WINAPI G2_SERVER_relay_jobs_task_message_init(g2_server_relay_jobs_task_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_relay_jobs_task_message_serialize(uint8_t *bytes, g2_server_relay_jobs_task_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_relay_jobs_task_message_deserialize(uint8_t *bytes, g2_server_relay_jobs_task_message_pt message);

typedef struct
{
    uint8_t relay_id;  /* 继电器ID */
    uint16_t job_config;  /* 任务配置编号 */
    g2_server_relay_jobs_task_message_t tasks[8];  /* 任务 */
} g2_server_relay_jobs_message_t, *g2_server_relay_jobs_message_pt;

WINAPI_EXPORT int WINAPI G2_SERVER_relay_jobs_message_init(g2_server_relay_jobs_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_relay_jobs_message_serialize(uint8_t *bytes, g2_server_relay_jobs_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_relay_jobs_message_deserialize(uint8_t *bytes, g2_server_relay_jobs_message_pt message);

typedef struct
{
    uint8_t oper;  /* 设置类型 */
    g2_server_relay_jobs_message_t relay;  /* 继电器定时任务 */
} g2_server_write_relay_jobs_message_t, *g2_server_write_relay_jobs_message_pt;

WINAPI_EXPORT int WINAPI G2_SERVER_write_relay_jobs_message_init(g2_server_write_relay_jobs_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_write_relay_jobs_message_serialize(uint8_t *bytes, g2_server_write_relay_jobs_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_write_relay_jobs_message_deserialize(uint8_t *bytes, g2_server_write_relay_jobs_message_pt message);

typedef struct
{
    uint8_t oper;  /* 设置类型 */
    uint8_t relay_id;  /* 继电器ID */
    uint16_t config;  /* 任务配置 */
} g2_server_delete_relay_jobs_message_t, *g2_server_delete_relay_jobs_message_pt;

WINAPI_EXPORT int WINAPI G2_SERVER_delete_relay_jobs_message_init(g2_server_delete_relay_jobs_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_delete_relay_jobs_message_serialize(uint8_t *bytes, g2_server_delete_relay_jobs_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_delete_relay_jobs_message_deserialize(uint8_t *bytes, g2_server_delete_relay_jobs_message_pt message);

typedef struct
{
    uint8_t probe_id;  /* 变送器ID */
    uint16_t salinity;  /* 盐度值 */
} g2_server_salinity_message_t, *g2_server_salinity_message_pt;

WINAPI_EXPORT int WINAPI G2_SERVER_salinity_message_init(g2_server_salinity_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_salinity_message_serialize(uint8_t *bytes, g2_server_salinity_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_salinity_message_deserialize(uint8_t *bytes, g2_server_salinity_message_pt message);

typedef struct
{
    uint8_t probe_id;  /* 变送器ID */
    uint16_t config;  /* 校正配置 */
    uint8_t type;  /* 校正类型 */
    uint8_t params;  /* 校正参数 */
} g2_server_calibrate_message_t, *g2_server_calibrate_message_pt;

WINAPI_EXPORT int WINAPI G2_SERVER_calibrate_message_init(g2_server_calibrate_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_calibrate_message_serialize(uint8_t *bytes, g2_server_calibrate_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_calibrate_message_deserialize(uint8_t *bytes, g2_server_calibrate_message_pt message);

typedef struct
{
    uint8_t year;  /* 年 */
    uint8_t month;  /* 月 */
    uint8_t day;  /* 日 */
    uint8_t week;  /* 星期 */
    uint8_t hour;  /* 时 */
    uint8_t minute;  /* 分 */
    uint8_t second;  /* 秒 */
} g2_server_time_message_t, *g2_server_time_message_pt;

WINAPI_EXPORT int WINAPI G2_SERVER_time_message_init(g2_server_time_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_time_message_serialize(uint8_t *bytes, g2_server_time_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_time_message_deserialize(uint8_t *bytes, g2_server_time_message_pt message);

typedef struct
{
    uint8_t year;  /* 年 */
    uint8_t month;  /* 月 */
    uint8_t day;  /* 日 */
    uint8_t hour;  /* 时 */
    uint8_t minute;  /* 分 */
    uint8_t second;  /* 秒 */
} g2_server_time_without_week_message_t, *g2_server_time_without_week_message_pt;

WINAPI_EXPORT int WINAPI G2_SERVER_time_without_week_message_init(g2_server_time_without_week_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_time_without_week_message_serialize(uint8_t *bytes, g2_server_time_without_week_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_time_without_week_message_deserialize(uint8_t *bytes, g2_server_time_without_week_message_pt message);

typedef struct
{
    uint8_t relay_id;  /* 继电器ID */
    uint16_t relay_channel;  /* 继电器通道 */
    uint8_t type;  /* 控制类型 */
    uint16_t duration;  /* 控制时长（分） */
} g2_server_relay_manual_message_t, *g2_server_relay_manual_message_pt;

WINAPI_EXPORT int WINAPI G2_SERVER_relay_manual_message_init(g2_server_relay_manual_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_relay_manual_message_serialize(uint8_t *bytes, g2_server_relay_manual_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_relay_manual_message_deserialize(uint8_t *bytes, g2_server_relay_manual_message_pt message);

typedef struct
{
    uint8_t relay_id;  /* 继电器ID */
    uint16_t relay_channel;  /* 继电器通道 */
    uint16_t enable;  /* 使能位 */
} g2_server_relay_manual_enable_message_t, *g2_server_relay_manual_enable_message_pt;

WINAPI_EXPORT int WINAPI G2_SERVER_relay_manual_enable_message_init(g2_server_relay_manual_enable_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_relay_manual_enable_message_serialize(uint8_t *bytes, g2_server_relay_manual_enable_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_relay_manual_enable_message_deserialize(uint8_t *bytes, g2_server_relay_manual_enable_message_pt message);

typedef struct
{
    uint16_t config;  /* 数据配置 */
    uint8_t display;  /* 显示切换周期 */
    uint8_t display_refresh;  /* 屏幕数据刷新周期 */
    uint8_t push_data_interval;  /* GPRS数据上传周期 */
    uint8_t push_signal_interval;  /* GPRS信号上传周期 */
    uint8_t sensor_wireless_interval;  /* 无线变送器处理间隔 */
    uint8_t relay_wireless_interval;  /* 无线继电器处理间隔 */
    uint8_t relay_release_time;  /* 释放手动继电器默认时间 */
} g2_server_system_message_t, *g2_server_system_message_pt;

WINAPI_EXPORT int WINAPI G2_SERVER_system_message_init(g2_server_system_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_system_message_serialize(uint8_t *bytes, g2_server_system_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_system_message_deserialize(uint8_t *bytes, g2_server_system_message_pt message);

typedef struct
{
    uint8_t reason;  /* 连接原因 */
} g2_server_connect_message_t, *g2_server_connect_message_pt;

WINAPI_EXPORT int WINAPI G2_SERVER_connect_message_init(g2_server_connect_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_connect_message_serialize(uint8_t *bytes, g2_server_connect_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_connect_message_deserialize(uint8_t *bytes, g2_server_connect_message_pt message);

typedef struct
{
    uint8_t enable;  /* 设备使能 */
} g2_server_connect_enable_message_t, *g2_server_connect_enable_message_pt;

WINAPI_EXPORT int WINAPI G2_SERVER_connect_enable_message_init(g2_server_connect_enable_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_connect_enable_message_serialize(uint8_t *bytes, g2_server_connect_enable_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_connect_enable_message_deserialize(uint8_t *bytes, g2_server_connect_enable_message_pt message);

typedef struct
{
    uint8_t probe_id;  /* 变送器ID */
    uint16_t config;  /* 变送器配置 */
    uint16_t enable;  /* 使能位 */
} g2_server_sensor_limit_enable_message_t, *g2_server_sensor_limit_enable_message_pt;

WINAPI_EXPORT int WINAPI G2_SERVER_sensor_limit_enable_message_init(g2_server_sensor_limit_enable_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_sensor_limit_enable_message_serialize(uint8_t *bytes, g2_server_sensor_limit_enable_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_sensor_limit_enable_message_deserialize(uint8_t *bytes, g2_server_sensor_limit_enable_message_pt message);

typedef struct
{
    uint8_t type;  /* 设备类型 */
    uint8_t node_id;  /* 节点ID */
    uint8_t enable;  /* 使能位 */
} g2_server_device_enable_message_t, *g2_server_device_enable_message_pt;

WINAPI_EXPORT int WINAPI G2_SERVER_device_enable_message_init(g2_server_device_enable_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_device_enable_message_serialize(uint8_t *bytes, g2_server_device_enable_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_device_enable_message_deserialize(uint8_t *bytes, g2_server_device_enable_message_pt message);

typedef struct
{
    uint16_t config;  /* 数据配置 */
    uint8_t sensor_size;  /* 变送器终端最大挂载个数 */
    uint8_t probe_size_per_sensor;  /* 单个变送器终端最大支持探头个数 */
    uint8_t gradient_size_per_probe;  /* 单个探头最大支持梯度控制组数 */
    uint8_t calibrate_jobs_per_sensor;  /* 变送器终端最大支持校正时间组数 */
    uint8_t relay_size;  /* 继电器终端最大挂载个数 */
    uint8_t channel_size_per_relay;  /* 单个继电器终端最大支持继电器通道数 */
    uint8_t job_size_per_channel;  /* 单个继电器终端最大支持继电器固定时间任务组数 */
    uint8_t time_size_per_job;  /* 单个继电器终端最大支持继电器固定时间任务最大支持时间组数 */
} g2_server_device_capability_message_t, *g2_server_device_capability_message_pt;

WINAPI_EXPORT int WINAPI G2_SERVER_device_capability_message_init(g2_server_device_capability_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_device_capability_message_serialize(uint8_t *bytes, g2_server_device_capability_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_device_capability_message_deserialize(uint8_t *bytes, g2_server_device_capability_message_pt message);

typedef struct
{
    uint8_t relay_id;  /* 继电器ID */
    g2_server_time_without_week_message_t time;  /* 时间 */
    uint16_t config;  /* 数据配置 */
    uint16_t limit_status;  /* 智能控制通道状态 */
    uint16_t times_status;  /* 定时控制通道状态 */
    uint16_t manual_status;  /* 手动控制通道状态 */
    uint16_t remote_manual_status;  /* 远程手动控制 */
    uint16_t local_manual_status;  /* 本地手动控制 */
    uint16_t all_off_manual_status;  /* 手动全关状态 */
    uint16_t output_status;  /* 开关控制状态 */
    uint16_t relay_output_status;  /* 继电器输出正常状态 */
    uint16_t relay_broken_status;  /* 继电器故障状态 */
    uint16_t relay_leakage_status;  /* 漏电检测状态 */
    uint16_t relay_loss_phase_status;  /* 缺相检测状态 */
    uint16_t relay_overload_status;  /* 过载检测状态 */
    uint16_t relay_ouv_status;  /* 过欠压检测状态 */
    uint16_t relay_loss_phase_protect_status; /* 缺相保护状态 */
    uint16_t relay_overload_protect_status; /* 过载保护状态 */
    uint16_t relay_ouv_protect_status;  /* 过欠压保护状态 */
} g2_server_relay_data_message_t, *g2_server_relay_data_message_pt;

WINAPI_EXPORT int WINAPI G2_SERVER_relay_data_message_init(g2_server_relay_data_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_relay_data_message_serialize(uint8_t *bytes, g2_server_relay_data_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_relay_data_message_deserialize(uint8_t *bytes, g2_server_relay_data_message_pt message);


typedef struct
{
    uint16_t value;  /* 数据 */
    uint16_t value_mv;  /* MV值 */
    uint8_t probe_status;  /* 探头状态 */
    uint8_t data_status;  /* 数据状态 */
} g2_server_sensor_data_indicator_message_t, *g2_server_sensor_data_indicator_message_pt;

WINAPI_EXPORT int WINAPI G2_SERVER_sensor_data_indicator_message_init(g2_server_sensor_data_indicator_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_sensor_data_indicator_message_serialize(uint8_t *bytes, g2_server_sensor_data_indicator_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_sensor_data_indicator_message_deserialize(uint8_t *bytes, g2_server_sensor_data_indicator_message_pt message);

typedef struct
{
    uint8_t probe_id;  /* 变送器ID */
    g2_server_time_without_week_message_t time;  /* 时间 */
    uint16_t pump_status;  /* 水泵输出状态 */
    uint16_t pump_broken_status;  /* 水泵故障状态 */
    uint16_t pump_leakage_status;  /* 水泵漏电状态 */
    uint16_t config;  /* 传感器配置 */
    g2_server_sensor_data_indicator_message_t indicators[INDICATOR_SIZE];  /* 数据 */
} g2_server_sensor_data_message_t, *g2_server_sensor_data_message_pt;

WINAPI_EXPORT int WINAPI G2_SERVER_sensor_data_message_init(g2_server_sensor_data_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_sensor_data_message_serialize(uint8_t *bytes, g2_server_sensor_data_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_sensor_data_message_deserialize(uint8_t *bytes, g2_server_sensor_data_message_pt message);

typedef struct
{
    uint8_t sensor_count;  /* 变送器节点个数 */
    uint8_t relay_count;  /* 继电器节点个数 */
} g2_server_device_used_capability_message_t, *g2_server_device_used_capability_message_pt;

WINAPI_EXPORT int WINAPI G2_SERVER_device_used_capability_message_init(g2_server_device_used_capability_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_device_used_capability_message_serialize(uint8_t *bytes, g2_server_device_used_capability_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_device_used_capability_message_deserialize(uint8_t *bytes, g2_server_device_used_capability_message_pt message);

typedef struct
{
    uint8_t relay_id;  /* 继电器ID */
    uint8_t channel_count;  /* 继电器通道数 */
} g2_server_relay_config_message_t, *g2_server_relay_config_message_pt;

WINAPI_EXPORT int WINAPI G2_SERVER_relay_config_message_init(g2_server_relay_config_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_relay_config_message_serialize(uint8_t *bytes, g2_server_relay_config_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_relay_config_message_deserialize(uint8_t *bytes, g2_server_relay_config_message_pt message);

typedef struct
{
    uint8_t oper;  /* 设置类型 */
    g2_server_relay_config_message_t relay;  /* 继电器配置 */
} g2_server_write_relay_config_message_t, *g2_server_write_relay_config_message_pt;

WINAPI_EXPORT int WINAPI G2_SERVER_write_relay_config_message_init(g2_server_write_relay_config_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_write_relay_config_message_serialize(uint8_t *bytes, g2_server_write_relay_config_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_write_relay_config_message_deserialize(uint8_t *bytes, g2_server_write_relay_config_message_pt message);

typedef struct
{
    uint8_t oper;  /* 设置类型 */
    uint8_t relay_id;  /* 继电器ID */
} g2_server_delete_relay_config_message_t, *g2_server_delete_relay_config_message_pt;

WINAPI_EXPORT int WINAPI G2_SERVER_delete_relay_config_message_init(g2_server_delete_relay_config_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_delete_relay_config_message_serialize(uint8_t *bytes, g2_server_delete_relay_config_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_delete_relay_config_message_deserialize(uint8_t *bytes, g2_server_delete_relay_config_message_pt message);

typedef struct
{
    uint8_t relay_id;  /* 继电器ID */
    uint16_t config;  /* 固定时间任务配置 */
    uint16_t enable;  /* 使能位 */
} g2_server_relay_jobs_enable_message_t, *g2_server_relay_jobs_enable_message_pt;

WINAPI_EXPORT int WINAPI G2_SERVER_relay_jobs_enable_message_init(g2_server_relay_jobs_enable_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_relay_jobs_enable_message_serialize(uint8_t *bytes, g2_server_relay_jobs_enable_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_relay_jobs_enable_message_deserialize(uint8_t *bytes, g2_server_relay_jobs_enable_message_pt message);

typedef struct
{
    uint8_t probe_id;  /* 变送器ID */
    uint16_t channel;  /* 通道 */
    uint16_t status;  /* 开关标志 */
} g2_server_pump_message_t, *g2_server_pump_message_pt;

WINAPI_EXPORT int WINAPI G2_SERVER_pump_message_init(g2_server_pump_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_pump_message_serialize(uint8_t *bytes, g2_server_pump_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_pump_message_deserialize(uint8_t *bytes, g2_server_pump_message_pt message);

typedef struct
{
    uint8_t rssi;  /* 信号值 */
} g2_server_gprs_signal_message_t, *g2_server_gprs_signal_message_pt;

WINAPI_EXPORT int WINAPI G2_SERVER_gprs_signal_message_init(g2_server_gprs_signal_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_gprs_signal_message_serialize(uint8_t *bytes, g2_server_gprs_signal_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_gprs_signal_message_deserialize(uint8_t *bytes, g2_server_gprs_signal_message_pt message);


typedef struct
{
    uint8_t probe_id;  /* 变送器ID */
    uint16_t config;  /* 数据配置 */
    uint8_t release;  /* 泄气时长（秒） */
    uint8_t read_wait;  /* 入水后多长时间允许读取数据（秒） */
    uint8_t read;  /* 读取数据持续时长（秒） */
    uint8_t charge;  /* 充气时长（秒） */
    uint8_t charge_wait;  /* 气吹自动等待时长（分钟） */
} g2_server_l6_settings_message_t, *g2_server_l6_settings_message_pt;

WINAPI_EXPORT int WINAPI G2_SERVER_l6_settings_message_init(g2_server_l6_settings_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_l6_settings_message_serialize(uint8_t *bytes, g2_server_l6_settings_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_l6_settings_message_deserialize(uint8_t *bytes, g2_server_l6_settings_message_pt message);

typedef struct
{
    uint8_t probe_id;  /* 变送器ID */
    uint16_t config;  /* 数据配置 */
    uint8_t device_status;  /* 气吹装置状态（0为正常，1为异常） */
    uint8_t data_status;  /* 获取数据状态（0为允许读数据，1为不允许读数据） */
    uint8_t bump_status;  /* 装置中气泵状态（0为正常，1为异常） */
    uint8_t solenoid_status;  /* 装置中电池阀状态（0为正常，1为异常） */
    uint8_t liquid_level_status;  /* 装置中液位传感器状态（0为正常，1为异常） */
} g2_server_l6_status_message_t, *g2_server_l6_status_message_pt;

WINAPI_EXPORT int WINAPI G2_SERVER_l6_status_message_init(g2_server_l6_status_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_l6_status_message_serialize(uint8_t *bytes, g2_server_l6_status_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_l6_status_message_deserialize(uint8_t *bytes, g2_server_l6_status_message_pt message);

typedef struct
{
    uint16_t config;  /* 数据配置 */
    int16_t temperature;  /* 温度 */
    int16_t humidity;  /* 湿度 */
    int16_t voltage;  /* 电压 */
    uint16_t phase_voltage_A;
    uint16_t phase_voltage_B;
    uint16_t phase_voltage_C;
    uint16_t channel_current_1;
    uint16_t channel_current_2;
    uint16_t channel_current_3;
    uint16_t channel_current_4;
    uint16_t channel_current_5;
} g2_server_environment_message_t, *g2_server_environment_message_pt;

WINAPI_EXPORT int WINAPI G2_SERVER_environment_message_init(g2_server_environment_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_environment_message_serialize(uint8_t *bytes, g2_server_environment_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_environment_message_deserialize(uint8_t *bytes, g2_server_environment_message_pt message);


typedef struct
{
    uint8_t type;  /* 诊断类型 */
    uint8_t status;  /* 诊断状态 */
} g2_server_self_diagnosis_message_t, *g2_server_self_diagnosis_message_pt;

WINAPI_EXPORT int WINAPI G2_SERVER_self_diagnosis_message_init(g2_server_self_diagnosis_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_self_diagnosis_message_serialize(uint8_t *bytes, g2_server_self_diagnosis_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_self_diagnosis_message_deserialize(uint8_t *bytes, g2_server_self_diagnosis_message_pt message);


/* 固件升级相关数据解析函数 */
typedef app_config_firmware_info_t	(g2_server_firmware_start_message_t);
typedef app_config_firmware_info_pt	(g2_server_firmware_start_message_pt);

typedef struct
{
    uint32_t read_offset;		// 需要读取的文件地址偏移
    uint32_t read_datalen;		// 需要读取的文件数据长度
}g2_server_firmware_data_ask_t, *g2_server_firmware_data_ask_pt;


typedef struct
{
    uint32_t data_offset;		// 数据在文件中的偏移量
    uint32_t data_len;			// 有效数据长度
    uint8_t *data;				// 有效数据内容指针
}g2_server_firmware_data_message_t, *g2_server_firmware_data_message_pt;


typedef struct
{
    uint8_t file_process_stage;		// 文件处理阶段,1:下载阶段; 2:升级阶段
    uint8_t file_process_result;	// 文件处理结果,1:完成; 2:失败
}g2_server_firmware_update_ack_t, *g2_server_firmware_update_ack_pt;

WINAPI_EXPORT int WINAPI G2_SERVER_firmware_start_message_init(void);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_firmware_start_message_serialize(uint8_t *bytes, void *message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_firmware_start_message_deserialize(uint8_t *bytes, g2_server_firmware_start_message_pt message);


WINAPI_EXPORT int WINAPI G2_SERVER_firmware_data_message_init(void);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_send_firmware_data_ask_message_serialize(uint8_t *bytes, g2_server_firmware_data_ask_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_firmware_data_message_deserialize(uint8_t *bytes, g2_server_firmware_data_message_pt message);


WINAPI_EXPORT int WINAPI G2_SERVER_send_firmware_update_ack_message_init(void);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_send_firmware_update_ack_message_serialize(uint8_t *bytes, g2_server_firmware_update_ack_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_send_firmware_update_ack_message_deserialize(void);
    
typedef struct 
{
    uint8_t hardware[3];  /* 硬件版本 */
    uint8_t firmware[3];  /* 固件版本 */
    uint8_t PN[10];  /* PN */
    uint8_t SN[8];  /* SN */
}g2_server_sensor_info_message_t, *g2_server_sensor_info_message_pt;

typedef struct
{
    uint8_t type;  /* 传感器类型 */
    uint8_t probe_id;  /* 变送器ID */
    uint16_t config;  /* 变送器配置 */
    g2_server_sensor_info_message_t sensors_info[PROBE_SENSOR_SIZE];  /* 信号值 */
} g2_server_device_sensors_message_t, *g2_server_device_sensors_message_pt;	
WINAPI_EXPORT int WINAPI G2_SERVER_device_sensors_message_init(g2_server_device_sensors_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_device_sensors_message_serialize(uint8_t *bytes, g2_server_device_sensors_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_device_sensors_message_deserialize(uint8_t *bytes, g2_server_device_sensors_message_pt message);

typedef struct 
{
    uint8_t len;
    uint8_t manufacturer;
    uint8_t iccid[22];
}g2_server_sim_iccid_message_t, *g2_server_sim_iccid_message_pt;

WINAPI_EXPORT int WINAPI G2_SERVER_iccid_message_init(g2_server_sim_iccid_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_iccid_message_serialize(uint8_t *bytes, g2_server_sim_iccid_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_iccid_message_deserialize(uint8_t *bytes, g2_server_sim_iccid_message_pt message);

typedef app_relay_power_config_t g2_server_channel_power_t;
typedef app_relay_power_config_pt g2_server_channel_power_pt;

typedef struct 
{
    uint8_t probe_id;  /* 变送器ID */
    uint16_t config;  /* 变送器配置 */
    g2_server_channel_power_t power[RELAY_SIZE][RELAY_CHANNEL_SIZE];
}g2_server_relay_power_config_message_t, *g2_server_relay_power_config_message_pt;
WINAPI_EXPORT int WINAPI G2_SERVER_relay_power_config_message_init(g2_server_relay_power_config_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_relay_power_config_message_serialize(uint8_t *bytes, g2_server_relay_power_config_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_relay_power_config_message_deserialize(uint8_t *bytes, g2_server_relay_power_config_message_pt message);


typedef g2_server_relay_power_config_message_t g2_server_loader_config_message_t;
typedef g2_server_relay_power_config_message_pt g2_server_loader_config_message_pt;

WINAPI_EXPORT int WINAPI G2_SERVER_loader_config_message_init(g2_server_loader_config_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_loader_config_message_serialize(uint8_t *bytes, g2_server_loader_config_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_loader_config_message_deserialize(uint8_t *bytes, g2_server_loader_config_message_pt message);

typedef struct 
{
    uint8_t type;       // 告警类型 
    uint8_t content;    // 告警内容 
}g2_server_device_warnning_message_t, *g2_server_device_warnning_message_pt;
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_device_warnning_message_init(g2_server_device_warnning_message_pt message);
WINAPI_EXPORT uint16_t WINAPI G2_SERVER_device_warnning_message_serialize(uint8_t *bytes, g2_server_device_warnning_message_pt message);

#ifdef __cplusplus
}
#endif
#endif
