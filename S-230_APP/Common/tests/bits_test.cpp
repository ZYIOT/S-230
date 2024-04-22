#include <gtest/gtest.h>

extern "C"
{
#include "bits.c"

#define __CHECK_RESET 0
#define __CHECK_SET 1
#define __CHECK_REVERSE 2

#define __PRINT_ERROR_DATA() \
  printf("\nERROR size:%d, type:%d, origin:%d, after:%d, start:%d, len:%d\n", size, check_type, data, actual, start, len)

  static int _bits_check(uint8_t size, uint8_t check_type, uint32_t data, uint32_t actual, uint8_t start, uint8_t len)
  {

    uint8_t bytes[32] = {0};
    uint8_t sets[32] = {0};
    uint32_t mask = 0x01, i = 0;
    for (i = 0; i < size; i++)
    {
      bytes[i] = (data & mask) > 0 ? 1 : 0;
      mask = mask << 1;
    }
    mask = 0x01;
    for (i = 0; i < size; i++)
    {
      sets[i] = (actual & mask) > 0 ? 1 : 0;
      mask = mask << 1;
    }
    for (i = 0; i < size; i++)
    {

      if (i >= start && i < start + len)
      {
        if (check_type == __CHECK_RESET && sets[i] > 0)
        {
          __PRINT_ERROR_DATA();
          return 0;
        }
        if (check_type == __CHECK_SET && sets[i] != 1)
        {
          __PRINT_ERROR_DATA();
          return 0;
        }
        if (check_type == __CHECK_REVERSE && bytes[i] == sets[i])
        {
          __PRINT_ERROR_DATA();
          return 0;
        }
      }
      else
      {
        if (bytes[i] != sets[i])
        {
          __PRINT_ERROR_DATA();
          return 0;
        }
      }
    }
    return 1;
  }
}

TEST(bits_uint8_t_set, pass)
{
  uint8_t v = 0b00000000;
  EXPECT_EQ(0b00000110, bits_uint8_t_set(v, 1, 2));
  EXPECT_EQ(0b00000011, bits_uint8_t_set(v, 0, 2));
  EXPECT_EQ(0b00000001, bits_uint8_t_set(v, 0, 1));
  EXPECT_EQ(0b00000000, bits_uint8_t_set(v, 1, 0));
  EXPECT_EQ(0b11000000, bits_uint8_t_set(v, 6, 5));
  EXPECT_EQ(0b00000000, bits_uint8_t_set(v, 10, 2));
  v = 0b00011000;
  EXPECT_EQ(0b00011110, bits_uint8_t_set(v, 1, 2));
  EXPECT_EQ(0b00011011, bits_uint8_t_set(v, 0, 2));
  EXPECT_EQ(0b00011001, bits_uint8_t_set(v, 0, 1));
  EXPECT_EQ(0b00011000, bits_uint8_t_set(v, 1, 0));
  EXPECT_EQ(0b11011000, bits_uint8_t_set(v, 6, 5));
  EXPECT_EQ(0b00011000, bits_uint8_t_set(v, 10, 2));
  v = 0b00011001;
  EXPECT_EQ(0b00011111, bits_uint8_t_set(v, 1, 2));
  EXPECT_EQ(0b00011011, bits_uint8_t_set(v, 0, 2));
  EXPECT_EQ(0b00011001, bits_uint8_t_set(v, 0, 1));
  EXPECT_EQ(0b00011001, bits_uint8_t_set(v, 1, 0));
  EXPECT_EQ(0b11011001, bits_uint8_t_set(v, 6, 5));
  EXPECT_EQ(0b00011001, bits_uint8_t_set(v, 10, 2));
  v = 0b10011001;
  EXPECT_EQ(0b10011111, bits_uint8_t_set(v, 1, 2));
  EXPECT_EQ(0b10011011, bits_uint8_t_set(v, 0, 2));
  EXPECT_EQ(0b10011001, bits_uint8_t_set(v, 0, 1));
  EXPECT_EQ(0b10011001, bits_uint8_t_set(v, 1, 0));
  EXPECT_EQ(0b11011001, bits_uint8_t_set(v, 6, 5));
  EXPECT_EQ(0b10011001, bits_uint8_t_set(v, 10, 2));
  EXPECT_EQ(1, _bits_check(8, __CHECK_SET, 0b10011001, 0b11011001, 6, 5));
  uint8_t s = 0;
  uint8_t l = 0;
  for (int i = 0; i < 255; i++)
  {
    v = rand() % 255;
    s = rand() % 8;
    l = rand() % 8;
    EXPECT_EQ(1, _bits_check(8, __CHECK_SET, v, bits_uint8_t_set(v, s, l), s, l));
  }
}

