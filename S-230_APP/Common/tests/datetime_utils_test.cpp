#include <gtest/gtest.h>

extern "C"
{
#include "datetime_utils.c"
#include "time.h"
}

TEST(localtime, pass)
{
  time_t rawtime = 1046448000;
  struct tm ts;
  char buf[80];
  ts = *localtime(&rawtime);
  strftime(buf, sizeof(buf), "%a %Y-%m-%d %H:%M:%S %Z", &ts);
  printf("%s\n", buf);
}

TEST(mktime, pass)
{
  struct tm ts = {0};
  ts.tm_year = 2020 - 1900;
  ts.tm_mon = 5 - 1;
  ts.tm_mday = 18;
  ts.tm_hour = 15;
  ts.tm_min = 12;
  ts.tm_sec = 12;
  ts.tm_isdst = 0;
  time_t time = mktime(&ts);
  printf("%lld\n", time);
}

TEST(calculate_week, pass)
{
  EXPECT_EQ(1, calculate_week(11, 6, 6));
  EXPECT_EQ(4, calculate_week(16, 6, 2));
  EXPECT_EQ(0, calculate_week(11, 5, 1));
  EXPECT_EQ(1, calculate_week(20, 6, 1));
  EXPECT_EQ(1, calculate_week(20, 6, 22));
  EXPECT_EQ(3, calculate_week(20, 7, 29));
  EXPECT_EQ(4, calculate_week(20, 8, 27));
  EXPECT_EQ(0, calculate_week(20, 7, 12));
  EXPECT_EQ(2, calculate_week(88, 7, 27));
  EXPECT_EQ(6, calculate_week(98, 5, 10));
  EXPECT_EQ(6, calculate_week(78, 5, 14));
  EXPECT_EQ(3, calculate_week(52, 12, 25));
  EXPECT_EQ(0, calculate_week(25, 5, 18));
  EXPECT_EQ(2, calculate_week(60, 3, 23));
  EXPECT_EQ(6, calculate_week(14, 4, 19));
  EXPECT_EQ(1, calculate_week(35, 7, 9));
  EXPECT_EQ(0, calculate_week(30, 12, 22));
  EXPECT_EQ(6, calculate_week(23, 7, 8));
  EXPECT_EQ(6, calculate_week(81, 12, 13));
  EXPECT_EQ(2, calculate_week(88, 4, 27));
  EXPECT_EQ(6, calculate_week(54, 8, 15));
  EXPECT_EQ(6, calculate_week(69, 5, 25));
  EXPECT_EQ(1, calculate_week(97, 3, 18));
  EXPECT_EQ(5, calculate_week(86, 8, 23));
  EXPECT_EQ(1, calculate_week(59, 11, 24));
  EXPECT_EQ(3, calculate_week(33, 12, 7));
  EXPECT_EQ(6, calculate_week(53, 8, 2));
  EXPECT_EQ(0, calculate_week(49, 12, 5));
  EXPECT_EQ(5, calculate_week(24, 5, 24));
  EXPECT_EQ(4, calculate_week(54, 3, 12));
  EXPECT_EQ(2, calculate_week(40, 3, 6));
  EXPECT_EQ(6, calculate_week(46, 9, 8));
  EXPECT_EQ(4, calculate_week(42, 7, 24));
  EXPECT_EQ(1, calculate_week(83, 1, 4));
  EXPECT_EQ(6, calculate_week(70, 5, 17));
  EXPECT_EQ(6, calculate_week(65, 9, 12));
  EXPECT_EQ(2, calculate_week(36, 3, 11));
  EXPECT_EQ(5, calculate_week(36, 10, 10));
  EXPECT_EQ(1, calculate_week(30, 3, 4));
  EXPECT_EQ(4, calculate_week(59, 9, 25));
  EXPECT_EQ(0, calculate_week(34, 5, 14));
  EXPECT_EQ(6, calculate_week(30, 11, 16));
  EXPECT_EQ(0, calculate_week(28, 2, 27));
  EXPECT_EQ(1, calculate_week(06, 5, 15));
  EXPECT_EQ(3, calculate_week(22, 11, 23));
  EXPECT_EQ(5, calculate_week(14, 2, 14));
  EXPECT_EQ(1, calculate_week(50, 7, 4));
  EXPECT_EQ(5, calculate_week(42, 1, 3));
  EXPECT_EQ(6, calculate_week(96, 5, 26));
  EXPECT_EQ(6, calculate_week(53, 9, 20));
  EXPECT_EQ(0, calculate_week(37, 2, 15));
  EXPECT_EQ(5, calculate_week(42, 1, 3));
  EXPECT_EQ(4, calculate_week(52, 8, 22));
  EXPECT_EQ(3, calculate_week(40, 1, 11));
  EXPECT_EQ(1, calculate_week(98, 4, 21));
  EXPECT_EQ(0, calculate_week(46, 10, 14));
  EXPECT_EQ(2, calculate_week(23, 4, 18));
  EXPECT_EQ(4, calculate_week(76, 2, 6));
  EXPECT_EQ(2, calculate_week(34, 12, 5));
  EXPECT_EQ(1, calculate_week(94, 9, 27));
  EXPECT_EQ(5, calculate_week(72, 9, 23));
  EXPECT_EQ(3, calculate_week(97, 3, 13));
  EXPECT_EQ(3, calculate_week(12, 4, 4));
  EXPECT_EQ(4, calculate_week(34, 10, 5));
  EXPECT_EQ(4, calculate_week(65, 5, 7));
  EXPECT_EQ(1, calculate_week(25, 2, 24));
  EXPECT_EQ(2, calculate_week(60, 10, 5));
  EXPECT_EQ(3, calculate_week(02, 8, 28));
  EXPECT_EQ(6, calculate_week(55, 9, 4));
  EXPECT_EQ(6, calculate_week(51, 12, 9));
  EXPECT_EQ(5, calculate_week(96, 7, 27));
  EXPECT_EQ(5, calculate_week(72, 6, 10));
  EXPECT_EQ(5, calculate_week(92, 8, 1));
  EXPECT_EQ(0, calculate_week(47, 11, 24));
  EXPECT_EQ(1, calculate_week(35, 11, 26));
  EXPECT_EQ(3, calculate_week(73, 8, 16));
  EXPECT_EQ(0, calculate_week(81, 9, 7));
  EXPECT_EQ(4, calculate_week(31, 11, 13));
  EXPECT_EQ(3, calculate_week(44, 9, 7));
  EXPECT_EQ(3, calculate_week(88, 11, 10));
  EXPECT_EQ(1, calculate_week(56, 4, 10));
  EXPECT_EQ(5, calculate_week(67, 7, 15));
  EXPECT_EQ(2, calculate_week(99, 5, 5));
  EXPECT_EQ(4, calculate_week(95, 4, 14));
  EXPECT_EQ(3, calculate_week(25, 10, 15));
  EXPECT_EQ(6, calculate_week(14, 11, 22));
  EXPECT_EQ(3, calculate_week(66, 10, 27));
  EXPECT_EQ(3, calculate_week(20, 6, 17));
  EXPECT_EQ(0, calculate_week(89, 10, 16));
  EXPECT_EQ(2, calculate_week(98, 6, 24));
  EXPECT_EQ(4, calculate_week(90, 1, 12));
  EXPECT_EQ(2, calculate_week(80, 6, 4));
  EXPECT_EQ(6, calculate_week(71, 7, 4));
  EXPECT_EQ(0, calculate_week(16, 9, 25));
  EXPECT_EQ(0, calculate_week(97, 11, 10));
  EXPECT_EQ(2, calculate_week(81, 4, 8));
  EXPECT_EQ(6, calculate_week(20, 2, 22));
  EXPECT_EQ(1, calculate_week(01, 7, 23));
  EXPECT_EQ(2, calculate_week(83, 7, 27));
  EXPECT_EQ(3, calculate_week(53, 3, 12));
  EXPECT_EQ(3, calculate_week(35, 3, 28));
  EXPECT_EQ(0, calculate_week(85, 5, 13));
  EXPECT_EQ(2, calculate_week(69, 2, 12));
  EXPECT_EQ(2, calculate_week(55, 1, 26));
  EXPECT_EQ(0, calculate_week(42, 12, 14));
  EXPECT_EQ(1, calculate_week(80, 11, 4));
  EXPECT_EQ(3, calculate_week(95, 3, 23));
  EXPECT_EQ(0, calculate_week(14, 7, 13));
  EXPECT_EQ(5, calculate_week(34, 7, 7));
  EXPECT_EQ(6, calculate_week(83, 10, 16));
  EXPECT_EQ(6, calculate_week(90, 10, 21));
  EXPECT_EQ(4, calculate_week(21, 11, 25));
  EXPECT_EQ(0, calculate_week(92, 5, 25));
  EXPECT_EQ(6, calculate_week(15, 7, 11));
  EXPECT_EQ(0, calculate_week(90, 4, 2));
  EXPECT_EQ(0, calculate_week(73, 4, 16));
  EXPECT_EQ(4, calculate_week(74, 1, 4));
  EXPECT_EQ(6, calculate_week(23, 6, 10));
  EXPECT_EQ(0, calculate_week(48, 2, 16));
  EXPECT_EQ(6, calculate_week(44, 10, 22));
  EXPECT_EQ(5, calculate_week(75, 8, 16));
  EXPECT_EQ(6, calculate_week(51, 3, 4));
  EXPECT_EQ(2, calculate_week(85, 3, 27));
  EXPECT_EQ(1, calculate_week(82, 5, 4));
  EXPECT_EQ(3, calculate_week(68, 12, 5));
  EXPECT_EQ(0, calculate_week(29, 2, 11));
  EXPECT_EQ(3, calculate_week(02, 3, 13));
  EXPECT_EQ(0, calculate_week(10, 6, 20));
  EXPECT_EQ(4, calculate_week(84, 11, 23));
  EXPECT_EQ(2, calculate_week(98, 8, 12));
  EXPECT_EQ(4, calculate_week(24, 10, 10));
  EXPECT_EQ(1, calculate_week(43, 7, 27));
  EXPECT_EQ(1, calculate_week(91, 3, 19));
  EXPECT_EQ(2, calculate_week(23, 8, 22));
  EXPECT_EQ(2, calculate_week(58, 7, 9));
  EXPECT_EQ(1, calculate_week(01, 7, 9));
  EXPECT_EQ(1, calculate_week(87, 7, 7));
  EXPECT_EQ(6, calculate_week(51, 6, 10));
  EXPECT_EQ(6, calculate_week(22, 7, 23));
  EXPECT_EQ(3, calculate_week(79, 12, 20));
  EXPECT_EQ(5, calculate_week(95, 2, 18));
  EXPECT_EQ(6, calculate_week(77, 9, 11));
  EXPECT_EQ(4, calculate_week(95, 1, 20));
  EXPECT_EQ(3, calculate_week(53, 5, 7));
  EXPECT_EQ(6, calculate_week(39, 5, 21));
  EXPECT_EQ(4, calculate_week(75, 7, 18));
  EXPECT_EQ(5, calculate_week(15, 7, 10));
  EXPECT_EQ(0, calculate_week(00, 7, 23));
  EXPECT_EQ(2, calculate_week(16, 2, 2));
  EXPECT_EQ(3, calculate_week(01, 12, 5));
  EXPECT_EQ(5, calculate_week(28, 10, 27));
  EXPECT_EQ(1, calculate_week(96, 3, 19));
  EXPECT_EQ(1, calculate_week(77, 1, 11));
  EXPECT_EQ(4, calculate_week(61, 4, 14));
  EXPECT_EQ(1, calculate_week(58, 7, 15));
  EXPECT_EQ(3, calculate_week(74, 11, 14));
  EXPECT_EQ(1, calculate_week(31, 4, 28));
  EXPECT_EQ(3, calculate_week(36, 6, 4));
  EXPECT_EQ(6, calculate_week(57, 1, 13));
  EXPECT_EQ(0, calculate_week(95, 9, 25));
  EXPECT_EQ(5, calculate_week(51, 6, 16));
  EXPECT_EQ(0, calculate_week(87, 12, 14));
  EXPECT_EQ(5, calculate_week(11, 3, 4));
  EXPECT_EQ(5, calculate_week(46, 8, 17));
  EXPECT_EQ(6, calculate_week(40, 8, 4));
  EXPECT_EQ(6, calculate_week(28, 7, 22));
  EXPECT_EQ(5, calculate_week(47, 9, 6));
  EXPECT_EQ(5, calculate_week(31, 3, 21));
  EXPECT_EQ(4, calculate_week(49, 3, 18));
  EXPECT_EQ(6, calculate_week(67, 3, 26));
  EXPECT_EQ(0, calculate_week(96, 1, 15));
  EXPECT_EQ(2, calculate_week(53, 6, 10));
  EXPECT_EQ(3, calculate_week(59, 5, 14));
  EXPECT_EQ(6, calculate_week(80, 1, 6));
  EXPECT_EQ(5, calculate_week(87, 1, 17));
  EXPECT_EQ(2, calculate_week(77, 9, 7));
  EXPECT_EQ(2, calculate_week(81, 7, 8));
  EXPECT_EQ(5, calculate_week(65, 2, 27));
  EXPECT_EQ(4, calculate_week(71, 11, 5));
  EXPECT_EQ(4, calculate_week(50, 6, 23));
  EXPECT_EQ(3, calculate_week(87, 7, 9));
  EXPECT_EQ(6, calculate_week(35, 6, 16));
  EXPECT_EQ(5, calculate_week(23, 6, 16));
  EXPECT_EQ(1, calculate_week(80, 2, 12));
  EXPECT_EQ(6, calculate_week(82, 5, 9));
  EXPECT_EQ(4, calculate_week(53, 3, 13));
  EXPECT_EQ(3, calculate_week(96, 9, 26));
  EXPECT_EQ(5, calculate_week(91, 10, 5));
  EXPECT_EQ(3, calculate_week(63, 9, 19));
  EXPECT_EQ(1, calculate_week(29, 5, 28));
  EXPECT_EQ(3, calculate_week(70, 3, 26));
  EXPECT_EQ(4, calculate_week(88, 9, 9));
  EXPECT_EQ(3, calculate_week(48, 3, 4));
  EXPECT_EQ(5, calculate_week(96, 3, 9));
  EXPECT_EQ(2, calculate_week(26, 5, 12));
  EXPECT_EQ(0, calculate_week(12, 6, 3));
  EXPECT_EQ(0, calculate_week(42, 8, 3));
  EXPECT_EQ(2, calculate_week(46, 6, 26));
  EXPECT_EQ(0, calculate_week(20, 3, 1));
  EXPECT_EQ(6, calculate_week(18, 6, 9));
  EXPECT_EQ(3, calculate_week(65, 9, 23));
  EXPECT_EQ(1, calculate_week(15, 2, 16));
  EXPECT_EQ(0, calculate_week(11, 2, 13));
  EXPECT_EQ(2, calculate_week(22, 5, 24));
  EXPECT_EQ(0, calculate_week(79, 2, 26));
  EXPECT_EQ(0, calculate_week(32, 4, 25));
  EXPECT_EQ(6, calculate_week(75, 9, 7));
  EXPECT_EQ(3, calculate_week(63, 2, 7));
}

