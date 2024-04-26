#ifndef __APP_CONFIG_EEPROM_MESSAGE_H
#define __APP_CONFIG_EEPROM_MESSAGE_H
#ifdef __cplusplus
extern "C"
{
#endif
#include "defines.h"
#include "common.h"


int APP_CONFIG_system_init(app_config_system_pt message)
{
    message->device_id = 1;
    memset(message->SN, 0, 5);
    memset(message->hardware, 0, 3);
    message->hardware[0]= BOOT_HARDWARE_MAJOR;
    message->hardware[1]= BOOT_HARDWARE_MINOR;
    message->hardware[2]= BOOT_HARDWARE_REVISION;
    message->log_level = BSP_LOG_DEFAULT_LEVEL; 
    return APP_OK;
}

static void _system_serialize(uint8_t bytes[EEPROM_BLOCK_SIZE], app_config_system_pt message)
{
	write_uint32_t(message->device_id, &bytes[APP_CONFIG_EEPROM_SYSTEM_DEVICE_ID_OFFSET]); 
	memcpy(&bytes[APP_CONFIG_EEPROM_SYSTEM_SN_OFFSET], message->SN, 5); 
	memcpy(&bytes[APP_CONFIG_EEPROM_SYSTEM_HARDWARE_OFFSET], message->hardware, 3); 
}

app_config_system_t app_config_system;
int APP_CONFIG_system_read( app_config_system_pt message)
{
    uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};
    
    int rc = epprom_read_block(APP_CONFIG_EEPROM_SYSTEM_PAGE, bytes);
    APP_CHECK_RC(rc)
    message->device_id = read_uint32_t(&bytes[APP_CONFIG_EEPROM_SYSTEM_DEVICE_ID_OFFSET]);
    memcpy(message->SN, &bytes[APP_CONFIG_EEPROM_SYSTEM_SN_OFFSET], 5);
    memcpy(message->hardware, &bytes[APP_CONFIG_EEPROM_SYSTEM_HARDWARE_OFFSET], 3);
    return APP_OK;
}

int APP_CONFIG_system_write( app_config_system_pt message)
{
    uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};
    
    _system_serialize(bytes, message);
    return epprom_write_block(APP_CONFIG_EEPROM_SYSTEM_PAGE, bytes);
}

int APP_CONFIG_system_write_device_id( app_config_system_pt message, uint32_t device_id)
{
    uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};
    
    _system_serialize(bytes, message);
    write_uint32_t(device_id, &bytes[APP_CONFIG_EEPROM_SYSTEM_DEVICE_ID_OFFSET]);
    int rc = epprom_write_block(APP_CONFIG_EEPROM_SYSTEM_PAGE, bytes);
    APP_CHECK_RC(rc)
    message->device_id = device_id; 
    return APP_OK;
}

int APP_CONFIG_system_write_sn( app_config_system_pt message, uint8_t SN[5], uint8_t hardware[3])
{
    uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};
    
    _system_serialize(bytes, message);
    memcpy(&bytes[APP_CONFIG_EEPROM_SYSTEM_SN_OFFSET], SN, 5);
    memcpy(&bytes[APP_CONFIG_EEPROM_SYSTEM_HARDWARE_OFFSET], hardware, 3);
    int rc = epprom_write_block(APP_CONFIG_EEPROM_SYSTEM_PAGE, bytes);
    APP_CHECK_RC(rc)
    memcpy(message->SN, SN, 5); 
    memcpy(message->hardware, hardware, 3); 
    return APP_OK;
}

int APP_CONFIG_system_load(void)
{
    int rc = 0;
    APP_CONFIG_system_init(&app_config_system);
    rc = APP_CONFIG_system_read(&app_config_system);
    APP_CHECK_RC(rc)
    return APP_OK;
}

int APP_CONFIG_system_recovery(void)
{
    int rc = 0;
    APP_CONFIG_system_init(&app_config_system);
    rc = APP_CONFIG_system_write( &app_config_system);
    APP_CHECK_RC(rc)
    return APP_OK;
}



app_config_firmware_setting_info_t firmware_setting_info1 = {0};
app_config_firmware_setting_info_t firmware_setting_info2 = {0};
static int _firmware_info_init(app_config_firmware_info_pt message)
{
	message->device_type = 0;
	message->version_h = 0;
	message->version_m = 0;
	message->version_l = 0;
	message->file_len = 0;
	message->file_crc = 0;
	
	return APP_OK;
}