TEST(bits_uint8_t_reset, pass)
{
  uint8_t v = 0b00000000;
  EXPECT_EQ(0b00000000, bits_uint8_t_reset(v, 1, 2));
  EXPECT_EQ(0b00000000, bits_uint8_t_reset(v, 0, 2));
  EXPECT_EQ(0b00000000, bits_uint8_t_reset(v, 0, 1));
  EXPECT_EQ(0b00000000, bits_uint8_t_reset(v, 1, 0));
  EXPECT_EQ(0b00000000, bits_uint8_t_reset(v, 6, 5));
  EXPECT_EQ(0b00000000, bits_uint8_t_reset(v, 10, 2));
  v = 0b00011011;
  EXPECT_EQ(0b00011001, bits_uint8_t_reset(v, 1, 2));
  EXPECT_EQ(0b00011000, bits_uint8_t_reset(v, 0, 2));
  EXPECT_EQ(0b00011010, bits_uint8_t_reset(v, 0, 1));
  EXPECT_EQ(0b00011011, bits_uint8_t_reset(v, 1, 0));
  EXPECT_EQ(0b00001011, bits_uint8_t_reset(v, 4, 5));
  EXPECT_EQ(0b00011011, bits_uint8_t_reset(v, 10, 2));
  v = 0b00011100;
  EXPECT_EQ(0b00011000, bits_uint8_t_reset(v, 1, 2));
  EXPECT_EQ(0b00011100, bits_uint8_t_reset(v, 0, 2));
  EXPECT_EQ(0b00011100, bits_uint8_t_reset(v, 0, 1));
  EXPECT_EQ(0b00011100, bits_uint8_t_reset(v, 1, 0));
  EXPECT_EQ(0b00001100, bits_uint8_t_reset(v, 4, 5));
  EXPECT_EQ(0b00011100, bits_uint8_t_reset(v, 10, 2));
  v = 0b00011001;
  EXPECT_EQ(0b00011001, bits_uint8_t_reset(v, 1, 2));
  EXPECT_EQ(0b00011000, bits_uint8_t_reset(v, 0, 2));
  EXPECT_EQ(0b00011000, bits_uint8_t_reset(v, 0, 1));
  EXPECT_EQ(0b00011001, bits_uint8_t_reset(v, 1, 0));
  EXPECT_EQ(0b00001001, bits_uint8_t_reset(v, 4, 5));
  EXPECT_EQ(0b00011001, bits_uint8_t_reset(v, 10, 2));
  v = 0b10011001;
  EXPECT_EQ(0b10011001, bits_uint8_t_reset(v, 1, 2));
  EXPECT_EQ(0b10011000, bits_uint8_t_reset(v, 0, 2));
  EXPECT_EQ(0b10011000, bits_uint8_t_reset(v, 0, 1));
  EXPECT_EQ(0b10011001, bits_uint8_t_reset(v, 1, 0));
  EXPECT_EQ(0b00001001, bits_uint8_t_reset(v, 4, 5));
  EXPECT_EQ(0b10011001, bits_uint8_t_reset(v, 10, 2));

  uint8_t s = 0;
  uint8_t l = 0;
  for (int i = 0; i < 255; i++)
  {
    v = rand() % 255;
    s = rand() % 8;
    l = rand() % 8;
    EXPECT_EQ(1, _bits_check(8, __CHECK_RESET, v, bits_uint8_t_reset(v, s, l), s, l));
  }
}