TEST(check_datetime, ok)
{
  datetime_t dt = {.year = 20, .month = 6, .day = 2, .hour = 12, .minute = 20, .second = 0, .microsecond = 0};
  EXPECT_EQ(0, check_datetime(&dt));
  dt = {.year = 20, .month = 1, .day = 31, .hour = 0, .minute = 0, .second = 0, .microsecond = 0};
  EXPECT_EQ(0, check_datetime(&dt));
  dt = {.year = 20, .month = 1, .day = 31, .hour = 0, .minute = 0, .second = 0, .microsecond = 0};
  EXPECT_EQ(0, check_datetime(&dt));
  dt = {.year = 20, .month = 2, .day = 28, .hour = 0, .minute = 0, .second = 0, .microsecond = 0};
  EXPECT_EQ(0, check_datetime(&dt));
  dt = {.year = 20, .month = 3, .day = 30, .hour = 0, .minute = 0, .second = 0, .microsecond = 0};
  EXPECT_EQ(0, check_datetime(&dt));
  dt = {.year = 20, .month = 8, .day = 31, .hour = 0, .minute = 0, .second = 0, .microsecond = 0};
  EXPECT_EQ(0, check_datetime(&dt));
  dt = {.year = 20, .month = 7, .day = 31, .hour = 0, .minute = 0, .second = 0, .microsecond = 0};
  EXPECT_EQ(0, check_datetime(&dt));
  dt = {.year = 20, .month = 12, .day = 31, .hour = 0, .minute = 0, .second = 0, .microsecond = 0};
  EXPECT_EQ(0, check_datetime(&dt));
  dt = {.year = 4, .month = 2, .day = 29, .hour = 0, .minute = 0, .second = 0, .microsecond = 0};
  EXPECT_EQ(0, check_datetime(&dt));
  dt = {.year = 0, .month = 2, .day = 29, .hour = 0, .minute = 0, .second = 0, .microsecond = 0};
  EXPECT_EQ(0, check_datetime(&dt));
  dt = {.year = 40, .month = 2, .day = 29, .hour = 0, .minute = 0, .second = 0, .microsecond = 0};
  EXPECT_EQ(0, check_datetime(&dt));
  dt = {.year = 80, .month = 2, .day = 29, .hour = 0, .minute = 0, .second = 0, .microsecond = 0};
  EXPECT_EQ(0, check_datetime(&dt));
}

