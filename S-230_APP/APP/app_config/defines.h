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

int EEPROM_ReadBlock(uint16_t page, uint8_t *buf);
int EEPROM_WriteBlock(uint16_t page, uint8_t *buf);
 
typedef struct
{
    uint32_t deviceID;  /* 设备ID */
    uint8_t SN[5];  /* SN：年，月，串号,保留位 */
    uint8_t hardware[3];  /* 硬件版本 */
    uint8_t logLevel;  /* 日志等级 */
} APP_CONFIG_System_t, *APP_CONFIG_System_pt;

int APP_CONFIG_SystemInit(APP_CONFIG_System_pt message);
int APP_CONFIG_SystemWrite( APP_CONFIG_System_pt message);
int APP_CONFIG_SystemRead( APP_CONFIG_System_pt message);
int APP_CONFIG_SystemLoad(void);
int APP_CONFIG_SystemRecovery(void);
int APP_CONFIG_SystemWriteDeviceID( APP_CONFIG_System_pt message, uint32_t deviceID);
int APP_CONFIG_SystemWriteSN( APP_CONFIG_System_pt message, uint8_t SN[5], uint8_t hardware[3]);
 
typedef struct
{
    uint8_t year;  /* 年 */
    uint8_t month;  /* 月 */
    uint8_t day;  /* 日 */
    uint8_t hour;  /* 时 */
    uint8_t minute;  /* 分 */
    uint8_t second;  /* 秒 */
} APP_CONFIG_Time_t, *APP_CONFIG_Time_pt;

int APP_CONFIG_TimeInit(APP_CONFIG_Time_pt message);
 
typedef struct
{
    uint8_t enable;  /* 是否启用设备 */
    APP_CONFIG_Time_t recordErrorAt;  /* 错误清0时间 */
} APP_CONFIG_DeviceEnable_t, *APP_CONFIG_DeviceEnable_pt;

int APP_CONFIG_DeviceEnableInit(APP_CONFIG_DeviceEnable_pt message);
int APP_CONFIG_DeviceEnableWrite( APP_CONFIG_DeviceEnable_pt message);
int APP_CONFIG_DeviceEnableRead( APP_CONFIG_DeviceEnable_pt message);
int APP_CONFIG_DeviceEnableLoad(void);
int APP_CONFIG_DeviceEnableRecovery(void);
int APP_CONFIG_DeviceEnableWriteEnable( APP_CONFIG_DeviceEnable_pt message, uint8_t enable);
int APP_CONFIG_DeviceEnableWriteRecordErrorAt( APP_CONFIG_DeviceEnable_pt message, APP_CONFIG_Time_t recordErrorAt);
 
typedef struct
{
    uint8_t reason;  /* 握手原因 */
} APP_CONFIG_DeviceConnect_t, *APP_CONFIG_DeviceConnect_pt;

int APP_CONFIG_DeviceConnectInit(APP_CONFIG_DeviceConnect_pt message);
int APP_CONFIG_DeviceConnectWrite( APP_CONFIG_DeviceConnect_pt message);
int APP_CONFIG_DeviceConnectRead( APP_CONFIG_DeviceConnect_pt message);
int APP_CONFIG_DeviceConnectLoad(void);
int APP_CONFIG_DeviceConnectRecovery(void);
int APP_CONFIG_DeviceConnectWriteReason( APP_CONFIG_DeviceConnect_pt message, uint8_t reason);
 
typedef struct
{
    uint8_t hasConfig;  /* 是否有配置 */
    uint8_t enable;  /* 是否启用 */
    uint8_t warning;  /* 是否启用告警 */
} APP_CONFIG_ProbeEnable_t, *APP_CONFIG_ProbeEnable_pt;

int APP_CONFIG_ProbeEnableInit(APP_CONFIG_ProbeEnable_pt message);
int APP_CONFIG_ProbeEnableWrite(uint8_t probeID,  APP_CONFIG_ProbeEnable_pt message);
int APP_CONFIG_ProbeEnableRead(uint8_t probeID,  APP_CONFIG_ProbeEnable_pt message);
int APP_CONFIG_ProbeEnableLoad(void);
int APP_CONFIG_ProbeEnableRecovery(void);
int APP_CONFIG_ProbeEnableWriteHasConfig(uint8_t probeID,  APP_CONFIG_ProbeEnable_pt message, uint8_t hasConfig);
int APP_CONFIG_ProbeEnableWriteEnable(uint8_t probeID,  APP_CONFIG_ProbeEnable_pt message, uint8_t enable);
int APP_CONFIG_ProbeEnableWriteWarnning(uint8_t probeID,  APP_CONFIG_ProbeEnable_pt message, uint8_t warning);
 
