#include "gtest/gtest.h"

#include "app_config.h"
int main(int argc, char **argv)
{
  APP_CONFIG_eeprom_recovery_all();
  APP_CONFIG_eeprom_load_all();
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