static int _firmware_info_deserialize(uint8_t *bytes, app_config_firmware_info_pt message)
{
	message->device_type = read_uint8_t(&bytes[APP_CONFIG_EEPROM_FIRMWARE_INFO_DEVICE_TYPE_OFFSET]); 
	message->version_h = read_uint8_t(&bytes[APP_CONFIG_EEPROM_FIRMWARE_INFO_VERSION_H_OFFSET]); 
	message->version_m = read_uint8_t(&bytes[APP_CONFIG_EEPROM_FIRMWARE_INFO_VERSION_M_OFFSET]); 
	message->version_l = read_uint8_t(&bytes[APP_CONFIG_EEPROM_FIRMWARE_INFO_VERSION_L_OFFSET]);
	message->file_len = read_uint32_t(&bytes[APP_CONFIG_EEPROM_FIRMWARE_INFO_FILE_LEN_OFFSET]); 
	message->file_crc = read_uint32_t(&bytes[APP_CONFIG_EEPROM_FIRMWARE_INFO_FILE_CRC_OFFSET]);
	
	return APP_OK;
}


static int _firmware_info_serialize(uint8_t *bytes, app_config_firmware_info_pt message)
{
	write_uint8_t(message->device_type, &bytes[APP_CONFIG_EEPROM_FIRMWARE_INFO_DEVICE_TYPE_OFFSET]); 
	write_uint8_t(message->version_h, &bytes[APP_CONFIG_EEPROM_FIRMWARE_INFO_VERSION_H_OFFSET]); 
	write_uint8_t(message->version_m, &bytes[APP_CONFIG_EEPROM_FIRMWARE_INFO_VERSION_M_OFFSET]); 
	write_uint8_t(message->version_l, &bytes[APP_CONFIG_EEPROM_FIRMWARE_INFO_VERSION_L_OFFSET]);
	write_uint32_t(message->file_len, &bytes[APP_CONFIG_EEPROM_FIRMWARE_INFO_FILE_LEN_OFFSET]); 
	write_uint32_t(message->file_crc, &bytes[APP_CONFIG_EEPROM_FIRMWARE_INFO_FILE_CRC_OFFSET]); 

	return APP_OK;
}

int APP_CONFIG_firmware_setting_info_init(app_config_firmware_setting_info_pt message)
{
	message->download_len = 0;
	firmware_setting_info2.reserved[0] = 0;
	firmware_setting_info2.reserved[1] = 0;
	firmware_setting_info2.reserved[2] = 0;
	firmware_setting_info2.reserved[3] = 0;
	_firmware_info_init(&message->firmware_info);
	
	return APP_OK;
}

int APP_CONFIG_firmware_setting_info_read(uint8_t infotype, app_config_firmware_setting_info_pt message)
{
	int rc = 0;
	uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};
	uint16_t page = 0;
		
	if(APP_CONFIG_FIRMWARE_SETTING_INFO == infotype)
	{
		page = APP_CONFIG_EEPROM_FIRMWARE_SETTING_INFO_PAGE;
	}
	else
	{
		page = APP_CONFIG_EEPROM_FIRMWARE_SETTING_INFO_BACK_PAGE;
	}

	rc = epprom_read_block(page, bytes);
	APP_CHECK_RC(rc)
	message->download_len = read_uint32_t(&bytes[APP_CONFIG_EEPROM_FIRMWARE_INFO_DOWNLOAD_LEN_OFFSET]);
	message->reserved[0] = read_uint8_t(&bytes[APP_CONFIG_EEPROM_FIRMWARE_INFO_RESERVED_OFFSET]);
	message->reserved[1] = read_uint8_t(&bytes[APP_CONFIG_EEPROM_FIRMWARE_INFO_RESERVED_OFFSET+1]);
	message->reserved[2] = read_uint8_t(&bytes[APP_CONFIG_EEPROM_FIRMWARE_INFO_RESERVED_OFFSET+2]);
	message->reserved[3] = read_uint8_t(&bytes[APP_CONFIG_EEPROM_FIRMWARE_INFO_RESERVED_OFFSET+3]);
	_firmware_info_deserialize(bytes, &message->firmware_info);
	
	return APP_OK;
}