TEST(bits_uint8_t_reverse, pass)
{
  uint8_t v = 0b00000000;
  EXPECT_EQ(0b00000110, bits_uint8_t_reverse(v, 1, 2));
  EXPECT_EQ(0b00000011, bits_uint8_t_reverse(v, 0, 2));
  EXPECT_EQ(0b00000001, bits_uint8_t_reverse(v, 0, 1));
  EXPECT_EQ(0b00000000, bits_uint8_t_reverse(v, 1, 0));
  EXPECT_EQ(0b11000000, bits_uint8_t_reverse(v, 6, 5));
  EXPECT_EQ(0b00000000, bits_uint8_t_reverse(v, 10, 2));
  v = 0b00011011;
  EXPECT_EQ(0b00011101, bits_uint8_t_reverse(v, 1, 2));
  EXPECT_EQ(0b00011000, bits_uint8_t_reverse(v, 0, 2));
  EXPECT_EQ(0b00011010, bits_uint8_t_reverse(v, 0, 1));
  EXPECT_EQ(0b00011011, bits_uint8_t_reverse(v, 1, 0));
  EXPECT_EQ(0b11101011, bits_uint8_t_reverse(v, 4, 5));
  EXPECT_EQ(0b00011011, bits_uint8_t_reverse(v, 10, 2));
  v = 0b00011100;
  EXPECT_EQ(0b00011010, bits_uint8_t_reverse(v, 1, 2));
  EXPECT_EQ(0b00011111, bits_uint8_t_reverse(v, 0, 2));
  EXPECT_EQ(0b00011101, bits_uint8_t_reverse(v, 0, 1));
  EXPECT_EQ(0b00011100, bits_uint8_t_reverse(v, 1, 0));
  EXPECT_EQ(0b11101100, bits_uint8_t_reverse(v, 4, 5));
  EXPECT_EQ(0b00011100, bits_uint8_t_reverse(v, 10, 2));
  v = 0b00011001;
  EXPECT_EQ(0b00011111, bits_uint8_t_reverse(v, 1, 2));
  EXPECT_EQ(0b00011010, bits_uint8_t_reverse(v, 0, 2));
  EXPECT_EQ(0b00011000, bits_uint8_t_reverse(v, 0, 1));
  EXPECT_EQ(0b00011001, bits_uint8_t_reverse(v, 1, 0));
  EXPECT_EQ(0b11101001, bits_uint8_t_reverse(v, 4, 5));
  EXPECT_EQ(0b00011001, bits_uint8_t_reverse(v, 10, 2));
  v = 0b10011001;
  EXPECT_EQ(0b10011111, bits_uint8_t_reverse(v, 1, 2));
  EXPECT_EQ(0b10011010, bits_uint8_t_reverse(v, 0, 2));
  EXPECT_EQ(0b10011000, bits_uint8_t_reverse(v, 0, 1));
  EXPECT_EQ(0b10011001, bits_uint8_t_reverse(v, 1, 0));
  EXPECT_EQ(0b01101001, bits_uint8_t_reverse(v, 4, 5));
  EXPECT_EQ(0b10011001, bits_uint8_t_reverse(v, 10, 2));
  uint8_t s = 0;
  uint8_t l = 0;
  for (int i = 0; i < 255; i++)
  {
    v = rand() % 255;
    s = rand() % 8;
    l = rand() % 8;
    EXPECT_EQ(1, _bits_check(8, __CHECK_REVERSE, v, bits_uint8_t_reverse(v, s, l), s, l));
  }
}

