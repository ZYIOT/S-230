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



app_config_firmware_setting_info_t app_firmware_info = {0};
app_config_firmware_setting_info_t download_firmware_info = {0};
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
	download_firmware_info.update_result = IAP_UPDATE_UNKNOW;
	download_firmware_info.reserved[0] = 0;
	download_firmware_info.reserved[1] = 0;
	download_firmware_info.firmware_type = UNKONW_FIRMWARE;
	_firmware_info_init(&message->firmware_info);
	
	return APP_OK;
}

int APP_CONFIG_firmware_setting_info_read(uint8_t infotype, app_config_firmware_setting_info_pt message)
{
	int rc = 0;
	uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};
	uint16_t page = 0;
		
	if(APP_FIRMWARE_INFO == infotype)
	{
		page = APP_CONFIG_EEPROM_APP_FIRMWARE_INFO_PAGE;
	}
	else
	{
		page = APP_CONFIG_EEPROM_DOWNLOAD_FIRMWARE_INFO_PAGE;
	}

	rc = epprom_read_block(page, bytes);
	APP_CHECK_RC(rc)
	message->download_len = read_uint32_t(&bytes[APP_CONFIG_EEPROM_FIRMWARE_INFO_DOWNLOAD_LEN_OFFSET]);
	message->update_result = read_uint8_t(&bytes[APP_CONFIG_EEPROM_FIRMWARE_INFO_UPDATE_RESULT_OFFSET]);
	message->reserved[0] = read_uint8_t(&bytes[APP_CONFIG_EEPROM_FIRMWARE_INFO_RESERVED_OFFSET]);
	message->reserved[1] = read_uint8_t(&bytes[APP_CONFIG_EEPROM_FIRMWARE_INFO_RESERVED_OFFSET+1]);
	message->firmware_type = read_uint8_t(&bytes[APP_CONFIG_EEPROM_FIRMWARE_INFO_FIRMWARE_TYPE_OFFSET]);
	_firmware_info_deserialize(bytes, &message->firmware_info);
	
	return APP_OK;
}

int APP_CONFIG_firmware_setting_info_write(uint8_t infotype, app_config_firmware_setting_info_pt message)
{
	uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};
	uint16_t page = 0;
	
	if(APP_FIRMWARE_INFO == infotype)
	{
		page = APP_CONFIG_EEPROM_APP_FIRMWARE_INFO_PAGE;
	}
	else
	{
		page = APP_CONFIG_EEPROM_DOWNLOAD_FIRMWARE_INFO_PAGE;
	}
	write_uint32_t(message->download_len, &bytes[APP_CONFIG_EEPROM_FIRMWARE_INFO_DOWNLOAD_LEN_OFFSET]);
	write_uint8_t(message->update_result,  &bytes[APP_CONFIG_EEPROM_FIRMWARE_INFO_UPDATE_RESULT_OFFSET]);
	write_uint8_t(message->reserved[0],  &bytes[APP_CONFIG_EEPROM_FIRMWARE_INFO_RESERVED_OFFSET]);
	write_uint8_t(message->reserved[1],  &bytes[APP_CONFIG_EEPROM_FIRMWARE_INFO_RESERVED_OFFSET+1]);
	write_uint8_t(message->firmware_type,  &bytes[APP_CONFIG_EEPROM_FIRMWARE_INFO_FIRMWARE_TYPE_OFFSET]);
	_firmware_info_serialize(bytes, &message->firmware_info);
	return epprom_write_block(page, bytes);
}

int APP_CONFIG_firmware_setting_info_overwrite(void)
{
	int rc = 0;
	memcpy((uint8_t *)&app_firmware_info, (uint8_t *)&download_firmware_info, sizeof(app_config_firmware_setting_info_t));
	app_firmware_info.update_result = IAP_UPDATE_SUCESS;
	rc = APP_CONFIG_firmware_setting_info_write(APP_FIRMWARE_INFO, &app_firmware_info);
	APP_CHECK_RC(rc)
	return APP_OK;
}

