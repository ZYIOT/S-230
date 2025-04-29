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

#define RECORD_INDICATOR(_idx, _value_field, _vm_filed) RecordWaterIndicator(_idx, indicator->_value_field, indicator->_vm_filed, indicator)

const uint8_t c_modelIdMappings[][4] = {
    // id 、manufacturer、model(type/theory)、model_id
    {RS485_DRIVER_SENSOR_ID_DO, RS485_DRIVER_DO_MANUFACTURER_ZYIOT, SENSOR_THEORY_MEMBRANE, RS485_SENSOR_MODEL_ID_DOY100},
    {RS485_DRIVER_SENSOR_ID_DO, RS485_DRIVER_DO_MANUFACTURER_ZYIOT, SENSOR_THEORY_OPTICS, RS485_SENSOR_MODEL_ID_DOY100},
    {RS485_DRIVER_SENSOR_ID_DO, RS485_DRIVER_DO_MANUFACTURER_PROBEST, SENSOR_THEORY_OPTICS, RS485_SENSOR_MODEL_ID_DOY121},
    // {RS485_DRIVER_SENSOR_ID_DO, RS485_DRIVER_DO_MANUFACTURER_ZYIOT, SENSOR_THEORY_OPTICS, RS485_SENSOR_MODEL_ID_DOY170},
    {RS485_DRIVER_SENSOR_ID_PH, RS485_DRIVER_PH_MANUFACTURER_ZYIOT, SENSOR_THEORY_MEMBRANE, RS485_SENSOR_MODEL_ID_PH201},
    {RS485_DRIVER_SENSOR_ID_PH, RS485_DRIVER_PH_MANUFACTURER_SZKLT, SENSOR_THEORY_MEMBRANE, RS485_SENSOR_MODEL_ID_PH230},
    {RS485_DRIVER_SENSOR_ID_PH, RS485_DRIVER_PH_MANUFACTURER_CHEMINS, SENSOR_THEORY_MEMBRANE, RS485_SENSOR_MODEL_ID_PH260},
    {RS485_DRIVER_SENSOR_ID_PH, RS485_DRIVER_PH_MANUFACTURER_SZSG, SENSOR_THEORY_MEMBRANE, RS485_SENSOR_MODEL_ID_PH280},
    {RS485_DRIVER_SENSOR_ID_ORP, RS485_DRIVER_ORP_MANUFACTURER_WHBW, SENSOR_THEORY_3, RS485_SENSOR_MODEL_ID_ORP320},
    {RS485_DRIVER_SENSOR_ID_ORP, RS485_DRIVER_ORP_MANUFACTURER_CHEMINS, SENSOR_THEORY_6, RS485_SENSOR_MODEL_ID_ORP620},
    // {RS485_DRIVER_SENSOR_ID_DO, RS485_DRIVER_DO_MANUFACTURER_PROBEST, SENSOR_THEORY_OPTICS, RS485_SENSOR_MODEL_ID_DOY123},   // 与DO-Y121的厂家和方法相同 
    // {RS485_DRIVER_SENSOR_ID_COD, RS485_DRIVER_COD_MANUFACTURER_CHEMINS, SENSOR_THEORY_OPTICS, RS485_SENSOR_MODEL_ID_COD860},
    {RS485_DRIVER_SENSOR_ID_COD, RS485_DRIVER_COD_MANUFACTURER_CHEMINS, SENSOR_THEORY_8, RS485_SENSOR_MODEL_ID_COD861},
    {RS485_DRIVER_SENSOR_ID_NH3, RS485_DRIVER_NH3_MANUFACTURER_CHEMINS, SENSOR_THEORY_7, RS485_SENSOR_MODEL_ID_NHN710},
    // {RS485_DRIVER_SENSOR_ID_NH3, RS485_DRIVER_NH3_MANUFACTURER_CHEMINS, SENSOR_THEORY_7, RS485_SENSOR_MODEL_ID_NHN760},
    {RS485_DRIVER_SENSOR_ID_LEVEL, RS485_DRIVER_LEVEL_MANUFACTURER_ANBULEILA, SENSOR_THEORY_9, RS485_SENSOR_MODEL_ID_WL991},
    {RS485_DRIVER_SENSOR_ID_SALINITY, RS485_DRIVER_SALINITY_MANUFACTURER_CHEMINS, SENSOR_THEORY_6, RS485_SENSOR_MODEL_ID_SAL610},
#ifdef ENABLE_RS485_DRIVER_SENSOR_XT_DO 
    {RS485_DRIVER_SENSOR_ID_DO, RS485_DRIVER_DO_MANUFACTURER_XT, SENSOR_THEORY_OPTICS, RS485_SENSOR_MODEL_ID_DOY181},
#endif    
#ifdef ENABLE_RS485_DRIVER_SENSOR_XT_TMP_PH_NH3 
    {RS485_DRIVER_SENSOR_ID_PH, RS485_DRIVER_PH_MANUFACTURER_XT, SENSOR_THEORY_MEMBRANE, RS485_SENSOR_MODEL_ID_PH291},
#endif

    
};

static APP_WATER_IndicatorValue_t s_indicatorsValues[APP_SENSORS_MAX_INDICATOR_SIZE] = {0};
static uint8_t sensor_PN[APP_SENSORS_MAX_INDICATOR_SIZE][RS485_SENSOR_MAX_PN_LEN] = {0};
static uint8_t sensor_SN[APP_SENSORS_MAX_INDICATOR_SIZE][RS485_SENSOR_MAX_SN_LEN] = {0};
static uint8_t sensor_HW[APP_SENSORS_MAX_INDICATOR_SIZE][RS485_SENSOR_MAX_HW_LEN] = {0};
static uint8_t sensor_FW[APP_SENSORS_MAX_INDICATOR_SIZE][RS485_SENSOR_MAX_FW_LEN] = {0};
// 注册驱动(可用)的传感器dev列表，依次存放 DO、PH 的 dev 结构体 
static rs485_sensor_t s_sensors[] = {
    RS485_SENSOR_NEW(RS485_DRIVER_SENSOR_ID_DO, RS485_ENABLE),
    RS485_SENSOR_NEW(RS485_DRIVER_SENSOR_ID_PH, RS485_ENABLE),
    RS485_SENSOR_NEW(RS485_DRIVER_SENSOR_ID_CONDUCTIVITY, RS485_DISABLE),
    RS485_SENSOR_NEW(RS485_DRIVER_SENSOR_ID_CHLOROPHYLL, RS485_DISABLE),
    RS485_SENSOR_NEW(RS485_DRIVER_SENSOR_ID_COD, RS485_DISABLE),
    RS485_SENSOR_NEW(RS485_DRIVER_SENSOR_ID_NH3, RS485_DISABLE),
    RS485_SENSOR_NEW(RS485_DRIVER_SENSOR_ID_LEVEL, RS485_DISABLE),
};

static rs485_sensor_pt s_pSensor = NULL;   // 传感器dev指针，指向 s_sensors[]  
// 带数据的传感器dev实体列表，依次存放 DO、PH 的实体数据 
static rs485_sensor_indicator_t s_sensorIndicators[INDICATOR_SIZE] = {
    {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}
};
static uint32_t s_sendDataTick = 0; // 发送数据到服务器的时间 tick 
static uint8_t s_countTick=0;        // DO 传感器采样的饱和度数据个数 
static uint16_t s_deltaSaturabilityData = 0;    // 饱和度的最大最小差值 
static uint32_t s_sensorCompensationTicks[INDICATOR_SIZE] = {0};

