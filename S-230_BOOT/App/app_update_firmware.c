

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

#define DOWNLOAD_FIRMWARE_CRC	(0)
#define APP_FIRMWARE_CRC 		(1)

const uint8_t c_appInfoCheckStr[] = "waiting for check app info ... ...\r\n";
const uint8_t c_appInfoSuccessStr[] = "app info check success !!!\r\nnow run application\r\n";
const uint8_t c_eraseFirmwareInfoStr[] = "erase firmware info page... ...\r\nnow run application\r\n";
const uint8_t c_appFileNameHeadStr[] = "_APP";

const errorTypeString_t c_updateErrorContent[] = 
{
	// 与定义的 UPDATE_ERROR_TYPE_e 中定义的类型和位置一一对应
	{UPDATE_ERROR_BIN_CONTENT, "bin content is error\r\n"},
	{UPDATE_ERROR_FILELEN, "file len is error\r\n"},
	{UPDATE_ERROR_FRAMELEN, "frame len is error\r\n"},
	{UPDATE_ERROR_FLASHERASE, "erase flash is error\r\n"},
	{UPDATE_ERROR_FLASHWRITE, "write flash is error\r\n"},
	{UPDATE_ERROR_DOWNLOAD_DATA, "download data check error\r\n"},
	{UPDATE_ERROR_DOWNLOAD_UNFINISH, "download data not finish\r\n"},
	{UPDATE_ERROR_FILE_EXIST, "firmware file exist\r\n"},
	{UPDATE_ERROR_NO_FIRMWARE, "no firmware exist\r\n"},
	
	{UPDATE_ERROR_END1, "update flow is end\r\n"},
};


