

#include "app_log.h"
#include "app_update_firmware.h"
#include "app_network.h"
#include "app_task.h"
#include "stm32_flash.h"
#include "bsp_protocol.h"
#include "bsp_link.h"
#include "bsp_mcu.h"
#include "bsp_wdg.h"

// macro 
#define SET_UPDATE_RESULT(_result)	(s_updateResult = _result)
#define GET_UPDATE_RESULT()			(s_updateResult)

#define SET_RESEND_CMD_ID(_cmd_id)	(s_resendCmdID = _cmd_id)
#define GET_RESEND_CMD_ID()			(s_resendCmdID)

// const 
const APP_CONFIG_FirmwareInfo_t c_factoryFirmwareInfo = 
{
	.downloadLen = 0,
	.updateResult = IAP_UPDATE_SUCESS, 
	.reserved[0] = 0,
	.reserved[1] = 0,
	.firmwareType = MCU_FACTORY_FIRMWARE, 
	.firmwareInfo = 
	{
		.deviceType = MASTER_CONTROL_DEV,
		.versionH = APP_FIRMWARE_MAJOR_DEFAULT,
		.versionM = APP_FIRMWARE_MINOR_DEFAULT,
		.versionL = APP_FIRMWARE_REVISION_DEFAULT,
		.fileLen = FLASH_DOWNLOAD_SIZE,
		.fileCrc = DEFAULT_CRC32_VALUE
	},
};
const uint32_t c_timeoutTickTable[] = {0, 3*1000, 5*1000, 30*1000, 60*1000, 120*1000, 0xFFFFFFFF};

// static 
static WaitTimeoutCtrl_t s_waitTimeoutCtrl = 
{
	.waitTimeoutEnable = 0,
	.updateRetryCnt = 0,
	.lastSendTick = 0
};

// 用于监控升级流程中的值
static UpdateFirmwareMonitor_t s_updateFirmwareMonitor = 
{
	.offset = 0,
	.readLen = 0,
	.updateError = UPDATE_NO_ERROR,
	.firmwareType = UNKONW_FIRMWARE, 
	.firmwareInfo = 
	{
		.fileLen = 0,
		.fileCrc = DEFAULT_CRC32_VALUE,
		.deviceType = UNKONW_DEV
	},
};

static FILE_TYPE_e s_fileType = UNKONW_FIRMWARE;
static uint8_t s_updateResult = UPDATE_FILE_PROCESS_RESULT_NOT_REQUIRED;
static uint16_t s_resendCmdID = 0x0000;

static void UpdateRecvAckTimerStart(void);
static void UpdateRecvAckTimerStop(void);

// extern 

// 判断固件是否下载完成 
static uint8_t IfDownloadFirmwareFinish(void)
{
	uint8_t ret = DOWNLOAD_CONTINUE;
	if(s_updateFirmwareMonitor.offset >= s_updateFirmwareMonitor.firmwareInfo.fileLen)
	{
		ret = DOWNLOAD_FINISH;
	}
	return ret;
}

/*****************************************************************************
 * 函 数 名  : check_update_firmware_info
 * 负 责 人  : Z&L
 * 创建日期  : 2021年10月26日
 * 函数功能  : 检查带升级的固件信息帧是否正确(检查固件 type、start_address、file_len、file_name 是否与固件对应)
 * 输入参数  : g2_server_firmware_info_message_pt pMsg  固件信息帧消息指针
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它    : 

*****************************************************************************/
static UPDATE_ERROR_TYPE_e CheckUpdateFirmwareInfo(g2_server_firmware_start_message_pt pMsg)
{
	UPDATE_ERROR_TYPE_e rc = UPDATE_NO_ERROR;
	FirmwareInfo_pt pFirmwareInfo = (FirmwareInfo_pt)&g_appFirmwareInfo.firmwareInfo;
	// 从 EEPROM 中读取 firmware setting info 数据
	APP_CONFIG_FirmwareInfoRead(APP_FIRMWARE_INFO, &g_appFirmwareInfo);
	switch(pMsg->deviceType)
	{
		case MASTER_CONTROL_DEV:
			if((pFirmwareInfo->fileCrc == pMsg->fileCrc)
			&& (pFirmwareInfo->versionH == pMsg->versionH)
			&& (pFirmwareInfo->versionM == pMsg->versionM)
			&& (pFirmwareInfo->versionL == pMsg->versionL))
			{
				rc = UPDATE_ERROR_FILE_EXIST;
			}
			else if(pMsg->fileLen > FLASH_APP_MAX_SIZE)
			{
				rc = UPDATE_ERROR_FILELEN;
			}
			else
			{
			}
			break;
		
		default :
			rc = UPDATE_ERROR_FILELEN;
			break;
	}
	
	return rc;
}


