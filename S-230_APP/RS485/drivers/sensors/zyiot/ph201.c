#include "drivers/sensors/zyiot/ph201.h"

static int _write_calibrate(rs485_sensor_t *rs485, uint8_t type, void *params);
static int _read_value1(rs485_sensor_pt rs485, rs485_sensor_indicator_pt indicator);
static int _read_info(rs485_sensor_pt rs485);
static int _match_pn_name(rs485_sensor_pt rs485, void *params);
static int _match_model(rs485_sensor_pt rs485, uint8_t model, uint8_t theory);
#include "drivers/sensors/zyiot/zyiot_sensor_common.h"

static rs485_sensor_action_t _action = {
    .recognition_mode = RECOGNITION_MODE_AUTO,
    .read_value1 = _read_value1,
    .read_info = _read_info,
    .write_calibrate = _write_calibrate,
    .match_pn = _match_pn_name,
    .match_model = _match_model,
};

static uint8_t __enabled_calibrate_types[] = {RS485_SENSOR_PH_CALIBRATE_7,
                                              RS485_SENSOR_PH_CALIBRATE_4,
                                              RS485_SENSOR_PH_CALIBRATE_10};

#define __CHECK_ENABLED_CALIBRATE_TYPE(type)                       \
    if (get_index_uint8_t(__enabled_calibrate_types, 3, type) < 0) \
    {                                                              \
        return RS485_PARAM_ERROR;                                  \
    }

// *** start 增加同驱动传感器时，增加以下部分内容
#define MAX_MATCH_TYPE_COUNT  (2)
static char _pn_match_name[MAX_MATCH_TYPE_COUNT][RS485_SENSOR_MAX_PN_LEN] = {"PH-20", "PH-20"};
const static char _manufacturer_match[MAX_MATCH_TYPE_COUNT] = {RS485_DRIVER_PH_MANUFACTURER_ZYIOT, RS485_DRIVER_PH_MANUFACTURER_ZYIOT};
const static char _model_match[MAX_MATCH_TYPE_COUNT] = {RS485_SENSOR_MODEL_ID_PH201, RS485_SENSOR_MODEL_ID_PH201};
const static char _pn_name[MAX_MATCH_TYPE_COUNT][RS485_SENSOR_MAX_PN_LEN] = {"PH-201", "PH202"};
// *** end

#define SENSOR_ID   (RS485_DRIVER_SENSOR_ID_PH)
static rs485_sensor_driver_t sensor_driver;
const static char _PN_default[RS485_SENSOR_MAX_PN_LEN] = {"PH-201"};
static char _PN[RS485_SENSOR_MAX_PN_LEN];


static int _init_sensor_driver(rs485_sensor_driver_pt sd, uint8_t match_sensor_index)
{
    if(match_sensor_index >= MAX_MATCH_TYPE_COUNT)
    {
        return -1;
    }
    if(0 == match_sensor_index)
    {
        strcpy(_PN, _PN_default);
    }
    _action.pn_match_name = _pn_match_name[match_sensor_index];
    sd->manufacturer = _manufacturer_match[match_sensor_index];
    sd->model = _model_match[match_sensor_index];
    sd->protocol = 0;
    sd->id = SENSOR_ID;
    sd->action = &_action;
    return 0;
}

static int _write_calibrate(rs485_sensor_pt rs485, uint8_t type, void *params)
{
    __CHECK_ENABLED_CALIBRATE_TYPE(type)
    return RS485_write_register(rs485->port, rs485->id, PH201_CALIBRATE_REG, type, NULL, NULL, NULL, NULL);
}

static int _parse_value(uint8_t buffer[RS485_BUFFER_MAX_SIZE], size_t len, void *ret, void *args)
{
    rs485_sensor_indicator_pt indicator = (rs485_sensor_indicator_pt)ret;
    indicator->value1 = read_uint16_t_BE(&buffer[2]) / 100.0;
    indicator->vm1 = read_int16_t_BE(&buffer[4]) / 10.0;
    indicator->value2 = 0;
    indicator->error_code = 0;
    indicator->status = RS485_OK;
    return RS485_OK;
}

static int _read_value1(rs485_sensor_pt rs485, rs485_sensor_indicator_pt indicator)
{
    int rc = RS485_read_register(rs485->port, rs485->id, PH201_VALUE_REG, 3, indicator, _parse_value, _parse_value_error, NULL);
    if (rc != RS485_OK)
    {
        indicator->status = rc;
        return rc;
    }
    return RS485_OK;
}

static int _match_pn_name(rs485_sensor_pt rs485, void *params)
{
    char (*pn_name)[RS485_SENSOR_MAX_PN_LEN] = _pn_match_name;
    _init_sensor_driver(&sensor_driver, 0);
    for(uint8_t i=0; i<MAX_MATCH_TYPE_COUNT; i++)
    {
        if(0 == strncmp((char *)params, *pn_name, strlen(*pn_name)))
        {
            _init_sensor_driver(&sensor_driver, i);
            return 0;
        }
        pn_name++;
    }
    return -1;
}

static int _match_model(rs485_sensor_pt rs485, uint8_t model, uint8_t theory)
{
    _init_sensor_driver(&sensor_driver, 0);
    strcpy(_PN, _PN_default);
    for(uint8_t i=0; i<MAX_MATCH_TYPE_COUNT; i++)
    {
        if(model == _model_match[i])
        {
            _init_sensor_driver(&sensor_driver, i);
            strcpy(_PN, _pn_name[i]);
            return 0;
        }
    }
    return -1;
}

int RS485_DRIVER_zyiot_ph201_init(void)
{
    int rc = 0;
    _action.PN = _PN;
    _init_sensor_driver(&sensor_driver, 0);
    rc = RS485_DRIVER_regsiter(&sensor_driver);
    RS485_CHECK_RC(rc)
#ifdef ENABLE_PH_SENSOR_ADDR_18
    static rs485_sensor_driver_t sd_18 = {.manufacturer = RS485_DRIVER_PH_MANUFACTURER_ZYIOT, .model = RS485_SENSOR_MODEL_ID_PH201, .protocol = 0, .id = 18, .action = &_action};
    rc = RS485_DRIVER_regsiter(&sd_18);
    RS485_CHECK_RC(rc)
#endif
    return rc;
}