extern float g_lcdSensorData[3];
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
static uint8_t GetIndicatorIndex(uint8_t sensorID)
{
    uint8_t index = 0;
    if(RS485_DRIVER_SENSOR_ID_DO == sensorID)
    {
        index = WATER_INDICATOR_INDEX_DO;
    }
    else if(RS485_DRIVER_SENSOR_ID_PH == sensorID)
    {
        index = WATER_INDICATOR_INDEX_PH;
    }
    // 此行判定需注意
    else if(RS485_DRIVER_SENSOR_ID_CONDUCTIVITY == sensorID)
    {
        index = WATER_INDICATOR_INDEX_SALINITY;
    }
    else if(RS485_DRIVER_SENSOR_ID_CHLOROPHYLL == sensorID)
    {
        index = WATER_INDICATOR_INDEX_CHLOROPHYLL;
    }
    else if(RS485_DRIVER_SENSOR_ID_COD == sensorID)
    {
        index = WATER_INDICATOR_INDEX_COD;
    }
    else if(RS485_DRIVER_SENSOR_ID_NH3 == sensorID)
    {
        index = WATER_INDICATOR_INDEX_NH3;
    }
    else if(RS485_DRIVER_SENSOR_ID_LEVEL == sensorID)
    {
        index = WATER_INDICATOR_INDEX_LEVEL;
    }
    return index;
}

static int SensorsInit(uint8_t id)
{
    s_sensors[id].port = NULL;
    s_sensors[id].id = id + 1;
    s_sensors[id].manufacturer = 0;
    s_sensors[id].model = 0;
    s_sensors[id].protocol = 0;
    s_sensors[id].status = 0;
    memset(s_sensors[id].firmware, 0, RS485_SENSOR_MAX_FW_LEN);
    memset(s_sensors[id].hardware, 0, RS485_SENSOR_MAX_HW_LEN);
    memset(s_sensors[id].SN, 0, RS485_SENSOR_MAX_SN_LEN);
    memset(s_sensors[id].PN, 0, RS485_SENSOR_MAX_PN_LEN);
    s_sensors[id].sensor_change = 0;
    s_sensors[id].action = NULL;
    s_sensors[id].enable = RS485_DISABLE;
    s_sensors[id].errorCount = 0;
    s_sensors[id].timeout_error_count = 0;
    s_sensors[id].crc_error_count = 0;
    s_sensors[id].rs485_error_code_count = 0;
    s_sensors[id].calibration_count = 0;
    memset(s_sensors[id].indicator, 0, sizeof(rs485_sensor_indicator_t));
}

// 根据传感器ID判断是否能返回正确的compensation_index 
static int GetCompensationIndex(uint8_t sensorID, uint8_t *compensation_index)
{
    if (sensorID != RS485_DRIVER_SENSOR_ID_DO)
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
static int indicatorsValueInit(uint8_t id)
{
    APP_WATER_IndicatorValue_pt water_indicator;
    int rc = APP_SENSORS_WaterIndicatorGet(id, &water_indicator);
    APP_CHECK_RC(rc)
    water_indicator->sensor = NULL;
    water_indicator->status = RS485_ERROR;
    water_indicator->value = 0;
    water_indicator->errorCount = 0;
    mean_filter_init(&water_indicator->filter, water_indicator->buffer, APP_SENSOR_INDICATOR_BUFFER_SIZE);
    mean_filter_reset(&water_indicator->filter);
    return APP_OK;
}

// 检测传感器探头状态 
static int CheckSensorProbeStatus(int indicatorID, APP_WATER_IndicatorValue_pt water_indicator, uint16_t *u_value)
{
    int rc1 = 0, rc2 = 0;
    double value = 0;
    rc1 = mean_filter_get(&water_indicator->filter, &value);
    rc2 = water_indicator_to_uint16_t(indicatorID, value, u_value);
    APP_LOG_Info("probe u_value = %d\r\n", *u_value);
    if (rc1 == APP_OK && rc2 == APP_OK && water_indicator->errorCount <= 3)
    {
        return APP_OK;
    }
    else
    {
        return APP_ERROR;
    }
}

static int CheckTimeRange(void)
{
    int rc1 = 0, rc2 = 0;
    APP_CONFIG_Time_t time;
    uint8_t week = 0;

    BSP_RTC_get(&time.year, &time.month, &time.day, &time.hour, &time.minute, &time.second, &week);
    rc1 = SensorTimeCompare(&time, 0, 0);
    rc2 = SensorTimeCompare(&time, 0, 40);
    if(rc1 == 1 && rc2 == -1)
    {
        return APP_OK;
    }
    else
    {
        return APP_ERROR;
    }
}

extern APP_CONFIG_DiagnosisMessage_t g_diagnosisMsg;
// 记录水质传感器采样数据 
static int RecordWaterIndicator(uint8_t id, float value, uint16_t vm, rs485_sensor_indicator_pt indicator)
{
    APP_WATER_IndicatorValue_pt water_indicator;
    int rc = APP_SENSORS_WaterIndicatorGet(id, &water_indicator);
    APP_CHECK_RC(rc)
    water_indicator->sensor = indicator->sensor;
	#ifdef SUPPORT_L6
    // DO传感器正常+可以读取空气中的饱和度值
	if((indicator->id==RS485_DRIVER_SENSOR_ID_DO) && APP_L6_ForceSaturabilityReady())
    {
        if(RS485_OK == indicator->status && APP_OK == CheckTimeRange())
        {
            #ifdef L6_DEBUG_TEST
            rs485_l6_status.saturability_data = 80;
            #else
            rs485_l6_status.saturability_data = (uint16_t) indicator->value3;
            #endif
            APP_LOG_Info("Saturability_data=%d \r\n", rs485_l6_status.saturability_data);
            // 与空气中的饱和度值范围进行比对，判断DO探头是否有附着
            if((rs485_l6_status.saturability_data<85)||(rs485_l6_status.saturability_data>120))
            {
                rs485_l6_status.saturability_count++;
                if(3 == rs485_l6_status.saturability_count)
                {
                    uint16_t u_value = 0;
                    if(APP_OK == CheckSensorProbeStatus(id, water_indicator, &u_value))
                    {
                        rs485_l6_status.saturability_alet_status = 1;
                        APP_CONFIG_Time_t time;
                        uint8_t week = 0;
                        g2_server_self_diagnosis_message_t message;
                        BSP_RTC_get(&time.year, &time.month, &time.day, &time.hour, &time.minute, &time.second, &week);
                        AppSelfDiagnosisPack(&message);
                        memcpy((uint8_t *)(&g_diagnosisMsg.msgTime), (uint8_t *)&time, sizeof(APP_CONFIG_Time_t));
                        g_diagnosisMsg.msgType = message.type;
                        g_diagnosisMsg.msgStatus = message.status;
                        APP_CONFIG_DiagnosisMessageWrite(&g_diagnosisMsg);
                    }                
                    APP_LOG_Info("Saturability_alet_status=%d \r\n", rs485_l6_status.saturability_alet_status);
                } 
            }
            else 
            {
                rs485_l6_status.saturability_alet_status = 0;
            }
            APP_LOG_Info("saturability_count=%d \r\n", rs485_l6_status.saturability_count);
        }
    }
    rs485_l6_status.saturability_status = 0;
	#endif
    if (indicator->status != RS485_OK)
    {
        water_indicator->status = indicator->status;
        mean_filter_out(&water_indicator->filter);
        water_indicator->errorCount++;
#ifdef SUPPORT_TFTLCD
        APP_LOG_Warn("id:%d, errcnt:%d\r\n", id, water_indicator->errorCount);
        if(water_indicator->errorCount >= 3)
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
            water_indicator->errorCount = 0;
            water_indicator->value = value;
            water_indicator->vm = vm;
            water_indicator->status = RS485_OK;
            mean_filter_add(&water_indicator->filter, value);
#ifdef SUPPORT_TFTLCD        
            g_lcdSensorData[id] = value;
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
            APP_CONFIG_IndicatorAlert_pt alert = &g_appConfigIndicatorAlert[0][id];
            if(!ENABLE_IS_VALID(alert))
            {
            }
            else if((dataValue > alert->max) || (dataValue < alert->min))
            {
                g_lcdSensorShow.lcdSensorDataStatus[id] = SENSOR_DATA_OVERFLOW;
            }
            APP_LOG_Debug("max:%d   min:%d\r\n", alert->max, alert->min);
#endif            
        }
        else
        {
            water_indicator->status = RS485_ERROR;
            mean_filter_out(&water_indicator->filter);
            water_indicator->errorCount++;
#ifdef SUPPORT_TFTLCD        
            g_lcdSensorShow.lcdSensorStatus[id] = SENSOR_NORMAL_STATE;
            g_lcdSensorShow.lcdSensorDataStatus[id] = SENSOR_DATA_NON;
#endif            
        }
        APP_LOG_Info("%s: status=%d value=%.2f\r\n", water_indicator_name_by_id(id),
                     indicator->status, value);
    }
    if (water_indicator->errorCount > 65530)
    {
        water_indicator->errorCount = 65530;
    }
    return APP_OK;
}

