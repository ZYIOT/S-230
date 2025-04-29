// #include <gtest/gtest.h>

// extern "C"
// {
// #include "app_relay.c"
// }

// TEST(time_job_compare, pass)
// {
//     APP_CONFIG_Time_t time = {
//         .year = 2,
//         .month = 2,
//         .day = 2,
//         .hour = 2,
//         .minute = 12,
//         .second = 2};

//     EXPECT_EQ(0, time_job_compare(&time, 2, 12));
//     EXPECT_EQ(1, time_job_compare(&time, 2, 10));
//     EXPECT_EQ(-1, time_job_compare(&time, 3, 1));
//     EXPECT_EQ(1, time_job_compare(&time, 1, 15));
//     EXPECT_EQ(-1, time_job_compare(&time, 2, 13));
//     time.second = 12;
//     EXPECT_EQ(0, time_job_compare(&time, 2, 12));
//     time.day = 12;
//     EXPECT_EQ(0, time_job_compare(&time, 2, 12));
//     time.month = 12;
//     EXPECT_EQ(0, time_job_compare(&time, 2, 12));
//     time.year = 12;
//     EXPECT_EQ(0, time_job_compare(&time, 2, 12));
// }

// TEST(time_job_in_range, in_one_day)
// {
//     APP_CONFIG_TimesJob_t times_job = {
//         .type = 0x81,
//         .startHour = 2,
//         .startMinute = 2,
//         .endHour = 2,
//         .endMinute = 3};
//     APP_CONFIG_Time_t time = {
//         .year = 2,
//         .month = 2,
//         .day = 2,
//         .hour = 2,
//         .minute = 12,
//         .second = 2};
//     EXPECT_EQ(0, time_job_in_range(&time, &times_job));
//     times_job.endMinute = 12;
//     EXPECT_EQ(0, time_job_in_range(&time, &times_job));
//     times_job.endMinute = 13;
//     EXPECT_EQ(1, time_job_in_range(&time, &times_job));

//     time.minute = 1;
//     EXPECT_EQ(0, time_job_in_range(&time, &times_job));
//     time.hour = 3;
//     EXPECT_EQ(0, time_job_in_range(&time, &times_job));

//     time.hour = 3;
//     time.minute = 1;
//     times_job.endHour = 3;
//     times_job.endMinute = 2;
//     EXPECT_EQ(1, time_job_in_range(&time, &times_job));
//     time.minute = 2;
//     EXPECT_EQ(0, time_job_in_range(&time, &times_job));

//     times_job.startHour = 2;
//     times_job.startMinute = 2;
//     time.hour = 1;
//     time.minute = 1;
//     EXPECT_EQ(0, time_job_in_range(&time, &times_job));
//     time.hour = 2;
//     EXPECT_EQ(0, time_job_in_range(&time, &times_job));
//     time.minute = 2;
//     EXPECT_EQ(1, time_job_in_range(&time, &times_job));
// }

// // ¿çÌì
// TEST(time_job_in_range, over_day)
// {
//     APP_CONFIG_TimesJob_t times_job = {
//         .type = 0x81,
//         .startHour = 21,
//         .startMinute = 2,
//         .endHour = 2,
//         .endMinute = 3};
//     APP_CONFIG_Time_t time = {
//         .year = 2,
//         .month = 2,
//         .day = 2,
//         .hour = 2,
//         .minute = 12,
//         .second = 2};

//     EXPECT_EQ(0, time_job_in_range(&time, &times_job));
//     times_job.endMinute = 12;
//     EXPECT_EQ(0, time_job_in_range(&time, &times_job));
//     times_job.endMinute = 13;
//     EXPECT_EQ(1, time_job_in_range(&time, &times_job));

//     time.minute = 1;
//     EXPECT_EQ(1, time_job_in_range(&time, &times_job));
//     time.hour = 3;
//     EXPECT_EQ(0, time_job_in_range(&time, &times_job));
//     time.hour = 21;
//     time.minute = 2;
//     EXPECT_EQ(1, time_job_in_range(&time, &times_job));

//     time.hour = 21;
//     time.minute = 1;
//     EXPECT_EQ(0, time_job_in_range(&time, &times_job));

//     time.hour = 22;
//     time.minute = 1;
//     EXPECT_EQ(1, time_job_in_range(&time, &times_job));

//     time.hour = 0;
//     time.minute = 1;
//     EXPECT_EQ(1, time_job_in_range(&time, &times_job));

//     time.hour = 0;
//     time.minute = 0;
//     EXPECT_EQ(1, time_job_in_range(&time, &times_job));
//     time.hour = 23;
//     time.minute = 59;
//     EXPECT_EQ(1, time_job_in_range(&time, &times_job));
//     time.hour = 1;
//     time.minute = 59;
//     EXPECT_EQ(1, time_job_in_range(&time, &times_job));

//     time.hour = 3;
//     time.minute = 1;
//     times_job.endHour = 3;
//     times_job.endMinute = 2;
//     EXPECT_EQ(1, time_job_in_range(&time, &times_job));
//     time.minute = 2;
//     EXPECT_EQ(0, time_job_in_range(&time, &times_job));

//     times_job.endHour = 2;
//     times_job.endMinute = 3;

//     time.hour = 1;
//     time.minute = 1;
//     EXPECT_EQ(1, time_job_in_range(&time, &times_job));
//     time.hour = 2;
//     time.minute = 3;
//     EXPECT_EQ(0, time_job_in_range(&time, &times_job));
//     time.minute = 2;
//     EXPECT_EQ(1, time_job_in_range(&time, &times_job));
// }

// TEST(valid_time_job, pass)
// {
//     APP_CONFIG_TimesJob_t times_job = {
//         .type = 0x81,
//         .startHour = 1,
//         .startMinute = 2,
//         .endHour = 12,
//         .endMinute = 3};
//     APP_CONFIG_Time_t time = {
//         .year = 2,
//         .month = 2,
//         .day = 2,
//         .hour = 2,
//         .minute = 12,
//         .second = 2};
//     EXPECT_EQ(1, valid_time_job(&time, &times_job));
//     times_job.type = 0x83;
//     EXPECT_EQ(0, valid_time_job(&time, &times_job));
//     times_job.type = 1;
//     EXPECT_EQ(0, valid_time_job(&time, &times_job));

//     times_job.type = 0x81;
//     EXPECT_EQ(1, valid_time_job(&time, &times_job));
//     time.hour = 14;
//     EXPECT_EQ(0, valid_time_job(&time, &times_job));
// }

// TEST(datetime_in_range, pass)
// {
// }
