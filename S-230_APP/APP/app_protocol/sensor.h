#include "water_indicator.h"
#include "app_sensors.h"

#ifdef SUPPORT_PUMP
#include "app_pump.h"
#endif

int G2_SERVER_read_salinity_message_process(g2_server_packet_pt packet)
{
    CHECK_PROTOCOL_MESSAGE
    g2_server_probe_id_message_pt pmsg = (g2_server_probe_id_message_pt)packet->parsed;
    CHECK_PROTOCOL_PROBE_ID_MESSAGE(pmsg->probe_id)
    app_config_compensation_pt cmsg = &(app_config_compensation[pmsg->probe_id - 1][WATER_INDICATOR_INDEX_SALINITY]);
    // CHECK_HAS_CONFIG_IS_VALID(cmsg)
    g2_server_salinity_message_t message = {0};
    G2_SERVER_salinity_message_init(&message);
    message.probe_id = pmsg->probe_id;
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
    CHECK_PROTOCOL_PROBE_ID_MESSAGE(pmsg->probe_id)
    app_config_compensation_pt cmsg = &(app_config_compensation[pmsg->probe_id - 1][WATER_INDICATOR_INDEX_SALINITY]);
    int rc = water_indicator_from_uint16_t(WATER_INDICATOR_INDEX_SALINITY, pmsg->salinity, &value);
    CHECK_CONFIG_MESSAGE_RC(rc)
    rc = APP_SENSOR_write_compensation(RS485_DRIVER_SENSOR_ID_DO, WATER_INDICATOR_INDEX_SALINITY, value, 1);
    if (rc != APP_OK)
    {
        BSP_PROTOCOL_send_response(packet, 0xe7);
        return PROTOCOL_OK;
    }
    rc = APP_CONFIG_compensation_write_data(pmsg->probe_id - 1, WATER_INDICATOR_INDEX_SALINITY, cmsg, APP_CONFIG_HAS_CONFIG, pmsg->salinity);
    CHECK_CONFIG_MESSAGE_RC(rc)
    BSP_PROTOCOL_send_ok_response(packet);
    return PROTOCOL_OK;
}

