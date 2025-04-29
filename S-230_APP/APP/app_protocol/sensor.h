#include "water_indicator.h"
#include "app_sensors.h"

#ifdef SUPPORT_PUMP
#include "app_pump.h"
#endif

int G2_SERVER_read_salinity_message_process(g2_server_packet_pt packet)
{
    CHECK_PROTOCOL_MESSAGE
    g2_server_probe_id_message_pt pmsg = (g2_server_probe_id_message_pt)packet->parsed;
    CHECK_PROTOCOL_PROBE_ID_MESSAGE(pmsg->probeID)
    APP_CONFIG_Compensation_pt cmsg = &(g_appConfigCompensation[pmsg->probeID - 1][WATER_INDICATOR_INDEX_SALINITY]);
    // CHECK_HAS_CONFIG_IS_VALID(cmsg)
    g2_server_salinity_message_t message = {0};
    G2_SERVER_salinity_message_init(&message);
    message.probeID = pmsg->probeID;
    if (HAS_CONFIG_IS_VALID(cmsg))
        message.salinity = cmsg->value;
    else
        message.salinity = 0;
    BSP_PROTOCOL_send_read_salinity_message(packet, &message);
    return PROTOCOL_OK;
}

int G2_SERVER_write_salinity_message_process(g2_server_packet_pt packet)
{
    float value = 0;
    CHECK_PROTOCOL_MESSAGE
    g2_server_salinity_message_pt pmsg = (g2_server_salinity_message_pt)packet->parsed;
    CHECK_PROTOCOL_PROBE_ID_MESSAGE(pmsg->probeID)
    APP_CONFIG_Compensation_pt cmsg = &(g_appConfigCompensation[pmsg->probeID - 1][WATER_INDICATOR_INDEX_SALINITY]);
    int rc = water_indicator_from_uint16_t(WATER_INDICATOR_INDEX_SALINITY, pmsg->salinity, &value);
    CHECK_CONFIG_MESSAGE_RC(rc)
    rc = APP_SENSOR_WriteCompensation(RS485_DRIVER_SENSOR_ID_DO, WATER_INDICATOR_INDEX_SALINITY, value, 1);
    if (rc != APP_OK)
    {
        BSP_PROTOCOL_send_response(packet, 0xe7);
        return PROTOCOL_OK;
    }
    rc = APP_CONFIG_CompensationWriteData(pmsg->probeID - 1, WATER_INDICATOR_INDEX_SALINITY, cmsg, APP_CONFIG_HAS_CONFIG, pmsg->salinity);
    CHECK_CONFIG_MESSAGE_RC(rc)
    BSP_PROTOCOL_send_ok_response(packet);
    return PROTOCOL_OK;
}

int G2_SERVER_read_sensor_config_message_process(g2_server_packet_pt packet)
{
    CHECK_PROTOCOL_MESSAGE
    g2_server_probe_id_message_pt pmsg = (g2_server_probe_id_message_pt)packet->parsed;
    CHECK_PROTOCOL_PROBE_ID_MESSAGE(pmsg->probeID)
    APP_CONFIG_Sensor_pt cmsg;
    g2_server_sensor_config_message_t message;
    G2_SERVER_sensor_config_message_init(&message);
    message.probeID = pmsg->probeID;
    message.config = 0;
    int i = 0;
    for (i = 0; i < PROBE_SENSOR_SIZE; i++)
    {
        cmsg = &g_appConfigSensor[pmsg->probeID - 1][i];
        if (ENABLE_IS_VALID(cmsg))
        {
            message.config |= 0x8000 >> i;
            message.data[i].manufacturer = cmsg->manufacturer;
            message.data[i].type = cmsg->model;
        }
    }
    BSP_PROTOCOL_send_read_sensor_config_message(packet, &message);
    return PROTOCOL_OK;
}


int G2_SERVER_upload_sensor_config_message_process(uint8_t probeID)
{
    g2_server_packet_t packet;
    G2_SERVER_PACKET_init(&packet);
    packet.buffer[0] = probeID;
    packet.parsed = packet.buffer;
    g2_server_probe_id_message_pt pmsg = (g2_server_probe_id_message_pt)packet.parsed;
    APP_CONFIG_Sensor_pt cmsg;
    g2_server_sensor_config_message_t message;
    G2_SERVER_sensor_config_message_init(&message);
    message.probeID = pmsg->probeID;
    message.config = 0;
    int i = 0;
    for (i = 0; i < PROBE_SENSOR_SIZE; i++)
    {
        cmsg = &g_appConfigSensor[pmsg->probeID - 1][i];
        if (ENABLE_IS_VALID(cmsg))
        {
            message.config |= 0x8000 >> i;
            message.data[i].manufacturer = cmsg->manufacturer;
            message.data[i].type = cmsg->model;
        }
    }
    BSP_PROTOCOL_send_read_sensor_config_message(&packet, &message);
    return PROTOCOL_OK;
}

