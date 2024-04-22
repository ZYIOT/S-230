

#include "app_log.h"
#include "app_update_firmware.h"
#include "app_network.h"
#include "app_task.h"
#include "stm32_flash.h"
#include "bsp_protocol.h"
#include "bsp_link.h"
#include "bsp_mcu.h"
#include "bsp_wdg.h"


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

		

const uint32_t timeout_tick_table[] = {0, 3*1000, 5*1000, 30*1000, 60*1000, 120*1000, 0xFFFFFFFF};
static wait_timeout_ctrl_t wait_timeout_ctrl = 
{
	.wait_timeout_enable = 0,
	.update_retry_cnt = 0,
	.last_send_tick = 0
};

// 用于监控升级流程中的值
update_firmware_monitor_t update_firmware_monitor = 
{
	.offset = 0,
	.read_len = 0,
	.update_error = UPDATE_NO_ERROR,
	.firmware_info = 
	{
		.file_len = 0,
		.file_crc = DEFAULT_CRC32_VALUE,
		.device_type = UNKONW_DEV
	},
};

static void _update_recvack_timer_start(void);
static void _update_recvack_timer_stop(void);


static uint8_t update_result = UPDATE_FILE_PROCESS_RESULT_NOT_REQUIRED;
#define SET_UPDATE_RESULT(_result)	(update_result = _result)
#define GET_UPDATE_RESULT()			(update_result)
static uint16_t resend_cmd_id = 0x0000;
#define SET_RESEND_CMD_ID(_cmd_id)	(resend_cmd_id = _cmd_id)
#define GET_RESEND_CMD_ID()			(resend_cmd_id)


static uint8_t _if_download_firmware_finish(void)
{
	uint8_t ret = DOWNLOAD_CONTINUE;

	if(update_firmware_monitor.offset >= update_firmware_monitor.firmware_info.file_len)
	{
		ret = DOWNLOAD_FINISH;
	}
	else
	{
	}
	
	return ret;
}