// 校验水质传感器ID值 
#define _CHECK_WATER_INDICATOR_ID(id)         \
    if (id >= APP_SENSORS_MAX_INDICATOR_SIZE) \
    {                                         \
        return APP_ERROR;                     \
    }

int APP_SENSORS_WaterIndicatorGet(uint8_t id, APP_WATER_IndicatorValue_t **vp)
{
    _CHECK_WATER_INDICATOR_ID(id)
    *vp = s_indicatorsValues + id;
    return APP_OK;
}



// 根据 id、manufacturer 和 theory 获取 model_id
uint8_t GetModelIdByManufacturerAndTheory(uint8_t id, uint8_t manufacturer, uint8_t theory)
{
    int i = 0;
    for (i = 0; i < ARRAY_LEN(c_modelIdMappings); i++)
    {
        if (id == c_modelIdMappings[i][0] && manufacturer == c_modelIdMappings[i][1] && theory == c_modelIdMappings[i][2])
        {
            return c_modelIdMappings[i][3];
        }
    }
    return 0;
}

// 根据 model_id 获取默认的 theory
static uint8_t GetTheoryByModelId(uint8_t model_id)
{
    int i = 0;
    if(RS485_SENSOR_MODEL_ID_DOY100 == model_id)    // DOY100 默认为探头类型为荧光法
    {
        return SENSOR_THEORY_OPTICS;
    }
    for (i = 1; i < ARRAY_LEN(c_modelIdMappings); i++)
    {
        if (model_id == c_modelIdMappings[i][3])
        {
            return c_modelIdMappings[i][2];
        }
    }
    return SENSOR_THEORY_OTHER;
}

static void SensorIndicatorInit(rs485_sensor_indicator_pt indicator)
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

// 打印 s_indicatorsValues[].sensor 的地址 
static void ShowWaterIndicatorSensorValue(void)
{
    APP_WATER_IndicatorValue_pt water_indicator;
    for (uint8_t id = 1; id < PROBE_SENSOR_SIZE; id++)
    {
        APP_SENSORS_WaterIndicatorGet(id, &water_indicator);
        if(water_indicator->sensor != NULL)
        {
            APP_LOG_Trace("s_indicatorsValues[%d].sensor = %p\r\n", id, &water_indicator->sensor);
        }
        else
        {
            APP_LOG_Trace("s_indicatorsValues[%d].sensor = NULL\r\n", id);
        }
    }
}

#define REFRESH_INIT    (-1)    // 初始化状态 
#define REFRESH_HOLD    (0)     // 未更新状态 
#define REFRESH_NEW     (1)     // 已更新状态 
static int8_t s_isRefresh = REFRESH_INIT;
static int64_t s_autoScanTick = REFRESH_HOLD;

#define SET_APP_CONFIG_REFRESH(status)  (s_isRefresh = status)
#define GET_APP_CONFIG_REFRESH()        (s_isRefresh)
#define SET_AUTO_SCAN_TICK(status)      (s_autoScanTick = status)
#define GET_AUTO_SCAN_TICK()            (s_autoScanTick)
#define SET_UPDATE_MESSAGE_TICK(status) (update_message_tick = status)
#define GET_UPDATE_MESSAGE_TICK()       (update_message_tick)

// 设置传感器配置更新的标志
void APP_SENSORS_RefreshNew(void)
{
    SET_APP_CONFIG_REFRESH(REFRESH_NEW);
}

#ifdef USE_NEW_SENSOR_SCAN_DRIVER    
static int DeinitSensorIndicator(uint8_t index)
{
    if(index < 1)
    {
        return APP_ERROR;
    }
    rs485_sensor_pt pSensor = &s_sensors[index - 1];
    SensorsInit(index-1);
    if (RS485_DRIVER_SENSOR_ID_DO == pSensor->id)
    {
        indicatorsValueInit(WATER_INDICATOR_INDEX_DO);
        indicatorsValueInit(WATER_INDICATOR_INDEX_TEMPERATURE);
    }
    else if (RS485_DRIVER_SENSOR_ID_PH == pSensor->id)
    {
        indicatorsValueInit(WATER_INDICATOR_INDEX_PH);
    }
    else if(RS485_DRIVER_SENSOR_ID_LEVEL == pSensor->id)
    {
        indicatorsValueInit(WATER_INDICATOR_INDEX_LEVEL);
    }
    // else if(RS485_DRIVER_SENSOR_ID_NH3 == pSensor->id)
    // {
    //     indicatorsValueInit(WATER_INDICATOR_INDEX_NH3);
    // }
    // else if(RS485_DRIVER_SENSOR_ID_COD == pSensor->id)
    // {
    //     indicatorsValueInit(WATER_INDICATOR_INDEX_COD);
    // }
    return APP_OK;
}
#endif