int APP_CONFIG_firmware_setting_info_load()
{
	int rc = 0;

	APP_CONFIG_firmware_setting_info_init(&app_firmware_info);
	APP_CONFIG_firmware_setting_info_init(&download_firmware_info);
	rc = APP_CONFIG_firmware_setting_info_read(APP_FIRMWARE_INFO, &app_firmware_info);
	APP_CHECK_RC(rc)
	rc = APP_CONFIG_firmware_setting_info_read(DOWNLOAD_FIRMWARE_INFO, &download_firmware_info);
	APP_CHECK_RC(rc)

	return APP_OK;
}

int APP_CONFIG_app_firmware_update_success(void)
{
	int rc = 0;
	app_firmware_info.update_result = IAP_UPDATE_SUCESS;
	app_firmware_info.firmware_type = MCU_APP_FIRMWARE;
	rc = APP_CONFIG_firmware_setting_info_write(APP_FIRMWARE_INFO, &app_firmware_info);
	APP_CHECK_RC(rc)
	return APP_OK;
}

int APP_CONFIG_app_firmware_update_fail(void)
{
	int rc = 0;
	app_firmware_info.update_result = IAP_UPDATE_FAIL;
	app_firmware_info.firmware_type = MCU_APP_FIRMWARE;
	rc = APP_CONFIG_firmware_setting_info_write(APP_FIRMWARE_INFO, &app_firmware_info);
	APP_CHECK_RC(rc)
	return APP_OK;
}

int APP_CONFIG_firmware_setting_info1_clear_result(void)
{
	int rc = 0;

	app_firmware_info.update_result = IAP_UPDATE_UNKNOW;
    app_firmware_info.firmware_type = UNKONW_FIRMWARE;
	rc = APP_CONFIG_firmware_setting_info_write(APP_FIRMWARE_INFO, &app_firmware_info);
	APP_CHECK_RC(rc)

	return APP_OK;
}


int APP_CONFIG_firmware_setting_info2_clear_result(void)
{
	int rc = 0;
	download_firmware_info.update_result = IAP_UPDATE_UNKNOW;
    download_firmware_info.firmware_type = UNKONW_FIRMWARE;
	rc = APP_CONFIG_firmware_setting_info_write(DOWNLOAD_FIRMWARE_INFO, &download_firmware_info);
	APP_CHECK_RC(rc)

	return APP_OK;
}

int APP_CONFIG_app_firmware_info_recovery(void)
{
	int rc = 0;
	app_config_firmware_setting_info_pt info = &app_firmware_info;
	info->download_len = 0;
	info->update_result = IAP_UPDATE_UNKNOW;
	info->reserved[0] = 0;
	info->reserved[1] = 0;
    info->firmware_type = UNKONW_FIRMWARE;
	info->firmware_info.device_type = 1;
	info->firmware_info.version_h = APP_VERSION_H_DEFAULT;
	info->firmware_info.version_m = APP_VERSION_M_DEFAULT;	
	info->firmware_info.version_l = APP_VERSION_L_DEFAULT;
	info->firmware_info.file_len = (0x20000);
	info->firmware_info.file_crc = DEFAULT_CRC32_VALUE;
	
	rc = APP_CONFIG_firmware_setting_info_write(APP_FIRMWARE_INFO, info);
	APP_CHECK_RC(rc)
	return APP_OK;
}

int APP_CONFIG_download_firmware_info_recovery(void)
{
	int rc = 0;
	app_config_firmware_setting_info_pt info = &download_firmware_info;
	info->download_len = 0;
	info->update_result = IAP_UPDATE_UNKNOW;
	info->reserved[0] = 0;
	info->reserved[1] = 0;
    info->firmware_type = UNKONW_FIRMWARE;
	info->firmware_info.device_type = 1;
	info->firmware_info.version_h = APP_VERSION_H_DEFAULT;
	info->firmware_info.version_m = APP_VERSION_M_DEFAULT;	
	info->firmware_info.version_l = APP_VERSION_L_DEFAULT;
	info->firmware_info.file_len = (0x20000);
	info->firmware_info.file_crc = DEFAULT_CRC32_VALUE;
	
	rc = APP_CONFIG_firmware_setting_info_write(DOWNLOAD_FIRMWARE_INFO, info);
	APP_CHECK_RC(rc)
	return APP_OK;
}


