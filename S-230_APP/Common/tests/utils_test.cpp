#include <gtest/gtest.h>

extern "C"
{
#include "utils.c"
}

TEST(read_write_int8_t, BE)
{
  uint8_t buffer[2] = {0};
  write_int8_t_BE(-16, buffer);
  int16_t value = read_int8_t_BE(buffer);
  EXPECT_EQ(-16, value);
}

TEST(read_write_int8_t, LE)
{
  uint8_t buffer[2] = {0};
  write_int8_t(-16, buffer);
  int16_t value = read_int8_t(buffer);
  EXPECT_EQ(-16, value);
}

TEST(read_write_int16_t, BE)
{
  uint8_t buffer[2] = {0};
  write_int16_t_BE(-236, buffer);
  int16_t value = read_int16_t_BE(buffer);
  EXPECT_EQ(-236, value);
}

TEST(read_write_int16_t, LE)
{
  uint8_t buffer[2] = {0};
  write_int16_t(-236, buffer);
  int16_t value = read_int16_t(buffer);
  EXPECT_EQ(-236, value);
}

TEST(read_write_uint16_t, BE)
{
  uint8_t buffer[2] = {0};
  write_uint16_t_BE(23456, buffer);
  uint16_t value = read_uint16_t_BE(buffer);
  EXPECT_EQ(23456, value);
  EXPECT_EQ(0x5b, buffer[0]);
  EXPECT_EQ(0xa0, buffer[1]);
}

TEST(read_write_uint16_t, LE)
{
  uint8_t buffer[2] = {0};
  write_uint16_t(23456, buffer);
  uint16_t value = read_uint16_t(buffer);
  EXPECT_EQ(23456, value);
  EXPECT_EQ(0x5b, buffer[1]);
  EXPECT_EQ(0xa0, buffer[0]);
}

TEST(read_write_uint32_t, BE)
{
  uint8_t buffer[4] = {0};
  write_uint32_t_BE(123456, buffer);
  uint32_t value = read_uint32_t_BE(buffer);
  EXPECT_EQ(123456, value);
  EXPECT_EQ(0, buffer[0]);
  EXPECT_EQ(0x01, buffer[1]);
  EXPECT_EQ(0xe2, buffer[2]);
  EXPECT_EQ(0x40, buffer[3]);
}

TEST(read_write_uint32_t, LE)
{
  uint8_t buffer[4] = {0};
  write_uint32_t(123456, buffer);
  uint32_t value = read_uint32_t(buffer);
  EXPECT_EQ(123456, value);
  EXPECT_EQ(0, buffer[3]);
  EXPECT_EQ(0x01, buffer[2]);
  EXPECT_EQ(0xe2, buffer[1]);
  EXPECT_EQ(0x40, buffer[0]);
}

TEST(read_write_int32_t, BE)
{
  uint8_t buffer[4] = {0};
  write_int32_t_BE(-123456, buffer);
  uint32_t value = read_int32_t_BE(buffer);
  EXPECT_EQ(-123456, value);
}

TEST(read_write_int32_t, LE)
{
  uint8_t buffer[4] = {0};
  write_int32_t(-123456, buffer);
  uint32_t value = read_int32_t(buffer);
  EXPECT_EQ(-123456, value);
}

TEST(read_write_float_t, BE)
{
  uint8_t buffer[4] = {0};
  write_float_BE(232.5, buffer);
  float value = read_float_BE(buffer);
  EXPECT_FLOAT_EQ(232.5, value);

  write_float_BE(2321.153, buffer);
  value = read_float_BE(buffer);
  EXPECT_FLOAT_EQ(2321.153, value);
}

TEST(read_write_float_t, LE)
{
  uint8_t buffer[4] = {0};
  write_float(232.5, buffer);
  float value = read_float(buffer);
  EXPECT_FLOAT_EQ(232.5, value);

  write_float(2321.153, buffer);
  value = read_float(buffer);
  EXPECT_FLOAT_EQ(2321.153, value);
}

TEST(read_write_double_t, BE)
{
  uint8_t buffer[8] = {0};
  write_double_BE(212343243423.5, buffer);
  double value = read_double_BE(buffer);
  EXPECT_DOUBLE_EQ(212343243423.5, value);

  write_double_BE(21234243423.521, buffer);
  value = read_double_BE(buffer);
  EXPECT_DOUBLE_EQ(21234243423.521, value);
}

TEST(read_write_double_t, LE)
{
  uint8_t buffer[8] = {0};
  write_double(212343243423.5, buffer);
  double value = read_double(buffer);
  EXPECT_DOUBLE_EQ(212343243423.5, value);

  write_double(21234243423.521, buffer);
  value = read_double(buffer);
  EXPECT_DOUBLE_EQ(21234243423.521, value);
}

TEST(get_default_crc16, LE)
{
  uint8_t buffer[] = {0xce};
  EXPECT_EQ(0x9481, get_default_crc16(&buffer[0], 1));
  uint8_t buffer1[] = {0xa5, 0x5a, 0xce, 0x00};
  EXPECT_EQ(0xbf56, get_default_crc16(&buffer1[0], 4));
  uint8_t buffer2[] = {0xa5, 0x00, 0x00, 0x03, 0xfe, 0x01, 0x00, 0x06, 0xa5, 0x5a, 0x86, 0x0, 0x60, 0xbf};
  EXPECT_EQ(0x47a5, get_default_crc16(&buffer2[0], 14));
  uint8_t buffer3[] = {0xa5, 0x00, 0x00, 0x03, 0xfe, 0x01, 0x00, 0x06, 0xa5, 0x5a, 0x86, 0x0};
  EXPECT_EQ(0x8aed, get_default_crc16(&buffer3[0], 12));
  uint8_t buffer4[] = {0xa5, 0x00, 0x00, 0x03, 0xfe, 0x01, 0x00, 0x06, 0xa5, 0x5a, 0x88, 0x0};
  EXPECT_EQ(0xeae9, get_default_crc16(&buffer4[0], 12));
}

