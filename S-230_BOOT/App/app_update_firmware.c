

#include "app_update_firmware.h"
#include "stm32_flash.h"
#include "board_config.h"
#include "bsp_mcu.h"
#include "bsp_wdg.h"
#include "app_log.h"
#include "app_config/message.h"
#include "utils.h"
#if DEBUG_FIRMWARE_SETTING_INFO_TEST
#include "firmware_setting_info_test.h"
#endif

uint8_t app_info_check_str[] = "waiting for check app info ... ...\r\n";
uint8_t app_info_success_str[] = "app info check success !!!\r\nnow run application\r\n";
uint8_t erase_firmware_info_str[] = "erase firmware info page... ...\r\nnow run application\r\n";
uint8_t app_file_name_head_str[] = "_APP";

const error_type_string_t update_error_content[] = 
{
	// 与定义的 UPDATE_ERROR_TYPE_E 中定义的类型和位置一一对应
	{UPDATE_NO_ERROR, "no error\r\n"},
	{UPDATE_ERROR_DEVBUSY, "device is busy\r\n"},
	{UPDATE_ERROR_FILETYPE, "file type is error\r\n"},
	{UPDATE_ERROR_FILENAME, "file name is error\r\n"},
	{UPDATE_ERROR_FILELEN, "file len is error\r\n"},
	{UPDATE_ERROR_FILECRC, "file crc is error\r\n"},
	{UPDATE_ERROR_STARTADDR, "start address is error\r\n"},
	{UPDATE_ERROR_OFFSET, "offset address is error\r\n"},
	{UPDATE_ERROR_FRAMENO, "frame no is error\r\n"},
	{UPDATE_ERROR_FRAMELEN, "frame len is error\r\n"},
	{UPDATE_ERROR_FRAMECRC, "frame crc is error\r\n"},
	{UPDATE_ERROR_FLASHERASE, "erase flash is error\r\n"},
	{UPDATE_ERROR_FLASHWRITE, "write flash is error\r\n"},
	{UPDATE_ERROR_FRAMECMD, "frame cmd is error\r\n"},
	{UPDATE_ERROR_RETRYFAIL, "retry send max but fail\r\n"},
	{UPDATE_ERROR_ERASEPARA, "erase paraments is error\r\n"},
	{UPDATE_ERROR_WRITEPARA, "write paraments is error\r\n"},
	{UPDATE_ERROR_FAKEFRAME, "frame type is fake\r\n"},
	{UPDATE_ERROR_RESENDOK, "frame type is resend\r\n"},
	{UPDATE_ERROR_MALLOC, "malloc is error\r\n"},
	{UPDATE_ERROR_REBOOTCNT, "reboot max but fail\r\n"},
	{UPDATE_ERROR_FILE_EXIST, "firmware file exist\r\n"},
	{UPDATE_ERROR_COPYFACTORY, "copy APP to Factory is error\r\n"},
	{UPDATE_ERROR_DOWNLOAD_DATA, "download data check error\r\n"},
	{UPDATE_ERROR_DOWNLOAD_UNFINISH, "download data not finish\r\n"},
	{UPDATE_ERROR_NO_FIRMWARE, "no firmware exist\r\n"},
	
	{UPDATE_ERROR_RESEND, "resend frame\r\n"},
	{UPDATE_ERROR_END1, "update flow is end\r\n"},
};



// 本地 boot 程序文件默认信息
app_config_firmware_info_t local_boot_info = 
{
	.file_len = FLASH_BOOTLOADER_SIZE,
	.file_crc = DEFAULT_CRC32_VALUE,
	.device_type = UNKONW_DEV,
};

// 本地 app 程序文件默认信息
app_config_firmware_info_t local_app_info = 
{
	.file_len = FLASH_APP_SIZE,
	.file_crc = DEFAULT_CRC32_VALUE,
	.device_type = UNKONW_DEV,
};

// 本地 download 区程序文件默认信息
app_config_firmware_info_t local_download_info = 
{
	.file_len = FLASH_DOWNLOAD_SIZE,
	.file_crc = DEFAULT_CRC32_VALUE,
	.device_type = UNKONW_DEV,
};

