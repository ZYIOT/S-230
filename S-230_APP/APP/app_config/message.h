#ifndef __APP_CONFIG_EEPROM_MESSAGE_H
#define __APP_CONFIG_EEPROM_MESSAGE_H
#ifdef __cplusplus
extern "C"
{
#endif

int APP_CONFIG_system_init(app_config_system_pt message)
{
    message->device_id = 1;
    memset(message->SN, 0, 5);
    memset(message->hardware, 0, 3);
    message->hardware[0]= APP_HARDWARE_MAJOR;
    message->hardware[1]= APP_HARDWARE_MINOR;
    message->hardware[2]= APP_HARDWARE_REVISION;
    message->log_level = BSP_LOG_DEFAULT_LEVEL; 
    return APP_OK;
}

static void _system_serialize(uint8_t bytes[EEPROM_BLOCK_SIZE], app_config_system_pt message)
{
    write_uint32_t(message->device_id, &bytes[APP_CONFIG_EEPROM_SYSTEM_DEVICE_ID_OFFSET]); 
    memcpy(&bytes[APP_CONFIG_EEPROM_SYSTEM_SN_OFFSET], message->SN, 5); 
    memcpy(&bytes[APP_CONFIG_EEPROM_SYSTEM_HARDWARE_OFFSET], message->hardware, 3); 
}

app_config_system_t app_config_system;
int APP_CONFIG_system_read( app_config_system_pt message)
{
    uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};
    
    int rc = epprom_read_block(APP_CONFIG_EEPROM_SYSTEM_PAGE, bytes);
    APP_CHECK_RC(rc)
    message->device_id = read_uint32_t(&bytes[APP_CONFIG_EEPROM_SYSTEM_DEVICE_ID_OFFSET]);
    memcpy(message->SN, &bytes[APP_CONFIG_EEPROM_SYSTEM_SN_OFFSET], 5);
    memcpy(message->hardware, &bytes[APP_CONFIG_EEPROM_SYSTEM_HARDWARE_OFFSET], 3);
    return APP_OK;
}

int APP_CONFIG_system_write( app_config_system_pt message)
{
    uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};
    
    _system_serialize(bytes, message);
    return epprom_write_block(APP_CONFIG_EEPROM_SYSTEM_PAGE, bytes);
}

int APP_CONFIG_system_write_device_id( app_config_system_pt message, uint32_t device_id)
{
    uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};
    
    _system_serialize(bytes, message);
    write_uint32_t(device_id, &bytes[APP_CONFIG_EEPROM_SYSTEM_DEVICE_ID_OFFSET]);
    int rc = epprom_write_block(APP_CONFIG_EEPROM_SYSTEM_PAGE, bytes);
    APP_CHECK_RC(rc)
    message->device_id = device_id; 
    return APP_OK;
}

int APP_CONFIG_system_write_sn( app_config_system_pt message, uint8_t SN[5], uint8_t hardware[3])
{
    uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};
    
    _system_serialize(bytes, message);
    memcpy(&bytes[APP_CONFIG_EEPROM_SYSTEM_SN_OFFSET], SN, 5);
    memcpy(&bytes[APP_CONFIG_EEPROM_SYSTEM_HARDWARE_OFFSET], hardware, 3);
    int rc = epprom_write_block(APP_CONFIG_EEPROM_SYSTEM_PAGE, bytes);
    APP_CHECK_RC(rc)
    memcpy(message->SN, SN, 5); 
    memcpy(message->hardware, hardware, 3); 
    return APP_OK;
}

int APP_CONFIG_system_load(void)
{
    int rc = 0;

    APP_CONFIG_system_init(&app_config_system);
    rc = APP_CONFIG_system_read( &app_config_system);
    APP_CHECK_RC(rc)
    return APP_OK;
}

int APP_CONFIG_system_recovery(void)
{
    int rc = 0;
    
    APP_CONFIG_system_init(&app_config_system);
    rc = APP_CONFIG_system_write( &app_config_system);
    APP_CHECK_RC(rc)
    return APP_OK;
}
int APP_CONFIG_time_init(app_config_time_pt message)
{
    message->year = 0;
    message->month = 0;
    message->day = 0;
    message->hour = 0;
    message->minute = 0;
    message->second = 0;
    return APP_OK;
}

static void _time_serialize(uint8_t bytes[EEPROM_BLOCK_SIZE], app_config_time_pt message)
{
    write_uint8_t(message->year, &bytes[APP_CONFIG_EEPROM_TIME_YEAR_OFFSET]); 
    write_uint8_t(message->month, &bytes[APP_CONFIG_EEPROM_TIME_MONTH_OFFSET]); 
    write_uint8_t(message->day, &bytes[APP_CONFIG_EEPROM_TIME_DAY_OFFSET]); 
    write_uint8_t(message->hour, &bytes[APP_CONFIG_EEPROM_TIME_HOUR_OFFSET]); 
    write_uint8_t(message->minute, &bytes[APP_CONFIG_EEPROM_TIME_MINUTE_OFFSET]); 
    write_uint8_t(message->second, &bytes[APP_CONFIG_EEPROM_TIME_SECOND_OFFSET]); 
}

static int _time_deserialize(uint8_t *bytes, app_config_time_pt message)
{
    message->year = read_uint8_t(&bytes[APP_CONFIG_EEPROM_TIME_YEAR_OFFSET]); 
    message->month = read_uint8_t(&bytes[APP_CONFIG_EEPROM_TIME_MONTH_OFFSET]); 
    message->day = read_uint8_t(&bytes[APP_CONFIG_EEPROM_TIME_DAY_OFFSET]); 
    message->hour = read_uint8_t(&bytes[APP_CONFIG_EEPROM_TIME_HOUR_OFFSET]); 
    message->minute = read_uint8_t(&bytes[APP_CONFIG_EEPROM_TIME_MINUTE_OFFSET]); 
    message->second = read_uint8_t(&bytes[APP_CONFIG_EEPROM_TIME_SECOND_OFFSET]); 
    return APP_OK;
}

static void _time_copy( app_config_time_pt dist, app_config_time_pt from)
{
    dist->year = from->year; 
    dist->month = from->month; 
    dist->day = from->day; 
    dist->hour = from->hour; 
    dist->minute = from->minute; 
    dist->second = from->second; 
}

int APP_CONFIG_device_enable_init(app_config_device_enable_pt message)
{
    message->enable = APP_ENABLE;
    APP_CONFIG_time_init(&message->record_error_at);
    return APP_OK;
}

static void _device_enable_serialize(uint8_t bytes[EEPROM_BLOCK_SIZE], app_config_device_enable_pt message)
{
    write_uint8_t(message->enable, &bytes[APP_CONFIG_EEPROM_DEVICE_ENABLE_ENABLE_OFFSET]); 
    _time_serialize(&bytes[APP_CONFIG_EEPROM_DEVICE_ENABLE_RECORD_ERROR_AT_OFFSET], &message->record_error_at); 
}

app_config_device_enable_t app_config_device_enable;
int APP_CONFIG_device_enable_read( app_config_device_enable_pt message)
{
    uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};
    
    int rc = epprom_read_block(APP_CONFIG_EEPROM_DEVICE_ENABLE_PAGE, bytes);
    APP_CHECK_RC(rc)
    message->enable = read_uint8_t(&bytes[APP_CONFIG_EEPROM_DEVICE_ENABLE_ENABLE_OFFSET]);
    _time_deserialize(&bytes[APP_CONFIG_EEPROM_DEVICE_ENABLE_RECORD_ERROR_AT_OFFSET], &message->record_error_at);
    return APP_OK;
}

int APP_CONFIG_device_enable_write( app_config_device_enable_pt message)
{
    uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};
    
    _device_enable_serialize(bytes, message);
    return epprom_write_block(APP_CONFIG_EEPROM_DEVICE_ENABLE_PAGE, bytes);
}

int APP_CONFIG_device_enable_write_enable( app_config_device_enable_pt message, uint8_t enable)
{
    uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};
    
    _device_enable_serialize(bytes, message);
    write_uint8_t(enable, &bytes[APP_CONFIG_EEPROM_DEVICE_ENABLE_ENABLE_OFFSET]);
    int rc = epprom_write_block(APP_CONFIG_EEPROM_DEVICE_ENABLE_PAGE, bytes);
    APP_CHECK_RC(rc)
    message->enable = enable; 
    return APP_OK;
}

int APP_CONFIG_device_enable_write_record_error_at( app_config_device_enable_pt message, app_config_time_t record_error_at)
{
    uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};
    
    _device_enable_serialize(bytes, message);
    _time_serialize(&bytes[APP_CONFIG_EEPROM_DEVICE_ENABLE_RECORD_ERROR_AT_OFFSET], &record_error_at);
    int rc = epprom_write_block(APP_CONFIG_EEPROM_DEVICE_ENABLE_PAGE, bytes);
    APP_CHECK_RC(rc)
    _time_copy(&message->record_error_at, &record_error_at); 
    return APP_OK;
}

int APP_CONFIG_device_enable_load(void)
{
    int rc = 0;

    APP_CONFIG_device_enable_init(&app_config_device_enable);
    rc = APP_CONFIG_device_enable_read( &app_config_device_enable);
    APP_CHECK_RC(rc)
    return APP_OK;
}

int APP_CONFIG_device_enable_recovery(void)
{
    int rc = 0;

    APP_CONFIG_device_enable_init(&app_config_device_enable);
    rc = APP_CONFIG_device_enable_write( &app_config_device_enable);
    APP_CHECK_RC(rc)
    return APP_OK;
}
int APP_CONFIG_device_connect_init(app_config_device_connect_pt message)
{
    message->reason = 0;
    return APP_OK;
}

static void _device_connect_serialize(uint8_t bytes[EEPROM_BLOCK_SIZE], app_config_device_connect_pt message)
{
    write_uint8_t(message->reason, &bytes[APP_CONFIG_EEPROM_DEVICE_CONNECT_REASON_OFFSET]); 
}

app_config_device_connect_t app_config_device_connect;
int APP_CONFIG_device_connect_read( app_config_device_connect_pt message)
{
    uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};
    
    int rc = epprom_read_block(APP_CONFIG_EEPROM_DEVICE_CONNECT_PAGE, bytes);
    APP_CHECK_RC(rc)
    message->reason = read_uint8_t(&bytes[APP_CONFIG_EEPROM_DEVICE_CONNECT_REASON_OFFSET]);
    return APP_OK;
}

int APP_CONFIG_device_connect_write( app_config_device_connect_pt message)
{
    uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};
    
    _device_connect_serialize(bytes, message);
    return epprom_write_block(APP_CONFIG_EEPROM_DEVICE_CONNECT_PAGE, bytes);
}

int APP_CONFIG_device_connect_write_reason( app_config_device_connect_pt message, uint8_t reason)
{
    uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};
    
    _device_connect_serialize(bytes, message);
    write_uint8_t(reason, &bytes[APP_CONFIG_EEPROM_DEVICE_CONNECT_REASON_OFFSET]);
    int rc = epprom_write_block(APP_CONFIG_EEPROM_DEVICE_CONNECT_PAGE, bytes);
    APP_CHECK_RC(rc)
    message->reason = reason; 
    return APP_OK;
}

int APP_CONFIG_device_connect_load(void)
{
    int rc = 0;

    APP_CONFIG_device_connect_init(&app_config_device_connect);
    rc = APP_CONFIG_device_connect_read( &app_config_device_connect);
    APP_CHECK_RC(rc)
    return APP_OK;
}

int APP_CONFIG_device_connect_recovery(void)
{
    int rc = 0;

    APP_CONFIG_device_connect_init(&app_config_device_connect);
    rc = APP_CONFIG_device_connect_write( &app_config_device_connect);
    APP_CHECK_RC(rc)
    return APP_OK;
}
int APP_CONFIG_probe_enable_init(app_config_probe_enable_pt message)
{
    message->has_config = 0;
    message->enable = 0;
    message->warning = 0;
    return APP_OK;
}

static void _probe_enable_serialize(uint8_t bytes[EEPROM_BLOCK_SIZE], app_config_probe_enable_pt message)
{
    write_uint8_t(message->has_config, &bytes[APP_CONFIG_EEPROM_PROBE_ENABLE_HAS_CONFIG_OFFSET]); 
    write_uint8_t(message->enable, &bytes[APP_CONFIG_EEPROM_PROBE_ENABLE_ENABLE_OFFSET]); 
    write_uint8_t(message->warning, &bytes[APP_CONFIG_EEPROM_PROBE_ENABLE_WARNING_OFFSET]); 
}

app_config_probe_enable_t app_config_probe_enable[PROBE_SIZE];
int APP_CONFIG_probe_enable_read(uint8_t probe_id,  app_config_probe_enable_pt message)
{
    uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};

    if((probe_id > PROBE_SIZE)){return APP_ERROR;}
    int rc = epprom_read_block(APP_CONFIG_EEPROM_PROBE_ENABLE_PAGE + probe_id, bytes);
    APP_CHECK_RC(rc)
    message->has_config = read_uint8_t(&bytes[APP_CONFIG_EEPROM_PROBE_ENABLE_HAS_CONFIG_OFFSET]);
    message->enable = read_uint8_t(&bytes[APP_CONFIG_EEPROM_PROBE_ENABLE_ENABLE_OFFSET]);
    message->warning = read_uint8_t(&bytes[APP_CONFIG_EEPROM_PROBE_ENABLE_WARNING_OFFSET]);
    return APP_OK;
}

