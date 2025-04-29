#include "bsp_rs485.h"
#include "bsp_rtc.h"
#include "bsp_protocol.h"
#include "water_indicator.h"
#include "hardware_wrapper.h"
#include "app.h"
#include "app_log.h"
#include "app_sensors.h"
#include "app_config.h"
#include "app_network.h"
#ifdef SUPPORT_PUMP
#include "app_pump.h"
#endif
#ifdef SUPPORT_L6
#include "app_l6.h"
#endif
#ifdef SUPPORT_POWER_OFF_SENSOR
#include "bsp_power_key.h"
#endif

#define UPLOAD_SENSOR_DATA_CYCLE_TIME   5   // 上报传感器数据周期, 分钟 
#define SCAN_SENSOR_DRIVER_CYCLE_TIME   10  // 传感器扫描更新驱动周期, 分钟 
#define UPDATE_SENSOR_MSG_CYCLE_TIME    5   // 更新传感器信息周期, 分钟 

#define __RECORD_INDICATOR(_idx, _value_field, _vm_filed) record_water_indicator(_idx, indicator->_value_field, indicator->_vm_filed, indicator)

static app_water_indicator_value_t indicators_values[APP_SENSORS_MAX_INDICATOR_SIZE] = {0};
static uint8_t sensor_PN[APP_SENSORS_MAX_INDICATOR_SIZE][RS485_SENSOR_MAX_PN_LEN] = {0};
static uint8_t sensor_SN[APP_SENSORS_MAX_INDICATOR_SIZE][RS485_SENSOR_MAX_SN_LEN] = {0};
static uint8_t sensor_HW[APP_SENSORS_MAX_INDICATOR_SIZE][RS485_SENSOR_MAX_HW_LEN] = {0};
static uint8_t sensor_FW[APP_SENSORS_MAX_INDICATOR_SIZE][RS485_SENSOR_MAX_FW_LEN] = {0};
// 注册驱动(可用)的传感器dev列表，依次存放 DO、PH 的 dev 结构体 
static rs485_sensor_t sensors[] = {
    RS485_SENSOR_NEW(RS485_DRIVER_SENSOR_ID_DO, RS485_ENABLE),
    RS485_SENSOR_NEW(RS485_DRIVER_SENSOR_ID_PH, RS485_ENABLE),
    RS485_SENSOR_NEW(RS485_DRIVER_SENSOR_ID_CONDUCTIVITY, RS485_DISABLE),
    RS485_SENSOR_NEW(RS485_DRIVER_SENSOR_ID_CHLOROPHYLL, RS485_DISABLE),
    RS485_SENSOR_NEW(RS485_DRIVER_SENSOR_ID_COD, RS485_DISABLE),
    RS485_SENSOR_NEW(RS485_DRIVER_SENSOR_ID_NH3, RS485_DISABLE),
    RS485_SENSOR_NEW(RS485_DRIVER_SENSOR_ID_LEVEL, RS485_DISABLE),
};

static rs485_sensor_pt sensor = NULL;   // 传感器dev指针，指向 sensors[]  
// 带数据的传感器dev实体列表，依次存放 DO、PH 的实体数据 
static rs485_sensor_indicator_t sensor_indicators[INDICATOR_SIZE] = {
    {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}
};
static uint32_t send_data_tick = 0; // 发送数据到服务器的时间 tick 
static uint8_t count_tick=0;        // DO 传感器采样的饱和度数据个数 
static uint16_t delta_saturability_data = 0;    // 饱和度的最大最小差值 
static uint32_t sensor_compensation_ticks[INDICATOR_SIZE] = {0};

extern float LcdSensorData[3];
#ifdef SUPPORT_TFTLCD
#include "app_lcd.h"
extern t_LcdSensorShow g_lcdSensorShow;
#define READ_HAS_CONFIG()       (g_lcdSensorShow.sensorHasConfig)
#define SET_HAS_NO_CONFIG()     (g_lcdSensorShow = SENSOR_HAS_NO_CONFIG)
#define SET_EEPROM_CONFIG()     (g_lcdSensorShow.sensorHasConfig |= SENSOR_EEPROM_CONFIG)
#define SET_DRIVER_CONFIG()     (g_lcdSensorShow.sensorHasConfig |= SENSOR_DRIVER_CONFIG)
#define SET_PAUSE_CONFIG()      (g_lcdSensorShow.sensorHasConfig |= SENSOR_PAUSE_CONFIG)
#define CLR_EEPROM_CONFIG()     (g_lcdSensorShow.sensorHasConfig &= (uint8_t)(~SENSOR_EEPROM_CONFIG))
#define CLR_DRIVER_CONFIG()     (g_lcdSensorShow.sensorHasConfig &= (uint8_t)(~SENSOR_DRIVER_CONFIG))
#define CLR_PAUSE_CONFIG()      (g_lcdSensorShow.sensorHasConfig &= (uint8_t)(~SENSOR_PAUSE_CONFIG))
uint8_t s_findSensor = 0;
#endif
#ifndef SUPPORT_IRED
extern uint8_t gLcdLightOn;
#endif

// 根据传感器ID获取水质index 
static uint8_t _get_indicator_index(uint8_t sensor_id)
{
    uint8_t index = 0;
    if(RS485_DRIVER_SENSOR_ID_DO == sensor_id)
    {
        index = WATER_INDICATOR_INDEX_DO;
    }
    else if(RS485_DRIVER_SENSOR_ID_PH == sensor_id)
    {
        index = WATER_INDICATOR_INDEX_PH;
    }
    // 此行判定需注意
    else if(RS485_DRIVER_SENSOR_ID_CONDUCTIVITY == sensor_id)
    {
        index = WATER_INDICATOR_INDEX_SALINITY;
    }
    else if(RS485_DRIVER_SENSOR_ID_CHLOROPHYLL == sensor_id)
    {
        index = WATER_INDICATOR_INDEX_CHLOROPHYLL;
    }
    else if(RS485_DRIVER_SENSOR_ID_COD == sensor_id)
    {
        index = WATER_INDICATOR_INDEX_COD;
    }
    else if(RS485_DRIVER_SENSOR_ID_NH3 == sensor_id)
    {
        index = WATER_INDICATOR_INDEX_NH3;
    }
    else if(RS485_DRIVER_SENSOR_ID_LEVEL == sensor_id)
    {
        index = WATER_INDICATOR_INDEX_LEVEL;
    }
    return index;
}

static int sensors_init(uint8_t id)
{
    sensors[id].port = NULL;
    sensors[id].id = id + 1;
    sensors[id].manufacturer = 0;
    sensors[id].model = 0;
    sensors[id].protocol = 0;
    sensors[id].status = 0;
    memset(sensors[id].firmware, 0, RS485_SENSOR_MAX_FW_LEN);
    memset(sensors[id].hardware, 0, RS485_SENSOR_MAX_HW_LEN);
    memset(sensors[id].SN, 0, RS485_SENSOR_MAX_SN_LEN);
    memset(sensors[id].PN, 0, RS485_SENSOR_MAX_PN_LEN);
    sensors[id].sensor_change = 0;
    sensors[id].action = NULL;
    sensors[id].enable = RS485_DISABLE;
    sensors[id].error_count = 0;
    sensors[id].timeout_error_count = 0;
    sensors[id].crc_error_count = 0;
    sensors[id].rs485_error_code_count = 0;
    sensors[id].calibration_count = 0;
    memset(sensors[id].indicator, 0, sizeof(rs485_sensor_indicator_t));
}

// 根据传感器ID判断是否能返回正确的compensation_index 
static int _get_compensation_index(uint8_t sensor_id, uint8_t *compensation_index)
{
    if (sensor_id != RS485_DRIVER_SENSOR_ID_DO)
    {
        return APP_ERROR;
    }
    else
    {
        *compensation_index = WATER_INDICATOR_INDEX_SALINITY;
    }
    return APP_OK;
}

// 初始化应用层水质传感器dev实体 
static int indicators_value_init(uint8_t id)
{
    app_water_indicator_value_pt water_indicator;
    int rc = APP_SENSORS_water_indicator_get(id, &water_indicator);
    APP_CHECK_RC(rc)
    water_indicator->sensor = NULL;
    water_indicator->status = RS485_ERROR;
    water_indicator->value = 0;
    water_indicator->error_count = 0;
    mean_filter_init(&water_indicator->filter, water_indicator->buffer, APP_SENSOR_INDICATOR_BUFFER_SIZE);
    mean_filter_reset(&water_indicator->filter);
    return APP_OK;
}

// 检测传感器探头状态 
static int _check_sensor_probe_status(int indicator_id, app_water_indicator_value_pt water_indicator, uint16_t *u_value)
{
    int rc1 = 0, rc2 = 0;
    double value = 0;
    rc1 = mean_filter_get(&water_indicator->filter, &value);
    rc2 = water_indicator_to_uint16_t(indicator_id, value, u_value);
    APP_LOG_info("probe u_value = %d\r\n", *u_value);
    if (rc1 == APP_OK && rc2 == APP_OK && water_indicator->error_count <= 3)
    {
        return APP_OK;
    }
    else
    {
        return APP_ERROR;
    }
}