typedef struct
{
    uint8_t status[16];  /* 水泵状态 */
} APP_CONFIG_PumpStatus_t, *APP_CONFIG_PumpStatus_pt;

int APP_CONFIG_PumpStatusInit(APP_CONFIG_PumpStatus_pt message);
int APP_CONFIG_PumpStatusWrite(uint8_t probeID,  APP_CONFIG_PumpStatus_pt message);
int APP_CONFIG_PumpStatusRead(uint8_t probeID,  APP_CONFIG_PumpStatus_pt message);
int APP_CONFIG_PumpStatusLoad(void);
int APP_CONFIG_PumpStatusRecovery(void);
int APP_CONFIG_PumpStatusWriteStatus(uint8_t probeID,  APP_CONFIG_PumpStatus_pt message, uint8_t status_index, uint8_t status);
 
typedef struct
{
    uint8_t hasConfig;  /* 是否有配置 */
    uint8_t enable;  /* 是否启用 */
    uint8_t warning;  /* 是否启用告警 */
    uint8_t manufacturer;  /* 探头厂商ID */
    uint8_t model;  /* 探头类型 */
    uint8_t protocol;  /* 协议ID */
} APP_CONFIG_Sensor_t, *APP_CONFIG_Sensor_pt;

int APP_CONFIG_SensorInit(APP_CONFIG_Sensor_pt message);
int APP_CONFIG_SensorWrite(uint8_t probeID, uint8_t sensorID,  APP_CONFIG_Sensor_pt message);
int APP_CONFIG_SensorRead(uint8_t probeID, uint8_t sensorID,  APP_CONFIG_Sensor_pt message);
int APP_CONFIG_SensorLoad(void);
int APP_CONFIG_SensorRecovery(void);
 
typedef struct
{
    uint8_t hasConfig;  /* 是否有配置 */
    uint16_t value;  /* 值 */
} APP_CONFIG_Compensation_t, *APP_CONFIG_Compensation_pt;

int APP_CONFIG_CompensationInit(APP_CONFIG_Compensation_pt message);
int APP_CONFIG_CompensationWrite(uint8_t probeID, uint8_t indicatorID,  APP_CONFIG_Compensation_pt message);
int APP_CONFIG_CompensationRead(uint8_t probeID, uint8_t indicatorID,  APP_CONFIG_Compensation_pt message);
int APP_CONFIG_CompensationLoad(void);
int APP_CONFIG_CompensationRecovery(void);
int APP_CONFIG_CompensationWriteData(uint8_t probeID, uint8_t indicatorID,  APP_CONFIG_Compensation_pt message, uint8_t hasConfig, uint16_t value);
 
typedef struct
{
    uint8_t hasConfig;  /* 是否有配置 */
    uint8_t enable;  /* 是否启用 */
    uint16_t max;  /* 上限 */
    uint16_t min;  /* 下限 */
    uint16_t threshold;  /* 窗口值 */
} APP_CONFIG_IndicatorAlert_t, *APP_CONFIG_IndicatorAlert_pt;

int APP_CONFIG_IndicatorAlertInit(APP_CONFIG_IndicatorAlert_pt message);
int APP_CONFIG_IndicatorAlertWrite(uint8_t probeID, uint8_t indicatorID,  APP_CONFIG_IndicatorAlert_pt message);
int APP_CONFIG_IndicatorAlertRead(uint8_t probeID, uint8_t indicatorID,  APP_CONFIG_IndicatorAlert_pt message);
int APP_CONFIG_IndicatorAlertLoad(void);
int APP_CONFIG_IndicatorAlertRecovery(void);
 
typedef struct
{
    uint8_t hasConfig;  /* 是否有配置 */
    uint8_t enable;  /* 是否启用 */
} APP_CONFIG_ConfigEnable_t, *APP_CONFIG_ConfigEnable_pt;

int APP_CONFIG_ConfigEnableInit(APP_CONFIG_ConfigEnable_pt message);
 