/*****************************************************************************
 * 函 数 名  : check_update_firmware_info
 * 负 责 人  : Z&L
 * 创建日期  : 2021年10月26日
 * 函数功能  : 检查带升级的固件信息帧是否正确(检查固件 type、start_address、file_len、file_name 是否与固件对应)
 * 输入参数  : g2_server_firmware_info_message_pt pmsg  固件信息帧消息指针
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它    : 

*****************************************************************************/
static UPDATE_ERROR_TYPE_E _check_update_firmware_info(g2_server_firmware_start_message_pt pmsg)
{
	UPDATE_ERROR_TYPE_E rc = UPDATE_NO_ERROR;
	app_config_firmware_info_pt pfirmware_info = (app_config_firmware_info_pt)&firmware_setting_info1.firmware_info;
	// 从 EEPROM 中读取 firmware setting info 数据
	APP_CONFIG_firmware_setting_info_read(APP_CONFIG_FIRMWARE_SETTING_INFO, &firmware_setting_info1);
	
	switch(pmsg->device_type)
	{
		case MASTER_CONTROL_DEV:
			if((pfirmware_info->file_crc == pmsg->file_crc)
			&& (pfirmware_info->version_h == pmsg->version_h)
			&& (pfirmware_info->version_m == pmsg->version_m)
			&& (pfirmware_info->version_l == pmsg->version_l))
			{
				rc = UPDATE_ERROR_FILE_EXIST;
			}
			else if(pmsg->file_len > FLASH_APP_MAX_SIZE)
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
 * 输入参数  : g2_server_firmware_frame_message_pt pmsg  固件数据帧消息指针
 * 输出参数  : 无
 * 返 回 值  : 
 * 调用关系  : 
 * 其    它    : 

*****************************************************************************/
UPDATE_ERROR_TYPE_E _check_update_firmware_frame(g2_server_firmware_data_message_pt pmsg)
{
	UPDATE_ERROR_TYPE_E rc = UPDATE_NO_ERROR;
	
	if(pmsg->data_offset != update_firmware_monitor.offset)
	{
		rc = UPDATE_ERROR_OFFSET;
	}
	else if((pmsg->data_len > update_firmware_monitor.read_len)
		 && (update_firmware_monitor.read_len + update_firmware_monitor.offset != update_firmware_monitor.firmware_info.file_len))
	{
		rc = UPDATE_ERROR_FRAMELEN;
	}
	else
	{		
	}

	return rc;
}

// 更新当前 update 固件信息到 eeprom 中
static UPDATE_ERROR_TYPE_E _update_firmware_setting_back(void)
{
	UPDATE_ERROR_TYPE_E rc = UPDATE_NO_ERROR;

	firmware_setting_info2.download_len = update_firmware_monitor.offset;
	firmware_setting_info2.reserved[0] = 0;
	firmware_setting_info2.reserved[1] = 0;
	firmware_setting_info2.reserved[2] = 0;
	firmware_setting_info2.reserved[3] = 0;
	memcpy((uint8_t *)&firmware_setting_info2.firmware_info, (uint8_t *)&update_firmware_monitor.firmware_info, sizeof(usr_firmware_info_t));
	if(DOWNLOAD_FINISH == _if_download_firmware_finish())
	{
		APP_CONFIG_firmware_setting_info_write(APP_CONFIG_FIRMWARE_SETTING_INFO_BACK, &firmware_setting_info2);
	}
	return rc;
}


// 校验固件 CRC32 值
static UPDATE_ERROR_TYPE_E _check_firmware_CRC32(void)
{
	UPDATE_ERROR_TYPE_E rc = UPDATE_NO_ERROR;
	uint32_t tempcrc = 0xFFFF;
	uint32_t len = update_firmware_monitor.firmware_info.file_len;
	uint32_t crc = update_firmware_monitor.firmware_info.file_crc;
	
	if(len <= FLASH_DOWNLOAD_SIZE)
	{
		tempcrc = crc32((uint8_t *)FLASH_DOWNLOAD_START_ADDR, len);
	}
	else
	{
		tempcrc = crc32((uint8_t *)FLASH_DOWNLOAD_START_ADDR, FLASH_DOWNLOAD_SIZE);
		tempcrc = add_crc32((uint8_t *)FLASH_DOWNLOAD1_START_ADDR, len - FLASH_DOWNLOAD_SIZE, tempcrc^0xFFFFFFFF);
	}
	if(crc != tempcrc)
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
static UPDATE_ERROR_TYPE_E _erase_firmware_flash(uint32_t size)
{
	UPDATE_ERROR_TYPE_E rc = UPDATE_NO_ERROR;

	if(0 != stm32_flashErase(FLASH_DOWNLOAD_START_ADDR, FLASH_DOWNLOAD_SIZE))
	{
		rc = UPDATE_ERROR_FLASHERASE;			
	}
	if(size > FLASH_DOWNLOAD_SIZE)
	{
		if(0 != stm32_flashErase(FLASH_DOWNLOAD1_START_ADDR, FLASH_DOWNLOAD1_SIZE))
		{
			rc = UPDATE_ERROR_FLASHERASE;			
		}
	}
	
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
static UPDATE_ERROR_TYPE_E _write_flash_firmware(uint8_t *databuf)
{
	UPDATE_ERROR_TYPE_E rc = UPDATE_NO_ERROR;
	uint32_t write_addr = 0;
	uint16_t size = 0;
	uint32_t *pdata = (uint32_t *)databuf;
	// 限定 FLASH_DOWNLOAD_SIZE 为 read_len 的整数倍
	if(update_firmware_monitor.offset < FLASH_DOWNLOAD_SIZE)
	{
		write_addr = update_firmware_monitor.offset + FLASH_DOWNLOAD_START_ADDR;
	}
	else
	{
		write_addr = update_firmware_monitor.offset - FLASH_DOWNLOAD_SIZE + FLASH_DOWNLOAD1_START_ADDR;
	}
	size = update_firmware_monitor.read_len;
	if(0 != stm32_flashWrite_Dire(write_addr, pdata, size))
	{
		rc = UPDATE_ERROR_FLASHWRITE;
	}
	
	return rc;
}

// 初始化 update_firmware_monitor 的值
static void _init_update_firmware_monitor(g2_server_firmware_start_message_pt pmsg)
{
	update_firmware_monitor.firmware_info.device_type = pmsg->device_type;
	update_firmware_monitor.firmware_info.version_h = pmsg->version_h;
	update_firmware_monitor.firmware_info.version_m = pmsg->version_m;
	update_firmware_monitor.firmware_info.version_l = pmsg->version_l;
	update_firmware_monitor.firmware_info.file_len = pmsg->file_len;
	update_firmware_monitor.firmware_info.file_crc = pmsg->file_crc;
	update_firmware_monitor.offset = 0x0;
	update_firmware_monitor.read_len = UDDATE_FRAME_DATA_SIZE;
	update_firmware_monitor.update_error = UPDATE_NO_ERROR;
}

// 更新 update_firmware_monitor 的值
static void _update_update_firmware_monitor(uint16_t data_len)
{
	update_firmware_monitor.offset += data_len;
	if(update_firmware_monitor.offset + UDDATE_FRAME_DATA_SIZE > update_firmware_monitor.firmware_info.file_len)
	{
		update_firmware_monitor.read_len = update_firmware_monitor.firmware_info.file_len - update_firmware_monitor.offset;
	}
	else
	{
		update_firmware_monitor.read_len = UDDATE_FRAME_DATA_SIZE;
	}
}

// 向服务器发送请求 update 固件数据帧
static int _send_next_firmware_data_ask_message(g2_server_packet_pt packet)
{
	int rc = APP_OK;
	g2_server_firmware_data_ask_t ask_pmsg = {0};
	
	ask_pmsg.read_offset = update_firmware_monitor.offset;
	ask_pmsg.read_datalen = update_firmware_monitor.read_len;
	BSP_PROTOCOL_send_write_firmware_data_ask_message(packet, &ask_pmsg);
	SET_RESEND_CMD_ID(packet->cmd_id);
	_update_recvack_timer_start();
	
	return rc;
}

// 重启请求 update 固件数据流程
static int _restart_firmware_data_ask_message(g2_server_packet_pt packet)
{
	int rc = APP_OK;
	uint32_t file_len = 0;

	file_len = update_firmware_monitor.firmware_info.file_len;	
	_erase_firmware_flash(file_len);
	update_firmware_monitor.offset = 0x0;
	update_firmware_monitor.read_len = UDDATE_FRAME_DATA_SIZE;
	update_firmware_monitor.update_error = UPDATE_NO_ERROR;
	_send_next_firmware_data_ask_message(packet);

	return rc;
}

// 向服务器发送 download 结束帧
static int _send_firmware_download_end_message(g2_server_packet_pt packet, uint8_t result)
{
	int rc = APP_OK;
	g2_server_firmware_update_ack_t ack_pmsg = {0};
	
	ack_pmsg.file_process_stage = UPDATE_FILE_PROCESS_STAGE_DOWNLOAD;
	if(UPDATE_NO_ERROR == result)
	{
		ack_pmsg.file_process_result = UPDATE_FILE_PROCESS_RESULT_SUCESS;
	}
	else if(UPDATE_ERROR_FILE_EXIST == result)
	{
		ack_pmsg.file_process_result = UPDATE_FILE_PROCESS_RESULT_NOT_REQUIRED;
	}
	else
	{
		ack_pmsg.file_process_result = UPDATE_FILE_PROCESS_RESULT_FAIL;
	}
	rc = BSP_PROTOCOL_send_write_firmware_update_ack_message(packet, &ack_pmsg);
	
	return rc;
}

// 启动超时重发定时器
static void _update_recvack_timer_start(void)
{
	wait_timeout_ctrl.last_send_tick = HARDWARE_GET_TICK();
	wait_timeout_ctrl.wait_timeout_enable = 1;
	wait_timeout_ctrl.update_retry_cnt++;
}

// 关闭超时重发定时器
static void _update_recvack_timer_stop(void)
{	
	wait_timeout_ctrl.wait_timeout_enable = 0;
	wait_timeout_ctrl.update_retry_cnt = 0;
	wait_timeout_ctrl.last_send_tick = HARDWARE_GET_TICK();
}

// 将 download 区数据拷贝到 bootloader 区
static int _update_bootloader_firmware(void)
{
	uint32_t *src = (uint32_t *)FLASH_DOWNLOAD_START_ADDR;
	uint32_t des = FLASH_BOOTLOADER_START_ADDR;
	uint32_t len = firmware_setting_info2.download_len;
	int rc = stm32_flashWrite(des, src, len);
	if(0 == rc)
	{
		if(firmware_setting_info2.firmware_info.file_crc == crc32((uint8_t *)des, len))
		{
			APP_LOG_debug("***** Update bootloader sucess *****\r\n");
			return UPDATE_NO_ERROR;
		}
		else
		{
			APP_LOG_debug("***** Update bootloader error <UPDATE_ERROR_FILECRC> *****\r\n");
			return UPDATE_ERROR_FILECRC;
		}
	}
	else
	{
		APP_LOG_debug("***** Update bootloader error <UPDATE_ERROR_FLASHWRITE> *****\r\n");
		return UPDATE_ERROR_FLASHWRITE;
	}
}

// 获取设备请求服务器的升级数据长度
uint32_t G2_SERVER_get_request_data_length(void)
{
	return (uint32_t)update_firmware_monitor.read_len;
}


// 使用 eerpom 中的 update 固件信息同步 update_firmware_monitor
UPDATE_ERROR_TYPE_E APP_UPDATE_FIRMWARE_sync_firmware_setting_back_to_monitor(void) 
{
	UPDATE_ERROR_TYPE_E rc = UPDATE_NO_ERROR;

	rc = APP_CONFIG_firmware_setting_info_read(APP_CONFIG_FIRMWARE_SETTING_INFO_BACK, &firmware_setting_info2);
	APP_CHECK_RC(rc)
	uint32_t file_len = firmware_setting_info2.firmware_info.file_len;
	uint32_t download_len = firmware_setting_info2.download_len;
	update_firmware_monitor.offset = firmware_setting_info2.download_len;
	if(file_len - download_len > UDDATE_FRAME_DATA_SIZE)
	{
		update_firmware_monitor.read_len = UDDATE_FRAME_DATA_SIZE;
	}
	else
	{
		update_firmware_monitor.read_len = (file_len - download_len);
	}
	update_firmware_monitor.update_error = UPDATE_NO_ERROR;
	memcpy((uint8_t *)&update_firmware_monitor.firmware_info, (uint8_t *)&firmware_setting_info2.firmware_info, sizeof(usr_firmware_info_t));

	return rc;
}

// 超时重发监测和处理函数
int APP_UPDATE_FIRMWARE_resend_data_ask(void)
{
	int rc = APP_OK;
	if ((1 == wait_timeout_ctrl.wait_timeout_enable) 
	&& (HARDWARE_GET_TICK() - wait_timeout_ctrl.last_send_tick) > timeout_tick_table[wait_timeout_ctrl.update_retry_cnt])
    {
		if(APP_SERVER_CONNECTED())	// 处于连上服务器的状态
		{
			g2_server_packet_t resend_packet = {0};
			g2_server_firmware_data_ask_t ask_pmsg = {0};
			G2_SERVER_PACKET_init(&resend_packet);
			resend_packet.cmd_id = GET_RESEND_CMD_ID();
			if(UPDATE_RETRY_MAX_CNT <= wait_timeout_ctrl.update_retry_cnt)	// 重发次数达到 UPDATE_RETRY_MAX_CNT 次
			{	// 发送升级结束命令
				_send_firmware_download_end_message(&resend_packet, UPDATE_ERROR_RETRYFAIL);
				_update_recvack_timer_stop();
			}
			else	// 执行重发操作
			{
				
				ask_pmsg.read_offset = update_firmware_monitor.offset;
				ask_pmsg.read_datalen = update_firmware_monitor.read_len;
				rc = BSP_PROTOCOL_send_write_firmware_data_ask_message(&resend_packet, &ask_pmsg);
				SET_RESEND_CMD_ID(resend_packet.cmd_id);
				_update_recvack_timer_start();
			}
		}
	}

	return rc;
}

// 通过比较 2个 firmware_setting_info 来判断是否需要完成后续的通讯升级流程
int APP_UPDATE_FIRMWARE_compare_app_firmware_setting_info(void)
{
	app_config_firmware_setting_info_pt pinfo1 = &firmware_setting_info1;
	app_config_firmware_setting_info_pt pinfo2 = &firmware_setting_info2;

	// 避免由于IAP升级后重新刷固件导致EEPROM中的信息与运行的APP不对应的情况

	if(((APP_FIRMWARE_MAJOR 		!= pinfo1->firmware_info.version_h)
	 || (APP_FIRMWARE_MINOR 		!= pinfo1->firmware_info.version_m)
	 || (APP_FIRMWARE_REVISION 		!= pinfo1->firmware_info.version_l))
	&& ((APP_FIRMWARE_MAJOR_DEFAULT 	!= pinfo1->firmware_info.version_h)	// 也与出厂默认值不对应
	 || (APP_FIRMWARE_MINOR_DEFAULT    	!= pinfo1->firmware_info.version_m)
	 || (APP_FIRMWARE_REVISION_DEFAULT 	!= pinfo1->firmware_info.version_l)))
	{
		APP_CONFIG_firmware_setting_info1_recovery();
		return APP_OK;
	}

	if(1 == pinfo1->reserved[0])
	{
		SET_UPDATE_RESULT(UPDATE_FILE_PROCESS_RESULT_FAIL);
		APP_CONFIG_firmware_setting_info1_clear_result();
		APP_TASK_create_update_finish_task();
	}
	else
	{
		if(pinfo1->download_len == pinfo1->firmware_info.file_len)
		{
			if(0 == memcmp((uint8_t *)pinfo1, 
							(uint8_t *)pinfo2, 
							sizeof(app_config_firmware_setting_info_t)))
			{
				SET_UPDATE_RESULT(UPDATE_FILE_PROCESS_RESULT_SUCESS);
				APP_CONFIG_firmware_setting_info2_recovery();
				// 创建任务，主动发送烧录完成帧, 发送完成后主动删除任务
				APP_TASK_create_update_finish_task();
			}
			else
			{	// 出厂时设置的默认值，无更新下载
				SET_UPDATE_RESULT(UPDATE_FILE_PROCESS_RESULT_NOT_REQUIRED);
			}
		}
		else
		{
			if(0 == memcmp((uint8_t *)&factory_firmware_settting_info, 
		 					(uint8_t *)pinfo2, 
						 	sizeof(app_config_firmware_setting_info_t)))
			{	// 出厂时设置的默认值，无更新下载
				SET_UPDATE_RESULT(UPDATE_FILE_PROCESS_RESULT_NOT_REQUIRED);
			}
			else
			{
				if(pinfo2->download_len != pinfo2->firmware_info.file_len)
				{	// 固件数据下载未完成
					SET_UPDATE_RESULT(UPDATE_FILE_PROCESS_RESULT_NOT_REQUIRED);
				}
				else
				{
				}
			}
		}
	}

	return APP_OK;
}

// 应答 update 起始帧，向服务器请求 update 的固件数据
int G2_SERVER_write_firmware_start_message_process(g2_server_packet_pt packet)
{	
	UPDATE_ERROR_TYPE_E rc = UPDATE_NO_ERROR;
	g2_server_firmware_start_message_pt pmsg = (g2_server_firmware_start_message_pt)packet->parsed;
	_update_recvack_timer_stop();
	// 判定是否需要升级
	rc = _check_update_firmware_info(pmsg);	
	_init_update_firmware_monitor(pmsg);
	if(UPDATE_NO_ERROR != rc)
	{
		// 发送不需要升级的应答
		_send_firmware_download_end_message(packet, rc);
	}
	else
	{
		// 擦除指定Flash区域
		BSP_WDG_feed();
		rc = _erase_firmware_flash(pmsg->file_len);
		BSP_WDG_feed();
        APP_LOG_debug("feed\r\n");
		if(UPDATE_NO_ERROR != rc)
		{
			// 应答升级失败
			_send_firmware_download_end_message(packet, rc);
		}
		else
		{	
			// 发送请求 update 的固件数据帧
			APP_CONFIG_firmware_setting_info2_recovery();
			_send_next_firmware_data_ask_message(packet);
		}
	}
	
	return PROTOCOL_OK;
}

// 处理服务器应答的固件数据，并向服务器请求下一帧
int G2_SERVER_write_firmware_data_message_process(g2_server_packet_pt packet)
{
	UPDATE_ERROR_TYPE_E rc = UPDATE_NO_ERROR;
	g2_server_firmware_data_message_pt pmsg = (g2_server_firmware_data_message_pt)packet->parsed;
	static uint8_t recnt = UPDATE_REWRITE_MAX_CNT;

	// 校验本帧数据内容
	rc = _check_update_firmware_frame(pmsg);
	update_firmware_monitor.update_error = rc;
	_update_recvack_timer_stop();

	if(UPDATE_NO_ERROR == rc)
	{	// 写入 flash
		rc = _write_flash_firmware(pmsg->data);
		if(UPDATE_NO_ERROR != rc)
		{// download 失败，则重新 erase flash 并且重置读取的 offset 和 len
			if(recnt > 0)
			{
				_restart_firmware_data_ask_message(packet);
				recnt--;
			}
			else
			{
				_send_firmware_download_end_message(packet, rc);
			}
		}
		else
		{	// 更新 monitor 的下次读取的偏移和长度
			_update_update_firmware_monitor(pmsg->data_len);
			if(DOWNLOAD_FINISH == _if_download_firmware_finish())
			{	// 校验整个 firmware 的 CRC
				rc = _check_firmware_CRC32();
				if(UPDATE_NO_ERROR != rc)
				{
					APP_LOG_debug("CRC32 check error!!!\r\n");
					APP_CONFIG_firmware_setting_info2_recovery();
				}
				else
				{
					// 更新写入的位置(到EEPROM)
					_update_firmware_setting_back();
				}
				// 发送 download 完成帧
				_send_firmware_download_end_message(packet, rc);
				if(UPDATE_NO_ERROR == rc)
				{
					switch(update_firmware_monitor.firmware_info.device_type)
					{
						case MASTER_CONTROL_DEV:
						#if 0
							rc = _update_bootloader_firmware();
							if(UPDATE_NO_ERROR == rc)
							{
								SET_UPDATE_RESULT(UPDATE_FILE_PROCESS_RESULT_SUCESS);
								APP_CONFIG_firmware_setting_info2_recovery();
								// 创建任务，主动发送烧录完成帧, 发送完成后主动删除任务
								APP_TASK_create_update_finish_task();								
							}
						#else
							HARDWARE_OS_DELAY_MS(3000);
							BSP_MCU_reset();
						#endif
							break;
						default:
							break;
					}
				}
			}
			else
			{	// 请求下一帧
				_send_next_firmware_data_ask_message(packet);
			}
		}
	}
	else
	{
		_send_firmware_download_end_message(packet, rc);
	}
	
	return PROTOCOL_OK;
}


// update 完成任务，用于主动发送 update finish 帧
void APP_UPDATE_FINISH_task_run(void *argument)
{
	g2_server_packet_pt update_finish_packet = NULL;
	for(;;)
	{
		if(APP_SERVER_CONNECTED())	// 处于连上服务器的状态
		{
			HARDWARE_OS_DELAY_MS(1000);
			update_finish_packet = malloc(sizeof(g2_server_packet_t));
			if(NULL != update_finish_packet)
			{
				G2_SERVER_PACKET_init(update_finish_packet);
				g2_server_firmware_update_ack_t ack_pmsg = {0};
				ack_pmsg.file_process_stage = UPDATE_FILE_PROCESS_STAGE_FLUSH;
				ack_pmsg.file_process_result = GET_UPDATE_RESULT();
				BSP_PROTOCOL_send_write_firmware_update_ack_message(update_finish_packet, &ack_pmsg);
				HARDWARE_OS_DELAY_MS(3000);
				// 主动上传一次软件版本信息
				G2_SERVER_read_version_message_process(update_finish_packet);

				HARDWARE_OS_DELAY_MS(100);
				free(update_finish_packet);

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
	// g2_server_firmware_update_ack_t ack_pmsg = {0};
	// uint16_t data_len = firmware_setting_info2.download_len;

	// memcpy((uint8_t *)&update_firmware_monitor.firmware_info, (uint8_t *)&firmware_setting_info2.firmware_info, sizeof(usr_firmware_info_t));
	// update_firmware_monitor.update_error = UPDATE_NO_ERROR;
	// _update_update_firmware_monitor(data_len);
	
	// G2_SERVER_PACKET_init(update_continue_packet);
	// // 请求下一帧
	// g2_server_firmware_data_ask_t ask_pmsg = {0};
	// ask_pmsg.read_offset = update_firmware_monitor.offset;
	// ask_pmsg.read_datalen = update_firmware_monitor.read_len;
	// BSP_PROTOCOL_send_write_firmware_data_ask_message(update_continue_packet, &ask_pmsg);
			
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