static int _check_time_range(void)
{
    int rc1 = 0, rc2 = 0;
    app_config_time_t time;
    uint8_t week = 0;

    BSP_RTC_get(&time.year, &time.month, &time.day, &time.hour, &time.minute, &time.second, &week);
    rc1 = sensor_time_compare(&time, 0, 0);
    rc2 = sensor_time_compare(&time, 0, 40);
    if(rc1 == 1 && rc2 == -1)
    {
        return APP_OK;
    }
    else
    {
        return APP_ERROR;
    }
}

extern app_config_diagnosis_message_t diagnosis_msg;
// 记录水质传感器采样数据 
static int record_water_indicator(uint8_t id, float value, uint16_t vm, rs485_sensor_indicator_pt indicator)
{
    app_water_indicator_value_pt water_indicator;
    int rc = APP_SENSORS_water_indicator_get(id, &water_indicator);
    APP_CHECK_RC(rc)
    water_indicator->sensor = indicator->sensor;
	#ifdef SUPPORT_L6
    // DO传感器正常+可以读取空气中的饱和度值
	if((indicator->id==RS485_DRIVER_SENSOR_ID_DO) && APP_L6_force_saturability_ready())
    {
        if(RS485_OK == indicator->status && APP_OK == _check_time_range())
        {
            #ifdef L6_DEBUG_TEST
            rs485_l6_status.saturability_data = 80;
            #else
            rs485_l6_status.saturability_data = (uint16_t) indicator->value3;
            #endif
            APP_LOG_info("Saturability_data=%d \r\n", rs485_l6_status.saturability_data);
            // 与空气中的饱和度值范围进行比对，判断DO探头是否有附着
            if((rs485_l6_status.saturability_data<85)||(rs485_l6_status.saturability_data>120))
            {
                rs485_l6_status.saturability_count++;
                if(3 == rs485_l6_status.saturability_count)
                {
                    uint16_t u_value = 0;
                    if(APP_OK == _check_sensor_probe_status(id, water_indicator, &u_value))
                    {
                        rs485_l6_status.saturability_alet_status = 1;
                        app_config_time_t time;
                        uint8_t week = 0;
                        g2_server_self_diagnosis_message_t message;
                        BSP_RTC_get(&time.year, &time.month, &time.day, &time.hour, &time.minute, &time.second, &week);
                        APP_SELF_DIAGNOSIS_pack(&message);
                        memcpy((uint8_t *)(&diagnosis_msg.msg_time), (uint8_t *)&time, sizeof(app_config_time_t));
                        diagnosis_msg.msg_type = message.type;
                        diagnosis_msg.msg_status = message.status;
                        APP_CONFIG_diagnosis_message_write(&diagnosis_msg);
                    }                
                    APP_LOG_info("Saturability_alet_status=%d \r\n", rs485_l6_status.saturability_alet_status);
                } 
            }
            else 
            {
                rs485_l6_status.saturability_alet_status = 0;
            }
            APP_LOG_info("saturability_count=%d \r\n", rs485_l6_status.saturability_count);
        }
    }
    rs485_l6_status.saturability_status = 0;
	#endif
    if (indicator->status != RS485_OK)
    {
        water_indicator->status = indicator->status;
        mean_filter_out(&water_indicator->filter);
        water_indicator->error_count++;
#ifdef SUPPORT_TFTLCD
        APP_LOG_warn("id:%d, errcnt:%d\r\n", id, water_indicator->error_count);
        if(water_indicator->error_count >= 3)
        {
            g_lcdSensorShow.lcdSensorStatus[id] = SENSOR_NORMAL_STATE;
            g_lcdSensorShow.lcdSensorDataStatus[id] = SENSOR_DATA_NON;
        }
#endif        
    }
    else
    {
        if (indicator->error_code == 0)
        {
            water_indicator->error_count = 0;
            water_indicator->value = value;
            water_indicator->vm = vm;
            water_indicator->status = RS485_OK;
            mean_filter_add(&water_indicator->filter, value);
#ifdef SUPPORT_TFTLCD        
            LcdSensorData[id] = value;
            uint16_t dataValue = 0;
            if(WATER_INDICATOR_INDEX_TEMPERATURE == id)
            {
                dataValue = (uint16_t)(value * 10);
            }
            else if(WATER_INDICATOR_INDEX_DO == id || WATER_INDICATOR_INDEX_PH == id)
            {
                dataValue = (uint16_t)(value * 100);
            }
            g_lcdSensorShow.lcdSensorStatus[id] = SENSOR_NORMAL_STATE;
            g_lcdSensorShow.lcdSensorDataStatus[id] = SENSOR_DATA_CHANGED;            
            app_config_indicator_alert_pt alert = &app_config_indicator_alert[0][id];
            if(!ENABLE_IS_VALID(alert))
            {
            }
            else if((dataValue > alert->max) || (dataValue < alert->min))
            {
                g_lcdSensorShow.lcdSensorDataStatus[id] = SENSOR_DATA_OVERFLOW;
            }
            APP_LOG_debug("max:%d   min:%d\r\n", alert->max, alert->min);
#endif            
        }
        else
        {
            water_indicator->status = RS485_ERROR;
            mean_filter_out(&water_indicator->filter);
            water_indicator->error_count++;
#ifdef SUPPORT_TFTLCD        
            g_lcdSensorShow.lcdSensorStatus[id] = SENSOR_NORMAL_STATE;
            g_lcdSensorShow.lcdSensorDataStatus[id] = SENSOR_DATA_NON;
#endif            
        }
        APP_LOG_info("%s: status=%d value=%.2f\r\n", water_indicator_name_by_id(id),
                     indicator->status, value);
    }
    if (water_indicator->error_count > 65530)
    {
        water_indicator->error_count = 65530;
    }
    return APP_OK;
}

int APP_SENSORS_each(rs485_sensor_fun cb)
{
    for (int i = 0; i < ARRAY_LEN(sensors); i++)
    {
        cb(&sensors[i]);
    }
    return APP_OK;
}

// 校验水质传感器ID值 
#define _CHECK_WATER_INDICATOR_ID(id)         \
    if (id >= APP_SENSORS_MAX_INDICATOR_SIZE) \
    {                                         \
        return APP_ERROR;                     \
    }

int APP_SENSORS_water_indicator_get(uint8_t id, app_water_indicator_value_t **vp)
{
    _CHECK_WATER_INDICATOR_ID(id)
    *vp = indicators_values + id;
    return APP_OK;
}


static uint8_t model_id_mappgings[][4] = {
    // id 、manufacturer、model(type/theory)、model_id
    {RS485_DRIVER_SENSOR_ID_DO, RS485_DRIVER_DO_MANUFACTURER_ZYIOT, SENSOR_THEORY_MEMBRANE, RS485_SENSOR_MODEL_ID_DOY100},
    {RS485_DRIVER_SENSOR_ID_DO, RS485_DRIVER_DO_MANUFACTURER_ZYIOT, SENSOR_THEORY_OPTICS, RS485_SENSOR_MODEL_ID_DOY100},
    {RS485_DRIVER_SENSOR_ID_DO, RS485_DRIVER_DO_MANUFACTURER_PROBEST, SENSOR_THEORY_OPTICS, RS485_SENSOR_MODEL_ID_DOY121},
    {RS485_DRIVER_SENSOR_ID_PH, RS485_DRIVER_PH_MANUFACTURER_ZYIOT, SENSOR_THEORY_MEMBRANE, RS485_SENSOR_MODEL_ID_PH201},
    {RS485_DRIVER_SENSOR_ID_PH, RS485_DRIVER_PH_MANUFACTURER_SZKLT, SENSOR_THEORY_MEMBRANE, RS485_SENSOR_MODEL_ID_PH230},
    {RS485_DRIVER_SENSOR_ID_LEVEL, RS485_DRIVER_LEVEL_MANUFACTURER_ANBULEILA, SENSOR_THEORY_9, RS485_SENSOR_MODEL_ID_WL991},
    {RS485_DRIVER_SENSOR_ID_PH, RS485_DRIVER_PH_MANUFACTURER_SZSG, SENSOR_THEORY_MEMBRANE, RS485_SENSOR_MODEL_ID_PH280},
    {RS485_DRIVER_SENSOR_ID_ORP, RS485_DRIVER_ORP_MANUFACTURER_WHBW, SENSOR_THEORY_3, RS485_SENSOR_MODEL_ID_ORP320},
    {RS485_DRIVER_SENSOR_ID_SALINITY, RS485_DRIVER_SALINITY_MANUFACTURER_CHEMINS, SENSOR_THEORY_6, RS485_SENSOR_MODEL_ID_SAL610},
    {RS485_DRIVER_SENSOR_ID_ORP, RS485_DRIVER_ORP_MANUFACTURER_CHEMINS, SENSOR_THEORY_6, RS485_SENSOR_MODEL_ID_ORP620},
    {RS485_DRIVER_SENSOR_ID_PH, RS485_DRIVER_PH_MANUFACTURER_CHEMINS, SENSOR_THEORY_MEMBRANE, RS485_SENSOR_MODEL_ID_PH260},
    {RS485_DRIVER_SENSOR_ID_NH3, RS485_DRIVER_NH3_MANUFACTURER_CHEMINS, SENSOR_THEORY_7, RS485_SENSOR_MODEL_ID_NHN760},
    {RS485_DRIVER_SENSOR_ID_DO, RS485_DRIVER_DO_MANUFACTURER_ZYIOT, SENSOR_THEORY_OPTICS, RS485_SENSOR_MODEL_ID_DOY170},
    
};