// 根据传感器配置情况，更新当前传感器匹配信息
void APP_SENSORS_RefreshStatus(void)
{
    APP_CONFIG_Sensor_pt config_sensor = NULL;
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
        APP_LOG_Debug("refresh status start\r\n");
#ifdef USE_NEW_SENSOR_SCAN_DRIVER        
        APP_CONFIG_SensorLoad();
#endif
        for (i = 1; i < PROBE_SENSOR_SIZE; i++)
        {
            config_sensor = &g_appConfigSensor[0][i];
            s_sensors[i - 1].indicator = &s_sensorIndicators[i - 1];   // 绑定实体数据
            if (HAS_CONFIG_IS_VALID(config_sensor))
            {
                s_sensors[i - 1].enable = RS485_ENABLE;
                s_sensors[i - 1].port = &rs485_port;
                s_sensors[i - 1].model = GetModelIdByManufacturerAndTheory(s_sensors[i - 1].id, config_sensor->manufacturer, config_sensor->model);
                APP_LOG_Debug("sensor config model_id = %d\r\n", s_sensors[i - 1].model);
                // if (0 == s_sensors[i - 1].model)
                // {
                //     // 设置传感器同类型的默认驱动 
                //     switch (s_sensors[i - 1].id)
                //     {
                //     case RS485_DRIVER_SENSOR_ID_NH3:
                //         s_sensors[i - 1].model = RS485_SENSOR_MODEL_ID_NHN760;
                //         break;
                //     case RS485_DRIVER_SENSOR_ID_COD:
                //         s_sensors[i - 1].model = RS485_SENSOR_MODEL_ID_COD861;
                //         break;
                //     default:
                //         break;
                //     }
                // }
            }
            else
            {
#ifdef USE_NEW_SENSOR_SCAN_DRIVER                
                DeinitSensorIndicator(i);
#else
                SensorsInit(i-1);
                if (s_sensors[i - 1].id == RS485_DRIVER_SENSOR_ID_DO)
                {
#ifndef ENABLE_RS485_DRIVER_SENSOR_XT_DO                    
                    indicatorsValueInit(WATER_INDICATOR_INDEX_DO);
                    indicatorsValueInit(WATER_INDICATOR_INDEX_TEMPERATURE);
#ifdef SUPPORT_TFTLCD        
                    g_lcdSensorShow.lcdSensorStatus[WATER_INDICATOR_INDEX_TEMPERATURE] = SENSOR_UNKNOW_STATE;
                    g_lcdSensorShow.lcdSensorDataStatus[WATER_INDICATOR_INDEX_TEMPERATURE] = SENSOR_DATA_UNCHANGE;
                    g_lcdSensorShow.lcdSensorStatus[WATER_INDICATOR_INDEX_DO] = SENSOR_UNKNOW_STATE;
                    g_lcdSensorShow.lcdSensorDataStatus[WATER_INDICATOR_INDEX_DO] = SENSOR_DATA_UNCHANGE;
#endif                  
#else
                    indicatorsValueInit(WATER_INDICATOR_INDEX_DO);
#endif                      
                }
                else if (s_sensors[i - 1].id == RS485_DRIVER_SENSOR_ID_PH)
                {
#ifndef ENABLE_RS485_DRIVER_SENSOR_XT_TMP_PH_NH3                    
                    indicatorsValueInit(WATER_INDICATOR_INDEX_PH);
#ifdef SUPPORT_TFTLCD        
                    g_lcdSensorShow.lcdSensorStatus[WATER_INDICATOR_INDEX_PH] = SENSOR_UNKNOW_STATE;
                    g_lcdSensorShow.lcdSensorDataStatus[WATER_INDICATOR_INDEX_PH] = SENSOR_DATA_UNCHANGE;
#endif                    
#else
                    indicatorsValueInit(WATER_INDICATOR_INDEX_TEMPERATURE);
                    indicatorsValueInit(WATER_INDICATOR_INDEX_PH);
                    indicatorsValueInit(WATER_INDICATOR_INDEX_NH3);
#endif
                }
                else if(s_sensors[i - 1].id == RS485_DRIVER_SENSOR_ID_LEVEL)
                {
                    indicatorsValueInit(WATER_INDICATOR_INDEX_LEVEL);
                }
                // else if(RS485_DRIVER_SENSOR_ID_NH3 == s_sensors[i - 1].id)
                // {
                //     indicatorsValueInit(WATER_INDICATOR_INDEX_NH3);
                // }
                // else if(RS485_DRIVER_SENSOR_ID_COD == s_sensors[i - 1].id)
                // {
                //     indicatorsValueInit(WATER_INDICATOR_INDEX_COD);
                // }
#endif
            }
        } 
        APP_LOG_Debug("refresh status end\r\n");
    }
}

int APP_SENSORS_Init(void)
{
    BSP_RS485_init();
    int i = 0;

#ifdef SUPPORT_L6
    APP_L6_Init();
    rs485_l6_status.reset_count = 0;
#endif
    for (i = 0; i < APP_SENSORS_MAX_INDICATOR_SIZE; i++)
    {
        indicatorsValueInit(i);
    }
    for (i = 0; i < ARRAY_LEN(s_sensorIndicators); i++)
    {
        SensorIndicatorInit(&s_sensorIndicators[i]);
    }
    
#ifdef SUPPORT_TFTLCD   
    APP_CONFIG_Sensor_pt config_sensor = NULL;
    for (i = 1; i < PROBE_SENSOR_SIZE; i++)
    {
        config_sensor = &g_appConfigSensor[0][i];
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
            APP_LOG_Trace("i:%d, init SENSOR_INITING_STATE\r\n", i);
        }
    }
#endif                
    return APP_OK;
}

#ifdef PH202_ADC_DEBUG   
int16_t *do_vm = NULL;
static int PhAdcUploadDebug(uint8_t id, rs485_sensor_indicator_pt indicator)
{
    APP_WATER_IndicatorValue_pt water_indicator;
    int rc = APP_SENSORS_WaterIndicatorGet(id, &water_indicator);
    APP_CHECK_RC(rc)
    water_indicator->sensor = indicator->sensor;
    if(1 == id)
    {
        do_vm = &(water_indicator->vm);
        APP_LOG_Trace("do_vm = 0x%08x\r\n", do_vm);
    }
    else if(2 == id)
    {
        APP_LOG_Trace("get do_vm = 0x%08x\r\n", do_vm);
        if(NULL != do_vm)
        {
            *do_vm = indicator->vm2;
            APP_LOG_Trace("set *do_vm = %d\r\n", *do_vm);
        }
        else
        {
            return APP_ERROR;
        }
    }
    return APP_OK;
}
#endif

static void ShowSensorIndicator(rs485_sensor_indicator_pt indicator)
{
    if (indicator->id == RS485_DRIVER_SENSOR_ID_DO)
    {
#ifndef ENABLE_RS485_DRIVER_SENSOR_XT_DO        
        RECORD_INDICATOR(WATER_INDICATOR_INDEX_DO, value2, vm2);
        RECORD_INDICATOR(WATER_INDICATOR_INDEX_TEMPERATURE, value1, vm1);
#else
        RECORD_INDICATOR(WATER_INDICATOR_INDEX_DO, value2, vm2);
#endif
    }
    else if (indicator->id == RS485_DRIVER_SENSOR_ID_PH)
    {
#ifndef ENABLE_RS485_DRIVER_SENSOR_XT_TMP_PH_NH3        
        RECORD_INDICATOR(WATER_INDICATOR_INDEX_PH, value1, vm1);
#else
        RECORD_INDICATOR(WATER_INDICATOR_INDEX_TEMPERATURE, value1, vm1);
        RECORD_INDICATOR(WATER_INDICATOR_INDEX_PH, value2, vm2);
        RECORD_INDICATOR(WATER_INDICATOR_INDEX_NH3, value3, vm3);
#endif
    }
    else if(indicator->id == RS485_DRIVER_SENSOR_ID_LEVEL)
    {
        RECORD_INDICATOR(WATER_INDICATOR_INDEX_LEVEL, value1, vm1);
    }
    // else if(indicator->id == RS485_DRIVER_SENSOR_ID_NH3)
    // {
    //     RECORD_INDICATOR(WATER_INDICATOR_INDEX_NH3, value1, vm1);
    // }
    // else if(indicator->id == RS485_DRIVER_SENSOR_ID_COD)
    // {
    //     RECORD_INDICATOR(WATER_INDICATOR_INDEX_COD, value1, vm1);
    // }
    
#ifdef PH202_ADC_DEBUG  
    if(indicator->id == RS485_DRIVER_SENSOR_ID_DO)
    {
        PhAdcUploadDebug(WATER_INDICATOR_INDEX_DO, indicator);
    } 
    else if(indicator->id == RS485_DRIVER_SENSOR_ID_PH)
    {
        PhAdcUploadDebug(WATER_INDICATOR_INDEX_PH, indicator);
    }
#endif
}

