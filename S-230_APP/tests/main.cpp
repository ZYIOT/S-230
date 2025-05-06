#include "gtest/gtest.h"

#include "app_config.h"
int main(int argc, char **argv)
{
  APP_CONFIG_EepromRecoveryAll();
  APP_CONFIG_EepromLoadAll();
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
