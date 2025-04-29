#include <gtest/gtest.h>

extern "C"
{
#include "mean_filter.c"
    mean_filter_t filter;
    double buffer[500];
}

TEST(mean_filter_init, pass)
{
    EXPECT_EQ(-1, mean_filter_init(&filter, buffer, 0));
    EXPECT_EQ(-1, mean_filter_init(&filter, buffer, 1));
    EXPECT_EQ(0, mean_filter_init(&filter, buffer, 50));
}

TEST(mean_filter_add, 5)
{
    double mean = 0;
    EXPECT_EQ(0, mean_filter_init(&filter, buffer, 5));
    EXPECT_EQ(-1, mean_filter_get(&filter, &mean));
    EXPECT_EQ(0, mean_filter_count(&filter));

    mean_filter_add(&filter, 5);
    EXPECT_EQ(1, mean_filter_count(&filter));
    EXPECT_EQ(0, mean_filter_get(&filter, &mean));
    EXPECT_FLOAT_EQ(5, mean);

    mean_filter_add(&filter, 4);
    EXPECT_EQ(2, mean_filter_count(&filter));
    EXPECT_EQ(0, mean_filter_get(&filter, &mean));
    EXPECT_FLOAT_EQ(4.5, mean);

    mean_filter_add(&filter, 3);
    EXPECT_EQ(3, mean_filter_count(&filter));
    EXPECT_EQ(0, mean_filter_get(&filter, &mean));
    EXPECT_FLOAT_EQ(4, mean);

    mean_filter_add(&filter, 2);
    EXPECT_EQ(4, mean_filter_count(&filter));
    EXPECT_EQ(0, mean_filter_get(&filter, &mean));
    EXPECT_FLOAT_EQ(3.5, mean);

    mean_filter_add(&filter, 1);
    EXPECT_EQ(5, mean_filter_count(&filter));
    EXPECT_EQ(0, mean_filter_get(&filter, &mean));
    EXPECT_FLOAT_EQ(3, mean);

    mean_filter_add(&filter, 0);
    EXPECT_EQ(5, mean_filter_count(&filter));
    EXPECT_EQ(0, mean_filter_get(&filter, &mean));
    EXPECT_FLOAT_EQ(2, mean);

    mean_filter_add(&filter, 0);
    EXPECT_EQ(5, mean_filter_count(&filter));
    EXPECT_EQ(0, mean_filter_get(&filter, &mean));
    EXPECT_FLOAT_EQ(1.2, mean);

    mean_filter_add(&filter, 0);
    EXPECT_EQ(5, mean_filter_count(&filter));
    EXPECT_EQ(0, mean_filter_get(&filter, &mean));
    EXPECT_FLOAT_EQ(0.6, mean);

    mean_filter_add(&filter, 0);
    EXPECT_EQ(5, mean_filter_count(&filter));
    EXPECT_EQ(0, mean_filter_get(&filter, &mean));
    EXPECT_FLOAT_EQ(0.2, mean);

    mean_filter_add(&filter, 0);
    EXPECT_EQ(5, mean_filter_count(&filter));
    EXPECT_EQ(0, mean_filter_get(&filter, &mean));
    EXPECT_FLOAT_EQ(0, mean);

    mean_filter_add(&filter, 5);
    EXPECT_EQ(5, mean_filter_count(&filter));
    EXPECT_EQ(0, mean_filter_get(&filter, &mean));
    EXPECT_FLOAT_EQ(1, mean);

    mean_filter_add(&filter, 0);
    EXPECT_EQ(5, mean_filter_count(&filter));
    EXPECT_EQ(0, mean_filter_get(&filter, &mean));
    EXPECT_FLOAT_EQ(1, mean);

    mean_filter_reset(&filter);
    EXPECT_EQ(0, mean_filter_count(&filter));
    EXPECT_EQ(-1, mean_filter_get(&filter, &mean));
}

