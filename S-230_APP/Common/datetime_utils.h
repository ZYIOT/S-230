#ifndef __COMMON_DATETIME_UTILS_H__
#define __COMMON_DATETIME_UTILS_H__

#ifdef __cplusplus
extern "C"
{
#endif
#include "stdint.h"
#include "time.h"
    typedef struct
    {
        uint8_t year;
        uint8_t month;
        uint8_t day;
        uint8_t hour;
        uint8_t minute;
        uint8_t second;
        uint16_t microsecond;
    } datetime_t, *datetime_pt;

    uint8_t calculate_week(uint8_t year, uint8_t month, uint8_t day);
    int datetime_add(datetime_pt datetime, time_t value);
    int timestamp_to_datetime(time_t time, datetime_pt datetime);
    time_t datetime_to_timestamp(datetime_pt datetime);
    int check_datetime(datetime_pt datetime);
    int check_datetime1(uint8_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second, uint16_t microsecond);
    int datetime_equals(datetime_pt datetime1, datetime_pt datetime2);
    int datetime_copy(datetime_pt datetime1, datetime_pt datetime2);
#ifdef __cplusplus
}
#endif
#endif
