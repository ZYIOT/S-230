#include "bsp_lcd.h"
#include "app_lcd.h"
#include "app_sensors.h"
#include "app_network.h"
#include "app_log.h"
#include "app.h"

// 传感器数据顺序与LCD显示顺序的映射关系 
const uint8_t cDataItemMap[3] = {1, 0, 2};  // DO - item0, 温度 - item1,  pH - item2 

float LcdSensorData[3] = {0.00};    // 温度 溶氧 pH 
t_LcdSensorShow g_lcdSensorShow = 
{
    .sensorHasConfig = SENSOR_HAS_NO_CONFIG,
    .lcdSensorStatus = {SENSOR_INITING_STATE, SENSOR_INITING_STATE, SENSOR_INITING_STATE},
    .lcdSensorDataStatus = {SENSOR_DATA_UNCHANGE, SENSOR_DATA_UNCHANGE, SENSOR_DATA_UNCHANGE}
};
// static uint8_t ItemShowStatus[3] = {0, 0, 0};
static uint8_t lastRange = LCD_SIGNAL_RANGE_NON;

uint8_t gLcdLightOn = 0;

int AppLcdShowAllItem(void)
{
    uint8_t popIndex = 0;
    for(uint8_t i=LCD_DO_INDEX; i<LCD_MAX_ITEM_CNT; i++)
    {
        BspLcdShowItem(i, popIndex);
        popIndex += 1;
    }
    BspLcdShowSignal(LCD_SIGNAL_RANGE0);
    return APP_OK;
}

uint8_t getSignalRange(void)
{
    uint8_t range = LCD_SIGNAL_RANGE0;
    if(app_network.csq >= 28 && app_network.csq <= 31)
    {
        range = LCD_SIGNAL_RANGE3;
    }
    else if(app_network.csq >= 20 && app_network.csq <= 27)
    {
        range = LCD_SIGNAL_RANGE2;
    }
    else if(app_network.csq >= 1 && app_network.csq <= 19)
    {
        range = LCD_SIGNAL_RANGE1;
    }
    else
    {
        range = LCD_SIGNAL_RANGE0;
    }
    return range;
}

int AppLcdShowSignal(void)
{
    uint8_t range = getSignalRange();
    if(range != lastRange)
    {
        BspLcdShowSignal(range);
    }
    lastRange = range;
    return APP_OK;
}

#ifndef SUPPORT_IRED
int AppLcdLightControl(void)
{
    static size_t sLcdLightOnTick = 0;
    if(0 == sLcdLightOnTick)
    {
        sLcdLightOnTick = HARDWARE_GET_TICK();
    }
    if(1 == gLcdLightOn)
    {
        gLcdLightOn = 0;
        sLcdLightOnTick = HARDWARE_GET_TICK();
        BSP_LCD_LIGHT_ON();
    }
    else
    {
        if(HARDWARE_GET_TICK() - sLcdLightOnTick >= 30 * 1000)
        {
            BSP_LCD_LIGHT_OFF();
        }
    }
    return APP_OK;
}
#endif

// #define LCD_SENSOR_STATUS(i)    (g_lcdSensorShow.lcdSensorStatus[i])
// #define LCD_DATA_STATUS(i)      (g_lcdSensorShow.lcdSensorDataStatus[i])
#define LCD_SENSOR_STATUS(i)    (g_lcdSensorShow.lcdSensorStatus[cDataItemMap[i]])
#define LCD_DATA_STATUS(i)      (g_lcdSensorShow.lcdSensorDataStatus[cDataItemMap[i]])
#define LCD_DATA_VALUE(i)       (LcdSensorData[cDataItemMap[i]])
#define LAST_SENSOR_STATUS(i)   (lastSensorState[cDataItemMap[i]])
#define LAST_DATA_STATUS(i)     (lastDataState[cDataItemMap[i]])