/*****************************************************************************
 * 函 数 名  : check_update_firmware_frame
 * 负 责 人  : Z&L
 * 创建日期  : 2021年10月26日
 * 函数功能  : 检查带升级的固件数据帧是否正确(检查本帧固件数据 offset_address、frame_no、frame_len、frame_crc 是否正确)
 * 输入参数  : g2_server_firmware_frame_message_pt pMsg  固件数据帧消息指针
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它    : 

*****************************************************************************/
static UPDATE_ERROR_TYPE_e CheckUpdateFirmwareFrame(g2_server_firmware_data_message_pt pMsg)
{
	UPDATE_ERROR_TYPE_e rc = UPDATE_NO_ERROR;
	
	if(pMsg->data_offset != s_updateFirmwareMonitor.offset)
	{
		rc = UPDATE_ERROR_OFFSET;
	}
	else if((pMsg->data_len > s_updateFirmwareMonitor.readLen)
		 && (s_updateFirmwareMonitor.readLen + s_updateFirmwareMonitor.offset != s_updateFirmwareMonitor.firmwareInfo.fileLen))
	{
		rc = UPDATE_ERROR_FRAMELEN;
	}
	else
	{		
	}

	return rc;
}

// 更新当前 update 固件信息到 eeprom 中
static UPDATE_ERROR_TYPE_e UpdateFirmwareSettingBack(uint8_t fileType)
{
	UPDATE_ERROR_TYPE_e rc = UPDATE_NO_ERROR;

	g_downloadFirmwareInfo.downloadLen = s_updateFirmwareMonitor.offset;
	g_downloadFirmwareInfo.updateResult = IAP_UPDATE_DOWNLOAD;
	g_downloadFirmwareInfo.reserved[0] = 0;
	g_downloadFirmwareInfo.reserved[1] = 0;
	g_downloadFirmwareInfo.firmwareType = fileType;
	memcpy((uint8_t *)&g_downloadFirmwareInfo.firmwareInfo, (uint8_t *)&s_updateFirmwareMonitor.firmwareInfo, sizeof(UsrFirmwareInfo_t));
	if(DOWNLOAD_FINISH == IfDownloadFirmwareFinish())
	{
		APP_CONFIG_FirmwareInfoWrite(DOWNLOAD_FIRMWARE_INFO, &g_downloadFirmwareInfo);
	}
	return rc;
}


// 校验固件 CRC32 值
static UPDATE_ERROR_TYPE_e CheckFirmwareCRC32(void)
{
	UPDATE_ERROR_TYPE_e rc = UPDATE_NO_ERROR;
	uint32_t tempCrc = 0xFFFF;
	uint32_t len = s_updateFirmwareMonitor.firmwareInfo.fileLen;
	uint32_t crc = s_updateFirmwareMonitor.firmwareInfo.fileCrc;
	
	if(len <= FLASH_DOWNLOAD_SIZE)
	{
		tempCrc = crc32((uint8_t *)FLASH_DOWNLOAD_START_ADDR, len);
	}
	else
	{
		tempCrc = crc32((uint8_t *)FLASH_DOWNLOAD_START_ADDR, FLASH_DOWNLOAD_SIZE);
		tempCrc = add_crc32((uint8_t *)FLASH_DOWNLOAD1_START_ADDR, len - FLASH_DOWNLOAD_SIZE, tempCrc ^ 0xFFFFFFFF);
	}
	if(crc != tempCrc)
	{
		rc = UPDATE_ERROR_FILECRC;
	}
	return rc;
}