int APP_CONFIG_probe_enable_write(uint8_t probe_id,  app_config_probe_enable_pt message)
{
    uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};

    if((probe_id > PROBE_SIZE)){return APP_ERROR;}
    _probe_enable_serialize(bytes, message);
    return epprom_write_block(APP_CONFIG_EEPROM_PROBE_ENABLE_PAGE + probe_id, bytes);
}

int APP_CONFIG_probe_enable_write_has_config(uint8_t probe_id,  app_config_probe_enable_pt message, uint8_t has_config)
{
    uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};

    if((probe_id > PROBE_SIZE)){return APP_ERROR;}
    _probe_enable_serialize(bytes, message);
    write_uint8_t(has_config, &bytes[APP_CONFIG_EEPROM_PROBE_ENABLE_HAS_CONFIG_OFFSET]);
    int rc = epprom_write_block(APP_CONFIG_EEPROM_PROBE_ENABLE_PAGE + probe_id, bytes);
    APP_CHECK_RC(rc)
    message->has_config = has_config; 
    return APP_OK;
}

int APP_CONFIG_probe_enable_write_enable(uint8_t probe_id,  app_config_probe_enable_pt message, uint8_t enable)
{
    uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};

    if((probe_id > PROBE_SIZE)){return APP_ERROR;}
    _probe_enable_serialize(bytes, message);
    write_uint8_t(enable, &bytes[APP_CONFIG_EEPROM_PROBE_ENABLE_ENABLE_OFFSET]);
    int rc = epprom_write_block(APP_CONFIG_EEPROM_PROBE_ENABLE_PAGE + probe_id, bytes);
    APP_CHECK_RC(rc)
    message->enable = enable; 
    return APP_OK;
}

int APP_CONFIG_probe_enable_write_warning(uint8_t probe_id,  app_config_probe_enable_pt message, uint8_t warning)
{
    uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};

    if((probe_id > PROBE_SIZE)){return APP_ERROR;}
    _probe_enable_serialize(bytes, message);
    write_uint8_t(warning, &bytes[APP_CONFIG_EEPROM_PROBE_ENABLE_WARNING_OFFSET]);
    int rc = epprom_write_block(APP_CONFIG_EEPROM_PROBE_ENABLE_PAGE + probe_id, bytes);
    APP_CHECK_RC(rc)
    message->warning = warning; 
    return APP_OK;
}

int APP_CONFIG_probe_enable_load(void)
{
    int rc = 0;
    uint8_t probe_id;

    for(probe_id = 0; probe_id < PROBE_SIZE; probe_id++ ) 
    {
        APP_CONFIG_probe_enable_init(&app_config_probe_enable[probe_id]);
        rc = APP_CONFIG_probe_enable_read(probe_id, &app_config_probe_enable[probe_id]);
        APP_CHECK_RC(rc)
    } 
    return APP_OK;
}

int APP_CONFIG_probe_enable_recovery(void)
{
    int rc = 0;
    uint8_t probe_id;

    for(probe_id = 0; probe_id < PROBE_SIZE; probe_id++ ) 
    {
        APP_CONFIG_probe_enable_init(&app_config_probe_enable[probe_id]);
        rc = APP_CONFIG_probe_enable_write(probe_id, &app_config_probe_enable[probe_id]);
        APP_CHECK_RC(rc)
    } 
    return APP_OK;
}

int APP_CONFIG_pump_status_init(app_config_pump_status_pt message)
{
    memset(message->status, 0, 16);
    return APP_OK;
}

static void _pump_status_serialize(uint8_t bytes[EEPROM_BLOCK_SIZE], app_config_pump_status_pt message)
{
    memcpy(&bytes[APP_CONFIG_EEPROM_PUMP_STATUS_STATUS_OFFSET], message->status, 16); 
}

app_config_pump_status_t app_config_pump_status[PROBE_SIZE];
int APP_CONFIG_pump_status_read(uint8_t probe_id,  app_config_pump_status_pt message)
{
    uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};

    if((probe_id > PROBE_SIZE)){return APP_ERROR;}
    int rc = epprom_read_block(APP_CONFIG_EEPROM_PUMP_STATUS_PAGE + probe_id, bytes);
    APP_CHECK_RC(rc)
    memcpy(message->status, &bytes[APP_CONFIG_EEPROM_PUMP_STATUS_STATUS_OFFSET], 16);
    return APP_OK;
}

int APP_CONFIG_pump_status_write(uint8_t probe_id,  app_config_pump_status_pt message)
{
    uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};

    if((probe_id > PROBE_SIZE)){return APP_ERROR;}
    _pump_status_serialize(bytes, message);
    return epprom_write_block(APP_CONFIG_EEPROM_PUMP_STATUS_PAGE + probe_id, bytes);
}

int APP_CONFIG_pump_status_write_status(uint8_t probe_id,  app_config_pump_status_pt message, uint8_t status_index, uint8_t status)
{
    uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};

    if((probe_id > PROBE_SIZE)){return APP_ERROR;}
    _pump_status_serialize(bytes, message);
    write_uint8_t(status, &bytes[APP_CONFIG_EEPROM_PUMP_STATUS_STATUS_OFFSET + 1 * status_index]);
    int rc = epprom_write_block(APP_CONFIG_EEPROM_PUMP_STATUS_PAGE + probe_id, bytes);
    APP_CHECK_RC(rc)
    message->status[status_index] = status; 
    return APP_OK;
}

int APP_CONFIG_pump_status_load(void)
{
    int rc = 0;
    uint8_t probe_id;

    for(probe_id = 0; probe_id < PROBE_SIZE; probe_id++ ) 
    {
        APP_CONFIG_pump_status_init(&app_config_pump_status[probe_id]);
        rc = APP_CONFIG_pump_status_read(probe_id, &app_config_pump_status[probe_id]);
        APP_CHECK_RC(rc)
    } 
    return APP_OK;
}

int APP_CONFIG_pump_status_recovery(void)
{
    int rc = 0;
    uint8_t probe_id;

    for(probe_id = 0; probe_id < PROBE_SIZE; probe_id++ ) 
    {
        APP_CONFIG_pump_status_init(&app_config_pump_status[probe_id]);
        rc = APP_CONFIG_pump_status_write(probe_id, &app_config_pump_status[probe_id]);
        APP_CHECK_RC(rc)
    } 
    return APP_OK;
}

int APP_CONFIG_sensor_init(app_config_sensor_pt message)
{
    message->has_config = 0;
    message->enable = 0;
    message->warning = 0;
    message->manufacturer = 0;
    message->model = 0;
    message->protocol = 0;
    return APP_OK;
}

static void _sensor_serialize(uint8_t bytes[EEPROM_BLOCK_SIZE], app_config_sensor_pt message)
{
    write_uint8_t(message->has_config, &bytes[APP_CONFIG_EEPROM_SENSOR_HAS_CONFIG_OFFSET]); 
    write_uint8_t(message->enable, &bytes[APP_CONFIG_EEPROM_SENSOR_ENABLE_OFFSET]); 
    write_uint8_t(message->warning, &bytes[APP_CONFIG_EEPROM_SENSOR_WARNING_OFFSET]); 
    write_uint8_t(message->manufacturer, &bytes[APP_CONFIG_EEPROM_SENSOR_MANUFACTURER_OFFSET]); 
    write_uint8_t(message->model, &bytes[APP_CONFIG_EEPROM_SENSOR_MODEL_OFFSET]); 
    write_uint8_t(message->protocol, &bytes[APP_CONFIG_EEPROM_SENSOR_PROTOCOL_OFFSET]); 
}

app_config_sensor_t app_config_sensor[PROBE_SIZE][PROBE_SENSOR_SIZE];
int APP_CONFIG_sensor_read(uint8_t probe_id, uint8_t sensor_id,  app_config_sensor_pt message)
{
    uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};

    if((probe_id > PROBE_SIZE) || (sensor_id > PROBE_SENSOR_SIZE)){return APP_ERROR;}
    int rc = epprom_read_block(APP_CONFIG_EEPROM_SENSOR_PAGE + (probe_id * 16) + sensor_id, bytes);
    APP_CHECK_RC(rc)
    message->has_config = read_uint8_t(&bytes[APP_CONFIG_EEPROM_SENSOR_HAS_CONFIG_OFFSET]);
    message->enable = read_uint8_t(&bytes[APP_CONFIG_EEPROM_SENSOR_ENABLE_OFFSET]);
    message->warning = read_uint8_t(&bytes[APP_CONFIG_EEPROM_SENSOR_WARNING_OFFSET]);
    message->manufacturer = read_uint8_t(&bytes[APP_CONFIG_EEPROM_SENSOR_MANUFACTURER_OFFSET]);
    message->model = read_uint8_t(&bytes[APP_CONFIG_EEPROM_SENSOR_MODEL_OFFSET]);
    message->protocol = read_uint8_t(&bytes[APP_CONFIG_EEPROM_SENSOR_PROTOCOL_OFFSET]);
    return APP_OK;
}

int APP_CONFIG_sensor_write(uint8_t probe_id, uint8_t sensor_id,  app_config_sensor_pt message)
{
    uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};

    if((probe_id > PROBE_SIZE) || (sensor_id > PROBE_SENSOR_SIZE)){return APP_ERROR;}
    _sensor_serialize(bytes, message);
    return epprom_write_block(APP_CONFIG_EEPROM_SENSOR_PAGE + (probe_id * 16) + sensor_id, bytes);
}

int APP_CONFIG_sensor_load(void)
{
    int rc = 0;
    uint8_t probe_id;
    uint8_t sensor_id;

    for(probe_id = 0; probe_id < PROBE_SIZE; probe_id++ ) 
    {
        for(sensor_id = 0; sensor_id < PROBE_SENSOR_SIZE; sensor_id++ ) 
        {
            APP_CONFIG_sensor_init(&app_config_sensor[probe_id][sensor_id]);
            rc = APP_CONFIG_sensor_read(probe_id,sensor_id, &app_config_sensor[probe_id][sensor_id]);
            APP_CHECK_RC(rc)
        } 
    } 
    return APP_OK;
}

int APP_CONFIG_sensor_recovery(void)
{
    int rc = 0;
    uint8_t probe_id;
    uint8_t sensor_id;

    for(probe_id = 0; probe_id < PROBE_SIZE; probe_id++ ) 
    {
        for(sensor_id = 0; sensor_id < PROBE_SENSOR_SIZE; sensor_id++ ) 
        {
            APP_CONFIG_sensor_init(&app_config_sensor[probe_id][sensor_id]);
            rc = APP_CONFIG_sensor_write(probe_id,sensor_id, &app_config_sensor[probe_id][sensor_id]);
            APP_CHECK_RC(rc)
        } 
    } 
    return APP_OK;
}
int APP_CONFIG_compensation_init(app_config_compensation_pt message)
{
    message->has_config = 0;
    message->value = 0;
    return APP_OK;
}

static void _compensation_serialize(uint8_t bytes[EEPROM_BLOCK_SIZE], app_config_compensation_pt message)
{
    write_uint8_t(message->has_config, &bytes[APP_CONFIG_EEPROM_COMPENSATION_HAS_CONFIG_OFFSET]); 
    write_uint16_t(message->value, &bytes[APP_CONFIG_EEPROM_COMPENSATION_VALUE_OFFSET]); 
}

app_config_compensation_t app_config_compensation[PROBE_SIZE][INDICATOR_SIZE];
int APP_CONFIG_compensation_read(uint8_t probe_id, uint8_t indicator_id,  app_config_compensation_pt message)
{
    uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};

    if((probe_id > PROBE_SIZE) || (indicator_id > INDICATOR_SIZE)){return APP_ERROR;}
    int rc = epprom_read_block(APP_CONFIG_EEPROM_COMPENSATION_PAGE + (probe_id * 16) + indicator_id, bytes);
    APP_CHECK_RC(rc)
    message->has_config = read_uint8_t(&bytes[APP_CONFIG_EEPROM_COMPENSATION_HAS_CONFIG_OFFSET]);
    message->value = read_uint16_t(&bytes[APP_CONFIG_EEPROM_COMPENSATION_VALUE_OFFSET]);
    return APP_OK;
}

int APP_CONFIG_compensation_write(uint8_t probe_id, uint8_t indicator_id,  app_config_compensation_pt message)
{
    uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};

    if((probe_id > PROBE_SIZE) || (indicator_id > INDICATOR_SIZE)){return APP_ERROR;}
    _compensation_serialize(bytes, message);
    return epprom_write_block(APP_CONFIG_EEPROM_COMPENSATION_PAGE + (probe_id * 16) + indicator_id, bytes);
}

int APP_CONFIG_compensation_write_data(uint8_t probe_id, uint8_t indicator_id,  app_config_compensation_pt message, uint8_t has_config, uint16_t value)
{
    uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};

    if((probe_id > PROBE_SIZE) || (indicator_id > INDICATOR_SIZE)){return APP_ERROR;}
    _compensation_serialize(bytes, message);
    write_uint8_t(has_config, &bytes[APP_CONFIG_EEPROM_COMPENSATION_HAS_CONFIG_OFFSET]);
    write_uint16_t(value, &bytes[APP_CONFIG_EEPROM_COMPENSATION_VALUE_OFFSET]);
    int rc = epprom_write_block(APP_CONFIG_EEPROM_COMPENSATION_PAGE + (probe_id * 16) + indicator_id, bytes);
    APP_CHECK_RC(rc)
    message->has_config = has_config; 
    message->value = value; 
    return APP_OK;
}

