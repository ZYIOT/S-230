#ifndef __APP_CONFIG_EEPROM_MESSAGE_H
#define __APP_CONFIG_EEPROM_MESSAGE_H
#ifdef __cplusplus
extern "C"
{
#endif
#include "defines.h"
#include "common.h"
#include "board_config.h"
#include "bsp_board_debug.h"


int APP_CONFIG_SystemInit(APP_CONFIG_System_pt message)
{
    message->deviceID = 1;
    memset(message->SN, 0, 5);
    memset(message->hardware, 0, 3);
    message->hardware[0]= BOOT_HARDWARE_MAJOR;
    message->hardware[1]= BOOT_HARDWARE_MINOR;
    message->hardware[2]= BOOT_HARDWARE_REVISION;
    message->logLevel = BSP_LOG_DEFAULT_LEVEL; 
    return APP_OK;
}

static void SystemSerialize(uint8_t bytes[EEPROM_BLOCK_SIZE], APP_CONFIG_System_pt message)
{
	write_uint32_t(message->deviceID, &bytes[APP_CONFIG_EEPROM_SYSTEM_DEVICE_ID_OFFSET]); 
	memcpy(&bytes[APP_CONFIG_EEPROM_SYSTEM_SN_OFFSET], message->SN, 5); 
	memcpy(&bytes[APP_CONFIG_EEPROM_SYSTEM_HARDWARE_OFFSET], message->hardware, 3); 
}

APP_CONFIG_System_t g_appConfigSystem;
int APP_CONFIG_SystemRead(APP_CONFIG_System_pt message)
{
    uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};
    
    int rc = EEPROM_ReadBlock(APP_CONFIG_EEPROM_SYSTEM_PAGE, bytes);
    APP_CHECK_RC(rc)
    message->deviceID = read_uint32_t(&bytes[APP_CONFIG_EEPROM_SYSTEM_DEVICE_ID_OFFSET]);
    memcpy(message->SN, &bytes[APP_CONFIG_EEPROM_SYSTEM_SN_OFFSET], 5);
    memcpy(message->hardware, &bytes[APP_CONFIG_EEPROM_SYSTEM_HARDWARE_OFFSET], 3);
    return APP_OK;
}

int APP_CONFIG_SystemWrite(APP_CONFIG_System_pt message)
{
    uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};
    
    SystemSerialize(bytes, message);
    return EEPROM_WriteBlock(APP_CONFIG_EEPROM_SYSTEM_PAGE, bytes);
}

int APP_CONFIG_SystemWriteDeviceID(APP_CONFIG_System_pt message, uint32_t deviceID)
{
    uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};
    
    SystemSerialize(bytes, message);
    write_uint32_t(deviceID, &bytes[APP_CONFIG_EEPROM_SYSTEM_DEVICE_ID_OFFSET]);
    int rc = EEPROM_WriteBlock(APP_CONFIG_EEPROM_SYSTEM_PAGE, bytes);
    APP_CHECK_RC(rc)
    message->deviceID = deviceID; 
    return APP_OK;
}

int APP_CONFIG_SystemWriteSN(APP_CONFIG_System_pt message, uint8_t SN[5], uint8_t hardware[3])
{
    uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};
    
    SystemSerialize(bytes, message);
    memcpy(&bytes[APP_CONFIG_EEPROM_SYSTEM_SN_OFFSET], SN, 5);
    memcpy(&bytes[APP_CONFIG_EEPROM_SYSTEM_HARDWARE_OFFSET], hardware, 3);
    int rc = EEPROM_WriteBlock(APP_CONFIG_EEPROM_SYSTEM_PAGE, bytes);
    APP_CHECK_RC(rc)
    memcpy(message->SN, SN, 5); 
    memcpy(message->hardware, hardware, 3); 
    return APP_OK;
}

int APP_CONFIG_SystemLoad(void)
{
    int rc = 0;
    APP_CONFIG_SystemInit(&g_appConfigSystem);
    rc = APP_CONFIG_SystemRead(&g_appConfigSystem);
    APP_CHECK_RC(rc)
    return APP_OK;
}