TEST(bits_uint16_t_set, pass)
{
  uint16_t v = 0b0000000000000000;
  EXPECT_EQ(0b0000000000000110, bits_uint16_t_set(v, 1, 2));
  EXPECT_EQ(0b0000000000000011, bits_uint16_t_set(v, 0, 2));
  EXPECT_EQ(0b0000000000000001, bits_uint16_t_set(v, 0, 1));
  EXPECT_EQ(0b0000000000000000, bits_uint16_t_set(v, 1, 0));
  EXPECT_EQ(0b0000011111000000, bits_uint16_t_set(v, 6, 5));
  EXPECT_EQ(0b0000110000000000, bits_uint16_t_set(v, 10, 2));
  v = 0b0000000000011000;
  EXPECT_EQ(0b0000000000011110, bits_uint16_t_set(v, 1, 2));
  EXPECT_EQ(0b0000000000011011, bits_uint16_t_set(v, 0, 2));
  EXPECT_EQ(0b0000000000011001, bits_uint16_t_set(v, 0, 1));
  EXPECT_EQ(0b0000000000011000, bits_uint16_t_set(v, 1, 0));
  EXPECT_EQ(0b0000011111011000, bits_uint16_t_set(v, 6, 5));
  EXPECT_EQ(0b0000110000011000, bits_uint16_t_set(v, 10, 2));
  v = 0b0000000000011001;
  EXPECT_EQ(0b0000000000011111, bits_uint16_t_set(v, 1, 2));
  EXPECT_EQ(0b0000000000011011, bits_uint16_t_set(v, 0, 2));
  EXPECT_EQ(0b0000000000011001, bits_uint16_t_set(v, 0, 1));
  EXPECT_EQ(0b0000000000011001, bits_uint16_t_set(v, 1, 0));
  EXPECT_EQ(0b0000011111011001, bits_uint16_t_set(v, 6, 5));
  EXPECT_EQ(0b0000110000011001, bits_uint16_t_set(v, 10, 2));
  v = 0b0000000010011001;
  EXPECT_EQ(0b0000000010011111, bits_uint16_t_set(v, 1, 2));
  EXPECT_EQ(0b0000000010011011, bits_uint16_t_set(v, 0, 2));
  EXPECT_EQ(0b0000000010011001, bits_uint16_t_set(v, 0, 1));
  EXPECT_EQ(0b0000000010011001, bits_uint16_t_set(v, 1, 0));
  EXPECT_EQ(0b0000011111011001, bits_uint16_t_set(v, 6, 5));
  EXPECT_EQ(0b0000110010011001, bits_uint16_t_set(v, 10, 2));

  uint8_t s = 0;
  uint8_t l = 0;
  for (int i = 0; i < 500; i++)
  {
    v = rand() % (0xffff);
    s = rand() % 16;
    l = rand() % 16;
    EXPECT_EQ(1, _bits_check(16, __CHECK_SET, v, bits_uint16_t_set(v, s, l), s, l));
  }
}