static void SetSenosrDefaultMessage(rs485_sensor_pt sensor, uint8_t theory)
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
                    // g_appConfigSensor[0][0].model = SENSOR_THEORY_OPTICS;
                    // g_appConfigSensor[0][sensor->id].model = SENSOR_THEORY_OPTICS;
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
            // case RS485_SENSOR_MODEL_ID_NHN760:
            //     sn_code = SENSOR_SN_CODE_NHN760;
            //     break;
            case RS485_SENSOR_MODEL_ID_COD861:
                sn_code = SENSOR_SN_CODE_COD861;
                break;
            case RS485_SENSOR_MODEL_ID_DOY181:
                sn_code = SENSOR_SN_CODE_DOY181;
                break;
            case RS485_SENSOR_MODEL_ID_PH291:
                sn_code = SENSOR_SN_CODE_PH291;
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
            // case RS485_DRIVER_SENSOR_ID_NH3:
            //     sn_id = SENSOR_SN_ID_NH3;
            //     break;
            // case RS485_DRIVER_SENSOR_ID_COD:
            //     sn_id = SENSOR_SN_ID_COD;
            //     break;
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
static int SensorSetDriverWithoutConnect(rs485_sensor_pt sensor, uint8_t sensorID)
{
    APP_CONFIG_Sensor_pt config = &g_appConfigSensor[0][sensorID];
    APP_LOG_Debug("[%s], hasConfig = %d\r\n", __func__, config->hasConfig);
    if(HAS_CONFIG_IS_VALID(config))
    {
        uint8_t theory = g_appConfigSensor[0][sensorID].model;
        sensor->enable = RS485_ENABLE;
        RS485_DRIVER_match_by_model(sensor, theory);
        // 设置默认的SN、软硬件版本号 
        SetSenosrDefaultMessage(sensor, theory);
        return APP_OK;
    }
    else if(GET_AUTO_SCAN_TICK() > REFRESH_NEW) // 仅当周期扫描且没有配置该传感器时，重新反初始化传感器 
    {
        APP_LOG_Debug("[DeinitSensorIndicator], sensorID = %d\r\n", sensorID);
        DeinitSensorIndicator(sensorID);
        return APP_ERROR;
    }
}

// 当接入了传感器但不能识别时，调用该函数设置默认驱动 
static int SensorSetDriverDefault(rs485_sensor_pt sensor)
{
    APP_LOG_Debug("[%s]\r\n", __func__);
    int rc = RS485_DRIVER_match_default(sensor);
    APP_CHECK_RC(rc)
    uint8_t theory = sensor->model;
    sensor->enable = RS485_ENABLE;
    SetSenosrDefaultMessage(sensor, theory);
    APP_LOG_Debug("Run setDefaultDriver, id = %d\r\n", sensor->id);
    return APP_OK;
}

// 扫描并匹配传感器，获取传感器信息(版本号/PN/SN) 
static int Rs485AutoScanSensor(uint8_t sensorID)
{
    // sensor.id = sensorID;
    int rc = RS485_DRIVER_match(s_pSensor);    // 先用中易的协议试看是否能匹配 
    // 如果是读超时(没有接传感器)，不要用单个的协议扫描  
    if ((RS485_READ_ERROR == rc) && (REFRESH_HOLD != GET_AUTO_SCAN_TICK()))  
    {
        SensorSetDriverWithoutConnect(s_pSensor, sensorID);
        return RS485_READ_ERROR;
    }
    if (RS485_ERROR == rc)  // 能读到数据，但不符合要求 
    {
        rc = RS485_DRIVER_scan(s_pSensor);
        if ((RS485_READ_ERROR == rc) && (REFRESH_HOLD != GET_AUTO_SCAN_TICK()))  
        {
            SensorSetDriverWithoutConnect(s_pSensor, sensorID);
            return RS485_READ_ERROR;
        }
        else if(RS485_ERROR == rc)
        {
            SensorSetDriverDefault(s_pSensor);
            return RS485_ERROR;
        }
    }
    rs485_sensor_action_pt driver_action = s_pSensor->action;
    if (NULL != driver_action->read_info)
    {
        driver_action->read_info(s_pSensor);
        // HARDWARE_OS_DELAY_MS(200);
    }
    s_pSensor->enable = RS485_ENABLE;
    return RS485_OK;
}
#else

// 扫描并匹配传感器，获取传感器信息(版本号/PN/SN) 
static int Rs485AutoScanSensor(uint8_t sensorID)
{
    // sensor.id = sensorID;
    int rc = RS485_DRIVER_match(s_pSensor);    // 先用中易的协议试看是否能匹配 
    if (rc == RS485_READ_ERROR)             // 如果是读超时(没有接传感器)，不要用单个的协议扫描 
    {
        if(REFRESH_HOLD != GET_AUTO_SCAN_TICK())    // 当使用EEPROM中的传感器配置且未接入传感器时，才调用该函数匹配驱动并设置默认值 
        {
            uint8_t theory = g_appConfigSensor[0][sensorID].model;
            rc = RS485_DRIVER_match_by_model(s_pSensor, theory);
#ifdef SUPPORT_TFTLCD
            if(RS485_OK == rc)
            {
                s_findSensor |= 0x1;
                if(RS485_DRIVER_SENSOR_ID_DO == sensorID)
                {
                    g_lcdSensorShow.lcdSensorStatus[WATER_INDICATOR_INDEX_TEMPERATURE] = SENSOR_INITING_STATE;
                    g_lcdSensorShow.lcdSensorDataStatus[WATER_INDICATOR_INDEX_TEMPERATURE] = SENSOR_DATA_UNCHANGE;
                    g_lcdSensorShow.lcdSensorStatus[WATER_INDICATOR_INDEX_DO] = SENSOR_INITING_STATE;
                    g_lcdSensorShow.lcdSensorDataStatus[WATER_INDICATOR_INDEX_DO] = SENSOR_DATA_UNCHANGE;
                }
                else if(RS485_DRIVER_SENSOR_ID_PH == sensorID)
                {
                    g_lcdSensorShow.lcdSensorStatus[WATER_INDICATOR_INDEX_PH] = SENSOR_INITING_STATE;
                    g_lcdSensorShow.lcdSensorDataStatus[WATER_INDICATOR_INDEX_PH] = SENSOR_DATA_UNCHANGE;
                }
                APP_LOG_Trace("sensorID:%d, match_by_model SENSOR_INITING_STATE\r\n", sensorID);
            }
#endif 
            // 设置默认的SN、软硬件版本号 
            SetSenosrDefaultMessage(s_pSensor, theory);
        }
        return RS485_READ_ERROR;
    }
    if (rc != RS485_OK)
    {
        rc = RS485_DRIVER_scan(s_pSensor);
    }
    RS485_CHECK_RC(rc)
    rs485_sensor_action_pt driver_action = s_pSensor->action;
    if (NULL != driver_action->read_info)
    {
        driver_action->read_info(s_pSensor);
        // HARDWARE_OS_DELAY_MS(200);
    }
    s_pSensor->enable = RS485_ENABLE;
#ifdef SUPPORT_TFTLCD
    if(RS485_DRIVER_SENSOR_ID_DO == sensorID)
    {
        g_lcdSensorShow.lcdSensorStatus[WATER_INDICATOR_INDEX_TEMPERATURE] = SENSOR_INITING_STATE;
        g_lcdSensorShow.lcdSensorDataStatus[WATER_INDICATOR_INDEX_TEMPERATURE] = SENSOR_DATA_UNCHANGE;
        g_lcdSensorShow.lcdSensorStatus[WATER_INDICATOR_INDEX_DO] = SENSOR_INITING_STATE;
        g_lcdSensorShow.lcdSensorDataStatus[WATER_INDICATOR_INDEX_DO] = SENSOR_DATA_UNCHANGE;
    }
    else if(RS485_DRIVER_SENSOR_ID_PH == sensorID)
    {
        g_lcdSensorShow.lcdSensorStatus[WATER_INDICATOR_INDEX_PH] = SENSOR_INITING_STATE;
        g_lcdSensorShow.lcdSensorDataStatus[WATER_INDICATOR_INDEX_PH] = SENSOR_DATA_UNCHANGE;
    }
    APP_LOG_Trace("sensorID:%d, match/scan SENSOR_INITING_STATE\r\n", sensorID);
#endif    
    return RS485_OK;
}
#endif