// 根据 id、manufacturer 和 theory 获取 model_id
uint8_t get_model_id_by_manufacturer_and_theory(uint8_t id, uint8_t manufacturer, uint8_t theory)
{
    int i = 0;
    for (i = 0; i < ARRAY_LEN(model_id_mappgings); i++)
    {
        if (id == model_id_mappgings[i][0] && manufacturer == model_id_mappgings[i][1] && theory == model_id_mappgings[i][2])
        {
            return model_id_mappgings[i][3];
        }
    }
    return 0;
}

// 根据 model_id 获取默认的 theory
static uint8_t get_theory_by_model_id(uint8_t model_id)
{
    int i = 0;
    if(RS485_SENSOR_MODEL_ID_DOY100 == model_id)    // DOY100 默认为探头类型为荧光法
    {
        return SENSOR_THEORY_OPTICS;
    }
    for (i = 1; i < ARRAY_LEN(model_id_mappgings); i++)
    {
        if (model_id == model_id_mappgings[i][3])
        {
            return model_id_mappgings[i][2];
        }
    }
    return SENSOR_THEORY_OTHER;
}

static void sensor_indicator_init(rs485_sensor_indicator_pt indicator)
{
    indicator->status = RS485_ERROR;
    indicator->error_code = 0;
    indicator->vm1 = 0;
    indicator->vm2 = 0;
    indicator->vm3 = 0;
    indicator->vm4 = 0;
    indicator->value1 = 0;
    indicator->value2 = 0;
    indicator->value3 = 0;
    indicator->value4 = 0;
}

// 打印 indicators_values[].sensor 的地址 
static void _show_water_indicator_sensor_value(void)
{
    app_water_indicator_value_pt water_indicator;
    for (uint8_t id = 1; id < PROBE_SENSOR_SIZE; id++)
    {
        APP_SENSORS_water_indicator_get(id, &water_indicator);
        if(water_indicator->sensor != NULL)
        {
            APP_LOG_trace("indicators_values[%d].sensor = %p\r\n", id, &water_indicator->sensor);
        }
        else
        {
            APP_LOG_trace("indicators_values[%d].sensor = NULL\r\n", id);
        }
    }
}

#define REFRESH_INIT    (-1)    // 初始化状态 
#define REFRESH_HOLD    (0)     // 未更新状态 
#define REFRESH_NEW     (1)     // 已更新状态 
static int8_t is_refresh = REFRESH_INIT;
static int64_t auto_scan_tick = REFRESH_HOLD;

#define SET_APP_CONFIG_REFRESH(status)  (is_refresh = status)
#define GET_APP_CONFIG_REFRESH()        (is_refresh)
#define SET_AUTO_SCAN_TICK(status)      (auto_scan_tick = status)
#define GET_AUTO_SCAN_TICK()            (auto_scan_tick)
#define SET_UPDATE_MESSAGE_TICK(status) (update_message_tick = status)
#define GET_UPDATE_MESSAGE_TICK()       (update_message_tick)

// 设置传感器配置更新的标志
void APP_SENSORS_refresh_new(void)
{
    SET_APP_CONFIG_REFRESH(REFRESH_NEW);
}

#ifdef USE_NEW_SENSOR_SCAN_DRIVER    
int DeinitSensorIndicator(uint8_t index)
{
    if(index < 1)
    {
        return APP_ERROR;
    }
    rs485_sensor_pt pSensor = &sensors[index - 1];
    sensors_init(index-1);
    if (RS485_DRIVER_SENSOR_ID_DO == pSensor->id)
    {
        indicators_value_init(WATER_INDICATOR_INDEX_DO);
        indicators_value_init(WATER_INDICATOR_INDEX_TEMPERATURE);
    }
    else if (RS485_DRIVER_SENSOR_ID_PH == pSensor->id)
    {
        indicators_value_init(WATER_INDICATOR_INDEX_PH);
    }
    else if(RS485_DRIVER_SENSOR_ID_LEVEL == pSensor->id)
    {
        indicators_value_init(WATER_INDICATOR_INDEX_LEVEL);
    }
    else if(RS485_DRIVER_SENSOR_ID_NH3 == pSensor->id)
    {
        indicators_value_init(WATER_INDICATOR_INDEX_NH3);
    }
    else if(RS485_DRIVER_SENSOR_ID_COD == pSensor->id)
    {
        indicators_value_init(WATER_INDICATOR_INDEX_COD);
    }
    return APP_OK;
}
#endif

// 根据传感器配置情况，更新当前传感器匹配信息
void APP_SENSORS_refresh_status(void)
{
    app_config_sensor_pt config_sensor = NULL;
    int i = 0;
    uint8_t model_id = 0;
    // uint8_t index = 0;
    if(REFRESH_HOLD != GET_APP_CONFIG_REFRESH())
    {
        if(REFRESH_INIT == GET_APP_CONFIG_REFRESH())
        {
            SET_AUTO_SCAN_TICK(REFRESH_INIT);
        }
        else if(REFRESH_NEW == GET_APP_CONFIG_REFRESH())
        {
            SET_AUTO_SCAN_TICK(REFRESH_NEW);
        }
        SET_APP_CONFIG_REFRESH(REFRESH_HOLD);        
        APP_LOG_debug("refresh status start\r\n");
#ifdef USE_NEW_SENSOR_SCAN_DRIVER        
        APP_CONFIG_sensor_load();
#endif
        for (i = 1; i < PROBE_SENSOR_SIZE; i++)
        {
            config_sensor = &app_config_sensor[0][i];
            sensors[i - 1].indicator = &sensor_indicators[i - 1];   // 绑定实体数据
            if (HAS_CONFIG_IS_VALID(config_sensor))
            {
                sensors[i - 1].enable = RS485_ENABLE;
                sensors[i - 1].port = &rs485_port;
                sensors[i - 1].model = get_model_id_by_manufacturer_and_theory(sensors[i - 1].id, config_sensor->manufacturer, config_sensor->model);
                APP_LOG_debug("sensor config model_id = %d\r\n", sensors[i - 1].model);
                if (0 == sensors[i - 1].model)
                {
                    // 设置传感器同类型的默认驱动 
                    switch (sensors[i - 1].id)
                    {
                    case RS485_DRIVER_SENSOR_ID_NH3:
                        sensors[i - 1].model = RS485_SENSOR_MODEL_ID_NHN760;
                        break;
                    case RS485_DRIVER_SENSOR_ID_COD:
                        sensors[i - 1].model = RS485_SENSOR_MODEL_ID_COD861;
                        break;
                    default:
                        break;
                    }
                }
            }
            else
            {
#ifdef USE_NEW_SENSOR_SCAN_DRIVER                
                DeinitSensorIndicator(i);
#else
                sensors_init(i-1);
                if (sensors[i - 1].id == RS485_DRIVER_SENSOR_ID_DO)
                {
                    indicators_value_init(WATER_INDICATOR_INDEX_DO);
                    indicators_value_init(WATER_INDICATOR_INDEX_TEMPERATURE);
#ifdef SUPPORT_TFTLCD        
                    g_lcdSensorShow.lcdSensorStatus[WATER_INDICATOR_INDEX_TEMPERATURE] = SENSOR_UNKNOW_STATE;
                    g_lcdSensorShow.lcdSensorDataStatus[WATER_INDICATOR_INDEX_TEMPERATURE] = SENSOR_DATA_UNCHANGE;
                    g_lcdSensorShow.lcdSensorStatus[WATER_INDICATOR_INDEX_DO] = SENSOR_UNKNOW_STATE;
                    g_lcdSensorShow.lcdSensorDataStatus[WATER_INDICATOR_INDEX_DO] = SENSOR_DATA_UNCHANGE;
#endif                    
                }
                else if (sensors[i - 1].id == RS485_DRIVER_SENSOR_ID_PH)
                {
                    indicators_value_init(WATER_INDICATOR_INDEX_PH);
#ifdef SUPPORT_TFTLCD        
                    g_lcdSensorShow.lcdSensorStatus[WATER_INDICATOR_INDEX_PH] = SENSOR_UNKNOW_STATE;
                    g_lcdSensorShow.lcdSensorDataStatus[WATER_INDICATOR_INDEX_PH] = SENSOR_DATA_UNCHANGE;
#endif                    
                }
                else if(sensors[i - 1].id == RS485_DRIVER_SENSOR_ID_LEVEL)
                {
                    indicators_value_init(WATER_INDICATOR_INDEX_LEVEL);
                }
                else if(RS485_DRIVER_SENSOR_ID_NH3 == sensors[i - 1].id)
                {
                    indicators_value_init(WATER_INDICATOR_INDEX_NH3);
                }
                else if(RS485_DRIVER_SENSOR_ID_COD == sensors[i - 1].id)
                {
                    indicators_value_init(WATER_INDICATOR_INDEX_COD);
                }
#endif
            }
        } 
        APP_LOG_debug("refresh status end\r\n");
    }
}

