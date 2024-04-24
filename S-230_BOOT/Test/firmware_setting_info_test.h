
#include "board_config.h"

#ifndef __FIRMWARE_SETTING_INFO_TEST_H__
#define __FIRMWARE_SETTING_INFO_TEST_H__

#define DEBUG_SYSCONFIG_SET				(0)	// 设置系统参数

#define DEBUG_DOWNLOAD_CONTINUE_SET		(0)	// 设置继续 download 参数
#define DEBUG_DOWNLOAD_FINISH_SET		(0)	// 设置完成 download 参数
#define DEBUG_EEPROM_RECOVERY_SET		(0)	// 设置恢复 firmware settings info 参数

#if DEBUG_FIRMWARE_SETTING_INFO_TEST
void firmware_setting_info_test_send(void);
void firmware_setting_info_test_recv(void);
#endif

#endif

