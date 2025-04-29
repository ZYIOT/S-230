

#include "app_update_firmware.h"
#include "stm32_flash.h"
#include "board_config.h"
#include "bsp_board_debug.h"
#include "bsp_mcu.h"
#include "bsp_wdg.h"
#include "app_config/map.h"
#include "app_config/defines.h"
#include "app_config/message.h"
#include "app_log.h"
#include "utils.h"

const uint8_t c_appInfoCheckStr[] = "waiting for check app info ... ...\r\n";
const uint8_t c_appInfoSuccessStr[] = "app info check success !!!\r\nnow run application\r\n";
const uint8_t c_eraseFirmwareInfoStr[] = "erase firmware info page... ...\r\nnow run application\r\n";
const uint8_t c_appFileNameHeadStr[] = "_APP";

const errorTypeString_t c_updateErrorContent[] = 
{
	// 与定义的 UPDATE_ERROR_TYPE_e 中定义的类型和位置一一对应
	{UPDATE_NO_ERROR, "no error\r\n"},
	{UPDATE_ERROR_BINTYPE, "bin type is error\r\n"},
	{UPDATE_ERROR_FILETYPE, "file type is error\r\n"},
	{UPDATE_ERROR_FILELEN, "file len is error\r\n"},
	{UPDATE_ERROR_FILECRC, "file crc is error\r\n"},
	{UPDATE_ERROR_OFFSET, "offset address is error\r\n"},
	{UPDATE_ERROR_FRAMELEN, "frame len is error\r\n"},
	{UPDATE_ERROR_FLASHERASE, "erase flash is error\r\n"},
	{UPDATE_ERROR_FLASHWRITE, "write flash is error\r\n"},
	{UPDATE_ERROR_RETRYFAIL, "retry send max but fail\r\n"},
	{UPDATE_ERROR_FILE_EXIST, "firmware file exist\r\n"},
	{UPDATE_ERROR_DOWNLOAD_DATA, "download data check error\r\n"},
	{UPDATE_ERROR_DOWNLOAD_UNFINISH, "download data not finish\r\n"},
	{UPDATE_ERROR_NO_FIRMWARE, "no firmware exist\r\n"},
	
	{UPDATE_ERROR_END1, "update flow is end\r\n"},
};

const pnCheckResult_t c_pnCheckArray[] = 
{
	{"S-230_APP", UPDATE_NO_ERROR}, 
	{"S-230_BOOT", UPDATE_ERROR_BINTYPE},
	{"S-230_G_APP", UPDATE_ERROR_BINTYPE},
	{"S-230_G_BOOT", UPDATE_ERROR_BINTYPE},
	{NULL, UPDATE_ERROR_FILETYPE}
};



// 本地 boot 程序文件默认信息
static firmwareInfo_t s_localBootInfo = 
{
	.fileLen = FLASH_BOOTLOADER_SIZE,
	.fileCrc = DEFAULT_CRC32_VALUE,
	.deviceType = UNKONW_DEV,
};

// 本地 app 程序文件默认信息
static firmwareInfo_t s_localAppInfo = 
{
	.fileLen = FLASH_APP_SIZE,
	.fileCrc = DEFAULT_CRC32_VALUE,
	.deviceType = UNKONW_DEV,
};

// 本地 download 区程序文件默认信息
static firmwareInfo_t s_localDownloadInfo = 
{
	.fileLen = FLASH_DOWNLOAD_SIZE,
	.fileCrc = DEFAULT_CRC32_VALUE,
	.deviceType = UNKONW_DEV,
};

void APP_UPDATE_FIRMWARE_ShowErrorTypeContent(UPDATE_ERROR_TYPE_e type)
{
	uint8_t index = 0;
	uint8_t count = sizeof(c_updateErrorContent)/sizeof(errorTypeString_t);

	if(UPDATE_NO_ERROR != type)
	{
		for(index=1; index<count; index++)
		{
			if(type == c_updateErrorContent[index].type)
			{
				break;
			}
		}
		APP_LOG_trace("[ERROR:%d]:%s", index, c_updateErrorContent[index].string);
	}
}

__weak void FirmwareSettingInfoTestSend(void)
{
}
__weak void FirmwareSettingInfoTestRecv(void)
{
}


