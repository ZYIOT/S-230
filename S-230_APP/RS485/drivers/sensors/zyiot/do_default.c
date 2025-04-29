#include "drivers/sensors/zyiot/do_default.h"

static int _write_calibrate(rs485_sensor_t *rs485, uint8_t type, void *params);
static int _read_value1(rs485_sensor_pt rs485, rs485_sensor_indicator_pt indicator);
static int _read_info(rs485_sensor_pt rs485);
static int _read_compensation(rs485_sensor_pt rs485, uint8_t indicatorID, float *value);
static int _write_compensation(rs485_sensor_pt rs485, uint8_t indicatorID, float value);
static int _check_sensor_compatible(rs485_sensor_pt rs485, rs485_sensor_driver_pt sd);
static int _check_sensor_doy170(rs485_sensor_pt rs485, rs485_sensor_driver_pt sd);
static int _match_pn_name(rs485_sensor_pt rs485, void *params);
static int _match_model(rs485_sensor_pt rs485, uint8_t model, uint8_t theory);
#include "drivers/sensors/zyiot/zyiot_sensor_common.h"

static rs485_sensor_action_t _action = {
    .recognition_mode = RECOGNITION_MODE_AUTO,
    .check_sensor = _check_sensor_doy170,
    .read_value1 = _read_value1,
    .read_info = _read_info,
    .read_compensation = _read_compensation,
    .write_compensation = _write_compensation,
    .write_calibrate = _write_calibrate,
    .match_pn = _match_pn_name,
    .match_model = _match_model,
};


// *** start 增加同驱动传感器时，增加以下部分内容
#define MAX_MATCH_TYPE_COUNT  (1)
static char _pn_match_name[MAX_MATCH_TYPE_COUNT][RS485_SENSOR_MAX_PN_LEN] = {"DO-Y17"};
const static char _manufacturer_match[MAX_MATCH_TYPE_COUNT] = {RS485_DRIVER_DO_MANUFACTURER_ZYIOT};
const static char _model_match[MAX_MATCH_TYPE_COUNT] = {RS485_SENSOR_MODEL_ID_DOY170};
const static char _theory_match[MAX_MATCH_TYPE_COUNT] = {SENSOR_THEORY_OPTICS};
// static char _theory_match[MAX_MATCH_TYPE_COUNT] = {SENSOR_THEORY_OPTICS, SENSOR_THEORY_OPTICS};
const static char _pn_name[MAX_MATCH_TYPE_COUNT][RS485_SENSOR_MAX_PN_LEN] = {"DO-Y170"};
// *** end

#define SENSOR_ID   (RS485_DRIVER_SENSOR_ID_DO)
static rs485_sensor_driver_t sensor_driver;
const static char _PN_default[RS485_SENSOR_MAX_PN_LEN] = {"DO-Y170"};
static char _PN[RS485_SENSOR_MAX_PN_LEN];

// 初始化传感器drv的值 
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

static int _check_valid_indicator(rs485_sensor_indicator_pt indicator)
{
    if (
        indicator->status != RS485_OK ||
        indicator->value1 > DO_DEFAULT_TMP_VALUE_MAX ||
        indicator->value2 > DO_DEFAULT_DO_VALUE_MAX ||
        indicator->value1 < DO_DEFAULT_TMP_VALUE_MIN ||
        indicator->value2 < DO_DEFAULT_DO_VALUE_MIN)
        return RS485_ERROR;
    return RS485_OK;
}

static int _check_sensor_compatible(rs485_sensor_pt rs485, rs485_sensor_driver_pt sd)
{
    rs485_sensor_indicator_t indicator = {0};
    indicator.status = RS485_OK;
    int rc = _read_value1(rs485, &indicator);
    RS485_CHECK_RC(rc)
    rc = _check_valid_indicator(&indicator);
    RS485_CHECK_RC(rc)
    // rs485->PN = sd->action->PN;
    return RS485_OK;
}