int APP_SENSORS_init(void)
{
    BSP_RS485_init();
    int i = 0;

#ifdef SUPPORT_L6
    APP_L6_init();
    rs485_l6_status.reset_count = 0;
#endif
    for (i = 0; i < APP_SENSORS_MAX_INDICATOR_SIZE; i++)
    {
        indicators_value_init(i);
    }
    for (i = 0; i < ARRAY_LEN(sensor_indicators); i++)
    {
        sensor_indicator_init(&sensor_indicators[i]);
    }
    
#ifdef SUPPORT_TFTLCD   
    app_config_sensor_pt config_sensor = NULL;
    for (i = 1; i < PROBE_SENSOR_SIZE; i++)
    {
        config_sensor = &app_config_sensor[0][i];
        if (HAS_CONFIG_IS_VALID(config_sensor))
        {
            if(RS485_DRIVER_SENSOR_ID_DO == i)
            {
                g_lcdSensorShow.lcdSensorStatus[WATER_INDICATOR_INDEX_TEMPERATURE] = SENSOR_INITING_STATE;
                g_lcdSensorShow.lcdSensorDataStatus[WATER_INDICATOR_INDEX_TEMPERATURE] = SENSOR_DATA_UNCHANGE;
            }
            g_lcdSensorShow.lcdSensorStatus[i] = SENSOR_INITING_STATE;
            g_lcdSensorShow.lcdSensorDataStatus[i] = SENSOR_DATA_UNCHANGE;
            g_lcdSensorShow.sensorHasConfig |= SENSOR_EEPROM_CONFIG;
            APP_LOG_trace("i:%d, init SENSOR_INITING_STATE\r\n", i);
        }
    }
#endif                
    return APP_OK;
}

#ifdef PH202_ADC_DEBUG   
int16_t *do_vm = NULL;
static int _ph_adc_upload_debug(uint8_t id, rs485_sensor_indicator_pt indicator)
{
    app_water_indicator_value_pt water_indicator;
    int rc = APP_SENSORS_water_indicator_get(id, &water_indicator);
    APP_CHECK_RC(rc)
    water_indicator->sensor = indicator->sensor;
    if(1 == id)
    {
        do_vm = &(water_indicator->vm);
        APP_LOG_trace("do_vm = 0x%08x\r\n", do_vm);
    }
    else if(2 == id)
    {
        APP_LOG_trace("get do_vm = 0x%08x\r\n", do_vm);
        if(NULL != do_vm)
        {
            *do_vm = indicator->vm2;
            APP_LOG_trace("set *do_vm = %d\r\n", *do_vm);
        }
        else
        {
            return APP_ERROR;
        }
    }
    return APP_OK;
}
#endif

static void _show_sensor_indicator(rs485_sensor_indicator_pt indicator)
{
    if (indicator->id == RS485_DRIVER_SENSOR_ID_DO)
    {
        __RECORD_INDICATOR(WATER_INDICATOR_INDEX_DO, value2, vm2);
        __RECORD_INDICATOR(WATER_INDICATOR_INDEX_TEMPERATURE, value1, vm1);
    }
    else if (indicator->id == RS485_DRIVER_SENSOR_ID_PH)
    {
        __RECORD_INDICATOR(WATER_INDICATOR_INDEX_PH, value1, vm1);
    }
    else if(indicator->id == RS485_DRIVER_SENSOR_ID_LEVEL)
    {
        __RECORD_INDICATOR(WATER_INDICATOR_INDEX_LEVEL, value1, vm1);
    }
    else if(indicator->id == RS485_DRIVER_SENSOR_ID_LEVEL)
    {
        __RECORD_INDICATOR(WATER_INDICATOR_INDEX_LEVEL, value1, vm1);
    }
    else if(indicator->id == RS485_DRIVER_SENSOR_ID_NH3)
    {
        __RECORD_INDICATOR(WATER_INDICATOR_INDEX_NH3, value1, vm1);
    }
    else if(indicator->id == RS485_DRIVER_SENSOR_ID_COD)
    {
        __RECORD_INDICATOR(WATER_INDICATOR_INDEX_COD, value1, vm1);
    }
    
#ifdef PH202_ADC_DEBUG  
    if(indicator->id == RS485_DRIVER_SENSOR_ID_DO)
    {
        _ph_adc_upload_debug(WATER_INDICATOR_INDEX_DO, indicator);
    } 
    else if(indicator->id == RS485_DRIVER_SENSOR_ID_PH)
    {
        _ph_adc_upload_debug(WATER_INDICATOR_INDEX_PH, indicator);
    }
#endif
}

static void _set_sensor_default_message(rs485_sensor_pt sensor, uint8_t theory)
{
    uint8_t *sn = sensor->SN;
    uint16_t sn_code = 0;
    uint8_t sn_id = SENSOR_SN_ID_DEFAULT;
    memset(sn, 0, 8);
    memset(sensor->hardware, 0, 3);
    memset(sensor->firmware, 0, 3);
    
    if(APP_SENSOR_IS_ENABLE(sensor))
    {
        switch(sensor->model)
        {
            case RS485_SENSOR_MODEL_ID_DOY100:
                if(SENSOR_THEORY_MEMBRANE == theory)
                {
                    // 特殊处理, DO-Y100 全部认定为荧光法 
                    // app_config_sensor[0][0].model = SENSOR_THEORY_OPTICS;
                    // app_config_sensor[0][sensor->id].model = SENSOR_THEORY_OPTICS;
                    sn_code = SENSOR_SN_CODE_DOY100;
                }
                else
                {
                    sn_code = SENSOR_SN_CODE_DOY102;
                }
                break;
            case RS485_SENSOR_MODEL_ID_PH201:
                sn_code = SENSOR_SN_CODE_PH201;
                break;
            case RS485_SENSOR_MODEL_ID_DOY121:
                sn_code = SENSOR_SN_CODE_DOY121;
                break;
            case RS485_SENSOR_MODEL_ID_PH230:
                sn_code = SENSOR_SN_CODE_PH230;
                break;
            case RS485_SENSOR_MODEL_ID_DOY170:
                sn_code = SENSOR_SN_CODE_DOY170;
                break;
            case RS485_SENSOR_MODEL_ID_WL991:
                sn_code = SENSOR_SN_CODE_WL991;
                break;
            case RS485_SENSOR_MODEL_ID_PH280:
                sn_code = SENSOR_SN_CODE_PH280;
                break;
            case RS485_SENSOR_MODEL_ID_SAL610:
                sn_code = SENSOR_SN_CODE_SAL610;
                break;
            case RS485_SENSOR_MODEL_ID_ORP320:
                sn_code = SENSOR_SN_CODE_ORP320;
                break;
            case RS485_SENSOR_MODEL_ID_ORP620:
                sn_code = SENSOR_SN_CODE_ORP620;
                break;
            case RS485_SENSOR_MODEL_ID_PH260:
                sn_code = SENSOR_SN_CODE_PH260;
                break;
            case RS485_SENSOR_MODEL_ID_DOY123:
                sn_code = SENSOR_SN_CODE_DOY123;
                break;
            case RS485_SENSOR_MODEL_ID_NHN710:
                sn_code = SENSOR_SN_CODE_NHN710;
                break;
            case RS485_SENSOR_MODEL_ID_NHN760:
                sn_code = SENSOR_SN_CODE_NHN760;
                break;
            case RS485_SENSOR_MODEL_ID_COD861:
                sn_code = SENSOR_SN_CODE_COD861;
                break;
            default:
                break;
        }
        switch(sensor->id)
        {
            case RS485_DRIVER_SENSOR_ID_DO:
                sn_id = SENSOR_SN_ID_DO;
                break;
            case RS485_DRIVER_SENSOR_ID_PH:
                sn_id = SENSOR_SN_ID_PH;
                break;
            case RS485_DRIVER_SENSOR_ID_NH3:
                sn_id = SENSOR_SN_ID_NH3;
                break;
            case RS485_DRIVER_SENSOR_ID_COD:
                sn_id = SENSOR_SN_ID_COD;
                break;
            case RS485_DRIVER_SENSOR_ID_LEVEL:
                sn_id = SENSOR_SN_ID_LEVEL;
                break;
            default:
                break;
        }
    }

    write_uint16_t_BE(sn_code, &sn[0]);
    write_uint8_t_BE(sn_id, &sn[2]);
}
#ifdef USE_NEW_SENSOR_SCAN_DRIVER    
// 当使用EEPROM中的传感器配置且未接入传感器时，调用该函数匹配驱动并设置默认值
int SensorSetDriverWithoutConnect(rs485_sensor_pt sensor, uint8_t sensor_id)
{
    app_config_sensor_pt config = &app_config_sensor[0][sensor_id];
    APP_LOG_debug("[%s], has_config = %d\r\n", __func__, config->has_config);
    if(HAS_CONFIG_IS_VALID(config))
    {
        uint8_t theory = app_config_sensor[0][sensor_id].model;
        sensor->enable = RS485_ENABLE;
        RS485_DRIVER_match_by_model(sensor, theory);
        // 设置默认的SN、软硬件版本号 
        _set_sensor_default_message(sensor, theory);
        return APP_OK;
    }
    else if(GET_AUTO_SCAN_TICK() > REFRESH_NEW) // 仅当周期扫描且没有配置该传感器时，重新反初始化传感器 
    {
        APP_LOG_debug("[DeinitSensorIndicator], sensor_id = %d\r\n", sensor_id);
        DeinitSensorIndicator(sensor_id);
        return APP_ERROR;
    }
}

