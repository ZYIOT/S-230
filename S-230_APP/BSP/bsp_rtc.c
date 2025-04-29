#include "app.h"
#include "bsp_rtc.h"
#include "bsp_log.h"
#include "bsp_wdg.h"
#include "utils.h"

extern int EEPROM_ReadBlock(uint16_t page, uint8_t *buf);
extern int EEPROM_WriteBlock(uint16_t page, uint8_t *buf);
#define EEPROM_BLOCK_SIZE 32
#define RTC_TIME_PAGE 31

int BSP_RTC_load_from_backup(void)
{
    datetime_t time = {0};
    uint8_t buf[EEPROM_BLOCK_SIZE] = {0};
    int rc = EEPROM_ReadBlock(RTC_TIME_PAGE, buf);
    APP_CHECK_RC(rc)
    if (buf[0] != 0xa5 || buf[1] != 0x5a || buf[2] < 20)
    {
        return APP_ERROR;
    }
    time.year = buf[2];
    time.month = buf[3];
    time.day = buf[4];
    time.hour = buf[5];
    time.minute = buf[6];
    time.second = buf[7];
    rc = BSP_RTC_set(time.year, time.month, time.day, time.hour, time.minute, time.second);
    APP_CHECK_RC(rc)
    BSP_LOG_debug("BSP_RTC_load_from_backup OK");
    return APP_OK;
}

int BSP_RTC_save_to_backup(datetime_pt datetime)
{
    if (datetime->year < 20)
    {
        return APP_ERROR;
    }
    uint8_t buf[EEPROM_BLOCK_SIZE] = {0};
    buf[0] = 0xa5;
    buf[1] = 0x5a;
    buf[2] = datetime->year;
    buf[3] = datetime->month;
    buf[4] = datetime->day;
    buf[5] = datetime->hour;
    buf[6] = datetime->minute;
    buf[7] = datetime->second;
    int rc = EEPROM_WriteBlock(RTC_TIME_PAGE, buf);
    APP_CHECK_RC(rc)
    BSP_LOG_debug("BSP_RTC_save_to_backup OK");
    return APP_OK;
}

int BSP_RTC_use_HSE(void)
{
    BSP_LOG_debug("RTC use HSE\r\n");
    HAL_PWR_EnableBkUpAccess();
    __HAL_RCC_LSE_CONFIG(RCC_LSE_OFF);
    HARDWARE_OS_DELAY_MS(50);
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
    PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_HSE_DIV16;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
        Error_Handler();
        return APP_ERROR;
    }
    hrtc.Instance = RTC;
    hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
    hrtc.Init.AsynchPrediv = 124;
    hrtc.Init.SynchPrediv = 3999;
    hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
    hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
    hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
    if (HAL_RTC_Init(&hrtc) != HAL_OK)
    {
        Error_Handler();
        return APP_ERROR;
    }
    BSP_LOG_debug("RTC use HSE OK\r\n");
    return APP_OK;
}

static int _init_LSE(void)
{
    BSP_LOG_debug("RTC init LSE\r\n");
    uint32_t tickstart = 0;
    FlagStatus pwrclkchanged = RESET;
    if (__HAL_RCC_PWR_IS_CLK_DISABLED())
    {
        __HAL_RCC_PWR_CLK_ENABLE();
        pwrclkchanged = SET;
    }

    if (HAL_IS_BIT_CLR(PWR->CR, PWR_CR_DBP))
    {
        SET_BIT(PWR->CR, PWR_CR_DBP);
        tickstart = HAL_GetTick();
        while (HAL_IS_BIT_CLR(PWR->CR, PWR_CR_DBP))
        {
            if ((HAL_GetTick() - tickstart) > RCC_DBP_TIMEOUT_VALUE)
            {
                return HAL_TIMEOUT;
            }
        }
    }
    __HAL_RCC_LSE_CONFIG(RCC_LSE_ON);
    tickstart = HAL_GetTick();
    while (__HAL_RCC_GET_FLAG(RCC_FLAG_LSERDY) == RESET)
    {
#ifdef HAL_RTC_WAITFOR_CALLBACK
        HAL_RTC_WAITFOR_CALLBACK
#endif
        if ((HAL_GetTick() - tickstart) > RCC_LSE_TIMEOUT_VALUE)
        {
            return HAL_TIMEOUT;
        }
    }
    if (pwrclkchanged == SET)
    {
        __HAL_RCC_PWR_CLK_DISABLE();
    }
    return HAL_OK;
}

