


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
	UPDATE_NO_ERROR = 0,		// 无错误 
	UPDATE_ERROR_FILETYPE,		// 文件类型错误(不识别升级的固件类型) 
	UPDATE_ERROR_BINTYPE,		// BIN类型错误(其他支持的固件文件) 
	UPDATE_ERROR_FILELEN,		// 文件长度错误(超长) 
	UPDATE_ERROR_FILECRC,		// 文件CRC校验错误 
	UPDATE_ERROR_OFFSET,		// 文件烧录的偏移地址错误 
	UPDATE_ERROR_FRAMELEN,		// 帧长度错误 
	UPDATE_ERROR_FLASHERASE,	// Flash 擦除失败 
	UPDATE_ERROR_FLASHWRITE,	// Flash 烧录失败 
	UPDATE_ERROR_RETRYFAIL,		// 重s发3次仍然失败 
	UPDATE_ERROR_DOWNLOAD_DATA,	// Download 区数据错误
	UPDATE_ERROR_DOWNLOAD_UNFINISH,	// Download 尚未完成
	UPDATE_ERROR_FILE_EXIST,	// 固件已存在，不用更新 
	UPDATE_ERROR_IAPFAIL,		// 升级失败  
	UPDATE_ERROR_NO_FIRMWARE,	// 没有可执行的固件
	
	UPDATE_ERROR_END1 = 0xFF,	// 升级流程结束
}UPDATE_ERROR_TYPE_E;


typedef struct
{
	UPDATE_ERROR_TYPE_E type;	// 错误码
	uint8_t *string;			// 错误信息字符串指针
}error_type_string_t;


typedef struct 
{
	uint8_t *pnKeyWords;
	UPDATE_ERROR_TYPE_E type;	// 错误码
}pn_check_result_t;


void APP_UPDATE_FIRMWARE_showErrorTypeContent(UPDATE_ERROR_TYPE_E type);
UPDATE_ERROR_TYPE_E APP_UPDATE_FIRMWARE_factory_check(void);
UPDATE_ERROR_TYPE_E APP_UPDATE_FIRMWARE_check_file(void);


#ifdef __cplusplus
}
#endif
#endif