int APP_CONFIG_compensation_load(void)
{
    int rc = 0;
    uint8_t probe_id;
    uint8_t indicator_id;

    for(probe_id = 0; probe_id < PROBE_SIZE; probe_id++ ) 
    {
        for(indicator_id = 0; indicator_id < INDICATOR_SIZE; indicator_id++ ) 
        {
            APP_CONFIG_compensation_init(&app_config_compensation[probe_id][indicator_id]);
            rc = APP_CONFIG_compensation_read(probe_id,indicator_id, &app_config_compensation[probe_id][indicator_id]);
            APP_CHECK_RC(rc)
        } 
    } 
    return APP_OK;
}

int APP_CONFIG_compensation_recovery(void)
{
    int rc = 0;
    uint8_t probe_id;
    uint8_t indicator_id;

    for(probe_id = 0; probe_id < PROBE_SIZE; probe_id++ ) 
    {
        for(indicator_id = 0; indicator_id < INDICATOR_SIZE; indicator_id++ ) 
        {
            APP_CONFIG_compensation_init(&app_config_compensation[probe_id][indicator_id]);
            rc = APP_CONFIG_compensation_write(probe_id,indicator_id, &app_config_compensation[probe_id][indicator_id]);
            APP_CHECK_RC(rc)
        } 
    } 
    return APP_OK;
}
int APP_CONFIG_indicator_alert_init(app_config_indicator_alert_pt message)
{
    message->has_config = 0;
    message->enable = 0;
    message->max = 0;
    message->min = 0;
    message->threshold = 0;
    return APP_OK;
}

static void _indicator_alert_serialize(uint8_t bytes[EEPROM_BLOCK_SIZE], app_config_indicator_alert_pt message)
{
    write_uint8_t(message->has_config, &bytes[APP_CONFIG_EEPROM_INDICATOR_ALERT_HAS_CONFIG_OFFSET]); 
    write_uint8_t(message->enable, &bytes[APP_CONFIG_EEPROM_INDICATOR_ALERT_ENABLE_OFFSET]); 
    write_uint16_t(message->max, &bytes[APP_CONFIG_EEPROM_INDICATOR_ALERT_MAX_OFFSET]); 
    write_uint16_t(message->min, &bytes[APP_CONFIG_EEPROM_INDICATOR_ALERT_MIN_OFFSET]); 
    write_uint16_t(message->threshold, &bytes[APP_CONFIG_EEPROM_INDICATOR_ALERT_THRESHOLD_OFFSET]); 
}

app_config_indicator_alert_t app_config_indicator_alert[PROBE_SIZE][INDICATOR_SIZE];
int APP_CONFIG_indicator_alert_read(uint8_t probe_id, uint8_t indicator_id,  app_config_indicator_alert_pt message)
{
    uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};

    if((probe_id > PROBE_SIZE) || (indicator_id > INDICATOR_SIZE)){return APP_ERROR;}
    int rc = epprom_read_block(APP_CONFIG_EEPROM_INDICATOR_ALERT_PAGE + (probe_id * 16) + indicator_id, bytes);
    APP_CHECK_RC(rc)
    message->has_config = read_uint8_t(&bytes[APP_CONFIG_EEPROM_INDICATOR_ALERT_HAS_CONFIG_OFFSET]);
    message->enable = read_uint8_t(&bytes[APP_CONFIG_EEPROM_INDICATOR_ALERT_ENABLE_OFFSET]);
    message->max = read_uint16_t(&bytes[APP_CONFIG_EEPROM_INDICATOR_ALERT_MAX_OFFSET]);
    message->min = read_uint16_t(&bytes[APP_CONFIG_EEPROM_INDICATOR_ALERT_MIN_OFFSET]);
    message->threshold = read_uint16_t(&bytes[APP_CONFIG_EEPROM_INDICATOR_ALERT_THRESHOLD_OFFSET]);
    return APP_OK;
}

int APP_CONFIG_indicator_alert_write(uint8_t probe_id, uint8_t indicator_id,  app_config_indicator_alert_pt message)
{
    uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};

    if((probe_id > PROBE_SIZE) || (indicator_id > INDICATOR_SIZE)){return APP_ERROR;}
    _indicator_alert_serialize(bytes, message);
    return epprom_write_block(APP_CONFIG_EEPROM_INDICATOR_ALERT_PAGE + (probe_id * 16) + indicator_id, bytes);
}

int APP_CONFIG_indicator_alert_load(void)
{
    int rc = 0;
    uint8_t probe_id;
    uint8_t indicator_id;

    for(probe_id = 0; probe_id < PROBE_SIZE; probe_id++ ) 
    {
        for(indicator_id = 0; indicator_id < INDICATOR_SIZE; indicator_id++ ) 
        {
            APP_CONFIG_indicator_alert_init(&app_config_indicator_alert[probe_id][indicator_id]);
            rc = APP_CONFIG_indicator_alert_read(probe_id,indicator_id, &app_config_indicator_alert[probe_id][indicator_id]);
            APP_CHECK_RC(rc)
        } 
    } 
    return APP_OK;
}

int APP_CONFIG_indicator_alert_recovery(void)
{
    int rc = 0;
    uint8_t probe_id;
    uint8_t indicator_id;

    for(probe_id = 0; probe_id < PROBE_SIZE; probe_id++ ) 
    {
        for(indicator_id = 0; indicator_id < INDICATOR_SIZE; indicator_id++ ) 
        {
            APP_CONFIG_indicator_alert_init(&app_config_indicator_alert[probe_id][indicator_id]);
            rc = APP_CONFIG_indicator_alert_write(probe_id,indicator_id, &app_config_indicator_alert[probe_id][indicator_id]);
            APP_CHECK_RC(rc)
        } 
    } 
    return APP_OK;
}
int APP_CONFIG_config_enable_init(app_config_config_enable_pt message)
{
    message->has_config = 0;
    message->enable = 0;
    return APP_OK;
}

static void _config_enable_serialize(uint8_t bytes[EEPROM_BLOCK_SIZE], app_config_config_enable_pt message)
{
    write_uint8_t(message->has_config, &bytes[APP_CONFIG_EEPROM_CONFIG_ENABLE_HAS_CONFIG_OFFSET]); 
    write_uint8_t(message->enable, &bytes[APP_CONFIG_EEPROM_CONFIG_ENABLE_ENABLE_OFFSET]); 
}

static int _config_enable_deserialize(uint8_t *bytes, app_config_config_enable_pt message)
{
    message->has_config = read_uint8_t(&bytes[APP_CONFIG_EEPROM_CONFIG_ENABLE_HAS_CONFIG_OFFSET]); 
    message->enable = read_uint8_t(&bytes[APP_CONFIG_EEPROM_CONFIG_ENABLE_ENABLE_OFFSET]); 
    return APP_OK;
}

static void _config_enable_copy( app_config_config_enable_pt dist, app_config_config_enable_pt from)
{
    dist->has_config = from->has_config; 
    dist->enable = from->enable; 
}

int APP_CONFIG_limit_init(app_config_limit_pt message)
{
    message->has_config = 0;
    message->enable = 0;
    message->relay_id = 0;
    message->channel = 0;
    message->type = 0;
    message->up_limit = 0;
    message->down_limit = 0;
    return APP_OK;
}

static void _limit_serialize(uint8_t bytes[EEPROM_BLOCK_SIZE], app_config_limit_pt message)
{
    write_uint8_t(message->has_config, &bytes[APP_CONFIG_EEPROM_LIMIT_HAS_CONFIG_OFFSET]); 
    write_uint8_t(message->enable, &bytes[APP_CONFIG_EEPROM_LIMIT_ENABLE_OFFSET]); 
    write_uint8_t(message->relay_id, &bytes[APP_CONFIG_EEPROM_LIMIT_RELAY_ID_OFFSET]); 
    write_uint16_t(message->channel, &bytes[APP_CONFIG_EEPROM_LIMIT_CHANNEL_OFFSET]); 
    write_uint8_t(message->type, &bytes[APP_CONFIG_EEPROM_LIMIT_TYPE_OFFSET]); 
    write_uint16_t(message->up_limit, &bytes[APP_CONFIG_EEPROM_LIMIT_UP_LIMIT_OFFSET]); 
    write_uint16_t(message->down_limit, &bytes[APP_CONFIG_EEPROM_LIMIT_DOWN_LIMIT_OFFSET]); 
}

app_config_limit_t app_config_limit[PROBE_SIZE][INDICATOR_SIZE][APP_CONFIG_MAX_LIMIT_TASK];
int APP_CONFIG_limit_read(uint8_t probe_id, uint8_t indicator_id, uint8_t task_id,  app_config_limit_pt message)
{
    uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};

    if((probe_id > PROBE_SIZE) || (indicator_id > INDICATOR_SIZE) || (task_id > APP_CONFIG_MAX_LIMIT_TASK)){return APP_ERROR;}
    int rc = epprom_read_block(APP_CONFIG_EEPROM_LIMIT_PAGE + (probe_id * 16 * APP_CONFIG_MAX_LIMIT_TASK) + (indicator_id * APP_CONFIG_MAX_LIMIT_TASK) + task_id, bytes);
    APP_CHECK_RC(rc)
    message->has_config = read_uint8_t(&bytes[APP_CONFIG_EEPROM_LIMIT_HAS_CONFIG_OFFSET]);
    message->enable = read_uint8_t(&bytes[APP_CONFIG_EEPROM_LIMIT_ENABLE_OFFSET]);
    message->relay_id = read_uint8_t(&bytes[APP_CONFIG_EEPROM_LIMIT_RELAY_ID_OFFSET]);
    message->channel = read_uint16_t(&bytes[APP_CONFIG_EEPROM_LIMIT_CHANNEL_OFFSET]);
    message->type = read_uint8_t(&bytes[APP_CONFIG_EEPROM_LIMIT_TYPE_OFFSET]);
    message->up_limit = read_uint16_t(&bytes[APP_CONFIG_EEPROM_LIMIT_UP_LIMIT_OFFSET]);
    message->down_limit = read_uint16_t(&bytes[APP_CONFIG_EEPROM_LIMIT_DOWN_LIMIT_OFFSET]);
    return APP_OK;
}

int APP_CONFIG_limit_write(uint8_t probe_id, uint8_t indicator_id, uint8_t task_id,  app_config_limit_pt message)
{
    uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};

    if((probe_id > PROBE_SIZE) || (indicator_id > INDICATOR_SIZE) || (task_id > APP_CONFIG_MAX_LIMIT_TASK)){return APP_ERROR;}
    _limit_serialize(bytes, message);
    return epprom_write_block(APP_CONFIG_EEPROM_LIMIT_PAGE + (probe_id * 16 * APP_CONFIG_MAX_LIMIT_TASK) + (indicator_id * APP_CONFIG_MAX_LIMIT_TASK) + task_id, bytes);
}

int APP_CONFIG_limit_write_enable(uint8_t probe_id, uint8_t indicator_id, uint8_t task_id,  app_config_limit_pt message, uint8_t has_config, uint8_t enable)
{
    uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};

    if((probe_id > PROBE_SIZE) || (indicator_id > INDICATOR_SIZE) || (task_id > APP_CONFIG_MAX_LIMIT_TASK)){return APP_ERROR;}
    _limit_serialize(bytes, message);
    write_uint8_t(has_config, &bytes[APP_CONFIG_EEPROM_LIMIT_HAS_CONFIG_OFFSET]);
    write_uint8_t(enable, &bytes[APP_CONFIG_EEPROM_LIMIT_ENABLE_OFFSET]);
    int rc = epprom_write_block(APP_CONFIG_EEPROM_LIMIT_PAGE + (probe_id * 16 * APP_CONFIG_MAX_LIMIT_TASK) + (indicator_id * APP_CONFIG_MAX_LIMIT_TASK) + task_id, bytes);
    APP_CHECK_RC(rc)
    message->has_config = has_config; 
    message->enable = enable; 
    return APP_OK;
}

int APP_CONFIG_limit_load(void)
{
    int rc = 0;
    uint8_t probe_id;
    uint8_t indicator_id;
    uint8_t task_id;

    for(probe_id = 0; probe_id < PROBE_SIZE; probe_id++ ) 
    {
        for(indicator_id = 0; indicator_id < INDICATOR_SIZE; indicator_id++ ) 
        {
            for(task_id = 0; task_id < APP_CONFIG_MAX_LIMIT_TASK; task_id++ ) 
            {
                APP_CONFIG_limit_init(&app_config_limit[probe_id][indicator_id][task_id]);
                rc = APP_CONFIG_limit_read(probe_id,indicator_id,task_id, &app_config_limit[probe_id][indicator_id][task_id]);
                APP_CHECK_RC(rc)
            } 
        } 
    } 
    return APP_OK;
}

