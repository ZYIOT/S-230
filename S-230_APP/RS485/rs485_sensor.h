#ifndef __RS485_SENSOR_H
#define __RS485_SENSOR_H
#ifdef __cplusplus
extern "C"
{
#endif
#include "rs485.h"
#define RS485_SENSOR_STATUS_OFFLINE 0x0
#define RS485_SENSOR_STATUS_INITIALIZE 0x1
#define RS485_SENSOR_STATUS_ONLINE 0x3

#define RS485_ENABLE 1
#define RS485_DISABLE 0

#define RS485_SENSOR_MAX_PN_LEN 10
#define RS485_SENSOR_MAX_SN_LEN 8
#define RS485_SENSOR_MAX_HW_LEN 3
#define RS485_SENSOR_MAX_FW_LEN 3

#define RS485_SENSOR_MODEL_ADDR 0x0801

#define NEW_RS485_SENSOR_INDICATOR(variable) rs485_sensor_indicator_t variable = {.value1 = 0, .value2 = 0, .value3 = 0, .value4 = 0, .vm1 = 0, .vm2 = 0, .vm3 = 0, .vm4 = 0, .id = 0, .status = 0, .rc = 0}
#define RS485_SENSOR_NEW(_id, _enable)                                                      \
    {                                                                                       \
        .port = NULL, .id = _id, .manufacturer = 0, .model = 0, .protocol = 0, .status = 0, \
        .firmware = &sensor_HW[_id][0],                                                     \
        .hardware = &sensor_FW[_id][0],                                                     \
        .SN = &sensor_SN[_id][0],                                                           \
        .PN = &sensor_PN[_id][0],                                                           \
        .sensor_change = 0,                                                                 \
        .action = NULL, .enable = _enable,                                                  \
        .errorCount = 0, .timeout_error_count = 0, .crc_error_count = 0,                   \
        .rs485_error_code_count = 0, .calibration_count = 0, .indicator = { 0 }             \
    }

    typedef struct rs485_sensor_s rs485_sensor_s_t;
    typedef struct rs485_sensor_action_s rs485_sensor_action_s_t;

/*
    溶氧传感器:
    value1: 温度 
    value2: 溶解氧 
    value3: 饱和度 

    pH传感器:
    value1: pH值 
    value2: 温度 
    vm1: ph电位值 
*/
    typedef struct
    {
        // 单次读取数据情况 
        float value1;
        float value2;
        float value3;
        float value4;
        int16_t vm1;
        int16_t vm2;
        int16_t vm3;
        int16_t vm4;
        uint8_t id;
        int status;     // 数据查询的状态 
        int error_code; // 485 通信的错误状态 
        // dev实体 
        rs485_sensor_s_t *sensor;
    } rs485_sensor_indicator_t, *rs485_sensor_indicator_pt;

    // 传感器drv结构体 
    typedef struct
    {
        rs485_port_pt port;
        uint8_t id;
        uint16_t manufacturer;
        uint8_t model;  // model_id 
        uint8_t protocol;
        rs485_sensor_action_s_t *action;
    } rs485_sensor_driver_t, *rs485_sensor_driver_pt;

    typedef int (*rs485_read_value)(rs485_sensor_s_t *rs485, rs485_sensor_indicator_pt indicator);
    typedef int (*rs485_read_info)(rs485_sensor_s_t *rs485);
    typedef int (*rs485_write_info)(rs485_sensor_s_t *rs485, void *params);
    typedef int (*rs485_write_calibrate)(rs485_sensor_s_t *rs485, uint8_t indicatorID, void *params);
    typedef int (*rs485_read_compensation)(rs485_sensor_s_t *rs485, uint8_t indicatorID, float *value);
    typedef int (*rs485_write_compensation)(rs485_sensor_s_t *rs485, uint8_t indicatorID, float value);
    typedef int (*rs485_check_sensor)(rs485_sensor_s_t *rs485, rs485_sensor_driver_pt sd);
    typedef int (*rs485_match_pn)(rs485_sensor_s_t *rs485, void *params);
    typedef int (*rs485_match_model)(rs485_sensor_s_t *rs485, uint8_t model, uint8_t theory);
    typedef enum
    {
        RECOGNITION_MODE_UNIDENTIFIED = 0,
        RECOGNITION_MODE_AUTO = 1,      // 自动识别 
        RECOGNITION_MODE_MANUAL = 2,    // 手动设置 
        RECOGNITION_MODE_COMPATIBLE = 3 // 兼容模式 
    } rs485_recognition_mode_t;

    // 传感器drv opt结构体 
    typedef struct rs485_sensor_action_s
    {
        rs485_read_value read_value1;
        rs485_read_value read_value2;
        rs485_read_value read_value3;
        rs485_read_value read_value4;
        rs485_read_info read_info;
        rs485_read_info read_version;
        rs485_write_info write_info;
        rs485_check_sensor check_sensor;
        rs485_match_pn match_pn;
        rs485_match_model match_model;
        rs485_write_compensation write_compensation;
        rs485_read_compensation read_compensation;
        rs485_write_calibrate write_calibrate;
        rs485_recognition_mode_t recognition_mode;
        char *PN;   // 完整的PN的指针 
        char *pn_match_name;    // 用于匹配PN关键词的指针 
    } rs485_sensor_action_t, *rs485_sensor_action_pt;

    // 传感器dev结构体 
    typedef struct rs485_sensor_s
    {
        // 驱动的底层硬件 
        rs485_port_pt port;
        uint8_t id;
        // 识别到的(用于匹配驱动的关键词) 
        uint16_t manufacturer;
        uint16_t model; // model_id 
        uint8_t protocol;
        uint8_t status;
        uint8_t *firmware;
        uint8_t *hardware;
        uint8_t *SN;
        char *PN;
        int sensor_change;
        // 匹配到的具体驱动的操作函数 
        rs485_sensor_action_pt action;
        // 应用层配置位 
        uint8_t enable;
        // 应用层记录设备操作的结果 
        uint16_t errorCount;
        uint16_t timeout_error_count;
        uint16_t crc_error_count;
        uint16_t rs485_error_code_count;
        uint16_t calibration_count;
        // 驱动读取到的数据 
        rs485_sensor_indicator_pt indicator;
    } rs485_sensor_t, *rs485_sensor_pt;

    // int RS485_SENSOR_scan(rs485_port_pt rs485, uint8_t id, rs485_sensor_pt sensor);
    char *RS485_SENSOR_status_name(uint8_t status);
    char *RS485_SENSOR_recognition_mode_name(rs485_recognition_mode_t recognition_mode);
#ifdef __cplusplus
}
#endif
#endif