/*****************************************************************************
 * 函 数 名  : erase_firmware_flash
 * 负 责 人  : Z&L
 * 创建日期  : 2021年10月26日
 * 函数功能  : 擦除固件升级所在区域的 flash 的扇区
 * 输入参数  : uint32_t start_address  待擦除的起始地址
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它    : 

*****************************************************************************/
static UPDATE_ERROR_TYPE_e EraseFlashFirmware(uint32_t size)
{
	UPDATE_ERROR_TYPE_e rc = UPDATE_NO_ERROR;
	BSP_WDG_feed();
	if(0 != stm32_flashErase(FLASH_DOWNLOAD_START_ADDR, FLASH_DOWNLOAD_SIZE))
	{
		rc = UPDATE_ERROR_FLASHERASE;			
	}
	if(size > FLASH_DOWNLOAD_SIZE)
	{
		BSP_WDG_feed();
		if(0 != stm32_flashErase(FLASH_DOWNLOAD1_START_ADDR, FLASH_DOWNLOAD1_SIZE))
		{
			rc = UPDATE_ERROR_FLASHERASE;			
		}
	}
	BSP_WDG_feed();
	return rc;
}


/*****************************************************************************
 * 函 数 名  : write_flash_firmware
 * 负 责 人  : Z&L
 * 创建日期  : 2021年10月26日
 * 函数功能  : 擦除固件升级所在区域的 flash 的扇区
 * 输入参数  : uint32_t writeAddr  待写入的起始地址
 			   uint16_t size	   待写入的数据大小(字节)
 			   uint8_t *dataBuf	   待写入的数据缓冲区指针
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它    : 

*****************************************************************************/
// 先按简单的情况处理(暂时忽略跨 sector 烧写的情况)
static UPDATE_ERROR_TYPE_e WriteFlashFirmware(uint8_t *databuf)
{
	UPDATE_ERROR_TYPE_e rc = UPDATE_NO_ERROR;
	uint32_t writeAddr = 0;
	uint16_t size = 0;
	uint32_t *pdata = (uint32_t *)databuf;
	// 限定 FLASH_DOWNLOAD_SIZE 为 readLen 的整数倍
	if(s_updateFirmwareMonitor.offset < FLASH_DOWNLOAD_SIZE)
	{
		writeAddr = s_updateFirmwareMonitor.offset + FLASH_DOWNLOAD_START_ADDR;
	}
	else
	{
		writeAddr = s_updateFirmwareMonitor.offset - FLASH_DOWNLOAD_SIZE + FLASH_DOWNLOAD1_START_ADDR;
	}
	size = s_updateFirmwareMonitor.readLen;
	if(0 != stm32_flashWrite_Dire(writeAddr, pdata, size))
	{
		rc = UPDATE_ERROR_FLASHWRITE;
	}
	
	return rc;
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
	else
	{
		return UPDATE_NO_ERROR;
	}
}

// 后续若丰富了设备的固件类型，则需要修改该函数 
static FILE_TYPE_e GetFirmwareType(uint8_t deviceType)
{
	FILE_TYPE_e firmwareType = UNKONW_FIRMWARE;
	switch(deviceType)
	{
		case RELAY_CONTROL_DEV:
			firmwareType = RELAY_APP_FIRMWARE;
			break;
		case MASTER_CONTROL_DEV:
			firmwareType = MCU_APP_FIRMWARE;
			break;
	}
	return firmwareType;
}

// 初始化 s_updateFirmwareMonitor 的值
static void InitUpdateFirmwareMonitor(g2_server_firmware_start_message_pt pMsg)
{
	s_updateFirmwareMonitor.firmwareInfo.deviceType = pMsg->deviceType;
	s_updateFirmwareMonitor.firmwareType = GetFirmwareType(pMsg->deviceType);
	s_updateFirmwareMonitor.firmwareInfo.versionH = pMsg->versionH;
	s_updateFirmwareMonitor.firmwareInfo.versionM = pMsg->versionM;
	s_updateFirmwareMonitor.firmwareInfo.versionL = pMsg->versionL;
	s_updateFirmwareMonitor.firmwareInfo.fileLen = pMsg->fileLen;
	s_updateFirmwareMonitor.firmwareInfo.fileCrc = pMsg->fileCrc;
	s_updateFirmwareMonitor.offset = 0x0;
	s_updateFirmwareMonitor.readLen = UDDATE_FRAME_DATA_SIZE;
	s_updateFirmwareMonitor.updateError = UPDATE_NO_ERROR;
}