TEST(check_datetime, error)
{
  datetime_t dt = {.year = 20, .month = 6, .day = 2, .hour = 12, .minute = 20, .second = 60, .microsecond = 0};
  EXPECT_EQ(-1, check_datetime(&dt));
  dt = {.year = 20, .month = 6, .day = 2, .hour = 12, .minute = 60, .second = 1, .microsecond = 0};
  EXPECT_EQ(-1, check_datetime(&dt));
  dt = {.year = 20, .month = 6, .day = 2, .hour = 24, .minute = 1, .second = 1, .microsecond = 0};
  EXPECT_EQ(-1, check_datetime(&dt));
  dt = {.year = 20, .month = 6, .day = 2, .hour = 1, .minute = 1, .second = 1, .microsecond = 10000};
  EXPECT_EQ(-1, check_datetime(&dt));
  dt = {.year = 20, .month = 1, .day = 0, .hour = 0, .minute = 0, .second = 0, .microsecond = 0};
  EXPECT_EQ(-1, check_datetime(&dt));
  dt = {.year = 20, .month = 1, .day = 32, .hour = 0, .minute = 0, .second = 0, .microsecond = 0};
  EXPECT_EQ(-1, check_datetime(&dt));
  dt = {.year = 20, .month = 2, .day = 30, .hour = 0, .minute = 0, .second = 0, .microsecond = 0};
  EXPECT_EQ(-1, check_datetime(&dt));
  dt = {.year = 20, .month = 4, .day = 31, .hour = 0, .minute = 0, .second = 0, .microsecond = 0};
  EXPECT_EQ(-1, check_datetime(&dt));
  dt = {.year = 20, .month = 6, .day = 31, .hour = 0, .minute = 0, .second = 0, .microsecond = 0};
  EXPECT_EQ(-1, check_datetime(&dt));
  dt = {.year = 20, .month = 11, .day = 31, .hour = 0, .minute = 0, .second = 0, .microsecond = 0};
  EXPECT_EQ(-1, check_datetime(&dt));
  dt = {.year = 5, .month = 2, .day = 29, .hour = 0, .minute = 0, .second = 0, .microsecond = 0};
  EXPECT_EQ(-1, check_datetime(&dt));
  dt = {.year = 3, .month = 2, .day = 29, .hour = 0, .minute = 0, .second = 0, .microsecond = 0};
  EXPECT_EQ(-1, check_datetime(&dt));
  dt = {.year = 100, .month = 2, .day = 29, .hour = 0, .minute = 0, .second = 0, .microsecond = 0};
  EXPECT_EQ(-1, check_datetime(&dt));
  dt = {.year = 200, .month = 2, .day = 29, .hour = 0, .minute = 0, .second = 0, .microsecond = 0};
  EXPECT_EQ(-1, check_datetime(&dt));
}