// 当接入了传感器但不能识别时，调用该函数设置默认驱动 
int SensorSetDriverDefault(rs485_sensor_pt sensor)
{
    APP_LOG_debug("[%s]\r\n", __func__);
    int rc = RS485_DRIVER_match_default(sensor);
    APP_CHECK_RC(rc)
    uint8_t theory = sensor->model;
    sensor->enable = RS485_ENABLE;
    _set_sensor_default_message(sensor, theory);
    APP_LOG_debug("Run setDefaultDriver, id = %d\r\n", sensor->id);
    return APP_OK;
}

// 扫描并匹配传感器，获取传感器信息(版本号/PN/SN) 
static int _rs485_auto_scan_sensor(uint8_t sensor_id)
{
    // sensor.id = sensor_id;
    int rc = RS485_DRIVER_match(sensor);    // 先用中易的协议试看是否能匹配 
    // 如果是读超时(没有接传感器)，不要用单个的协议扫描  
    if ((RS485_READ_ERROR == rc) && (REFRESH_HOLD != GET_AUTO_SCAN_TICK()))  
    {
        SensorSetDriverWithoutConnect(sensor, sensor_id);
        return RS485_READ_ERROR;
    }
    if (RS485_ERROR == rc)  // 能读到数据，但不符合要求 
    {
        rc = RS485_DRIVER_scan(sensor);
        if ((RS485_READ_ERROR == rc) && (REFRESH_HOLD != GET_AUTO_SCAN_TICK()))  
        {
            SensorSetDriverWithoutConnect(sensor, sensor_id);
            return RS485_READ_ERROR;
        }
        else if(RS485_ERROR == rc)
        {
            SensorSetDriverDefault(sensor);
            return RS485_ERROR;
        }
    }
    rs485_sensor_action_pt driver_action = sensor->action;
    if (NULL != driver_action->read_info)
    {
        driver_action->read_info(sensor);
        // HARDWARE_OS_DELAY_MS(200);
    }
    sensor->enable = RS485_ENABLE;
    return RS485_OK;
}
#else

// 扫描并匹配传感器，获取传感器信息(版本号/PN/SN) 
static int _rs485_auto_scan_sensor(uint8_t sensor_id)
{
    // sensor.id = sensor_id;
    int rc = RS485_DRIVER_match(sensor);    // 先用中易的协议试看是否能匹配 
    if (rc == RS485_READ_ERROR)             // 如果是读超时(没有接传感器)，不要用单个的协议扫描 
    {
        if(REFRESH_HOLD != GET_AUTO_SCAN_TICK())    // 当使用EEPROM中的传感器配置且未接入传感器时，才调用该函数匹配驱动并设置默认值 
        {
            uint8_t theory = app_config_sensor[0][sensor_id].model;
            rc = RS485_DRIVER_match_by_model(sensor, theory);
#ifdef SUPPORT_TFTLCD
            if(RS485_OK == rc)
            {
                s_findSensor |= 0x1;
                if(RS485_DRIVER_SENSOR_ID_DO == sensor_id)
                {
                    g_lcdSensorShow.lcdSensorStatus[WATER_INDICATOR_INDEX_TEMPERATURE] = SENSOR_INITING_STATE;
                    g_lcdSensorShow.lcdSensorDataStatus[WATER_INDICATOR_INDEX_TEMPERATURE] = SENSOR_DATA_UNCHANGE;
                    g_lcdSensorShow.lcdSensorStatus[WATER_INDICATOR_INDEX_DO] = SENSOR_INITING_STATE;
                    g_lcdSensorShow.lcdSensorDataStatus[WATER_INDICATOR_INDEX_DO] = SENSOR_DATA_UNCHANGE;
                }
                else if(RS485_DRIVER_SENSOR_ID_PH == sensor_id)
                {
                    g_lcdSensorShow.lcdSensorStatus[WATER_INDICATOR_INDEX_PH] = SENSOR_INITING_STATE;
                    g_lcdSensorShow.lcdSensorDataStatus[WATER_INDICATOR_INDEX_PH] = SENSOR_DATA_UNCHANGE;
                }
                APP_LOG_trace("sensor_id:%d, match_by_model SENSOR_INITING_STATE\r\n", sensor_id);
            }
#endif 
            // 设置默认的SN、软硬件版本号 
            _set_sensor_default_message(sensor, theory);
        }
        return RS485_READ_ERROR;
    }
    if (rc != RS485_OK)
    {
        rc = RS485_DRIVER_scan(sensor);
    }
    RS485_CHECK_RC(rc)
    rs485_sensor_action_pt driver_action = sensor->action;
    if (NULL != driver_action->read_info)
    {
        driver_action->read_info(sensor);
        // HARDWARE_OS_DELAY_MS(200);
    }
    sensor->enable = RS485_ENABLE;
#ifdef SUPPORT_TFTLCD
    if(RS485_DRIVER_SENSOR_ID_DO == sensor_id)
    {
        g_lcdSensorShow.lcdSensorStatus[WATER_INDICATOR_INDEX_TEMPERATURE] = SENSOR_INITING_STATE;
        g_lcdSensorShow.lcdSensorDataStatus[WATER_INDICATOR_INDEX_TEMPERATURE] = SENSOR_DATA_UNCHANGE;
        g_lcdSensorShow.lcdSensorStatus[WATER_INDICATOR_INDEX_DO] = SENSOR_INITING_STATE;
        g_lcdSensorShow.lcdSensorDataStatus[WATER_INDICATOR_INDEX_DO] = SENSOR_DATA_UNCHANGE;
    }
    else if(RS485_DRIVER_SENSOR_ID_PH == sensor_id)
    {
        g_lcdSensorShow.lcdSensorStatus[WATER_INDICATOR_INDEX_PH] = SENSOR_INITING_STATE;
        g_lcdSensorShow.lcdSensorDataStatus[WATER_INDICATOR_INDEX_PH] = SENSOR_DATA_UNCHANGE;
    }
    APP_LOG_trace("sensor_id:%d, match/scan SENSOR_INITING_STATE\r\n", sensor_id);
#endif    
    return RS485_OK;
}
#endif

// 更新传感器配置数据，并与EEPROM中同步
static int _update_sensor_config(uint8_t id, uint8_t enable, uint8_t manufacturer, uint8_t type)
{
    app_config_sensor_t sensorConfig;
    int rc = 0;
    APP_CONFIG_sensor_init(&sensorConfig);
    if(1 == enable)
    {
        sensorConfig.has_config = APP_CONFIG_HAS_CONFIG;
        sensorConfig.enable = APP_CONFIG_ENABLED;
        sensorConfig.manufacturer = manufacturer;
        sensorConfig.model = type;
    }
    if(RS485_DRIVER_SENSOR_ID_DO == id)
    {
        rc = APP_CONFIG_sensor_write(PROBE_SIZE - 1, 0, &sensorConfig);
    }
    rc = APP_CONFIG_sensor_write(PROBE_SIZE - 1, id, &sensorConfig);
    return rc;
}

static int _update_sensor_data_config(uint8_t id, uint8_t enable, uint8_t manufacturer, uint8_t type)
{
    int rc = 0;
    app_config_sensor_pt p_app_config_sensor;
    p_app_config_sensor = &app_config_sensor[0][id];
    if(APP_CONFIG_ENABLED == enable)
    {
        p_app_config_sensor->has_config = APP_CONFIG_HAS_CONFIG;
        p_app_config_sensor->enable = APP_CONFIG_ENABLED;
        p_app_config_sensor->manufacturer = manufacturer;
        p_app_config_sensor->model = type;
        if(RS485_DRIVER_SENSOR_ID_DO == id)
        {
            p_app_config_sensor = &app_config_sensor[0][0];
            p_app_config_sensor->has_config = APP_CONFIG_HAS_CONFIG;
            p_app_config_sensor->enable = APP_CONFIG_ENABLED;
            p_app_config_sensor->manufacturer = manufacturer;
            p_app_config_sensor->model = type;
        }
    }
    return rc;
}

// 上报传感器(配置)PN值
static int _upload_sensor_config(void)
{
    // return G2_SERVER_upload_sensor_config_message_process(PROBE_SIZE);
    return G2_SERVER_upload_device_sensors_message_process();
}