typedef struct
{
    uint8_t hasConfig;  /* 是否有配置 */
    uint8_t enable;  /* 是否启用 */
    uint8_t relayID;  /* 继电器ID */
    uint16_t channel;  /* 通道 */
    uint8_t type;  /* 控制类型 */
    uint16_t upLimit;  /* 上限 */
    uint16_t downLimit;  /* 下限 */
} APP_CONFIG_Limit_t, *APP_CONFIG_Limit_pt;

int APP_CONFIG_LimitInit(APP_CONFIG_Limit_pt message);
int APP_CONFIG_LimitWrite(uint8_t probeID, uint8_t indicatorID, uint8_t taskID,  APP_CONFIG_Limit_pt message);
int APP_CONFIG_LimitRead(uint8_t probeID, uint8_t indicatorID, uint8_t taskID,  APP_CONFIG_Limit_pt message);
int APP_CONFIG_LimitLoad(void);
int APP_CONFIG_LimitRecovery(void);
int APP_CONFIG_LimitWriteEnable(uint8_t probeID, uint8_t indicatorID, uint8_t taskID,  APP_CONFIG_Limit_pt message, uint8_t hasConfig, uint8_t enable);
 
typedef struct
{
    uint8_t hasConfig;  /* 是否有配置 */
    uint8_t enable;  /* 是否启用 */
    uint8_t warning;  /* 是否启用告警 */
} APP_CONFIG_Relay_t, *APP_CONFIG_Relay_pt;

int APP_CONFIG_RelayInit(APP_CONFIG_Relay_pt message);
int APP_CONFIG_RelayWrite(uint8_t relayID,  APP_CONFIG_Relay_pt message);
int APP_CONFIG_RelayRead(uint8_t relayID,  APP_CONFIG_Relay_pt message);
int APP_CONFIG_RelayLoad(void);
int APP_CONFIG_RelayRecovery(void);
int APP_CONFIG_RelayWriteHasConfig(uint8_t relayID,  APP_CONFIG_Relay_pt message, uint8_t hasConfig);
int APP_CONFIG_RelayWriteEnable(uint8_t relayID,  APP_CONFIG_Relay_pt message, uint8_t enable);
int APP_CONFIG_RelayWriteWarnning(uint8_t relayID,  APP_CONFIG_Relay_pt message, uint8_t warning);
 
typedef struct
{
    APP_CONFIG_ConfigEnable_t task[8];  /* 任务 */
} APP_CONFIG_TimeEnable_t, *APP_CONFIG_TimeEnable_pt;

int APP_CONFIG_TimesEnableInit(APP_CONFIG_TimeEnable_pt message);
int APP_CONFIG_TimesEnableWrite(uint8_t relayID,  APP_CONFIG_TimeEnable_pt message);
int APP_CONFIG_TimesEnableRead(uint8_t relayID,  APP_CONFIG_TimeEnable_pt message);
int APP_CONFIG_TimesEnableLoad(void);
int APP_CONFIG_TimesEnableRecovery(void);
int APP_CONFIG_TimesEnableWriteConfigEnable(uint8_t relayID,  APP_CONFIG_TimeEnable_pt message, uint8_t taskIndex, APP_CONFIG_ConfigEnable_t task);
 
typedef struct
{
    uint8_t type;  /* 控制类型|是否有配置 */
    uint8_t startHour;  /* 开启时间 时 */
    uint8_t startMinute;  /* 开启时间 分 */
    uint8_t endHour;  /* 结束时间 时 */
    uint8_t endMinute;  /* 结束时间 分 */
} APP_CONFIG_TimesJob_t, *APP_CONFIG_TimesJob_pt;

int APP_CONFIG_TimesJobInit(APP_CONFIG_TimesJob_pt message);
 
typedef struct
{
    uint8_t hasLimit;  /* 是否有门限 */
    uint16_t channel;  /* 通道 */
    uint8_t probeID;  /* 变送器ID */
    uint8_t indicatorID;  /* 水质数据ID */
    uint16_t value;  /* 门限 */
    uint16_t threshold;  /* 窗口值 */
    uint8_t executeType;  /* 执行类型 */
    APP_CONFIG_TimesJob_t times[4];  /* 时间段 */
} APP_CONFIG_Times_t, *APP_CONFIG_Times_pt;