void APP_UPDATE_FIRMWARE_showErrorTypeContent(UPDATE_ERROR_TYPE_E type)
{
	uint8_t index = 0;
	uint8_t count = sizeof(update_error_content)/sizeof(error_type_string_t);

	if(UPDATE_NO_ERROR != type)
	{
		for(index=1; index<count; index++)
		{
			if(type == update_error_content[index].type)
			{
				break;
			}
		}
		APP_LOG_trace("[ERROR:%d]:%s", index, update_error_content[index].string);
	}
}

__weak void firmware_setting_info_test_send(void)
{
}
__weak void firmware_setting_info_test_recv(void)
{
}



static UPDATE_ERROR_TYPE_E _flash_write_dire_flash(uint32_t read_addr, uint32_t write_addr, uint32_t file_len)
{
	uint32_t *pread = (uint32_t *)read_addr;
	UPDATE_ERROR_TYPE_E ret = UPDATE_NO_ERROR;

	if(0 != stm32_flashWrite_Dire(write_addr, pread, file_len))
	{
		ret = UPDATE_ERROR_FLASHWRITE;
	}

	return ret;
}

static UPDATE_ERROR_TYPE_E _flash_write_flash(uint32_t read_addr, uint32_t write_addr, uint32_t file_len)
{
	uint32_t *pread = (uint32_t *)read_addr;
	UPDATE_ERROR_TYPE_E ret = UPDATE_NO_ERROR;

	if(0 == stm32_flashErase(write_addr, file_len))
    {
		if(0 != stm32_flashWrite_Dire(write_addr, pread, (file_len)))
		{
			ret = UPDATE_ERROR_FLASHWRITE;
		}
	}
	else
	{
		ret = UPDATE_ERROR_FLASHERASE;
	}

	return ret;
}

static UPDATE_ERROR_TYPE_E _check_block_crc(uint32_t addr, uint32_t len, uint32_t crc)
{
	uint32_t cal_crc = 0;

	cal_crc = crc32((uint8_t *)addr, len);
	if(cal_crc != crc)
	{
		return UPDATE_ERROR_FILECRC;
	}
	return UPDATE_NO_ERROR;
}

static UPDATE_ERROR_TYPE_E _check_two_block_crc(uint32_t addr, uint32_t len, uint32_t addr2, uint32_t len2, uint32_t crc)
{
	uint32_t cal_crc = 0;

	cal_crc = crc32((uint8_t *)addr, len);
	cal_crc = add_crc32((uint8_t *)addr2, len2, cal_crc^0xFFFFFFFF);
	if(cal_crc != crc)
	{
		return UPDATE_ERROR_FILECRC;
	}
	else
	{
		return UPDATE_NO_ERROR;
	}
}

UPDATE_ERROR_TYPE_E _copy_app_to_factory(void)
{
	UPDATE_ERROR_TYPE_E ret = UPDATE_NO_ERROR;
	
	// 执行升级操作
	APP_LOG_trace("Start copy APP1 to Factory...\r\n");
	ret = _flash_write_flash(FLASH_APP_START_ADDR, 
							FLASH_FACTORY_START_ADDR, 
							FLASH_FACTORY_SIZE);
	APP_LOG_trace("Stop,copy APP1 to Factory finish!!!\r\n");
    
	return ret;
}


UPDATE_ERROR_TYPE_E _copy_factory_to_app(void)
{
	UPDATE_ERROR_TYPE_E ret = UPDATE_NO_ERROR;

	// 执行升级操作
	APP_LOG_trace("Start copy Factory to APP1...\r\n");
	ret = _flash_write_flash(FLASH_FACTORY_START_ADDR, 
							FLASH_APP_START_ADDR, 
							FLASH_FACTORY_SIZE);	
	APP_LOG_trace("Stop,copy Factory to APP1 finish!!!\r\n");
	
	return ret;
}