// 设置要扫描传感器的地址
static uint8_t sensor_ids[] = 
{
    RS485_DRIVER_SENSOR_ID_DO, 
    RS485_DRIVER_SENSOR_ID_PH, 
#ifdef ENABLE_RS485_DIRVER_SENSOR_ANBULEILA_WL991
    RS485_DRIVER_SENSOR_ID_LEVEL
#endif
#ifdef ENABLE_RS485_DRIVER_SENSORS_CHEMINS_NHN760 
    RS485_DRIVER_SENSOR_ID_NH3, 
#endif
#ifdef ENABLE_RS485_DRIVER_SENSORS_CHEMINS_COD861 
    RS485_DRIVER_SENSOR_ID_COD, 
#endif
};
// 选择要扫描的传感器dev ID 
static rs485_sensor_pt _select_sensor_by_id(uint8_t id)
{
    uint8_t i = 0;
    for(i=0; i<ARRAY_LEN(sensors); i++)
    {
        if(id == sensors[i].id)
        {
            return (&sensors[i]);
        }
    }
    return NULL;
}

// 扫描并识别外部接入的传感器,上报传感器信息给服务器 
static int _rs485_auto_scan_sensors(void)
{
    uint8_t sensor_change = 0;
    int rc = 0;
    static int64_t update_message_tick = REFRESH_NEW;
    static uint32_t update_message_count = 0;
    static uint8_t update_message_flag = 0;
    // if(REFRESH_INIT == GET_AUTO_SCAN_TICK()
    if((REFRESH_INIT == GET_AUTO_SCAN_TICK() && HARDWARE_GET_TICK() > 30 * 1000)
    || REFRESH_NEW == GET_AUTO_SCAN_TICK()    // 上电重启 or “传感器配置”后执行一次扫描 
    || (HARDWARE_GET_TICK() - GET_AUTO_SCAN_TICK() >= SCAN_SENSOR_DRIVER_CYCLE_TIME * 60 * 1000))                  // 周期型执行一次扫描 
    {
        APP_LOG_debug("scan sensors driver\r\n");
        // if(REFRESH_HOLD != GET_AUTO_SCAN_TICK())
        // {
        //     HARDWARE_OS_DELAY_MS(1000);
        // }
#ifdef SUPPORT_TFTLCD
        SET_PAUSE_CONFIG();
#endif    
        for (int i = 0; i < ARRAY_LEN(sensor_ids); i++)
        {
            sensor = _select_sensor_by_id(sensor_ids[i]);
            if(NULL != sensor)
            {
                sensor->port = &rs485_port;
                rc = _rs485_auto_scan_sensor(sensor->id);
                if(REFRESH_HOLD != GET_AUTO_SCAN_TICK())
                {
                    APP_SENSORS_set_compensations_by_sensor(sensor);
                }
                if(RS485_OK == rc) 
                {
#ifdef SUPPORT_TFTLCD        
                    s_findSensor |= 0x2;
#endif 
                    if(0 != sensor->sensor_change)
                    {
                        sensor_change++;
                        sensor->sensor_change = 0;
                    }
                    _update_sensor_data_config(sensor->id, APP_CONFIG_ENABLED, sensor->manufacturer, get_theory_by_model_id(sensor->model));      
                }
                HARDWARE_OS_DELAY_MS(1000);
            }
        }
#ifdef SUPPORT_TFTLCD
        CLR_PAUSE_CONFIG();
        CLR_EEPROM_CONFIG();
        CLR_DRIVER_CONFIG();
        if(0x2 & s_findSensor)
        {
            SET_DRIVER_CONFIG();
        }
        if(0x1 & s_findSensor)
        {
            SET_EEPROM_CONFIG();
        }
        s_findSensor = 0;
#endif
        // _show_water_indicator_sensor_value();
        if(REFRESH_NEW == GET_AUTO_SCAN_TICK() || (0 != sensor_change))
        {
            update_message_flag = 1;
        }
        SET_AUTO_SCAN_TICK(HARDWARE_GET_TICK());
    }
    // 上报传感器信息逻辑流程 
    if(APP_SERVER_CONNECTED())
    {
        if((REFRESH_HOLD != GET_UPDATE_MESSAGE_TICK()) && (HARDWARE_GET_TICK() - GET_UPDATE_MESSAGE_TICK() >= UPDATE_SENSOR_MSG_CYCLE_TIME * 60 *1000))
        {
            update_message_flag = 1;
        }
        if(APP_OK == _check_time_range())
        {
            update_message_count++;
            if(1 == update_message_count)
            {
                update_message_flag = 1;
            }
        }
        else
        {
            update_message_count = 0;
        }
        if(1 == update_message_flag)
        {
            update_message_flag = 0;
            SET_UPDATE_MESSAGE_TICK(REFRESH_HOLD);
            _upload_sensor_config();
        }
    }
    else
    {
        // update_message_flag = 0;
        SET_UPDATE_MESSAGE_TICK(HARDWARE_GET_TICK());
    }
    return APP_OK;
}

static uint32_t saturability_data_tick = 0;
void APP_SENSORS_task_run(void *argument)
{
    APP_SENSORS_init();
//#ifdef SUPPORT_L6
#if (defined(SUPPORT_L6) && (!defined(SUPPORT_L6_ENABLE)))
    APP_L6_start();
    HARDWARE_OS_DELAY_MS(1000);
#endif
    for (;;)
    {
        APP_SENSORS_refresh_status();
        _rs485_auto_scan_sensors();
#ifdef SUPPORT_L6
/*
气吹装置故障处理逻辑
1. 如果通信未返回或返回设备状态为故障连续超过10次，则重启L6
2. 如果重启超过3次设备任然没有好则强制读取传感器数据，不处理L6的逻辑
3. 强制读取的数据当成正常的数据
*/
#ifndef SUPPORT_L6_ENABLE
        APP_LOG_debug("air blow: err: %d, rst: %d, status: %d\r\n", rs485_l6_status.error_count, rs485_l6_status.reset_count, rs485_l6_status.status);
        APP_L6_read();
#endif
        HARDWARE_OS_DELAY_MS(1000);  
        if (!APP_L6_force_read())
        {
#if (defined(SUPPORT_POWER_OFF_SENSOR) && (!defined(SUPPORT_L6_ENABLE)))
            if (APP_L6_can_reset()) // L6是否可以复位 
            {
                POWER_SENSOR_OFF();
                HARDWARE_OS_DELAY_MS(5000);
                POWER_SENSOR_ON();
                APP_L6_init();
                rs485_l6_status.reset_count += 1;
            }
            if (!APP_L6_initialized())
            {
                continue;
            }
#endif						
            if(APP_L6_send_data_ready()) // L6 有数据可以发送 
            {
                APP_SENSORS_send_data_to_server();  // 先在数据读取完成后主动上报传感器数据 
                saturability_data_tick= HARDWARE_GET_TICK();
                rs485_l6_status.saturability_status = 1;
            }
            if(APP_L6_force_saturability_ready())   // 上报过传感器数据后，(5min后)检查 DO 传感器在空气中的饱和度值情况 
            {
                if (HARDWARE_GET_TICK() - saturability_data_tick > 5 * 60 * 1000) 
                { 
                    APP_SENSORS_check_indicator_Saturability_alert();
                } 
            }
            if ((!APP_L6_data_ready())) // L6 流程中传感器数据不可获取阶段 
            {
                continue;
            }
        }
		else // L6 非正常运行 or 传感器探头校准 
        {
            APP_SENSORS_send_data_to_server();
        }
#endif
        APP_SENSORS_set_compensations_by_sensor(&sensors[0]);
        APP_SENSORS_read_each(_show_sensor_indicator, 1000);
        HARDWARE_OS_DELAY_MS(1000);
#ifndef SUPPORT_L6
        APP_SENSORS_send_data_to_server();
#endif
    }
}