TEST(bits_uint16_t_reset, pass)
{
  uint16_t v = 0b0000000000000000;
  EXPECT_EQ(0b0000000000000000, bits_uint16_t_reset(v, 1, 2));
  EXPECT_EQ(0b0000000000000000, bits_uint16_t_reset(v, 0, 2));
  EXPECT_EQ(0b0000000000000000, bits_uint16_t_reset(v, 0, 1));
  EXPECT_EQ(0b0000000000000000, bits_uint16_t_reset(v, 1, 0));
  EXPECT_EQ(0b0000000000000000, bits_uint16_t_reset(v, 6, 5));
  EXPECT_EQ(0b0000000000000000, bits_uint16_t_reset(v, 10, 2));
  v = 0b0000000000011011;
  EXPECT_EQ(0b0000000000011001, bits_uint16_t_reset(v, 1, 2));
  EXPECT_EQ(0b0000000000011000, bits_uint16_t_reset(v, 0, 2));
  EXPECT_EQ(0b0000000000011010, bits_uint16_t_reset(v, 0, 1));
  EXPECT_EQ(0b0000000000011011, bits_uint16_t_reset(v, 1, 0));
  EXPECT_EQ(0b0000000000001011, bits_uint16_t_reset(v, 4, 5));
  EXPECT_EQ(0b0000000000011011, bits_uint16_t_reset(v, 10, 2));
  v = 0b0000000000011100;
  EXPECT_EQ(0b0000000000011000, bits_uint16_t_reset(v, 1, 2));
  EXPECT_EQ(0b0000000000011100, bits_uint16_t_reset(v, 0, 2));
  EXPECT_EQ(0b0000000000011100, bits_uint16_t_reset(v, 0, 1));
  EXPECT_EQ(0b0000000000011100, bits_uint16_t_reset(v, 1, 0));
  EXPECT_EQ(0b0000000000001100, bits_uint16_t_reset(v, 4, 5));
  EXPECT_EQ(0b0000000000011100, bits_uint16_t_reset(v, 10, 2));
  v = 0b0000000000011001;
  EXPECT_EQ(0b0000000000011001, bits_uint16_t_reset(v, 1, 2));
  EXPECT_EQ(0b0000000000011000, bits_uint16_t_reset(v, 0, 2));
  EXPECT_EQ(0b0000000000011000, bits_uint16_t_reset(v, 0, 1));
  EXPECT_EQ(0b0000000000011001, bits_uint16_t_reset(v, 1, 0));
  EXPECT_EQ(0b0000000000001001, bits_uint16_t_reset(v, 4, 5));
  EXPECT_EQ(0b0000000000011001, bits_uint16_t_reset(v, 10, 2));
  v = 0b0000000010011001;
  EXPECT_EQ(0b0000000010011001, bits_uint16_t_reset(v, 1, 2));
  EXPECT_EQ(0b0000000010011000, bits_uint16_t_reset(v, 0, 2));
  EXPECT_EQ(0b0000000010011000, bits_uint16_t_reset(v, 0, 1));
  EXPECT_EQ(0b0000000010011001, bits_uint16_t_reset(v, 1, 0));
  EXPECT_EQ(0b0000000000001001, bits_uint16_t_reset(v, 4, 5));
  EXPECT_EQ(0b0000000010011001, bits_uint16_t_reset(v, 10, 2));
  uint8_t s = 0;
  uint8_t l = 0;
  for (int i = 0; i < 500; i++)
  {
    v = rand() % (0xffff);
    s = rand() % 16;
    l = rand() % 16;
    EXPECT_EQ(1, _bits_check(16, __CHECK_RESET, v, bits_uint16_t_reset(v, s, l), s, l));
  }
}

TEST(bits_uint16_t_reverse, pass)
{
  uint16_t v = 0b0000000000000000;
  EXPECT_EQ(0b0000000000000110, bits_uint16_t_reverse(v, 1, 2));
  EXPECT_EQ(0b0000000000000011, bits_uint16_t_reverse(v, 0, 2));
  EXPECT_EQ(0b0000000000000001, bits_uint16_t_reverse(v, 0, 1));
  EXPECT_EQ(0b0000000000000000, bits_uint16_t_reverse(v, 1, 0));
  EXPECT_EQ(0b0000011111000000, bits_uint16_t_reverse(v, 6, 5));
  EXPECT_EQ(0b0000110000000000, bits_uint16_t_reverse(v, 10, 2));
  v = 0b0000000000011011;
  EXPECT_EQ(0b0000000000011101, bits_uint16_t_reverse(v, 1, 2));
  EXPECT_EQ(0b0000000000011000, bits_uint16_t_reverse(v, 0, 2));
  EXPECT_EQ(0b0000000000011010, bits_uint16_t_reverse(v, 0, 1));
  EXPECT_EQ(0b0000000000011011, bits_uint16_t_reverse(v, 1, 0));
  EXPECT_EQ(0b0000000111101011, bits_uint16_t_reverse(v, 4, 5));
  EXPECT_EQ(0b0000110000011011, bits_uint16_t_reverse(v, 10, 2));
  v = 0b0000000000011100;
  EXPECT_EQ(0b0000000000011010, bits_uint16_t_reverse(v, 1, 2));
  EXPECT_EQ(0b0000000000011111, bits_uint16_t_reverse(v, 0, 2));
  EXPECT_EQ(0b0000000000011101, bits_uint16_t_reverse(v, 0, 1));
  EXPECT_EQ(0b0000000000011100, bits_uint16_t_reverse(v, 1, 0));
  EXPECT_EQ(0b0000000111101100, bits_uint16_t_reverse(v, 4, 5));
  EXPECT_EQ(0b0000110000011100, bits_uint16_t_reverse(v, 10, 2));
  v = 0b0000000000011001;
  EXPECT_EQ(0b0000000000011111, bits_uint16_t_reverse(v, 1, 2));
  EXPECT_EQ(0b0000000000011010, bits_uint16_t_reverse(v, 0, 2));
  EXPECT_EQ(0b0000000000011000, bits_uint16_t_reverse(v, 0, 1));
  EXPECT_EQ(0b0000000000011001, bits_uint16_t_reverse(v, 1, 0));
  EXPECT_EQ(0b0000000111101001, bits_uint16_t_reverse(v, 4, 5));
  EXPECT_EQ(0b0000110000011001, bits_uint16_t_reverse(v, 10, 2));
  v = 0b0000000010011001;
  EXPECT_EQ(0b0000000010011111, bits_uint16_t_reverse(v, 1, 2));
  EXPECT_EQ(0b0000000010011010, bits_uint16_t_reverse(v, 0, 2));
  EXPECT_EQ(0b0000000010011000, bits_uint16_t_reverse(v, 0, 1));
  EXPECT_EQ(0b0000000010011001, bits_uint16_t_reverse(v, 1, 0));
  EXPECT_EQ(0b0000000101101001, bits_uint16_t_reverse(v, 4, 5));
  EXPECT_EQ(0b0000110010011001, bits_uint16_t_reverse(v, 10, 2));

  uint8_t s = 0;
  uint8_t l = 0;
  for (int i = 0; i < 500; i++)
  {
    v = rand() % (0xffff);
    s = rand() % 16;
    l = rand() % 16;
    EXPECT_EQ(1, _bits_check(16, __CHECK_REVERSE, v, bits_uint16_t_reverse(v, s, l), s, l));
  }
}

