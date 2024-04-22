


#ifndef __APP_UPDATE_FIRMWARE_H__
#define __APP_UPDATE_FIRMWARE_H__


#include "g2_server_common.h"
#include "g2_server_packet.h"
#include "g2_server_message.h"



// FLASH 中各个空间分布（需要与 BOOT 程序中的设置同步）
/*
____________________________________
| firwmare type | size |  address  |
____________________________________
| Bootloader    | 32K  | 0x8000000 |
____________________________________
| Download1		| 32K  | 0x8008000 |
____________________________________
| App			| 160K | 0x8010000 |
 ---------------------------------
| Reserved      | 32K  | 0x8038000 |
____________________________________
| Download      | 128K | 0x8040000 |
____________________________________
| Factory		| 128K | 0x8060000 |
____________________________________
 固件下载的位置 包含 Download 和 Download1 共 2 个区，其中 Download 存储前 128K 内容，Download1 存储后 32K 内容。
 运行的 App 总共占用 64K + 128K 空间，其中最后 32K 空间保留不使用。
 出厂时先烧录 App 固件，然后再烧录 Bootloader 固件。
 Bootloader 固件在首次运行时，将 App 固件拷贝到 Factory 区中。
 App 固件在 APP 程序中下载到 Download 区后，再通过 BOOT 程序拷贝到 App 区。
 Bootloader 固件在 APP 程序中下载到 Download 区后，再直接拷贝到 Bootloader 区。
 当连续多次通过 BOOT 程序重启 APP 程序仍然失败后，由 BOOT 程序将 Factory 区中的数据直接拷贝到 App 区中，然后再运行 APP 程序。
*/
#define FLASH_START_ADDR					(0x8000000)
#define FLASH_BOOT_MAX_SIZE					(0x8000)	// 32K
#define FLASH_BOOTLOADER_START_ADDR			(FLASH_START_ADDR)		                                // 0x8000000
#define FLASH_BOOTLOADER_SIZE				FLASH_BOOT_MAX_SIZE
#define FLASH_DOWNLOAD1_MAX_SIZE			(0x8000)	// 32K
#define FLASH_DOWNLOAD1_START_ADDR			(FLASH_BOOTLOADER_START_ADDR + FLASH_BOOTLOADER_SIZE)	// 0x8008000
#define FLASH_DOWNLOAD1_SIZE				FLASH_DOWNLOAD1_MAX_SIZE	
#define FLASH_APP_MAX_SIZE					(0x30000)	// 192K
#define FLASH_APP_START_ADDR				(FLASH_DOWNLOAD1_START_ADDR + FLASH_DOWNLOAD1_SIZE)	    // 0x8010000
#define FLASH_APP_SIZE						(0x28000)	// 160K
#define FLASH_DOWNLOAD_START_ADDR			(FLASH_APP_START_ADDR + FLASH_APP_MAX_SIZE)				// 0X8040000
#define FLASH_DOWNLOAD_SIZE					(0x20000)	// 128K
#define FLASH_FACTORY_START_ADDR			(FLASH_DOWNLOAD_START_ADDR + FLASH_DOWNLOAD_SIZE)		// 0x8060000
#define FLASH_FACTORY_SIZE					(0x20000)	// 128K
#define FLASH_END_ADDR						(0x807FFFF)



#define REBOOT_TRY_MAX_COUNT	(5)		// 尝试 reboot 的最大次数
#define UPDATE_RETRY_MAX_CNT	(5)		// 尝试重发 update 帧的最大次数
#define UPDATE_REWRITE_MAX_CNT	(3)		// 尝试重新烧写 flash 的最大次数
#define UDDATE_FRAME_DATA_SIZE	(SIZE_128B)

typedef enum
{
	UNKONW_DEV = 0x0,			// 未知设备
	MASTER_CONTROL_DEV = 0x1,	// 主控设备
	COM_MODULE_DEV = 0x2,		// 通信模块设备
	DO_SENSOR_DEV = 0x11,		// 溶氧传感器设备
	PH_SENSOR_DEV = 0x21,		// pH传感器设备
}DEVICE_TYPE_E;

typedef enum
{
	MCU_BOOT_FIRMWARE = 0,		// BOOT 固件
	MCU_APP_FIRMWARE,			// APP 固件
	MCU_PARAMENT_FIRMWARE,		// PARAMENT 固件
    MCU_FACTORY_FIRMWARE,		// Factory 固件
    GPRS_MDL_FIRMWARE,			// GPRS 模块固件
	UNKONW_FIRMWARE = 255,		// 未知固件类型
}FILE_TYPE_E;