TEST(check_datetime1, ok)
{
  EXPECT_EQ(0, check_datetime1(20, 6, 2, 12, 20, 0, 0));
  EXPECT_EQ(0, check_datetime1(20, 1, 31, 0, 0, 0, 0));
  EXPECT_EQ(0, check_datetime1(20, 1, 31, 0, 0, 0, 0));
  EXPECT_EQ(0, check_datetime1(20, 2, 28, 0, 0, 0, 0));
  EXPECT_EQ(0, check_datetime1(20, 3, 30, 0, 0, 0, 0));
  EXPECT_EQ(0, check_datetime1(20, 8, 31, 0, 0, 0, 0));
  EXPECT_EQ(0, check_datetime1(20, 7, 31, 0, 0, 0, 0));
  EXPECT_EQ(0, check_datetime1(20, 12, 31, 0, 0, 0, 0));
  EXPECT_EQ(0, check_datetime1(4, 2, 29, 0, 0, 0, 0));
  EXPECT_EQ(0, check_datetime1(0, 2, 29, 0, 0, 0, 0));
  EXPECT_EQ(0, check_datetime1(40, 2, 29, 0, 0, 0, 0));
  EXPECT_EQ(0, check_datetime1(80, 2, 29, 0, 0, 0, 0));
}

TEST(check_datetime1, error)
{
  EXPECT_EQ(-1, check_datetime1(20, 6, 2, 12, 20, 60, 0));
  EXPECT_EQ(-1, check_datetime1(20, 6, 2, 12, 60, 1, 0));
  EXPECT_EQ(-1, check_datetime1(20, 6, 2, 24, 1, 1, 0));
  EXPECT_EQ(-1, check_datetime1(20, 6, 2, 1, 1, 1, 10000));
  EXPECT_EQ(-1, check_datetime1(20, 1, 0, 0, 0, 0, 0));
  EXPECT_EQ(-1, check_datetime1(20, 1, 32, 0, 0, 0, 0));
  EXPECT_EQ(-1, check_datetime1(20, 2, 30, 0, 0, 0, 0));
  EXPECT_EQ(-1, check_datetime1(20, 4, 31, 0, 0, 0, 0));
  EXPECT_EQ(-1, check_datetime1(20, 6, 31, 0, 0, 0, 0));
  EXPECT_EQ(-1, check_datetime1(20, 11, 31, 0, 0, 0, 0));
  EXPECT_EQ(-1, check_datetime1(5, 2, 29, 0, 0, 0, 0));
  EXPECT_EQ(-1, check_datetime1(3, 2, 29, 0, 0, 0, 0));
  EXPECT_EQ(-1, check_datetime1(100, 2, 29, 0, 0, 0, 0));
  EXPECT_EQ(-1, check_datetime1(200, 2, 29, 0, 0, 0, 0));
}