// 更新 s_updateFirmwareMonitor 的值
static void UpdateUpdateFirmwareMonitor(uint16_t dataLen)
{
	s_updateFirmwareMonitor.offset += dataLen;
	if(s_updateFirmwareMonitor.offset + UDDATE_FRAME_DATA_SIZE > s_updateFirmwareMonitor.firmwareInfo.fileLen)
	{
		s_updateFirmwareMonitor.readLen = s_updateFirmwareMonitor.firmwareInfo.fileLen - s_updateFirmwareMonitor.offset;
	}
	else
	{
		s_updateFirmwareMonitor.readLen = UDDATE_FRAME_DATA_SIZE;
	}
}

// 向服务器发送请求 update 固件数据帧
static int SendNextFirmwareDataAskMessage(g2_server_packet_pt packet)
{
	int rc = APP_OK;
	g2_server_firmware_data_ask_t askMsg = {0};
	
	askMsg.read_offset = s_updateFirmwareMonitor.offset;
	askMsg.read_datalen = s_updateFirmwareMonitor.readLen;
	BSP_PROTOCOL_send_write_firmware_data_ask_message(packet, &askMsg);
	SET_RESEND_CMD_ID(packet->cmd_id);
	UpdateRecvAckTimerStart();
	
	return rc;
}

// 重启请求 update 固件数据流程
static int RestartFirmwareDataAskMessage(g2_server_packet_pt packet)
{
	int rc = APP_OK;
	uint32_t fileLen = 0;

	fileLen = s_updateFirmwareMonitor.firmwareInfo.fileLen;	
	EraseFlashFirmware(fileLen);
	s_updateFirmwareMonitor.offset = 0x0;
	s_updateFirmwareMonitor.readLen = UDDATE_FRAME_DATA_SIZE;
	s_updateFirmwareMonitor.updateError = UPDATE_NO_ERROR;
	SendNextFirmwareDataAskMessage(packet);

	return rc;
}

// 向服务器发送 download 结束帧
static int SendFirmwareDownloadEndMessage(g2_server_packet_pt packet, uint8_t result)
{
	int rc = APP_OK;
	g2_server_firmware_update_ack_t askMsg = {0};
	
	askMsg.file_process_stage = UPDATE_FILE_PROCESS_STAGE_DOWNLOAD;
	if(UPDATE_NO_ERROR == result)
	{
		askMsg.file_process_result = UPDATE_FILE_PROCESS_RESULT_SUCESS;
	}
	else if(UPDATE_ERROR_FILE_EXIST == result)
	{
		askMsg.file_process_result = UPDATE_FILE_PROCESS_RESULT_NOT_REQUIRED;
	}
	else
	{
		askMsg.file_process_result = UPDATE_FILE_PROCESS_RESULT_FAIL;
	}
	rc = BSP_PROTOCOL_send_write_firmware_update_ack_message(packet, &askMsg);
	
	return rc;
}

// 启动超时重发定时器
static void UpdateRecvAckTimerStart(void)
{
	s_waitTimeoutCtrl.lastSendTick = HARDWARE_GET_TICK();
	s_waitTimeoutCtrl.waitTimeoutEnable = 1;
	s_waitTimeoutCtrl.updateRetryCnt++;
}

// 关闭超时重发定时器
static void UpdateRecvAckTimerStop(void)
{	
	s_waitTimeoutCtrl.waitTimeoutEnable = 0;
	s_waitTimeoutCtrl.updateRetryCnt = 0;
	s_waitTimeoutCtrl.lastSendTick = HARDWARE_GET_TICK();
}