// 更新传感器配置数据，并与EEPROM中同步
static int UpdateSensorConfig(uint8_t id, uint8_t enable, uint8_t manufacturer, uint8_t type)
{
    APP_CONFIG_Sensor_t sensorConfig;
    int rc = 0;
    APP_CONFIG_SensorInit(&sensorConfig);
    if(1 == enable)
    {
        sensorConfig.hasConfig = APP_CONFIG_HAS_CONFIG;
        sensorConfig.enable = APP_CONFIG_ENABLED;
        sensorConfig.manufacturer = manufacturer;
        sensorConfig.model = type;
    }
    if(RS485_DRIVER_SENSOR_ID_DO == id)
    {
        rc = APP_CONFIG_SensorWrite(PROBE_SIZE - 1, 0, &sensorConfig);
    }
    rc = APP_CONFIG_SensorWrite(PROBE_SIZE - 1, id, &sensorConfig);
    return rc;
}

static int UpdateSensorDataConfig(uint8_t id, uint8_t enable, uint8_t manufacturer, uint8_t type)
{
    int rc = 0;
    APP_CONFIG_Sensor_pt p_app_config_sensor;
    p_app_config_sensor = &g_appConfigSensor[0][id];
    if(APP_CONFIG_ENABLED == enable)
    {
        p_app_config_sensor->hasConfig = APP_CONFIG_HAS_CONFIG;
        p_app_config_sensor->enable = APP_CONFIG_ENABLED;
        p_app_config_sensor->manufacturer = manufacturer;
        p_app_config_sensor->model = type;
        if(RS485_DRIVER_SENSOR_ID_DO == id)
        {
            p_app_config_sensor = &g_appConfigSensor[0][0];
            p_app_config_sensor->hasConfig = APP_CONFIG_HAS_CONFIG;
            p_app_config_sensor->enable = APP_CONFIG_ENABLED;
            p_app_config_sensor->manufacturer = manufacturer;
            p_app_config_sensor->model = type;
        }
    }
    return rc;
}

// 上报传感器(配置)PN值
static int UploadSensorConfig(void)
{
    // return G2_SERVER_upload_sensor_config_message_process(PROBE_SIZE);
    return G2_SERVER_upload_device_sensors_message_process();
}

// 设置要扫描传感器的地址
static uint8_t s_sensorIDs[] = 
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
static rs485_sensor_pt SelectSensorByID(uint8_t id)
{
    uint8_t i = 0;
    for(i=0; i<ARRAY_LEN(s_sensors); i++)
    {
        if(id == s_sensors[i].id)
        {
            return (&s_sensors[i]);
        }
    }
    return NULL;
}

// 扫描并识别外部接入的传感器,上报传感器信息给服务器 
static int Rs485AutoScanSensors(void)
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
        APP_LOG_Debug("scan sensors driver\r\n");
        // if(REFRESH_HOLD != GET_AUTO_SCAN_TICK())
        // {
        //     HARDWARE_OS_DELAY_MS(1000);
        // }
#ifdef SUPPORT_TFTLCD
        SET_PAUSE_CONFIG();