TEST(mean_filter_out, 5)
{
    double mean = 0;
    EXPECT_EQ(0, mean_filter_init(&filter, buffer, 5));
    EXPECT_EQ(-1, mean_filter_out(&filter));
    EXPECT_EQ(0, mean_filter_count(&filter));

    mean_filter_add(&filter, 5);
    EXPECT_EQ(1, mean_filter_count(&filter));
    EXPECT_EQ(0, mean_filter_get(&filter, &mean));
    EXPECT_FLOAT_EQ(5, mean);
    EXPECT_EQ(0, mean_filter_out(&filter));
    EXPECT_EQ(0, mean_filter_count(&filter));
    EXPECT_EQ(-1, mean_filter_get(&filter, &mean));

    mean_filter_add(&filter, 4);
    EXPECT_EQ(1, mean_filter_count(&filter));
    EXPECT_EQ(0, mean_filter_get(&filter, &mean));
    EXPECT_FLOAT_EQ(4, mean);

    mean_filter_add(&filter, 3);
    EXPECT_EQ(2, mean_filter_count(&filter));
    EXPECT_EQ(0, mean_filter_get(&filter, &mean));
    EXPECT_FLOAT_EQ(3.5, mean);
    EXPECT_EQ(0, mean_filter_out(&filter));
    EXPECT_EQ(1, mean_filter_count(&filter));
    EXPECT_EQ(0, mean_filter_get(&filter, &mean));
    EXPECT_FLOAT_EQ(3, mean);
    EXPECT_EQ(0, mean_filter_out(&filter));
    EXPECT_EQ(0, mean_filter_count(&filter));
    EXPECT_EQ(-1, mean_filter_get(&filter, &mean));

    EXPECT_EQ(-1, mean_filter_out(&filter));
    EXPECT_EQ(-1, mean_filter_out(&filter));
    EXPECT_EQ(-1, mean_filter_out(&filter));
    EXPECT_EQ(-1, mean_filter_out(&filter));
    EXPECT_EQ(-1, mean_filter_out(&filter));
    EXPECT_EQ(-1, mean_filter_out(&filter));
    EXPECT_EQ(-1, mean_filter_out(&filter));
    EXPECT_EQ(-1, mean_filter_out(&filter));
    EXPECT_EQ(0, mean_filter_count(&filter));

    mean_filter_add(&filter, 2);
    mean_filter_add(&filter, -1);
    EXPECT_EQ(2, mean_filter_count(&filter));
    EXPECT_EQ(0, mean_filter_get(&filter, &mean));
    EXPECT_FLOAT_EQ(0.5, mean);

    mean_filter_add(&filter, -100);
    EXPECT_EQ(0, mean_filter_get(&filter, &mean));
    EXPECT_FLOAT_EQ(-33, mean);

    mean_filter_add(&filter, -100);
    EXPECT_EQ(0, mean_filter_get(&filter, &mean));
    EXPECT_FLOAT_EQ(-49.75, mean);

    mean_filter_add(&filter, -100);
    EXPECT_EQ(0, mean_filter_get(&filter, &mean));
    EXPECT_FLOAT_EQ(-59.8, mean);

    mean_filter_add(&filter, -100);
    EXPECT_EQ(0, mean_filter_get(&filter, &mean));
    EXPECT_FLOAT_EQ(-80.2, mean);

    mean_filter_add(&filter, -100);
    EXPECT_EQ(0, mean_filter_get(&filter, &mean));
    EXPECT_FLOAT_EQ(-100, mean);
}

TEST(mean_filter_count, pass)
{
    double mean = 0;
    for (int j = 2; j < 500; j++)
    {
        EXPECT_EQ(0, mean_filter_init(&filter, buffer, j));
        for (int i = 1; i < 1000; i++)
        {
            mean_filter_add(&filter, 1.234);
            if (i >= j)
            {
                EXPECT_EQ(j, mean_filter_count(&filter));
            }
            else
            {
                EXPECT_EQ(i, mean_filter_count(&filter));
            }
            EXPECT_EQ(0, mean_filter_get(&filter, &mean));
            EXPECT_FLOAT_EQ(1.234, mean);
        }
    }
}

TEST(mean_filter_out, pass)
{
    double mean = 0;
    for (int j = 2; j < 500; j++)
    {
        EXPECT_EQ(0, mean_filter_init(&filter, buffer, j));
        for (int i = 1; i < 1000; i++)
        {
            mean_filter_add(&filter, 1.234);
            EXPECT_EQ(0, mean_filter_out(&filter));

            EXPECT_EQ(0, mean_filter_count(&filter));
            EXPECT_EQ(-1, mean_filter_get(&filter, &mean));
        }
    }
}