static FILE_TYPE_e SelectUpdateFirmwareType(void)
{
	char *fileName = (char *)(FLASH_DOWNLOAD_START_ADDR + 0x400);
	if(NULL != strstr(fileName, PN_CODE))
	{
		if(NULL != strstr(fileName, FILE_BOOT_SUBSTR))
		{
			APP_LOG_Debug("s_fileType:MCU_BOOT_FIRMWARE\r\n");
			return MCU_BOOT_FIRMWARE;
		}
		else if(NULL != strstr(fileName, FILE_APP_SUBSTR))
		{
			APP_LOG_Debug("s_fileType:MCU_APP_FIRMWARE\r\n");
			return MCU_APP_FIRMWARE;
		}
		else if(NULL != strstr(fileName, FILE_CONFIG_SUBSTR))
		{
			APP_LOG_Debug("s_fileType:MCU_CFG_FIRMWARE\r\n");
			return MCU_CFG_FIRMWARE;
		}
		else if(NULL != strstr(fileName, FILE_FACTORY_SUBSTR))
		{
			APP_LOG_Debug("s_fileType:MCU_FACTORY_FIRMWARE\r\n");
			return MCU_FACTORY_FIRMWARE;
		}
	}
	else if(NULL != strstr(fileName, FILE_GPRS_SUBSTR))
	{
		APP_LOG_Debug("s_fileType:GPRS_MDL_FIRMWARE\r\n");
		return GPRS_MDL_FIRMWARE;
	}
	else if(NULL != strstr(fileName, FILE_RELAY_SUBSTR))
	{
		APP_LOG_Debug("s_fileType:RELAY_APP_FIRMWARE\r\n");
		return RELAY_APP_FIRMWARE;
	}
	APP_LOG_Error("s_fileType:UNKONW_FIRMWARE\r\n");
	return UNKONW_FIRMWARE;
}

// 将 download 区数据拷贝到 bootloader 区
static int UpdateBootloaderFirmware(void)
{
	uint32_t *src = (uint32_t *)FLASH_DOWNLOAD_START_ADDR;
	uint32_t des = FLASH_BOOTLOADER_START_ADDR;
	uint32_t len = g_downloadFirmwareInfo.downloadLen;
	int rc = stm32_flashWrite(des, src, len);
	if(0 == rc)
	{
		if(g_downloadFirmwareInfo.firmwareInfo.fileCrc == crc32((uint8_t *)des, len))
		{
			APP_LOG_Debug("***** Update bootloader sucess *****\r\n");
			return UPDATE_NO_ERROR;
		}
		else
		{
			rc = stm32_flashWrite(des, src, len);
			if(0 == rc)
			{
				if(g_downloadFirmwareInfo.firmwareInfo.fileCrc == crc32((uint8_t *)des, len))
				{
					APP_LOG_Debug("***** Update bootloader sucess *****\r\n");
					return UPDATE_NO_ERROR;
				}
			}
			APP_LOG_Error("***** Update bootloader error <UPDATE_ERROR_FILECRC> *****\r\n");
			return UPDATE_ERROR_FILECRC;
		}
	}
	else
	{
		rc = stm32_flashWrite(des, src, len);
		if(0 == rc)
		{
			if(g_downloadFirmwareInfo.firmwareInfo.fileCrc == crc32((uint8_t *)des, len))
			{
				APP_LOG_Debug("***** Update bootloader sucess *****\r\n");
				return UPDATE_NO_ERROR;
			}
			else
			{			
				APP_LOG_Error("***** Update bootloader error <UPDATE_ERROR_FILECRC> *****\r\n");
				return UPDATE_ERROR_FILECRC;
			}
		}
		APP_LOG_Error("***** Update bootloader error <UPDATE_ERROR_FLASHWRITE> *****\r\n");
		return UPDATE_ERROR_FLASHWRITE;
	}
}

// 获取设备请求服务器的升级数据长度
uint32_t G2_SERVER_GetRequestDataLength(void)
{
	return (uint32_t)s_updateFirmwareMonitor.readLen;
}



// 超时重发监测和处理函数
int APP_UPDATE_FIRMWARE_ResendDataAsk(void)
{
	int rc = APP_OK;
	if ((1 == s_waitTimeoutCtrl.waitTimeoutEnable) 
	&& (HARDWARE_GET_TICK() - s_waitTimeoutCtrl.lastSendTick) > c_timeoutTickTable[s_waitTimeoutCtrl.updateRetryCnt])
    {
		if(APP_SERVER_CONNECTED())	// 处于连上服务器的状态
		{
			g2_server_packet_t resendPacket = {0};
			g2_server_firmware_data_ask_t askMsg = {0};
			G2_SERVER_PACKET_init(&resendPacket);
			resendPacket.cmd_id = GET_RESEND_CMD_ID();
			if(UPDATE_RETRY_MAX_CNT <= s_waitTimeoutCtrl.updateRetryCnt)	// 重发次数达到 UPDATE_RETRY_MAX_CNT 次
			{	// 发送升级结束命令
				SendFirmwareDownloadEndMessage(&resendPacket, UPDATE_ERROR_RETRYFAIL);
				UpdateRecvAckTimerStop();
			}
			else	// 执行重发操作
			{				
				askMsg.read_offset = s_updateFirmwareMonitor.offset;
				askMsg.read_datalen = s_updateFirmwareMonitor.readLen;
				rc = BSP_PROTOCOL_send_write_firmware_data_ask_message(&resendPacket, &askMsg);
				SET_RESEND_CMD_ID(resendPacket.cmd_id);
				UpdateRecvAckTimerStart();
			}
		}
	}

	return rc;
}