int APP_CONFIG_limit_recovery(void)
{
    int rc = 0;
    uint8_t probe_id;
    uint8_t indicator_id;
    uint8_t task_id;

    for(probe_id = 0; probe_id < PROBE_SIZE; probe_id++ ) 
    {
        for(indicator_id = 0; indicator_id < INDICATOR_SIZE; indicator_id++ ) 
        {
            for(task_id = 0; task_id < APP_CONFIG_MAX_LIMIT_TASK; task_id++ ) 
            {
                APP_CONFIG_limit_init(&app_config_limit[probe_id][indicator_id][task_id]);
                rc = APP_CONFIG_limit_write(probe_id,indicator_id,task_id, &app_config_limit[probe_id][indicator_id][task_id]);
                APP_CHECK_RC(rc)
            } 
        } 
    } 
    return APP_OK;
}

int APP_CONFIG_relay_init(app_config_relay_pt message)
{
    message->has_config = 0;
    message->enable = 0;
    message->warning = 0;
    return APP_OK;
}

static void _relay_serialize(uint8_t bytes[EEPROM_BLOCK_SIZE], app_config_relay_pt message)
{
    write_uint8_t(message->has_config, &bytes[APP_CONFIG_EEPROM_RELAY_HAS_CONFIG_OFFSET]); 
    write_uint8_t(message->enable, &bytes[APP_CONFIG_EEPROM_RELAY_ENABLE_OFFSET]); 
    write_uint8_t(message->warning, &bytes[APP_CONFIG_EEPROM_RELAY_WARNING_OFFSET]); 
}

app_config_relay_t app_config_relay[RELAY_SIZE];
int APP_CONFIG_relay_read(uint8_t relay_id,  app_config_relay_pt message)
{
    uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};

    if((relay_id > RELAY_SIZE)){return APP_ERROR;}
    int rc = epprom_read_block(APP_CONFIG_EEPROM_RELAY_PAGE + relay_id, bytes);
    APP_CHECK_RC(rc)
    message->has_config = read_uint8_t(&bytes[APP_CONFIG_EEPROM_RELAY_HAS_CONFIG_OFFSET]);
    message->enable = read_uint8_t(&bytes[APP_CONFIG_EEPROM_RELAY_ENABLE_OFFSET]);
    message->warning = read_uint8_t(&bytes[APP_CONFIG_EEPROM_RELAY_WARNING_OFFSET]);
    return APP_OK;
}

int APP_CONFIG_relay_write(uint8_t relay_id,  app_config_relay_pt message)
{
    uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};

    if((relay_id > RELAY_SIZE)){return APP_ERROR;}
    _relay_serialize(bytes, message);
    return epprom_write_block(APP_CONFIG_EEPROM_RELAY_PAGE + relay_id, bytes);
}

int APP_CONFIG_relay_write_has_config(uint8_t relay_id,  app_config_relay_pt message, uint8_t has_config)
{
    uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};

    if((relay_id > RELAY_SIZE)){return APP_ERROR;}
    _relay_serialize(bytes, message);
    write_uint8_t(has_config, &bytes[APP_CONFIG_EEPROM_RELAY_HAS_CONFIG_OFFSET]);
    int rc = epprom_write_block(APP_CONFIG_EEPROM_RELAY_PAGE + relay_id, bytes);
    APP_CHECK_RC(rc)
    message->has_config = has_config; 
    return APP_OK;
}

int APP_CONFIG_relay_write_enable(uint8_t relay_id,  app_config_relay_pt message, uint8_t enable)
{
    uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};

    if((relay_id > RELAY_SIZE)){return APP_ERROR;}
    _relay_serialize(bytes, message);
    write_uint8_t(enable, &bytes[APP_CONFIG_EEPROM_RELAY_ENABLE_OFFSET]);
    int rc = epprom_write_block(APP_CONFIG_EEPROM_RELAY_PAGE + relay_id, bytes);
    APP_CHECK_RC(rc)
    message->enable = enable; 
    return APP_OK;
}

int APP_CONFIG_relay_write_warning(uint8_t relay_id,  app_config_relay_pt message, uint8_t warning)
{
    uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};

    if((relay_id > RELAY_SIZE)){return APP_ERROR;}
    _relay_serialize(bytes, message);
    write_uint8_t(warning, &bytes[APP_CONFIG_EEPROM_RELAY_WARNING_OFFSET]);
    int rc = epprom_write_block(APP_CONFIG_EEPROM_RELAY_PAGE + relay_id, bytes);
    APP_CHECK_RC(rc)
    message->warning = warning; 
    return APP_OK;
}

int APP_CONFIG_relay_load(void)
{
    int rc = 0;
    uint8_t relay_id;

    for(relay_id = 0; relay_id < RELAY_SIZE; relay_id++ ) 
    {
        APP_CONFIG_relay_init(&app_config_relay[relay_id]);
        rc = APP_CONFIG_relay_read(relay_id, &app_config_relay[relay_id]);
        APP_CHECK_RC(rc)
    } 
    return APP_OK;
}

int APP_CONFIG_relay_recovery(void)
{
    int rc = 0;
    uint8_t relay_id;
    for(relay_id = 0; relay_id < RELAY_SIZE; relay_id++ ) 
    {
        APP_CONFIG_relay_init(&app_config_relay[relay_id]);
        rc = APP_CONFIG_relay_write(relay_id, &app_config_relay[relay_id]);
        APP_CHECK_RC(rc)
    } 
    return APP_OK;
}
int APP_CONFIG_times_enable_init(app_config_times_enable_pt message)
{
    APP_CONFIG_config_enable_init(&message->task[0]);
    APP_CONFIG_config_enable_init(&message->task[1]);
    APP_CONFIG_config_enable_init(&message->task[2]);
    APP_CONFIG_config_enable_init(&message->task[3]);
    APP_CONFIG_config_enable_init(&message->task[4]);
    APP_CONFIG_config_enable_init(&message->task[5]);
    APP_CONFIG_config_enable_init(&message->task[6]);
    APP_CONFIG_config_enable_init(&message->task[7]);
    return APP_OK;
}

static void _times_enable_serialize(uint8_t bytes[EEPROM_BLOCK_SIZE], app_config_times_enable_pt message)
{
    _config_enable_serialize(&bytes[APP_CONFIG_EEPROM_TIMES_ENABLE_TASK_OFFSET + 0], &message->task[0]);
    _config_enable_serialize(&bytes[APP_CONFIG_EEPROM_TIMES_ENABLE_TASK_OFFSET + 2], &message->task[1]);
    _config_enable_serialize(&bytes[APP_CONFIG_EEPROM_TIMES_ENABLE_TASK_OFFSET + 4], &message->task[2]);
    _config_enable_serialize(&bytes[APP_CONFIG_EEPROM_TIMES_ENABLE_TASK_OFFSET + 6], &message->task[3]);
    _config_enable_serialize(&bytes[APP_CONFIG_EEPROM_TIMES_ENABLE_TASK_OFFSET + 8], &message->task[4]);
    _config_enable_serialize(&bytes[APP_CONFIG_EEPROM_TIMES_ENABLE_TASK_OFFSET + 10], &message->task[5]);
    _config_enable_serialize(&bytes[APP_CONFIG_EEPROM_TIMES_ENABLE_TASK_OFFSET + 12], &message->task[6]);
    _config_enable_serialize(&bytes[APP_CONFIG_EEPROM_TIMES_ENABLE_TASK_OFFSET + 14], &message->task[7]);
}

app_config_times_enable_t app_config_times_enable[RELAY_SIZE];
int APP_CONFIG_times_enable_read(uint8_t relay_id,  app_config_times_enable_pt message)
{
    uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};

    if((relay_id > RELAY_SIZE)){return APP_ERROR;}
    int rc = epprom_read_block(APP_CONFIG_EEPROM_TIMES_ENABLE_PAGE + relay_id, bytes);
    APP_CHECK_RC(rc)
    _config_enable_deserialize(&bytes[APP_CONFIG_EEPROM_TIMES_ENABLE_TASK_OFFSET + 0], &message->task[0]);
    _config_enable_deserialize(&bytes[APP_CONFIG_EEPROM_TIMES_ENABLE_TASK_OFFSET + 2], &message->task[1]);
    _config_enable_deserialize(&bytes[APP_CONFIG_EEPROM_TIMES_ENABLE_TASK_OFFSET + 4], &message->task[2]);
    _config_enable_deserialize(&bytes[APP_CONFIG_EEPROM_TIMES_ENABLE_TASK_OFFSET + 6], &message->task[3]);
    _config_enable_deserialize(&bytes[APP_CONFIG_EEPROM_TIMES_ENABLE_TASK_OFFSET + 8], &message->task[4]);
    _config_enable_deserialize(&bytes[APP_CONFIG_EEPROM_TIMES_ENABLE_TASK_OFFSET + 10], &message->task[5]);
    _config_enable_deserialize(&bytes[APP_CONFIG_EEPROM_TIMES_ENABLE_TASK_OFFSET + 12], &message->task[6]);
    _config_enable_deserialize(&bytes[APP_CONFIG_EEPROM_TIMES_ENABLE_TASK_OFFSET + 14], &message->task[7]);
    return APP_OK;
}

int APP_CONFIG_times_enable_write(uint8_t relay_id,  app_config_times_enable_pt message)
{
    uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};

    if((relay_id > RELAY_SIZE)){return APP_ERROR;}
    _times_enable_serialize(bytes, message);
    return epprom_write_block(APP_CONFIG_EEPROM_TIMES_ENABLE_PAGE + relay_id, bytes);
}

int APP_CONFIG_times_enable_write_config_enable(uint8_t relay_id,  app_config_times_enable_pt message, uint8_t task_index, app_config_config_enable_t task)
{
    uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};

    if((relay_id > RELAY_SIZE)){return APP_ERROR;}
    _times_enable_serialize(bytes, message);
    _config_enable_serialize(&bytes[APP_CONFIG_EEPROM_TIMES_ENABLE_TASK_OFFSET + 2 * task_index], &task);
    int rc = epprom_write_block(APP_CONFIG_EEPROM_TIMES_ENABLE_PAGE + relay_id, bytes);
    APP_CHECK_RC(rc)
    _config_enable_copy(&message->task[task_index], &task); 
    return APP_OK;
}

int APP_CONFIG_times_enable_load(void)
{
    int rc = 0;
    uint8_t relay_id;

    for(relay_id = 0; relay_id < RELAY_SIZE; relay_id++ ) 
    {
        APP_CONFIG_times_enable_init(&app_config_times_enable[relay_id]);
        rc = APP_CONFIG_times_enable_read(relay_id, &app_config_times_enable[relay_id]);
        APP_CHECK_RC(rc)
    } 
    return APP_OK;
}

int APP_CONFIG_times_enable_recovery(void)
{
    int rc = 0;
    uint8_t relay_id;

    for(relay_id = 0; relay_id < RELAY_SIZE; relay_id++ ) 
    {
        APP_CONFIG_times_enable_init(&app_config_times_enable[relay_id]);
        rc = APP_CONFIG_times_enable_write(relay_id, &app_config_times_enable[relay_id]);
        APP_CHECK_RC(rc)
    } 
    return APP_OK;
}
int APP_CONFIG_times_job_init(app_config_times_job_pt message)
{
    message->type = 0;
    message->start_hour = 0;
    message->start_minute = 0;
    message->end_hour = 0;
    message->end_minute = 0;
    return APP_OK;
}

static void _times_job_serialize(uint8_t bytes[EEPROM_BLOCK_SIZE], app_config_times_job_pt message)
{
    write_uint8_t(message->type, &bytes[APP_CONFIG_EEPROM_TIMES_JOB_TYPE_OFFSET]); 
    write_uint8_t(message->start_hour, &bytes[APP_CONFIG_EEPROM_TIMES_JOB_START_HOUR_OFFSET]); 
    write_uint8_t(message->start_minute, &bytes[APP_CONFIG_EEPROM_TIMES_JOB_START_MINUTE_OFFSET]); 
    write_uint8_t(message->end_hour, &bytes[APP_CONFIG_EEPROM_TIMES_JOB_END_HOUR_OFFSET]); 
    write_uint8_t(message->end_minute, &bytes[APP_CONFIG_EEPROM_TIMES_JOB_END_MINUTE_OFFSET]); 
}

static int _times_job_deserialize(uint8_t *bytes, app_config_times_job_pt message)
{
    message->type = read_uint8_t(&bytes[APP_CONFIG_EEPROM_TIMES_JOB_TYPE_OFFSET]); 
    message->start_hour = read_uint8_t(&bytes[APP_CONFIG_EEPROM_TIMES_JOB_START_HOUR_OFFSET]); 
    message->start_minute = read_uint8_t(&bytes[APP_CONFIG_EEPROM_TIMES_JOB_START_MINUTE_OFFSET]); 
    message->end_hour = read_uint8_t(&bytes[APP_CONFIG_EEPROM_TIMES_JOB_END_HOUR_OFFSET]); 
    message->end_minute = read_uint8_t(&bytes[APP_CONFIG_EEPROM_TIMES_JOB_END_MINUTE_OFFSET]); 
    return APP_OK;
}

static void _times_job_copy( app_config_times_job_pt dist, app_config_times_job_pt from)
{
    dist->type = from->type; 
    dist->start_hour = from->start_hour; 
    dist->start_minute = from->start_minute; 
    dist->end_hour = from->end_hour; 
    dist->end_minute = from->end_minute; 
}

int APP_CONFIG_times_init(app_config_times_pt message)
{
    message->has_limit = 0;
    message->channel = 0;
    message->probe_id = 0;
    message->indicator_id = 0;
    message->value = 0;
    message->threshold = 0;
    message->execute_type = 0;
    APP_CONFIG_times_job_init(&message->times[0]);
    APP_CONFIG_times_job_init(&message->times[1]);
    APP_CONFIG_times_job_init(&message->times[2]);
    APP_CONFIG_times_job_init(&message->times[3]);
    return APP_OK;
}

