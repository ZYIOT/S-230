#include <gtest/gtest.h>
#include <gmock/gmock.h>
extern "C"
{
#include "config_bit.c"
}

using namespace testing;

TEST(config_bit_to_index, pass)
{
  EXPECT_EQ(1, config_bit_to_index(0x8000));
  EXPECT_EQ(2, config_bit_to_index(0x4000));
  EXPECT_EQ(8, config_bit_to_index(0x0100));
  EXPECT_EQ(9, config_bit_to_index(0x0080));
  EXPECT_EQ(16, config_bit_to_index(0x01));
  EXPECT_EQ(15, config_bit_to_index(0x03));
}

TEST(config_bit_from_index, pass)
{
  EXPECT_EQ(0x8000, config_bit_from_index(1));
  EXPECT_EQ(0x4000, config_bit_from_index(2));
  EXPECT_EQ(0x0100, config_bit_from_index(8));
  EXPECT_EQ(0x0080, config_bit_from_index(9));
  EXPECT_EQ(0x01, config_bit_from_index(16));
  EXPECT_EQ(0x02, config_bit_from_index(15));
}

TEST(config_bit_1_bits, pass)
{
  uint8_t count = 0;
  uint8_t bits[16] = {0};

  count = config_bit_1_bits(0x8000, bits);
  EXPECT_EQ(1, count);
  uint8_t e1[] = {1};
  EXPECT_THAT(e1, ElementsAreArray(bits, count));

  count = config_bit_1_bits(0x8001, bits);
  EXPECT_EQ(2, count);
  uint8_t e2[] = {1, 16};
  EXPECT_THAT(e2, ElementsAreArray(bits, count));

  count = config_bit_1_bits(0x8301, bits);
  EXPECT_EQ(4, count);
  uint8_t e3[] = {1, 7, 8, 16};
  EXPECT_THAT(e3, ElementsAreArray(bits, count));

  count = config_bit_1_bits(0x83f1, bits);
  EXPECT_EQ(8, count);
  uint8_t e4[] = {1, 7, 8, 9, 10, 11, 12, 16};
  EXPECT_THAT(e4, ElementsAreArray(bits, count));
}

TEST(config_bit_1_bit_count, pass)
{
  EXPECT_EQ(1, config_bit_1_bits(0x8000, NULL));
  EXPECT_EQ(2, config_bit_1_bits(0x8001, NULL));
  EXPECT_EQ(4, config_bit_1_bits(0x8301, NULL));
  EXPECT_EQ(8, config_bit_1_bits(0x83f1, NULL));
  EXPECT_EQ(2, config_bit_1_bits(0x0300, NULL));
  EXPECT_EQ(4, config_bit_1_bits(0x3300, NULL));
  EXPECT_EQ(8, config_bit_1_bits(0x33F0, NULL));
  EXPECT_EQ(16, config_bit_1_bits(0xFFFF, NULL));
  EXPECT_EQ(0, config_bit_1_bits(0, NULL));
}