int APP_CONFIG_SystemRecovery(void)
{
    int rc = 0;
    APP_CONFIG_SystemInit(&g_appConfigSystem);
    rc = APP_CONFIG_SystemWrite( &g_appConfigSystem);
    APP_CHECK_RC(rc)
    return APP_OK;
}



APP_CONFIG_FirmwareInfo_t g_appFirmwareInfo = {0};
APP_CONFIG_FirmwareInfo_t g_downloadFirmwareInfo = {0};
static int FirmwareInfoInit(firmwareInfo_pt message)
{
	message->deviceType = 0;
	message->versionH = 0;
	message->versionM = 0;
	message->versionL = 0;
	message->fileLen = 0;
	message->fileCrc = 0;
	
	return APP_OK;
}

static int FirmwareInfoDeserialize(uint8_t *bytes, firmwareInfo_pt message)
{
	message->deviceType = read_uint8_t(&bytes[APP_CONFIG_EEPROM_FIRMWARE_INFO_DEVICE_TYPE_OFFSET]); 
	message->versionH = read_uint8_t(&bytes[APP_CONFIG_EEPROM_FIRMWARE_INFO_VERSION_H_OFFSET]); 
	message->versionM = read_uint8_t(&bytes[APP_CONFIG_EEPROM_FIRMWARE_INFO_VERSION_M_OFFSET]); 
	message->versionL = read_uint8_t(&bytes[APP_CONFIG_EEPROM_FIRMWARE_INFO_VERSION_L_OFFSET]);
	message->fileLen = read_uint32_t(&bytes[APP_CONFIG_EEPROM_FIRMWARE_INFO_FILE_LEN_OFFSET]); 
	message->fileCrc = read_uint32_t(&bytes[APP_CONFIG_EEPROM_FIRMWARE_INFO_FILE_CRC_OFFSET]);
	
	return APP_OK;
}


static int FirmwareInfoSerialize(uint8_t *bytes, firmwareInfo_pt message)
{
	write_uint8_t(message->deviceType, &bytes[APP_CONFIG_EEPROM_FIRMWARE_INFO_DEVICE_TYPE_OFFSET]); 
	write_uint8_t(message->versionH, &bytes[APP_CONFIG_EEPROM_FIRMWARE_INFO_VERSION_H_OFFSET]); 
	write_uint8_t(message->versionM, &bytes[APP_CONFIG_EEPROM_FIRMWARE_INFO_VERSION_M_OFFSET]); 
	write_uint8_t(message->versionL, &bytes[APP_CONFIG_EEPROM_FIRMWARE_INFO_VERSION_L_OFFSET]);
	write_uint32_t(message->fileLen, &bytes[APP_CONFIG_EEPROM_FIRMWARE_INFO_FILE_LEN_OFFSET]); 
	write_uint32_t(message->fileCrc, &bytes[APP_CONFIG_EEPROM_FIRMWARE_INFO_FILE_CRC_OFFSET]); 

	return APP_OK;
}

int APP_CONFIG_FirmwareInfoInit(APP_CONFIG_FirmwareInfo_pt message)
{
	message->downloadLen = 0;
    message->updateResult = IAP_UPDATE_UNKNOW;
    message->reserved[0] = 0;
    message->reserved[1] = 0;
    message->firmwareType = UNKONW_FIRMWARE;
	FirmwareInfoInit(&message->firmwareInfo);
	
	return APP_OK;
}

int APP_CONFIG_FirmwareInfoRead(uint8_t infoType, APP_CONFIG_FirmwareInfo_pt message)
{
	int rc = 0;
	uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};
	uint16_t page = 0;
		
	if(APP_FIRMWARE_INFO == infoType)
	{
		page = APP_CONFIG_EEPROM_APP_FIRMWARE_INFO_PAGE;
	}
	else
	{
		page = APP_CONFIG_EEPROM_DOWNLOAD_FIRMWARE_INFO_PAGE;
	}

	rc = EEPROM_ReadBlock(page, bytes);
	APP_CHECK_RC(rc)
	message->downloadLen = read_uint32_t(&bytes[APP_CONFIG_EEPROM_FIRMWARE_INFO_DOWNLOAD_LEN_OFFSET]);
    message->updateResult = read_uint8_t(&bytes[APP_CONFIG_EEPROM_FIRMWARE_INFO_UPDATE_RESULT_OFFSET]);
	message->reserved[0] = read_uint8_t(&bytes[APP_CONFIG_EEPROM_FIRMWARE_INFO_RESERVED_OFFSET]);
	message->reserved[1] = read_uint8_t(&bytes[APP_CONFIG_EEPROM_FIRMWARE_INFO_RESERVED_OFFSET+1]);
	message->firmwareType = read_uint8_t(&bytes[APP_CONFIG_EEPROM_FIRMWARE_INFO_FIRMWARE_TYPE_OFFSET]);
	FirmwareInfoDeserialize(bytes, &message->firmwareInfo);
	
	return APP_OK;
}