int APP_CONFIG_firmware_setting_info_write(uint8_t infotype, app_config_firmware_setting_info_pt message)
{
	uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};
	uint16_t page = 0;
	
	if(APP_CONFIG_FIRMWARE_SETTING_INFO == infotype)
	{
		page = APP_CONFIG_EEPROM_FIRMWARE_SETTING_INFO_PAGE;
	}
	else
	{
		page = APP_CONFIG_EEPROM_FIRMWARE_SETTING_INFO_BACK_PAGE;
	}
	write_uint32_t(message->download_len, &bytes[APP_CONFIG_EEPROM_FIRMWARE_INFO_DOWNLOAD_LEN_OFFSET]);
	write_uint8_t(message->reserved[0],  &bytes[APP_CONFIG_EEPROM_FIRMWARE_INFO_RESERVED_OFFSET]);
	write_uint8_t(message->reserved[1],  &bytes[APP_CONFIG_EEPROM_FIRMWARE_INFO_RESERVED_OFFSET+1]);
	write_uint8_t(message->reserved[2],  &bytes[APP_CONFIG_EEPROM_FIRMWARE_INFO_RESERVED_OFFSET+2]);
	write_uint8_t(message->reserved[3],  &bytes[APP_CONFIG_EEPROM_FIRMWARE_INFO_RESERVED_OFFSET+3]);
	_firmware_info_serialize(bytes, &message->firmware_info);
	return epprom_write_block(page, bytes);
}

int APP_CONFIG_firmware_setting_info_overwrite()
{
	int rc = 0;
	rc = APP_CONFIG_firmware_setting_info_write(APP_CONFIG_FIRMWARE_SETTING_INFO_BACK, &firmware_setting_info2);
	APP_CHECK_RC(rc)
	memcpy((uint8_t *)&firmware_setting_info1, (uint8_t *)&firmware_setting_info2, sizeof(app_config_firmware_setting_info_t));
	rc = APP_CONFIG_firmware_setting_info_write(APP_CONFIG_FIRMWARE_SETTING_INFO, &firmware_setting_info1);
	APP_CHECK_RC(rc)
}

int APP_CONFIG_firmware_setting_info_load()
{
	int rc = 0;

	APP_CONFIG_firmware_setting_info_init(&firmware_setting_info1);
	APP_CONFIG_firmware_setting_info_init(&firmware_setting_info2);
	rc = APP_CONFIG_firmware_setting_info_read(APP_CONFIG_FIRMWARE_SETTING_INFO, &firmware_setting_info1);
	APP_CHECK_RC(rc)
	rc = APP_CONFIG_firmware_setting_info_read(APP_CONFIG_FIRMWARE_SETTING_INFO_BACK, &firmware_setting_info2);
	APP_CHECK_RC(rc)

	return APP_OK;
}

int APP_CONFIG_firmware_setting_info2_recovery()
{
	int rc = 0;
	
	firmware_setting_info2.download_len = 0;
	firmware_setting_info2.reserved[0] = 0;
	firmware_setting_info2.reserved[1] = 0;
	firmware_setting_info2.reserved[2] = 0;
	firmware_setting_info2.reserved[3] = 0;
	firmware_setting_info2.firmware_info.device_type = 1;
	firmware_setting_info2.firmware_info.version_h = APP_VERSION_H_DEFAULT;
	firmware_setting_info2.firmware_info.version_m = APP_VERSION_M_DEFAULT;	
	firmware_setting_info2.firmware_info.version_l = APP_VERSION_L_DEFAULT;
	firmware_setting_info2.firmware_info.file_len = (0x20000);
	firmware_setting_info2.firmware_info.file_crc = DEFAULT_CRC32_VALUE;
	
	rc = APP_CONFIG_firmware_setting_info_write(APP_CONFIG_FIRMWARE_SETTING_INFO_BACK, &firmware_setting_info2);
	APP_CHECK_RC(rc)
}

