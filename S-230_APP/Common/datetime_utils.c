
#include "datetime_utils.h"
#include "utils.h"
#include "time.h"

int datetime_add(datetime_pt datetime, time_t value)
{
    time_t time = datetime_to_timestamp(datetime);
    time += value;
    return timestamp_to_datetime(time, datetime);
}

int datetime_sub(datetime_pt datetime, time_t value)
{
    time_t time = datetime_to_timestamp(datetime);
    time -= value;
    return timestamp_to_datetime(time, datetime);
}

int datetime_diff(datetime_pt datetime1, datetime_pt datetime2)
{
    return datetime_to_timestamp(datetime1) - datetime_to_timestamp(datetime2);
}

int timestamp_to_datetime(time_t time, datetime_pt datetime)
{
    struct tm *tsp = NULL;
    tsp = localtime(&time);
    if (tsp == NULL)
    {
        return -1;
    }
    datetime->year = tsp->tm_year + 1900 - 2000;
    datetime->month = tsp->tm_mon + 1;
    datetime->day = tsp->tm_mday;
    datetime->hour = tsp->tm_hour;
    datetime->minute = tsp->tm_min;
    datetime->second = tsp->tm_sec;
    return 0;
}

time_t datetime_to_timestamp(datetime_pt datetime)
{
    struct tm ts = {0};
    ts.tm_year = 2000 - 1900 + datetime->year;
    ts.tm_mon = datetime->month - 1;
    ts.tm_mday = datetime->day;
    ts.tm_hour = datetime->hour;
    ts.tm_min = datetime->minute;
    ts.tm_sec = datetime->second;
    ts.tm_isdst = 0;
    return mktime(&ts);
}

#define _CHECK_DATETIME_EQUALS_FIELD(_field)    \
    if (datetime1->_field != datetime2->_field) \
        return 0;

int datetime_equals(datetime_pt datetime1, datetime_pt datetime2)
{
    _CHECK_DATETIME_EQUALS_FIELD(microsecond)
    _CHECK_DATETIME_EQUALS_FIELD(second)
    _CHECK_DATETIME_EQUALS_FIELD(minute)
    _CHECK_DATETIME_EQUALS_FIELD(hour)
    _CHECK_DATETIME_EQUALS_FIELD(day)
    _CHECK_DATETIME_EQUALS_FIELD(month)
    _CHECK_DATETIME_EQUALS_FIELD(year)
    return 1;
}

#define _COPY_DATETIME_FIELD(_field) \
    datetime1->_field = datetime2->_field;

int datetime_copy(datetime_pt datetime1, datetime_pt datetime2)
{
    _COPY_DATETIME_FIELD(second)
    _COPY_DATETIME_FIELD(minute)
    _COPY_DATETIME_FIELD(hour)
    _COPY_DATETIME_FIELD(day)
    _COPY_DATETIME_FIELD(month)
    _COPY_DATETIME_FIELD(year)
    _COPY_DATETIME_FIELD(microsecond)
    return 1;
}

// 蔡勒公式: w=y+[y/4]+[c/4]-2c+[26(m+1)/10]+d-1
// 某年的1,2月份要看作上一年的13,14月份来计算，
// 如：2003年1月1日看作2002年13月1日来计算）
uint8_t calculate_week(uint8_t year, uint8_t month, uint8_t day)
{
    int c = 20;
    int y = year;
    int d = day;
    int m = month;
    if (m == 1 || m == 2)
    {
        m = 12 + m;
        y--;
    }
    int w = y + (y / 4) + (c / 4) - 2 * c + (26 * (m + 1) / 10) + d - 1;
    while (w < 0)
        w += 7;
    w = w % 7;

    return (uint8_t)w;
}

static uint8_t _big_months[] = {1, 3, 5, 7, 8, 10, 12};

#define IS_LEAP_YEAR(year) ((((year) % 4 == 0) && ((year) % 100 != 0)) || ((year) % 400 == 0))

int check_datetime1(uint8_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second, uint16_t microsecond)
{
    if ((month > 12) ||
        (day > 31) ||
        (hour > 23) ||
        (month < 1) ||
        (day < 1) ||
        (minute > 59) ||
        (second > 59) ||
        (microsecond > 999))
    {
        return -1;
    }
    if (day <= 28)
    {
        return 0;
    }
    if (month == 2)
    {
        uint16_t y = year + 2000;
        if (IS_LEAP_YEAR(y))
        {
            if (day <= 29)
            {
                return 0;
            }
        }
        return -1;
    }
    if (day <= 30)
    {
        return 0;
    }
    if (get_index_uint8_t(_big_months, 7, month) >= 0 && day <= 31)
    {
        return 0;
    }

    return -1;
}

int check_datetime(datetime_pt datetime)
{
    return check_datetime1(datetime->year, datetime->month, datetime->day, datetime->hour, datetime->minute, datetime->second, datetime->microsecond);
}