typedef enum
{
	UPDATE_NO_ERROR = 0,		// 无错误									0
	UPDATE_ERROR_DEVBUSY,		// 设备暂忙，不能升级						1
	UPDATE_ERROR_FILETYPE,		// 文件类型错误(不识别升级的固件类型)		2
	UPDATE_ERROR_FILENAME,		// 文件名错误(命名不符合规范)				3
	UPDATE_ERROR_FILELEN,		// 文件长度错误(超长)						4
	UPDATE_ERROR_FILECRC,		// 文件CRC校验错误							5
	UPDATE_ERROR_STARTADDR,		// 文件的起始烧录地址错误					6
	UPDATE_ERROR_OFFSET,		// 文件烧录的偏移地址错误					7
	UPDATE_ERROR_FRAMENO,		// 帧号错误									8
	UPDATE_ERROR_FRAMELEN,		// 帧长度错误								9
	UPDATE_ERROR_FRAMECRC,		// 帧CRC校验错误							A
	UPDATE_ERROR_FLASHERASE,	// Flash 擦除失败							B
	UPDATE_ERROR_FLASHWRITE,	// Flash 烧录失败							C
	UPDATE_ERROR_FRAMECMD,		// 帧命令错误								D
	UPDATE_ERROR_RETRYFAIL,		// 重发n次仍然失败							E
	UPDATE_ERROR_ERASEPARA,		// 参数擦除失败								F
	UPDATE_ERROR_WRITEPARA,		// 参数写入失败								0x10
	UPDATE_ERROR_FAKEFRAME,		// 已处理过的 fake frame 数据				0x11
	UPDATE_ERROR_RESENDOK,		// 已处理过的 resend frame 数据				0x12
	UPDATE_ERROR_MALLOC,        // 申请内存失败                             0x13
	UPDATE_ERROR_REBOOTCNT,		// BOOT重启多次失败							0x14
	UPDATE_ERROR_FILE_EXIST,	// 固件已存在，不用更新						0x15
	UPDATE_ERROR_COPYFACTORY,	// copy APP 到 Factory 失败					0x16
	
	UPDATE_ERROR_RESEND = 0xFE,	// 重发数据
	UPDATE_ERROR_END1 = 0xFF,	// 升级流程结束
	
}UPDATE_ERROR_TYPE_E;



enum
{
	UPDATE_FILE_PROCESS_STAGE_DOWNLOAD = 1,	// 下载阶段
	UPDATE_FILE_PROCESS_STAGE_FLUSH = 2,	// 烧录阶段
};

enum
{
	UPDATE_FILE_PROCESS_RESULT_SUCESS = 1,	// 成功
	UPDATE_FILE_PROCESS_RESULT_FAIL,		// 失败
	UPDATE_FILE_PROCESS_RESULT_NOT_REQUIRED,// 不需要升级(包括无更新、下载未完成)
};

enum
{
	DOWNLOAD_CONTINUE = 0,		// 烧录未完成
	DOWNLOAD_FINISH,			// 烧录完成
};

typedef app_config_firmware_info_t	(usr_firmware_info_t);
typedef app_config_firmware_info_pt	(usr_firmware_info_pt);


typedef struct
{	
	uint32_t offset;			// 下次烧录起始偏移地址
	uint16_t read_len;			// 下次要读取的数据长度
	uint8_t  update_error;		// 升级过程中的错误类型
	uint8_t  reserved;
	usr_firmware_info_t firmware_info;
}update_firmware_monitor_t, *update_firmware_monitor_pt;


typedef struct
{
	uint8_t wait_timeout_enable;
	uint8_t update_retry_cnt;
	uint32_t last_send_tick;
}wait_timeout_ctrl_t;



uint32_t G2_SERVER_get_request_data_length(void);
int APP_UPDATE_FIRMWARE_resend_data_ask(void);


int APP_UPDATE_FIRMWARE_compare_app_firmware_setting_info(void);
int G2_SERVER_write_firmware_start_message_process(g2_server_packet_pt packet);
int G2_SERVER_write_firmware_data_message_process(g2_server_packet_pt packet);
UPDATE_ERROR_TYPE_E APP_UPDATE_FIRMWARE_sync_firmware_setting_back_to_monitor(void);


void APP_UPDATE_FINISH_task_run(void *argument);
void APP_UPDATE_CONTINUE_task_run(void *argument);
void APP_UPDATE_COM_MODULE_task_run(void *argument);

#endif


