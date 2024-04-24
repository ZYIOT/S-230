
#include "firmware_setting_info_test.h"
#if DEBUG_FIRMWARE_SETTING_INFO_TEST
#include "app_config/message.h"
#include "app_update_firmware.h"
#include "app_log.h"



const app_config_firmware_setting_info_t factory_firmware_settting_info = 
{
	.download_len = 0,
	.reserved[0] = 0,
	.reserved[1] = 0,
	.reserved[2] = 0,
	.reserved[3] = 0,
	.firmware_info = 
	{
		.device_type = MASTER_CONTROL_DEV,
		.version_h = 2,
		.version_m = 0,
		.version_l = 1,
		.file_len = FLASH_DOWNLOAD_SIZE,
		.file_crc = DEFAULT_CRC32_VALUE
	},
};

void firmware_setting_info_test_send(void)
{
    HARDWARE_HAL_DELAY_MS(200);
#if DEBUG_SYSCONFIG_SET
	#define SYSCONFIG_YEAR	(2021)
	#define SYSCONFIG_MON	(11)
	app_config_system_t usr_system = {0};
	usr_system.device_id = 0x001E8F20;
	usr_system.SN[0] = (SYSCONFIG_YEAR >>8) & 0xFF;
	usr_system.SN[1] = (SYSCONFIG_YEAR) & 0xFF;
	usr_system.SN[2] = (SYSCONFIG_MON);
	usr_system.SN[3] = (SN_CODE >> 8) & 0xFF;
	usr_system.SN[4] = (SN_CODE) & 0xFF;
	usr_system.hardware[0] = BOOT_HARDWARE_MAJOR;
	usr_system.hardware[1] = BOOT_HARDWARE_MINOR;
	usr_system.hardware[2] = BOOT_HARDWARE_REVISION;
	usr_system.log_level = BSP_LOG_DEFAULT_LEVEL;
	APP_CONFIG_system_write(&usr_system);
	
	APP_CONFIG_system_load();
	HARDWARE_HAL_DELAY_MS(20);
#endif

#if DEBUG_DOWNLOAD_CONTINUE_SET
	app_config_firmware_setting_info_t usr_setting_firmware_info = {0};
	usr_setting_firmware_info.download_len = 0x400;
	usr_setting_firmware_info.reserved[0] = 0;
	usr_setting_firmware_info.reserved[1] = 0;
	usr_setting_firmware_info.reserved[2] = 0;
	usr_setting_firmware_info.reserved[3] = 0;
	usr_setting_firmware_info.firmware_info.device_type = MASTER_CONTROL_DEV;
	usr_setting_firmware_info.firmware_info.version_h = APP_VERSION_H_DEFAULT;
	usr_setting_firmware_info.firmware_info.version_m = APP_VERSION_M_DEFAULT;
	usr_setting_firmware_info.firmware_info.version_l = APP_VERSION_L_DEFAULT;
	usr_setting_firmware_info.firmware_info.file_len = 0x8000;
	usr_setting_firmware_info.firmware_info.file_crc = 0x12345678;
	APP_CONFIG_firmware_setting_info_write(DOWNLOAD_FIRMWARE_INFO, &usr_setting_firmware_info);
#elif DEBUG_DOWNLOAD_FINISH_SET
	app_config_firmware_setting_info_t usr_setting_firmware_info = {0};
	usr_setting_firmware_info.download_len = FLASH_DOWNLOAD_SIZE;
	usr_setting_firmware_info.reserved[0] = 0;
	usr_setting_firmware_info.reserved[1] = 0;
	usr_setting_firmware_info.reserved[2] = 0;
	usr_setting_firmware_info.reserved[3] = 0;
	usr_setting_firmware_info.firmware_info.device_type = MASTER_CONTROL_DEV;
	usr_setting_firmware_info.firmware_info.version_h = 2;
	usr_setting_firmware_info.firmware_info.version_m = 0;
	usr_setting_firmware_info.firmware_info.version_l = 3;
	usr_setting_firmware_info.firmware_info.file_len = FLASH_DOWNLOAD_SIZE;
	usr_setting_firmware_info.firmware_info.file_crc = crc32((uint8_t *)FLASH_DOWNLOAD_START_ADDR, FLASH_DOWNLOAD_SIZE);
	APP_CONFIG_firmware_setting_info_write(DOWNLOAD_FIRMWARE_INFO, &usr_setting_firmware_info);

	usr_setting_firmware_info.firmware_info.version_l = 1;
	usr_setting_firmware_info.firmware_info.file_crc = DEFAULT_CRC32_VALUE;
	APP_CONFIG_firmware_setting_info_write(APP_FIRMWARE_INFO, &usr_setting_firmware_info);
#elif DEBUG_EEPROM_RECOVERY_SET
	APP_CONFIG_app_firmware_info_recovery();
	APP_CONFIG_download_firmware_info_recovery();
#else
#endif
    HARDWARE_HAL_DELAY_MS(20);
	APP_CONFIG_firmware_setting_info_load();

}


void firmware_setting_info_test_recv(void)
{
	app_config_firmware_setting_info_pt pinfo1 = &app_firmware_info;
	app_config_firmware_setting_info_pt pinfo2 = &download_firmware_info;

	if(download_firmware_info.download_len != download_firmware_info.firmware_info.file_len)
	{
		// 创建任务，主动发送继续下载帧，发送完成后主动删除任务
		APP_LOG_trace("create download continue thread...\r\n");
		while(1)
		{
			HARDWARE_HAL_DELAY_MS(3*1000);
			APP_LOG_trace("to be continue test...\r\n");
		}
	}
	else
	{
		if((0 == memcmp((uint8_t *)pinfo1, 
						(uint8_t *)pinfo2, 
						sizeof(app_config_firmware_setting_info_t)))
		&& (0 != memcmp((uint8_t *)&factory_firmware_settting_info, 
		 				(uint8_t *)pinfo2, 
						 sizeof(app_config_firmware_setting_info_t))))
		{
			APP_CONFIG_download_firmware_info_recovery();
			APP_LOG_trace("create flush ok thread...\r\n");
			while(1)
			{
				HARDWARE_HAL_DELAY_MS(3*1000);
				APP_LOG_trace("to be continue test...\r\n");
			}
		}
		else
		{
			APP_LOG_trace("create none thread...\r\n");
			while(1)
			{
				HARDWARE_HAL_DELAY_MS(3*1000);
				APP_LOG_trace("to be continue test...\r\n");
			}
		}
	}
}

#endif