int G2_SERVER_read_sensor_config_message_process(g2_server_packet_pt packet)
{
    CHECK_PROTOCOL_MESSAGE
    g2_server_probe_id_message_pt pmsg = (g2_server_probe_id_message_pt)packet->parsed;
    CHECK_PROTOCOL_PROBE_ID_MESSAGE(pmsg->probe_id)
    app_config_sensor_pt cmsg;
    g2_server_sensor_config_message_t message;
    G2_SERVER_sensor_config_message_init(&message);
    message.probe_id = pmsg->probe_id;
    message.config = 0;
    int i = 0;
    for (i = 0; i < PROBE_SENSOR_SIZE; i++)
    {
        cmsg = &app_config_sensor[pmsg->probe_id - 1][i];
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


int G2_SERVER_upload_sensor_config_message_process(uint8_t probe_id)
{
    g2_server_packet_t packet;
    G2_SERVER_PACKET_init(&packet);
    packet.buffer[0] = probe_id;
    packet.parsed = packet.buffer;
    g2_server_probe_id_message_pt pmsg = (g2_server_probe_id_message_pt)packet.parsed;
    app_config_sensor_pt cmsg;
    g2_server_sensor_config_message_t message;
    G2_SERVER_sensor_config_message_init(&message);
    message.probe_id = pmsg->probe_id;
    message.config = 0;
    int i = 0;
    for (i = 0; i < PROBE_SENSOR_SIZE; i++)
    {
        cmsg = &app_config_sensor[pmsg->probe_id - 1][i];
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
    app_config_sensor_t app_config_sensor;

    for (i = 0; i < PROBE_SENSOR_SIZE; i++)
    {
        APP_CONFIG_sensor_init(&app_config_sensor);
        mask = 0x8000 >> i;
        if ((config->config & mask) == mask)
        {
            app_config_sensor.has_config = APP_CONFIG_HAS_CONFIG;
            app_config_sensor.enable = APP_CONFIG_ENABLED;
            app_config_sensor.manufacturer = config->data[i].manufacturer;
            app_config_sensor.model = config->data[i].type;
        }
        rc = APP_CONFIG_sensor_write(config->probe_id - 1, i, &app_config_sensor);
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
    CHECK_PROTOCOL_PROBE_ID_MESSAGE(config->probe_id)

    int rc = save_protocol_to_sensor_config(config);
    CHECK_CONFIG_MESSAGE_RC(rc)
    rc = APP_CONFIG_sensor_load();
    CHECK_CONFIG_MESSAGE_RC(rc)
    APP_SENSORS_refresh_new();
    BSP_PROTOCOL_send_ok_response(packet);
    return PROTOCOL_OK;
}

int G2_SERVER_delete_sensor_config_message_process(g2_server_packet_pt packet)
{
    CHECK_PROTOCOL_MESSAGE
    g2_server_delete_sensor_config_message_pt pmsg = (g2_server_delete_sensor_config_message_pt)packet->parsed;
    CHECK_PROTOCOL_CHECK_DELETE_MESSAGE(pmsg->oper)
    CHECK_PROTOCOL_PROBE_ID_MESSAGE(pmsg->probe_id)
    int i = 0;
    int rc = 0;
    app_config_sensor_t config_sensor;
    APP_CONFIG_sensor_init(&config_sensor);
    for (i = 0; i < PROBE_SENSOR_SIZE; i++)
    {
        rc = APP_CONFIG_sensor_write(pmsg->probe_id - 1, i, &config_sensor);
        CHECK_CONFIG_MESSAGE_RC(rc)
    }
    rc = APP_CONFIG_sensor_load();
    CHECK_CONFIG_MESSAGE_RC(rc)
    APP_SENSORS_refresh_new();
    BSP_PROTOCOL_send_ok_response(packet);
    return PROTOCOL_OK;
}

int G2_SERVER_read_sensor_alert_message_process(g2_server_packet_pt packet)
{
    CHECK_PROTOCOL_MESSAGE
    g2_server_probe_id_message_pt pmsg = (g2_server_probe_id_message_pt)packet->parsed;
    CHECK_PROTOCOL_PROBE_ID_MESSAGE(pmsg->probe_id)
    g2_server_sensor_alert_message_t message;
    G2_SERVER_sensor_alert_message_init(&message);
    message.probe_id = pmsg->probe_id;
    message.config = 0;
    int i = 0;
    app_config_indicator_alert_pt indicator_alert = NULL;
    for (i = 0; i < INDICATOR_SIZE; i++)
    {
        indicator_alert = &app_config_indicator_alert[pmsg->probe_id - 1][i];
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
    app_config_indicator_alert_t indicator_alert;
    for (i = 0; i < INDICATOR_SIZE; i++)
    {
        APP_CONFIG_indicator_alert_init(&indicator_alert);
        mask = 0x8000 >> i;
        if ((config->config & mask) == mask)
        {
            indicator_alert.has_config = APP_CONFIG_HAS_CONFIG;
            indicator_alert.enable = APP_CONFIG_ENABLED;
            indicator_alert.max = config->data[i].max;
            indicator_alert.min = config->data[i].min;
            indicator_alert.threshold = config->data[i].threshold;
        }
        rc = APP_CONFIG_indicator_alert_write(config->probe_id - 1, i, &indicator_alert);
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
    CHECK_PROTOCOL_PROBE_ID_MESSAGE(config->probe_id)

    int rc = save_protocol_to_sensor_alert(config);
    CHECK_CONFIG_MESSAGE_RC(rc)
    rc = APP_CONFIG_indicator_alert_load();
    CHECK_CONFIG_MESSAGE_RC(rc)
    BSP_PROTOCOL_send_ok_response(packet);
    return PROTOCOL_OK;
}

int G2_SERVER_delete_sensor_alert_message_process(g2_server_packet_pt packet)
{
    CHECK_PROTOCOL_MESSAGE
    g2_server_delete_sensor_alert_message_pt pmsg = (g2_server_delete_sensor_alert_message_pt)packet->parsed;
    CHECK_PROTOCOL_CHECK_DELETE_MESSAGE(pmsg->oper)
    CHECK_PROTOCOL_PROBE_ID_MESSAGE(pmsg->probe_id)
    app_config_indicator_alert_t indicator_alert;
    APP_CONFIG_indicator_alert_init(&indicator_alert);
    int rc = 0;
    for (int i = 0; i < INDICATOR_SIZE; i++)
    {
        rc = APP_CONFIG_indicator_alert_write(pmsg->probe_id - 1, i, &indicator_alert);
        CHECK_CONFIG_MESSAGE_RC(rc)
    }
    rc = APP_CONFIG_indicator_alert_load();
    CHECK_CONFIG_MESSAGE_RC(rc)
    BSP_PROTOCOL_send_ok_response(packet);
    return PROTOCOL_OK;
}

#ifdef BOARD_HAS_RELAY
int G2_SERVER_read_sensor_limit_message_process(g2_server_packet_pt packet)
{
    CHECK_PROTOCOL_MESSAGE
    g2_server_probe_id_message_pt pmsg = (g2_server_probe_id_message_pt)packet->parsed;
    CHECK_PROTOCOL_PROBE_ID_MESSAGE(pmsg->probe_id)

    g2_server_sensor_limit_message_t message;
    G2_SERVER_sensor_limit_message_init(&message);
    app_config_limit_pt limit;
    message.probe_id = pmsg->probe_id;
    for (int indicator_id = 0; indicator_id < INDICATOR_SIZE; indicator_id++)
    {
        message.sensors[indicator_id].relay_id = 1;
        message.sensors[indicator_id].gradient = 0;
        for (int task_id = 0; task_id < APP_CONFIG_MAX_LIMIT_TASK; task_id++)
        {
            limit = &app_config_limit[pmsg->probe_id - 1][indicator_id][task_id];
            if (HAS_CONFIG_IS_VALID(limit))
            {
                message.sensors[indicator_id].data[task_id].relay_channel = limit->channel;
                message.sensors[indicator_id].data[task_id].type = limit->type;
                message.sensors[indicator_id].data[task_id].max = limit->up_limit;
                message.sensors[indicator_id].data[task_id].min = limit->down_limit;
                message.sensors[indicator_id].gradient |= 0x80 >> task_id;
            }
        }
        if (message.sensors[indicator_id].gradient > 0)
        {
            message.config |= 0x8000 >> indicator_id;
        }
    }
    BSP_PROTOCOL_send_read_sensor_limit_message(packet, &message);
    return PROTOCOL_OK;
}

static int save_protocol_to_sensor_limit(g2_server_sensor_limit_message_pt config)
{
    uint8_t indicator_id = config_bit_to_index(config->config);
    if (indicator_id < 1 || indicator_id > INDICATOR_SIZE)
    {
        return APP_ERROR;
    }
    uint8_t task_id = config_bit_to_index((config->sensors[indicator_id - 1].gradient) << 8);
    if (task_id < 1 || task_id > APP_CONFIG_MAX_LIMIT_TASK)
    {
        return APP_ERROR;
    }
    // app_config_limit_pt limit = app_config_limit[config->probe_id - 1][indicator_id - 1][task_id - 1];
    app_config_limit_t limit;
    APP_CONFIG_limit_init(&limit);
    g2_server_sensor_limit_data_message_pt data = &config->sensors[indicator_id - 1].data[task_id - 1];
    limit.has_config = APP_CONFIG_HAS_CONFIG;
    limit.enable = APP_CONFIG_ENABLED;
    limit.relay_id = config->sensors[indicator_id - 1].relay_id;
    limit.channel = data->relay_channel;
    limit.type = data->type;
    limit.up_limit = data->max;
    limit.down_limit = data->min;
    #if 0
    app_config_sensor_pt cmsg;
    cmsg = &app_config_sensor[PROBE_SIZE - 1][indicator_id - 1];
    if(!APP_SENSOR_IS_ENABLE(cmsg))
    {
        return APP_ERROR;
    }
    #endif
    return APP_CONFIG_limit_write(config->probe_id - 1, indicator_id - 1, task_id - 1, &limit);
}

int G2_SERVER_write_sensor_limit_message_process(g2_server_packet_pt packet)
{
    int rc = 0;
    CHECK_PROTOCOL_MESSAGE
    g2_server_write_sensor_limit_message_pt pmsg = (g2_server_write_sensor_limit_message_pt)packet->parsed;
    CHECK_PROTOCOL_CHECK_WRITE_MESSAGE(pmsg->oper)
    g2_server_sensor_limit_message_pt config = &pmsg->sensor;
    CHECK_PROTOCOL_PROBE_ID_MESSAGE(config->probe_id)
    rc = save_protocol_to_sensor_limit(config);
    CHECK_CONFIG_MESSAGE_RC(rc)
    rc = APP_CONFIG_limit_load();
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
    CHECK_PROTOCOL_PROBE_ID_MESSAGE(pmsg->probe_id)
    CHECK_CONFIG_MESSAGE_RC(rc)
    uint8_t indicator_id = config_bit_to_index(pmsg->config);
    CHECK_PROTOCOL_INDICATOR_ID_MESSAGE(indicator_id)
    uint8_t mask = 0x80;
    for (int task_id = 0; task_id < APP_CONFIG_MAX_LIMIT_TASK; task_id++)
    {
        mask = 0x80 >> task_id;
        if ((pmsg->gradient & mask) == mask)
        {
            rc = APP_CONFIG_limit_write_enable(pmsg->probe_id - 1, indicator_id - 1, task_id, &app_config_limit[pmsg->probe_id - 1][indicator_id - 1][task_id], 0, 0);
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
    CHECK_PROTOCOL_PROBE_ID_MESSAGE(pmsg->probe_id)
    uint8_t sensor_id = config_bit_to_index(pmsg->config);
    CHECK_PROTOCOL_PROBE_SENSOR_MESSAGE(sensor_id)
    int rc = APP_SENSOR_calibrate(pmsg->probe_id - 1, sensor_id - 1, pmsg->type, &pmsg->params);
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
    CHECK_PROTOCOL_PROBE_ID_MESSAGE(pmsg->probe_id)
    g2_server_sensor_data_message_t message;
    G2_SERVER_sensor_data_message_init(&message);
    message.probe_id = pmsg->probe_id;
    message.config = 0;
    uint8_t week;

#ifdef SUPPORT_PUMP
    // APP_PUMP_check(&changed); 主动请求的数据，服务器不保存 
    if (app_pump.status == APP_PUMP_STATUS_ON)
    {
        message.pump_status = 0x6000;
        if (app_pump.check != APP_PUMP_CHECK_NORMAL)
            message.pump_broken_status = 0x6000;
    }
#else
    message.pump_status = 0;
    message.pump_broken_status = 0;
#endif
    BSP_RTC_get(&message.time.year, &message.time.month, &message.time.day, &message.time.hour, &message.time.minute, &message.time.second, &week);
    app_water_indicator_value_pt water_indicator;
    int rc = 0, rc2 = 0;
    uint16_t u_value = 0;
    for (int indicator_id = 0; indicator_id < INDICATOR_SIZE; indicator_id++)
    {
        rc = APP_SENSORS_water_indicator_get(indicator_id, &water_indicator);
        if (rc == APP_OK && water_indicator->sensor != NULL)
        {
            message.config |= 0x8000 >> indicator_id;
            rc2 = water_indicator_to_uint16_t(indicator_id, water_indicator->value, &u_value);
            if (water_indicator->status == RS485_OK && rc2 == APP_OK)
            {
                message.indicators[indicator_id].value = u_value;
                message.indicators[indicator_id].probe_status = 0;
                message.indicators[indicator_id].value_mv = water_indicator->vm;
                APP_SENSORS_check_indicator_alert(pmsg->probe_id - 1, indicator_id, &message.indicators[indicator_id]);
            }
            else
            {
                message.indicators[indicator_id].probe_status = 0xe0;
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
    APP_SENSORS_read_device_sensors(&message);
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
    APP_SENSORS_read_device_sensors(&message);
    BSP_PROTOCOL_send_read_device_sensors_message(&packet, &message);
    return PROTOCOL_OK;
}
