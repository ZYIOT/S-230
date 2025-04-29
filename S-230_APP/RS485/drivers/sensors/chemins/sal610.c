#include "drivers/sensors/chemins/sal610.h"

static int _read_info(rs485_sensor_pt rs485);
static int _read_value1(rs485_sensor_pt rs485, rs485_sensor_indicator_pt indicator);
static int _check_sensor_compatible(rs485_sensor_pt rs485, rs485_sensor_driver_pt sd);
static int _check_sensor_salinity(rs485_sensor_pt rs485, rs485_sensor_driver_pt sd);
static int _match_pn_name(rs485_sensor_pt rs485, void *params);
static int _match_model(rs485_sensor_pt rs485, uint8_t model, uint8_t theory);



static rs485_sensor_action_t _action = {
    .recognition_mode = RECOGNITION_MODE_AUTO,
    .check_sensor = _check_sensor_salinity,
    .read_value1 = _read_value1,
    .read_info = _read_info,
    .match_pn = _match_pn_name,
    .match_model = _match_model,
};

// *** start 增加同驱动传感器时，增加以下部分内容
#define MAX_MATCH_TYPE_COUNT  (1)
// 此处传感器的 PN 命名，需要讨论下
static char _pn_match_name[MAX_MATCH_TYPE_COUNT][RS485_SENSOR_MAX_PN_LEN] = {"SAL-61"};
const static char _manufacturer_match[MAX_MATCH_TYPE_COUNT] = {RS485_DRIVER_SALINITY_MANUFACTURER_CHEMINS};
const static char _model_match[MAX_MATCH_TYPE_COUNT] = {RS485_SENSOR_MODEL_ID_SAL610};
const static char _pn_name[MAX_MATCH_TYPE_COUNT][RS485_SENSOR_MAX_PN_LEN] = {"SAL-610"};
// *** end



#define SENSOR_ID   (RS485_DRIVER_SENSOR_ID_SALINITY)
static rs485_sensor_driver_t sensor_driver;
const static char _PN_default[RS485_SENSOR_MAX_PN_LEN] = {"SAL-610"};
static char _PN[RS485_SENSOR_MAX_PN_LEN];
// 对于无version和SN的传感器，设置为如下值
const static uint8_t _hardware[RS485_SENSOR_MAX_HW_LEN] = {0, 0, 0};
const static uint8_t _firmware[RS485_SENSOR_MAX_FW_LEN] = {0, 0, 0};
const static uint8_t _SN[RS485_SENSOR_MAX_SN_LEN] = {(SENSOR_SN_CODE_SAL610>>8), SENSOR_SN_CODE_SAL610, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00};


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



static int _parse_value(uint8_t buffer[RS485_BUFFER_MAX_SIZE], size_t len, void *ret, void *args)
{
    rs485_sensor_indicator_pt indicator = (rs485_sensor_indicator_pt)ret;
    uint16_t value = read_int16_t_BE(&buffer[0]);
    uint16_t dotcnt = read_int16_t_BE(&buffer[2]);

    if(1 != dotcnt) // 默认1位小数 
    {
        indicator->status = RS485_ERROR;
    }
    else
    {
        indicator->value1 = value / 10.0;
        indicator->status = RS485_OK;
    }

    indicator->error_code = 0;
    return RS485_OK;
}

static int _check_valid_indicator(rs485_sensor_indicator_pt indicator)
{
    if ((indicator->status != RS485_OK) 
    || (indicator->value1 < DEFAULT_MIN_VALUE) 
    || (indicator->value1 > DEFAULT_MAX_VALUE))
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
    return RS485_OK;
}

static int _check_sensor_salinity(rs485_sensor_pt rs485, rs485_sensor_driver_pt sd)
{
#ifndef SENSOR_SALINITY_TEST_DEBUG
    uint8_t buffer[2] = {0};
    int rc = RS485_read_register(rs485->port, rs485->id, SALINITY_ADDR_REG, SALINITY_ADDR_REG_LEN, buffer, RS485_DRIVER_copy_buffer, NULL, NULL);
    RS485_CHECK_RC(rc)
    if (buffer[1] != DEFAULT_ADDR)
    {
        return RS485_ERROR;
    }
#else
#endif
    return _check_sensor_compatible(rs485, sd);
}


static int _read_value1(rs485_sensor_pt rs485, rs485_sensor_indicator_pt indicator)
{
#ifndef SENSOR_SALINITY_TEST_DEBUG
    int rc = RS485_read_register(rs485->port, rs485->id, SALINITY_VALUE_REG, SALINITY_VALUE_REG_LEN, indicator, _parse_value, _parse_value_error, NULL);
    if (rc != RS485_OK)
    {
        indicator->status = rc;
        return rc;
    }
#else
    
    indicator->value1 = 456 / 10.0;
    indicator->vm1 = 999;
    indicator->status = RS485_OK;
    indicator->error_code = 0;
#endif    
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


int RS485_DRIVER_chemins_sal610_init(void)
{
    int rc = 0;
    _action.PN = _PN;
    _init_sensor_driver(&sensor_driver, 0);
    rc = RS485_DRIVER_regsiter(&sensor_driver);    
    RS485_CHECK_RC(rc)
    return rc;
}