int APP_CONFIG_FirmwareInfoWrite(uint8_t infoType, APP_CONFIG_FirmwareInfo_pt message)
{
	uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};
	uint16_t page = 0;
	
	if(APP_FIRMWARE_INFO == infoType)
	{
		page = APP_CONFIG_EEPROM_APP_FIRMWARE_INFO_PAGE;
	}
	else
	{
		page = APP_CONFIG_EEPROM_DOWNLOAD_FIRMWARE_INFO_PAGE;
	}
	write_uint32_t(message->downloadLen, &bytes[APP_CONFIG_EEPROM_FIRMWARE_INFO_DOWNLOAD_LEN_OFFSET]);
    write_uint8_t(message->updateResult,  &bytes[APP_CONFIG_EEPROM_FIRMWARE_INFO_UPDATE_RESULT_OFFSET]);
    write_uint8_t(message->reserved[0],  &bytes[APP_CONFIG_EEPROM_FIRMWARE_INFO_RESERVED_OFFSET]);
	write_uint8_t(message->reserved[1],  &bytes[APP_CONFIG_EEPROM_FIRMWARE_INFO_RESERVED_OFFSET+1]);
	write_uint8_t(message->firmwareType,  &bytes[APP_CONFIG_EEPROM_FIRMWARE_INFO_FIRMWARE_TYPE_OFFSET]);
	FirmwareInfoSerialize(bytes, &message->firmwareInfo);
	return EEPROM_WriteBlock(page, bytes);
}

int APP_CONFIG_FirmwareInfoOverwrite(void)
{
	int rc = 0;
	rc = APP_CONFIG_FirmwareInfoWrite(DOWNLOAD_FIRMWARE_INFO, &g_downloadFirmwareInfo);
	APP_CHECK_RC(rc)
	memcpy((uint8_t *)&g_appFirmwareInfo, (uint8_t *)&g_downloadFirmwareInfo, sizeof(APP_CONFIG_FirmwareInfo_t));
	rc = APP_CONFIG_FirmwareInfoWrite(APP_FIRMWARE_INFO, &g_appFirmwareInfo);
	APP_CHECK_RC(rc)
	return APP_OK;
}

int APP_CONFIG_FirmwareInfoLoad()
{
	int rc = 0;

	APP_CONFIG_FirmwareInfoInit(&g_appFirmwareInfo);
	APP_CONFIG_FirmwareInfoInit(&g_downloadFirmwareInfo);
	rc = APP_CONFIG_FirmwareInfoRead(APP_FIRMWARE_INFO, &g_appFirmwareInfo);
	APP_CHECK_RC(rc)
	rc = APP_CONFIG_FirmwareInfoRead(DOWNLOAD_FIRMWARE_INFO, &g_downloadFirmwareInfo);
	APP_CHECK_RC(rc)

	return APP_OK;
}


int APP_CONFIG_AppFirmwareInfoRecovery(void)
{
	int rc = 0;
	APP_CONFIG_FirmwareInfo_pt info = &g_appFirmwareInfo;
	info->downloadLen = 0;
    info->updateResult = IAP_UPDATE_UNKNOW;
	info->reserved[0] = 0;
	info->reserved[1] = 0;
    info->firmwareType = UNKONW_FIRMWARE;
	info->firmwareInfo.deviceType = UNKONW_DEV;
	info->firmwareInfo.versionH = APP_VERSION_H_DEFAULT;
	info->firmwareInfo.versionM = APP_VERSION_M_DEFAULT;	
	info->firmwareInfo.versionL = APP_VERSION_L_DEFAULT;
	info->firmwareInfo.fileLen = FLASH_DOWNLOAD_SIZE;
	info->firmwareInfo.fileCrc = DEFAULT_CRC32_VALUE;
	
	rc = APP_CONFIG_FirmwareInfoWrite(APP_FIRMWARE_INFO, info);
	APP_CHECK_RC(rc)
	return APP_OK;
}