int APP_CONFIG_firmware_setting_info_all_recovery()
{
	int rc = 0;
	
	firmware_setting_info2.download_len = 0;
	firmware_setting_info2.reserved[0] = 0;
	firmware_setting_info2.reserved[1] = 0;
	firmware_setting_info2.reserved[2] = 0;
	firmware_setting_info2.reserved[3] = 0;
	firmware_setting_info2.firmware_info.file_crc = DEFAULT_CRC32_VALUE;
	firmware_setting_info2.firmware_info.file_len = FLASH_DOWNLOAD_SIZE;
	firmware_setting_info2.firmware_info.device_type = MASTER_CONTROL_DEV;
	firmware_setting_info2.firmware_info.version_h = APP_VERSION_H_DEFAULT;
	firmware_setting_info2.firmware_info.version_m = APP_VERSION_M_DEFAULT;	
	firmware_setting_info2.firmware_info.version_l = APP_VERSION_L_DEFAULT;
	memcpy((uint8_t *)&firmware_setting_info1, (uint8_t *)&firmware_setting_info2, sizeof(app_config_firmware_setting_info_t));
	rc = APP_CONFIG_firmware_setting_info_write(APP_CONFIG_FIRMWARE_SETTING_INFO, &firmware_setting_info1);
	APP_CHECK_RC(rc)
	rc = APP_CONFIG_firmware_setting_info_write(APP_CONFIG_FIRMWARE_SETTING_INFO_BACK, &firmware_setting_info2);
	APP_CHECK_RC(rc)
}

int APP_CONFIG_firmware_setting_info_update_fail()
{
	int rc = 0;
	
	firmware_setting_info2.download_len = 0;
	firmware_setting_info2.reserved[0] = 0;		// 1:update fail
	firmware_setting_info2.reserved[1] = 0;
	firmware_setting_info2.reserved[2] = 0;
	firmware_setting_info2.reserved[3] = 0;
	firmware_setting_info2.firmware_info.file_crc = DEFAULT_CRC32_VALUE;
	firmware_setting_info2.firmware_info.file_len = FLASH_DOWNLOAD_SIZE;
	firmware_setting_info2.firmware_info.device_type = MASTER_CONTROL_DEV;
	firmware_setting_info2.firmware_info.version_h = APP_VERSION_H_DEFAULT;
	firmware_setting_info2.firmware_info.version_m = APP_VERSION_M_DEFAULT;	
	firmware_setting_info2.firmware_info.version_l = APP_VERSION_L_DEFAULT;
	memcpy((uint8_t *)&firmware_setting_info1, (uint8_t *)&firmware_setting_info2, sizeof(app_config_firmware_setting_info_t));
	firmware_setting_info1.reserved[0] = 1;		// 1:update fail
	rc = APP_CONFIG_firmware_setting_info_write(APP_CONFIG_FIRMWARE_SETTING_INFO, &firmware_setting_info1);
	APP_CHECK_RC(rc)	
	rc = APP_CONFIG_firmware_setting_info_write(APP_CONFIG_FIRMWARE_SETTING_INFO_BACK, &firmware_setting_info2);
	APP_CHECK_RC(rc)
}


int APP_CONFIG_firmware_setting_info_update_fail1()
{
	int rc = 0;
	firmware_setting_info1.reserved[0] = 1;		// 1:update fail
	rc = APP_CONFIG_firmware_setting_info_write(APP_CONFIG_FIRMWARE_SETTING_INFO, &firmware_setting_info1);
	APP_CHECK_RC(rc)
	firmware_setting_info2.download_len = 0;
	firmware_setting_info2.reserved[0] = 0;		// 1:update fail
	firmware_setting_info2.reserved[1] = 0;
	firmware_setting_info2.reserved[2] = 0;
	firmware_setting_info2.reserved[3] = 0;
	firmware_setting_info2.firmware_info.file_crc = DEFAULT_CRC32_VALUE;
	firmware_setting_info2.firmware_info.file_len = FLASH_DOWNLOAD_SIZE;
	firmware_setting_info2.firmware_info.device_type = MASTER_CONTROL_DEV;
	firmware_setting_info2.firmware_info.version_h = APP_VERSION_H_DEFAULT;
	firmware_setting_info2.firmware_info.version_m = APP_VERSION_M_DEFAULT;	
	firmware_setting_info2.firmware_info.version_l = APP_VERSION_L_DEFAULT;
	rc = APP_CONFIG_firmware_setting_info_write(APP_CONFIG_FIRMWARE_SETTING_INFO_BACK, &firmware_setting_info2);
	APP_CHECK_RC(rc)
}



#ifdef __cplusplus
}
#endif
#endif