static void _times_serialize(uint8_t bytes[EEPROM_BLOCK_SIZE], app_config_times_pt message)
{
    write_uint8_t(message->has_limit, &bytes[APP_CONFIG_EEPROM_TIMES_HAS_LIMIT_OFFSET]); 
    write_uint16_t(message->channel, &bytes[APP_CONFIG_EEPROM_TIMES_CHANNEL_OFFSET]); 
    write_uint8_t(message->probe_id, &bytes[APP_CONFIG_EEPROM_TIMES_PROBE_ID_OFFSET]); 
    write_uint8_t(message->indicator_id, &bytes[APP_CONFIG_EEPROM_TIMES_INDICATOR_ID_OFFSET]); 
    write_uint16_t(message->value, &bytes[APP_CONFIG_EEPROM_TIMES_VALUE_OFFSET]); 
    write_uint16_t(message->threshold, &bytes[APP_CONFIG_EEPROM_TIMES_THRESHOLD_OFFSET]); 
    write_uint8_t(message->execute_type, &bytes[APP_CONFIG_EEPROM_TIMES_EXECUTE_TYPE_OFFSET]); 
    _times_job_serialize(&bytes[APP_CONFIG_EEPROM_TIMES_TIMES_OFFSET + 0], &message->times[0]);
    _times_job_serialize(&bytes[APP_CONFIG_EEPROM_TIMES_TIMES_OFFSET + 5], &message->times[1]);
    _times_job_serialize(&bytes[APP_CONFIG_EEPROM_TIMES_TIMES_OFFSET + 10], &message->times[2]);
    _times_job_serialize(&bytes[APP_CONFIG_EEPROM_TIMES_TIMES_OFFSET + 15], &message->times[3]);
}

app_config_times_t app_config_times[RELAY_SIZE][APP_CONFIG_MAX_TIMES_TASK];
int APP_CONFIG_times_read(uint8_t relay_id, uint8_t task_id,  app_config_times_pt message)
{
    uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};

    if((relay_id > RELAY_SIZE) || (task_id > APP_CONFIG_MAX_TIMES_TASK)){return APP_ERROR;}
    int rc = epprom_read_block(APP_CONFIG_EEPROM_TIMES_PAGE + (relay_id * APP_CONFIG_MAX_TIMES_TASK) + task_id, bytes);
    APP_CHECK_RC(rc)
    message->has_limit = read_uint8_t(&bytes[APP_CONFIG_EEPROM_TIMES_HAS_LIMIT_OFFSET]);
    message->channel = read_uint16_t(&bytes[APP_CONFIG_EEPROM_TIMES_CHANNEL_OFFSET]);
    message->probe_id = read_uint8_t(&bytes[APP_CONFIG_EEPROM_TIMES_PROBE_ID_OFFSET]);
    message->indicator_id = read_uint8_t(&bytes[APP_CONFIG_EEPROM_TIMES_INDICATOR_ID_OFFSET]);
    message->value = read_uint16_t(&bytes[APP_CONFIG_EEPROM_TIMES_VALUE_OFFSET]);
    message->threshold = read_uint16_t(&bytes[APP_CONFIG_EEPROM_TIMES_THRESHOLD_OFFSET]);
    message->execute_type = read_uint8_t(&bytes[APP_CONFIG_EEPROM_TIMES_EXECUTE_TYPE_OFFSET]);
    _times_job_deserialize(&bytes[APP_CONFIG_EEPROM_TIMES_TIMES_OFFSET + 0], &message->times[0]);
    _times_job_deserialize(&bytes[APP_CONFIG_EEPROM_TIMES_TIMES_OFFSET + 5], &message->times[1]);
    _times_job_deserialize(&bytes[APP_CONFIG_EEPROM_TIMES_TIMES_OFFSET + 10], &message->times[2]);
    _times_job_deserialize(&bytes[APP_CONFIG_EEPROM_TIMES_TIMES_OFFSET + 15], &message->times[3]);
    return APP_OK;
}

int APP_CONFIG_times_write(uint8_t relay_id, uint8_t task_id,  app_config_times_pt message)
{
    uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};

    if((relay_id > RELAY_SIZE) || (task_id > APP_CONFIG_MAX_TIMES_TASK)){return APP_ERROR;}
    _times_serialize(bytes, message);
    return epprom_write_block(APP_CONFIG_EEPROM_TIMES_PAGE + (relay_id * APP_CONFIG_MAX_TIMES_TASK) + task_id, bytes);
}

int APP_CONFIG_times_load(void)
{
    int rc = 0;
    uint8_t relay_id;
    uint8_t task_id;

    for(relay_id = 0; relay_id < RELAY_SIZE; relay_id++ ) 
    {
        for(task_id = 0; task_id < APP_CONFIG_MAX_TIMES_TASK; task_id++ ) 
        {
            APP_CONFIG_times_init(&app_config_times[relay_id][task_id]);
            rc = APP_CONFIG_times_read(relay_id,task_id, &app_config_times[relay_id][task_id]);
            APP_CHECK_RC(rc)
        } 
    } 
    return APP_OK;
}

int APP_CONFIG_times_recovery(void)
{
    int rc = 0;
    uint8_t relay_id;
    uint8_t task_id;

    for(relay_id = 0; relay_id < RELAY_SIZE; relay_id++ ) 
    {
        for(task_id = 0; task_id < APP_CONFIG_MAX_TIMES_TASK; task_id++ ) 
        {
            APP_CONFIG_times_init(&app_config_times[relay_id][task_id]);
            rc = APP_CONFIG_times_write(relay_id,task_id, &app_config_times[relay_id][task_id]);
            APP_CHECK_RC(rc)
        } 
    } 
    return APP_OK;
}
int APP_CONFIG_manual_relay_init(app_config_manual_relay_pt message)
{
    message->has_config = 0;
    message->enable = 0;
    message->type = 0;
    APP_CONFIG_time_init(&message->start);
    APP_CONFIG_time_init(&message->end);
    return APP_OK;
}

static void _manual_relay_serialize(uint8_t bytes[EEPROM_BLOCK_SIZE], app_config_manual_relay_pt message)
{
    write_uint8_t(message->has_config, &bytes[APP_CONFIG_EEPROM_MANUAL_RELAY_HAS_CONFIG_OFFSET]); 
    write_uint8_t(message->enable, &bytes[APP_CONFIG_EEPROM_MANUAL_RELAY_ENABLE_OFFSET]); 
    write_uint8_t(message->type, &bytes[APP_CONFIG_EEPROM_MANUAL_RELAY_TYPE_OFFSET]); 
    _time_serialize(&bytes[APP_CONFIG_EEPROM_MANUAL_RELAY_START_OFFSET], &message->start); 
    _time_serialize(&bytes[APP_CONFIG_EEPROM_MANUAL_RELAY_END_OFFSET], &message->end); 
}

app_config_manual_relay_t app_config_manual_relay[RELAY_SIZE][RELAY_CHANNEL_SIZE];
int APP_CONFIG_manual_relay_read(uint8_t relay_id, uint8_t channel_id,  app_config_manual_relay_pt message)
{
    uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};

    if((relay_id > RELAY_SIZE) || (channel_id > RELAY_CHANNEL_SIZE)){return APP_ERROR;}
    int rc = epprom_read_block(APP_CONFIG_EEPROM_MANUAL_RELAY_PAGE + (relay_id * 16) + channel_id, bytes);
    APP_CHECK_RC(rc)
    message->has_config = read_uint8_t(&bytes[APP_CONFIG_EEPROM_MANUAL_RELAY_HAS_CONFIG_OFFSET]);
    message->enable = read_uint8_t(&bytes[APP_CONFIG_EEPROM_MANUAL_RELAY_ENABLE_OFFSET]);
    message->type = read_uint8_t(&bytes[APP_CONFIG_EEPROM_MANUAL_RELAY_TYPE_OFFSET]);
    _time_deserialize(&bytes[APP_CONFIG_EEPROM_MANUAL_RELAY_START_OFFSET], &message->start);
    _time_deserialize(&bytes[APP_CONFIG_EEPROM_MANUAL_RELAY_END_OFFSET], &message->end);
    return APP_OK;
}

int APP_CONFIG_manual_relay_write(uint8_t relay_id, uint8_t channel_id,  app_config_manual_relay_pt message)
{
    uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};

    if((relay_id > RELAY_SIZE) || (channel_id > RELAY_CHANNEL_SIZE)){return APP_ERROR;}
    _manual_relay_serialize(bytes, message);
    return epprom_write_block(APP_CONFIG_EEPROM_MANUAL_RELAY_PAGE + (relay_id * 16) + channel_id, bytes);
}

int APP_CONFIG_manual_relay_write_enable(uint8_t relay_id, uint8_t channel_id,  app_config_manual_relay_pt message, uint8_t enable)
{
    uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};

    if((relay_id > RELAY_SIZE) || (channel_id > RELAY_CHANNEL_SIZE)){return APP_ERROR;}
    _manual_relay_serialize(bytes, message);
    write_uint8_t(enable, &bytes[APP_CONFIG_EEPROM_MANUAL_RELAY_ENABLE_OFFSET]);
    int rc = epprom_write_block(APP_CONFIG_EEPROM_MANUAL_RELAY_PAGE + (relay_id * 16) + channel_id, bytes);
    APP_CHECK_RC(rc)
    message->enable = enable; 
    return APP_OK;
}

int APP_CONFIG_manual_relay_write_time(uint8_t relay_id, uint8_t channel_id,  app_config_manual_relay_pt message, uint8_t has_config, uint8_t enable, uint8_t type, app_config_time_t start, app_config_time_t end)
{
    uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};

    if((relay_id > RELAY_SIZE) || (channel_id > RELAY_CHANNEL_SIZE)){return APP_ERROR;}
    _manual_relay_serialize(bytes, message);
    write_uint8_t(has_config, &bytes[APP_CONFIG_EEPROM_MANUAL_RELAY_HAS_CONFIG_OFFSET]);
    write_uint8_t(enable, &bytes[APP_CONFIG_EEPROM_MANUAL_RELAY_ENABLE_OFFSET]);
    write_uint8_t(type, &bytes[APP_CONFIG_EEPROM_MANUAL_RELAY_TYPE_OFFSET]);
    _time_serialize(&bytes[APP_CONFIG_EEPROM_MANUAL_RELAY_START_OFFSET], &start);
    _time_serialize(&bytes[APP_CONFIG_EEPROM_MANUAL_RELAY_END_OFFSET], &end);
    int rc = epprom_write_block(APP_CONFIG_EEPROM_MANUAL_RELAY_PAGE + (relay_id * 16) + channel_id, bytes);
    APP_CHECK_RC(rc)
    message->has_config = has_config; 
    message->enable = enable; 
    message->type = type; 
    _time_copy(&message->start, &start); 
    _time_copy(&message->end, &end); 
    return APP_OK;
}

int APP_CONFIG_manual_relay_load(void)
{
    int rc = 0;
    uint8_t relay_id;
    uint8_t channel_id;

    for(relay_id = 0; relay_id < RELAY_SIZE; relay_id++ ) 
    {
        for(channel_id = 0; channel_id < RELAY_CHANNEL_SIZE; channel_id++ ) 
        {
            APP_CONFIG_manual_relay_init(&app_config_manual_relay[relay_id][channel_id]);
            rc = APP_CONFIG_manual_relay_read(relay_id,channel_id, &app_config_manual_relay[relay_id][channel_id]);
            APP_CHECK_RC(rc)
        } 
    } 
    return APP_OK;
}

int APP_CONFIG_manual_relay_recovery(void)
{
    int rc = 0;
    uint8_t relay_id;
    uint8_t channel_id;

    for(relay_id = 0; relay_id < RELAY_SIZE; relay_id++ ) 
    {
        for(channel_id = 0; channel_id < RELAY_CHANNEL_SIZE; channel_id++ ) 
        {
            APP_CONFIG_manual_relay_init(&app_config_manual_relay[relay_id][channel_id]);
            rc = APP_CONFIG_manual_relay_write(relay_id,channel_id, &app_config_manual_relay[relay_id][channel_id]);
            APP_CHECK_RC(rc)
        } 
    } 
    return APP_OK;
}
app_config_l6_t app_config_l6;
int APP_CONFIG_l6_init(app_config_l6_pt message)
{
	message->has_config=0XF8;
	message->release=20;
	message->read_wait=95;
	message->read=50;
	message->charge=15;
	message->charge_wait=7;
	return APP_OK;
}

static void _l6_serialize(uint8_t bytes[EEPROM_BLOCK_SIZE], app_config_l6_pt message)
{
    write_uint8_t(message->has_config, &bytes[APP_CONFIG_EEPROM_L6_HAS_CONFIG_OFFSET]); 
    write_uint8_t(message->release, &bytes[APP_CONFIG_EEPROM_L6_REALEASE_OFFSET]); 
    write_uint8_t(message->read_wait, &bytes[APP_CONFIG_EEPROM_L6_READ_WAIT_OFFSET]); 
    write_uint8_t(message->read, &bytes[APP_CONFIG_EEPROM_L6_READ_OFFSET]); 
    write_uint8_t(message->charge, &bytes[APP_CONFIG_EEPROM_L6_CHARGE_OFFSET]); 
    write_uint8_t(message->charge_wait, &bytes[APP_CONFIG_EEPROM_L6_CHARGE_WAIT_OFFSET]); 

}

