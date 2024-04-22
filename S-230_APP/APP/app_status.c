
#include "bsp_mcu.h"
#include "app_status.h"
#ifdef BOARD_HAS_RELAY
#include "app_can.h"
#endif
#include "app_network.h"
#include "app_log.h"


static uint8_t app_status_tick_cnt = 0;
extern int8_t relay_board_error;

// 指示主控板+继电器控制板正常运行 
static void _show_run_normal(uint8_t tick_cnt)
{
    if(5 == tick_cnt)
    {
        MCU_LED_ON();
    }
    else if(0 == tick_cnt)
    {
        MCU_LED_OFF();
    }
}

// 指示主控板正常运行，未检测到继电器控制板 
static void _show_run_error1(uint8_t tick_cnt)
{
    if(2 == tick_cnt % 3)
    {
        MCU_LED_ON();
    }
    else if(0 == tick_cnt % 3)
    {
        MCU_LED_OFF();
    }
}

static void _judge_board_connect(size_t run_tick)
{
    int rc = 0;
    #if 0
    // 15s内没有收到继电器控制板的数据，则判定为“继电器控制板故障” 
    if((HARDWARE_GET_TICK() - run_tick >= 15000) && IF_RELAY_BOARD_CHECK_UNSET())
    {
        SET_RELAY_BOARD_CHECK_ERROR();
    }
    rc = HARDWARE_TAKE_NOTIFY(100);  // 查询是否有"需要检查与控制板之间的连接"的消息 
    if(0 != rc) // 没有查询到通知而超时 
    {
        // 发送"link"指令 
        // rc = APP_CAN_send_dev_link();
        // if(APP_OK != rc)
        // {
        //     SET_RELAY_BOARD_CHECK_ERROR();
        // }
    }
    #else
    HARDWARE_OS_DELAY_MS(100);
    #endif
}

// 指示主控板/继电器控制板的运行状态 
static void _show_board_run_status(void)
{
#ifdef BOARD_HAS_RELAY
    if(IF_RELAY_BOARD_CHECK_ERROR())
    {
        _show_run_error1(app_status_tick_cnt);
        app_status_tick_cnt++;
        if(app_status_tick_cnt >= 15)
        {
            app_status_tick_cnt = 0;
        }
    }
    else
    {
        _show_run_normal(app_status_tick_cnt);
        app_status_tick_cnt++;
        if(app_status_tick_cnt >= 10)
        {
            app_status_tick_cnt = 0;
        }
    }
    if(0 == app_status_tick_cnt % 10)
    {
        APP_LOG_debug("device:%u csq:%d net:%d server:%d, relay:%d\r\n", 
                    APP_CONFIG_device_id(), 
                    app_network.csq, app_network.network_status, app_network.connected_status, 
                    GET_RELAY_BOARD_CHECK_RESULT());
    }
#else
    // app_status_tick_cnt++;
    // if(0 == app_status_tick_cnt % 30)
    // {
    //     APP_LOG_debug("device:%u csq:%d net:%d server:%d\r\n", 
    //                 APP_CONFIG_device_id(), 
    //                 app_network.csq, 
    //                 app_network.network_status, 
    //                 app_network.connected_status);
    // }
#endif

}


static void _feed_wdg(void)
{
    if(0 == app_status_tick_cnt % 10)
    {
        BSP_WDG_feed();
        // APP_LOG_show("feed\r\n");
    }
}

int APP_STATUS_init(void)
{
    return APP_OK;
}


void APP_STATUS_task_run(void *argument)
{
    size_t run_tick = 0;
#ifdef BOARD_HAS_RELAY
    INIT_REALY_BOARD_CHECK();
#endif    
    run_tick = HARDWARE_GET_TICK();
    for (;;)
    {
        _feed_wdg();
        _judge_board_connect(run_tick);
        _show_board_run_status();
        
        // HARDWARE_OS_DELAY_MS(100);
    }
}


