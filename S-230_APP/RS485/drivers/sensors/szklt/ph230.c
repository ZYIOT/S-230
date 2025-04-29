#include "drivers/sensors/szklt/ph230.h"

static int _write_calibrate(rs485_sensor_t *rs485, uint8_t type, void *params);
static int _read_value1(rs485_sensor_pt rs485, rs485_sensor_indicator_pt indicator);
static int _read_info(rs485_sensor_pt rs485);
static int _check_sensor_ph230(rs485_sensor_pt rs485, rs485_sensor_driver_pt sd);
static int _check_sensor_compatible(rs485_sensor_pt rs485, rs485_sensor_driver_pt sd);
static int _match_pn_name(rs485_sensor_pt rs485, void *params);
static int _match_model(rs485_sensor_pt rs485, uint8_t model, uint8_t theory);
#include "drivers/sensors/zyiot/zyiot_sensor_common.h"

static rs485_sensor_action_t _action = {
    .recognition_mode = RECOGNITION_MODE_AUTO,
    .check_sensor = _check_sensor_ph230,
    .read_value1 = _read_value1,
    .read_info = _read_info,
    .write_calibrate = _write_calibrate,
    .match_pn = _match_pn_name,
    .match_model = _match_model,
};

#ifdef RS485_ENABLE_COMPATIBLE_MODE
static rs485_sensor_action_t _action_compatible = {
    .recognition_mode = RECOGNITION_MODE_COMPATIBLE,
    .check_sensor = _check_sensor_compatible,
    .read_value1 = _read_value1};
#endif

// *** start 增加同驱动传感器时，增加以下部分内容
#define MAX_MATCH_TYPE_COUNT  (2)
static char _pn_match_name[MAX_MATCH_TYPE_COUNT][RS485_SENSOR_MAX_PN_LEN] = {"PH-23", "PH-28"};
const static char _manufacturer_match[MAX_MATCH_TYPE_COUNT] = {RS485_DRIVER_PH_MANUFACTURER_SZKLT, RS485_DRIVER_PH_MANUFACTURER_SZSG};
const static char _model_match[MAX_MATCH_TYPE_COUNT] = {RS485_SENSOR_MODEL_ID_PH230, RS485_SENSOR_MODEL_ID_PH280};
const char _pn_name[MAX_MATCH_TYPE_COUNT][RS485_SENSOR_MAX_PN_LEN] = {"PH-230", "PH-280"};
// *** end

#define SENSOR_ID   (RS485_DRIVER_SENSOR_ID_PH)
static rs485_sensor_driver_t sensor_driver;
const static char _PN_default[RS485_SENSOR_MAX_PN_LEN] = {"PH-230"};
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

// static int _parse_value_error(uint8_t error_code, void *ret, void *args)
// {
//     rs485_sensor_indicator_pt indicator = (rs485_sensor_indicator_pt)ret;
//     indicator->error_code = error_code;
//     indicator->status = RS485_OK;
//     return RS485_OK;
// }

static int _check_valid_indicator(rs485_sensor_indicator_pt indicator)
{
    if (
        indicator->value1 > 14 ||
        indicator->value1 < 0.01f ||
        indicator->vm1 > 2000 ||
        indicator->vm1 < -2000)
        return RS485_ERROR;
    return RS485_OK;
}

static int _check_sensor_compatible(rs485_sensor_pt rs485, rs485_sensor_driver_pt sd)
{
    rs485_sensor_indicator_t indicator = {0};
    RS485_delay_ms(10);
    int rc = _read_value1(rs485, &indicator);
    RS485_CHECK_RC(rc)
    rc = _check_valid_indicator(&indicator);
    RS485_CHECK_RC(rc)
    // rs485->PN = sd->action->PN;
    return RS485_OK;
}

