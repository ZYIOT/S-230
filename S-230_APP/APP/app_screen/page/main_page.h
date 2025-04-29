#include "app_sensors.h"

#ifdef SUPPORT_L6
#define SCREEN_INDICATOR_ERROR_TIMEOUT 60000
#else
#define SCREEN_INDICATOR_ERROR_TIMEOUT 150000
#endif

char *get_sensor_indicator_value_string(uint8_t sensorID, char *format, get_sensor_indicator_value_fun fun)
{
    GET_SENSOR(sensorID, "--", indicator);
    if (sensor->indicator->status == RS485_OK)
    {
        snprintf(app_screen_msg, APP_SCREEN_MSG_BUFFER_SIZE - 1, format, fun(sensor));
    }
    else
    {
        if (HARDWARE_GET_TICK() > SCREEN_INDICATOR_ERROR_TIMEOUT)
        {
            snprintf(app_screen_msg, APP_SCREEN_MSG_BUFFER_SIZE - 1, "¹ÊÕÏ");
        }
        else
        {
            snprintf(app_screen_msg, APP_SCREEN_MSG_BUFFER_SIZE - 1, "--");
        }
    }
    return app_screen_msg;
}

char *get_sensor_indicator_vm_string(uint8_t sensorID, char *format, get_sensor_indicator_vm_fun fun)
{
    GET_SENSOR(sensorID, "--", indicator);
    if (sensor->indicator->status == RS485_OK)
    {
        snprintf(app_screen_msg, APP_SCREEN_MSG_BUFFER_SIZE - 1, format, fun(sensor));
    }
    else
    {
        if (HARDWARE_GET_TICK() > SCREEN_INDICATOR_ERROR_TIMEOUT)
        {
            snprintf(app_screen_msg, APP_SCREEN_MSG_BUFFER_SIZE - 1, "¹ÊÕÏ");
        }
        else
        {
            snprintf(app_screen_msg, APP_SCREEN_MSG_BUFFER_SIZE - 1, "--");
        }
    }
    return app_screen_msg;
}

int get_water_indicator_value(uint8_t indicatorID, double *value, uint8_t *data_status)
{
    APP_WATER_IndicatorValue_pt water_indicator;
    int rc = APP_SENSORS_WaterIndicatorGet(indicatorID, &water_indicator);
    if (rc != APP_OK || water_indicator->sensor == NULL)
    {
        return APP_ERROR;
    }
    uint16_t u_value = 0;
    int rc1 = mean_filter_get(&water_indicator->filter, value);
    int rc2 = water_indicator_to_uint16_t(indicatorID, *value, &u_value);
    if (rc1 != APP_OK || rc2 != APP_OK || water_indicator->errorCount > 3)
    {
        return APP_ERROR;
    }
    g2_server_sensor_data_indicator_message_t indicator;
    indicator.value = u_value;
    APP_SENSORS_CheckIndicatorAlert(0, indicatorID, &indicator);
    *data_status = indicator.data_status;
    return APP_OK;
}
int get_sensor_indicator_visibility(uint8_t sensorID)
{
    GET_SENSOR(sensorID, 0, id);
    return 1;
}

int get_sensor_temperature_visibility(void)
{
    return get_sensor_indicator_visibility(RS485_DRIVER_SENSOR_ID_DO);
}

int get_sensor_do_visibility(void)
{
    return get_sensor_indicator_visibility(RS485_DRIVER_SENSOR_ID_DO);
}

int get_sensor_ph_visibility(void)
{
    return get_sensor_indicator_visibility(RS485_DRIVER_SENSOR_ID_PH);
}

char *get_water_indicator_value_string(uint8_t indicatorID, char *format)
{
    double value;
    uint8_t data_status;
    int rc = get_water_indicator_value(indicatorID, &value, &data_status);
    if (rc != APP_OK)
    {
        if (HARDWARE_GET_TICK() > SCREEN_INDICATOR_ERROR_TIMEOUT)
        {
            return "¹ÊÕÏ";
        }
        else
        {
            return "--";
        }
    }
    char alert = 0x20;
    switch (data_status)
    {
    case 0x01:
        alert = 0x18;
        break;
    case 0x02:
        alert = 0x19;
        break;
    }
    snprintf(app_screen_msg, APP_SCREEN_MSG_BUFFER_SIZE - 1, format, value, alert);
    return app_screen_msg;
}