static int save_protocol_to_sensor_config(g2_server_sensor_config_message_pt config)
{
    int i = 0;
    int rc = 0;
    uint16_t mask = 0;
    APP_CONFIG_Sensor_t g_appConfigSensor;

    for (i = 0; i < PROBE_SENSOR_SIZE; i++)
    {
        APP_CONFIG_SensorInit(&g_appConfigSensor);
        mask = 0x8000 >> i;
        if ((config->config & mask) == mask)
        {
            g_appConfigSensor.hasConfig = APP_CONFIG_HAS_CONFIG;
            g_appConfigSensor.enable = APP_CONFIG_ENABLED;
            g_appConfigSensor.manufacturer = config->data[i].manufacturer;
            g_appConfigSensor.model = config->data[i].type;
        }
        rc = APP_CONFIG_SensorWrite(config->probeID - 1, i, &g_appConfigSensor);
        APP_CHECK_RC(rc)
    }
    return rc;
}


int G2_SERVER_write_sensor_config_message_process(g2_server_packet_pt packet)
{
    CHECK_PROTOCOL_MESSAGE
    g2_server_write_sensor_config_message_pt pmsg = (g2_server_write_sensor_config_message_pt)packet->parsed;
    CHECK_PROTOCOL_CHECK_WRITE_MESSAGE(pmsg->oper)
    g2_server_sensor_config_message_pt config = &pmsg->sensor;
    CHECK_PROTOCOL_PROBE_ID_MESSAGE(config->probeID)

    int rc = save_protocol_to_sensor_config(config);
    CHECK_CONFIG_MESSAGE_RC(rc)
    rc = APP_CONFIG_SensorLoad();
    CHECK_CONFIG_MESSAGE_RC(rc)
    APP_SENSORS_RefreshNew();
    BSP_PROTOCOL_send_ok_response(packet);
    return PROTOCOL_OK;
}

int G2_SERVER_delete_sensor_config_message_process(g2_server_packet_pt packet)
{
    CHECK_PROTOCOL_MESSAGE
    g2_server_delete_sensor_config_message_pt pmsg = (g2_server_delete_sensor_config_message_pt)packet->parsed;
    CHECK_PROTOCOL_CHECK_DELETE_MESSAGE(pmsg->oper)
    CHECK_PROTOCOL_PROBE_ID_MESSAGE(pmsg->probeID)
    int i = 0;
    int rc = 0;
    APP_CONFIG_Sensor_t config_sensor;
    APP_CONFIG_SensorInit(&config_sensor);
    for (i = 0; i < PROBE_SENSOR_SIZE; i++)
    {
        rc = APP_CONFIG_SensorWrite(pmsg->probeID - 1, i, &config_sensor);
        CHECK_CONFIG_MESSAGE_RC(rc)
    }
    rc = APP_CONFIG_SensorLoad();
    CHECK_CONFIG_MESSAGE_RC(rc)
    APP_SENSORS_RefreshNew();
    BSP_PROTOCOL_send_ok_response(packet);
    return PROTOCOL_OK;
}

int G2_SERVER_read_sensor_alert_message_process(g2_server_packet_pt packet)
{
    CHECK_PROTOCOL_MESSAGE
    g2_server_probe_id_message_pt pmsg = (g2_server_probe_id_message_pt)packet->parsed;
    CHECK_PROTOCOL_PROBE_ID_MESSAGE(pmsg->probeID)
    g2_server_sensor_alert_message_t message;
    G2_SERVER_sensor_alert_message_init(&message);
    message.probeID = pmsg->probeID;
    message.config = 0;
    int i = 0;
    APP_CONFIG_IndicatorAlert_pt indicator_alert = NULL;
    for (i = 0; i < INDICATOR_SIZE; i++)
    {
        indicator_alert = &g_appConfigIndicatorAlert[pmsg->probeID - 1][i];
        if (HAS_CONFIG_IS_VALID(indicator_alert))
        {
            message.config |= 0x8000 >> i;
            message.data[i].max = indicator_alert->max;
            message.data[i].min = indicator_alert->min;
            message.data[i].threshold = indicator_alert->threshold;
        }
    }
    BSP_PROTOCOL_send_read_sensor_alert_message(packet, &message);
    return PROTOCOL_OK;
}