#define EXPECT_EQ_DATETIME(dt, dt1)           \
  EXPECT_EQ(dt.year, dt1.year);               \
  EXPECT_EQ(dt.month, dt1.month);             \
  EXPECT_EQ(dt.day, dt1.day);                 \
  EXPECT_EQ(dt.hour, dt1.hour);               \
  EXPECT_EQ(dt.minute, dt1.minute);           \
  EXPECT_EQ(dt.second, dt1.second);           \
  EXPECT_EQ(dt.microsecond, dt1.microsecond); \
  EXPECT_EQ(0, dt1.microsecond);

TEST(datetime_to_timestamp, ok)
{
  datetime_t dt = {.year = 20, .month = 6, .day = 2, .hour = 12, .minute = 20, .second = 0, .microsecond = 0};
  datetime_t dt1 = {0};
  time_t time = 0;
  time = datetime_to_timestamp(&dt);
  EXPECT_EQ(0, timestamp_to_datetime(time, &dt1));
  EXPECT_EQ_DATETIME(dt, dt1)
  dt = {.year = 20, .month = 1, .day = 31, .hour = 0, .minute = 0, .second = 0, .microsecond = 0};
  time = datetime_to_timestamp(&dt);
  EXPECT_EQ(0, timestamp_to_datetime(time, &dt1));
  EXPECT_EQ_DATETIME(dt, dt1)
  dt = {.year = 20, .month = 1, .day = 31, .hour = 0, .minute = 0, .second = 0, .microsecond = 0};
  time = datetime_to_timestamp(&dt);
  EXPECT_EQ(0, timestamp_to_datetime(time, &dt1));
  EXPECT_EQ_DATETIME(dt, dt1)
  dt = {.year = 20, .month = 2, .day = 28, .hour = 0, .minute = 0, .second = 0, .microsecond = 0};
  time = datetime_to_timestamp(&dt);
  EXPECT_EQ(0, timestamp_to_datetime(time, &dt1));
  EXPECT_EQ_DATETIME(dt, dt1)
  dt = {.year = 20, .month = 3, .day = 30, .hour = 0, .minute = 0, .second = 0, .microsecond = 0};
  time = datetime_to_timestamp(&dt);
  EXPECT_EQ(0, timestamp_to_datetime(time, &dt1));
  EXPECT_EQ_DATETIME(dt, dt1)
  dt = {.year = 20, .month = 8, .day = 31, .hour = 0, .minute = 0, .second = 0, .microsecond = 0};
  time = datetime_to_timestamp(&dt);
  EXPECT_EQ(0, timestamp_to_datetime(time, &dt1));
  EXPECT_EQ_DATETIME(dt, dt1)
  dt = {.year = 20, .month = 7, .day = 31, .hour = 0, .minute = 0, .second = 0, .microsecond = 0};
  time = datetime_to_timestamp(&dt);
  EXPECT_EQ(0, timestamp_to_datetime(time, &dt1));
  EXPECT_EQ_DATETIME(dt, dt1)
  dt = {.year = 20, .month = 12, .day = 31, .hour = 0, .minute = 0, .second = 0, .microsecond = 0};
  time = datetime_to_timestamp(&dt);
  EXPECT_EQ(0, timestamp_to_datetime(time, &dt1));
  EXPECT_EQ_DATETIME(dt, dt1)
  dt = {.year = 4, .month = 2, .day = 29, .hour = 0, .minute = 0, .second = 0, .microsecond = 0};
  time = datetime_to_timestamp(&dt);
  EXPECT_EQ(0, timestamp_to_datetime(time, &dt1));
  EXPECT_EQ_DATETIME(dt, dt1)
  dt = {.year = 0, .month = 2, .day = 29, .hour = 0, .minute = 0, .second = 0, .microsecond = 0};
  time = datetime_to_timestamp(&dt);
  EXPECT_EQ(0, timestamp_to_datetime(time, &dt1));
  EXPECT_EQ_DATETIME(dt, dt1)
  dt = {.year = 40, .month = 2, .day = 29, .hour = 0, .minute = 0, .second = 0, .microsecond = 0};
  time = datetime_to_timestamp(&dt);
  EXPECT_EQ(0, timestamp_to_datetime(time, &dt1));
  EXPECT_EQ_DATETIME(dt, dt1)
  dt = {.year = 80, .month = 2, .day = 29, .hour = 0, .minute = 0, .second = 0, .microsecond = 0};
  time = datetime_to_timestamp(&dt);
  EXPECT_EQ(0, timestamp_to_datetime(time, &dt1));
  EXPECT_EQ_DATETIME(dt, dt1)
}