int AppLcdRefresh(void)
{
    static int8_t lastSensorState[3] = {SENSOR_UNKNOW_STATE, SENSOR_UNKNOW_STATE, SENSOR_UNKNOW_STATE};
    static int8_t lastDataState[3] = {SENSOR_DATA_UNCHANGE, SENSOR_DATA_UNCHANGE, SENSOR_DATA_UNCHANGE};
    static int8_t hasConfig = -1;
    uint8_t popIndex = 0;
    uint8_t i = 0;
    
    BSP_LOG_debug("config: %d, sensor status: %d %d %d, data status: %d %d %d\r\n", 
                g_lcdSensorShow.sensorHasConfig, 
                LCD_SENSOR_STATUS(0), LCD_SENSOR_STATUS(1), LCD_SENSOR_STATUS(2), 
                LCD_DATA_STATUS(0), LCD_DATA_STATUS(1), LCD_DATA_STATUS(2));

    if(g_lcdSensorShow.sensorHasConfig >= SENSOR_EEPROM_CONFIG && g_lcdSensorShow.sensorHasConfig <= (SENSOR_EEPROM_CONFIG | SENSOR_DRIVER_CONFIG))     // 有配置or识别到传感器 
    {
        hasConfig = 1;
        for(i=LCD_DO_INDEX; i<LCD_MAX_ITEM_CNT; i++)
        {
            if(SENSOR_INITING_STATE == LCD_SENSOR_STATUS(i))
            {
                if(SENSOR_INITING_STATE != LAST_SENSOR_STATUS(i))
                {
                    LAST_SENSOR_STATUS(i) = SENSOR_INITING_STATE;
                    // 显示 "item:--"
                    BspLcdShowItem(i, popIndex);
                    BspLcdShowUnknowValue(i, popIndex);
                }
                popIndex += 1;
            }
            else if(SENSOR_NORMAL_STATE == LCD_SENSOR_STATUS(i))
            {
                LAST_SENSOR_STATUS(i) = SENSOR_NORMAL_STATE;
                if(SENSOR_DATA_CHANGED == LCD_DATA_STATUS(i))
                {
                    LCD_DATA_STATUS(i) = SENSOR_DATA_UNCHANGE;
                    LAST_DATA_STATUS(i) = SENSOR_DATA_UNCHANGE;
                    // 显示 "item:xx.xx" 
                    BspLcdShowItem(i, popIndex);
                    // BspLcdClearValue(popIndex);
                    BspLcdShowValue(LCD_DATA_VALUE(i), i, popIndex, 0);
                }
                else if(SENSOR_DATA_OVERFLOW == LCD_DATA_STATUS(i))
                {
                    LCD_DATA_STATUS(i) = SENSOR_DATA_UNCHANGE;
                    LAST_DATA_STATUS(i) = SENSOR_DATA_UNCHANGE;
                    // 显示 "item:xx.xx" 
                    BspLcdShowItem(i, popIndex);
                    // BspLcdClearValue(popIndex);
                    BspLcdShowValue(LCD_DATA_VALUE(i), i, popIndex, 1);
                }
                else if(SENSOR_DATA_NON == LCD_DATA_STATUS(i))
                {
                    if(SENSOR_DATA_NON != LAST_DATA_STATUS(i))
                    {
                        LAST_DATA_STATUS(i) = SENSOR_DATA_NON;
                        // 显示 "item:故障" 
                        BspLcdShowItem(i, popIndex);
                        BspLcdClearValue(popIndex);
                        BspLcdShowWarnning(i, popIndex);
                    }
                }
                popIndex += 1;
            }
        }
        if(popIndex < LCD_MAX_ITEM_CNT)
        {
            for(; popIndex < LCD_MAX_ITEM_CNT; popIndex++)
            {
                BspLcdClearItem(popIndex);
                BspLcdClearValue(popIndex);
            }
        }
    }
    else if(SENSOR_HAS_NO_CONFIG == g_lcdSensorShow.sensorHasConfig)   // 没有识别任何传感器 
    {
        if(SENSOR_HAS_NO_CONFIG != hasConfig)
        {
            hasConfig = SENSOR_HAS_NO_CONFIG;
            APP_LCD_SHOW_TITLE();
        }
    }

    return APP_OK;
}


// 网络监测任务
void APP_TFTLCD_task_run(void *argument)
{
    APP_LCD_INIT();
    APP_LCD_SHOW_HOME_PAGE();
    HARDWARE_OS_DELAY_MS(2000);
    APP_LCD_CLR_SCREEN();
    APP_LCD_SHOW_TOPIC();
    for (;;)
    {
        AppLcdRefresh();
#ifndef SUPPORT_IRED
        AppLcdLightControl();
#endif
        AppLcdShowSignal();
        HARDWARE_OS_DELAY_MS(1000);
    }
}


#ifdef SUPPORT_IRED
#if 0
int AppLcdIredCheck(void)
{
    static size_t sIredCheckTick = 0;
    if(0 == sIredCheckTick)
    {
        sIredCheckTick = HARDWARE_GET_TICK();
    }
    if(IF_IRED_CHECK_VALID())
    {
        HARDWARE_OS_DELAY_MS(100);
        if(IF_IRED_CHECK_VALID())
        {
            sIredCheckTick = HARDWARE_GET_TICK();
            BSP_LCD_LIGHT_ON();
            APP_LOG_debug("IRED toggle ON\r\n");
        }
    }
    else
    {
        if(HARDWARE_GET_TICK() - sIredCheckTick >= LCD_LIGHT_ON_TIMEOUT)
        {
            BSP_LCD_LIGHT_OFF();
            APP_LOG_debug("IRED toggle OFF\r\n");
        }
    }
    return APP_OK;
}
#else
int AppLcdIredCheck(void)
{
    static size_t sIredCheckTick = 0;
    static uint8_t sLightOnFlag = 1;
    if(0 == sIredCheckTick)
    {
        sIredCheckTick = HARDWARE_GET_TICK();
    }
    if(0 == sLightOnFlag)
    {
        if(IF_IRED_CHECK_VALID())
        {
            HARDWARE_OS_DELAY_MS(100);
            if(IF_IRED_CHECK_VALID())
            {
                BSP_LCD_LIGHT_ON();
                sLightOnFlag = 1;
                sIredCheckTick = HARDWARE_GET_TICK();
                APP_LOG_debug("IRED toggle ON\r\n");
            }
        }
    }
    else
    {
        if(HARDWARE_GET_TICK() - sIredCheckTick >= LCD_LIGHT_ON_TIMEOUT)
        {
            if(!IF_IRED_CHECK_VALID())
            {
                HARDWARE_OS_DELAY_MS(100);
                if(!IF_IRED_CHECK_VALID())
                {
                    BSP_LCD_LIGHT_OFF();
                    sLightOnFlag = 0;
                    APP_LOG_debug("IRED toggle OFF\r\n");
                }
            }
        }
    }
    return APP_OK;
}
#endif

void APP_IRED_task_run(void *argument)
{
    app_config_time_t time;
    uint8_t week = 0;
    static size_t time_tick = 0;
    for (;;)
    {
        AppLcdIredCheck();
        if (HARDWARE_GET_TICK() - time_tick >= 3000)
        {
            BSP_RTC_get(&time.year, &time.month, &time.day, &time.hour, &time.minute, &time.second, &week);
            APP_LOG_debug("now: 20%02d-%02d-%02d %02d:%02d:%02d\r\n", time.year, time.month, time.day, time.hour, time.minute, time.second);
            time_tick = HARDWARE_GET_TICK();
        }
        HARDWARE_OS_DELAY_MS(1000);
    }
}
#endif