static int save_protocol_to_sensor_alert(g2_server_sensor_alert_message_pt config)
{
    int i = 0;
    int rc = 0;
    uint16_t mask = 0;
    APP_CONFIG_IndicatorAlert_t indicator_alert;
    for (i = 0; i < INDICATOR_SIZE; i++)
    {
        APP_CONFIG_IndicatorAlertInit(&indicator_alert);
        mask = 0x8000 >> i;
        if ((config->config & mask) == mask)
        {
            indicator_alert.hasConfig = APP_CONFIG_HAS_CONFIG;
            indicator_alert.enable = APP_CONFIG_ENABLED;
            indicator_alert.max = config->data[i].max;
            indicator_alert.min = config->data[i].min;
            indicator_alert.threshold = config->data[i].threshold;
        }
        rc = APP_CONFIG_IndicatorAlertWrite(config->probeID - 1, i, &indicator_alert);
        APP_CHECK_RC(rc)
    }

    return rc;
}

int G2_SERVER_write_sensor_alert_message_process(g2_server_packet_pt packet)
{
    CHECK_PROTOCOL_MESSAGE
    g2_server_write_sensor_alert_message_pt pmsg = (g2_server_write_sensor_alert_message_pt)packet->parsed;
    CHECK_PROTOCOL_CHECK_WRITE_MESSAGE(pmsg->oper)
    g2_server_sensor_alert_message_pt config = &pmsg->sensor;
    CHECK_PROTOCOL_PROBE_ID_MESSAGE(config->probeID)

    int rc = save_protocol_to_sensor_alert(config);
    CHECK_CONFIG_MESSAGE_RC(rc)
    rc = APP_CONFIG_IndicatorAlertLoad();
    CHECK_CONFIG_MESSAGE_RC(rc)
    BSP_PROTOCOL_send_ok_response(packet);
    return PROTOCOL_OK;
}

int G2_SERVER_delete_sensor_alert_message_process(g2_server_packet_pt packet)
{
    CHECK_PROTOCOL_MESSAGE
    g2_server_delete_sensor_alert_message_pt pmsg = (g2_server_delete_sensor_alert_message_pt)packet->parsed;
    CHECK_PROTOCOL_CHECK_DELETE_MESSAGE(pmsg->oper)
    CHECK_PROTOCOL_PROBE_ID_MESSAGE(pmsg->probeID)
    APP_CONFIG_IndicatorAlert_t indicator_alert;
    APP_CONFIG_IndicatorAlertInit(&indicator_alert);
    int rc = 0;
    for (int i = 0; i < INDICATOR_SIZE; i++)
    {
        rc = APP_CONFIG_IndicatorAlertWrite(pmsg->probeID - 1, i, &indicator_alert);
        CHECK_CONFIG_MESSAGE_RC(rc)
    }
    rc = APP_CONFIG_IndicatorAlertLoad();
    CHECK_CONFIG_MESSAGE_RC(rc)
    BSP_PROTOCOL_send_ok_response(packet);
    return PROTOCOL_OK;
}

#ifdef BOARD_HAS_RELAY
int G2_SERVER_read_sensor_limit_message_process(g2_server_packet_pt packet)
{
    CHECK_PROTOCOL_MESSAGE
    g2_server_probe_id_message_pt pmsg = (g2_server_probe_id_message_pt)packet->parsed;
    CHECK_PROTOCOL_PROBE_ID_MESSAGE(pmsg->probeID)

    g2_server_sensor_limit_message_t message;
    G2_SERVER_sensor_limit_message_init(&message);
    APP_CONFIG_Limit_pt limit;
    message.probeID = pmsg->probeID;
    for (int indicatorID = 0; indicatorID < INDICATOR_SIZE; indicatorID++)
    {
        message.sensors[indicatorID].relayID = 1;
        message.sensors[indicatorID].gradient = 0;
        for (int taskID = 0; taskID < APP_CONFIG_MAX_LIMIT_TASK; taskID++)
        {
            limit = &g_appConfigLimit[pmsg->probeID - 1][indicatorID][taskID];
            if (HAS_CONFIG_IS_VALID(limit))
            {
                message.sensors[indicatorID].data[taskID].relay_channel = limit->channel;
                message.sensors[indicatorID].data[taskID].type = limit->type;
                message.sensors[indicatorID].data[taskID].max = limit->upLimit;
                message.sensors[indicatorID].data[taskID].min = limit->downLimit;
                message.sensors[indicatorID].gradient |= 0x80 >> taskID;
            }
        }
        if (message.sensors[indicatorID].gradient > 0)
        {
            message.config |= 0x8000 >> indicatorID;
        }
    }
    BSP_PROTOCOL_send_read_sensor_limit_message(packet, &message);
    return PROTOCOL_OK;
}