int APP_CONFIG_DownloadFirmwareInfoRecovery(void)
{
	int rc = 0;
	APP_CONFIG_FirmwareInfo_pt info = &g_downloadFirmwareInfo;
	info->downloadLen = 0;
    info->updateResult = IAP_UPDATE_UNKNOW;
	info->reserved[0] = 0;
	info->reserved[1] = 0;
    info->firmwareType = UNKONW_FIRMWARE;
	info->firmwareInfo.deviceType = UNKONW_DEV;
	info->firmwareInfo.versionH = APP_VERSION_H_DEFAULT;
	info->firmwareInfo.versionM = APP_VERSION_M_DEFAULT;	
	info->firmwareInfo.versionL = APP_VERSION_L_DEFAULT;
	info->firmwareInfo.fileLen = FLASH_DOWNLOAD_SIZE;
	info->firmwareInfo.fileCrc = DEFAULT_CRC32_VALUE;
	
	rc = APP_CONFIG_FirmwareInfoWrite(DOWNLOAD_FIRMWARE_INFO, info);
	APP_CHECK_RC(rc)
	return APP_OK;
}

int APP_CONFIG_AllFirmwareInfoRecovery()
{
	int rc = 0;
	APP_CONFIG_FirmwareInfo_pt info = &g_appFirmwareInfo;
	info->downloadLen = 0;
    info->updateResult = IAP_UPDATE_UNKNOW;
	info->reserved[0] = 0;
	info->reserved[1] = 0;
    info->firmwareType = UNKONW_FIRMWARE;
	info->firmwareInfo.deviceType = UNKONW_DEV;
	info->firmwareInfo.versionH = APP_VERSION_H_DEFAULT;
	info->firmwareInfo.versionM = APP_VERSION_M_DEFAULT;	
	info->firmwareInfo.versionL = APP_VERSION_L_DEFAULT;
	info->firmwareInfo.fileLen = FLASH_DOWNLOAD_SIZE;
	info->firmwareInfo.fileCrc = DEFAULT_CRC32_VALUE;
	rc = APP_CONFIG_FirmwareInfoWrite(APP_FIRMWARE_INFO, &g_appFirmwareInfo);
	APP_CHECK_RC(rc)
	memcpy((uint8_t *)&g_downloadFirmwareInfo, (uint8_t *)&g_appFirmwareInfo, sizeof(APP_CONFIG_FirmwareInfo_t));
	rc = APP_CONFIG_FirmwareInfoWrite(DOWNLOAD_FIRMWARE_INFO, &g_downloadFirmwareInfo);
	APP_CHECK_RC(rc)
}

int APP_CONFIG_AppFirmwareInfoSync(UPDATE_RESULT_e iapResult)
{
	int rc = 0;
	APP_CONFIG_FirmwareInfo_pt appInfo = &g_appFirmwareInfo;
	APP_CONFIG_FirmwareInfo_pt dwnInfo = &g_downloadFirmwareInfo;
	if(IAP_UPDATE_SUCESS == iapResult)
	{
		memcpy((uint8_t *)appInfo, (uint8_t *)dwnInfo, sizeof(APP_CONFIG_FirmwareInfo_t));
		appInfo->updateResult = IAP_UPDATE_SUCESS;
	}
	else if(IAP_UPDATE_FAIL == iapResult)
	{
		appInfo->updateResult = IAP_UPDATE_FAIL;
	}
	else
	{
		APP_CONFIG_AppFirmwareInfoRecovery();
		appInfo->updateResult = IAP_UPDATE_FAIL;
	}
	rc = APP_CONFIG_FirmwareInfoWrite(APP_FIRMWARE_INFO, &g_appFirmwareInfo);
	APP_CHECK_RC(rc)
}


#ifdef __cplusplus
}
#endif
#endif