// int APP_CONFIG_firmware_setting_info_update_fail()
// {
// 	int rc = 0;
	
// 	download_firmware_info.download_len = 0;
// 	download_firmware_info.update_result = IAP_UPDATE_UNKNOW;
// 	download_firmware_info.reserved[0] = 0;
// 	download_firmware_info.reserved[1] = 0;
//     download_firmware_info.firmware_type = UNKONW_FIRMWARE;
// 	download_firmware_info.firmware_info.file_crc = DEFAULT_CRC32_VALUE;
// 	download_firmware_info.firmware_info.file_len = FLASH_DOWNLOAD_SIZE;
// 	download_firmware_info.firmware_info.device_type = MASTER_CONTROL_DEV;
// 	download_firmware_info.firmware_info.version_h = APP_VERSION_H_DEFAULT;
// 	download_firmware_info.firmware_info.version_m = APP_VERSION_M_DEFAULT;	
// 	download_firmware_info.firmware_info.version_l = APP_VERSION_L_DEFAULT;
// 	memcpy((uint8_t *)&app_firmware_info, (uint8_t *)&download_firmware_info, sizeof(app_config_firmware_setting_info_t));
// 	app_firmware_info.update_result = IAP_UPDATE_FAIL;
// 	rc = APP_CONFIG_firmware_setting_info_write(APP_FIRMWARE_INFO, &app_firmware_info);
// 	APP_CHECK_RC(rc)	
// 	rc = APP_CONFIG_firmware_setting_info_write(DOWNLOAD_FIRMWARE_INFO, &download_firmware_info);
// 	APP_CHECK_RC(rc)
// }


// int APP_CONFIG_firmware_setting_info_update_fail1()
// {
// 	int rc = 0;
// 	app_firmware_info.update_result = IAP_UPDATE_FAIL;
// 	rc = APP_CONFIG_firmware_setting_info_write(APP_FIRMWARE_INFO, &app_firmware_info);
// 	APP_CHECK_RC(rc)
// 	download_firmware_info.download_len = 0;
// 	download_firmware_info.update_result = IAP_UPDATE_UNKNOW;
// 	download_firmware_info.reserved[0] = 0;
// 	download_firmware_info.reserved[1] = 0;
//     download_firmware_info.firmware_type = UNKONW_FIRMWARE;
// 	download_firmware_info.firmware_info.file_crc = DEFAULT_CRC32_VALUE;
// 	download_firmware_info.firmware_info.file_len = FLASH_DOWNLOAD_SIZE;
// 	download_firmware_info.firmware_info.device_type = MASTER_CONTROL_DEV;
// 	download_firmware_info.firmware_info.version_h = APP_VERSION_H_DEFAULT;
// 	download_firmware_info.firmware_info.version_m = APP_VERSION_M_DEFAULT;	
// 	download_firmware_info.firmware_info.version_l = APP_VERSION_L_DEFAULT;
// 	rc = APP_CONFIG_firmware_setting_info_write(DOWNLOAD_FIRMWARE_INFO, &download_firmware_info);
// 	APP_CHECK_RC(rc)
// }

// info 区的数据是否有效 
int APP_CONFIG_firmware_check_info_valid(app_config_firmware_setting_info_pt info)
{
    if((0 == info->download_len) || (UNKONW_FIRMWARE == info->firmware_type) || (DEFAULT_CRC32_VALUE == info->firmware_info.file_crc))
    {
        return APP_ERROR;
    }
    return APP_OK;
}


#ifdef __cplusplus
}
#endif
#endif