static int _check_sensor_ph230(rs485_sensor_pt rs485, rs485_sensor_driver_pt sd)
{
    uint8_t buffer[2] = {0};
    _init_sensor_driver(sd, 0);
    RS485_delay_ms(10);
    int rc = RS485_read_register(rs485->port, rs485->id, PH230_SENSOR_ID_REG, 1, buffer, RS485_DRIVER_copy_buffer, NULL, NULL);
    RS485_CHECK_RC(rc)
#ifdef ENABLE_PH_SENSOR_ADDR_18
    if (read_uint16_t_BE(&buffer[0]) != RS485_DRIVER_SENSOR_ID_PH && read_uint16_t_BE(&buffer[0]) != 18)
#else
    if (read_uint16_t_BE(&buffer[0]) != RS485_DRIVER_SENSOR_ID_PH)
#endif
    {
        return RS485_ERROR;
    }
    return _check_sensor_compatible(rs485, sd);
}

static uint8_t __enabled_calibrate_types[] = {RS485_SENSOR_PH_CALIBRATE_7,
                                              RS485_SENSOR_PH_CALIBRATE_4,
                                              RS485_SENSOR_PH_CALIBRATE_10};

#define __CHECK_ENABLED_CALIBRATE_TYPE(type)                       \
    if (get_index_uint8_t(__enabled_calibrate_types, 3, type) < 0) \
    {                                                              \
        return RS485_PARAM_ERROR;                                  \
    }

static int _write_calibrate(rs485_sensor_pt rs485, uint8_t type, void *params)
{
    __CHECK_ENABLED_CALIBRATE_TYPE(type)
    return RS485_write_register(rs485->port, rs485->id, PH230_CALIBRATE_REG, type, NULL, NULL, NULL, NULL);
}

static int _parse_value(uint8_t buffer[RS485_BUFFER_MAX_SIZE], size_t len, void *ret, void *args)
{
    rs485_sensor_indicator_pt indicator = (rs485_sensor_indicator_pt)ret;
    indicator->value1 = read_uint16_t_BE(&buffer[2]) / 100.0;
    indicator->vm1 = read_int16_t_BE(&buffer[4]) / 10.0;
    indicator->value2 = read_uint16_t_BE(&buffer[0]) / 10.0;
    indicator->error_code = 0;
    indicator->status = RS485_OK;
    return RS485_OK;
}

static int _read_value1(rs485_sensor_pt rs485, rs485_sensor_indicator_pt indicator)
{
    int rc = RS485_read_register(rs485->port, rs485->id, PH230_VALUE_REG, 3, indicator, _parse_value, _parse_value_error, NULL);
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

int RS485_DRIVER_szklt_ph230_init(void)
{
    int rc = 0;
    _action.PN = _PN;
    _init_sensor_driver(&sensor_driver, 0);
    rc = RS485_DRIVER_regsiter(&sensor_driver);
    RS485_CHECK_RC(rc)

#ifdef ENABLE_PH_SENSOR_ADDR_18
    static rs485_sensor_driver_t sd_18 = {.manufacturer = RS485_DRIVER_PH_MANUFACTURER_SZKLT, .model = RS485_SENSOR_MODEL_ID_PH230, .protocol = 0, .id = 18, .action = &_action};
    rc = RS485_DRIVER_regsiter(&sd_18);
    RS485_CHECK_RC(rc)
#endif

#ifdef RS485_ENABLE_COMPATIBLE_MODE
    _action_compatible.pn_match_name = _pn_match_name;
    _action_compatible.PN = _PN_default;
    static rs485_sensor_driver_t sd_compatible = {.manufacturer = RS485_DRIVER_PH_MANUFACTURER_SZKLT, .model = RS485_SENSOR_MODEL_ID_PH230, .protocol = 0, .id = RS485_DRIVER_SENSOR_ID_PH, .action = &_action_compatible};
    rc = RS485_DRIVER_regsiter(&sd_compatible);
    RS485_CHECK_RC(rc)
#endif
    return rc;
}
