
#include "bsp_mcu.h"
#include "app_status.h"
#ifdef BOARD_HAS_RELAY
#include "app_can.h"
#endif
#include "app_network.h"
#include "app_log.h"


static uint8_t s_appStatusTickCnt = 0;

// 指示主控板+继电器控制板正常运行 
static void ShowRunNormal(uint8_t tick_cnt)
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
static void ShowRunError(uint8_t tick_cnt)
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

static void JudgeBoardConnect(size_t run_tick)
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
static void ShowBoardRunStatus(void)
{
#ifdef BOARD_HAS_RELAY
    if(IF_RELAY_BOARD_CHECK_ERROR())
    {
        ShowRunError(s_appStatusTickCnt);
        s_appStatusTickCnt++;
        if(s_appStatusTickCnt >= 15)
        {
            s_appStatusTickCnt = 0;
        }
    }
    else
    {
        ShowRunNormal(s_appStatusTickCnt);
        s_appStatusTickCnt++;
        if(s_appStatusTickCnt >= 10)
        {
            s_appStatusTickCnt = 0;
        }
    }
    if(0 == s_appStatusTickCnt % 10)
    {
        APP_LOG_Debug("device:%u csq:%d net:%d server:%d, relay:%d\r\n", 
                    APP_CONFIG_DeviceID(), 
                    g_appNetwork.csq, g_appNetwork.networkStatus, g_appNetwork.connectedStatus, 
                    GET_RELAY_BOARD_CHECK_RESULT());
    }
#else
    s_appStatusTickCnt++;
    if(0 == s_appStatusTickCnt % 30)
    {
        APP_LOG_Debug("device:%u csq:%d net:%d server:%d\r\n", 
                    APP_CONFIG_DeviceID(), 
                    g_appNetwork.csq, 
                    g_appNetwork.networkStatus, 
                    g_appNetwork.connectedStatus);
    }
#endif

}


static void FeedWdg(void)
{
    if(0 == s_appStatusTickCnt % 10)
    {
        BSP_WDG_feed();
        // APP_LOG_Show("feed\r\n");
    }
}

int APP_STATUS_Init(void)
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
        FeedWdg();
        JudgeBoardConnect(run_tick);
        ShowBoardRunStatus();
        
        // HARDWARE_OS_DELAY_MS(100);
    }
}