int APP_UPDATE_FIRMWAER_StartFinishTask(uint8_t result)
{
	APP_CONFIG_FirmwareInfo_pt info = &g_downloadFirmwareInfo;
	if((UPDATE_NO_ERROR == result) || (UPDATE_ERROR_FILE_EXIST == result) || (UPDATE_ERROR_BINTYPE == result))
	{
		if(UPDATE_NO_ERROR == result)
		{
			SET_UPDATE_RESULT(UPDATE_FILE_PROCESS_RESULT_SUCESS);
		}
		else if(UPDATE_ERROR_FILE_EXIST == result)
		{
			SET_UPDATE_RESULT(UPDATE_FILE_PROCESS_RESULT_NOT_REQUIRED);
		}
		else if(UPDATE_ERROR_BINTYPE == result)
		{
			SET_UPDATE_RESULT(UPDATE_FILE_PROCESS_RESULT_FAIL);
		}
#if 0		
		stm32_flashErase(FLASH_DOWNLOAD_START_ADDR, FLASH_DOWNLOAD_SIZE);
		if(info->downloadLen > FLASH_DOWNLOAD_SIZE)
		{
			stm32_flashErase(FLASH_DOWNLOAD1_START_ADDR, FLASH_DOWNLOAD1_SIZE);
		}
		APP_CONFIG_DownloadFirmwareInfoRecovery();
#endif		
	}
	else
	{
		SET_UPDATE_RESULT(UPDATE_FILE_PROCESS_RESULT_FAIL);
	}
	// APP_LOG_Debug("HEAP:0x%08X\r\n", xPortGetFreeHeapSize());
	// 创建任务，主动发送烧录完成帧, 发送完成后主动删除任务
	APP_TASK_CreateUpdateFinishTask();
	// APP_LOG_Debug("HEAP:0x%08X\r\n", xPortGetFreeHeapSize());
	return APP_OK;
}