static int save_protocol_to_sensor_limit(g2_server_sensor_limit_message_pt config)
{
    uint8_t indicatorID = config_bit_to_index(config->config);
    if (indicatorID < 1 || indicatorID > INDICATOR_SIZE)
    {
        return APP_ERROR;
    }
    uint8_t taskID = config_bit_to_index((config->sensors[indicatorID - 1].gradient) << 8);
    if (taskID < 1 || taskID > APP_CONFIG_MAX_LIMIT_TASK)
    {
        return APP_ERROR;
    }
    // APP_CONFIG_Limit_pt limit = g_appConfigLimit[config->probeID - 1][indicatorID - 1][taskID - 1];
    APP_CONFIG_Limit_t limit;
    APP_CONFIG_LimitInit(&limit);
    g2_server_sensor_limit_data_message_pt data = &config->sensors[indicatorID - 1].data[taskID - 1];
    limit.hasConfig = APP_CONFIG_HAS_CONFIG;
    limit.enable = APP_CONFIG_ENABLED;
    limit.relayID = config->sensors[indicatorID - 1].relayID;
    limit.channel = data->relay_channel;
    limit.type = data->type;
    limit.upLimit = data->max;
    limit.downLimit = data->min;
    #if 0
    APP_CONFIG_Sensor_pt cmsg;
    cmsg = &g_appConfigSensor[PROBE_SIZE - 1][indicatorID - 1];
    if(!APP_SENSOR_IS_ENABLE(cmsg))
    {
        return APP_ERROR;
    }
    #endif
    return APP_CONFIG_LimitWrite(config->probeID - 1, indicatorID - 1, taskID - 1, &limit);
}

int G2_SERVER_write_sensor_limit_message_process(g2_server_packet_pt packet)
{
    int rc = 0;
    CHECK_PROTOCOL_MESSAGE
    g2_server_write_sensor_limit_message_pt pmsg = (g2_server_write_sensor_limit_message_pt)packet->parsed;
    CHECK_PROTOCOL_CHECK_WRITE_MESSAGE(pmsg->oper)
    g2_server_sensor_limit_message_pt config = &pmsg->sensor;
    CHECK_PROTOCOL_PROBE_ID_MESSAGE(config->probeID)
    rc = save_protocol_to_sensor_limit(config);
    CHECK_CONFIG_MESSAGE_RC(rc)
    rc = APP_CONFIG_LimitLoad();
    CHECK_CONFIG_MESSAGE_RC(rc)
    BSP_PROTOCOL_send_ok_response(packet);
    return PROTOCOL_OK;
}

int G2_SERVER_delete_sensor_limit_message_process(g2_server_packet_pt packet)
{
    int rc = 0;
    CHECK_PROTOCOL_MESSAGE
    g2_server_delete_sensor_limit_message_pt pmsg = (g2_server_delete_sensor_limit_message_pt)packet->parsed;
    CHECK_PROTOCOL_CHECK_DELETE_MESSAGE(pmsg->oper)
    CHECK_PROTOCOL_PROBE_ID_MESSAGE(pmsg->probeID)
    CHECK_CONFIG_MESSAGE_RC(rc)
    uint8_t indicatorID = config_bit_to_index(pmsg->config);
    CHECK_PROTOCOL_INDICATOR_ID_MESSAGE(indicatorID)
    uint8_t mask = 0x80;
    for (int taskID = 0; taskID < APP_CONFIG_MAX_LIMIT_TASK; taskID++)
    {
        mask = 0x80 >> taskID;
        if ((pmsg->gradient & mask) == mask)
        {
            rc = APP_CONFIG_LimitWriteEnable(pmsg->probeID - 1, indicatorID - 1, taskID, &g_appConfigLimit[pmsg->probeID - 1][indicatorID - 1][taskID], 0, 0);
            CHECK_CONFIG_MESSAGE_RC(rc)
        }
    }
    BSP_PROTOCOL_send_ok_response(packet);
    return PROTOCOL_OK;
}
#else
int G2_SERVER_read_sensor_limit_message_process(g2_server_packet_pt packet)
{
    BSP_PROTOCOL_send_error_response(packet);
    return PROTOCOL_ERROR;
}

int G2_SERVER_write_sensor_limit_message_process(g2_server_packet_pt packet)
{
    BSP_PROTOCOL_send_error_response(packet);
    return PROTOCOL_ERROR;
}

int G2_SERVER_delete_sensor_limit_message_process(g2_server_packet_pt packet)
{
    BSP_PROTOCOL_send_error_response(packet);
    return PROTOCOL_ERROR;
}
#endif

