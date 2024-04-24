#ifndef __APP_CONFIG_EEPROM_DEFINES_H
#define __APP_CONFIG_EEPROM_DEFINES_H
#ifdef __cplusplus
extern "C"
{
#endif

#include "app.h"

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
int APP_CONFIG_system_read( app_config_system_pt message);
int APP_CONFIG_system_write( app_config_system_pt message);
int APP_CONFIG_system_write_device_id( app_config_system_pt message, uint32_t device_id);
int APP_CONFIG_system_write_sn( app_config_system_pt message, uint8_t SN[5], uint8_t hardware[3]);
int APP_CONFIG_system_load(void);
int APP_CONFIG_system_recovery(void);


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
}DEVICE_TYPE_E;

enum
{
	NEW_FILE = 0,	// 新文件
	OLD_FILE = 1,	// 旧文件
};

typedef enum
{
    IAP_UPDATE_UNKNOW = 0,
	IAP_UPDATE_FAIL = 1,		// 升级失败 
	IAP_UPDATE_SUCESS = 2,		// 升级成功 
}UPDATE_RESULT_E;

typedef enum
{
	UNKONW_FIRMWARE = 0,		// 未知固件类型
	MCU_BOOT_FIRMWARE = 1,		// BOOT 固件
	MCU_APP_FIRMWARE,			// APP 固件
	MCU_PARAMENT_FIRMWARE,		// PARAMENT 固件
    MCU_FACTORY_FIRMWARE,		// Factory 固件
    GPRS_MDL_FIRMWARE,			// GPRS 模块固件
	RELAY_APP_FIRMWARE,			// relay APP 固件
}FILE_TYPE_E;

typedef enum
{
    UPDATE_FORCE_NOT = 0,       // 不强制升级
    UPDATE_FORCE,               // 强制升级
}UPDATE_FORCE_E;


typedef enum
{
	APP1_FACTORY_FILE_NON_EXIST 	= 0x0,	// APP1 和 FACTORY 程序数据文件都不存在
	ONLY_APP1_FILE_EXIST 			= 0x1,	// 只存在 APP1
	ONLY_FACTORY_FILE_EXIST 		= 0x2,	// 只存在 FACTORY
	APP1_FACTORY_FILE_EXIST 		= 0x3,	// APP1 和 FACTORY 都存在
};


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
    uint8_t update_result;  // 0:升级成功 
	uint8_t reserved[2];	// 保留 
    uint8_t firmware_type;  // 固件类型 
	app_config_firmware_info_t firmware_info;
}app_config_firmware_setting_info_t, *app_config_firmware_setting_info_pt;

int APP_CONFIG_firmware_setting_info_init(app_config_firmware_setting_info_pt message);
int APP_CONFIG_firmware_setting_info_read(uint8_t infotype, app_config_firmware_setting_info_pt message);
int APP_CONFIG_firmware_setting_info_write(uint8_t infotype, app_config_firmware_setting_info_pt message);
int APP_CONFIG_firmware_setting_info_overwrite(void);
int APP_CONFIG_firmware_setting_info_load(void);
int APP_CONFIG_app_firmware_update_success(void);
int APP_CONFIG_app_firmware_update_fail(void);
int APP_CONFIG_app_firmware_info_recovery(void);
int APP_CONFIG_download_firmware_info_recovery(void);
int APP_CONFIG_firmware_check_info_valid(app_config_firmware_setting_info_pt info);


extern app_config_system_t app_config_system;

extern app_config_firmware_setting_info_t app_firmware_info;
extern app_config_firmware_setting_info_t download_firmware_info;

#ifdef __cplusplus
}
#endif
#endif