int APP_UPDATE_FIRMWARE_CheckFirmwareInfo(void)
{
	APP_CONFIG_FirmwareInfo_pt appInfo = &g_appFirmwareInfo;
	APP_CONFIG_FirmwareInfo_pt dwnInfo = &g_downloadFirmwareInfo;
	// 检查 APP 信息 
	if((MASTER_CONTROL_DEV == appInfo->firmwareInfo.deviceType) 
		&& (MCU_APP_FIRMWARE == appInfo->firmwareType))
	{
		if(IAP_UPDATE_UNKNOW == appInfo->updateResult) 
		{
			if((APP_FIRMWARE_MAJOR == appInfo->firmwareInfo.versionH) 
			&& (APP_FIRMWARE_MINOR == appInfo->firmwareInfo.versionM) 
			&& (APP_FIRMWARE_REVISION == appInfo->firmwareInfo.versionL))
			{
				uint32_t fileLen = appInfo->firmwareInfo.fileLen;
				uint32_t fileCrc = appInfo->firmwareInfo.fileCrc;
				if(DEFAULT_CRC32_VALUE != fileCrc)
				{
					if(fileCrc != crc32((uint8_t *)FLASH_APP_START_ADDR, fileLen))
					{
						APP_CONFIG_AppFirmwareInfoRecovery();
					}
				}
			}
		}
		else if(IAP_UPDATE_FAIL == appInfo->updateResult)
		{
			SET_UPDATE_RESULT(UPDATE_FILE_PROCESS_RESULT_FAIL);
			APP_CONFIG_AppFirmwareInfoClearResult();
			APP_TASK_CreateUpdateFinishTask();
		}
		else if(IAP_UPDATE_SUCESS == appInfo->updateResult)
		{
			SET_UPDATE_RESULT(UPDATE_FILE_PROCESS_RESULT_SUCESS);
			APP_CONFIG_AppFirmwareInfoClearResult();
			APP_TASK_CreateUpdateFinishTask();
		}
	}
	else
	{
		APP_CONFIG_AppFirmwareInfoRecovery();
	}
	// 检查 DOWNLOAD 信息 
	if(IAP_UPDATE_DOWNLOAD == dwnInfo->updateResult)
	{
		if((MASTER_CONTROL_DEV == dwnInfo->firmwareInfo.deviceType) && (MCU_APP_FIRMWARE == dwnInfo->firmwareType))
		{
			APP_CONFIG_DownloadFirmwareInfoRecovery();
		}
		else
		{
			// 启动 IAP 任务，升级其他设备的固件 
			// IAP 完成后，启动 IAP_FINISH 任务上报操作结果，并设定 DOWNLOAD 信息为初始值 
		}
	}
	return APP_OK;
}
// 应答 update 起始帧，向服务器请求 update 的固件数据
int G2_SERVER_WriteFirmwareStartMessageProcess(g2_server_packet_pt packet)
{	
	UPDATE_ERROR_TYPE_e rc = UPDATE_NO_ERROR;
	g2_server_firmware_start_message_pt pMsg = (g2_server_firmware_start_message_pt)packet->parsed;
	UpdateRecvAckTimerStop();
	// 判定是否需要升级
	rc = CheckUpdateFirmwareInfo(pMsg);	
	InitUpdateFirmwareMonitor(pMsg);
	if(UPDATE_NO_ERROR != rc)
	{
		// 发送不需要升级的应答
		SendFirmwareDownloadEndMessage(packet, rc);
	}
	else
	{
		// 擦除指定Flash区域 
		rc = EraseFlashFirmware(pMsg->fileLen);
		if(UPDATE_NO_ERROR != rc)
		{
			// 应答升级失败
			SendFirmwareDownloadEndMessage(packet, rc);
		}
		else
		{	
			// 发送请求 update 的固件数据帧
			APP_CONFIG_DownloadFirmwareInfoRecovery();
			SendNextFirmwareDataAskMessage(packet);
		}
	}
	return PROTOCOL_OK;
}

// 处理服务器应答的固件数据，并向服务器请求下一帧
int G2_SERVER_WriteFirmwareDataMessageProcess(g2_server_packet_pt packet)
{
	UPDATE_ERROR_TYPE_e rc = UPDATE_NO_ERROR;
	g2_server_firmware_data_message_pt pMsg = (g2_server_firmware_data_message_pt)packet->parsed;
	static uint8_t recnt = UPDATE_REWRITE_MAX_CNT;

	// 校验本帧数据内容
	rc = CheckUpdateFirmwareFrame(pMsg);
	s_updateFirmwareMonitor.updateError = rc;
	UpdateRecvAckTimerStop();

	if(UPDATE_NO_ERROR == rc)
	{	// 写入 flash
		rc = WriteFlashFirmware(pMsg->data);
		if(UPDATE_NO_ERROR != rc)
		{// download 失败，则重新 erase flash 并且重置读取的 offset 和 len
			if(recnt > 0)
			{
				RestartFirmwareDataAskMessage(packet);
				recnt--;
			}
			else
			{
				SendFirmwareDownloadEndMessage(packet, rc);
			}
		}
		else
		{	// 更新 monitor 的下次读取的偏移和长度
			UpdateUpdateFirmwareMonitor(pMsg->data_len);
			if(DOWNLOAD_FINISH == IfDownloadFirmwareFinish())
			{	// 校验整个 firmware 的 CRC
				rc = CheckFirmwareCRC32();
				if(UPDATE_NO_ERROR != rc)
				{
					APP_LOG_Debug("CRC32 check error!!!\r\n");
					APP_CONFIG_DownloadFirmwareInfoRecovery();
					EraseFlashFirmware(s_updateFirmwareMonitor.firmwareInfo.fileLen);
				}
				else
				{
					// 更新写入的位置(到EEPROM)
					UpdateFirmwareSettingBack(SelectUpdateFirmwareType());
				}
				// 发送 download 完成帧
				SendFirmwareDownloadEndMessage(packet, rc);
				if(UPDATE_NO_ERROR == rc)
				{
					switch(s_updateFirmwareMonitor.firmwareInfo.deviceType)
					{
						case MASTER_CONTROL_DEV:
							s_fileType = SelectUpdateFirmwareType();						
							if(MCU_APP_FIRMWARE == s_fileType)
							{
								HARDWARE_OS_DELAY_MS(3000);
								BSP_MCU_reset();
							}
							else if(MCU_BOOT_FIRMWARE == s_fileType)
							{
								rc = UpdateBootloaderFirmware();
								if(UPDATE_NO_ERROR == rc)
								{
									APP_UPDATE_FIRMWAER_StartFinishTask(UPDATE_NO_ERROR);
								}
								else
								{
									APP_UPDATE_FIRMWAER_StartFinishTask(UPDATE_ERROR_BINTYPE);
								}
							}
							else
							{
								APP_UPDATE_FIRMWAER_StartFinishTask(UPDATE_ERROR_BINTYPE);
							}
							break;
						default:
							break;
					}
				}
			}
			else
			{	// 请求下一帧
				SendNextFirmwareDataAskMessage(packet);
			}
		}
	}
	else
	{
		SendFirmwareDownloadEndMessage(packet, rc);
	}
	return PROTOCOL_OK;
}