int APP_CONFIG_l6_write(uint8_t probe_id,app_config_l6_pt message)
{
	uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};

	if((probe_id > PROBE_SIZE)){return APP_ERROR;}
	_l6_serialize(bytes, message);
	int rc = epprom_write_block(APP_CONFIG_EEPROM_L6_PAGE + probe_id, bytes);
    APP_CHECK_RC(rc)
	return APP_OK;
}

int APP_CONFIG_l6_read(uint8_t probe_id,app_config_l6_pt message)
{
	uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};

	if((probe_id > PROBE_SIZE)){return APP_ERROR;}
	int rc=epprom_read_block(APP_CONFIG_EEPROM_L6_PAGE + probe_id, bytes);
	APP_CHECK_RC(rc)
	message->has_config=read_uint8_t(&bytes[APP_CONFIG_EEPROM_L6_HAS_CONFIG_OFFSET]);
	message->release=read_uint8_t(&bytes[APP_CONFIG_EEPROM_L6_REALEASE_OFFSET]);
	message->read_wait=read_uint8_t(&bytes[APP_CONFIG_EEPROM_L6_READ_WAIT_OFFSET]);
	message->read=read_uint8_t(&bytes[APP_CONFIG_EEPROM_L6_READ_OFFSET]);
	message->charge=read_uint8_t(&bytes[APP_CONFIG_EEPROM_L6_CHARGE_OFFSET]);
	message->charge_wait=read_uint8_t(&bytes[APP_CONFIG_EEPROM_L6_CHARGE_WAIT_OFFSET]);
	return APP_OK;
}

int APP_CONFIG_l6_load(void)
{
	uint8_t probe_id;

    for(probe_id = 0; probe_id < PROBE_SIZE; probe_id++ ) 
	{
        APP_CONFIG_l6_init(&app_config_l6);
        int rc=APP_CONFIG_l6_read(probe_id, &app_config_l6);
        APP_CHECK_RC(rc)
	}
	return APP_OK;
}

int APP_CONFIG_l6_recovery(void)
{
	int rc=0;
    uint8_t probe_id;

    for(probe_id = 0; probe_id < PROBE_SIZE; probe_id++ ) 
	{
        APP_CONFIG_l6_init(&app_config_l6);
        rc=APP_CONFIG_l6_write(probe_id, &app_config_l6);
        APP_CHECK_RC(rc)
	}
	return APP_OK;
}




app_config_firmware_setting_info_t firmware_setting_info1 = {0};
app_config_firmware_setting_info_t firmware_setting_info2 = {0};
static int _firmware_info_init(app_config_firmware_info_pt message)
{
	message->device_type = 0;
	message->version_h = 0;
	message->version_m = 0;
	message->version_l = 0;
	message->file_len = 0;
	message->file_crc = 0;
	
	return APP_OK;
}

static int _firmware_info_deserialize(uint8_t *bytes, app_config_firmware_info_pt message)
{
	message->device_type = read_uint8_t(&bytes[APP_CONFIG_EEPROM_FIRMWARE_INFO_DEVICE_TYPE_OFFSET]); 
	message->version_h = read_uint8_t(&bytes[APP_CONFIG_EEPROM_FIRMWARE_INFO_VERSION_H_OFFSET]); 
	message->version_m = read_uint8_t(&bytes[APP_CONFIG_EEPROM_FIRMWARE_INFO_VERSION_M_OFFSET]); 
	message->version_l = read_uint8_t(&bytes[APP_CONFIG_EEPROM_FIRMWARE_INFO_VERSION_L_OFFSET]);
	message->file_len = read_uint32_t(&bytes[APP_CONFIG_EEPROM_FIRMWARE_INFO_FILE_LEN_OFFSET]); 
	message->file_crc = read_uint32_t(&bytes[APP_CONFIG_EEPROM_FIRMWARE_INFO_FILE_CRC_OFFSET]);
	
	return APP_OK;
}



static int _firmware_info_serialize(uint8_t *bytes, app_config_firmware_info_pt message)
{
	write_uint8_t(message->device_type, &bytes[APP_CONFIG_EEPROM_FIRMWARE_INFO_DEVICE_TYPE_OFFSET]); 
	write_uint8_t(message->version_h, &bytes[APP_CONFIG_EEPROM_FIRMWARE_INFO_VERSION_H_OFFSET]); 
	write_uint8_t(message->version_m, &bytes[APP_CONFIG_EEPROM_FIRMWARE_INFO_VERSION_M_OFFSET]); 
	write_uint8_t(message->version_l, &bytes[APP_CONFIG_EEPROM_FIRMWARE_INFO_VERSION_L_OFFSET]);
	write_uint32_t(message->file_len, &bytes[APP_CONFIG_EEPROM_FIRMWARE_INFO_FILE_LEN_OFFSET]); 
	write_uint32_t(message->file_crc, &bytes[APP_CONFIG_EEPROM_FIRMWARE_INFO_FILE_CRC_OFFSET]); 

	return APP_OK;
}



int APP_CONFIG_firmware_setting_info_init(app_config_firmware_setting_info_pt message)
{
	message->download_len = 0;
	firmware_setting_info2.reserved[0] = 0;
	firmware_setting_info2.reserved[1] = 0;
	firmware_setting_info2.reserved[2] = 0;
	firmware_setting_info2.reserved[3] = 0;
	_firmware_info_init(&message->firmware_info);
	
	return APP_OK;
}


int APP_CONFIG_firmware_setting_info_read(uint8_t infotype, app_config_firmware_setting_info_pt message)
{
	int rc = 0;
	uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};
	uint16_t page = 0;
		
	if(APP_CONFIG_FIRMWARE_SETTING_INFO == infotype)
	{
		page = APP_CONFIG_EEPROM_FIRMWARE_SETTING_INFO_PAGE;
	}
	else
	{
		page = APP_CONFIG_EEPROM_FIRMWARE_SETTING_INFO_BACK_PAGE;
	}

	rc = epprom_read_block(page, bytes);
	APP_CHECK_RC(rc)
	message->download_len = read_uint32_t(&bytes[APP_CONFIG_EEPROM_FIRMWARE_INFO_DOWNLOAD_LEN_OFFSET]);
    message->reserved[0] = read_uint8_t(&bytes[APP_CONFIG_EEPROM_FIRMWARE_INFO_RESERVED_OFFSET]);
	message->reserved[1] = read_uint8_t(&bytes[APP_CONFIG_EEPROM_FIRMWARE_INFO_RESERVED_OFFSET+1]);
	message->reserved[2] = read_uint8_t(&bytes[APP_CONFIG_EEPROM_FIRMWARE_INFO_RESERVED_OFFSET+2]);
	message->reserved[3] = read_uint8_t(&bytes[APP_CONFIG_EEPROM_FIRMWARE_INFO_RESERVED_OFFSET+3]);
	_firmware_info_deserialize(bytes, &message->firmware_info);
	
	return APP_OK;
}


int APP_CONFIG_firmware_setting_info_write(uint8_t infotype, app_config_firmware_setting_info_pt message)
{
	uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};
	uint16_t page = 0;
	
	if(APP_CONFIG_FIRMWARE_SETTING_INFO == infotype)
	{
		page = APP_CONFIG_EEPROM_FIRMWARE_SETTING_INFO_PAGE;
	}
	else
	{
		page = APP_CONFIG_EEPROM_FIRMWARE_SETTING_INFO_BACK_PAGE;
	}
	write_uint32_t(message->download_len, &bytes[APP_CONFIG_EEPROM_FIRMWARE_INFO_DOWNLOAD_LEN_OFFSET]);
    write_uint8_t(message->reserved[0],  &bytes[APP_CONFIG_EEPROM_FIRMWARE_INFO_RESERVED_OFFSET]);
	write_uint8_t(message->reserved[1],  &bytes[APP_CONFIG_EEPROM_FIRMWARE_INFO_RESERVED_OFFSET+1]);
	write_uint8_t(message->reserved[2],  &bytes[APP_CONFIG_EEPROM_FIRMWARE_INFO_RESERVED_OFFSET+2]);
	write_uint8_t(message->reserved[3],  &bytes[APP_CONFIG_EEPROM_FIRMWARE_INFO_RESERVED_OFFSET+3]);
	_firmware_info_serialize(bytes, &message->firmware_info);
	return epprom_write_block(page, bytes);
}


int APP_CONFIG_firmware_setting_info_overwrite(void)
{
	int rc = 0;

	rc = APP_CONFIG_firmware_setting_info_write(APP_CONFIG_FIRMWARE_SETTING_INFO_BACK, &firmware_setting_info2);
	APP_CHECK_RC(rc)
	memcpy((uint8_t *)&firmware_setting_info1, (uint8_t *)&firmware_setting_info2, sizeof(app_config_firmware_setting_info_t));
	rc = APP_CONFIG_firmware_setting_info_write(APP_CONFIG_FIRMWARE_SETTING_INFO, &firmware_setting_info1);
	APP_CHECK_RC(rc)

	return APP_OK;
}

// int APP_CONFIG_firmware_setting_info_recoverywrite(void)
// {
// 	int rc = 0;
// 	memcpy((uint8_t *)&firmware_setting_info2, (uint8_t *)&firmware_setting_info1, sizeof(app_config_firmware_setting_info_t));
// 	rc = APP_CONFIG_firmware_setting_info_write(APP_CONFIG_FIRMWARE_SETTING_INFO_BACK, &firmware_setting_info2);
// 	APP_CHECK_RC(rc)
// }

// int APP_CONFIG_firmware_setting_info_overwrite1(void)
// {
// 	int rc = 0;
// 	rc = APP_CONFIG_firmware_setting_info_write(APP_CONFIG_FIRMWARE_SETTING_INFO_BACK, &firmware_setting_info2);
// 	APP_CHECK_RC(rc)
// }


int APP_CONFIG_firmware_setting_info_load(void)
{
	int rc = 0;

	APP_CONFIG_firmware_setting_info_init(&firmware_setting_info1);
	APP_CONFIG_firmware_setting_info_init(&firmware_setting_info2);
	rc = APP_CONFIG_firmware_setting_info_read(APP_CONFIG_FIRMWARE_SETTING_INFO, &firmware_setting_info1);
	APP_CHECK_RC(rc)
	rc = APP_CONFIG_firmware_setting_info_read(APP_CONFIG_FIRMWARE_SETTING_INFO_BACK, &firmware_setting_info2);
	APP_CHECK_RC(rc)

	return APP_OK;
}

int APP_CONFIG_firmware_setting_info1_clear_result(void)
{
	int rc = 0;

	firmware_setting_info1.reserved[0] = 0;
	rc = APP_CONFIG_firmware_setting_info_write(APP_CONFIG_FIRMWARE_SETTING_INFO, &firmware_setting_info1);
	APP_CHECK_RC(rc)

	return APP_OK;
}


int APP_CONFIG_firmware_setting_info2_clear_result(void)
{
	int rc = 0;
	firmware_setting_info2.reserved[0] = 0;
	rc = APP_CONFIG_firmware_setting_info_write(APP_CONFIG_FIRMWARE_SETTING_INFO_BACK, &firmware_setting_info2);
	APP_CHECK_RC(rc)

	return APP_OK;
}

int APP_CONFIG_firmware_setting_info1_recovery(void)
{
	int rc = 0;
	
	firmware_setting_info1.download_len = 0;
	firmware_setting_info1.reserved[0] = 0;
	firmware_setting_info1.reserved[1] = 0;
	firmware_setting_info1.reserved[2] = 0;
	firmware_setting_info1.reserved[3] = 0;
	firmware_setting_info1.firmware_info.device_type = 1;
	firmware_setting_info1.firmware_info.version_h = APP_FIRMWARE_MAJOR_DEFAULT;
	firmware_setting_info1.firmware_info.version_m = APP_FIRMWARE_MINOR_DEFAULT;	
	firmware_setting_info1.firmware_info.version_l = APP_FIRMWARE_REVISION_DEFAULT;
	firmware_setting_info1.firmware_info.file_len = (0x20000);
	firmware_setting_info1.firmware_info.file_crc = DEFAULT_CRC32_VALUE;
	
	rc = APP_CONFIG_firmware_setting_info_write(APP_CONFIG_FIRMWARE_SETTING_INFO, &firmware_setting_info1);
	APP_CHECK_RC(rc)

	return APP_OK;
}


int APP_CONFIG_firmware_setting_info2_recovery(void)
{
	int rc = 0;
	
	firmware_setting_info2.download_len = 0;
	firmware_setting_info2.reserved[0] = 0;
	firmware_setting_info2.reserved[1] = 0;
	firmware_setting_info2.reserved[2] = 0;
	firmware_setting_info2.reserved[3] = 0;
	firmware_setting_info2.firmware_info.device_type = 1;
	firmware_setting_info2.firmware_info.version_h = APP_FIRMWARE_MAJOR_DEFAULT;
	firmware_setting_info2.firmware_info.version_m = APP_FIRMWARE_MINOR_DEFAULT;	
	firmware_setting_info2.firmware_info.version_l = APP_FIRMWARE_REVISION_DEFAULT;
	firmware_setting_info2.firmware_info.file_len = (0x20000);
	firmware_setting_info2.firmware_info.file_crc = DEFAULT_CRC32_VALUE;
	
	rc = APP_CONFIG_firmware_setting_info_write(APP_CONFIG_FIRMWARE_SETTING_INFO_BACK, &firmware_setting_info2);
	APP_CHECK_RC(rc)

	return APP_OK;
}