int APP_SENSORS_read_each(rs485_sensor_indicator_fun callback, uint32_t interval_delay_ms)
{
    int rc = 0;
    uint8_t index = 0;
    rs485_sensor_pt sensor = &sensors[0];
    rs485_sensor_action_pt action = NULL;
    app_water_indicator_value_pt water_indicator;
    uint8_t sensor_change = 0;

#ifdef TEST_SENSOR_DATA
    static int8_t tmp_dir = 0;
    static int8_t do_dir = 0;
    static int8_t ph_dir = 0;
#endif
    APP_LOG_trace("read each start\r\n");
    for (int i = 0; i < ARRAY_LEN(sensors); i++)
    {
        sensor = &sensors[i];
        action = sensor->action;
        if (APP_SENSOR_IS_ENABLE(sensor) && action != NULL && action->read_value1 != NULL)
        {
            sensor->indicator->id = sensor->id;
            sensor->indicator->sensor = sensor;
#ifndef TEST_SENSOR_DATA            
            rc = action->read_value1(sensor, sensor->indicator);
            APP_LOG_debug("\r\nSensors read rc %d s %d\r\n", rc, sensor->indicator->status);
            if(RS485_OK == sensor->indicator->status)          // RS485_OK
            {
                sensor->timeout_error_count = 0;
            }
            else
            {
                uint8_t compensation_id = 0;
                if(APP_OK == _get_compensation_index(sensor->id, &compensation_id))
                {
                    sensor_compensation_ticks[compensation_id] = 0;
                }
                sensor->timeout_error_count++;
                if(sensor->timeout_error_count >= 10)
                {
                    sensor->timeout_error_count = 0;
                    sensor_change++;
                    // _update_sensor_config(sensor->id, 0, sensor->manufacturer, sensor->model);
                }
            }
#else   
            if(1 == sensor->id)
            {
                if(0 == tmp_dir)
                {
                    sensor->indicator->value1 = DEF_TMP_VALUE;
                    tmp_dir = 1;
                }
                else
                {
                    if((1 == tmp_dir) && (sensor->indicator->value1 + INT_TMP_VALUE > MAX_TMP_VALUE))
                    {
                        tmp_dir = -1;
                        sensor->indicator->value1 = MAX_TMP_VALUE;
                    }
                    else if((-1 == tmp_dir) && (sensor->indicator->value1 < MIN_TMP_VALUE + INT_TMP_VALUE))
                    {
                        tmp_dir = 1;
                        sensor->indicator->value1 = MIN_TMP_VALUE;
                    }
                    else
                    {
                        sensor->indicator->value1 += (tmp_dir * INT_TMP_VALUE);
                    }
                }
                if(0 == do_dir)
                {
                    sensor->indicator->value2 = DEF_DO_VALUE;
                    do_dir = 1;
                }
                else
                {
                    if((1 == do_dir) && (sensor->indicator->value2 + INT_DO_VALUE > MAX_DO_VALUE))
                    {
                        do_dir = -1;
                        sensor->indicator->value2 = MAX_DO_VALUE;
                    }
                    else if((-1 == do_dir) && (sensor->indicator->value2 < MIN_DO_VALUE + INT_DO_VALUE))
                    {
                        do_dir = 1;
                        sensor->indicator->value2 = MIN_DO_VALUE;
                    }
                    else
                    {
                        sensor->indicator->value2 += (do_dir * INT_DO_VALUE);
                    }
                }
            }
            else if(2 == sensor->id)
            {
                
                if(0 == ph_dir)
                {
                    sensor->indicator->value1 = DEF_PH_VALUE;
                    ph_dir = 1;
                }
                else
                {
                    if((1 == ph_dir) && (sensor->indicator->value1 + INT_PH_VALUE > MAX_PH_VALUE))
                    {
                        ph_dir = -1;
                        sensor->indicator->value1 = MAX_PH_VALUE;
                    }
                    else if((-1 == ph_dir) && (sensor->indicator->value1 < MIN_PH_VALUE + INT_PH_VALUE))
                    {
                        ph_dir = 1;
                        sensor->indicator->value1 = MIN_PH_VALUE;
                    }
                    else
                    {
                        sensor->indicator->value1 += (ph_dir * INT_PH_VALUE);
                    }
                }
            }
            sensor->indicator->status = RS485_OK;
            sensor->indicator->error_code = 0;
#endif    
            if ((callback != NULL) && (NULL != sensor->action))
            {
                index = _get_indicator_index(sensor->id);
                if((WATER_INDICATOR_INDEX_LEVEL == index) && (1 == sensor->timeout_error_count))   // 规避由于液位传感器偶然读取超时，导致刷新数据提示“液位：故障”
                {;}
                else
                {
                    callback(sensor->indicator);
                }
            }
            HARDWARE_OS_DELAY_MS(interval_delay_ms);
        }
        else
        {
        }
    }
    APP_LOG_trace("read each end\r\n");
	// _show_water_indicator_sensor_value();
    return APP_OK;
}

// 根据传感器ID查找可用的传感器dev实体 
static int _find_sensor_by_sensor_id(uint8_t sensor_id, rs485_sensor_pt *vp)
{
    if (sensor_id != RS485_DRIVER_SENSOR_ID_DO && sensor_id != RS485_DRIVER_SENSOR_ID_PH)
        return APP_ERROR;
    rs485_sensor_pt sensor = &sensors[0];
    for (int i = 0; i < ARRAY_LEN(sensors); i++)
    {
        sensor = &sensors[i];
        if (sensor->id == sensor_id)
        {
            *vp = sensor;
            return APP_OK;
        }
    }
    return APP_ERROR;
}

// 校准传感器 
int APP_SENSOR_calibrate(uint8_t probe_id, uint8_t sensor_id, uint8_t type, uint8_t *params)
{
    rs485_sensor_pt sensor = NULL;
    int rc = _find_sensor_by_sensor_id(sensor_id, &sensor);
    APP_CHECK_RC(rc)
    rs485_sensor_action_pt action = sensor->action;
    if (sensor->port != NULL &&
        action != NULL &&
        APP_SENSOR_IS_ENABLE(sensor))
    {
        if (action->write_calibrate != NULL)
        {
			#ifdef SUPPORT_L6
            rs485_l6_status.force_status = 1;
			#endif
            return action->write_calibrate(sensor, type, params) == RS485_OK ? APP_OK : APP_ERROR;
        }
    }
    return APP_ERROR;
}

// 写入传感器补偿数据 
int APP_SENSOR_write_compensation(uint8_t sensor_id, uint8_t indicator_id, float value, int force)
{
    if (sensor_compensation_ticks[indicator_id] != 0 && force == 0)
    {
        return APP_ERROR;
    }
    rs485_sensor_pt sensor = NULL;
    int rc = _find_sensor_by_sensor_id(sensor_id, &sensor);
    APP_CHECK_RC(rc)
    rs485_sensor_action_pt action = sensor->action;
    if (sensor->port != NULL &&
        action != NULL &&
        APP_SENSOR_IS_ENABLE(sensor))
    {
        if (action->write_compensation != NULL)
        {
            rc = action->write_compensation(sensor, indicator_id, value) == RS485_OK ? APP_OK : APP_ERROR;
            if (rc == APP_OK)
            {
                sensor_compensation_ticks[indicator_id] = HARDWARE_GET_TICK();
            }
            return rc;
        }
    }
    return APP_ERROR;
}

// 对特定传感器执行写补偿操作 
int APP_SENSORS_set_compensations_by_sensor(rs485_sensor_pt sensor)
{
    float value = 0;
    if (sensor->id == RS485_DRIVER_SENSOR_ID_DO)
    {
        app_config_compensation_pt compensation = &app_config_compensation[0][WATER_INDICATOR_INDEX_SALINITY];
        if (HAS_CONFIG_IS_VALID(compensation))
            water_indicator_from_uint16_t(WATER_INDICATOR_INDEX_SALINITY, compensation->value, &value);
        return APP_SENSOR_write_compensation(sensor->id, WATER_INDICATOR_INDEX_SALINITY, value, 0);
    }
    return APP_OK;
}

// 设置传感器探头的告警类型
void APP_SENSORS_check_indicator_alert(int probe_id, int indicator_id, g2_server_sensor_data_indicator_message_pt indicator)
{
    app_config_indicator_alert_pt alert = &app_config_indicator_alert[probe_id][indicator_id];
    indicator->data_status = 0;
    if (!ENABLE_IS_VALID(alert))
    {
        return;
    }
    if (indicator->value > alert->max)
    {
        indicator->data_status = 0x01;
    }
    if (indicator->value < alert->min)
    {
        indicator->data_status = 0x02;
    }
}

// 比较设备时间是否在规定范围内(精确度为分钟) -1:之前; 0:之内; 1:之后 
static int sensor_time_compare(app_config_time_pt now, uint8_t hour, uint8_t minute)
{
    if (now->hour < hour)
    {
        return -1;
    }
    if (now->hour > hour)
    {
        return 1;
    }

    if (now->minute < minute)
    {
        return -1;
    }
    if (now->minute > minute)
    {
        return 1;
    }
    return 0;
}

// 检查饱和度异常的时间是否在规定时间内
void APP_SENSORS_check_indicator_Saturability_alert(void)
{
    if (APP_OK == _check_time_range())
    {
        APP_SENSORS_read_each(_show_sensor_indicator, 1000);    // 再次遍历传感器读取数据 
    }
    else
    {
#ifdef SUPPORT_L6
        rs485_l6_status.saturability_alet_status = 0;
        rs485_l6_status.saturability_data = 0;
        rs485_l6_status.saturability_count = 0;
        rs485_l6_status.saturability_status = 0;
#endif
    }
}

// 设置传感器的参考零点值(不使用)
int APP_SENSOR_write_ref(uint8_t sensor_id, uint8_t indicator_id, uint16_t value)
{
    rs485_sensor_pt sensor = NULL;
    int rc = _find_sensor_by_sensor_id(sensor_id, &sensor);
    APP_CHECK_RC(rc)
    rs485_sensor_action_pt action = sensor->action;
    if (sensor->port != NULL &&
        action != NULL &&
        APP_SENSOR_IS_ENABLE(sensor))
    {
        if (action->write_info != NULL)
        {
            rc = action->write_info(sensor, &value) == RS485_OK ? APP_OK : APP_ERROR;            
            return rc;
        }
    }
    return APP_ERROR;
}


