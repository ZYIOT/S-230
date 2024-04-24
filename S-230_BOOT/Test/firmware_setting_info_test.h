
#include "board_config.h"

#ifndef __FIRMWARE_SETTING_INFO_TEST_H__
#define __FIRMWARE_SETTING_INFO_TEST_H__

#define DEBUG_SYSCONFIG_SET				(0)	// ����ϵͳ����

#define DEBUG_DOWNLOAD_CONTINUE_SET		(0)	// ���ü��� download ����
#define DEBUG_DOWNLOAD_FINISH_SET		(0)	// ������� download ����
#define DEBUG_EEPROM_RECOVERY_SET		(0)	// ���ûָ� firmware settings info ����

#if DEBUG_FIRMWARE_SETTING_INFO_TEST
void firmware_setting_info_test_send(void);
void firmware_setting_info_test_recv(void);
#endif

#endif