static int _check_sensor_doy170(rs485_sensor_pt rs485, rs485_sensor_driver_pt sd)
{
    uint8_t buffer[2] = {0};
    int rc = RS485_read_register(rs485->port, rs485->id, DO_DEFAULT_SENSOR_ID_REG, 1, buffer, RS485_DRIVER_copy_buffer, NULL, NULL);
    RS485_CHECK_RC(rc)
    if (read_uint16_t_BE(&buffer[0]) != RS485_DRIVER_SENSOR_ID_DO)
    {
        return RS485_ERROR;
    }
    return _check_sensor_compatible(rs485, sd);
}

// 读DO盐度补偿值 
static int _read_compensation(rs485_sensor_s_t *rs485, uint8_t indicatorID, float *value)
{
    if (indicatorID != WATER_INDICATOR_INDEX_SALINITY)
    {
        return RS485_ERROR;
    }
    uint8_t buffer[2] = {0};
    int rc = RS485_read_register(rs485->port, rs485->id, DO_DEFAULT_COMPENSATION_REG, 1, buffer, RS485_DRIVER_copy_buffer, NULL, NULL);
    *value = read_uint16_t_BE(&buffer[0]) / 10.0;
    return RS485_OK;
}

// 写DO盐度补偿值 
static int _write_compensation(rs485_sensor_s_t *rs485, uint8_t indicatorID, float value)
{
    if (indicatorID != WATER_INDICATOR_INDEX_SALINITY)
    {
        return RS485_ERROR;
    }
    if (value > 40)
    {
        value = 40;
    }
    uint16_t f = (uint16_t)(value * 10);

    return RS485_write_register(rs485->port, rs485->id, DO_DEFAULT_COMPENSATION_REG, f, NULL, NULL, NULL, NULL);
}

// 写DO校准 
static int _write_calibrate(rs485_sensor_pt rs485, uint8_t type, void *params)
{
    if (type != RS485_SENSOR_DO_CALIBRATE_SATURATED)
    {
        return RS485_PARAM_ERROR;
    }
    return RS485_write_register(rs485->port, rs485->id, DO_DEFAULT_CALIBRATE_REG, 0x02, NULL, NULL, NULL, NULL);
}

// 解析数值 
static int _parse_value(uint8_t buffer[RS485_BUFFER_MAX_SIZE], size_t len, void *ret, void *args)
{
    rs485_sensor_indicator_pt indicator = (rs485_sensor_indicator_pt)ret;
    indicator->value1 = read_uint16_t_BE(&buffer[0]) / 10.0;
    indicator->value2 = read_uint16_t_BE(&buffer[8]) / 100.0;
    indicator->value3 = read_uint16_t_BE(&buffer[2]) / 10.0;
    indicator->error_code = 0;
    indicator->status = RS485_OK;
    return RS485_OK;
}

static int _read_value1(rs485_sensor_pt rs485, rs485_sensor_indicator_pt indicator)
{
    int rc = RS485_read_register(rs485->port, rs485->id, DO_DEFAULT_VALUE_REG, 5, indicator, _parse_value, _parse_value_error, NULL);
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
        if((model == _model_match[i]) && (theory == _theory_match[i]))
        {
            _init_sensor_driver(&sensor_driver, i);
            strcpy(_PN, _pn_name[i]);
            return 0;
        }
    }
    return -1;
}

int RS485_DRIVER_zyiot_do_default_init(void)
{
    int rc = 0;
    _action.PN = _PN;
    _init_sensor_driver(&sensor_driver, 0);
    rc = RS485_DRIVER_regsiter(&sensor_driver);
    RS485_CHECK_RC(rc)

#ifdef ENABLE_DO_SENSOR_ADDR_17
    static rs485_sensor_driver_t sd_17 = {.manufacturer = RS485_DRIVER_DO_MANUFACTURER_ZYIOT, .model = RS485_SENSOR_MODEL_ID_DO_DEFAULT, .protocol = 0, .id = 17, .action = &_action};
    rc = RS485_DRIVER_regsiter(&sd_17);
    RS485_CHECK_RC(rc)
#endif
    return rc;
}
