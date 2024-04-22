#ifndef __APP_CONFIG_EEPROM_DEFINES_H
#define __APP_CONFIG_EEPROM_DEFINES_H
#ifdef __cplusplus
extern "C"
{
#endif

#include "app_config.h"

#define APP_CONFIG_ENABLED 1
#define APP_CONFIG_HAS_CONFIG 1
#define APP_CONFIG_MAX_TIMES_TASK 8
#define APP_CONFIG_MAX_LIMIT_TASK 4

int epprom_read_block(uint16_t page, uint8_t *buf);
int epprom_write_block(uint16_t page, uint8_t *buf);
 
typedef struct
{
    uint32_t device_id;  /* 设备ID */
    uint8_t SN[5];  /* SN：年，月，串号,保留位 */
    uint8_t hardware[3];  /* 硬件版本 */
    uint8_t log_level;  /* 日志等级 */
} app_config_system_t, *app_config_system_pt;

int APP_CONFIG_system_init(app_config_system_pt message);
int APP_CONFIG_system_write( app_config_system_pt message);
int APP_CONFIG_system_read( app_config_system_pt message);
int APP_CONFIG_system_load(void);
int APP_CONFIG_system_recovery(void);
int APP_CONFIG_system_write_device_id( app_config_system_pt message, uint32_t device_id);
int APP_CONFIG_system_write_sn( app_config_system_pt message, uint8_t SN[5], uint8_t hardware[3]);
 
typedef struct
{
    uint8_t year;  /* 年 */
    uint8_t month;  /* 月 */
    uint8_t day;  /* 日 */
    uint8_t hour;  /* 时 */
    uint8_t minute;  /* 分 */
    uint8_t second;  /* 秒 */
} app_config_time_t, *app_config_time_pt;

int APP_CONFIG_time_init(app_config_time_pt message);
 
typedef struct
{
    uint8_t enable;  /* 是否启用设备 */
    app_config_time_t record_error_at;  /* 错误清0时间 */
} app_config_device_enable_t, *app_config_device_enable_pt;

int APP_CONFIG_device_enable_init(app_config_device_enable_pt message);
int APP_CONFIG_device_enable_write( app_config_device_enable_pt message);
int APP_CONFIG_device_enable_read( app_config_device_enable_pt message);
int APP_CONFIG_device_enable_load(void);
int APP_CONFIG_device_enable_recovery(void);
int APP_CONFIG_device_enable_write_enable( app_config_device_enable_pt message, uint8_t enable);
int APP_CONFIG_device_enable_write_record_error_at( app_config_device_enable_pt message, app_config_time_t record_error_at);
 
typedef struct
{
    uint8_t reason;  /* 握手原因 */
} app_config_device_connect_t, *app_config_device_connect_pt;

int APP_CONFIG_device_connect_init(app_config_device_connect_pt message);
int APP_CONFIG_device_connect_write( app_config_device_connect_pt message);
int APP_CONFIG_device_connect_read( app_config_device_connect_pt message);
int APP_CONFIG_device_connect_load(void);
int APP_CONFIG_device_connect_recovery(void);
int APP_CONFIG_device_connect_write_reason( app_config_device_connect_pt message, uint8_t reason);
 
typedef struct
{
    uint8_t has_config;  /* 是否有配置 */
    uint8_t enable;  /* 是否启用 */
    uint8_t warning;  /* 是否启用告警 */
} app_config_probe_enable_t, *app_config_probe_enable_pt;

int APP_CONFIG_probe_enable_init(app_config_probe_enable_pt message);
int APP_CONFIG_probe_enable_write(uint8_t probe_id,  app_config_probe_enable_pt message);
int APP_CONFIG_probe_enable_read(uint8_t probe_id,  app_config_probe_enable_pt message);
int APP_CONFIG_probe_enable_load(void);
int APP_CONFIG_probe_enable_recovery(void);
int APP_CONFIG_probe_enable_write_has_config(uint8_t probe_id,  app_config_probe_enable_pt message, uint8_t has_config);
int APP_CONFIG_probe_enable_write_enable(uint8_t probe_id,  app_config_probe_enable_pt message, uint8_t enable);
int APP_CONFIG_probe_enable_write_warning(uint8_t probe_id,  app_config_probe_enable_pt message, uint8_t warning);
 
typedef struct
{
    uint8_t status[16];  /* 水泵状态 */
} app_config_pump_status_t, *app_config_pump_status_pt;

int APP_CONFIG_pump_status_init(app_config_pump_status_pt message);
int APP_CONFIG_pump_status_write(uint8_t probe_id,  app_config_pump_status_pt message);
int APP_CONFIG_pump_status_read(uint8_t probe_id,  app_config_pump_status_pt message);
int APP_CONFIG_pump_status_load(void);
int APP_CONFIG_pump_status_recovery(void);
int APP_CONFIG_pump_status_write_status(uint8_t probe_id,  app_config_pump_status_pt message, uint8_t status_index, uint8_t status);
 
typedef struct
{
    uint8_t has_config;  /* 是否有配置 */
    uint8_t enable;  /* 是否启用 */
    uint8_t warning;  /* 是否启用告警 */
    uint8_t manufacturer;  /* 探头厂商ID */
    uint8_t model;  /* 探头类型 */
    uint8_t protocol;  /* 协议ID */
} app_config_sensor_t, *app_config_sensor_pt;

int APP_CONFIG_sensor_init(app_config_sensor_pt message);
int APP_CONFIG_sensor_write(uint8_t probe_id, uint8_t sensor_id,  app_config_sensor_pt message);
int APP_CONFIG_sensor_read(uint8_t probe_id, uint8_t sensor_id,  app_config_sensor_pt message);
int APP_CONFIG_sensor_load(void);
int APP_CONFIG_sensor_recovery(void);
 
typedef struct
{
    uint8_t has_config;  /* 是否有配置 */
    uint16_t value;  /* 值 */
} app_config_compensation_t, *app_config_compensation_pt;

int APP_CONFIG_compensation_init(app_config_compensation_pt message);
int APP_CONFIG_compensation_write(uint8_t probe_id, uint8_t indicator_id,  app_config_compensation_pt message);
int APP_CONFIG_compensation_read(uint8_t probe_id, uint8_t indicator_id,  app_config_compensation_pt message);
int APP_CONFIG_compensation_load(void);
int APP_CONFIG_compensation_recovery(void);
int APP_CONFIG_compensation_write_data(uint8_t probe_id, uint8_t indicator_id,  app_config_compensation_pt message, uint8_t has_config, uint16_t value);
 
typedef struct
{
    uint8_t has_config;  /* 是否有配置 */
    uint8_t enable;  /* 是否启用 */
    uint16_t max;  /* 上限 */
    uint16_t min;  /* 下限 */
    uint16_t threshold;  /* 窗口值 */
} app_config_indicator_alert_t, *app_config_indicator_alert_pt;

int APP_CONFIG_indicator_alert_init(app_config_indicator_alert_pt message);
int APP_CONFIG_indicator_alert_write(uint8_t probe_id, uint8_t indicator_id,  app_config_indicator_alert_pt message);
int APP_CONFIG_indicator_alert_read(uint8_t probe_id, uint8_t indicator_id,  app_config_indicator_alert_pt message);
int APP_CONFIG_indicator_alert_load(void);
int APP_CONFIG_indicator_alert_recovery(void);
 
typedef struct
{
    uint8_t has_config;  /* 是否有配置 */
    uint8_t enable;  /* 是否启用 */
} app_config_config_enable_t, *app_config_config_enable_pt;

int APP_CONFIG_config_enable_init(app_config_config_enable_pt message);
 
typedef struct
{
    uint8_t has_config;  /* 是否有配置 */
    uint8_t enable;  /* 是否启用 */
    uint8_t relay_id;  /* 继电器ID */
    uint16_t channel;  /* 通道 */
    uint8_t type;  /* 控制类型 */
    uint16_t up_limit;  /* 上限 */
    uint16_t down_limit;  /* 下限 */
} app_config_limit_t, *app_config_limit_pt;

int APP_CONFIG_limit_init(app_config_limit_pt message);
int APP_CONFIG_limit_write(uint8_t probe_id, uint8_t indicator_id, uint8_t task_id,  app_config_limit_pt message);
int APP_CONFIG_limit_read(uint8_t probe_id, uint8_t indicator_id, uint8_t task_id,  app_config_limit_pt message);
int APP_CONFIG_limit_load(void);
int APP_CONFIG_limit_recovery(void);
int APP_CONFIG_limit_write_enable(uint8_t probe_id, uint8_t indicator_id, uint8_t task_id,  app_config_limit_pt message, uint8_t has_config, uint8_t enable);
 
typedef struct
{
    uint8_t has_config;  /* 是否有配置 */
    uint8_t enable;  /* 是否启用 */
    uint8_t warning;  /* 是否启用告警 */
} app_config_relay_t, *app_config_relay_pt;

int APP_CONFIG_relay_init(app_config_relay_pt message);
int APP_CONFIG_relay_write(uint8_t relay_id,  app_config_relay_pt message);
int APP_CONFIG_relay_read(uint8_t relay_id,  app_config_relay_pt message);
int APP_CONFIG_relay_load(void);
int APP_CONFIG_relay_recovery(void);
int APP_CONFIG_relay_write_has_config(uint8_t relay_id,  app_config_relay_pt message, uint8_t has_config);
int APP_CONFIG_relay_write_enable(uint8_t relay_id,  app_config_relay_pt message, uint8_t enable);
int APP_CONFIG_relay_write_warning(uint8_t relay_id,  app_config_relay_pt message, uint8_t warning);
 
typedef struct
{
    app_config_config_enable_t task[8];  /* 任务 */
} app_config_times_enable_t, *app_config_times_enable_pt;

int APP_CONFIG_times_enable_init(app_config_times_enable_pt message);
int APP_CONFIG_times_enable_write(uint8_t relay_id,  app_config_times_enable_pt message);
int APP_CONFIG_times_enable_read(uint8_t relay_id,  app_config_times_enable_pt message);
int APP_CONFIG_times_enable_load(void);
int APP_CONFIG_times_enable_recovery(void);
int APP_CONFIG_times_enable_write_config_enable(uint8_t relay_id,  app_config_times_enable_pt message, uint8_t task_index, app_config_config_enable_t task);
 
typedef struct
{
    uint8_t type;  /* 控制类型|是否有配置 */
    uint8_t start_hour;  /* 开启时间 时 */
    uint8_t start_minute;  /* 开启时间 分 */
    uint8_t end_hour;  /* 结束时间 时 */
    uint8_t end_minute;  /* 结束时间 分 */
} app_config_times_job_t, *app_config_times_job_pt;

int APP_CONFIG_times_job_init(app_config_times_job_pt message);
 
typedef struct
{
    uint8_t has_limit;  /* 是否有门限 */
    uint16_t channel;  /* 通道 */
    uint8_t probe_id;  /* 变送器ID */
    uint8_t indicator_id;  /* 水质数据ID */
    uint16_t value;  /* 门限 */
    uint16_t threshold;  /* 窗口值 */
    uint8_t execute_type;  /* 执行类型 */
    app_config_times_job_t times[4];  /* 时间段 */
} app_config_times_t, *app_config_times_pt;

int APP_CONFIG_times_init(app_config_times_pt message);
int APP_CONFIG_times_write(uint8_t relay_id, uint8_t task_id,  app_config_times_pt message);
int APP_CONFIG_times_read(uint8_t relay_id, uint8_t task_id,  app_config_times_pt message);
int APP_CONFIG_times_load(void);
int APP_CONFIG_times_recovery(void);
 
typedef struct
{
    uint8_t has_config;  /* 是否有配置 */
    uint8_t enable;  /* 是否启用 */
    uint8_t type;  /* 控制类型 */
    app_config_time_t start;  /* 开始时间 */
    app_config_time_t end;  /* 开始时间 */
} app_config_manual_relay_t, *app_config_manual_relay_pt;

int APP_CONFIG_manual_relay_init(app_config_manual_relay_pt message);
int APP_CONFIG_manual_relay_write(uint8_t relay_id, uint8_t channel_id,  app_config_manual_relay_pt message);
int APP_CONFIG_manual_relay_read(uint8_t relay_id, uint8_t channel_id,  app_config_manual_relay_pt message);
int APP_CONFIG_manual_relay_load(void);
int APP_CONFIG_manual_relay_recovery(void);
int APP_CONFIG_manual_relay_write_enable(uint8_t relay_id, uint8_t channel_id,  app_config_manual_relay_pt message, uint8_t enable);
int APP_CONFIG_manual_relay_write_time(uint8_t relay_id, uint8_t channel_id,  app_config_manual_relay_pt message, uint8_t has_config, uint8_t enable, uint8_t type, app_config_time_t start, app_config_time_t end);


int APP_CONFIG_eeprom_load_all(void);
int APP_CONFIG_eeprom_recovery_all(void);


typedef struct
{
		uint8_t has_config;       /* 是否有配置 */
		uint8_t release;       //泄气时间
		uint8_t read_wait;     //入水后多长时间允许读取数据（传感器在水中等待时长）
		uint8_t read;          //读取数据持续时间
		uint8_t charge;        //充气时间
		uint8_t charge_wait;   //气吹自动等待时长(传感器在空气中时长)
} app_config_l6_t, *app_config_l6_pt;

int APP_CONFIG_l6_init(app_config_l6_pt message);
int APP_CONFIG_l6_write(uint8_t probe_id,app_config_l6_pt message);
int APP_CONFIG_l6_read(uint8_t probe_id,app_config_l6_pt message);
int APP_CONFIG_l6_load(void);
int APP_CONFIG_l6_recovery(void);


#define APP_CONFIG_FIRMWARE_SETTING_INFO		(0)
#define APP_CONFIG_FIRMWARE_SETTING_INFO_BACK	(1)

#define DEFAULT_CRC16_VALUE		(0xFFFF)
#define DEFAULT_CRC32_VALUE		(0xFFFFFFFF)
typedef struct
{
	uint8_t  device_type;		// 目标设备类型
	uint8_t  version_h;			// 版本号 H
	uint8_t  version_m;			// 版本号 M
	uint8_t  version_l;			// 版本号 L
	uint32_t file_len;			// 文件的总长度
	uint32_t file_crc;			// 文件 CRC32 的值
}app_config_firmware_info_t, *app_config_firmware_info_pt;


typedef struct 
{
	uint32_t download_len;	// 已 download 的数据长度
	uint8_t reserved[4];	// 保留 reserved[0]:update_result
	app_config_firmware_info_t firmware_info;	
}app_config_firmware_setting_info_t, *app_config_firmware_setting_info_pt;

int APP_CONFIG_firmware_setting_info_init(app_config_firmware_setting_info_pt message);
int APP_CONFIG_firmware_setting_info_read(uint8_t infotype, app_config_firmware_setting_info_pt message);
int APP_CONFIG_firmware_setting_info_write(uint8_t infotype, app_config_firmware_setting_info_pt message);
int APP_CONFIG_firmware_setting_info_load(void);
int APP_CONFIG_firmware_setting_info_recovery(void);


#define APP_RELAY_PROTECT_DISABLE   (0)
#define APP_RELAY_PROTECT_ENABLE    (!APP_RELAY_PROTECT_DISABLE)
enum 
{
    APP_RELAY_PHASE_UNKNOW = 0,
    APP_RELAY_PHASE_1V1 = 1,
    APP_RELAY_PHASE_3V3 = 3,
    APP_RELAY_PHASE_3V4 = 4,
};
enum 
{
    APP_RELAY_LOADER_TYPE_AERATOR = 0,      // 普通增氧机/水泵/投饵机 
    APP_RELAY_LOADER_TYPE_VVVF_AERATOR = 1, // 变频增氧机 
};

typedef struct 
{
    uint8_t enable;
    uint8_t phase_config;
    uint8_t power;  // 有功功率之和(合相功率)*0.1kW 
    uint8_t loss_phase_protect_enable;
    uint8_t overload_protect_enable;
    uint8_t ouv_protect_enable;
    uint8_t loader_type;    // 负载类型  0:普通负载设备, 1:变频负载设备 
}app_relay_power_config_t, *app_relay_power_config_pt;

typedef struct 
{
    app_config_time_t msg_time;
    uint8_t msg_type;
    uint8_t msg_status;
}app_config_diagnosis_message_t, *app_config_diagnosis_message_pt;


enum 
{
    APP_DEVICE_WARNNING_CENTOR = 1,     // 主控设备 
    APP_DEVICE_WARNNING_RELAY = 2,      // 继电器板 
};

enum
{
    APP_DEVICE_WARNNING_OK = 0,         // 设备无告警 
    APP_DEVICE_POWER_DOWN,              // 断电 
    APP_DEVICE_WARNNING_LOADER,         // 设备进线与负载类型不匹配 
    APP_DEVICE_WARNNING_DISCONNECTED,   // 设备与继电器板失联 
};

extern app_config_system_t app_config_system;
extern app_config_device_enable_t app_config_device_enable;
extern app_config_device_connect_t app_config_device_connect;
extern app_config_probe_enable_t app_config_probe_enable[PROBE_SIZE];
extern app_config_pump_status_t app_config_pump_status[PROBE_SIZE];
extern app_config_sensor_t app_config_sensor[PROBE_SIZE][PROBE_SENSOR_SIZE];
extern app_config_compensation_t app_config_compensation[PROBE_SIZE][INDICATOR_SIZE];
extern app_config_indicator_alert_t app_config_indicator_alert[PROBE_SIZE][INDICATOR_SIZE];
extern app_config_limit_t app_config_limit[PROBE_SIZE][INDICATOR_SIZE][APP_CONFIG_MAX_LIMIT_TASK];
extern app_config_relay_t app_config_relay[RELAY_SIZE];
extern app_config_times_enable_t app_config_times_enable[RELAY_SIZE];
extern app_config_times_t app_config_times[RELAY_SIZE][APP_CONFIG_MAX_TIMES_TASK];
extern app_config_manual_relay_t app_config_manual_relay[RELAY_SIZE][RELAY_CHANNEL_SIZE];
extern app_config_l6_t app_config_l6;
extern app_config_firmware_setting_info_t firmware_setting_info1;
extern app_config_firmware_setting_info_t firmware_setting_info2;
extern app_relay_power_config_t app_relay_power_config[RELAY_SIZE][RELAY_CHANNEL_SIZE];


#ifdef __cplusplus
}
#endif
#endif