static UPDATE_ERROR_TYPE_E _copy_download_to_app(uint32_t file_len)
{
	UPDATE_ERROR_TYPE_E ret = UPDATE_NO_ERROR;

	if(file_len <= FLASH_DOWNLOAD_SIZE)
	{
		ret = _flash_write_flash(FLASH_DOWNLOAD_START_ADDR,
								FLASH_APP_START_ADDR,
								file_len);
	}		
	else
	{
		ret = _flash_write_flash(FLASH_DOWNLOAD_START_ADDR,
								FLASH_APP_START_ADDR,
								FLASH_DOWNLOAD_SIZE);
		ret = _flash_write_dire_flash(FLASH_DOWNLOAD1_START_ADDR,
								FLASH_APP_START_ADDR+FLASH_DOWNLOAD_SIZE,
								file_len-FLASH_DOWNLOAD_SIZE);
	}

	return ret;
}


static void _set_factory_firmware_setting_info()
{
	firmware_setting_info2.download_len = FLASH_FACTORY_SIZE;
	firmware_setting_info2.reserved[0] = 0;
	firmware_setting_info2.reserved[1] = 0;
	firmware_setting_info2.reserved[2] = 0;
	firmware_setting_info2.reserved[3] = 0;
	firmware_setting_info2.firmware_info.file_crc = crc32((uint8_t *)FLASH_FACTORY_START_ADDR, FLASH_FACTORY_SIZE);
	firmware_setting_info2.firmware_info.file_len = FLASH_FACTORY_SIZE;
	firmware_setting_info2.firmware_info.device_type = MASTER_CONTROL_DEV;
	firmware_setting_info2.firmware_info.version_h = APP_VERSION_H_DEFAULT;
	firmware_setting_info2.firmware_info.version_m = APP_VERSION_M_DEFAULT;	
	firmware_setting_info2.firmware_info.version_l = APP_VERSION_L_DEFAULT;
}

/*****************************************************************************
 * 函 数 名  : app_para_init
 * 负 责 人  : Z&L
 * 创建日期  : 2021年10月9日
 * 函数功能  : 从Flash中读取升级信息
 * 输入参数  : 无
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它    : 

*****************************************************************************/
static int _app_para_init(void)
{
	int rc = 0;
	
	rc = APP_CONFIG_firmware_setting_info_load();
	if(APP_OK != rc)
	{
		APP_CONFIG_firmware_setting_info_all_recovery();
	}
	APP_CHECK_RC(rc)

	return APP_OK;
}

/*****************************************************************************
 * 函 数 名  : _app_para_clear
 * 负 责 人  : Z&L
 * 创建日期  : 2021年10月11日
 * 函数功能  : 清除APP更新信息参数
 * 输入参数  : 无
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它    : 

*****************************************************************************/
static void _app_para_clear(void)
{
	if(APP_OK == APP_CONFIG_firmware_setting_info_write(APP_CONFIG_FIRMWARE_SETTING_INFO_BACK, &firmware_setting_info2))
	{
	}
}


static uint32_t _check_boot_update_info(void)
{
    uint32_t flag = OLD_FILE;
	
    return flag;
}