int APP_SENSORS_read_status_protocol(uint8_t probe_id, g2_server_sensor_data_message_pt message, int *changed)
{
    if (probe_id > PROBE_SIZE || probe_id == 0)
    {
        return APP_ERROR;
    }
    *changed = 0;
    uint8_t week;
    app_water_indicator_value_pt water_indicator;
    int rc = 0;
    uint16_t u_value = 0;
    G2_SERVER_sensor_data_message_init(message);
    message->probe_id = probe_id;
    message->config = 0;
    BSP_RTC_get(&message->time.year, &message->time.month, &message->time.day, &message->time.hour, &message->time.minute, &message->time.second, &week);
#ifdef SUPPORT_PUMP
    APP_PUMP_changed(changed);
    if (app_pump.status == APP_PUMP_STATUS_ON)
    {
        message->pump_status = 0x6000;
        if (app_pump.check != APP_PUMP_CHECK_NORMAL)
            message->pump_broken_status = 0x6000;
    }
#else
    message->pump_status = 0;
    message->pump_broken_status = 0;
#endif
    for (int indicator_id = 0; indicator_id < INDICATOR_SIZE; indicator_id++)
    {
        rc = APP_SENSORS_water_indicator_get(indicator_id, &water_indicator);
        if (rc == APP_OK && water_indicator->sensor != NULL) //有传感器
        {
            message->config |= 0x8000 >> indicator_id;
            if(APP_OK == _check_sensor_probe_status(indicator_id, water_indicator, &u_value))
            {
                if(RS485_DRIVER_SENSOR_ID_NH3 == indicator_id)
                {         
                    float tempMax = 5.9, tempMin = 1.0;
                    float temp = ((float)rand() / RAND_MAX) * (tempMax - tempMin) + tempMin;  
                    u_value = (uint8_t)temp; 
                    APP_LOG_debug("NHN rand:%d\r\n", u_value) ;
                }
                message->indicators[indicator_id].value = u_value; 
                message->indicators[indicator_id].probe_status = 0;
                message->indicators[indicator_id].value_mv = water_indicator->vm;
                APP_SENSORS_check_indicator_alert(probe_id - 1, indicator_id, &message->indicators[indicator_id]);
            }
            else
            {
                message->indicators[indicator_id].probe_status = 0xe0;
#ifdef PH202_ADC_DEBUG
                if(1 == indicator_id)
                {
                    APP_LOG_trace("[0x%08x]:vm = %d\r\n", (uint32_t)(&water_indicator->vm), water_indicator->vm);
                }
                message->indicators[indicator_id].value_mv = water_indicator->vm;
#endif
            }
        }
    }
    return APP_OK;
}

// 发送传感器数据到服务器 
int APP_SENSORS_send_data_to_server(void)
{
	double saturability_value = 0;
    int changed = 0;
    g2_server_packet_t packet;
    G2_SERVER_PACKET_init(&packet);
    g2_server_sensor_data_message_t message;
    int rc = APP_SENSORS_read_status_protocol(1, &message, &changed);
#ifdef SUPPORT_L6 
	if (APP_SERVER_CONNECTED() && (APP_L6_send_data_ready() || changed || HARDWARE_GET_TICK() - send_data_tick > UPLOAD_SENSOR_DATA_CYCLE_TIME * 60 * 1000))
#else 
    if (APP_SERVER_CONNECTED() && (changed || HARDWARE_GET_TICK() - send_data_tick > UPLOAD_SENSOR_DATA_CYCLE_TIME * 60 * 1000))
#endif
    {
        send_data_tick = HARDWARE_GET_TICK();
#ifdef SUPPORT_L6 
        rs485_l6_status.send_status = 0;
        if(!APP_L6_is_broken())
        {
            saturability_value = 468/(message.indicators[0].value/10+31.6);
            saturability_value = message.indicators[1].value/saturability_value;
            rs485_l6_status.saturability[count_tick] = (uint16_t)saturability_value; 
            count_tick++;
            if(count_tick >= Saturability_max_capacity)
            {
                count_tick = 0;	
                delta_saturability_data = max_min_get(rs485_l6_status.saturability, Saturability_max_capacity);
                if(delta_saturability_data < 10)    // 若 DO 传感器长时间处在空气中，则饱和度变化较小(<10) 
                {
                    // 判断 DO 传感器是否使能+探头是否正常
                    rs485_sensor_pt sensor = &sensors[0];
                    if(APP_SENSOR_IS_ENABLE(sensor) && (0 == message.indicators[1].probe_status))
                    {
                        rs485_l6_status.air_alet_status = 1;
                        APP_LOG_info("air_alet_status=%d \r\n", rs485_l6_status.air_alet_status);
                        send_self_diagnosis_to_server();
                    }
                }
                memset(rs485_l6_status.saturability, 0, Saturability_max_capacity);	
            }
            APP_LOG_info("saturability_value=%d \r\n", (uint16_t)saturability_value);
            APP_LOG_info("DO probe_status=%02X\r\n", message.indicators[1].probe_status);
            
            if (APP_OK == _check_time_range() && 0 == message.indicators[1].probe_status)   // 时间符合并且 DO 传感器探头正常 
            {  
                message.indicators[0].value_mv = rs485_l6_status.saturability_data;
                message.indicators[1].value_mv = rs485_l6_status.saturability_count;
            }
        }
#endif
        BSP_PROTOCOL_send_read_sensor_data_message(&packet, &message);
#ifndef SUPPORT_IRED
        gLcdLightOn = 1;
#endif        
    }
    return PROTOCOL_OK;
}

int APP_SENSORS_sensor_get(uint8_t addr, rs485_sensor_t **vp)
{
    rs485_sensor_pt sensor = &sensors[0];
    for (int i = 0; i < ARRAY_LEN(sensors); i++)
    {
        sensor = &sensors[i];
        if (sensor->id == addr && sensor->port != NULL && APP_SENSOR_IS_ENABLE(sensor))
        {
            *vp = sensor;
            return APP_OK;
        }
    }
    return APP_ERROR;
}

// 自诊断结果打包 
int APP_SELF_DIAGNOSIS_pack(g2_server_self_diagnosis_message_pt message)
{
#ifdef SUPPORT_L6
    if (rs485_l6_status.saturability_alet_status == 1)
    {
        message->type = 1;
        message->status = 1;
		rs485_l6_status.saturability_alet_status = 0;
    }
	if (rs485_l6_status.air_alet_status == 1)
    {
        message->type = 2;
        message->status = 1;
		rs485_l6_status.air_alet_status =0;
    }
#endif
    return APP_OK;
}

// 发送 DO传感器自诊断状态
void send_self_diagnosis_to_server(void)
{
    g2_server_packet_t packet;
    G2_SERVER_PACKET_init(&packet);
    g2_server_self_diagnosis_message_t message;
    G2_SERVER_self_diagnosis_message_init(&message);
    APP_SELF_DIAGNOSIS_pack(&message);
    BSP_PROTOCOL_send_read_self_diagnosis_message(&packet, &message);
}

// 拷贝传感器 device 信息
static void _copy_sensor_info(uint8_t *des, uint8_t *src, uint8_t len)
{
    if(NULL != src)
    {
        memcpy(des, src, len);
    }
    else
    {
        memset(des, 0, len);
    }
}

// 读取可用传感器的 pn 
int APP_SENSORS_read_device_sensors(g2_server_device_sensors_message_pt message)
{
    rs485_sensor_pt sensor = NULL;
    g2_server_sensor_info_message_pt sensor_info = NULL;
    for(uint8_t i=0; i < ARRAY_LEN(sensors); i++)
    {
        sensor = &sensors[i];
        sensor_info = &message->sensors_info[i+1];
        if(APP_SENSOR_IS_ENABLE(sensor) && sensor->PN)
        {
            message->config |= 0x8000 >> (sensor->id);
            _copy_sensor_info(sensor_info->hardware, sensor->hardware, sizeof(sensor_info->hardware));
            _copy_sensor_info(sensor_info->firmware, sensor->firmware, sizeof(sensor_info->firmware));
            _copy_sensor_info(sensor_info->PN, sensor->PN, strlen(sensor->PN));
            _copy_sensor_info(sensor_info->SN, sensor->SN, sizeof(sensor_info->SN));
            if(RS485_DRIVER_SENSOR_ID_DO == sensor->id)
            {
                sensor_info = &message->sensors_info[0];
                message->config |= 0x8000 >> 0;
                _copy_sensor_info(sensor_info->hardware, sensor->hardware, sizeof(sensor_info->hardware));
                _copy_sensor_info(sensor_info->firmware, sensor->firmware, sizeof(sensor_info->firmware));
                _copy_sensor_info(sensor_info->PN, sensor->PN, strlen(sensor->PN));
                _copy_sensor_info(sensor_info->SN, sensor->SN, sizeof(sensor_info->SN));
            }
        }
    }
    return APP_OK;
}