int BSP_RTC_use_LSE(void)
{
    BSP_LOG_debug("RTC use LSE\r\n");
    __HAL_RCC_LSE_CONFIG(RCC_LSE_ON);
    HARDWARE_OS_DELAY_MS(50);
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
    PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
        Error_Handler();
        return APP_ERROR;
    }
    hrtc.Instance = RTC;
    hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
    hrtc.Init.AsynchPrediv = 127;
    hrtc.Init.SynchPrediv = 255;
    hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
    hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
    hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
    if (HAL_RTC_Init(&hrtc) != HAL_OK)
    {
        Error_Handler();
        return APP_ERROR;
    }
    BSP_LOG_debug("RTC use LSE OK\r\n");
    return APP_OK;
}

int BSP_RTC_set(uint8_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second)
{
    RTC_DateTypeDef s_date;
    RTC_TimeTypeDef s_time;

    s_date.Year = year;
    s_date.Month = month;
    s_date.Date = day;
    uint8_t week = calculate_week(year, month, day);
    if (week == 0)
    {
        week = 7;
    }
    s_date.WeekDay = week;

    if (HAL_RTC_SetDate(&hrtc, &s_date, RTC_FORMAT_BIN) != HAL_OK)
    {
        return APP_ERROR;
    }

    s_time.Hours = hour;
    s_time.Minutes = minute;
    s_time.Seconds = second;
    s_time.TimeFormat = RTC_HOURFORMAT12_AM;
    s_time.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
    s_time.StoreOperation = RTC_STOREOPERATION_RESET;

    if (HAL_RTC_SetTime(&hrtc, &s_time, RTC_FORMAT_BIN) != HAL_OK)
    {
        return APP_ERROR;
    }
    return APP_OK;
}

int BSP_RTC_get_datetime(datetime_pt datetime)
{
    uint8_t week;
    return BSP_RTC_get(&datetime->year, &datetime->month, &datetime->day, &datetime->hour, &datetime->minute, &datetime->second, &week);
}

int BSP_RTC_get(uint8_t *year, uint8_t *month, uint8_t *day, uint8_t *hour, uint8_t *minute, uint8_t *second, uint8_t *week)
{
    RTC_DateTypeDef s_date;
    RTC_TimeTypeDef s_time;

    if (HAL_RTC_GetTime(&hrtc, &s_time, RTC_FORMAT_BIN) != HAL_OK)
    {
        return APP_ERROR;
    }
    if (HAL_RTC_GetDate(&hrtc, &s_date, RTC_FORMAT_BIN) != HAL_OK)
    {
        return APP_ERROR;
    }

    *year = s_date.Year;
    *month = s_date.Month;
    *day = s_date.Date;
    *week = s_date.WeekDay;

    *hour = s_time.Hours;
    *minute = s_time.Minutes;
    *second = s_time.Seconds;
    return APP_OK;
}

int BSP_RTC_init(void)
{
    int rc = _init_LSE();
    if (rc == APP_OK && __HAL_RCC_GET_FLAG(RCC_FLAG_LSERDY) != RESET)
    {
        rc = BSP_RTC_use_LSE();
        if (rc == APP_OK)
        {
            return rc;
        }
    }

    BSP_LOG_debug("RTC use LSE ERROR\r\n");
    return BSP_RTC_use_HSE();
}
