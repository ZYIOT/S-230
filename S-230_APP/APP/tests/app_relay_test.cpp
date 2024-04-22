// #include <gtest/gtest.h>

// extern "C"
// {
// #include "app_relay.c"
// }

// TEST(time_job_compare, pass)
// {
//     app_config_time_t time = {
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
//     app_config_times_job_t times_job = {
//         .type = 0x81,
//         .start_hour = 2,
//         .start_minute = 2,
//         .end_hour = 2,
//         .end_minute = 3};
//     app_config_time_t time = {
//         .year = 2,
//         .month = 2,
//         .day = 2,
//         .hour = 2,
//         .minute = 12,
//         .second = 2};
//     EXPECT_EQ(0, time_job_in_range(&time, &times_job));
//     times_job.end_minute = 12;
//     EXPECT_EQ(0, time_job_in_range(&time, &times_job));
//     times_job.end_minute = 13;
//     EXPECT_EQ(1, time_job_in_range(&time, &times_job));

//     time.minute = 1;
//     EXPECT_EQ(0, time_job_in_range(&time, &times_job));
//     time.hour = 3;
//     EXPECT_EQ(0, time_job_in_range(&time, &times_job));

//     time.hour = 3;
//     time.minute = 1;
//     times_job.end_hour = 3;
//     times_job.end_minute = 2;
//     EXPECT_EQ(1, time_job_in_range(&time, &times_job));
//     time.minute = 2;
//     EXPECT_EQ(0, time_job_in_range(&time, &times_job));

//     times_job.start_hour = 2;
//     times_job.start_minute = 2;
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
//     app_config_times_job_t times_job = {
//         .type = 0x81,
//         .start_hour = 21,
//         .start_minute = 2,
//         .end_hour = 2,
//         .end_minute = 3};
//     app_config_time_t time = {
//         .year = 2,
//         .month = 2,
//         .day = 2,
//         .hour = 2,
//         .minute = 12,
//         .second = 2};

//     EXPECT_EQ(0, time_job_in_range(&time, &times_job));
//     times_job.end_minute = 12;
//     EXPECT_EQ(0, time_job_in_range(&time, &times_job));
//     times_job.end_minute = 13;
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
//     times_job.end_hour = 3;
//     times_job.end_minute = 2;
//     EXPECT_EQ(1, time_job_in_range(&time, &times_job));
//     time.minute = 2;
//     EXPECT_EQ(0, time_job_in_range(&time, &times_job));

//     times_job.end_hour = 2;
//     times_job.end_minute = 3;

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
//     app_config_times_job_t times_job = {
//         .type = 0x81,
//         .start_hour = 1,
//         .start_minute = 2,
//         .end_hour = 12,
//         .end_minute = 3};
//     app_config_time_t time = {
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
