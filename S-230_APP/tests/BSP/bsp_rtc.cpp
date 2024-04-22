
#include "bsp_rtc.h"
#include "hardware_wrapper.h"
#include "utils.h"
#include "time.h"
#ifdef __cplusplus
extern "C"
{
#endif
void BSP_RTC_init(void)
{
}

int BSP_RTC_set(uint8_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second)
{
    return APP_OK;
}

int BSP_RTC_get_datetime(datetime_pt datetime)
{
    uint8_t week;
    return BSP_RTC_get(&datetime->year, &datetime->month, &datetime->day, &datetime->hour, &datetime->minute, &datetime->second, &week);
}

int BSP_RTC_get(uint8_t *year, uint8_t *month, uint8_t *day, uint8_t *hour, uint8_t *minute, uint8_t *second, uint8_t *week)
{
    time_t timep;
    struct tm *p;
    time(&timep);
    p = localtime(&timep);
    *year = 2000 - 1900 + p->tm_year;
    *month = p->tm_mon + 1;
    *day = p->tm_mday;
    *hour = p->tm_hour;
    *minute = p->tm_min;
    *second = p->tm_sec;
    return APP_OK;
}
#ifdef __cplusplus
}
#endif