int APP_CONFIG_TimesInit(APP_CONFIG_Times_pt message);
int APP_CONFIG_TimesWrite(uint8_t relayID, uint8_t taskID,  APP_CONFIG_Times_pt message);
int APP_CONFIG_TimesRead(uint8_t relayID, uint8_t taskID,  APP_CONFIG_Times_pt message);
int APP_CONFIG_TimesLoad(void);
int APP_CONFIG_TimesRecovery(void);
 
typedef struct
{
    uint8_t hasConfig;  /* 是否有配置 */
    uint8_t enable;  /* 是否启用 */
    uint8_t type;  /* 控制类型 */
    APP_CONFIG_Time_t start;  /* 开始时间 */
    APP_CONFIG_Time_t end;  /* 开始时间 */
} APP_CONFIG_ManualRelay_t, *APP_CONFIG_ManualRelay_pt;

int APP_CONFIG_ManualRelayInit(APP_CONFIG_ManualRelay_pt message);
int APP_CONFIG_ManualRelayWrite(uint8_t relayID, uint8_t channelID,  APP_CONFIG_ManualRelay_pt message);
int APP_CONFIG_ManualRelayRead(uint8_t relayID, uint8_t channelID,  APP_CONFIG_ManualRelay_pt message);
int APP_CONFIG_ManualRelayLoad(void);
int APP_CONFIG_ManualRelayRecovery(void);
int APP_CONFIG_ManualRelayWriteEnable(uint8_t relayID, uint8_t channelID,  APP_CONFIG_ManualRelay_pt message, uint8_t enable);
int APP_CONFIG_ManualRelayWriteTime(uint8_t relayID, uint8_t channelID,  APP_CONFIG_ManualRelay_pt message, uint8_t hasConfig, uint8_t enable, uint8_t type, APP_CONFIG_Time_t start, APP_CONFIG_Time_t end);


int APP_CONFIG_EepromLoadAll(void);
int APP_CONFIG_EepromRecoveryAll(void);


typedef struct
{
		uint8_t hasConfig;       /* 是否有配置 */
		uint8_t release;       //泄气时间
		uint8_t readWait;     //入水后多长时间允许读取数据（传感器在水中等待时长）
		uint8_t read;          //读取数据持续时间
		uint8_t charge;        //充气时间
		uint8_t chargeWait;   //气吹自动等待时长(传感器在空气中时长)
} APP_CONFIG_L6_t, *APP_CONFIG_L6_pt;

int APP_CONFIG_L6Init(APP_CONFIG_L6_pt message);
int APP_CONFIG_L6Write(uint8_t probeID,APP_CONFIG_L6_pt message);
int APP_CONFIG_L6Read(uint8_t probeID,APP_CONFIG_L6_pt message);
int APP_CONFIG_L6Load(void);
int APP_CONFIG_L6Recovery(void);


#define APP_FIRMWARE_INFO		(0)
#define DOWNLOAD_FIRMWARE_INFO	(1)

#define DEFAULT_CRC16_VALUE		(0xFFFF)
#define DEFAULT_CRC32_VALUE		(0xFFFFFFFF)
typedef struct
{
	uint8_t  deviceType;		// 目标设备类型
	uint8_t  versionH;			// 版本号 H
	uint8_t  versionM;			// 版本号 M
	uint8_t  versionL;			// 版本号 L
	uint32_t fileLen;			// 文件的总长度
	uint32_t fileCrc;			// 文件 CRC32 的值
}FirmwareInfo_t, *FirmwareInfo_pt;


typedef struct 
{
	uint32_t downloadLen;	// 已 download 的数据长度
    uint8_t updateResult;   // IAP 升级结果 
	uint8_t reserved[2];	// 保留 
    uint8_t firmwareType;   // 固件类型 
	FirmwareInfo_t firmwareInfo;	
}APP_CONFIG_FirmwareInfo_t, *APP_CONFIG_FirmwareInfo_pt;

int APP_CONFIG_FirmwareInfoInit(APP_CONFIG_FirmwareInfo_pt message);
int APP_CONFIG_FirmwareInfoRead(uint8_t infotype, APP_CONFIG_FirmwareInfo_pt message);
int APP_CONFIG_FirmwareInfoWrite(uint8_t infotype, APP_CONFIG_FirmwareInfo_pt message);
int APP_CONFIG_FirmwareInfoLoad(void);
int APP_CONFIG_AllFirmwareInfoRecovery(void);


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
    uint8_t phaseConfig;
    uint8_t power;  // 有功功率之和(合相功率)*0.1kW 
    uint8_t lossPhaseProtectEnable;
    uint8_t overloadProtectEnable;
    uint8_t ouvProtectEnable;
    uint8_t loaderType;    // 负载类型  0:普通负载设备, 1:变频负载设备 
}APP_RELAY_PowerConfig_t, *APP_RELAY_PowerConfig_pt;

