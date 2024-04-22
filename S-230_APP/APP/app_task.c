#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#include "board_config.h"
#include "app.h"
#include "bsp_usart.h"
#include "bsp_wdg.h"
#include "bsp_mcu.h"
#include "bsp_rtc.h"
#include "bsp_rng.h"
#include "bsp_link.h"
#include "app_task.h"
#include "app_network.h"
#include "app_protocol.h"
#include "app_sensors.h"
#include "app_pump.h"
#include "app_relay.h"
#include "app_sensors.h"
#include "hardware_wrapper.h"
#include "time.h"
#include "app_l6.h"
#include "app_update_firmware.h"
#include "app_gprs.h"
#include "app_status.h"
#ifdef SUPPORT_TFTLCD
#include "app_lcd.h"
#endif
#include "app_log.h"

#define __RUN_TASK(_name, _priority, _stack)                                                      \
    const osThreadAttr_t _name##_task_attributes = {                                              \
        .name = #_name "Task",                                                                    \
        .priority = (osPriority_t)_priority,                                                      \
        .stack_size = _stack};                                                                    \
    _##_name##_task_handle = osThreadNew(APP_##_name##_task_run, NULL, &_name##_task_attributes); \
    APP_LOG_debug(#_name " handle %x\r\n", _##_name##_task_handle);                               \
    HAL_Delay(10);


#define __DEF_TASK(_name) \
    osThreadId_t _##_name##_task_handle;

#define __GET_TASK(_name) \
	_##_name##_task_handle;

#define _DEL_TASK(_name)    \
    vTaskDelete(_##_name##_task_handle)

#define _PAUSE_TASK(_name)  \
    vTaskSuspend(_##_name##_task_handle)

// 网络监测任务
void APP_CHECK_NET_task_run(void *argument)
{
    for (;;)
    {
        if (APP_SERVER_CONNECTED())
        {
            // 从 eeprom 中读取 update_firmware_monitor 信息
            int rc = APP_UPDATE_FIRMWARE_sync_firmware_setting_back_to_monitor();
            APP_LOG_trace("firmware info load rc: %d\r\n", rc);
            vTaskDelete(osThreadGetId());
            HARDWARE_OS_DELAY_MS(100);
            // 创建继续升级任务，执行断网续传
            // APP_TASK_create_update_continue_task();
        }
        HARDWARE_OS_DELAY_MS(3000);
    }
    
}

__DEF_TASK(NETWORK)
__DEF_TASK(PROTOCOL)
#ifdef SUPPORT_PUMP
__DEF_TASK(PUMP)
#endif
__DEF_TASK(SENSORS)
__DEF_TASK(STATUS)
#ifdef BOARD_HAS_RELAY
// __DEF_TASK(RELAY)
#endif
__DEF_TASK(GPRS)
// __DEF_TASK(CAN_tx)
// __DEF_TASK(CAN_rx)
#ifdef SUPPORT_L6_ENABLE
__DEF_TASK(L6)
#endif
#ifdef SUPPORT_TFTLCD
__DEF_TASK(TFTLCD)
#endif

#ifdef SUPPORT_IRED
__DEF_TASK(IRED)
#endif

int APP_TASK_init(void)
{   
    /*
    【注意】：
    1、和 usart 使用的 steambuffer 一样都是从 ucHeap[configTOTAL_HEAP_SIZE] 中动态分配的空间。
    2、注意任务中使用 printf 的开销对任务堆栈空间的影响。
    3、分配任务空间后，至少应留有2K空余空间用于程序调度，否则可能会导致程序运行异常。
    4、注意 DMA 相关的 buffer 不要分配到 CCM 中。
    */
    osKernelInitialize();
    __RUN_TASK(PROTOCOL, osPriorityHigh, 0x1C00);
    __RUN_TASK(SENSORS, osPriorityHigh, 0x1C00);
    __RUN_TASK(NETWORK, osPriorityAboveNormal, 0x1C00);
    __RUN_TASK(STATUS, osPriorityHigh, 0x400);
#ifdef BOARD_HAS_RELAY
    // __RUN_TASK(RELAY, osPriorityNormal1, 0x1C00);
#endif
#ifdef SUPPORT_PUMP
    // __RUN_TASK(PUMP, osPriorityLow, 0x400);
#endif
#ifdef SUPPORT_L6_ENABLE
    __RUN_TASK(L6, osPriorityNormal, 0x400); 
#endif	
    __RUN_TASK(GPRS, osPriorityAboveNormal, 0x800);
#ifdef SUPPORT_TFTLCD
    __RUN_TASK(TFTLCD, osPriorityNormal2, 0x2000);
#endif    
#ifdef SUPPORT_IRED
    __RUN_TASK(IRED, osPriorityAboveNormal, 0x400);
#endif
    return APP_OK;
}

void APP_TASK_run(void)
{
    osKernelStart();
    while (1)
    {
    }
}

osThreadId_t APP_TASK_get_status_handle(void)
{
    return __GET_TASK(STATUS);
}

// timer2 的1s定时中断处理
void APP_TASK_run_per_second_ISR()
{
#ifdef SUPPORT_PUMP
    BSP_PUMP_check_frequency();
#endif
}

// 创建升级结束任务
void APP_TASK_create_update_finish_task(void)
{
	__DEF_TASK(UPDATE_FINISH)
	__RUN_TASK(UPDATE_FINISH, osPriorityLow, 0x800);
}

// 创建继续升级任务
void APP_TASK_create_update_continue_task(void)
{
	__DEF_TASK(UPDATE_CONTINUE)
	__RUN_TASK(UPDATE_CONTINUE, osPriorityNormal, 0x600);
}


// 创建网络监测任务
void APP_TASK_create_check_net_task(void)
{
    __DEF_TASK(CHECK_NET)
	__RUN_TASK(CHECK_NET, osPriorityNormal, 0x200);
}