#endif    
        for (int i = 0; i < ARRAY_LEN(s_sensorIDs); i++)
        {
            s_pSensor = SelectSensorByID(s_sensorIDs[i]);
            if(NULL != s_pSensor)
            {
                s_pSensor->port = &rs485_port;
                rc = Rs485AutoScanSensor(s_pSensor->id);
                if(REFRESH_HOLD != GET_AUTO_SCAN_TICK())
                {
                    APP_SENSORS_SetCompensationsBySensor(s_pSensor);
                }
                if(RS485_OK == rc) 
                {
#ifdef SUPPORT_TFTLCD        
                    s_findSensor |= 0x2;
#endif 
                    if(0 != s_pSensor->sensor_change)
                    {
                        sensor_change++;
                        s_pSensor->sensor_change = 0;
                    }
                    UpdateSensorDataConfig(s_pSensor->id, APP_CONFIG_ENABLED, s_pSensor->manufacturer, GetTheoryByModelId(s_pSensor->model));      
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
        // ShowWaterIndicatorSensorValue();
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
        if(APP_OK == CheckTimeRange())
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
            UploadSensorConfig();
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
    APP_SENSORS_Init();
//#ifdef SUPPORT_L6
#if (defined(SUPPORT_L6) && (!defined(SUPPORT_L6_ENABLE)))
    APP_L6_Start();
    HARDWARE_OS_DELAY_MS(1000);
#endif
    for (;;)
    {
        APP_SENSORS_RefreshStatus();
        Rs485AutoScanSensors();
#ifdef SUPPORT_L6
/*
气吹装置故障处理逻辑
1. 如果通信未返回或返回设备状态为故障连续超过10次，则重启L6
2. 如果重启超过3次设备任然没有好则强制读取传感器数据，不处理L6的逻辑
3. 强制读取的数据当成正常的数据
*/
#ifndef SUPPORT_L6_ENABLE
        APP_LOG_Debug("air blow: err: %d, rst: %d, status: %d\r\n", rs485_l6_status.errorCount, rs485_l6_status.reset_count, rs485_l6_status.status);
        APP_L6_Read();
#endif
        HARDWARE_OS_DELAY_MS(1000);  
        if (!APP_L6_ForceRead())
        {
#if (defined(SUPPORT_POWER_OFF_SENSOR) && (!defined(SUPPORT_L6_ENABLE)))
            if (APP_L6_CanReset()) // L6是否可以复位 
            {
                POWER_SENSOR_OFF();
                HARDWARE_OS_DELAY_MS(5000);
                POWER_SENSOR_ON();
                APP_L6_Init();
                rs485_l6_status.reset_count += 1;
            }
            if (!APP_L6_Initialized())
            {
                continue;
            }
#endif						
            if(APP_L6_SendDataReady()) // L6 有数据可以发送 
            {
                APP_SENSORS_SendDataToServer();  // 先在数据读取完成后主动上报传感器数据 
                saturability_data_tick= HARDWARE_GET_TICK();
                rs485_l6_status.saturability_status = 1;
            }
            if(APP_L6_ForceSaturabilityReady())   // 上报过传感器数据后，(5min后)检查 DO 传感器在空气中的饱和度值情况 
            {
                if (HARDWARE_GET_TICK() - saturability_data_tick > 5 * 60 * 1000) 
                { 
                    APP_SENSORS_CheckIndicatorSaturabilityAlert();
                } 
            }
            if ((!APP_L6_DataReady())) // L6 流程中传感器数据不可获取阶段 
            {
                continue;
            }
        }
		else // L6 非正常运行 or 传感器探头校准 
        {
            APP_SENSORS_SendDataToServer();
        }
#endif
        APP_SENSORS_SetCompensationsBySensor(&s_sensors[0]);
        APP_SENSORS_ReadEach(ShowSensorIndicator, 1000);
        HARDWARE_OS_DELAY_MS(1000);
#ifndef SUPPORT_L6
        APP_SENSORS_SendDataToServer();
#endif
    }
}

int APP_SENSORS_ReadEach(rs485_sensor_indicator_fun callback, uint32_t interval_delay_ms)
{
    int rc = 0;
    uint8_t index = 0;
    rs485_sensor_pt sensor = &s_sensors[0];
    rs485_sensor_action_pt action = NULL;
    APP_WATER_IndicatorValue_pt water_indicator;
    uint8_t sensor_change = 0;

#ifdef TEST_SENSOR_DATA
    static int8_t tmp_dir = 0;
    static int8_t do_dir = 0;
    static int8_t ph_dir = 0;
#endif
    APP_LOG_Trace("read each start\r\n");
    for (int i = 0; i < ARRAY_LEN(s_sensors); i++)
    {
        sensor = &s_sensors[i];
        action = sensor->action;
        if (APP_SENSOR_IS_ENABLE(sensor) && action != NULL && action->read_value1 != NULL)
        {
            sensor->indicator->id = sensor->id;
            sensor->indicator->sensor = sensor;
#ifndef TEST_SENSOR_DATA            
            rc = action->read_value1(sensor, sensor->indicator);
            APP_LOG_Debug("\r\nSensors read rc %d s %d\r\n", rc, sensor->indicator->status);
            if(RS485_OK == sensor->indicator->status)          // RS485_OK
            {
                sensor->timeout_error_count = 0;
            }
            else
            {
                uint8_t compensation_id = 0;
                if(APP_OK == GetCompensationIndex(sensor->id, &compensation_id))
                {
                    s_sensorCompensationTicks[compensation_id] = 0;
                }
                sensor->timeout_error_count++;
                if(sensor->timeout_error_count >= 10)
                {
                    sensor->timeout_error_count = 0;
                    sensor_change++;
                    // UpdateSensorConfig(sensor->id, 0, sensor->manufacturer, sensor->model);
                }
            }
#else   
            if(RS485_DRIVER_SENSOR_ID_DO == sensor->id)
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
            else if(RS485_DRIVER_SENSOR_ID_PH == sensor->id)
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
                index = GetIndicatorIndex(sensor->id);
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
    APP_LOG_Trace("read each end\r\n");
	// ShowWaterIndicatorSensorValue();
    return APP_OK;
}

// 根据传感器ID查找可用的传感器dev实体 
static int FindSensorBySensorID(uint8_t sensorID, rs485_sensor_pt *vp)
{
    if (sensorID != RS485_DRIVER_SENSOR_ID_DO && sensorID != RS485_DRIVER_SENSOR_ID_PH)
        return APP_ERROR;
    rs485_sensor_pt s_pSensor = &s_sensors[0];
    for (int i = 0; i < ARRAY_LEN(s_sensors); i++)
    {
        s_pSensor = &s_sensors[i];
        if (s_pSensor->id == sensorID)
        {
            *vp = s_pSensor;
            return APP_OK;
        }
    }
    return APP_ERROR;
}

// 校准传感器 
int APP_SENSOR_Calibrate(uint8_t probeID, uint8_t sensorID, uint8_t type, uint8_t *params)
{
    rs485_sensor_pt sensor = NULL;
    int rc = FindSensorBySensorID(sensorID, &sensor);
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
int APP_SENSOR_WriteCompensation(uint8_t sensorID, uint8_t indicatorID, float value, int force)
{
    if (s_sensorCompensationTicks[indicatorID] != 0 && force == 0)
    {
        return APP_ERROR;
    }
    rs485_sensor_pt sensor = NULL;
    int rc = FindSensorBySensorID(sensorID, &sensor);
    APP_CHECK_RC(rc)
    rs485_sensor_action_pt action = sensor->action;
    if (sensor->port != NULL &&
        action != NULL &&
        APP_SENSOR_IS_ENABLE(sensor))
    {
        if (action->write_compensation != NULL)
        {
            rc = action->write_compensation(sensor, indicatorID, value) == RS485_OK ? APP_OK : APP_ERROR;
            if (rc == APP_OK)
            {
                s_sensorCompensationTicks[indicatorID] = HARDWARE_GET_TICK();
            }
            return rc;
        }
    }
    return APP_ERROR;
}

// 对特定传感器执行写补偿操作 
int APP_SENSORS_SetCompensationsBySensor(rs485_sensor_pt sensor)
{
    float value = 0;
    if (sensor->id == RS485_DRIVER_SENSOR_ID_DO)
    {
        APP_CONFIG_Compensation_pt compensation = &g_appConfigCompensation[0][WATER_INDICATOR_INDEX_SALINITY];
        if (HAS_CONFIG_IS_VALID(compensation))
            water_indicator_from_uint16_t(WATER_INDICATOR_INDEX_SALINITY, compensation->value, &value);
        return APP_SENSOR_WriteCompensation(sensor->id, WATER_INDICATOR_INDEX_SALINITY, value, 0);
    }
    return APP_OK;
}

// 设置传感器探头的告警类型
void APP_SENSORS_CheckIndicatorAlert(int probeID, int indicatorID, g2_server_sensor_data_indicator_message_pt indicator)
{
    APP_CONFIG_IndicatorAlert_pt alert = &g_appConfigIndicatorAlert[probeID][indicatorID];
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
static int SensorTimeCompare(APP_CONFIG_Time_pt now, uint8_t hour, uint8_t minute)
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
void APP_SENSORS_CheckIndicatorSaturabilityAlert(void)
{
    if (APP_OK == CheckTimeRange())
    {
        APP_SENSORS_ReadEach(ShowSensorIndicator, 1000);    // 再次遍历传感器读取数据 
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
int APP_SENSOR_WriteRef(uint8_t sensorID, uint8_t indicatorID, uint16_t value)
{
    rs485_sensor_pt sensor = NULL;
    int rc = FindSensorBySensorID(sensorID, &sensor);
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


int APP_SENSORS_ReadStatusProtocol(uint8_t probeID, g2_server_sensor_data_message_pt message, int *changed)
{
    if (probeID > PROBE_SIZE || probeID == 0)
    {
        return APP_ERROR;
    }
    *changed = 0;
    uint8_t week;
    APP_WATER_IndicatorValue_pt water_indicator;
    int rc = 0;
    uint16_t u_value = 0;
    G2_SERVER_sensor_data_message_init(message);
    message->probeID = probeID;
    message->config = 0;
    BSP_RTC_get(&message->time.year, &message->time.month, &message->time.day, &message->time.hour, &message->time.minute, &message->time.second, &week);
#ifdef SUPPORT_PUMP
    APP_PUMP_Changed(changed);
    if (g_appPump.status == APP_PUMP_STATUS_ON)
    {
        message->pump_status = 0x6000;
        if (g_appPump.check != APP_PUMP_CHECK_NORMAL)
            message->pump_broken_status = 0x6000;
    }
#else
    message->pump_status = 0;
    message->pump_broken_status = 0;
#endif
    for (int indicatorID = 0; indicatorID < INDICATOR_SIZE; indicatorID++)
    {
        rc = APP_SENSORS_WaterIndicatorGet(indicatorID, &water_indicator);
        if (rc == APP_OK && water_indicator->sensor != NULL) //有传感器
        {
            message->config |= 0x8000 >> indicatorID;
            if(APP_OK == CheckSensorProbeStatus(indicatorID, water_indicator, &u_value))
            {
                if(RS485_DRIVER_SENSOR_ID_NH3 == indicatorID)
                {
                    uint16_t randMax = (g_appRandOpt.max /10);
                    uint16_t randMin = (g_appRandOpt.min /10);
                    float tempMax = (randMax + 0.9);
                    float tempMin = (randMin + 0);
                    float temp = ((float)rand() / RAND_MAX) * (tempMax - tempMin) + tempMin;  
                    u_value = (uint8_t)temp; 
                    APP_LOG_Debug("NHN rand:%d\r\n", u_value) ;
                }
                message->indicators[indicatorID].value = u_value;
                message->indicators[indicatorID].probe_status = 0;
                message->indicators[indicatorID].value_mv = water_indicator->vm;
                APP_SENSORS_CheckIndicatorAlert(probeID - 1, indicatorID, &message->indicators[indicatorID]);
            }
            else
            {
                message->indicators[indicatorID].probe_status = 0xe0;
#ifdef PH202_ADC_DEBUG
                if(1 == indicatorID)
                {
                    APP_LOG_Trace("[0x%08x]:vm = %d\r\n", (uint32_t)(&water_indicator->vm), water_indicator->vm);
                }
                message->indicators[indicatorID].value_mv = water_indicator->vm;
#endif
            }
        }
    }
    return APP_OK;
}

// 发送传感器数据到服务器 
int APP_SENSORS_SendDataToServer(void)
{
	double saturability_value = 0;
    int changed = 0;
    g2_server_packet_t packet;
    G2_SERVER_PACKET_init(&packet);
    g2_server_sensor_data_message_t message;
    int rc = APP_SENSORS_ReadStatusProtocol(1, &message, &changed);
#ifdef SUPPORT_L6 
	if (APP_SERVER_CONNECTED() && (APP_L6_SendDataReady() || changed || HARDWARE_GET_TICK() - s_sendDataTick > UPLOAD_SENSOR_DATA_CYCLE_TIME * 60 * 1000))
#else 
    if (APP_SERVER_CONNECTED() && (changed || HARDWARE_GET_TICK() - s_sendDataTick > UPLOAD_SENSOR_DATA_CYCLE_TIME * 60 * 1000))
#endif
    {
        s_sendDataTick = HARDWARE_GET_TICK();
#ifdef SUPPORT_L6 
        rs485_l6_status.send_status = 0;
        if(!APP_L6_IsBroken())
        {
            saturability_value = 468/(message.indicators[0].value/10+31.6);
            saturability_value = message.indicators[1].value/saturability_value;
            rs485_l6_status.saturability[s_countTick] = (uint16_t)saturability_value; 
            s_countTick++;
            if(s_countTick >= Saturability_max_capacity)
            {
                s_countTick = 0;	
                s_deltaSaturabilityData = max_min_get(rs485_l6_status.saturability, Saturability_max_capacity);
                if(s_deltaSaturabilityData < 10)    // 若 DO 传感器长时间处在空气中，则饱和度变化较小(<10) 
                {
                    // 判断 DO 传感器是否使能+探头是否正常
                    rs485_sensor_pt sensor = &s_sensors[0];
                    if(APP_SENSOR_IS_ENABLE(sensor) && (0 == message.indicators[1].probe_status))
                    {
                        rs485_l6_status.air_alet_status = 1;
                        APP_LOG_Info("air_alet_status=%d \r\n", rs485_l6_status.air_alet_status);
                        APP_SENSOR_SendSelfDiagnosisToServer();
                    }
                }
                memset(rs485_l6_status.saturability, 0, Saturability_max_capacity);	
            }
            APP_LOG_Info("saturability_value=%d \r\n", (uint16_t)saturability_value);
            APP_LOG_Info("DO probe_status=%02X\r\n", message.indicators[1].probe_status);
            
            if (APP_OK == CheckTimeRange() && 0 == message.indicators[1].probe_status)   // 时间符合并且 DO 传感器探头正常 
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

int APP_SENSORS_SensorGet(uint8_t addr, rs485_sensor_t **vp)
{
    rs485_sensor_pt sensor = &s_sensors[0];
    for (int i = 0; i < ARRAY_LEN(s_sensors); i++)
    {
        sensor = &s_sensors[i];
        if (sensor->id == addr && sensor->port != NULL && APP_SENSOR_IS_ENABLE(sensor))
        {
            *vp = sensor;
            return APP_OK;
        }
    }
    return APP_ERROR;
}

// 自诊断结果打包 
static int AppSelfDiagnosisPack(g2_server_self_diagnosis_message_pt message)
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
void APP_SENSOR_SendSelfDiagnosisToServer(void)
{
    g2_server_packet_t packet;
    G2_SERVER_PACKET_init(&packet);
    g2_server_self_diagnosis_message_t message;
    G2_SERVER_self_diagnosis_message_init(&message);
    AppSelfDiagnosisPack(&message);
    BSP_PROTOCOL_send_read_self_diagnosis_message(&packet, &message);
}

// 拷贝传感器 device 信息
static void CopySensorInfo(uint8_t *des, uint8_t *src, uint8_t len)
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
int APP_SENSORS_ReadDeviceSensors(g2_server_device_sensors_message_pt message)
{
    rs485_sensor_pt sensor = NULL;
    g2_server_sensor_info_message_pt sensor_info = NULL;
    for(uint8_t i=0; i < ARRAY_LEN(s_sensors); i++)
    {
        sensor = &s_sensors[i];
        sensor_info = &message->sensors_info[i+1];
        if(APP_SENSOR_IS_ENABLE(sensor) && sensor->PN)
        {
            message->config |= 0x8000 >> (sensor->id);
            CopySensorInfo(sensor_info->hardware, sensor->hardware, sizeof(sensor_info->hardware));
            CopySensorInfo(sensor_info->firmware, sensor->firmware, sizeof(sensor_info->firmware));
            CopySensorInfo(sensor_info->PN, sensor->PN, strlen(sensor->PN));
            CopySensorInfo(sensor_info->SN, sensor->SN, sizeof(sensor_info->SN));
            if(RS485_DRIVER_SENSOR_ID_DO == sensor->id)
            {
                sensor_info = &message->sensors_info[0];
                message->config |= 0x8000 >> 0;
                CopySensorInfo(sensor_info->hardware, sensor->hardware, sizeof(sensor_info->hardware));
                CopySensorInfo(sensor_info->firmware, sensor->firmware, sizeof(sensor_info->firmware));
                CopySensorInfo(sensor_info->PN, sensor->PN, strlen(sensor->PN));
                CopySensorInfo(sensor_info->SN, sensor->SN, sizeof(sensor_info->SN));
            }
        }
    }
    return APP_OK;
}