int G2_SERVER_write_calibrate_message_process(g2_server_packet_pt packet)
{
    CHECK_PROTOCOL_MESSAGE
    g2_server_calibrate_message_pt pmsg = (g2_server_calibrate_message_pt)packet->parsed;
    CHECK_PROTOCOL_PROBE_ID_MESSAGE(pmsg->probeID)
    uint8_t sensorID = config_bit_to_index(pmsg->config);
    CHECK_PROTOCOL_PROBE_SENSOR_MESSAGE(sensorID)
    int rc = APP_SENSOR_Calibrate(pmsg->probeID - 1, sensorID - 1, pmsg->type, &pmsg->params);
    if (rc != APP_OK)
    {
        BSP_PROTOCOL_send_response(packet, 0xe6);
        return PROTOCOL_OK;
    }
    BSP_PROTOCOL_send_ok_response(packet);
    return PROTOCOL_OK;
}

int G2_SERVER_read_sensor_data_message_process(g2_server_packet_pt packet)
{
    CHECK_PROTOCOL_MESSAGE
    g2_server_probe_id_message_pt pmsg = (g2_server_probe_id_message_pt)packet->parsed;
    CHECK_PROTOCOL_PROBE_ID_MESSAGE(pmsg->probeID)
    g2_server_sensor_data_message_t message;
    G2_SERVER_sensor_data_message_init(&message);
    message.probeID = pmsg->probeID;
    message.config = 0;
    uint8_t week;

#ifdef SUPPORT_PUMP
    // APP_PUMP_Check(&changed); 主动请求的数据，服务器不保存 
    if (g_appPump.status == APP_PUMP_STATUS_ON)
    {
        message.pump_status = 0x6000;
        if (g_appPump.check != APP_PUMP_CHECK_NORMAL)
            message.pump_broken_status = 0x6000;
    }
#else
    message.pump_status = 0;
    message.pump_broken_status = 0;
#endif
    BSP_RTC_get(&message.time.year, &message.time.month, &message.time.day, &message.time.hour, &message.time.minute, &message.time.second, &week);
    APP_WATER_IndicatorValue_pt water_indicator;
    int rc = 0, rc2 = 0;
    uint16_t u_value = 0;
    for (int indicatorID = 0; indicatorID < INDICATOR_SIZE; indicatorID++)
    {
        rc = APP_SENSORS_WaterIndicatorGet(indicatorID, &water_indicator);
        if (rc == APP_OK && water_indicator->sensor != NULL)
        {
            message.config |= 0x8000 >> indicatorID;
            rc2 = water_indicator_to_uint16_t(indicatorID, water_indicator->value, &u_value);
            if (water_indicator->status == RS485_OK && rc2 == APP_OK)
            {
                message.indicators[indicatorID].value = u_value;
                message.indicators[indicatorID].probe_status = 0;
                message.indicators[indicatorID].value_mv = water_indicator->vm;
                APP_SENSORS_CheckIndicatorAlert(pmsg->probeID - 1, indicatorID, &message.indicators[indicatorID]);
            }
            else
            {
                message.indicators[indicatorID].probe_status = 0xe0;
#ifdef PH202_ADC_DEBUG
                if(1 == indicatorID)
                {
                    APP_LOG_Trace("[0x%08x]:vm = %d\r\n", (uint32_t)(&water_indicator->vm), water_indicator->vm);
                }
                message.indicators[indicatorID].value_mv = water_indicator->vm;
#endif
            }
        }
    }
    BSP_PROTOCOL_send_read_sensor_data_message(packet, &message);
    return PROTOCOL_OK;
}

// 应答服务器读取传感器信息 
int G2_SERVER_read_device_sensors_message_process(g2_server_packet_pt packet)
{
    g2_server_device_sensors_message_t message;
    G2_SERVER_device_sensors_message_init(&message);
    APP_SENSORS_ReadDeviceSensors(&message);
    BSP_PROTOCOL_send_read_device_sensors_message(packet, &message);
    return PROTOCOL_OK;
}

// 主动给服务器上报传感器信息 
int G2_SERVER_upload_device_sensors_message_process(void)
{
    g2_server_packet_t packet;
    G2_SERVER_PACKET_init(&packet);
    packet.parsed = packet.buffer;
    g2_server_device_sensors_message_t message;
    G2_SERVER_device_sensors_message_init(&message);
    APP_SENSORS_ReadDeviceSensors(&message);
    BSP_PROTOCOL_send_read_device_sensors_message(&packet, &message);
    return PROTOCOL_OK;
}
