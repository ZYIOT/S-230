#include "drivers/sensors/anbuleila/wl991.h"

static int _read_value1(rs485_sensor_pt rs485, rs485_sensor_indicator_pt indicator);
static int _read_info(rs485_sensor_pt rs485);
static int _check_sensor_compatible(rs485_sensor_pt rs485, rs485_sensor_driver_pt sd);
static int _check_sensor_wl9a0(rs485_sensor_pt rs485, rs485_sensor_driver_pt sd);
static int _write_info(rs485_sensor_s_t *rs485, void *params);
static int _match_pn_name(rs485_sensor_pt rs485, void *params);
static int _match_model(rs485_sensor_pt rs485, uint8_t model, uint8_t theory);

static rs485_sensor_action_t _action = {
    .recognition_mode = RECOGNITION_MODE_AUTO,
    .check_sensor = _check_sensor_wl9a0,
    .read_value1 = _read_value1,
    .read_info = _read_info,
    // .write_info = _write_info,
    .match_pn = _match_pn_name,
    .match_model = _match_model,
};


// *** start 增加同驱动传感器时，增加以下部分内容
#define MAX_MATCH_TYPE_COUNT  (1)
// 此处传感器的 PN 命名，需要讨论下
static char _pn_match_name[MAX_MATCH_TYPE_COUNT][RS485_SENSOR_MAX_PN_LEN] = {"WL-99"};
const static char _manufacturer_match[MAX_MATCH_TYPE_COUNT] = {RS485_DRIVER_LEVEL_MANUFACTURER_ANBULEILA};
const static char _model_match[MAX_MATCH_TYPE_COUNT] = {RS485_SENSOR_MODEL_ID_WL991};
const static char _pn_name[MAX_MATCH_TYPE_COUNT][RS485_SENSOR_MAX_PN_LEN] = {"WL-991"};
// *** end

#define SENSOR_ID   (RS485_DRIVER_SENSOR_ID_LEVEL)
static rs485_sensor_driver_t sensor_driver;
const static char _PN_default[RS485_SENSOR_MAX_PN_LEN] = {"WL-991"};
static char _PN[RS485_SENSOR_MAX_PN_LEN];
// 对于无version和SN的传感器，设置为如下值
const static uint8_t _hardware[RS485_SENSOR_MAX_HW_LEN] = {0, 0, 0};
const static uint8_t _firmware[RS485_SENSOR_MAX_FW_LEN] = {0, 0, 0};
const static uint8_t _SN[RS485_SENSOR_MAX_SN_LEN] = {0x0F, 0x97, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00};


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

static uint32_t baud = 9600;
static uint8_t addr = 0x7;
static uint16_t zero_ref = 0;       // 参考零点
static float measure_max = 8.00;    // 测量的最大值

static int _parse_baud(uint8_t buffer[RS485_BUFFER_MAX_SIZE], size_t len, void *ret, void *args)
{
    uint8_t buad_type = 0;
    buad_type = buffer[0];
    if(0 == buad_type)
    {
        baud = 2400;
    }
    else if(1 == buad_type)
    {
        baud = 4800;
    }
    else if(2 == buad_type)
    {
        baud = 9600;
    }
    else if(3 == buad_type)
    {
        baud = 19200;
    }
    return RS485_OK;
}

static int _parse_addr(uint8_t buffer[RS485_BUFFER_MAX_SIZE], size_t len, void *ret, void *args)
{
    addr = buffer[1];
    return RS485_OK;
}

static int _parse_ref(uint8_t buffer[RS485_BUFFER_MAX_SIZE], size_t len, void *ret, void *args)
{
    zero_ref = read_uint16_t_BE(buffer);
    return RS485_OK;
}

static int _parse_mes_max(uint8_t buffer[RS485_BUFFER_MAX_SIZE], size_t len, void *ret, void *args)
{
    uint16_t data = read_uint16_t_BE(buffer);
    measure_max = (float)data / 100.00;
    return RS485_OK;
}

static int _parse_value(uint8_t buffer[RS485_BUFFER_MAX_SIZE], size_t len, void *ret, void *args)
{
    rs485_sensor_indicator_pt indicator = (rs485_sensor_indicator_pt)ret;
    uint16_t value = read_int16_t_BE(&buffer[0]);
    // 设备的单位固定为 mm ，无法更改
    if(value & 0x8000)  // 负数
    {
        indicator->status = RS485_ERROR;
    }
    else                // 正数
    {
        indicator->value1 = (value & 0x7FFF)/1000.0;
        indicator->status = RS485_OK;
    }
    indicator->error_code = 0;
    return RS485_OK;
}

static int _check_valid_indicator(rs485_sensor_indicator_pt indicator)
{
    if (indicator->status != RS485_OK ||
        indicator->value1 > measure_max || 
        indicator->value1 < 0)
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

static int _check_sensor_wl9a0(rs485_sensor_pt rs485, rs485_sensor_driver_pt sd)
{
    uint8_t buffer[2] = {0};
    int rc = RS485_read_register(rs485->port, rs485->id, WL991_DEV_REG, 1, buffer, RS485_DRIVER_copy_buffer, NULL, NULL);
    RS485_CHECK_RC(rc)
    if (buffer[1] != RS485_DRIVER_SENSOR_ID_LEVEL)
    {
        return RS485_ERROR;
    }
    // rc = RS485_read_register(rs485->port, rs485->id, WL991_MES_MAX_REG, 1, buffer, _parse_mes_max, NULL, NULL);
    // RS485_CHECK_RC(rc)
    return _check_sensor_compatible(rs485, sd);
}

static int _read_value1(rs485_sensor_pt rs485, rs485_sensor_indicator_pt indicator)
{
    int rc = RS485_read_register(rs485->port, rs485->id, WL991_DISTANCE_REG, 1, indicator, _parse_value, _parse_value_error, NULL);
    if (rc != RS485_OK)
    {
        indicator->status = rc;
        return rc;
    }
    return RS485_OK;
}

static int _read_info(rs485_sensor_pt rs485)
{
    // RS485_read_register(rs485->port, rs485->id, WL991_BAUD_REG, 1, rs485, _parse_baud, NULL, NULL);
    // RS485_read_register(rs485->port, rs485->id, WL991_DEV_REG, 1, rs485, _parse_addr, NULL, NULL);
    // RS485_read_register(rs485->port, rs485->id, WL991_REF_REG, 1, rs485, _parse_ref, NULL, NULL);
    // RS485_read_register(rs485->port, rs485->id, WL991_MES_MAX_REG, 1, rs485, _parse_mes_max, NULL, NULL);
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


// 设置参考零点的值
static int _write_info(rs485_sensor_s_t *rs485, void *params)
{
    uint16_t value = *(uint16_t *)params;
    if (value > 5000)
    {
        value = 5000;
    }
    else if(value < 50)
    {
        value = 50;
    }
    return RS485_write_register(rs485->port, rs485->id, WL991_REF_REG, value, NULL, NULL, NULL, NULL);
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

int RS485_DRIVER_level_anbuleila_init(void)
{
    int rc = 0;
    _action.PN = _PN;
    _init_sensor_driver(&sensor_driver, 0);
    rc = RS485_DRIVER_regsiter(&sensor_driver);    
    RS485_CHECK_RC(rc)
    return rc;
}