static UPDATE_ERROR_TYPE_e FlashWriteDireFlash(uint32_t readAddr, uint32_t writeAddr, uint32_t fileLen)
{
	uint32_t *pread = (uint32_t *)readAddr;
	UPDATE_ERROR_TYPE_e ret = UPDATE_NO_ERROR;

	if(0 != stm32_flashWrite_Dire(writeAddr, pread, fileLen))
	{
		ret = UPDATE_ERROR_FLASHWRITE;
	}

	return ret;
}

static UPDATE_ERROR_TYPE_e FlashWriteFlash(uint32_t readAddr, uint32_t writeAddr, uint32_t fileLen)
{
	uint32_t *pread = (uint32_t *)readAddr;
	UPDATE_ERROR_TYPE_e ret = UPDATE_NO_ERROR;

	if(0 == stm32_flashErase(writeAddr, fileLen))
    {
		if(0 != stm32_flashWrite_Dire(writeAddr, pread, (fileLen)))
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

// 擦除 download 区 
static UPDATE_ERROR_TYPE_e FlashEarseDownload(void)
{
	APP_LOG_debug("Erase Download Sector\r\n");
	stm32_flashErase(FLASH_DOWNLOAD_START_ADDR, FLASH_DOWNLOAD_SIZE);
	stm32_flashErase(FLASH_DOWNLOAD1_START_ADDR, FLASH_DOWNLOAD1_SIZE);
	return UPDATE_NO_ERROR;
}

static UPDATE_ERROR_TYPE_e CheckBlockCrc(uint32_t addr, uint32_t len, uint32_t crc)
{
	uint32_t calCrc = 0;

	calCrc = crc32((uint8_t *)addr, len);
	if(calCrc != crc)
	{
		return UPDATE_ERROR_FILECRC;
	}
	return UPDATE_NO_ERROR;
}

static UPDATE_ERROR_TYPE_e CheckTwoBlockCrc(uint32_t addr, uint32_t len, uint32_t addr2, uint32_t len2, uint32_t crc)
{
	uint32_t calCrc = 0;

	calCrc = crc32((uint8_t *)addr, len);
	calCrc = add_crc32((uint8_t *)addr2, len2, calCrc^0xFFFFFFFF);
	if(calCrc != crc)
	{
		return UPDATE_ERROR_FILECRC;
	}
	return UPDATE_NO_ERROR;
}

static UPDATE_ERROR_TYPE_e CopyAppToFactory(void)
{
	UPDATE_ERROR_TYPE_e ret = UPDATE_NO_ERROR;
	
	// 执行升级操作
	APP_LOG_trace("Start copy APP1 to Factory...\r\n");
	ret = FlashWriteFlash(FLASH_APP_START_ADDR, 
							FLASH_FACTORY_START_ADDR, 
							FLASH_FACTORY_SIZE);
	APP_LOG_trace("Stop,copy APP1 to Factory finish!!!\r\n");
    
	return ret;
}


static UPDATE_ERROR_TYPE_e CopyFactoryToApp(void)
{
	UPDATE_ERROR_TYPE_e ret = UPDATE_NO_ERROR;

	// 执行升级操作
	APP_LOG_trace("Start copy Factory to APP1...\r\n");
	ret = FlashWriteFlash(FLASH_FACTORY_START_ADDR, 
							FLASH_APP_START_ADDR, 
							FLASH_FACTORY_SIZE);	
	APP_LOG_trace("Stop,copy Factory to APP1 finish!!!\r\n");
	
	return ret;
}

static UPDATE_ERROR_TYPE_e CopyDownloadToApp(uint32_t fileLen)
{
	UPDATE_ERROR_TYPE_e ret = UPDATE_NO_ERROR;

	if(fileLen <= FLASH_DOWNLOAD_SIZE)
	{
		ret = FlashWriteFlash(FLASH_DOWNLOAD_START_ADDR,
								FLASH_APP_START_ADDR,
								fileLen);
	}		
	else
	{
		ret = FlashWriteFlash(FLASH_DOWNLOAD_START_ADDR,
								FLASH_APP_START_ADDR,
								FLASH_DOWNLOAD_SIZE);
		ret = FlashWriteDireFlash(FLASH_DOWNLOAD1_START_ADDR,
								FLASH_APP_START_ADDR+FLASH_DOWNLOAD_SIZE,
								fileLen-FLASH_DOWNLOAD_SIZE);
	}

	return ret;
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
static int AppParamentsInit(void)
{
	int rc = 0;
	
	rc = APP_CONFIG_FirmwareInfoLoad();
	if(APP_OK != rc)
	{
		APP_CONFIG_AllFirmwareInfoRecovery();
	}
	APP_CHECK_RC(rc)

	return APP_OK;
}

/*****************************************************************************
 * 函 数 名  : AppParamentsClear
 * 负 责 人  : Z&L
 * 创建日期  : 2021年10月11日
 * 函数功能  : 清除APP更新信息参数
 * 输入参数  : 无
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它    : 

*****************************************************************************/
static void AppParamentsClear(void)
{
	if(APP_OK == APP_CONFIG_FirmwareInfoWrite(DOWNLOAD_FIRMWARE_INFO, &g_downloadFirmwareInfo))
	{
	}
}


static uint32_t CheckBootUpdateInfo(void)
{
    uint32_t flag = OLD_FILE;
	
    return flag;
}

// 搬运前，检查 Download 文件信息，以及文件的 PN_CODE
#if 0
static uint32_t CheckAppUpdateInfo(uint8_t *ret)
{
	uint32_t flag = OLD_FILE;
	uint32_t download_pn_code_addr = FLASH_DOWNLOAD_START_ADDR + 0x400;
	uint8_t downloadPnCode[32] = {0};
	uint8_t pnCodeKeyStrLen = 0;
	uint8_t i = 0;
	
	APP_LOG_trace("PN_CODE:");
	*ret = UPDATE_NO_ERROR;
	if((0 == g_downloadFirmwareInfo.downloadLen) 
		|| (IAP_UPDATE_UNKNOW == g_downloadFirmwareInfo.updateResult) 
		|| (UNKONW_FIRMWARE == g_downloadFirmwareInfo.firmwareType) 
		|| ((FLASH_DOWNLOAD_SIZE == g_downloadFirmwareInfo.firmwareInfo.fileLen) && (DEFAULT_CRC32_VALUE == g_downloadFirmwareInfo.firmwareInfo.fileCrc)))
	{	// download 完成, flush 完成, 成功跳转到了 APP 中
		*ret = UPDATE_ERROR_FILE_EXIST;
	}
	// else if()
	// {	// download 未完成 
	// 	*ret = UPDATE_ERROR_DOWNLOAD_UNFINISH;
	// }
	else
	{
		uint8_t arraySize = sizeof(c_pnCheckArray)/sizeof(c_pnCheckArray[0]);
		uint8_t index = 0;
		stm32_flashRead(download_pn_code_addr, (uint32_t *)downloadPnCode, 32/sizeof(uint32_t));
		APP_LOG_debug("%s\r\n", downloadPnCode);
		for(index=0; index<arraySize; index++)
		{
			if(NULL == c_pnCheckArray[index].pnKeyWords)
			{
				break;
			}
			pnCodeKeyStrLen = strlen(c_pnCheckArray[index].pnKeyWords);
			// APP_LOG_debug("PN keystr len = %d, str:%s\r\n", pnCodeKeyStrLen, c_pnCheckArray[index].pnKeyWords);
			for(i=0; i<pnCodeKeyStrLen; i++)
			{
				if(downloadPnCode[i] != c_pnCheckArray[index].pnKeyWords[i])
				{
					break;
				}
			}
			if(i == pnCodeKeyStrLen)
			{
				break;
			}
		}
		if(index >= arraySize - 1)
		{
			// 不在支持的固件类型列表中，判定为不识别的固件类型 
			*ret = UPDATE_ERROR_FILETYPE;
		}
		else if(i == pnCodeKeyStrLen)
		{
			if(0 == index)
			{
				// 是自身的 APP 固件 
				*ret = UPDATE_NO_ERROR;
				flag = NEW_FILE;// 需要执行 flush
				APP_LOG_trace("find new app file\r\n");
			}
			else
			{
				*ret = UPDATE_ERROR_BINTYPE;

			}
		}
	}
	return flag;
}
#else
char *board_pn_key_table[] = 
{
	"S-230_APP", 
};

static uint32_t CheckAppUpdateInfo(uint8_t *ret)
{
	uint32_t flag = OLD_FILE;
	APP_CONFIG_FirmwareInfo_pt info = &g_downloadFirmwareInfo;
	char *downloadPnName = (char *)(FLASH_DOWNLOAD_START_ADDR + 0x400);
	char *appPnName = (char *)(FLASH_APP_START_ADDR + 0x400);
	uint8_t findSubstrFlag = 0;
	uint8_t i = 0;

	APP_LOG_trace("PN_CODE:");
	*ret = UPDATE_NO_ERROR;
	if((0 == info->downloadLen) 
		|| (IAP_UPDATE_UNKNOW == info->updateResult) 
		|| (UNKONW_FIRMWARE == info->firmwareType) 
		|| (DEFAULT_CRC32_VALUE == info->firmwareInfo.fileCrc))
	{	// download 完成, flush 完成, 成功跳转到了 APP 中
		*ret = UPDATE_ERROR_FILE_EXIST;
	}
	// else if()
	// {	// download 未完成 
	// 	*ret = UPDATE_ERROR_DOWNLOAD_UNFINISH;
	// }
	else
	{
		for(i = 0; i < ARRAY_LEN(board_pn_key_table); i++)
		{
			if(NULL != strstr(downloadPnName, board_pn_key_table[i]))
			{
				findSubstrFlag = 1;
			}
		}
		if(findSubstrFlag)
		{	// DOWNLOAD 文件的 PN_CODE 与 APP 文件的 PN_CODE 相同
			flag = NEW_FILE;// 需要执行 flush
			APP_LOG_trace("find new app file\r\n");
		}
		else
		{
			*ret = UPDATE_ERROR_BINTYPE;
			APP_LOG_trace("DOWNLOAD PN_CODE data:%s\r\n", downloadPnName);
		}
	}
}
#endif


/*****************************************************************************
 * 函 数 名  : AppInfoCheck
 * 负 责 人  : Z&L
 * 创建日期  : 2021年10月9日
 * 函数功能  : 检查信息并执行升级操作
 * 输入参数  : 无
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它    : 

*****************************************************************************/
static UPDATE_ERROR_TYPE_e AppInfoCheck(void)
{
	UPDATE_ERROR_TYPE_e ret = UPDATE_NO_ERROR;
	UPDATE_ERROR_TYPE_e rc = UPDATE_NO_ERROR;
	uint32_t fileCrc = g_downloadFirmwareInfo.firmwareInfo.fileCrc;
	uint32_t fileLen = g_downloadFirmwareInfo.firmwareInfo.fileLen;
	
    if(NEW_FILE == CheckAppUpdateInfo(&rc))
    {	
		if(fileLen > FLASH_DOWNLOAD_SIZE)
		{
			rc = CheckTwoBlockCrc(FLASH_DOWNLOAD_START_ADDR, FLASH_DOWNLOAD_SIZE, FLASH_DOWNLOAD1_START_ADDR, (fileLen - FLASH_DOWNLOAD_SIZE), fileCrc);
		}
		else 
		{
			rc = CheckBlockCrc(FLASH_DOWNLOAD_START_ADDR, fileLen, fileCrc);
		}
		if(UPDATE_NO_ERROR != rc)
		{
			return UPDATE_ERROR_DOWNLOAD_DATA;
		}
		else
		{
			// 将 DOWNLOAD (和 DOWNLOAD1)区的数据写入到 APP 区
			rc = CopyDownloadToApp(fileLen);
			if(UPDATE_NO_ERROR != rc)
			{
				HARDWARE_HAL_DELAY_MS(500);
				rc = CopyDownloadToApp(fileLen);
				if(UPDATE_NO_ERROR != rc)
				{
					ret = UPDATE_ERROR_FLASHWRITE;
					goto write_app_factory;
				}
			}
			rc = CheckBlockCrc(FLASH_APP_START_ADDR, fileLen, fileCrc);
			if(UPDATE_NO_ERROR != rc)
			{
				ret = UPDATE_ERROR_FLASHWRITE;
				goto write_app_factory;
			}
		}
	}
	else
	{
		ret = rc;
	}
	return ret;
write_app_factory:
#if (1 == HAS_FACTORY_SECTOR)
	CopyFactoryToApp();
#endif	
	return ret;
}


static uint8_t CheckAppFactoryExit(void)
{
	uint8_t ret = APP1_FACTORY_FILE_NON_EXIST;
	uint32_t firstData = 0x0;
	uint8_t fileNameStr[64] = {0};

	// 分别检查 0x8010000 和 0x8010400 地址的数据是否是有效数据
	stm32_flashRead(FLASH_APP_START_ADDR, &firstData, 1);
	stm32_flashRead(FLASH_APP_START_ADDR + 0x400, (uint32_t *)fileNameStr, 64>>2);
	if((0xFFFFFFFF != firstData) 
	&& (NULL != strstr(fileNameStr, c_appFileNameHeadStr)))
	{
		ret |= ONLY_APP1_FILE_EXIST;
		APP_LOG_trace("APP file exist\r\n");
	}
	// firstData = 0x0;
	// memset(fileNameStr, 0, 64);
	// stm32_flashRead(FLASH_FACTORY_START_ADDR, &firstData, 1);
	// stm32_flashRead(FLASH_FACTORY_START_ADDR + 0x400, (uint32_t *)fileNameStr, 64>>2);
	// if((0xFFFFFFFF != firstData) 
	// && (NULL != strstr(fileNameStr, c_pnCheckArray[0].pnKeyWords)))
	// {
	// 	ret |= ONLY_FACTORY_FILE_EXIST;
	// 	APP_LOG_trace("Factory file exist\r\n");
	// }
	
	return ret;
}


UPDATE_ERROR_TYPE_e APP_UPDATE_FIRMWARE_FactoryCheck(void)
{
	uint8_t ret = APP1_FACTORY_FILE_NON_EXIST;
	// 2、判断 APP 区是否有数据
	ret = CheckAppFactoryExit();
	if(ONLY_APP1_FILE_EXIST == ret)
	{
		// 3、拷贝 APP 区到 Factory 区(128K全部拷贝)
		// APP_UPDATE_FIRMWARE_ShowErrorTypeContent(CopyAppToFactory());
	}
	// else if(ONLY_FACTORY_FILE_EXIST == ret)
	// {
	// 	UPDATE_ERROR_TYPE_e check = UPDATE_NO_ERROR;
	// 	check = _recovery_to_factory();
	// 	if(UPDATE_NO_ERROR == check)
	// 	{
	// 		HARDWARE_HAL_DELAY_MS(1000);
	// 		IapLoadApp(FLASH_APP_START_ADDR);
	// 	}
	// 	return UPDATE_ERROR_NO_FIRMWARE;
	// }
	else if(APP1_FACTORY_FILE_NON_EXIST == ret)
	{
		return UPDATE_ERROR_NO_FIRMWARE;
	}
	
	return UPDATE_NO_ERROR;
}



UPDATE_ERROR_TYPE_e APP_UPDATE_FIRMWARE_CheckFile(void)
{
	UPDATE_ERROR_TYPE_e check = UPDATE_NO_ERROR;
	uint8_t eraseDownloadDataFlag = 0;
    int rc = APP_OK;

	// DEBUG 时设置 EEPROM 中的参数值
	FirmwareSettingInfoTestSend();
	// 读取 EEPROM 中的 firmware setting 信息
	rc = AppParamentsInit();
	if(APP_OK != rc)
	{
		APP_LOG_trace("%s", c_eraseFirmwareInfoStr);
		HARDWARE_HAL_DELAY_MS(100);
		return rc;
	}
	// 检查 APP 信息字段和 Download 区数据内容
	APP_LOG_trace("%s", c_appInfoCheckStr);
	HARDWARE_HAL_DELAY_MS(10);
	check = AppInfoCheck();
	APP_UPDATE_FIRMWARE_ShowErrorTypeContent(check);
	if(UPDATE_NO_ERROR == check)
	{
		// flush 成功, 更新 g_appFirmwareInfo 
		eraseDownloadDataFlag = 1;
		APP_CONFIG_AppFirmwareInfoSync(IAP_UPDATE_SUCESS);
		APP_LOG_trace("%s\r\n", c_appInfoSuccessStr);
		HARDWARE_HAL_DELAY_MS(100);
		// 跳转到对应的 APP 程序中
	}
	else if((UPDATE_ERROR_FILE_EXIST == check) || (UPDATE_ERROR_DOWNLOAD_UNFINISH == check) || (UPDATE_ERROR_BINTYPE == check))
	{	
		if(UPDATE_ERROR_BINTYPE == check)
		{
			eraseDownloadDataFlag = 1;
		}
		// 无固件更新，直接运行原 APP 程序 
		// 跳转到对应的 APP 程序中
	}
	else if(UPDATE_ERROR_FLASHWRITE == check)	// APP烧写失败 
	{
		eraseDownloadDataFlag = 1;
		APP_CONFIG_AppFirmwareInfoSync(IAP_UPDATE_FAIL);
	}
	if(1 == eraseDownloadDataFlag)
	{
		FlashEarseDownload();
		APP_CONFIG_DownloadFirmwareInfoRecovery();
	}
	return check;
}