int APP_CONFIG_firmware_setting_info_recovery(void)
{
	int rc = 0;
	
	firmware_setting_info2.download_len = 0;
	firmware_setting_info2.reserved[0] = 0;
	firmware_setting_info2.reserved[1] = 0;
	firmware_setting_info2.reserved[2] = 0;
	firmware_setting_info2.reserved[3] = 0;
	firmware_setting_info2.firmware_info.file_crc = DEFAULT_CRC32_VALUE;
	firmware_setting_info2.firmware_info.file_len = (0x20000);
	firmware_setting_info2.firmware_info.device_type = 1;
	firmware_setting_info2.firmware_info.version_h = APP_FIRMWARE_MAJOR_DEFAULT;
	firmware_setting_info2.firmware_info.version_m = APP_FIRMWARE_MINOR_DEFAULT;	
	firmware_setting_info2.firmware_info.version_l = APP_FIRMWARE_REVISION_DEFAULT;
	memcpy((uint8_t *)&firmware_setting_info1, (uint8_t *)&firmware_setting_info2, sizeof(app_config_firmware_setting_info_t));
	rc = APP_CONFIG_firmware_setting_info_write(APP_CONFIG_FIRMWARE_SETTING_INFO, &firmware_setting_info1);
	APP_CHECK_RC(rc)
	rc = APP_CONFIG_firmware_setting_info_write(APP_CONFIG_FIRMWARE_SETTING_INFO_BACK, &firmware_setting_info2);
	APP_CHECK_RC(rc)

	return APP_OK;
}

/* 继电器通道接线、功率、故障使能配置相关 */
app_relay_power_config_t app_relay_power_config[RELAY_SIZE][RELAY_CHANNEL_SIZE];
static int _relay_power_config_deserialize(uint8_t *bytes, app_relay_power_config_pt message)
{
	message->enable = read_uint8_t(&bytes[APP_CONFIG_EEPROM_RELAY_POWER_CONFIG_ENABLE_OFFSET]); 
	message->phase_config = read_uint8_t(&bytes[APP_CONFIG_EEPROM_RELAY_POWER_CONFIG_PHASE_OFFSET]); 
	message->power = read_uint8_t(&bytes[APP_CONFIG_EEPROM_RELAY_POWER_CONFIG_POWER_OFFSET]); 
	message->loss_phase_protect_enable = read_uint8_t(&bytes[APP_CONFIG_EEPROM_RELAY_POWER_CONFIG_LOSSPHASE_PROTECT_ENABLE_OFFSET]); 
	message->overload_protect_enable = read_uint8_t(&bytes[APP_CONFIG_EEPROM_RELAY_POWER_CONFIG_OVERLOAD_PROTECT_ENABLE_OFFSET]);
	message->ouv_protect_enable = read_uint8_t(&bytes[APP_CONFIG_EEPROM_RELAY_POWER_CONFIG_OUV_PROTECT_ENABLE_OFFSET]); 
	message->loader_type = read_uint8_t(&bytes[APP_CONFIG_EEPROM_RELAY_POWER_CONFIG_LOADER_TYPE]); 
	return APP_OK;
}

static int _relay_power_config_serialize(uint8_t *bytes, app_relay_power_config_pt message)
{
	write_uint8_t(message->enable, &bytes[APP_CONFIG_EEPROM_RELAY_POWER_CONFIG_ENABLE_OFFSET]); 
	write_uint8_t(message->phase_config, &bytes[APP_CONFIG_EEPROM_RELAY_POWER_CONFIG_PHASE_OFFSET]); 
	write_uint8_t(message->power, &bytes[APP_CONFIG_EEPROM_RELAY_POWER_CONFIG_POWER_OFFSET]); 
	write_uint8_t(message->loss_phase_protect_enable, &bytes[APP_CONFIG_EEPROM_RELAY_POWER_CONFIG_LOSSPHASE_PROTECT_ENABLE_OFFSET]); 
	write_uint8_t(message->overload_protect_enable, &bytes[APP_CONFIG_EEPROM_RELAY_POWER_CONFIG_OVERLOAD_PROTECT_ENABLE_OFFSET]); 
	write_uint8_t(message->ouv_protect_enable, &bytes[APP_CONFIG_EEPROM_RELAY_POWER_CONFIG_OUV_PROTECT_ENABLE_OFFSET]); 
	write_uint8_t(message->loader_type, &bytes[APP_CONFIG_EEPROM_RELAY_POWER_CONFIG_LOADER_TYPE]); 

	return APP_OK;
}

int APP_CONFIG_relay_power_config_init(uint8_t channel_id, app_relay_power_config_pt message)
{
    message->enable = ENABLE;
    if(4 != channel_id)
    {
        message->phase_config = APP_RELAY_PHASE_3V3;
        message->power = 30;
    }
	else
    {
        message->phase_config = APP_RELAY_PHASE_1V1;
        message->power = 10;
    }
	message->loss_phase_protect_enable = APP_RELAY_PROTECT_DISABLE;
	message->overload_protect_enable = APP_RELAY_PROTECT_DISABLE;
	message->ouv_protect_enable = APP_RELAY_PROTECT_DISABLE;
    message->loader_type = APP_RELAY_LOADER_TYPE_AERATOR;
	
	return APP_OK;
}


int APP_CONFIG_relay_power_config_write(uint8_t channel_id, app_relay_power_config_pt message)
{
	uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};

	if(channel_id > RELAY_CHANNEL_SIZE)
    {
        return APP_ERROR;
    }
    _relay_power_config_serialize(bytes, message);
	int rc = epprom_write_block(APP_CONFIG_EEPROM_RELAY_POWER_CONFIG_PAGE + channel_id, bytes);
    APP_CHECK_RC(rc)
    return APP_OK;
}


int APP_CONFIG_relay_power_config_read(uint8_t channel_id, app_relay_power_config_pt message)
{
	uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};

    if(channel_id > RELAY_CHANNEL_SIZE)
    {
        return APP_ERROR;
    }
	int rc = epprom_read_block(APP_CONFIG_EEPROM_RELAY_POWER_CONFIG_PAGE + channel_id, bytes);
    APP_CHECK_RC(rc)
    _relay_power_config_deserialize(bytes, message);
    return APP_OK;
}

int APP_CONFIG_relay_power_config_check(uint8_t channel_id, app_relay_power_config_pt message)
{
    if(ENABLE != message->enable)
    {
        APP_CONFIG_relay_power_config_init(channel_id, message);
        APP_CONFIG_relay_power_config_write(channel_id, message);
        return APP_ERROR;
    }
#if 0
    if((APP_RELAY_PHASE_1V1 != message->phase_config) && (APP_RELAY_PHASE_3V3 != message->phase_config))   message->phase_config = APP_RELAY_PHASE_UNKNOW;
    if(APP_RELAY_PROTECT_ENABLE != message->loss_phase_protect_enable)  message->loss_phase_protect_enable = APP_RELAY_PROTECT_DISABLE;
    if(APP_RELAY_PROTECT_ENABLE != message->overload_protect_enable)  message->overload_protect_enable = APP_RELAY_PROTECT_DISABLE;
    if(APP_RELAY_PROTECT_ENABLE != message->ouv_protect_enable)  message->ouv_protect_enable = APP_RELAY_PROTECT_DISABLE;
#endif
}

int APP_CONFIG_relay_power_config_load(void)
{
	int rc = APP_OK;
    uint8_t channel_id;
    app_relay_power_config_pt pconfig = NULL;
    
    for(channel_id = 0; channel_id < RELAY_CHANNEL_SIZE; channel_id++) 
    {
        pconfig = &app_relay_power_config[0][channel_id];
        rc = APP_CONFIG_relay_power_config_read(channel_id, pconfig);
        APP_CONFIG_relay_power_config_check(channel_id, pconfig);
        APP_CHECK_RC(rc)
    }
    return APP_OK;
}

int APP_CONFIG_relay_power_config_recovery(void)
{
    int rc = 0;
    uint8_t channel_id;
    app_relay_power_config_pt pconfig = NULL;

    for(channel_id = 0; channel_id < RELAY_CHANNEL_SIZE; channel_id++) 
    {
        pconfig = &app_relay_power_config[0][channel_id];
        APP_CONFIG_relay_power_config_init(channel_id, pconfig);
        rc = APP_CONFIG_relay_power_config_write(channel_id, pconfig);
        APP_CHECK_RC(rc)
    }
    return APP_OK;
}

app_config_diagnosis_message_t diagnosis_msg;

int APP_CONFIG_diagnosis_message_init(app_config_diagnosis_message_pt message)
{
    message->msg_time.year = 0;
    message->msg_time.month = 1;
    message->msg_time.day = 1;
    message->msg_time.hour = 0;
    message->msg_time.minute = 0;
    message->msg_time.second = 0;
    message->msg_type = 0;
    message->msg_status = 0;
}

int APP_CONFIG_diagnosis_message_read(app_config_diagnosis_message_pt message)
{
	uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};
    int rc = epprom_read_block(APP_CONFIG_EEPROM_DIAGNOSIS_MESSAGE_PAGE, bytes);
    APP_CHECK_RC(rc)
    message->msg_time.year = read_uint8_t(&bytes[APP_CONFIG_EEPROM_DIAGNOSIS_TIME_YEAR_OFFSET]);
    message->msg_time.month = read_uint8_t(&bytes[APP_CONFIG_EEPROM_DIAGNOSIS_TIME_MONTH_OFFSET]);
    message->msg_time.day = read_uint8_t(&bytes[APP_CONFIG_EEPROM_DIAGNOSIS_TIME_DAY_OFFSET]);
    message->msg_time.hour = read_uint8_t(&bytes[APP_CONFIG_EEPROM_DIAGNOSIS_TIME_HOUR_OFFSET]);
    message->msg_time.minute = read_uint8_t(&bytes[APP_CONFIG_EEPROM_DIAGNOSIS_TIME_MINUTE_OFFSET]);
    message->msg_time.second = read_uint8_t(&bytes[APP_CONFIG_EEPROM_DIAGNOSIS_TIME_SECOND_OFFSET]);
    message->msg_type = read_uint8_t(&bytes[APP_CONFIG_EEPROM_DIAGNOSIS_TYPE]);
    message->msg_status = read_uint8_t(&bytes[APP_CONFIG_EEPROM_DIAGNOSIS_STATUS]);
    return APP_OK;
}

int APP_CONFIG_diagnosis_message_write(app_config_diagnosis_message_pt message)
{
	uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};
    write_uint8_t(message->msg_time.year,  &bytes[APP_CONFIG_EEPROM_DIAGNOSIS_TIME_YEAR_OFFSET]);
    write_uint8_t(message->msg_time.month,  &bytes[APP_CONFIG_EEPROM_DIAGNOSIS_TIME_MONTH_OFFSET]);
    write_uint8_t(message->msg_time.day,  &bytes[APP_CONFIG_EEPROM_DIAGNOSIS_TIME_DAY_OFFSET]);
    write_uint8_t(message->msg_time.hour,  &bytes[APP_CONFIG_EEPROM_DIAGNOSIS_TIME_HOUR_OFFSET]);
    write_uint8_t(message->msg_time.minute,  &bytes[APP_CONFIG_EEPROM_DIAGNOSIS_TIME_MINUTE_OFFSET]);
    write_uint8_t(message->msg_time.second,  &bytes[APP_CONFIG_EEPROM_DIAGNOSIS_TIME_SECOND_OFFSET]);
    write_uint8_t(message->msg_type,  &bytes[APP_CONFIG_EEPROM_DIAGNOSIS_TYPE]);
    write_uint8_t(message->msg_status,  &bytes[APP_CONFIG_EEPROM_DIAGNOSIS_STATUS]);
    int rc = epprom_write_block(APP_CONFIG_EEPROM_DIAGNOSIS_MESSAGE_PAGE, bytes);
}

int APP_CONFIG_diagnosis_message_load(void)
{
    int rc = 0;
    APP_CONFIG_diagnosis_message_init(&diagnosis_msg);
    APP_CONFIG_diagnosis_message_read(&diagnosis_msg);
    if((0 == diagnosis_msg.msg_time.year) && (0 == diagnosis_msg.msg_time.month) && (0 == diagnosis_msg.msg_time.day))
    {   // 用于处理之前没有使用该BLOCK区，整片格式化时全部赋值为0的情况，重新将数据校正  
        APP_CONFIG_diagnosis_message_recovery(&diagnosis_msg);
    }
    return APP_OK;
}

int APP_CONFIG_diagnosis_message_recovery(void)
{
    int rc = 0;
    APP_CONFIG_diagnosis_message_init(&diagnosis_msg);
    APP_CONFIG_diagnosis_message_write(&diagnosis_msg);
    return APP_OK;
}

