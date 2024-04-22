#ifndef __APP_CONFIG_EEPROM_MAP_H
#define __APP_CONFIG_EEPROM_MAP_H
#ifdef __cplusplus
extern "C"
{
#endif

// 此处自定义 PAGE = 32Byte
#define EEPROM_HALF_MAX_PAGE	(2048)					// 0x800
/* EEPROM 中，已使用的各个部分的 Page 分布情况如下：
_____________________________________________________
| name                  | size  | range             |
|---------------------------------------------------|
| system                | 1     | 0x8               |
|---------------------------------------------------|
| device enable         | 1     | 0x9               |
|---------------------------------------------------|
| device connect        | 1     | 0xa               |
|---------------------------------------------------|
| probe enable          | 1     | 0x20              |
|---------------------------------------------------|
| pump status           | 1     | 0x28              |
|---------------------------------------------------|
| sensor                | 8     | 0x38 ~ 0x3f       |
|---------------------------------------------------|
| compensation          | 8     | 0x138 ~ 0x13f     |
|---------------------------------------------------|
| indicator alert       | 8     | 0x1c0 ~ 0x1c7     |
|---------------------------------------------------|
| limit                 | 32    | 0x2c0 ~ 0x2df     |
|---------------------------------------------------|
| relay                 | 1     | 0x5d8             |
|---------------------------------------------------|
| times enable          | 1     | 0x5e0             |
|---------------------------------------------------|
| times                 | 8     | 0x5e8 ~ 0x5ef     |
|---------------------------------------------------|
| manual relay          | 5     | 0x628 ~ 0x62c     |
|---------------------------------------------------|
| l6                    | 1     | 0x6a8             |
|---------------------------------------------------|
| relay power config    | 5     | 0x6b0 ~ 0x6b4     |
|---------------------------------------------------|
| diagnosis message     | 1     | 0x6b8             |
|---------------------------------------------------|
| firmware info         | 2     | 0x7fe ~ 0x7ff     |
|___________________________________________________|
*/
#define APP_CONFIG_EEPROM_SYSTEM_PAGE 8					// 0x8
#define APP_CONFIG_EEPROM_DEVICE_ENABLE_PAGE 9			// 0x9
#define APP_CONFIG_EEPROM_DEVICE_CONNECT_PAGE 10		// 0xa
#define APP_CONFIG_EEPROM_PROBE_ENABLE_PAGE 32			// 0x20
#define APP_CONFIG_EEPROM_PUMP_STATUS_PAGE 40			// 0x28
#define APP_CONFIG_EEPROM_SENSOR_PAGE 56				// 0x38
#define APP_CONFIG_EEPROM_COMPENSATION_PAGE 312			// 0x138
#define APP_CONFIG_EEPROM_INDICATOR_ALERT_PAGE 448		// 0x1C0
#define APP_CONFIG_EEPROM_LIMIT_PAGE 704				// 0x2C0
#define APP_CONFIG_EEPROM_RELAY_PAGE 1496				// 0x5D8
#define APP_CONFIG_EEPROM_TIMES_ENABLE_PAGE 1504		// 0x5E0
#define APP_CONFIG_EEPROM_TIMES_PAGE 1512				// 0x5E8
#define APP_CONFIG_EEPROM_MANUAL_RELAY_PAGE 1576		// 0x628
#define APP_CONFIG_EEPROM_L6_PAGE 1704					// 0x6A8
#define APP_CONFIG_EEPROM_RELAY_POWER_CONFIG_PAGE 1712  // 0x6B0 ~ 0x6B5 
#define APP_CONFIG_EEPROM_DIAGNOSIS_MESSAGE_PAGE  1720  // 0x6B8
#define APP_CONFIG_EEPROM_FIRMWARE_SETTING_INFO_PAGE		0x7FE
#define APP_CONFIG_EEPROM_FIRMWARE_SETTING_INFO_BACK_PAGE	0x7FF


#define APP_CONFIG_EEPROM_SYSTEM_DEVICE_ID_OFFSET 0
#define APP_CONFIG_EEPROM_SYSTEM_SN_OFFSET 4
#define APP_CONFIG_EEPROM_SYSTEM_HARDWARE_OFFSET 9

#define APP_CONFIG_EEPROM_TIME_YEAR_OFFSET 0
#define APP_CONFIG_EEPROM_TIME_MONTH_OFFSET 1
#define APP_CONFIG_EEPROM_TIME_DAY_OFFSET 2
#define APP_CONFIG_EEPROM_TIME_HOUR_OFFSET 3
#define APP_CONFIG_EEPROM_TIME_MINUTE_OFFSET 4
#define APP_CONFIG_EEPROM_TIME_SECOND_OFFSET 5

#define APP_CONFIG_EEPROM_DEVICE_ENABLE_ENABLE_OFFSET 0
#define APP_CONFIG_EEPROM_DEVICE_ENABLE_RECORD_ERROR_AT_OFFSET 1

#define APP_CONFIG_EEPROM_DEVICE_CONNECT_REASON_OFFSET 0

#define APP_CONFIG_EEPROM_PROBE_ENABLE_HAS_CONFIG_OFFSET 0
#define APP_CONFIG_EEPROM_PROBE_ENABLE_ENABLE_OFFSET 1
#define APP_CONFIG_EEPROM_PROBE_ENABLE_WARNING_OFFSET 2

#define APP_CONFIG_EEPROM_PUMP_STATUS_STATUS_OFFSET 0

#define APP_CONFIG_EEPROM_SENSOR_HAS_CONFIG_OFFSET 0
#define APP_CONFIG_EEPROM_SENSOR_ENABLE_OFFSET 1
#define APP_CONFIG_EEPROM_SENSOR_WARNING_OFFSET 2
#define APP_CONFIG_EEPROM_SENSOR_MANUFACTURER_OFFSET 3
#define APP_CONFIG_EEPROM_SENSOR_MODEL_OFFSET 4
#define APP_CONFIG_EEPROM_SENSOR_PROTOCOL_OFFSET 5

#define APP_CONFIG_EEPROM_COMPENSATION_HAS_CONFIG_OFFSET 0
#define APP_CONFIG_EEPROM_COMPENSATION_VALUE_OFFSET 1

#define APP_CONFIG_EEPROM_INDICATOR_ALERT_HAS_CONFIG_OFFSET 0
#define APP_CONFIG_EEPROM_INDICATOR_ALERT_ENABLE_OFFSET 1
#define APP_CONFIG_EEPROM_INDICATOR_ALERT_MAX_OFFSET 2
#define APP_CONFIG_EEPROM_INDICATOR_ALERT_MIN_OFFSET 4
#define APP_CONFIG_EEPROM_INDICATOR_ALERT_THRESHOLD_OFFSET 6

#define APP_CONFIG_EEPROM_CONFIG_ENABLE_HAS_CONFIG_OFFSET 0
#define APP_CONFIG_EEPROM_CONFIG_ENABLE_ENABLE_OFFSET 1

#define APP_CONFIG_EEPROM_LIMIT_HAS_CONFIG_OFFSET 0
#define APP_CONFIG_EEPROM_LIMIT_ENABLE_OFFSET 1
#define APP_CONFIG_EEPROM_LIMIT_RELAY_ID_OFFSET 2
#define APP_CONFIG_EEPROM_LIMIT_CHANNEL_OFFSET 3
#define APP_CONFIG_EEPROM_LIMIT_TYPE_OFFSET 5
#define APP_CONFIG_EEPROM_LIMIT_UP_LIMIT_OFFSET 6
#define APP_CONFIG_EEPROM_LIMIT_DOWN_LIMIT_OFFSET 8

#define APP_CONFIG_EEPROM_RELAY_HAS_CONFIG_OFFSET 0
#define APP_CONFIG_EEPROM_RELAY_ENABLE_OFFSET 1
#define APP_CONFIG_EEPROM_RELAY_WARNING_OFFSET 2

#define APP_CONFIG_EEPROM_TIMES_ENABLE_TASK_OFFSET 0

#define APP_CONFIG_EEPROM_TIMES_JOB_TYPE_OFFSET 0
#define APP_CONFIG_EEPROM_TIMES_JOB_START_HOUR_OFFSET 1
#define APP_CONFIG_EEPROM_TIMES_JOB_START_MINUTE_OFFSET 2
#define APP_CONFIG_EEPROM_TIMES_JOB_END_HOUR_OFFSET 3
#define APP_CONFIG_EEPROM_TIMES_JOB_END_MINUTE_OFFSET 4

#define APP_CONFIG_EEPROM_TIMES_HAS_LIMIT_OFFSET 0
#define APP_CONFIG_EEPROM_TIMES_CHANNEL_OFFSET 1
#define APP_CONFIG_EEPROM_TIMES_PROBE_ID_OFFSET 3
#define APP_CONFIG_EEPROM_TIMES_INDICATOR_ID_OFFSET 4
#define APP_CONFIG_EEPROM_TIMES_VALUE_OFFSET 5
#define APP_CONFIG_EEPROM_TIMES_THRESHOLD_OFFSET 7
#define APP_CONFIG_EEPROM_TIMES_EXECUTE_TYPE_OFFSET 9
#define APP_CONFIG_EEPROM_TIMES_TIMES_OFFSET 10

#define APP_CONFIG_EEPROM_MANUAL_RELAY_HAS_CONFIG_OFFSET 0
#define APP_CONFIG_EEPROM_MANUAL_RELAY_ENABLE_OFFSET 1
#define APP_CONFIG_EEPROM_MANUAL_RELAY_TYPE_OFFSET 2
#define APP_CONFIG_EEPROM_MANUAL_RELAY_START_OFFSET 3
#define APP_CONFIG_EEPROM_MANUAL_RELAY_END_OFFSET 9

#define APP_CONFIG_EEPROM_L6_HAS_CONFIG_OFFSET 0
#define APP_CONFIG_EEPROM_L6_REALEASE_OFFSET 1
#define APP_CONFIG_EEPROM_L6_READ_WAIT_OFFSET 2
#define APP_CONFIG_EEPROM_L6_READ_OFFSET 3
#define APP_CONFIG_EEPROM_L6_CHARGE_OFFSET 4
#define APP_CONFIG_EEPROM_L6_CHARGE_WAIT_OFFSET 5

// usr_firmware_info_t 中各个元素的偏移地址(注意每 30Bytes 增加 2Bytes 的偏移量)
#define APP_CONFIG_EEPROM_FIRMWARE_INFO_DOWNLOAD_LEN_OFFSET		0
#define APP_CONFIG_EEPROM_FIRMWARE_INFO_RESERVED_OFFSET			4
#define APP_CONFIG_EEPROM_FIRMWARE_INFO_DEVICE_TYPE_OFFSET		8
#define APP_CONFIG_EEPROM_FIRMWARE_INFO_VERSION_H_OFFSET		9
#define APP_CONFIG_EEPROM_FIRMWARE_INFO_VERSION_M_OFFSET		10
#define APP_CONFIG_EEPROM_FIRMWARE_INFO_VERSION_L_OFFSET		11
#define APP_CONFIG_EEPROM_FIRMWARE_INFO_FILE_LEN_OFFSET			12
#define APP_CONFIG_EEPROM_FIRMWARE_INFO_FILE_CRC_OFFSET			16


// app_relay_channel_power_t 中各个元素的偏移地址 
#define APP_CONFIG_EEPROM_RELAY_POWER_CONFIG_ENABLE_OFFSET                      0
#define APP_CONFIG_EEPROM_RELAY_POWER_CONFIG_PHASE_OFFSET                       1
#define APP_CONFIG_EEPROM_RELAY_POWER_CONFIG_POWER_OFFSET                       2
#define APP_CONFIG_EEPROM_RELAY_POWER_CONFIG_LOSSPHASE_PROTECT_ENABLE_OFFSET    3
#define APP_CONFIG_EEPROM_RELAY_POWER_CONFIG_OVERLOAD_PROTECT_ENABLE_OFFSET     4
#define APP_CONFIG_EEPROM_RELAY_POWER_CONFIG_OUV_PROTECT_ENABLE_OFFSET          5
#define APP_CONFIG_EEPROM_RELAY_POWER_CONFIG_LOADER_TYPE                        6


// DO 自诊断故障发生时间、类型、状态信息的元素偏移地址 
#define APP_CONFIG_EEPROM_DIAGNOSIS_TIME_YEAR_OFFSET        0
#define APP_CONFIG_EEPROM_DIAGNOSIS_TIME_MONTH_OFFSET       1
#define APP_CONFIG_EEPROM_DIAGNOSIS_TIME_DAY_OFFSET         2
#define APP_CONFIG_EEPROM_DIAGNOSIS_TIME_HOUR_OFFSET        3
#define APP_CONFIG_EEPROM_DIAGNOSIS_TIME_MINUTE_OFFSET      4
#define APP_CONFIG_EEPROM_DIAGNOSIS_TIME_SECOND_OFFSET      5
#define APP_CONFIG_EEPROM_DIAGNOSIS_TYPE                    6
#define APP_CONFIG_EEPROM_DIAGNOSIS_STATUS                  7


#ifdef __cplusplus
}
#endif
#endif
