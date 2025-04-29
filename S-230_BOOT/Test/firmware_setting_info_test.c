
#include "bsp_board_debug.h"
#ifdef DEBUG_FIRMWARE_SETTING_INFO_TEST
#include "firmware_setting_info_test.h"
#include "board_config.h"
#include "app_config/message.h"
#include "app_update_firmware.h"
#include "app_log.h"



const APP_CONFIG_FirmwareInfo_t factory_firmware_settting_info = 
{
	.downloadLen = 0,
	.reserved[0] = 0,
	.reserved[1] = 0,
	.firmware_type = MCU_APP_FIRMWARE,
	.update_result = IAP_UPDATE_SUCESS,
	.firmwareInfo = 
	{
		.deviceType = MASTER_CONTROL_DEV,
		.versionH = APP_VERSION_H_DEFAULT,
		.versionM = APP_VERSION_M_DEFAULT,
		.versionL = APP_VERSION_L_DEFAULT,
		.fileLen = FLASH_DOWNLOAD_SIZE,
		.fileCrc = DEFAULT_CRC32_VALUE
	},
};

void FirmwareSettingInfoTestSend(void)
{
    HARDWARE_HAL_DELAY_MS(200);
#if DEBUG_SYSCONFIG_SET
	#define SYSCONFIG_YEAR	(2021)
	#define SYSCONFIG_MON	(11)
	APP_CONFIG_System_t usr_system = {0};
	usr_system.deviceID = 0x001E8F20;
	usr_system.SN[0] = (SYSCONFIG_YEAR >>8) & 0xFF;
	usr_system.SN[1] = (SYSCONFIG_YEAR) & 0xFF;
	usr_system.SN[2] = (SYSCONFIG_MON);
	usr_system.SN[3] = (SN_CODE >> 8) & 0xFF;
	usr_system.SN[4] = (SN_CODE) & 0xFF;
	usr_system.hardware[0] = BOOT_HARDWARE_MAJOR;
	usr_system.hardware[1] = BOOT_HARDWARE_MINOR;
	usr_system.hardware[2] = BOOT_HARDWARE_REVISION;
	usr_system.logLevel = BSP_LOG_DEFAULT_LEVEL;
	APP_CONFIG_SystemWrite(&usr_system);
	
	APP_CONFIG_SystemLoad();
	HARDWARE_HAL_DELAY_MS(20);
#endif

#if DEBUG_DOWNLOAD_CONTINUE_SET
	APP_CONFIG_FirmwareInfo_t usr_setting_firmware_info = {0};
	usr_setting_firmware_info.downloadLen = 0x400;
	usr_setting_firmware_info.reserved[0] = 0;
	usr_setting_firmware_info.reserved[1] = 0;
	usr_setting_firmware_info.reserved[2] = 0;
	usr_setting_firmware_info.reserved[3] = 0;
	usr_setting_firmware_info.firmwareInfo.deviceType = MASTER_CONTROL_DEV;
	usr_setting_firmware_info.firmwareInfo.versionH = APP_VERSION_H_DEFAULT;
	usr_setting_firmware_info.firmwareInfo.versionM = APP_VERSION_M_DEFAULT;
	usr_setting_firmware_info.firmwareInfo.versionL = APP_VERSION_L_DEFAULT;
	usr_setting_firmware_info.firmwareInfo.fileLen = 0x8000;
	usr_setting_firmware_info.firmwareInfo.fileCrc = 0x12345678;
	APP_CONFIG_FirmwareInfoWrite(DOWNLOAD_FIRMWARE_INFO, &usr_setting_firmware_info);
#elif DEBUG_DOWNLOAD_FINISH_SET
	APP_CONFIG_FirmwareInfo_t usr_setting_firmware_info = {0};
	usr_setting_firmware_info.downloadLen = FLASH_DOWNLOAD_SIZE;
	usr_setting_firmware_info.reserved[0] = 0;
	usr_setting_firmware_info.reserved[1] = 0;
	usr_setting_firmware_info.reserved[2] = 0;
	usr_setting_firmware_info.reserved[3] = 0;
	usr_setting_firmware_info.firmwareInfo.deviceType = MASTER_CONTROL_DEV;
	usr_setting_firmware_info.firmwareInfo.versionH = 2;
	usr_setting_firmware_info.firmwareInfo.versionM = 0;
	usr_setting_firmware_info.firmwareInfo.versionL = 3;
	usr_setting_firmware_info.firmwareInfo.fileLen = FLASH_DOWNLOAD_SIZE;
	usr_setting_firmware_info.firmwareInfo.fileCrc = crc32((uint8_t *)FLASH_DOWNLOAD_START_ADDR, FLASH_DOWNLOAD_SIZE);
	APP_CONFIG_FirmwareInfoWrite(DOWNLOAD_FIRMWARE_INFO, &usr_setting_firmware_info);

	usr_setting_firmware_info.firmwareInfo.versionL = 1;
	usr_setting_firmware_info.firmwareInfo.fileCrc = DEFAULT_CRC32_VALUE;
	APP_CONFIG_FirmwareInfoWrite(APP_FIRMWARE_INFO, &usr_setting_firmware_info);
#elif DEBUG_EEPROM_RECOVERY_SET
	APP_CONFIG_AppFirmwareInfoRecovery();
	APP_CONFIG_DownloadFirmwareInfoRecovery();
#else
#endif
    HARDWARE_HAL_DELAY_MS(20);
	APP_CONFIG_FirmwareInfoLoad();

}


void FirmwareSettingInfoTestRecv(void)
{
	APP_CONFIG_FirmwareInfo_pt pinfo1 = &g_appFirmwareInfo;
	APP_CONFIG_FirmwareInfo_pt pinfo2 = &g_downloadFirmwareInfo;

	if(g_downloadFirmwareInfo.downloadLen != g_downloadFirmwareInfo.firmwareInfo.fileLen)
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
						sizeof(APP_CONFIG_FirmwareInfo_t)))
		&& (0 != memcmp((uint8_t *)&factory_firmware_settting_info, 
		 				(uint8_t *)pinfo2, 
						 sizeof(APP_CONFIG_FirmwareInfo_t))))
		{
			APP_CONFIG_DownloadFirmwareInfoRecovery();
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

