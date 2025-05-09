#include "bsp_board_debug.h"
// 控制箱/采集器 支持的传感器使能 
#define ENABLE_RS485_DRIVER_SENSORS_ZYIOT_PH201
#define ENABLE_RS485_DRIVER_SENSORS_ZYIOT_DOY100
#define ENABLE_RS485_DRIVER_SENSORS_PROBEST_DOY121
#define ENABLE_RS485_DRIVER_SENSORS_SZKLT_PH230
// #define ENABLE_RS485_DRIVER_SENSORS_FIVE_PHASE_DOY170
// #define ENABLE_RS485_DIRVER_SENSOR_ANBULEILA_WL991
#define ENABLE_RS485_DIRVER_SENSORS_CHEMINS_PH260
#ifdef ENABLE_DO_SENSOR_DEFAULT
    #define ENABLE_RS485_DRIVER_SENSOR_DO_DEFAULT 
#endif
#define ENABLE_RS485_DRIVER_SENSORS_CHEMINS_NHN760   // (PH260 仿冒 NH3传感器) 
#define SENSOR_NHN_TEST_DEBUG
#define RS485_delay_ms(ms) osDelay(ms)