// 搬运前，检查 Download 文件信息，以及文件的 PN_CODE
static uint32_t _check_app_update_info(uint8_t *ret)
{
	uint32_t flag = OLD_FILE;
	uint32_t download_pn_code_addr = FLASH_DOWNLOAD_START_ADDR + 0x400;
	uint32_t app_pn_code_addr = FLASH_APP_START_ADDR + 0x400;
	uint8_t pn_code_str_len = 0;
	uint8_t i = 0;
	
	APP_LOG_trace("PN_CODE:");
	while ((*(uint8_t *)app_pn_code_addr != '_') 
		&& (*(uint8_t *)app_pn_code_addr)
		&& (*(uint8_t *)app_pn_code_addr < 0x80))
	{
		APP_LOG_trace("%c", *(uint8_t *)app_pn_code_addr);
		pn_code_str_len++;
		app_pn_code_addr++;
	}
	APP_LOG_trace("\r\n");

	*ret = UPDATE_NO_ERROR;
	if(firmware_setting_info2.download_len != firmware_setting_info2.firmware_info.file_len)
	{// download 尚未完成
		*ret = UPDATE_ERROR_DOWNLOAD_UNFINISH;
	}
	else if(firmware_setting_info1.firmware_info.file_crc == firmware_setting_info2.firmware_info.file_crc)
	{// download 完成, flush 完成, 成功跳转到了 APP 中
		*ret = UPDATE_ERROR_FILE_EXIST;
	}
	else
	{		
		app_pn_code_addr = FLASH_APP_START_ADDR + 0x400;
		for(i=0; i<pn_code_str_len; i++)
		{
			if(0 == *(uint8_t *)(download_pn_code_addr+i))
			{
				break;
			}
			if(*(uint8_t *)(download_pn_code_addr+i) != *(uint8_t *)(app_pn_code_addr+i))
			{
				break;
			}
		}
		
		if(i == pn_code_str_len)
		{	// DOWNLOAD 文件的 PN_CODE 与 APP 文件的 PN_CODE 相同
			flag = NEW_FILE;// 需要执行 flush
			APP_LOG_trace("find new app file\r\n");
		}
		else
		{
			*ret = UPDATE_ERROR_FILETYPE;
			APP_LOG_trace("DOWNLOAD PN_CODE data:");
			for(i=0; i<pn_code_str_len; i++)
			{
				APP_LOG_trace("%c", *(uint8_t *)download_pn_code_addr);
				download_pn_code_addr++;
			}
			APP_LOG_trace("\r\n");
		}
	}
	return flag;
}


/*****************************************************************************
 * 函 数 名  : _app_info_check
 * 负 责 人  : Z&L
 * 创建日期  : 2021年10月9日
 * 函数功能  : 检查信息并执行升级操作
 * 输入参数  : 无
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它    : 

*****************************************************************************/
static UPDATE_ERROR_TYPE_E _app_info_check(void)
{
	UPDATE_ERROR_TYPE_E ret = UPDATE_NO_ERROR;
	uint32_t file_crc = firmware_setting_info2.firmware_info.file_crc;
	uint32_t file_len = firmware_setting_info2.firmware_info.file_len;
    if(NEW_FILE == _check_app_update_info(&ret))
    {	// 将 DOWNLOAD (和 DOWNLOAD1)区的数据写入到 APP 区
		ret = _copy_download_to_app(file_len);
		ret = _check_block_crc(FLASH_APP_START_ADDR, file_len, file_crc);
		if(UPDATE_NO_ERROR != ret)
		{
			if(file_len <= FLASH_DOWNLOAD_SIZE)
			{
				ret = _check_block_crc(FLASH_DOWNLOAD_START_ADDR, file_len, file_crc);
			}
			else
			{
				ret = _check_two_block_crc(FLASH_DOWNLOAD_START_ADDR, FLASH_DOWNLOAD_SIZE,
										FLASH_DOWNLOAD1_START_ADDR, file_len - FLASH_DOWNLOAD_SIZE,
										file_crc);
			}
			if(UPDATE_NO_ERROR == ret)
			{
				ret = _copy_download_to_app(file_len);
				ret = _check_block_crc(FLASH_APP_START_ADDR, file_len, file_crc);
			}
			else
			{
				ret = UPDATE_ERROR_DOWNLOAD_DATA;
			}
		}
    }
	else
	{

	}
	APP_UPDATE_FIRMWARE_showErrorTypeContent(ret);
	
	return ret;
}


static uint8_t _check_app_factory_exit(void)
{
	uint8_t ret = APP1_FACTORY_FILE_NON_EXIST;
	uint32_t first_data = 0x0;
	uint8_t file_name_str[64] = {0};

	// 分别检查 0x8010000 和 0x8010400 地址的数据是否是有效数据
	stm32_flashRead(FLASH_APP_START_ADDR, &first_data, 1);
	stm32_flashRead(FLASH_APP_START_ADDR + 0x400, (uint32_t *)file_name_str, 64>>2);
	if((0xFFFFFFFF != first_data) 
	&& (NULL != strstr(file_name_str, app_file_name_head_str)))
	{
		ret |= ONLY_APP1_FILE_EXIST;
		APP_LOG_trace("APP file exist\r\n");
	}
	// first_data = 0x0;
	// memset(file_name_str, 0, 64);
	// stm32_flashRead(FLASH_FACTORY_START_ADDR, &first_data, 1);
	// stm32_flashRead(FLASH_FACTORY_START_ADDR + 0x400, (uint32_t *)file_name_str, 64>>2);
	// if((0xFFFFFFFF != first_data) 
	// && (NULL != strstr(file_name_str, app_file_name_head_str)))
	// {
	// 	ret |= ONLY_FACTORY_FILE_EXIST;
	// 	APP_LOG_trace("Factory file exist\r\n");
	// }
	
	return ret;
}