char *get_water_temperature()
{
    return get_water_indicator_value_string(WATER_INDICATOR_INDEX_TEMPERATURE, "%.1f %c");
}

char *get_water_do()
{

    return get_water_indicator_value_string(WATER_INDICATOR_INDEX_DO, "%.2f %c");
}

char *get_water_ph()
{
    return get_water_indicator_value_string(WATER_INDICATOR_INDEX_PH, "%.2f %c");
}

// char *get_water_chlorophyll()
// {
//     return get_water_indicator_value_string(WATER_INDICATOR_INDEX_CHLOROPHYLL, "%d");
// }
// int get_water_chlorophyll_visibility(void)
// {
//     return get_water_indicator_visibility(RS485_DRIVER_SENSOR_ID_CHLOROPHYLL);
// }
// char *get_water_cod()
// {
//     return get_water_indicator_value_string(WATER_INDICATOR_INDEX_COD, "%d");
// }
// int get_water_cod_visibility(void)
// {
//     return get_water_indicator_visibility(RS485_DRIVER_SENSOR_ID_COD);
// }
// char *get_water_nh3()
// {
//     return get_water_indicator_value_string(WATER_INDICATOR_INDEX_NH3, "%.2f");
// }
// int get_water_nh3_visibility(void)
// {
//     return get_water_indicator_visibility(RS485_DRIVER_SENSOR_ID_NH3);
// }
static int has_sensor_by_id(uint8_t sensorID)
{
    rs485_sensor_pt sensor = NULL;
    int rc = APP_SENSORS_SensorGet(sensorID, &sensor);
    return (rc == APP_OK) ? 1 : 0;
}

static int has_do_sensor_check(app_screen_page_pt page)
{
    return has_sensor_by_id(RS485_DRIVER_SENSOR_ID_DO);
}

static int has_ph_sensor_check(app_screen_page_pt page)
{
    return has_sensor_by_id(RS485_DRIVER_SENSOR_ID_PH);
}

static int has_sensor_check(app_screen_page_pt page)
{
    return has_sensor_by_id(RS485_DRIVER_SENSOR_ID_DO) || has_sensor_by_id(RS485_DRIVER_SENSOR_ID_PH);
}

int has_sensors_visibility(void)
{
    rs485_sensor_pt sensor = NULL;
    int rc1 = APP_SENSORS_SensorGet(RS485_DRIVER_SENSOR_ID_DO, &sensor);
    int rc2 = APP_SENSORS_SensorGet(RS485_DRIVER_SENSOR_ID_PH, &sensor);
    return (rc1 == APP_OK || rc2 == APP_OK) ? 1 : 0;
}

int no_sensors_visibility(void)
{
    return !has_sensors_visibility();
}

app_screen_paged_message_node_t main_messages[] = {
    {"ÎÂ¶È: ", .handler = &get_water_temperature, .visibility = &get_sensor_temperature_visibility},
    {"ÈÜÑõ: ", .handler = &get_water_do, .visibility = &get_sensor_do_visibility},
    {"pH: ", .handler = &get_water_ph, .visibility = &get_sensor_ph_visibility},

    // {"Ò¶ÂÌËØ:", .handler = &get_water_chlorophyll, .visibility = &get_sensor_chlorophyll_visibility},
    // {"COD:", .handler = &get_water_cod, .visibility = &get_sensor_cod_visibility},
    // {"°±µª:", .handler = &get_water_nh3, .visibility = &get_sensor_nh3_visibility},
    {"  Î´ÅäÖÃ´«¸ÐÆ÷", .handler = NULL, .visibility = &no_sensors_visibility},
    APP_SCREEN_PAGED_MESSAGE_NODE_END};

NEW_PAGED_MESSAGE_PAGE(main_page, main_messages, NULL, NULL, NULL)