TEST(get_modbus_crc16, LE)
{
  uint8_t buffer[] = {0xce};
  EXPECT_EQ(0xd43e, get_modbus_crc16(&buffer[0], 1));
  uint8_t buffer1[] = {0xa5, 0x5a, 0xce, 0x00};
  EXPECT_EQ(0x9b56, get_modbus_crc16(&buffer1[0], 4));
}

TEST(uint8_t_to_bcd, BE)
{
  EXPECT_EQ(0x79, uint8_t_to_bcd(79));
  EXPECT_EQ(0x59, uint8_t_to_bcd(59));
  EXPECT_EQ(0x81, uint8_t_to_bcd(81));
  EXPECT_EQ(0x80, uint8_t_to_bcd(80));
  EXPECT_EQ(0x61, uint8_t_to_bcd(61));
  EXPECT_EQ(0x72, uint8_t_to_bcd(72));
  EXPECT_EQ(0x56, uint8_t_to_bcd(56));
  EXPECT_EQ(0x17, uint8_t_to_bcd(17));
  EXPECT_EQ(0x23, uint8_t_to_bcd(23));
  EXPECT_EQ(0x30, uint8_t_to_bcd(30));
  EXPECT_EQ(0x15, uint8_t_to_bcd(15));
  EXPECT_EQ(0x69, uint8_t_to_bcd(69));
  EXPECT_EQ(0x8, uint8_t_to_bcd(8));
  EXPECT_EQ(0x7, uint8_t_to_bcd(7));
  EXPECT_EQ(0x83, uint8_t_to_bcd(83));
  EXPECT_EQ(0x95, uint8_t_to_bcd(95));
  EXPECT_EQ(0x19, uint8_t_to_bcd(19));
  EXPECT_EQ(0x22, uint8_t_to_bcd(22));
  EXPECT_EQ(0x47, uint8_t_to_bcd(47));
  EXPECT_EQ(0x4, uint8_t_to_bcd(4));
  EXPECT_EQ(0x25, uint8_t_to_bcd(25));
  EXPECT_EQ(0x6, uint8_t_to_bcd(6));
  EXPECT_EQ(0x93, uint8_t_to_bcd(93));
  EXPECT_EQ(0x2, uint8_t_to_bcd(2));
  EXPECT_EQ(0x21, uint8_t_to_bcd(21));
  EXPECT_EQ(0x96, uint8_t_to_bcd(96));
  EXPECT_EQ(0x37, uint8_t_to_bcd(37));
  EXPECT_EQ(0x24, uint8_t_to_bcd(24));
  EXPECT_EQ(0x88, uint8_t_to_bcd(88));
  EXPECT_EQ(0x71, uint8_t_to_bcd(71));
  EXPECT_EQ(0x36, uint8_t_to_bcd(36));
  EXPECT_EQ(0x38, uint8_t_to_bcd(38));
}

TEST(uint8_t_from_bcd, BE)
{
  EXPECT_EQ(41, uint8_t_from_bcd(0x41));
  EXPECT_EQ(86, uint8_t_from_bcd(0x86));
  EXPECT_EQ(49, uint8_t_from_bcd(0x49));
  EXPECT_EQ(14, uint8_t_from_bcd(0x14));
  EXPECT_EQ(70, uint8_t_from_bcd(0x70));
  EXPECT_EQ(56, uint8_t_from_bcd(0x56));
  EXPECT_EQ(17, uint8_t_from_bcd(0x17));
  EXPECT_EQ(21, uint8_t_from_bcd(0x21));
  EXPECT_EQ(99, uint8_t_from_bcd(0x99));
  EXPECT_EQ(34, uint8_t_from_bcd(0x34));
  EXPECT_EQ(27, uint8_t_from_bcd(0x27));
  EXPECT_EQ(94, uint8_t_from_bcd(0x94));
  EXPECT_EQ(69, uint8_t_from_bcd(0x69));
  EXPECT_EQ(72, uint8_t_from_bcd(0x72));
  EXPECT_EQ(46, uint8_t_from_bcd(0x46));
  EXPECT_EQ(98, uint8_t_from_bcd(0x98));
  EXPECT_EQ(45, uint8_t_from_bcd(0x45));
  EXPECT_EQ(22, uint8_t_from_bcd(0x22));
  EXPECT_EQ(81, uint8_t_from_bcd(0x81));
  EXPECT_EQ(9, uint8_t_from_bcd(0x9));
  EXPECT_EQ(40, uint8_t_from_bcd(0x40));
  EXPECT_EQ(82, uint8_t_from_bcd(0x82));
  EXPECT_EQ(37, uint8_t_from_bcd(0x37));
  EXPECT_EQ(62, uint8_t_from_bcd(0x62));
  EXPECT_EQ(63, uint8_t_from_bcd(0x63));
  EXPECT_EQ(55, uint8_t_from_bcd(0x55));
  EXPECT_EQ(73, uint8_t_from_bcd(0x73));
  EXPECT_EQ(33, uint8_t_from_bcd(0x33));
  EXPECT_EQ(93, uint8_t_from_bcd(0x93));
  EXPECT_EQ(4, uint8_t_from_bcd(0x4));
  EXPECT_EQ(71, uint8_t_from_bcd(0x71));
  EXPECT_EQ(13, uint8_t_from_bcd(0x13));
}