static uint8_t _recovery_to_factory(void)
{
	uint8_t ret = UPDATE_NO_ERROR;

	ret = _copy_factory_to_app();
	APP_CONFIG_firmware_setting_info_update_fail();

	return ret;
}



UPDATE_ERROR_TYPE_E APP_UPDATE_FIRMWARE_factory_check(void)
{
	uint8_t ret = APP1_FACTORY_FILE_NON_EXIST;
	// 2、判断 APP 区是否有数据
	ret = _check_app_factory_exit();
	if(ONLY_APP1_FILE_EXIST == ret)
	{
		// 3、拷贝 APP 区到 Factory 区(128K全部拷贝)
		// APP_UPDATE_FIRMWARE_showErrorTypeContent(_copy_app_to_factory());
		// ret = APP_CONFIG_firmware_setting_info_all_recovery();
	}
	else if((APP1_FACTORY_FILE_NON_EXIST == ret) || (ONLY_FACTORY_FILE_EXIST == ret))
	{
		// if(ONLY_FACTORY_FILE_EXIST == ret)
		// {
		// 	UPDATE_ERROR_TYPE_E check = UPDATE_NO_ERROR;
		// 	check = _recovery_to_factory();
		// 	if(UPDATE_NO_ERROR == check)
		// 	{
		// 		HARDWARE_HAL_DELAY_MS(1000);
		// 		_iap_load_app(FLASH_APP_START_ADDR);
		// 	}
		// }
		return UPDATE_ERROR_NO_FIRMWARE;
	}
	
	return UPDATE_NO_ERROR;
}



UPDATE_ERROR_TYPE_E APP_UPDATE_FIRMWARE_check_file(void)
{
	UPDATE_ERROR_TYPE_E check = UPDATE_NO_ERROR;
    int rc = APP_OK;

	// DEBUG 时设置 EEPROM 中的参数值
	firmware_setting_info_test_send();
	// 读取 EEPROM 中的 firmware setting 信息
	rc = _app_para_init();
	if(APP_OK != rc)
	{
		APP_LOG_trace("%s", erase_firmware_info_str);
		HARDWARE_HAL_DELAY_MS(100);
		return check;
	}
	// 检查 APP 信息字段和 Download 区数据内容
	APP_LOG_trace("%s", app_info_check_str);
	HARDWARE_HAL_DELAY_MS(10);
	check = _app_info_check();
	if(UPDATE_NO_ERROR == check)
	{
		// flush 成功, 更新 firmware_setting_info1
		APP_CONFIG_firmware_setting_info_overwrite();
		APP_LOG_trace("%s\r\n", app_info_success_str);
		HARDWARE_HAL_DELAY_MS(100);
		// 跳转到对应的 APP 程序中
	}
	else if((UPDATE_ERROR_FILE_EXIST == check) || (UPDATE_ERROR_DOWNLOAD_UNFINISH == check) || (UPDATE_ERROR_FILETYPE == check))
	{	// 无固件更新，直接运行原 APP 程序
		if(UPDATE_ERROR_FILETYPE == check)
		{
			APP_CONFIG_firmware_setting_info_update_fail1();
		}
		// 跳转到对应的 APP 程序中
	}
	else	// 校验出错，使用 Factory 区数据填充 APP 区
	{
		// 当校验 Download 区数据有误时，将 Factory 中的数据拷贝到 APP 中
		check = _recovery_to_factory();
		if(UPDATE_NO_ERROR == check)
		{
			HARDWARE_HAL_DELAY_MS(1000);
			// 跳转到对应的 APP 程序中
		}
	}
	return check;
}