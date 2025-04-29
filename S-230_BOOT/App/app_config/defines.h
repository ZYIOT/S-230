#ifndef __APP_CONFIG_EEPROM_DEFINES_H
#define __APP_CONFIG_EEPROM_DEFINES_H
#ifdef __cplusplus
extern "C"
{
#endif

#include "app.h"

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
int APP_CONFIG_SystemRead( APP_CONFIG_System_pt message);
int APP_CONFIG_SystemWrite( APP_CONFIG_System_pt message);
int APP_CONFIG_SystemWriteDeviceID( APP_CONFIG_System_pt message, uint32_t deviceID);
int APP_CONFIG_SystemWriteSN( APP_CONFIG_System_pt message, uint8_t SN[5], uint8_t hardware[3]);
int APP_CONFIG_SystemLoad(void);
int APP_CONFIG_SystemRecovery(void);


#define DEFAULT_CRC32_VALUE		(0xFFFFFFFF)
#define APP_FIRMWARE_INFO		(0)
#define DOWNLOAD_FIRMWARE_INFO	(1)


typedef enum
{
	UNKONW_DEV = 0x0,			// 未知设备
	MASTER_CONTROL_DEV = 0x1,	// 主控设备
	COM_MODULE_DEV = 0x2,		// 通信模块设备
	RELAY_CONTROL_DEV = 0x3,	// 继电器控制板设备 
	DO_SENSOR_DEV = 0x11,		// 溶氧传感器设备
	PH_SENSOR_DEV = 0x21,		// pH传感器设备
}DEVICE_TYPE_e;

enum
{
	NEW_FILE = 0,	// 新文件
	OLD_FILE = 1,	// 旧文件
};

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
	RELAY_FIRMWARE, 			// RELAY 的固件 
    UNKONW_FIRMWARE = 255,      // 未知固件类型
}FILE_TYPE_e;

typedef enum
{
    UPDATE_FORCE_NOT = 0,       // 不强制升级
    UPDATE_FORCE,               // 强制升级
}UPDATE_FORCE_e;


enum
{
	APP1_FACTORY_FILE_NON_EXIST 	= 0x0,	// APP1 和 FACTORY 程序数据文件都不存在
	ONLY_APP1_FILE_EXIST 			= 0x1,	// 只存在 APP1
	ONLY_FACTORY_FILE_EXIST 		= 0x2,	// 只存在 FACTORY
	APP1_FACTORY_FILE_EXIST 		= 0x3,	// APP1 和 FACTORY 都存在
};


typedef struct
{
	uint8_t  deviceType;		// 目标设备类型
	uint8_t  versionH;			// 版本号 H
	uint8_t  versionM;			// 版本号 M
	uint8_t  versionL;			// 版本号 L
	uint32_t fileLen;			// 文件的总长度
	uint32_t fileCrc;			// 文件 CRC32 的值
}firmwareInfo_t, *firmwareInfo_pt;

typedef struct 
{
	uint32_t downloadLen;	// 已 download 的数据长度
	uint8_t updateResult;   // IAP 升级结果 
	uint8_t reserved[2];	// 保留 
    uint8_t firmwareType;   // 固件类型 
	firmwareInfo_t firmwareInfo;
}APP_CONFIG_FirmwareInfo_t, *APP_CONFIG_FirmwareInfo_pt;

int APP_CONFIG_FirmwareInfoInit(APP_CONFIG_FirmwareInfo_pt message);
int APP_CONFIG_FirmwareInfoRead(uint8_t infotype, APP_CONFIG_FirmwareInfo_pt message);
int APP_CONFIG_FirmwareInfoWrite(uint8_t infotype, APP_CONFIG_FirmwareInfo_pt message);
int APP_CONFIG_FirmwareInfoOverwrite(void);
int APP_CONFIG_FirmwareInfoLoad(void);
int APP_CONFIG_app_firmware_update_success(void);
int APP_CONFIG_app_firmware_update_fail(void);
int APP_CONFIG_AppFirmwareInfoRecovery(void);
int APP_CONFIG_DownloadFirmwareInfoRecovery(void);
int APP_CONFIG_firmware_check_info_valid(APP_CONFIG_FirmwareInfo_pt info);


extern APP_CONFIG_System_t g_appConfigSystem;

extern APP_CONFIG_FirmwareInfo_t g_appFirmwareInfo;
extern APP_CONFIG_FirmwareInfo_t g_downloadFirmwareInfo;

#ifdef __cplusplus
}
#endif
#endif