TEST(datetime_add, ok)
{
  datetime_t dt = {.year = 20, .month = 6, .day = 2, .hour = 12, .minute = 20, .second = 0, .microsecond = 0};
  datetime_t dt1 = {.year = 20, .month = 6, .day = 2, .hour = 12, .minute = 20, .second = 1, .microsecond = 0};
  EXPECT_EQ(0, datetime_add(&dt, 1));
  EXPECT_EQ_DATETIME(dt1, dt)

  dt = {.year = 20, .month = 1, .day = 31, .hour = 0, .minute = 0, .second = 0, .microsecond = 0};
  dt1 = {.year = 20, .month = 1, .day = 31, .hour = 0, .minute = 1, .second = 0, .microsecond = 0};
  EXPECT_EQ(0, datetime_add(&dt, 60));
  EXPECT_EQ_DATETIME(dt1, dt)

  dt = {.year = 20, .month = 1, .day = 31, .hour = 0, .minute = 0, .second = 0, .microsecond = 0};
  dt1 = {.year = 20, .month = 1, .day = 31, .hour = 2, .minute = 6, .second = 0, .microsecond = 0};
  EXPECT_EQ(0, datetime_add(&dt, 2 * 60 * 60 + 6 * 60));
  EXPECT_EQ_DATETIME(dt1, dt)

  dt = {.year = 20, .month = 1, .day = 31, .hour = 0, .minute = 0, .second = 0, .microsecond = 0};
  dt1 = {.year = 20, .month = 2, .day = 3, .hour = 2, .minute = 6, .second = 0, .microsecond = 0};
  EXPECT_EQ(0, datetime_add(&dt, 3 * 24 * 60 * 60 + 2 * 60 * 60 + 6 * 60));
  EXPECT_EQ_DATETIME(dt1, dt)

  dt = {.year = 20, .month = 1, .day = 31, .hour = 0, .minute = 0, .second = 0, .microsecond = 0};
  dt1 = {.year = 20, .month = 1, .day = 31, .hour = 1, .minute = 0, .second = 0, .microsecond = 0};
  EXPECT_EQ(0, datetime_add(&dt, 60 * 60));
  EXPECT_EQ_DATETIME(dt1, dt)

  dt = {.year = 20, .month = 1, .day = 31, .hour = 0, .minute = 0, .second = 0, .microsecond = 0};
  dt1 = {.year = 20, .month = 2, .day = 1, .hour = 0, .minute = 0, .second = 0, .microsecond = 0};
  EXPECT_EQ(0, datetime_add(&dt, 24 * 60 * 60));
  EXPECT_EQ_DATETIME(dt1, dt)

  dt = {.year = 20, .month = 2, .day = 28, .hour = 0, .minute = 0, .second = 0, .microsecond = 0};
  dt1 = {.year = 20, .month = 2, .day = 29, .hour = 0, .minute = 0, .second = 0, .microsecond = 0};
  EXPECT_EQ(0, datetime_add(&dt, 24 * 60 * 60));
  EXPECT_EQ_DATETIME(dt1, dt)

  dt = {.year = 21, .month = 2, .day = 28, .hour = 0, .minute = 0, .second = 0, .microsecond = 0};
  dt1 = {.year = 21, .month = 3, .day = 1, .hour = 0, .minute = 0, .second = 0, .microsecond = 0};
  EXPECT_EQ(0, datetime_add(&dt, 24 * 60 * 60));
  EXPECT_EQ_DATETIME(dt1, dt)

  dt = {.year = 20, .month = 3, .day = 30, .hour = 0, .minute = 0, .second = 0, .microsecond = 0};
  dt1 = {.year = 20, .month = 4, .day = 29, .hour = 0, .minute = 0, .second = 0, .microsecond = 0};
  EXPECT_EQ(0, datetime_add(&dt, 30 * 24 * 60 * 60));
  EXPECT_EQ_DATETIME(dt1, dt)

  dt = {.year = 20, .month = 4, .day = 30, .hour = 0, .minute = 0, .second = 0, .microsecond = 0};
  dt1 = {.year = 20, .month = 5, .day = 30, .hour = 0, .minute = 0, .second = 0, .microsecond = 0};
  EXPECT_EQ(0, datetime_add(&dt, 30 * 24 * 60 * 60));
  EXPECT_EQ_DATETIME(dt1, dt)

  dt = {.year = 20, .month = 4, .day = 30, .hour = 0, .minute = 0, .second = 0, .microsecond = 0};
  dt1 = {.year = 20, .month = 5, .day = 30, .hour = 0, .minute = 0, .second = 5, .microsecond = 0};
  EXPECT_EQ(0, datetime_add(&dt, 30 * 24 * 60 * 60 + 5));
  EXPECT_EQ_DATETIME(dt1, dt)

  dt = {.year = 20, .month = 8, .day = 31, .hour = 0, .minute = 0, .second = 0, .microsecond = 0};
  dt1 = {.year = 21, .month = 8, .day = 31, .hour = 0, .minute = 0, .second = 1, .microsecond = 0};
  EXPECT_EQ(0, datetime_add(&dt, 365 * 24 * 60 * 60 + 1));
  EXPECT_EQ_DATETIME(dt1, dt)

  dt = {.year = 19, .month = 8, .day = 31, .hour = 0, .minute = 0, .second = 0, .microsecond = 0};
  dt1 = {.year = 20, .month = 8, .day = 30, .hour = 0, .minute = 1, .second = 0, .microsecond = 0};
  EXPECT_EQ(0, datetime_add(&dt, 365 * 24 * 60 * 60 + 60));
  EXPECT_EQ_DATETIME(dt1, dt)
}

