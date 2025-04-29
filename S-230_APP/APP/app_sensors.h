

#ifndef __APP_SENSORS_H
#define __APP_SENSORS_H
#ifdef __cplusplus
extern "C"
{
#endif
#include "app.h"
#include "bsp_rs485.h"
#include "bsp_protocol.h"
#include "mean_filter.h"

// #define TEST_SENSOR_DATA    // 调试用，测试传感器数据在LCD上显示时的宏定义 
#ifdef TEST_SENSOR_DATA
    #define MIN_TMP_VALUE   0.1
    #define MAX_TMP_VALUE   45.6
    #define DEF_TMP_VALUE   43.2
    #define INT_TMP_VALUE   0.7

    #define MIN_DO_VALUE    0.90
    #define MAX_DO_VALUE    21.00
    #define DEF_DO_VALUE    9.12
    #define INT_DO_VALUE    0.43

    #define MIN_PH_VALUE    0.00
    #define MAX_PH_VALUE    14.00
    #define DEF_PH_VALUE    7.89
    #define INT_PH_VALUE    0.61
#endif

#ifdef SUPPORT_L6
#define APP_SENSOR_INDICATOR_BUFFER_SIZE 10     // 在L6的可读取传感器数据流程中，最多能读取到11组数据，因此此处设置为10组 
#else
#define APP_SENSOR_INDICATOR_BUFFER_SIZE 30
#endif

#define SENSOR_HAS_NO_CONFIG    0   // 没有配置和识别传感器 
#define SENSOR_EEPROM_CONFIG    1   // EEPROM 中配置了传感器 
#define SENSOR_DRIVER_CONFIG    2   // 自识别 中配置了传感器 
#define SENSOR_PAUSE_CONFIG     4   // 传感器识别期间，暂停刷新 

#define SENSOR_INITING_STATE    -1  // 初始化，等待识别之后读取数据 
#define SENSOR_UNKNOW_STATE     0   // 未识别 
#define SENSOR_NORMAL_STATE     1   // 已识别，且已经可以读取数据 

#define SENSOR_DATA_NON         -1  // 读取不到数据 
#define SENSOR_DATA_UNCHANGE    0   // 数据无更新 
#define SENSOR_DATA_CHANGED     1   // 数据(正常)有更新 
#define SENSOR_DATA_OVERFLOW    2   // 数据溢出上限和下限 

typedef struct
{
    int status; // 数据是否正常 
    float value;
    int16_t vm;
    uint16_t errorCount; // 连续出错次数 
    mean_filter_t filter;
    rs485_sensor_pt sensor;
    double buffer[APP_SENSOR_INDICATOR_BUFFER_SIZE];
} APP_WATER_IndicatorValue_t, *APP_WATER_IndicatorValue_pt;

typedef void (*rs485_sensor_indicator_fun)(rs485_sensor_indicator_pt indicator);
typedef void (*rs485_sensor_fun)(rs485_sensor_pt sensor);


#define APP_SENSOR_IS_ENABLE(sensor) ((sensor)->enable != RS485_DISABLE)
#define APP_SENSORS_MAX_INDICATOR_SIZE INDICATOR_SIZE

int APP_SENSORS_Init(void);
void APP_SENSORS_RefreshNew(void);
void APP_SENSORS_RefreshStatus(void);
void APP_SENSORS_task_run(void *argument);
int APP_SENSORS_WaterIndicatorGet(uint8_t index, APP_WATER_IndicatorValue_t **vp);
int APP_SENSORS_SensorGet(uint8_t addr, rs485_sensor_t **vp);
int APP_SENSORS_ReadEach(rs485_sensor_indicator_fun callback, uint32_t interval_delay_ms);
void APP_SENSORS_CheckIndicatorAlert(int probeID, int indicatorID, g2_server_sensor_data_indicator_message_pt indicator);
int APP_SENSORS_SendDataToServer(void);
int APP_SENSORS_SetCompensationsBySensor(rs485_sensor_pt sensor);
int APP_SENSOR_WriteCompensation(uint8_t probeID, uint8_t indicatorID, float value, int force);
int APP_SENSOR_Calibrate(uint8_t probeID, uint8_t sensorID, uint8_t type, uint8_t *params);
void APP_SENSORS_CheckIndicatorSaturabilityAlert(void);
int APP_SENSOR_WriteRef(uint8_t sensorID, uint8_t indicatorID, uint16_t value);
void APP_SENSOR_SendSelfDiagnosisToServer(void);
int APP_SENSORS_ReadDeviceSensors(g2_server_device_sensors_message_pt message);

#ifdef __cplusplus
}
#endif
#endif