// update 完成任务，用于主动发送 update finish 帧
void APP_UPDATE_FINISH_task_run(void *argument)
{
	g2_server_packet_pt pUpdateFinishPacket = NULL;
	for(;;)
	{
		if(APP_SERVER_CONNECTED())	// 处于连上服务器的状态
		{
			HARDWARE_OS_DELAY_MS(1000);
			pUpdateFinishPacket = malloc(sizeof(g2_server_packet_t));
			if(NULL != pUpdateFinishPacket)
			{
				G2_SERVER_PACKET_init(pUpdateFinishPacket);
				g2_server_firmware_update_ack_t askMsg = {0};
				askMsg.file_process_stage = UPDATE_FILE_PROCESS_STAGE_FLUSH;
				askMsg.file_process_result = GET_UPDATE_RESULT();
				BSP_PROTOCOL_send_write_firmware_update_ack_message(pUpdateFinishPacket, &askMsg);
				HARDWARE_OS_DELAY_MS(3000);
				// 主动上传一次软件版本信息
				G2_SERVER_read_version_message_process(pUpdateFinishPacket);
				HARDWARE_OS_DELAY_MS(100);
				free(pUpdateFinishPacket);

				vTaskDelete(osThreadGetId());
			}
		}
		HARDWARE_OS_DELAY_MS(1000);
	}	
}

// update 继续任务，用于主动发送 请求update data 帧
void APP_UPDATE_CONTINUE_task_run(void *argument)
{
	// g2_server_packet_pt update_continue_packet = NULL;
	// update_continue_packet = malloc(sizeof(g2_server_packet_t));
	// g2_server_firmware_update_ack_t askMsg = {0};
	// uint16_t data_len = g_downloadFirmwareInfo.downloadLen;

	// memcpy((uint8_t *)&s_updateFirmwareMonitor.firmwareInfo, (uint8_t *)&g_downloadFirmwareInfo.firmwareInfo, sizeof(usr_firmware_info_t));
	// s_updateFirmwareMonitor.updateError = UPDATE_NO_ERROR;
	// UpdateUpdateFirmwareMonitor(data_len);
	
	// G2_SERVER_PACKET_init(update_continue_packet);
	// // 请求下一帧
	// g2_server_firmware_data_ask_t askMsg = {0};
	// askMsg.read_offset = s_updateFirmwareMonitor.offset;
	// askMsg.read_datalen = s_updateFirmwareMonitor.readLen;
	// BSP_PROTOCOL_send_write_firmware_data_ask_message(update_continue_packet, &askMsg);
			
	HARDWARE_OS_DELAY_MS(100);
	// free(update_continue_packet);
	vTaskDelete(osThreadGetId());
	HARDWARE_OS_DELAY_MS(100);

}

// 与模组通讯任务
void APP_UPDATE_COM_MODULE_task_run(void *argument)
{
	// 初始化任务，并启动更新通讯模块程序流程。
	// 升级完成后，删除任务自身
	HARDWARE_OS_DELAY_MS(100);
	vTaskDelete(osThreadGetId());
	HARDWARE_OS_DELAY_MS(100);
}


