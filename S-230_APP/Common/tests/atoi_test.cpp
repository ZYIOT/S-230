#include <gtest/gtest.h>

extern "C"
{
#define AT_PARSER_OK 0
#define AT_PARSER_ERROR -1
#define AT_PARSER_READ_TIMEOUT -2

  int AT_PARSER_find_integer(char *buffer, int *value)
  {
    char *p = buffer;
    int rc = AT_PARSER_ERROR;
    int v = 0;
    while (*p != 0)
    {
      if ((*p >= '0') && (*p <= '9'))
      {
        v = v * 10;
        v += *p - '0';
        rc = AT_PARSER_OK;
      }
      else
      {
        if (rc == AT_PARSER_OK)
        {
          break;
        }
      }
      p++;
    }
    *value = v;
    return rc;
  }

}

TEST(AT_PARSER_find_integer, ok)
{
  int size = 0;
  EXPECT_EQ(AT_PARSER_OK, AT_PARSER_find_integer((char*)"+QIRD: 1", &size));
  EXPECT_EQ(1, size);
  EXPECT_EQ(AT_PARSER_OK, AT_PARSER_find_integer((char*)"+QIRD: 100", &size));
  EXPECT_EQ(100, size);
  EXPECT_EQ(AT_PARSER_OK, AT_PARSER_find_integer((char*)"+QI233RD: 100", &size));
  EXPECT_EQ(233, size);
  EXPECT_EQ(AT_PARSER_OK, AT_PARSER_find_integer((char*)"+QIRD: 0 3 45 6", &size));
  EXPECT_EQ(0, size);
  EXPECT_EQ(AT_PARSER_OK, AT_PARSER_find_integer((char*)"+QIRD: 23 0 3 45 6", &size));
  EXPECT_EQ(23, size);
  EXPECT_EQ(AT_PARSER_OK, AT_PARSER_find_integer((char*)"+adfQIRD: adf0asdf", &size));
  EXPECT_EQ(0, size);
  EXPECT_EQ(AT_PARSER_OK, AT_PARSER_find_integer((char*)"+adfQ2IRD: a3df0asdf", &size));
  EXPECT_EQ(2, size);
  EXPECT_EQ(AT_PARSER_OK, AT_PARSER_find_integer((char*)"+QIRD1: 10 3 45 6", &size));
  EXPECT_EQ(1, size);
}

TEST(AT_PARSER_find_integer, error)
{
  int size = 0;
  EXPECT_EQ(AT_PARSER_ERROR, AT_PARSER_find_integer((char*)"+QIRD: a", &size));
  EXPECT_EQ(AT_PARSER_ERROR, AT_PARSER_find_integer((char*)"+QIRD: ,", &size));
  EXPECT_EQ(AT_PARSER_ERROR, AT_PARSER_find_integer((char*)"+QIRD:d", &size));
}