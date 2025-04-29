#include "drivers/sensors/probest/doy121.h"

static int _write_calibrate(rs485_sensor_t *rs485, uint8_t type, void *params);
static int _read_value1(rs485_sensor_pt rs485, rs485_sensor_indicator_pt indicator);
static int _read_info(rs485_sensor_pt rs485);
static int _read_compensation(rs485_sensor_s_t *rs485, uint8_t indicatorID, float *value);
static int _write_compensation(rs485_sensor_s_t *rs485, uint8_t indicatorID, float value);
static int _check_sensor_compatible(rs485_sensor_pt rs485, rs485_sensor_driver_pt sd);
static int _check_sensor_doy121(rs485_sensor_pt rs485, rs485_sensor_driver_pt sd);
static int _match_pn_name(rs485_sensor_pt rs485, void *params);
static int _match_model(rs485_sensor_pt rs485, uint8_t model, uint8_t theory);

static rs485_sensor_action_t _action = {
    .recognition_mode = RECOGNITION_MODE_AUTO,
    .check_sensor = _check_sensor_doy121,
    .read_value1 = _read_value1,
    .read_info = _read_info,
    .read_compensation = _read_compensation,
    .write_compensation = _write_compensation,
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
static char _pn_match_name[MAX_MATCH_TYPE_COUNT][RS485_SENSOR_MAX_PN_LEN] = {"DO-Y12", "DO-Y12"};
const static char _manufacturer_match[MAX_MATCH_TYPE_COUNT] = {RS485_DRIVER_DO_MANUFACTURER_PROBEST, RS485_DRIVER_DO_MANUFACTURER_PROBEST};
const static char _model_match[MAX_MATCH_TYPE_COUNT] = {RS485_SENSOR_MODEL_ID_DOY121, RS485_SENSOR_MODEL_ID_DOY123};
const static char _pn_name[MAX_MATCH_TYPE_COUNT][RS485_SENSOR_MAX_PN_LEN] = {"DO-Y121", "DO-Y123"};
// *** end

#define SENSOR_ID   (RS485_DRIVER_SENSOR_ID_DO)
static rs485_sensor_driver_t sensor_driver;
const static char _PN_default[9] = {"DO-Y121"};
static char _PN[9];
// 对于无version和SN的传感器，设置为如下值
const static uint8_t _hardware[RS485_SENSOR_MAX_HW_LEN] = {0, 0, 0};
const static uint8_t _firmware[RS485_SENSOR_MAX_FW_LEN] = {0, 0, 0};
const static uint8_t _SN[RS485_SENSOR_MAX_SN_LEN] = {0x0C, 0x31, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00};

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

static int _parse_value_error(uint8_t error_code, void *ret, void *args)
{
    rs485_sensor_indicator_pt indicator = (rs485_sensor_indicator_pt)ret;
    indicator->error_code = error_code;
    indicator->status = RS485_OK;
    return RS485_OK;
}

static int _check_valid_indicator(rs485_sensor_indicator_pt indicator)
{
    if (
        indicator->status != RS485_OK ||
        indicator->value1 > 65 ||
        indicator->value2 > 21.05f ||
        indicator->value3 > 200 ||
        indicator->value1 < -100 ||
        indicator->value2 < 0.01f ||
        indicator->value3 < 0.01f)
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

static int _check_sensor_doy121(rs485_sensor_pt rs485, rs485_sensor_driver_pt sd)
{
    uint8_t buffer[2] = {0};
    int rc = RS485_read_register(rs485->port, rs485->id, DOY121_BAUDRATE_REG_RO, 1, buffer, RS485_DRIVER_copy_buffer, NULL, NULL);
    RS485_CHECK_RC(rc)
    if (read_uint16_t_BE(&buffer[0]) != 9600 && read_uint16_t_BE(&buffer[0]) != 1)
    {
        return RS485_ERROR;
    }
    rc = RS485_read_register(rs485->port, rs485->id, DOY121_SENSOR_ID_REG, 1, buffer, RS485_DRIVER_copy_buffer, NULL, NULL);
    RS485_CHECK_RC(rc)
#ifdef ENABLE_DO_SENSOR_ADDR_17
    if (read_uint16_t_BE(&buffer[0]) != RS485_DRIVER_SENSOR_ID_DO && read_uint16_t_BE(&buffer[0]) != 17)
#else
    if (read_uint16_t_BE(&buffer[0]) != RS485_DRIVER_SENSOR_ID_DO)
#endif
    {
        return RS485_ERROR;
    }
    return _check_sensor_compatible(rs485, sd);
}

static int _read_compensation(rs485_sensor_s_t *rs485, uint8_t indicatorID, float *value)
{
    if (indicatorID != WATER_INDICATOR_INDEX_SALINITY)
    {
        return RS485_ERROR;
    }
    uint8_t buffer[2] = {0};
    int rc = RS485_read_register(rs485->port, rs485->id, DOY121_COMPENSATION_REG, 1, buffer, RS485_DRIVER_copy_buffer, NULL, NULL);
    *value = read_uint16_t_BE(&buffer[0]) / 10.0;
    return RS485_OK;
}

static int _write_compensation(rs485_sensor_s_t *rs485, uint8_t indicatorID, float value)
{
    if (indicatorID != WATER_INDICATOR_INDEX_SALINITY)
    {
        return RS485_ERROR;
    }
    if (value > 50)
    {
        value = 50;
    }
    uint16_t f = (uint16_t)(value * 10);
    return RS485_write_register(rs485->port, rs485->id, DOY121_COMPENSATION_REG, f, NULL, NULL, NULL, NULL);
}
static int calibrate_confirm_tick = 0;
static int _write_calibrate(rs485_sensor_pt rs485, uint8_t type, void *params)
{
    if (type != RS485_SENSOR_DO_CALIBRATE_SATURATED)
    {
        return RS485_PARAM_ERROR;
    }
    int rc = RS485_write_register(rs485->port, rs485->id, DOY121_CALIBRATE_REG, 0x10, NULL, NULL, NULL, NULL); // 校准第一步
    RS485_CHECK_RC(rc)
    calibrate_confirm_tick = 10;
    return RS485_OK;
}

static int _parse_value(uint8_t buffer[RS485_BUFFER_MAX_SIZE], size_t len, void *ret, void *args)
{
    rs485_sensor_indicator_pt indicator = (rs485_sensor_indicator_pt)ret;
    indicator->value1 = read_int16_t_BE(&buffer[0]) / 10.0;
    indicator->value2 = read_uint16_t_BE(&buffer[8]) / 100.0;
    indicator->value3 = read_uint16_t_BE(&buffer[2]);
    indicator->error_code = 0;
    indicator->status = RS485_OK;
    return RS485_OK;
}

static int _read_value1(rs485_sensor_pt rs485, rs485_sensor_indicator_pt indicator)
{
    if (calibrate_confirm_tick > 0)
    {
        calibrate_confirm_tick--;
        if (calibrate_confirm_tick == 0)
        {
            RS485_write_register(rs485->port, rs485->id, DOY121_CALIBRATE_REG, 0x13, NULL, NULL, NULL, NULL); // 校准第二步
        }
    }

    int rc = RS485_read_register(rs485->port, rs485->id, DOY121_VALUE_REG, 5, indicator, _parse_value, _parse_value_error, NULL);
    if (rc != RS485_OK)
    {
        indicator->status = rc;
        return rc;
    }
    return RS485_OK;
}


static int _read_info(rs485_sensor_pt rs485)
{
    uint8_t cnt = 0;
    cnt = RS485_SENSOR_MAX_HW_LEN;
    for (int i = 0; i < cnt; i++)
    {
        rs485->hardware[i] = _hardware[i];
    }
    cnt = RS485_SENSOR_MAX_FW_LEN;
    for (int i = 0; i < cnt; i++)
    {
        rs485->firmware[i] = _firmware[i];
    }
    cnt = RS485_SENSOR_MAX_SN_LEN;
    for (int i = 0; i < cnt; i++)
    {
        rs485->SN[i] = _SN[i];
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

int RS485_DRIVER_probest_doy121_init(void)
{
    int rc = 0;
    _action.PN = _PN;
    _init_sensor_driver(&sensor_driver, 0);
    rc = RS485_DRIVER_regsiter(&sensor_driver);
    RS485_CHECK_RC(rc)
    
#ifdef ENABLE_DO_SENSOR_ADDR_17
    static rs485_sensor_driver_t sd_17 = {.manufacturer = RS485_DRIVER_DO_MANUFACTURER_PROBEST, .model = RS485_SENSOR_MODEL_ID_DOY121, .protocol = 0, .id = 17, .action = &_action};
    rc = RS485_DRIVER_regsiter(&sd_17);
    RS485_CHECK_RC(rc)
#endif

#ifdef RS485_ENABLE_COMPATIBLE_MODE
    _action_compatible.pn_match_name = _pn_match_name;
    _action_compatible.PN = _PN_default;
    static rs485_sensor_driver_t sd_compatible = {.manufacturer = RS485_DRIVER_DO_MANUFACTURER_PROBEST, .model = RS485_SENSOR_MODEL_ID_DOY121, .protocol = 0, .id = RS485_DRIVER_SENSOR_ID_DO, .action = &_action_compatible};
    rc = RS485_DRIVER_regsiter(&sd_compatible);
#endif
    return rc;
}