TEST(datetime_sub, ok)
{
  datetime_t dt = {.year = 20, .month = 6, .day = 2, .hour = 12, .minute = 20, .second = 0, .microsecond = 0};
  datetime_t dt1 = {.year = 20, .month = 6, .day = 2, .hour = 12, .minute = 20, .second = 1, .microsecond = 0};
  EXPECT_EQ(0, datetime_sub(&dt1, 1));
  EXPECT_EQ_DATETIME(dt, dt1)

  dt = {.year = 20, .month = 1, .day = 31, .hour = 0, .minute = 0, .second = 0, .microsecond = 0};
  dt1 = {.year = 20, .month = 1, .day = 31, .hour = 0, .minute = 1, .second = 0, .microsecond = 0};
  EXPECT_EQ(0, datetime_sub(&dt1, 60));
  EXPECT_EQ_DATETIME(dt, dt1)

  dt = {.year = 20, .month = 1, .day = 31, .hour = 0, .minute = 0, .second = 0, .microsecond = 0};
  dt1 = {.year = 20, .month = 1, .day = 31, .hour = 2, .minute = 6, .second = 0, .microsecond = 0};
  EXPECT_EQ(0, datetime_sub(&dt1, 2 * 60 * 60 + 6 * 60));
  EXPECT_EQ_DATETIME(dt, dt1)

  dt = {.year = 20, .month = 1, .day = 31, .hour = 0, .minute = 0, .second = 0, .microsecond = 0};
  dt1 = {.year = 20, .month = 2, .day = 3, .hour = 2, .minute = 6, .second = 0, .microsecond = 0};
  EXPECT_EQ(0, datetime_sub(&dt1, 3 * 24 * 60 * 60 + 2 * 60 * 60 + 6 * 60));
  EXPECT_EQ_DATETIME(dt, dt1)

  dt = {.year = 20, .month = 1, .day = 31, .hour = 0, .minute = 0, .second = 0, .microsecond = 0};
  dt1 = {.year = 20, .month = 1, .day = 31, .hour = 1, .minute = 0, .second = 0, .microsecond = 0};
  EXPECT_EQ(0, datetime_sub(&dt1, 60 * 60));
  EXPECT_EQ_DATETIME(dt, dt1)

  dt = {.year = 20, .month = 1, .day = 31, .hour = 0, .minute = 0, .second = 0, .microsecond = 0};
  dt1 = {.year = 20, .month = 2, .day = 1, .hour = 0, .minute = 0, .second = 0, .microsecond = 0};
  EXPECT_EQ(0, datetime_sub(&dt1, 24 * 60 * 60));
  EXPECT_EQ_DATETIME(dt, dt1)

  dt = {.year = 20, .month = 2, .day = 28, .hour = 0, .minute = 0, .second = 0, .microsecond = 0};
  dt1 = {.year = 20, .month = 2, .day = 29, .hour = 0, .minute = 0, .second = 0, .microsecond = 0};
  EXPECT_EQ(0, datetime_sub(&dt1, 24 * 60 * 60));
  EXPECT_EQ_DATETIME(dt, dt1)

  dt = {.year = 21, .month = 2, .day = 28, .hour = 0, .minute = 0, .second = 0, .microsecond = 0};
  dt1 = {.year = 21, .month = 3, .day = 1, .hour = 0, .minute = 0, .second = 0, .microsecond = 0};
  EXPECT_EQ(0, datetime_sub(&dt1, 24 * 60 * 60));
  EXPECT_EQ_DATETIME(dt, dt1)

  dt = {.year = 20, .month = 3, .day = 30, .hour = 0, .minute = 0, .second = 0, .microsecond = 0};
  dt1 = {.year = 20, .month = 4, .day = 29, .hour = 0, .minute = 0, .second = 0, .microsecond = 0};
  EXPECT_EQ(0, datetime_sub(&dt1, 30 * 24 * 60 * 60));
  EXPECT_EQ_DATETIME(dt, dt1)

  dt = {.year = 20, .month = 4, .day = 30, .hour = 0, .minute = 0, .second = 0, .microsecond = 0};
  dt1 = {.year = 20, .month = 5, .day = 30, .hour = 0, .minute = 0, .second = 0, .microsecond = 0};
  EXPECT_EQ(0, datetime_sub(&dt1, 30 * 24 * 60 * 60));
  EXPECT_EQ_DATETIME(dt, dt1)

  dt = {.year = 20, .month = 4, .day = 30, .hour = 0, .minute = 0, .second = 0, .microsecond = 0};
  dt1 = {.year = 20, .month = 5, .day = 30, .hour = 0, .minute = 0, .second = 5, .microsecond = 0};
  EXPECT_EQ(0, datetime_sub(&dt1, 30 * 24 * 60 * 60 + 5));
  EXPECT_EQ_DATETIME(dt, dt1)

  dt = {.year = 20, .month = 8, .day = 31, .hour = 0, .minute = 0, .second = 0, .microsecond = 0};
  dt1 = {.year = 21, .month = 8, .day = 31, .hour = 0, .minute = 0, .second = 1, .microsecond = 0};
  EXPECT_EQ(0, datetime_sub(&dt1, 365 * 24 * 60 * 60 + 1));
  EXPECT_EQ_DATETIME(dt, dt1)

  dt = {.year = 19, .month = 8, .day = 31, .hour = 0, .minute = 0, .second = 0, .microsecond = 0};
  dt1 = {.year = 20, .month = 8, .day = 30, .hour = 0, .minute = 0, .second = 1, .microsecond = 0};
  EXPECT_EQ(0, datetime_sub(&dt1, 365 * 24 * 60 * 60 + 1));
  EXPECT_EQ_DATETIME(dt, dt1)
}