const pn_check_result_t c_boardPnKeyTable[] = 
{
	{"S-230_APP"},
	{"S-230_BOOT"},
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

// 【注意】直接擦除 APP 的整个 192K 可能会导致 WDG 复位(具体参照STM32/GD32的芯片手册说明)，因此在擦除 APP 时采用分段擦除 
static UPDATE_ERROR_TYPE_e FlashEraseFlash(uint32_t startAddr, uint32_t len) 
{
	UPDATE_ERROR_TYPE_e ret = UPDATE_NO_ERROR;
	BSP_WDG_feed();
	if(0 != stm32_flashErase(startAddr, len))
	{
		ret = UPDATE_ERROR_FLASHERASE;
	}
	BSP_WDG_feed();
	return ret;
}

static UPDATE_ERROR_TYPE_e FlashWriteDireFlash(uint32_t readAddr, uint32_t writeAddr, uint32_t fileLen)
{
	uint32_t *pread = (uint32_t *)readAddr;
	UPDATE_ERROR_TYPE_e ret = UPDATE_NO_ERROR;
    BSP_WDG_feed();
	if(0 != stm32_flashWrite_Dire(writeAddr, pread, fileLen))
	{
		ret = UPDATE_ERROR_FLASHWRITE;
	}
    BSP_WDG_feed();
	return ret;
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

static UPDATE_ERROR_TYPE_e AppCheckFirmwareCrc(uint8_t type, uint32_t len, uint32_t crc)
{
	UPDATE_ERROR_TYPE_e ret = UPDATE_NO_ERROR;
	if(DOWNLOAD_FIRMWARE_CRC == type)
	{
		if(len <= FLASH_DOWNLOAD_SIZE)
		{
			ret = CheckBlockCrc(FLASH_DOWNLOAD_START_ADDR, len, crc);
		}
		else
		{
			ret = CheckTwoBlockCrc(FLASH_DOWNLOAD_START_ADDR, FLASH_DOWNLOAD_SIZE,
									FLASH_DOWNLOAD1_START_ADDR, len - FLASH_DOWNLOAD_SIZE,
									crc);
		}
	}
	else if(APP_FIRMWARE_CRC == type)
	{
		ret = CheckBlockCrc(FLASH_APP_START_ADDR, len, crc);
	}
	return ret;
}

static UPDATE_ERROR_TYPE_e CopyAppToFactory(void)
{
	UPDATE_ERROR_TYPE_e ret = UPDATE_NO_ERROR;
	
	// 执行升级操作
	APP_LOG_trace("Start copy APP1 to Factory...\r\n");
	ret = FlashEraseFlash(FLASH_FACTORY_START_ADDR, FLASH_FACTORY_SIZE);
	ret = FlashWriteDireFlash(FLASH_APP_START_ADDR, FLASH_FACTORY_START_ADDR, FLASH_FACTORY_SIZE);
	APP_LOG_trace("Stop,copy APP1 to Factory finish!!!\r\n");
    
	return ret;
}


static UPDATE_ERROR_TYPE_e CopyFactoryToApp(void)
{
	UPDATE_ERROR_TYPE_e ret = UPDATE_NO_ERROR;

	// 执行升级操作
	APP_LOG_trace("Start copy Factory to APP1...\r\n");
	ret = FlashEraseFlash(FLASH_APP_START_ADDR, FLASH_APP_MAX_SIZE0);
	ret = FlashEraseFlash((FLASH_APP_START_ADDR + FLASH_APP_MAX_SIZE0), FLASH_APP_MAX_SIZE1);
	ret = FlashWriteDireFlash(FLASH_FACTORY_START_ADDR, FLASH_APP_START_ADDR, FLASH_FACTORY_SIZE);
	APP_LOG_trace("Stop,copy Factory to APP1 finish!!!\r\n");
	
	return ret;
}

static UPDATE_ERROR_TYPE_e CopyDownloadToApp(uint32_t fileLen)
{
	UPDATE_ERROR_TYPE_e ret = UPDATE_NO_ERROR;

	ret = FlashEraseFlash(FLASH_APP_START_ADDR, FLASH_APP_MAX_SIZE0);
	if(fileLen > FLASH_APP_MAX_SIZE0)
	{
		ret = FlashEraseFlash((FLASH_APP_START_ADDR + FLASH_APP_MAX_SIZE0), FLASH_APP_MAX_SIZE1);
	}
	if(fileLen <= FLASH_DOWNLOAD_SIZE)
	{
		ret = FlashWriteDireFlash(FLASH_DOWNLOAD_START_ADDR, FLASH_APP_START_ADDR, fileLen);
	}		
	else
	{
		ret = FlashWriteDireFlash(FLASH_DOWNLOAD_START_ADDR, FLASH_APP_START_ADDR, FLASH_DOWNLOAD_SIZE);
		ret = FlashWriteDireFlash(FLASH_DOWNLOAD1_START_ADDR, FLASH_APP_START_ADDR+FLASH_DOWNLOAD_SIZE, (fileLen - FLASH_DOWNLOAD_SIZE));
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
		APP_CONFIG_AppFirmwareRecovery();
		APP_CONFIG_DownloadFirmwareRecovery();
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

static uint32_t CheckAppUpdateInfo(uint8_t *ret)
{
	uint32_t flag = OLD_FILE;
	APP_CONFIG_FirmwareInfo_pt info = &g_downloadFirmwareInfo;
	char *downloadPnName = (char *)(FLASH_DOWNLOAD_START_ADDR + 0x400);
	char *appPnName = (char *)(FLASH_APP_START_ADDR + 0x400);
	uint8_t findSubstrFlag = 0;
    uint8_t count = ARRAY_LEN(c_boardPnKeyTable);
	uint8_t i = 0;

	APP_LOG_debug("APP PN_CODE:%s\r\n", appPnName);
	*ret = UPDATE_NO_ERROR;
	{
		for(i = 0; i < count; i++)
		{
			if(NULL != strstr(downloadPnName, c_boardPnKeyTable[i].pnKeyWords))
			{
				findSubstrFlag = 1;
			}
		}
		if(findSubstrFlag)
		{	// DOWNLOAD 文件的 PN_CODE 与 APP 文件的 PN_CODE 相同
			flag = NEW_FILE;// 需要执行 flush
			APP_LOG_debug("find new app file\r\n");
		}
		else
		{
			*ret = UPDATE_ERROR_FILETYPE;
			APP_LOG_error("DOWNLOAD PN_CODE data:%s\r\n", downloadPnName);
		}
	}
}

// 检查 download 区固件的完整性(文件长度、flash末尾内容、crc) 
static UPDATE_ERROR_TYPE_e CheckDownloadFirmwareIntegrity(void)
{
	UPDATE_ERROR_TYPE_e result = UPDATE_NO_ERROR;
	APP_CONFIG_FirmwareInfo_pt info = &g_downloadFirmwareInfo;
	if(APP_OK != APP_CONFIG_FirmwareCheckInfoValid(info))
	{
		return UPDATE_ERROR_FILE_EXIST;
	}
	if(info->downloadLen != info->firmwareInfo.fileLen)
	{
		result = UPDATE_ERROR_DOWNLOAD_UNFINISH;
	}
	else
	{
		uint32_t endAddr = 0;
		if(info->downloadLen > FLASH_DOWNLOAD_SIZE + FLASH_DOWNLOAD1_SIZE)
		{
			result = UPDATE_ERROR_FILELEN;
		}
		else if(info->downloadLen > FLASH_DOWNLOAD_SIZE)
		{
			endAddr = info->downloadLen - FLASH_DOWNLOAD_SIZE + FLASH_DOWNLOAD1_START_ADDR;
			if(0xFFFFFFFF != *(uint32_t *)(endAddr))
			{
				result = UPDATE_ERROR_DOWNLOAD_DATA;
			}
		}
		else if(info->downloadLen)
		{
			endAddr = FLASH_DOWNLOAD_START_ADDR + info->downloadLen;
			if(0xFFFFFFFF != *(uint32_t *)(endAddr))
			{
				result = UPDATE_ERROR_DOWNLOAD_DATA;
			}
		}
		else
		{
			result = UPDATE_ERROR_FILELEN;
		}
	}
	if(UPDATE_NO_ERROR == result)
	{
		if(UPDATE_NO_ERROR != AppCheckFirmwareCrc(DOWNLOAD_FIRMWARE_CRC, info->downloadLen, info->firmwareInfo.fileCrc))
        {
            result = UPDATE_ERROR_BIN_CONTENT;
        }
	}
	return result;
}

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


uint8_t eraseDownloadDataFlag = APP_FALSE;
uint8_t recoveryFactoryFlag = APP_FALSE;
void AppIapErrorProcess(UPDATE_ERROR_TYPE_e type)
{
    APP_UPDATE_FIRMWARE_ShowErrorTypeContent(type);
    if((UPDATE_ERROR_FILE_EXIST == type) || (UPDATE_ERROR_DOWNLOAD_UNFINISH == type))
    {
        // 固件更新完成/未下载完毕，均不作处理 
    }
    else if((UPDATE_ERROR_BIN_CONTENT == type) || (UPDATE_ERROR_DOWNLOAD_DATA == type) || (UPDATE_ERROR_FILELEN == type))
    {   // Download 区固件内容错误、区间数据错误、固件信息错误，则擦除 Download 区的 flash 数据 
        eraseDownloadDataFlag = APP_TRUE;
    }
    else if((UPDATE_ERROR_FLASHWRITE == type) || (UPDATE_ERROR_FLASHERASE == type))
    {   // 更新 APP 区间过程中出错，则擦除 Download 区 flash 数据，并将 Factory 区数据拷贝到 APP 区 
        eraseDownloadDataFlag = APP_TRUE;
        recoveryFactoryFlag = APP_TRUE;
    }
    else
    {

    }
}


UPDATE_ERROR_TYPE_e APP_UPDATE_FIRMWARE_CheckFile(void)
{
	UPDATE_ERROR_TYPE_e check = UPDATE_NO_ERROR;
    int rc = APP_OK;

	// DEBUG 时设置 EEPROM 中的参数值
	FirmwareSettingInfoTestSend();
	// 读取 EEPROM 中的 firmware setting 信息
	check = AppParamentsInit();
	if(APP_OK != rc)
	{
		APP_LOG_trace("%s", c_eraseFirmwareInfoStr);
		HARDWARE_HAL_DELAY_MS(100);
		return check;
	}
	check = CheckDownloadFirmwareIntegrity();
    if(UPDATE_NO_ERROR != check)
    {
		APP_LOG_error("file:%s, line:%d\r\n", __FILE__ ,__LINE__);
        AppIapErrorProcess(check);
    }
    else
	{
		// 检查 APP 信息字段和 Download 区数据内容
		APP_LOG_trace("%s", c_appInfoCheckStr);
		HARDWARE_HAL_DELAY_MS(10);
		check = AppInfoCheck();
        if(UPDATE_NO_ERROR != check)
        {
			APP_LOG_error("file:%s, line:%d\r\n", __FILE__ ,__LINE__);
            AppIapErrorProcess(check);
        }
        else
        {
            // flush 成功, 更新 g_appFirmwareInfo 
            eraseDownloadDataFlag = APP_TRUE;
            APP_CONFIG_AppFirmwareInfoSync(IAP_UPDATE_SUCESS);
            APP_LOG_debug("%s\r\n", c_appInfoSuccessStr);
            HARDWARE_HAL_DELAY_MS(100);
            // 跳转到对应的 APP 程序中
        }
	}
	if(APP_TRUE == eraseDownloadDataFlag)
	{
		APP_LOG_debug("Erase Download Sector\r\n");
		APP_CONFIG_FirmwareInfo_pt info = &g_downloadFirmwareInfo;
		FlashEraseFlash(FLASH_DOWNLOAD_START_ADDR, FLASH_DOWNLOAD_SIZE);
		if(info->downloadLen >= FLASH_DOWNLOAD_SIZE)
		{
			FlashEraseFlash(FLASH_DOWNLOAD1_START_ADDR, FLASH_DOWNLOAD1_SIZE);
		}
		APP_CONFIG_DownloadFirmwareRecovery();
        eraseDownloadDataFlag = APP_FALSE;
	}
	return check;
}