TEST(bits_uint32_t_set, pass)
{
  uint32_t v = 0b0000000000000000;
  EXPECT_EQ(0b0000000000000110, bits_uint32_t_set(v, 1, 2));
  EXPECT_EQ(0b0000000000000011, bits_uint32_t_set(v, 0, 2));
  EXPECT_EQ(0b0000000000000001, bits_uint32_t_set(v, 0, 1));
  EXPECT_EQ(0b0000000000000000, bits_uint32_t_set(v, 1, 0));
  EXPECT_EQ(0b0000011111000000, bits_uint32_t_set(v, 6, 5));
  EXPECT_EQ(0b0000110000000000, bits_uint32_t_set(v, 10, 2));
  v = 0b0000000000011000;
  EXPECT_EQ(0b0000000000011110, bits_uint32_t_set(v, 1, 2));
  EXPECT_EQ(0b0000000000011011, bits_uint32_t_set(v, 0, 2));
  EXPECT_EQ(0b0000000000011001, bits_uint32_t_set(v, 0, 1));
  EXPECT_EQ(0b0000000000011000, bits_uint32_t_set(v, 1, 0));
  EXPECT_EQ(0b0000011111011000, bits_uint32_t_set(v, 6, 5));
  EXPECT_EQ(0b0000110000011000, bits_uint32_t_set(v, 10, 2));
  v = 0b0000000000011001;
  EXPECT_EQ(0b0000000000011111, bits_uint32_t_set(v, 1, 2));
  EXPECT_EQ(0b0000000000011011, bits_uint32_t_set(v, 0, 2));
  EXPECT_EQ(0b0000000000011001, bits_uint32_t_set(v, 0, 1));
  EXPECT_EQ(0b0000000000011001, bits_uint32_t_set(v, 1, 0));
  EXPECT_EQ(0b0000011111011001, bits_uint32_t_set(v, 6, 5));
  EXPECT_EQ(0b0000110000011001, bits_uint32_t_set(v, 10, 2));
  v = 0b0000000010011001;
  EXPECT_EQ(0b0000000010011111, bits_uint32_t_set(v, 1, 2));
  EXPECT_EQ(0b0000000010011011, bits_uint32_t_set(v, 0, 2));
  EXPECT_EQ(0b0000000010011001, bits_uint32_t_set(v, 0, 1));
  EXPECT_EQ(0b0000000010011001, bits_uint32_t_set(v, 1, 0));
  EXPECT_EQ(0b0000011111011001, bits_uint32_t_set(v, 6, 5));
  EXPECT_EQ(0b0000110010011001, bits_uint32_t_set(v, 10, 2));

  uint8_t s = 0;
  uint8_t l = 0;
  for (int i = 0; i < 500; i++)
  {
    v = rand() % (0xffffffff);
    s = rand() % 32;
    l = rand() % 32;
    EXPECT_EQ(1, _bits_check(32, __CHECK_SET, v, bits_uint32_t_set(v, s, l), s, l));
  }
}