typedef struct 
{
    APP_CONFIG_Time_t msgTime;
    uint8_t msgType;
    uint8_t msgStatus;
}APP_CONFIG_DiagnosisMessage_t, *APP_CONFIG_DiagnosisMessage_pt;

typedef struct 
{
    uint16_t valid;
    uint16_t max;
    uint16_t min;
}APP_CONFIG_RandOpt_t, *APP_CONFIG_RandOpt_pt;

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

typedef enum
{
	UNKONW_DEV = 0x0,			// 未知设备
	MASTER_CONTROL_DEV = 0x1,	// 主控设备
	COM_MODULE_DEV = 0x2,		// 通信模块设备
	DO_SENSOR_DEV = 0x11,		// 溶氧传感器设备
	PH_SENSOR_DEV = 0x21,		// pH传感器设备 
	RELAY_CONTROL_DEV = 0x31	// 继电器设备 
}DEVICE_TYPE_e;

typedef enum
{
    IAP_UPDATE_UNKNOW = 0,
    IAP_UPDATE_DOWNLOAD,    // 下载完成 
	IAP_UPDATE_FAIL,		// 升级失败 
	IAP_UPDATE_SUCESS,		// 升级成功 
}UPDATE_RESULT_e;

typedef enum
{
	MCU_BOOT_FIRMWARE = 0,		// 主控板 BOOT 固件
	MCU_APP_FIRMWARE,			// 主控板 APP 固件
	MCU_CFG_FIRMWARE,			// 主控板 CFG 固件
    MCU_FACTORY_FIRMWARE,		// 主控板 Factory 固件
    GPRS_MDL_FIRMWARE,			// GPRS 模块固件
	RELAY_APP_FIRMWARE, 	    // RELAY APP 固件 
	UNKONW_FIRMWARE = 255,		// 未知固件类型
}FILE_TYPE_e;

extern APP_CONFIG_System_t g_appConfigSystem;
extern APP_CONFIG_DeviceEnable_t g_appConfigDeviceEnable;
extern APP_CONFIG_DeviceConnect_t g_appConfigDeviceConnect;
extern APP_CONFIG_ProbeEnable_t g_appConfigProbeEnable[PROBE_SIZE];
extern APP_CONFIG_PumpStatus_t g_appConfigPumpStatus[PROBE_SIZE];
extern APP_CONFIG_Sensor_t g_appConfigSensor[PROBE_SIZE][PROBE_SENSOR_SIZE];
extern APP_CONFIG_Compensation_t g_appConfigCompensation[PROBE_SIZE][INDICATOR_SIZE];
extern APP_CONFIG_IndicatorAlert_t g_appConfigIndicatorAlert[PROBE_SIZE][INDICATOR_SIZE];
extern APP_CONFIG_Limit_t g_appConfigLimit[PROBE_SIZE][INDICATOR_SIZE][APP_CONFIG_MAX_LIMIT_TASK];
extern APP_CONFIG_Relay_t g_appConfigRelay[RELAY_SIZE];
extern APP_CONFIG_TimeEnable_t g_appConfigTimesEnable[RELAY_SIZE];
extern APP_CONFIG_Times_t g_appConfigTimes[RELAY_SIZE][APP_CONFIG_MAX_TIMES_TASK];
extern APP_CONFIG_ManualRelay_t g_appConfigManualRelay[RELAY_SIZE][RELAY_CHANNEL_SIZE];
extern APP_CONFIG_L6_t g_appConfigL6;
extern APP_CONFIG_FirmwareInfo_t g_appFirmwareInfo;
extern APP_CONFIG_FirmwareInfo_t g_downloadFirmwareInfo;
extern APP_RELAY_PowerConfig_t g_appRelayPowerConfig[RELAY_SIZE][RELAY_CHANNEL_SIZE];
extern APP_CONFIG_RandOpt_t g_appRandOpt;


#ifdef __cplusplus
}
#endif
#endif