TEST(datetime_diff, ok)
{
  datetime_t dt = {.year = 20, .month = 6, .day = 2, .hour = 12, .minute = 20, .second = 0, .microsecond = 0};
  datetime_t dt1 = {.year = 20, .month = 6, .day = 2, .hour = 12, .minute = 20, .second = 1, .microsecond = 0};
  EXPECT_EQ(1, datetime_diff(&dt1, &dt));

  dt = {.year = 20, .month = 1, .day = 31, .hour = 0, .minute = 0, .second = 0, .microsecond = 0};
  dt1 = {.year = 20, .month = 1, .day = 31, .hour = 0, .minute = 1, .second = 0, .microsecond = 0};
  EXPECT_EQ(60, datetime_diff(&dt1, &dt));

  dt = {.year = 20, .month = 1, .day = 31, .hour = 0, .minute = 0, .second = 0, .microsecond = 0};
  dt1 = {.year = 20, .month = 1, .day = 31, .hour = 2, .minute = 6, .second = 0, .microsecond = 0};
  EXPECT_EQ(2 * 60 * 60 + 6 * 60, datetime_diff(&dt1, &dt));

  dt = {.year = 20, .month = 1, .day = 31, .hour = 0, .minute = 0, .second = 0, .microsecond = 0};
  dt1 = {.year = 20, .month = 2, .day = 3, .hour = 2, .minute = 6, .second = 0, .microsecond = 0};
  EXPECT_EQ(3 * 24 * 60 * 60 + 2 * 60 * 60 + 6 * 60, datetime_diff(&dt1, &dt));

  dt = {.year = 20, .month = 1, .day = 31, .hour = 0, .minute = 0, .second = 0, .microsecond = 0};
  dt1 = {.year = 20, .month = 1, .day = 31, .hour = 1, .minute = 0, .second = 0, .microsecond = 0};
  EXPECT_EQ(60 * 60, datetime_diff(&dt1, &dt));

  dt = {.year = 20, .month = 1, .day = 31, .hour = 0, .minute = 0, .second = 0, .microsecond = 0};
  dt1 = {.year = 20, .month = 2, .day = 1, .hour = 0, .minute = 0, .second = 0, .microsecond = 0};
  EXPECT_EQ(24 * 60 * 60, datetime_diff(&dt1, &dt));

  dt = {.year = 20, .month = 2, .day = 28, .hour = 0, .minute = 0, .second = 0, .microsecond = 0};
  dt1 = {.year = 20, .month = 2, .day = 29, .hour = 0, .minute = 0, .second = 0, .microsecond = 0};
  EXPECT_EQ(24 * 60 * 60, datetime_diff(&dt1, &dt));

  dt = {.year = 21, .month = 2, .day = 28, .hour = 0, .minute = 0, .second = 0, .microsecond = 0};
  dt1 = {.year = 21, .month = 3, .day = 1, .hour = 0, .minute = 0, .second = 0, .microsecond = 0};
  EXPECT_EQ(24 * 60 * 60, datetime_diff(&dt1, &dt));

  dt = {.year = 20, .month = 3, .day = 30, .hour = 0, .minute = 0, .second = 0, .microsecond = 0};
  dt1 = {.year = 20, .month = 4, .day = 29, .hour = 0, .minute = 0, .second = 0, .microsecond = 0};
  EXPECT_EQ(30 * 24 * 60 * 60, datetime_diff(&dt1, &dt));

  dt = {.year = 20, .month = 4, .day = 30, .hour = 0, .minute = 0, .second = 0, .microsecond = 0};
  dt1 = {.year = 20, .month = 5, .day = 30, .hour = 0, .minute = 0, .second = 0, .microsecond = 0};
  EXPECT_EQ(30 * 24 * 60 * 60, datetime_diff(&dt1, &dt));

  dt = {.year = 20, .month = 4, .day = 30, .hour = 0, .minute = 0, .second = 0, .microsecond = 0};
  dt1 = {.year = 20, .month = 5, .day = 30, .hour = 0, .minute = 0, .second = 5, .microsecond = 0};
  EXPECT_EQ(30 * 24 * 60 * 60 + 5, datetime_diff(&dt1, &dt));

  dt = {.year = 20, .month = 8, .day = 31, .hour = 0, .minute = 0, .second = 0, .microsecond = 0};
  dt1 = {.year = 21, .month = 8, .day = 31, .hour = 0, .minute = 0, .second = 1, .microsecond = 0};
  EXPECT_EQ(365 * 24 * 60 * 60 + 1, datetime_diff(&dt1, &dt));

  dt = {.year = 19, .month = 8, .day = 31, .hour = 0, .minute = 0, .second = 0, .microsecond = 0};
  dt1 = {.year = 20, .month = 8, .day = 30, .hour = 0, .minute = 0, .second = 1, .microsecond = 0};
  EXPECT_EQ(365 * 24 * 60 * 60 + 1, datetime_diff(&dt1, &dt));
}