TEST(bits_uint32_t_reset, pass)
{
  uint32_t v = 0b0000000000000000;
  EXPECT_EQ(0b0000000000000000, bits_uint32_t_reset(v, 1, 2));
  EXPECT_EQ(0b0000000000000000, bits_uint32_t_reset(v, 0, 2));
  EXPECT_EQ(0b0000000000000000, bits_uint32_t_reset(v, 0, 1));
  EXPECT_EQ(0b0000000000000000, bits_uint32_t_reset(v, 1, 0));
  EXPECT_EQ(0b0000000000000000, bits_uint32_t_reset(v, 6, 5));
  EXPECT_EQ(0b0000000000000000, bits_uint32_t_reset(v, 10, 2));
  v = 0b0000000000011011;
  EXPECT_EQ(0b0000000000011001, bits_uint32_t_reset(v, 1, 2));
  EXPECT_EQ(0b0000000000011000, bits_uint32_t_reset(v, 0, 2));
  EXPECT_EQ(0b0000000000011010, bits_uint32_t_reset(v, 0, 1));
  EXPECT_EQ(0b0000000000011011, bits_uint32_t_reset(v, 1, 0));
  EXPECT_EQ(0b0000000000001011, bits_uint32_t_reset(v, 4, 5));
  EXPECT_EQ(0b0000000000011011, bits_uint32_t_reset(v, 10, 2));
  v = 0b0000000000011100;
  EXPECT_EQ(0b0000000000011000, bits_uint32_t_reset(v, 1, 2));
  EXPECT_EQ(0b0000000000011100, bits_uint32_t_reset(v, 0, 2));
  EXPECT_EQ(0b0000000000011100, bits_uint32_t_reset(v, 0, 1));
  EXPECT_EQ(0b0000000000011100, bits_uint32_t_reset(v, 1, 0));
  EXPECT_EQ(0b0000000000001100, bits_uint32_t_reset(v, 4, 5));
  EXPECT_EQ(0b0000000000011100, bits_uint32_t_reset(v, 10, 2));
  v = 0b0000000000011001;
  EXPECT_EQ(0b0000000000011001, bits_uint32_t_reset(v, 1, 2));
  EXPECT_EQ(0b0000000000011000, bits_uint32_t_reset(v, 0, 2));
  EXPECT_EQ(0b0000000000011000, bits_uint32_t_reset(v, 0, 1));
  EXPECT_EQ(0b0000000000011001, bits_uint32_t_reset(v, 1, 0));
  EXPECT_EQ(0b0000000000001001, bits_uint32_t_reset(v, 4, 5));
  EXPECT_EQ(0b0000000000011001, bits_uint32_t_reset(v, 10, 2));
  v = 0b0000000010011001;
  EXPECT_EQ(0b0000000010011001, bits_uint32_t_reset(v, 1, 2));
  EXPECT_EQ(0b0000000010011000, bits_uint32_t_reset(v, 0, 2));
  EXPECT_EQ(0b0000000010011000, bits_uint32_t_reset(v, 0, 1));
  EXPECT_EQ(0b0000000010011001, bits_uint32_t_reset(v, 1, 0));
  EXPECT_EQ(0b0000000000001001, bits_uint32_t_reset(v, 4, 5));
  EXPECT_EQ(0b0000000010011001, bits_uint32_t_reset(v, 10, 2));
  uint8_t s = 0;
  uint8_t l = 0;
  for (int i = 0; i < 500; i++)
  {
    v = rand() % (0xffffffff);
    s = rand() % 32;
    l = rand() % 32;
    EXPECT_EQ(1, _bits_check(32, __CHECK_RESET, v, bits_uint32_t_reset(v, s, l), s, l));
  }
}

