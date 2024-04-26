


#ifndef __APP_UPDATE_FIRMWARE_H__
#define __APP_UPDATE_FIRMWARE_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "main.h"
#include "app_config/map.h"
#include "app_config/defines.h"


#define REBOOT_TRY_MAX_COUNT	(5)		// 尝试 reboot 的最大次数 
#define UPDATE_RETRY_MAX_CNT	(5)		// 尝试重发 update 帧的最大次数 
#define UPDATE_REWRITE_MAX_CNT	(3)		// 尝试重新烧写 flash 的最大次数 
#define UDDATE_FRAME_DATA_SIZE	(SIZE_128B)



typedef enum
{
	UPDATE_NO_ERROR = 0,		// 无错误								0
	UPDATE_ERROR_DEVBUSY,		// 设备暂忙，不能升级					1
	UPDATE_ERROR_FILETYPE,		// 文件类型错误(不识别升级的固件类型)	2
	UPDATE_ERROR_FILENAME,		// 文件名错误(命名不符合规范)			3
	UPDATE_ERROR_FILELEN,		// 文件长度错误(超长)					4
	UPDATE_ERROR_FILECRC,		// 文件CRC校验错误						5
	UPDATE_ERROR_STARTADDR,		// 文件的起始烧录地址错误				6
	UPDATE_ERROR_OFFSET,		// 文件烧录的偏移地址错误				7
	UPDATE_ERROR_FRAMENO,		// 帧号错误								8
	UPDATE_ERROR_FRAMELEN,		// 帧长度错误							9
	UPDATE_ERROR_FRAMECRC,		// 帧CRC校验错误						A
	UPDATE_ERROR_FLASHERASE,	// Flash 擦除失败						B
	UPDATE_ERROR_FLASHWRITE,	// Flash 烧录失败						C
	UPDATE_ERROR_FRAMECMD,		// 帧命令错误							D
	UPDATE_ERROR_RETRYFAIL,		// 重发3次仍然失败						E
	UPDATE_ERROR_ERASEPARA,		// 参数擦除失败							F
	UPDATE_ERROR_WRITEPARA,		// 参数写入失败							0x10
	UPDATE_ERROR_FAKEFRAME,		// 已处理过的 fake frame 数据			0x11
	UPDATE_ERROR_RESENDOK,		// 已处理过的 resend frame 数据			0x12
	UPDATE_ERROR_MALLOC,        // 申请内存失败							0x13
	UPDATE_ERROR_REBOOTCNT,		// BOOT重启多次失败						0x14
	UPDATE_ERROR_FILE_EXIST,	// 固件已存在，不用更新
	UPDATE_ERROR_COPYFACTORY,	// copy APP 到 Factory 失败				0x16
	UPDATE_ERROR_DOWNLOAD_DATA,	// Download 区数据错误
	UPDATE_ERROR_DOWNLOAD_UNFINISH,	// Download 尚未完成
	UPDATE_ERROR_NO_FIRMWARE,	// 没有可执行的固件
	
	UPDATE_ERROR_RESEND = 0xFE,	// 重发数据
	UPDATE_ERROR_END1 = 0xFF,	// 升级流程结束
}UPDATE_ERROR_TYPE_E;


typedef struct
{
	UPDATE_ERROR_TYPE_E type;	// 错误码
	uint8_t *string;			// 错误信息字符串指针
}error_type_string_t;


void APP_UPDATE_FIRMWARE_showErrorTypeContent(UPDATE_ERROR_TYPE_E type);
UPDATE_ERROR_TYPE_E APP_UPDATE_FIRMWARE_factory_check(void);
UPDATE_ERROR_TYPE_E APP_UPDATE_FIRMWARE_check_file(void);


#ifdef __cplusplus
}
#endif
#endif