int APP_CONFIG_eeprom_load_all(void)
{
    int rc = 0;

    rc = eeprom_read_id();
    BSP_LOG_trace("eeprom_id read rc = %d\r\n", rc);
    APP_CHECK_RC(rc)

    rc = APP_CONFIG_system_load();
    BSP_LOG_trace("system load rc: %d\r\n", rc);
    APP_CHECK_RC(rc)
    rc = APP_CONFIG_device_enable_load();
    BSP_LOG_trace("device_enable load rc: %d\r\n", rc);
    APP_CHECK_RC(rc)
    rc = APP_CONFIG_device_connect_load();
    BSP_LOG_trace("device_connect load rc: %d\r\n", rc);
    APP_CHECK_RC(rc)
    rc = APP_CONFIG_probe_enable_load();
    BSP_LOG_trace("probe_enable load rc: %d\r\n", rc);
    APP_CHECK_RC(rc)
    rc = APP_CONFIG_pump_status_load();
    BSP_LOG_trace("pump_status load rc: %d\r\n", rc);
    APP_CHECK_RC(rc)
    rc = APP_CONFIG_sensor_load();
    BSP_LOG_trace("sensor load rc: %d\r\n", rc);
    APP_CHECK_RC(rc)
    rc = APP_CONFIG_compensation_load();
    BSP_LOG_trace("compensation load rc: %d\r\n", rc);
    APP_CHECK_RC(rc)
    rc = APP_CONFIG_indicator_alert_load();
    BSP_LOG_trace("indicator_alert load rc: %d\r\n", rc);
    APP_CHECK_RC(rc)
    BSP_WDG_feed();
    rc = APP_CONFIG_limit_load();
    BSP_LOG_trace("limit load rc: %d\r\n", rc);
    APP_CHECK_RC(rc)
    BSP_WDG_feed();
    rc = APP_CONFIG_relay_load();
    BSP_LOG_trace("relay load rc: %d\r\n", rc);
    APP_CHECK_RC(rc)
    rc = APP_CONFIG_times_enable_load();
    BSP_LOG_trace("times_enable load rc: %d\r\n", rc);
    APP_CHECK_RC(rc)
    rc = APP_CONFIG_times_load();
    BSP_LOG_trace("times load rc: %d\r\n", rc);
    APP_CHECK_RC(rc)
    rc = APP_CONFIG_manual_relay_load();
    BSP_LOG_trace("manual_relay load rc: %d\r\n", rc);
    APP_CHECK_RC(rc)
#ifdef SUPPORT_L6_ENABLE
    rc = APP_CONFIG_l6_load();
    BSP_LOG_trace("l6 load rc: %d\r\n", rc);
    APP_CHECK_RC(rc)
#endif
    rc = APP_CONFIG_relay_power_config_load();
    BSP_LOG_trace("relay power config load rc: %d\r\n", rc);
    APP_CHECK_RC(rc)
    rc = APP_CONFIG_diagnosis_message_load();
    BSP_LOG_trace("diagnosis message load rc: %d\r\n", rc);
    APP_CHECK_RC(rc)
    rc = APP_CONFIG_firmware_setting_info_load();
    BSP_LOG_trace("firmware info load rc: %d\r\n", rc);
    APP_CHECK_RC(rc)
    return APP_OK;
}

// SGM706 的看门狗复位周期为 1.6s，此函数耗时较长，需插入喂狗操作
int APP_CONFIG_eeprom_recovery_all(void)
{
    int rc = 0;

    rc = APP_CONFIG_device_enable_recovery();
    BSP_LOG_trace("device_enable recovery rc: %d\r\n", rc);
    APP_CHECK_RC(rc)
    rc = APP_CONFIG_device_connect_recovery();
    BSP_LOG_trace("device_connect recovery rc: %d\r\n", rc);
    APP_CHECK_RC(rc)
    rc = APP_CONFIG_probe_enable_recovery();
    BSP_LOG_trace("probe_enable recovery rc: %d\r\n", rc);
    APP_CHECK_RC(rc)
    rc = APP_CONFIG_pump_status_recovery();
    BSP_LOG_trace("pump_status recovery rc: %d\r\n", rc);
    APP_CHECK_RC(rc)
    rc = APP_CONFIG_sensor_recovery();
    BSP_LOG_trace("sensor recovery rc: %d\r\n", rc);
    APP_CHECK_RC(rc)
    rc = APP_CONFIG_compensation_recovery();
    BSP_LOG_trace("compensation recovery rc: %d\r\n", rc);
    APP_CHECK_RC(rc)
    rc = APP_CONFIG_indicator_alert_recovery();
    BSP_LOG_trace("indicator_alert recovery rc: %d\r\n", rc);
    APP_CHECK_RC(rc)
    BSP_WDG_feed();
    rc = APP_CONFIG_limit_recovery();
    BSP_LOG_trace("limit recovery rc: %d\r\n", rc);
    APP_CHECK_RC(rc)
    BSP_WDG_feed();
    rc = APP_CONFIG_relay_recovery();
    BSP_LOG_trace("relay recovery rc: %d\r\n", rc);
    APP_CHECK_RC(rc)
    rc = APP_CONFIG_times_enable_recovery();
    BSP_LOG_trace("times_enable recovery rc: %d\r\n", rc);
    APP_CHECK_RC(rc)
    rc = APP_CONFIG_times_recovery();
    BSP_LOG_trace("times recovery rc: %d\r\n", rc);
    APP_CHECK_RC(rc)
    rc = APP_CONFIG_manual_relay_recovery();
    BSP_LOG_trace("manual_relay recovery rc: %d\r\n", rc);
    APP_CHECK_RC(rc)
#ifdef SUPPORT_L6_ENABLE
    rc = APP_CONFIG_l6_recovery();
    BSP_LOG_trace("l6 recovery rc: %d\r\n", rc);
    APP_CHECK_RC(rc)
#endif
    rc = APP_CONFIG_relay_power_config_recovery();
    BSP_LOG_trace("relay power config rc: %d\r\n", rc);
    APP_CHECK_RC(rc)
    rc = APP_CONFIG_diagnosis_message_recovery();
    BSP_LOG_trace("diagnosis message recovery rc: %d\r\n", rc);
    APP_CHECK_RC(rc)
    rc = APP_CONFIG_firmware_setting_info_recovery();
    BSP_LOG_trace("firmware info recovery rc: %d\r\n", rc);
    APP_CHECK_RC(rc)
    return APP_OK;
}


int APP_CONFIG_eeprom_recovery_free_page(void)
{
  uint32_t page = 0;
  uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};
  int rc = 0;

  for(page=0; page<APP_CONFIG_EEPROM_SYSTEM_PAGE; page++)
  {
    if(0 == (page & 0xF)) // every 16 pages, feed wdg 
    {
      BSP_WDG_feed();
    }
    rc = epprom_write_block(page, bytes);
    APP_CHECK_RC(rc)
  }
  BSP_WDG_feed();
  for(page=APP_CONFIG_EEPROM_DEVICE_CONNECT_PAGE+1; page<APP_CONFIG_EEPROM_PROBE_ENABLE_PAGE; page++)
  {
    if(0 == (page & 0xF)) // every 16 pages, feed wdg 
    {
      BSP_WDG_feed();
    }
    rc = epprom_write_block(page, bytes);
    APP_CHECK_RC(rc)
  }
  BSP_WDG_feed();
  for(page=APP_CONFIG_EEPROM_PROBE_ENABLE_PAGE+PROBE_SIZE; page<APP_CONFIG_EEPROM_PUMP_STATUS_PAGE; page++)
  {
    if(0 == (page & 0xF)) // every 16 pages, feed wdg 
    {
      BSP_WDG_feed();
    }
    rc = epprom_write_block(page, bytes);
    APP_CHECK_RC(rc)
  }
  BSP_WDG_feed();
  for(page=APP_CONFIG_EEPROM_PUMP_STATUS_PAGE+PROBE_SIZE; page<APP_CONFIG_EEPROM_SENSOR_PAGE; page++)
  {
    if(0 == (page & 0xF)) // every 16 pages, feed wdg 
    {
      BSP_WDG_feed();
    }
    rc = epprom_write_block(page, bytes);
    APP_CHECK_RC(rc)
  }
  BSP_WDG_feed();
  for(page=APP_CONFIG_EEPROM_SENSOR_PAGE+(PROBE_SIZE*PROBE_SENSOR_SIZE); page<APP_CONFIG_EEPROM_COMPENSATION_PAGE; page++)
  {
    if(0 == (page & 0xF)) // every 16 pages, feed wdg 
    {
      BSP_WDG_feed();
    }
    rc = epprom_write_block(page, bytes);
    APP_CHECK_RC(rc)
  }
  BSP_WDG_feed();
  for(page=APP_CONFIG_EEPROM_COMPENSATION_PAGE+(PROBE_SIZE*INDICATOR_SIZE); page<APP_CONFIG_EEPROM_INDICATOR_ALERT_PAGE; page++)
  {
    if(0 == (page & 0xF)) // every 16 pages, feed wdg 
    {
      BSP_WDG_feed();
    }
    rc = epprom_write_block(page, bytes);
    APP_CHECK_RC(rc)
  }
  BSP_WDG_feed();
  for(page=APP_CONFIG_EEPROM_INDICATOR_ALERT_PAGE+(PROBE_SIZE*INDICATOR_SIZE); page<APP_CONFIG_EEPROM_LIMIT_PAGE; page++)
  {
    if(0 == (page & 0xF)) // every 16 pages, feed wdg 
    {
      BSP_WDG_feed();
    }
    rc = epprom_write_block(page, bytes);
    APP_CHECK_RC(rc)
  }
  BSP_WDG_feed();
  for(page=APP_CONFIG_EEPROM_LIMIT_PAGE+(PROBE_SIZE*INDICATOR_SIZE*APP_CONFIG_MAX_LIMIT_TASK); page<APP_CONFIG_EEPROM_RELAY_PAGE; page++)
  {
    if(0 == (page & 0xF)) // every 16 pages, feed wdg 
    {
      BSP_WDG_feed();
    }
    rc = epprom_write_block(page, bytes);
    APP_CHECK_RC(rc)
  }
  BSP_WDG_feed();
  for(page=APP_CONFIG_EEPROM_RELAY_PAGE+RELAY_SIZE; page<APP_CONFIG_EEPROM_TIMES_ENABLE_PAGE; page++)
  {
    if(0 == (page & 0xF)) // every 16 pages, feed wdg 
    {
      BSP_WDG_feed();
    }
    rc = epprom_write_block(page, bytes);
    APP_CHECK_RC(rc)
  }
  BSP_WDG_feed();
  for(page=APP_CONFIG_EEPROM_TIMES_ENABLE_PAGE+RELAY_SIZE; page<APP_CONFIG_EEPROM_TIMES_PAGE; page++)
  {
    if(0 == (page & 0xF)) // every 16 pages, feed wdg 
    {
      BSP_WDG_feed();
    }
    rc = epprom_write_block(page, bytes);
    APP_CHECK_RC(rc)
  }
  BSP_WDG_feed();
  for(page=APP_CONFIG_EEPROM_TIMES_PAGE+(RELAY_SIZE*APP_CONFIG_MAX_TIMES_TASK); page<APP_CONFIG_EEPROM_MANUAL_RELAY_PAGE; page++)
  {
    if(0 == (page & 0xF)) // every 16 pages, feed wdg 
    {
      BSP_WDG_feed();
    }
    rc = epprom_write_block(page, bytes);
    APP_CHECK_RC(rc)
  }
  BSP_WDG_feed();
  for(page=APP_CONFIG_EEPROM_MANUAL_RELAY_PAGE+(RELAY_SIZE*RELAY_CHANNEL_SIZE); page<APP_CONFIG_EEPROM_L6_PAGE; page++)
  {
    if(0 == (page & 0xF)) // every 16 pages, feed wdg 
    {
      BSP_WDG_feed();
    }
    rc = epprom_write_block(page, bytes);
    APP_CHECK_RC(rc)
  }
  BSP_WDG_feed();
  for(page=APP_CONFIG_EEPROM_L6_PAGE+PROBE_SIZE; page<APP_CONFIG_EEPROM_RELAY_POWER_CONFIG_PAGE; page++)
  {
    if(0 == (page & 0xF)) // every 16 pages, feed wdg 
    {
      BSP_WDG_feed();
    }
    rc = epprom_write_block(page, bytes);
    APP_CHECK_RC(rc)
  }
  BSP_WDG_feed();
  for(page=APP_CONFIG_EEPROM_RELAY_POWER_CONFIG_PAGE+(RELAY_SIZE*RELAY_CHANNEL_SIZE); page<APP_CONFIG_EEPROM_DIAGNOSIS_MESSAGE_PAGE; page++)
  {
    if(0 == (page & 0xF)) // every 16 pages, feed wdg 
    {
      BSP_WDG_feed();
    }
    rc = epprom_write_block(page, bytes);
    APP_CHECK_RC(rc)
  }
  BSP_WDG_feed();
  for(page=APP_CONFIG_EEPROM_DIAGNOSIS_MESSAGE_PAGE+1; page<APP_CONFIG_EEPROM_FIRMWARE_SETTING_INFO_PAGE; page++)
  {
    if(0 == (page & 0xF)) // every 16 pages, feed wdg 
    {
      BSP_WDG_feed();
    }
    rc = epprom_write_block(page, bytes);
    APP_CHECK_RC(rc)
  }
  BSP_WDG_feed();
  
  BSP_LOG_debug("free page recovery rc: %d\r\n", rc);
  return APP_OK;
}

#ifdef __cplusplus
}
#endif
#endif