TEST(bits_uint32_t_reverse, pass)
{
  uint32_t v = 0b0000000000000000;
  EXPECT_EQ(0b0000000000000110, bits_uint32_t_reverse(v, 1, 2));
  EXPECT_EQ(0b0000000000000011, bits_uint32_t_reverse(v, 0, 2));
  EXPECT_EQ(0b0000000000000001, bits_uint32_t_reverse(v, 0, 1));
  EXPECT_EQ(0b0000000000000000, bits_uint32_t_reverse(v, 1, 0));
  EXPECT_EQ(0b0000011111000000, bits_uint32_t_reverse(v, 6, 5));
  EXPECT_EQ(0b0000110000000000, bits_uint32_t_reverse(v, 10, 2));
  v = 0b0000000000011011;
  EXPECT_EQ(0b0000000000011101, bits_uint32_t_reverse(v, 1, 2));
  EXPECT_EQ(0b0000000000011000, bits_uint32_t_reverse(v, 0, 2));
  EXPECT_EQ(0b0000000000011010, bits_uint32_t_reverse(v, 0, 1));
  EXPECT_EQ(0b0000000000011011, bits_uint32_t_reverse(v, 1, 0));
  EXPECT_EQ(0b0000000111101011, bits_uint32_t_reverse(v, 4, 5));
  EXPECT_EQ(0b0000110000011011, bits_uint32_t_reverse(v, 10, 2));
  v = 0b0000000000011100;
  EXPECT_EQ(0b0000000000011010, bits_uint32_t_reverse(v, 1, 2));
  EXPECT_EQ(0b0000000000011111, bits_uint32_t_reverse(v, 0, 2));
  EXPECT_EQ(0b0000000000011101, bits_uint32_t_reverse(v, 0, 1));
  EXPECT_EQ(0b0000000000011100, bits_uint32_t_reverse(v, 1, 0));
  EXPECT_EQ(0b0000000111101100, bits_uint32_t_reverse(v, 4, 5));
  EXPECT_EQ(0b0000110000011100, bits_uint32_t_reverse(v, 10, 2));
  v = 0b0000000000011001;
  EXPECT_EQ(0b0000000000011111, bits_uint32_t_reverse(v, 1, 2));
  EXPECT_EQ(0b0000000000011010, bits_uint32_t_reverse(v, 0, 2));
  EXPECT_EQ(0b0000000000011000, bits_uint32_t_reverse(v, 0, 1));
  EXPECT_EQ(0b0000000000011001, bits_uint32_t_reverse(v, 1, 0));
  EXPECT_EQ(0b0000000111101001, bits_uint32_t_reverse(v, 4, 5));
  EXPECT_EQ(0b0000110000011001, bits_uint32_t_reverse(v, 10, 2));
  v = 0b0000000010011001;
  EXPECT_EQ(0b0000000010011111, bits_uint32_t_reverse(v, 1, 2));
  EXPECT_EQ(0b0000000010011010, bits_uint32_t_reverse(v, 0, 2));
  EXPECT_EQ(0b0000000010011000, bits_uint32_t_reverse(v, 0, 1));
  EXPECT_EQ(0b0000000010011001, bits_uint32_t_reverse(v, 1, 0));
  EXPECT_EQ(0b0000000101101001, bits_uint32_t_reverse(v, 4, 5));
  EXPECT_EQ(0b0000110010011001, bits_uint32_t_reverse(v, 10, 2));

  uint8_t s = 0;
  uint8_t l = 0;
  for (int i = 0; i < 500; i++)
  {
    v = rand() % (0xffffffff);
    s = rand() % 32;
    l = rand() % 32;
    EXPECT_EQ(1, _bits_check(32, __CHECK_REVERSE, v, bits_uint32_t_reverse(v, s, l), s, l));
  }
}
