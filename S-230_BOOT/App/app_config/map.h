#ifndef __APP_CONFIG_EEPROM_MAP_H
#define __APP_CONFIG_EEPROM_MAP_H
#ifdef __cplusplus
extern "C"
{
#endif

#include "bsp_at24cm1.h"

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
#define APP_CONFIG_EEPROM_FIRMWARE_SETTING_INFO_PAGE		0x7FE
#define APP_CONFIG_EEPROM_FIRMWARE_SETTING_INFO_BACK_PAGE	0x7FF


// app_config_system_t 中各个元素的偏移地址 
#define APP_CONFIG_EEPROM_SYSTEM_DEVICE_ID_OFFSET               0
#define APP_CONFIG_EEPROM_SYSTEM_SN_OFFSET                      4
#define APP_CONFIG_EEPROM_SYSTEM_HARDWARE_OFFSET                9

// app_config_firmware_info_t 中各个元素的偏移地址(注意每 30Bytes 增加 2Bytes 的偏移量)
#define APP_CONFIG_EEPROM_FIRMWARE_INFO_DOWNLOAD_LEN_OFFSET		0
#define APP_CONFIG_EEPROM_FIRMWARE_INFO_RESERVED_OFFSET			4
#define APP_CONFIG_EEPROM_FIRMWARE_INFO_DEVICE_TYPE_OFFSET		8
#define APP_CONFIG_EEPROM_FIRMWARE_INFO_VERSION_H_OFFSET		9
#define APP_CONFIG_EEPROM_FIRMWARE_INFO_VERSION_M_OFFSET		10
#define APP_CONFIG_EEPROM_FIRMWARE_INFO_VERSION_L_OFFSET		11
#define APP_CONFIG_EEPROM_FIRMWARE_INFO_FILE_LEN_OFFSET			12
#define APP_CONFIG_EEPROM_FIRMWARE_INFO_FILE_CRC_OFFSET			16


// FLASH 中各个空间分布（需要与 BOOT 程序中的设置同步）
/*
____________________________________
| firwmare type | size |  address  |
____________________________________
| Bootloader    | 32K  | 0x8000000 |
____________________________________
| Download1		| 32K  | 0x8008000 |
____________________________________
| App			| 160K | 0x8010000 |
 ---------------------------------
| Reserved      | 32K  | 0x8038000 |
____________________________________
| Download      | 128K | 0x8040000 |
____________________________________
| Factory		| 128K | 0x8060000 |
____________________________________
 固件下载的位置 包含 Download 和 Download1 共 2 个区，其中 Download 存储前 128K 内容，Download1 存储后 32K 内容。
 运行的 App 总共占用 64K + 128K 空间，其中最后 32K 空间保留不使用。
 出厂时先烧录 App 固件，然后再烧录 Bootloader 固件。
 Bootloader 固件在首次运行时，将 App 固件拷贝到 Factory 区中。
 App 固件在 APP 程序中下载到 Download 区后，再通过 BOOT 程序拷贝到 App 区。
 Bootloader 固件在 APP 程序中下载到 Download 区后，再直接拷贝到 Bootloader 区。
 当连续多次通过 BOOT 程序重启 APP 程序仍然失败后，由 BOOT 程序将 Factory 区中的数据直接拷贝到 App 区中，然后再运行 APP 程序。
*/
#define FLASH_START_ADDR					(0x8000000)
#define FLASH_BOOT_MAX_SIZE					(0x8000)	// 32K
#define FLASH_BOOTLOADER_START_ADDR			(FLASH_START_ADDR)		                                // 0x8000000
#define FLASH_BOOTLOADER_SIZE				FLASH_BOOT_MAX_SIZE
#define FLASH_DOWNLOAD1_MAX_SIZE			(0x8000)	// 32K
#define FLASH_DOWNLOAD1_START_ADDR			(FLASH_BOOTLOADER_START_ADDR + FLASH_BOOTLOADER_SIZE)	// 0x8008000
#define FLASH_DOWNLOAD1_SIZE				FLASH_DOWNLOAD1_MAX_SIZE	
#define FLASH_APP_MAX_SIZE					(0x30000)	// 192K
#define FLASH_APP_START_ADDR				(FLASH_DOWNLOAD1_START_ADDR + FLASH_DOWNLOAD1_SIZE)	    // 0x8010000
#define FLASH_APP_SIZE						(0x28000)	// 160K
#define FLASH_DOWNLOAD_START_ADDR			(FLASH_APP_START_ADDR + FLASH_APP_MAX_SIZE)				// 0X8040000
#define FLASH_DOWNLOAD_SIZE					(0x20000)	// 128K
#define FLASH_FACTORY_START_ADDR			(FLASH_DOWNLOAD_START_ADDR + FLASH_DOWNLOAD_SIZE)		// 0x8060000
#define FLASH_FACTORY_SIZE					(0x20000)	// 128K
#define FLASH_END_ADDR						(0x807FFFF)


#ifdef __cplusplus
}
#endif
#endif
