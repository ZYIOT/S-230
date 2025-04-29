#ifndef __APP_CONFIG_EEPROM_MESSAGE_H
#define __APP_CONFIG_EEPROM_MESSAGE_H
#ifdef __cplusplus
extern "C"
{
#endif

int APP_CONFIG_SystemInit(APP_CONFIG_System_pt message)
{
    message->deviceID = 1;
    memset(message->SN, 0, 5);
    memset(message->hardware, 0, 3);
    message->hardware[0]= APP_HARDWARE_MAJOR;
    message->hardware[1]= APP_HARDWARE_MINOR;
    message->hardware[2]= APP_HARDWARE_REVISION;
    message->logLevel = BSP_LOG_DEFAULT_LEVEL; 
    return APP_OK;
}

static void SystemSerialize(uint8_t bytes[EEPROM_BLOCK_SIZE], APP_CONFIG_System_pt message)
{
    write_uint32_t(message->deviceID, &bytes[APP_CONFIG_EEPROM_SYSTEM_DEVICE_ID_OFFSET]); 
    memcpy(&bytes[APP_CONFIG_EEPROM_SYSTEM_SN_OFFSET], message->SN, 5); 
    memcpy(&bytes[APP_CONFIG_EEPROM_SYSTEM_HARDWARE_OFFSET], message->hardware, 3); 
}

APP_CONFIG_System_t g_appConfigSystem;
int APP_CONFIG_SystemRead( APP_CONFIG_System_pt message)
{
    uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};
    
    int rc = EEPROM_ReadBlock(APP_CONFIG_EEPROM_SYSTEM_PAGE, bytes);
    APP_CHECK_RC(rc)
    message->deviceID = read_uint32_t(&bytes[APP_CONFIG_EEPROM_SYSTEM_DEVICE_ID_OFFSET]);
    memcpy(message->SN, &bytes[APP_CONFIG_EEPROM_SYSTEM_SN_OFFSET], 5);
    memcpy(message->hardware, &bytes[APP_CONFIG_EEPROM_SYSTEM_HARDWARE_OFFSET], 3);
    return APP_OK;
}

int APP_CONFIG_SystemWrite( APP_CONFIG_System_pt message)
{
    uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};
    
    SystemSerialize(bytes, message);
    return EEPROM_WriteBlock(APP_CONFIG_EEPROM_SYSTEM_PAGE, bytes);
}

int APP_CONFIG_SystemWriteDeviceID( APP_CONFIG_System_pt message, uint32_t deviceID)
{
    uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};
    
    SystemSerialize(bytes, message);
    write_uint32_t(deviceID, &bytes[APP_CONFIG_EEPROM_SYSTEM_DEVICE_ID_OFFSET]);
    int rc = EEPROM_WriteBlock(APP_CONFIG_EEPROM_SYSTEM_PAGE, bytes);
    APP_CHECK_RC(rc)
    message->deviceID = deviceID; 
    return APP_OK;
}

int APP_CONFIG_SystemWriteSN( APP_CONFIG_System_pt message, uint8_t SN[5], uint8_t hardware[3])
{
    uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};
    
    SystemSerialize(bytes, message);
    memcpy(&bytes[APP_CONFIG_EEPROM_SYSTEM_SN_OFFSET], SN, 5);
    memcpy(&bytes[APP_CONFIG_EEPROM_SYSTEM_HARDWARE_OFFSET], hardware, 3);
    int rc = EEPROM_WriteBlock(APP_CONFIG_EEPROM_SYSTEM_PAGE, bytes);
    APP_CHECK_RC(rc)
    memcpy(message->SN, SN, 5); 
    memcpy(message->hardware, hardware, 3); 
    return APP_OK;
}

int APP_CONFIG_SystemLoad(void)
{
    int rc = 0;

    APP_CONFIG_SystemInit(&g_appConfigSystem);
    rc = APP_CONFIG_SystemRead( &g_appConfigSystem);
    APP_CHECK_RC(rc)
    return APP_OK;
}

int APP_CONFIG_SystemRecovery(void)
{
    int rc = 0;
    
    APP_CONFIG_SystemInit(&g_appConfigSystem);
    rc = APP_CONFIG_SystemWrite( &g_appConfigSystem);
    APP_CHECK_RC(rc)
    return APP_OK;
}
int APP_CONFIG_TimeInit(APP_CONFIG_Time_pt message)
{
    message->year = 0;
    message->month = 0;
    message->day = 0;
    message->hour = 0;
    message->minute = 0;
    message->second = 0;
    return APP_OK;
}

static void TimeSerialize(uint8_t bytes[EEPROM_BLOCK_SIZE], APP_CONFIG_Time_pt message)
{
    write_uint8_t(message->year, &bytes[APP_CONFIG_EEPROM_TIME_YEAR_OFFSET]); 
    write_uint8_t(message->month, &bytes[APP_CONFIG_EEPROM_TIME_MONTH_OFFSET]); 
    write_uint8_t(message->day, &bytes[APP_CONFIG_EEPROM_TIME_DAY_OFFSET]); 
    write_uint8_t(message->hour, &bytes[APP_CONFIG_EEPROM_TIME_HOUR_OFFSET]); 
    write_uint8_t(message->minute, &bytes[APP_CONFIG_EEPROM_TIME_MINUTE_OFFSET]); 
    write_uint8_t(message->second, &bytes[APP_CONFIG_EEPROM_TIME_SECOND_OFFSET]); 
}

static int TimeDeserialize(uint8_t *bytes, APP_CONFIG_Time_pt message)
{
    message->year = read_uint8_t(&bytes[APP_CONFIG_EEPROM_TIME_YEAR_OFFSET]); 
    message->month = read_uint8_t(&bytes[APP_CONFIG_EEPROM_TIME_MONTH_OFFSET]); 
    message->day = read_uint8_t(&bytes[APP_CONFIG_EEPROM_TIME_DAY_OFFSET]); 
    message->hour = read_uint8_t(&bytes[APP_CONFIG_EEPROM_TIME_HOUR_OFFSET]); 
    message->minute = read_uint8_t(&bytes[APP_CONFIG_EEPROM_TIME_MINUTE_OFFSET]); 
    message->second = read_uint8_t(&bytes[APP_CONFIG_EEPROM_TIME_SECOND_OFFSET]); 
    return APP_OK;
}

static void TimeCopy( APP_CONFIG_Time_pt dist, APP_CONFIG_Time_pt from)
{
    dist->year = from->year; 
    dist->month = from->month; 
    dist->day = from->day; 
    dist->hour = from->hour; 
    dist->minute = from->minute; 
    dist->second = from->second; 
}

int APP_CONFIG_DeviceEnableInit(APP_CONFIG_DeviceEnable_pt message)
{
    message->enable = APP_ENABLE;
    APP_CONFIG_TimeInit(&message->recordErrorAt);
    return APP_OK;
}

static void DeviceEnableSerialize(uint8_t bytes[EEPROM_BLOCK_SIZE], APP_CONFIG_DeviceEnable_pt message)
{
    write_uint8_t(message->enable, &bytes[APP_CONFIG_EEPROM_DEVICE_ENABLE_ENABLE_OFFSET]); 
    TimeSerialize(&bytes[APP_CONFIG_EEPROM_DEVICE_ENABLE_RECORD_ERROR_AT_OFFSET], &message->recordErrorAt); 
}

APP_CONFIG_DeviceEnable_t g_appConfigDeviceEnable;
int APP_CONFIG_DeviceEnableRead( APP_CONFIG_DeviceEnable_pt message)
{
    uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};
    
    int rc = EEPROM_ReadBlock(APP_CONFIG_EEPROM_DEVICE_ENABLE_PAGE, bytes);
    APP_CHECK_RC(rc)
    message->enable = read_uint8_t(&bytes[APP_CONFIG_EEPROM_DEVICE_ENABLE_ENABLE_OFFSET]);
    TimeDeserialize(&bytes[APP_CONFIG_EEPROM_DEVICE_ENABLE_RECORD_ERROR_AT_OFFSET], &message->recordErrorAt);
    return APP_OK;
}

int APP_CONFIG_DeviceEnableWrite( APP_CONFIG_DeviceEnable_pt message)
{
    uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};
    
    DeviceEnableSerialize(bytes, message);
    return EEPROM_WriteBlock(APP_CONFIG_EEPROM_DEVICE_ENABLE_PAGE, bytes);
}

int APP_CONFIG_DeviceEnableWriteEnable( APP_CONFIG_DeviceEnable_pt message, uint8_t enable)
{
    uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};
    
    DeviceEnableSerialize(bytes, message);
    write_uint8_t(enable, &bytes[APP_CONFIG_EEPROM_DEVICE_ENABLE_ENABLE_OFFSET]);
    int rc = EEPROM_WriteBlock(APP_CONFIG_EEPROM_DEVICE_ENABLE_PAGE, bytes);
    APP_CHECK_RC(rc)
    message->enable = enable; 
    return APP_OK;
}

int APP_CONFIG_DeviceEnableWriteRecordErrorAt( APP_CONFIG_DeviceEnable_pt message, APP_CONFIG_Time_t recordErrorAt)
{
    uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};
    
    DeviceEnableSerialize(bytes, message);
    TimeSerialize(&bytes[APP_CONFIG_EEPROM_DEVICE_ENABLE_RECORD_ERROR_AT_OFFSET], &recordErrorAt);
    int rc = EEPROM_WriteBlock(APP_CONFIG_EEPROM_DEVICE_ENABLE_PAGE, bytes);
    APP_CHECK_RC(rc)
    TimeCopy(&message->recordErrorAt, &recordErrorAt); 
    return APP_OK;
}

int APP_CONFIG_DeviceEnableLoad(void)
{
    int rc = 0;

    APP_CONFIG_DeviceEnableInit(&g_appConfigDeviceEnable);
    rc = APP_CONFIG_DeviceEnableRead( &g_appConfigDeviceEnable);
    APP_CHECK_RC(rc)
    return APP_OK;
}

int APP_CONFIG_DeviceEnableRecovery(void)
{
    int rc = 0;

    APP_CONFIG_DeviceEnableInit(&g_appConfigDeviceEnable);
    rc = APP_CONFIG_DeviceEnableWrite( &g_appConfigDeviceEnable);
    APP_CHECK_RC(rc)
    return APP_OK;
}
int APP_CONFIG_DeviceConnectInit(APP_CONFIG_DeviceConnect_pt message)
{
    message->reason = 0;
    return APP_OK;
}

static void DeviceConnectSerialize(uint8_t bytes[EEPROM_BLOCK_SIZE], APP_CONFIG_DeviceConnect_pt message)
{
    write_uint8_t(message->reason, &bytes[APP_CONFIG_EEPROM_DEVICE_CONNECT_REASON_OFFSET]); 
}

APP_CONFIG_DeviceConnect_t g_appConfigDeviceConnect;
int APP_CONFIG_DeviceConnectRead( APP_CONFIG_DeviceConnect_pt message)
{
    uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};
    
    int rc = EEPROM_ReadBlock(APP_CONFIG_EEPROM_DEVICE_CONNECT_PAGE, bytes);
    APP_CHECK_RC(rc)
    message->reason = read_uint8_t(&bytes[APP_CONFIG_EEPROM_DEVICE_CONNECT_REASON_OFFSET]);
    return APP_OK;
}

int APP_CONFIG_DeviceConnectWrite( APP_CONFIG_DeviceConnect_pt message)
{
    uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};
    
    DeviceConnectSerialize(bytes, message);
    return EEPROM_WriteBlock(APP_CONFIG_EEPROM_DEVICE_CONNECT_PAGE, bytes);
}

int APP_CONFIG_DeviceConnectWriteReason( APP_CONFIG_DeviceConnect_pt message, uint8_t reason)
{
    uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};
    
    DeviceConnectSerialize(bytes, message);
    write_uint8_t(reason, &bytes[APP_CONFIG_EEPROM_DEVICE_CONNECT_REASON_OFFSET]);
    int rc = EEPROM_WriteBlock(APP_CONFIG_EEPROM_DEVICE_CONNECT_PAGE, bytes);
    APP_CHECK_RC(rc)
    message->reason = reason; 
    return APP_OK;
}

int APP_CONFIG_DeviceConnectLoad(void)
{
    int rc = 0;

    APP_CONFIG_DeviceConnectInit(&g_appConfigDeviceConnect);
    rc = APP_CONFIG_DeviceConnectRead( &g_appConfigDeviceConnect);
    APP_CHECK_RC(rc)
    return APP_OK;
}

int APP_CONFIG_DeviceConnectRecovery(void)
{
    int rc = 0;

    APP_CONFIG_DeviceConnectInit(&g_appConfigDeviceConnect);
    rc = APP_CONFIG_DeviceConnectWrite( &g_appConfigDeviceConnect);
    APP_CHECK_RC(rc)
    return APP_OK;
}
int APP_CONFIG_ProbeEnableInit(APP_CONFIG_ProbeEnable_pt message)
{
    message->hasConfig = 0;
    message->enable = 0;
    message->warning = 0;
    return APP_OK;
}

static void ProbeEnableSerialize(uint8_t bytes[EEPROM_BLOCK_SIZE], APP_CONFIG_ProbeEnable_pt message)
{
    write_uint8_t(message->hasConfig, &bytes[APP_CONFIG_EEPROM_PROBE_ENABLE_HAS_CONFIG_OFFSET]); 
    write_uint8_t(message->enable, &bytes[APP_CONFIG_EEPROM_PROBE_ENABLE_ENABLE_OFFSET]); 
    write_uint8_t(message->warning, &bytes[APP_CONFIG_EEPROM_PROBE_ENABLE_WARNING_OFFSET]); 
}

APP_CONFIG_ProbeEnable_t g_appConfigProbeEnable[PROBE_SIZE];
int APP_CONFIG_ProbeEnableRead(uint8_t probeID,  APP_CONFIG_ProbeEnable_pt message)
{
    uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};

    if((probeID > PROBE_SIZE)){return APP_ERROR;}
    int rc = EEPROM_ReadBlock(APP_CONFIG_EEPROM_PROBE_ENABLE_PAGE + probeID, bytes);
    APP_CHECK_RC(rc)
    message->hasConfig = read_uint8_t(&bytes[APP_CONFIG_EEPROM_PROBE_ENABLE_HAS_CONFIG_OFFSET]);
    message->enable = read_uint8_t(&bytes[APP_CONFIG_EEPROM_PROBE_ENABLE_ENABLE_OFFSET]);
    message->warning = read_uint8_t(&bytes[APP_CONFIG_EEPROM_PROBE_ENABLE_WARNING_OFFSET]);
    return APP_OK;
}

int APP_CONFIG_ProbeEnableWrite(uint8_t probeID,  APP_CONFIG_ProbeEnable_pt message)
{
    uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};

    if((probeID > PROBE_SIZE)){return APP_ERROR;}
    ProbeEnableSerialize(bytes, message);
    return EEPROM_WriteBlock(APP_CONFIG_EEPROM_PROBE_ENABLE_PAGE + probeID, bytes);
}

int APP_CONFIG_ProbeEnableWriteHasConfig(uint8_t probeID,  APP_CONFIG_ProbeEnable_pt message, uint8_t hasConfig)
{
    uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};

    if((probeID > PROBE_SIZE)){return APP_ERROR;}
    ProbeEnableSerialize(bytes, message);
    write_uint8_t(hasConfig, &bytes[APP_CONFIG_EEPROM_PROBE_ENABLE_HAS_CONFIG_OFFSET]);
    int rc = EEPROM_WriteBlock(APP_CONFIG_EEPROM_PROBE_ENABLE_PAGE + probeID, bytes);
    APP_CHECK_RC(rc)
    message->hasConfig = hasConfig; 
    return APP_OK;
}

int APP_CONFIG_ProbeEnableWriteEnable(uint8_t probeID,  APP_CONFIG_ProbeEnable_pt message, uint8_t enable)
{
    uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};

    if((probeID > PROBE_SIZE)){return APP_ERROR;}
    ProbeEnableSerialize(bytes, message);
    write_uint8_t(enable, &bytes[APP_CONFIG_EEPROM_PROBE_ENABLE_ENABLE_OFFSET]);
    int rc = EEPROM_WriteBlock(APP_CONFIG_EEPROM_PROBE_ENABLE_PAGE + probeID, bytes);
    APP_CHECK_RC(rc)
    message->enable = enable; 
    return APP_OK;
}

int APP_CONFIG_ProbeEnableWriteWarnning(uint8_t probeID,  APP_CONFIG_ProbeEnable_pt message, uint8_t warning)
{
    uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};

    if((probeID > PROBE_SIZE)){return APP_ERROR;}
    ProbeEnableSerialize(bytes, message);
    write_uint8_t(warning, &bytes[APP_CONFIG_EEPROM_PROBE_ENABLE_WARNING_OFFSET]);
    int rc = EEPROM_WriteBlock(APP_CONFIG_EEPROM_PROBE_ENABLE_PAGE + probeID, bytes);
    APP_CHECK_RC(rc)
    message->warning = warning; 
    return APP_OK;
}

int APP_CONFIG_ProbeEnableLoad(void)
{
    int rc = 0;
    uint8_t probeID;

    for(probeID = 0; probeID < PROBE_SIZE; probeID++ ) 
    {
        APP_CONFIG_ProbeEnableInit(&g_appConfigProbeEnable[probeID]);
        rc = APP_CONFIG_ProbeEnableRead(probeID, &g_appConfigProbeEnable[probeID]);
        APP_CHECK_RC(rc)
    } 
    return APP_OK;
}

int APP_CONFIG_ProbeEnableRecovery(void)
{
    int rc = 0;
    uint8_t probeID;

    for(probeID = 0; probeID < PROBE_SIZE; probeID++ ) 
    {
        APP_CONFIG_ProbeEnableInit(&g_appConfigProbeEnable[probeID]);
        rc = APP_CONFIG_ProbeEnableWrite(probeID, &g_appConfigProbeEnable[probeID]);
        APP_CHECK_RC(rc)
    } 
    return APP_OK;
}

int APP_CONFIG_PumpStatusInit(APP_CONFIG_PumpStatus_pt message)
{
    memset(message->status, 0, 16);
    return APP_OK;
}

static void PumpStatusSerialize(uint8_t bytes[EEPROM_BLOCK_SIZE], APP_CONFIG_PumpStatus_pt message)
{
    memcpy(&bytes[APP_CONFIG_EEPROM_PUMP_STATUS_STATUS_OFFSET], message->status, 16); 
}

APP_CONFIG_PumpStatus_t g_appConfigPumpStatus[PROBE_SIZE];
int APP_CONFIG_PumpStatusRead(uint8_t probeID,  APP_CONFIG_PumpStatus_pt message)
{
    uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};

    if((probeID > PROBE_SIZE)){return APP_ERROR;}
    int rc = EEPROM_ReadBlock(APP_CONFIG_EEPROM_PUMP_STATUS_PAGE + probeID, bytes);
    APP_CHECK_RC(rc)
    memcpy(message->status, &bytes[APP_CONFIG_EEPROM_PUMP_STATUS_STATUS_OFFSET], 16);
    return APP_OK;
}

int APP_CONFIG_PumpStatusWrite(uint8_t probeID,  APP_CONFIG_PumpStatus_pt message)
{
    uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};

    if((probeID > PROBE_SIZE)){return APP_ERROR;}
    PumpStatusSerialize(bytes, message);
    return EEPROM_WriteBlock(APP_CONFIG_EEPROM_PUMP_STATUS_PAGE + probeID, bytes);
}

int APP_CONFIG_PumpStatusWriteStatus(uint8_t probeID,  APP_CONFIG_PumpStatus_pt message, uint8_t status_index, uint8_t status)
{
    uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};

    if((probeID > PROBE_SIZE)){return APP_ERROR;}
    PumpStatusSerialize(bytes, message);
    write_uint8_t(status, &bytes[APP_CONFIG_EEPROM_PUMP_STATUS_STATUS_OFFSET + 1 * status_index]);
    int rc = EEPROM_WriteBlock(APP_CONFIG_EEPROM_PUMP_STATUS_PAGE + probeID, bytes);
    APP_CHECK_RC(rc)
    message->status[status_index] = status; 
    return APP_OK;
}

int APP_CONFIG_PumpStatusLoad(void)
{
    int rc = 0;
    uint8_t probeID;

    for(probeID = 0; probeID < PROBE_SIZE; probeID++ ) 
    {
        APP_CONFIG_PumpStatusInit(&g_appConfigPumpStatus[probeID]);
        rc = APP_CONFIG_PumpStatusRead(probeID, &g_appConfigPumpStatus[probeID]);
        APP_CHECK_RC(rc)
    } 
    return APP_OK;
}

int APP_CONFIG_PumpStatusRecovery(void)
{
    int rc = 0;
    uint8_t probeID;

    for(probeID = 0; probeID < PROBE_SIZE; probeID++ ) 
    {
        APP_CONFIG_PumpStatusInit(&g_appConfigPumpStatus[probeID]);
        rc = APP_CONFIG_PumpStatusWrite(probeID, &g_appConfigPumpStatus[probeID]);
        APP_CHECK_RC(rc)
    } 
    return APP_OK;
}

int APP_CONFIG_SensorInit(APP_CONFIG_Sensor_pt message)
{
    message->hasConfig = 0;
    message->enable = 0;
    message->warning = 0;
    message->manufacturer = 0;
    message->model = 0;
    message->protocol = 0;
    return APP_OK;
}

static void SensorSerialize(uint8_t bytes[EEPROM_BLOCK_SIZE], APP_CONFIG_Sensor_pt message)
{
    write_uint8_t(message->hasConfig, &bytes[APP_CONFIG_EEPROM_SENSOR_HAS_CONFIG_OFFSET]); 
    write_uint8_t(message->enable, &bytes[APP_CONFIG_EEPROM_SENSOR_ENABLE_OFFSET]); 
    write_uint8_t(message->warning, &bytes[APP_CONFIG_EEPROM_SENSOR_WARNING_OFFSET]); 
    write_uint8_t(message->manufacturer, &bytes[APP_CONFIG_EEPROM_SENSOR_MANUFACTURER_OFFSET]); 
    write_uint8_t(message->model, &bytes[APP_CONFIG_EEPROM_SENSOR_MODEL_OFFSET]); 
    write_uint8_t(message->protocol, &bytes[APP_CONFIG_EEPROM_SENSOR_PROTOCOL_OFFSET]); 
}

APP_CONFIG_Sensor_t g_appConfigSensor[PROBE_SIZE][PROBE_SENSOR_SIZE];
int APP_CONFIG_SensorRead(uint8_t probeID, uint8_t sensorID,  APP_CONFIG_Sensor_pt message)
{
    uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};

    if((probeID > PROBE_SIZE) || (sensorID > PROBE_SENSOR_SIZE)){return APP_ERROR;}
    int rc = EEPROM_ReadBlock(APP_CONFIG_EEPROM_SENSOR_PAGE + (probeID * 16) + sensorID, bytes);
    APP_CHECK_RC(rc)
    message->hasConfig = read_uint8_t(&bytes[APP_CONFIG_EEPROM_SENSOR_HAS_CONFIG_OFFSET]);
    message->enable = read_uint8_t(&bytes[APP_CONFIG_EEPROM_SENSOR_ENABLE_OFFSET]);
    message->warning = read_uint8_t(&bytes[APP_CONFIG_EEPROM_SENSOR_WARNING_OFFSET]);
    message->manufacturer = read_uint8_t(&bytes[APP_CONFIG_EEPROM_SENSOR_MANUFACTURER_OFFSET]);
    message->model = read_uint8_t(&bytes[APP_CONFIG_EEPROM_SENSOR_MODEL_OFFSET]);
    message->protocol = read_uint8_t(&bytes[APP_CONFIG_EEPROM_SENSOR_PROTOCOL_OFFSET]);
    return APP_OK;
}

int APP_CONFIG_SensorWrite(uint8_t probeID, uint8_t sensorID,  APP_CONFIG_Sensor_pt message)
{
    uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};

    if((probeID > PROBE_SIZE) || (sensorID > PROBE_SENSOR_SIZE)){return APP_ERROR;}
    SensorSerialize(bytes, message);
    return EEPROM_WriteBlock(APP_CONFIG_EEPROM_SENSOR_PAGE + (probeID * 16) + sensorID, bytes);
}

int APP_CONFIG_SensorLoad(void)
{
    int rc = 0;
    uint8_t probeID;
    uint8_t sensorID;

    for(probeID = 0; probeID < PROBE_SIZE; probeID++ ) 
    {
        for(sensorID = 0; sensorID < PROBE_SENSOR_SIZE; sensorID++ ) 
        {
            APP_CONFIG_SensorInit(&g_appConfigSensor[probeID][sensorID]);
            rc = APP_CONFIG_SensorRead(probeID,sensorID, &g_appConfigSensor[probeID][sensorID]);
            APP_CHECK_RC(rc)
        } 
    } 
    return APP_OK;
}

int APP_CONFIG_SensorRecovery(void)
{
    int rc = 0;
    uint8_t probeID;
    uint8_t sensorID;

    for(probeID = 0; probeID < PROBE_SIZE; probeID++ ) 
    {
        for(sensorID = 0; sensorID < PROBE_SENSOR_SIZE; sensorID++ ) 
        {
            APP_CONFIG_SensorInit(&g_appConfigSensor[probeID][sensorID]);
            rc = APP_CONFIG_SensorWrite(probeID,sensorID, &g_appConfigSensor[probeID][sensorID]);
            APP_CHECK_RC(rc)
        } 
    } 
    return APP_OK;
}
int APP_CONFIG_CompensationInit(APP_CONFIG_Compensation_pt message)
{
    message->hasConfig = 0;
    message->value = 0;
    return APP_OK;
}

static void CompensationSerialize(uint8_t bytes[EEPROM_BLOCK_SIZE], APP_CONFIG_Compensation_pt message)
{
    write_uint8_t(message->hasConfig, &bytes[APP_CONFIG_EEPROM_COMPENSATION_HAS_CONFIG_OFFSET]); 
    write_uint16_t(message->value, &bytes[APP_CONFIG_EEPROM_COMPENSATION_VALUE_OFFSET]); 
}

APP_CONFIG_Compensation_t g_appConfigCompensation[PROBE_SIZE][INDICATOR_SIZE];
int APP_CONFIG_CompensationRead(uint8_t probeID, uint8_t indicatorID,  APP_CONFIG_Compensation_pt message)
{
    uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};

    if((probeID > PROBE_SIZE) || (indicatorID > INDICATOR_SIZE)){return APP_ERROR;}
    int rc = EEPROM_ReadBlock(APP_CONFIG_EEPROM_COMPENSATION_PAGE + (probeID * 16) + indicatorID, bytes);
    APP_CHECK_RC(rc)
    message->hasConfig = read_uint8_t(&bytes[APP_CONFIG_EEPROM_COMPENSATION_HAS_CONFIG_OFFSET]);
    message->value = read_uint16_t(&bytes[APP_CONFIG_EEPROM_COMPENSATION_VALUE_OFFSET]);
    return APP_OK;
}

int APP_CONFIG_CompensationWrite(uint8_t probeID, uint8_t indicatorID,  APP_CONFIG_Compensation_pt message)
{
    uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};

    if((probeID > PROBE_SIZE) || (indicatorID > INDICATOR_SIZE)){return APP_ERROR;}
    CompensationSerialize(bytes, message);
    return EEPROM_WriteBlock(APP_CONFIG_EEPROM_COMPENSATION_PAGE + (probeID * 16) + indicatorID, bytes);
}

int APP_CONFIG_CompensationWriteData(uint8_t probeID, uint8_t indicatorID,  APP_CONFIG_Compensation_pt message, uint8_t hasConfig, uint16_t value)
{
    uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};

    if((probeID > PROBE_SIZE) || (indicatorID > INDICATOR_SIZE)){return APP_ERROR;}
    CompensationSerialize(bytes, message);
    write_uint8_t(hasConfig, &bytes[APP_CONFIG_EEPROM_COMPENSATION_HAS_CONFIG_OFFSET]);
    write_uint16_t(value, &bytes[APP_CONFIG_EEPROM_COMPENSATION_VALUE_OFFSET]);
    int rc = EEPROM_WriteBlock(APP_CONFIG_EEPROM_COMPENSATION_PAGE + (probeID * 16) + indicatorID, bytes);
    APP_CHECK_RC(rc)
    message->hasConfig = hasConfig; 
    message->value = value; 
    return APP_OK;
}

int APP_CONFIG_CompensationLoad(void)
{
    int rc = 0;
    uint8_t probeID;
    uint8_t indicatorID;

    for(probeID = 0; probeID < PROBE_SIZE; probeID++ ) 
    {
        for(indicatorID = 0; indicatorID < INDICATOR_SIZE; indicatorID++ ) 
        {
            APP_CONFIG_CompensationInit(&g_appConfigCompensation[probeID][indicatorID]);
            rc = APP_CONFIG_CompensationRead(probeID,indicatorID, &g_appConfigCompensation[probeID][indicatorID]);
            APP_CHECK_RC(rc)
        } 
    } 
    return APP_OK;
}

int APP_CONFIG_CompensationRecovery(void)
{
    int rc = 0;
    uint8_t probeID;
    uint8_t indicatorID;

    for(probeID = 0; probeID < PROBE_SIZE; probeID++ ) 
    {
        for(indicatorID = 0; indicatorID < INDICATOR_SIZE; indicatorID++ ) 
        {
            APP_CONFIG_CompensationInit(&g_appConfigCompensation[probeID][indicatorID]);
            rc = APP_CONFIG_CompensationWrite(probeID,indicatorID, &g_appConfigCompensation[probeID][indicatorID]);
            APP_CHECK_RC(rc)
        } 
    } 
    return APP_OK;
}
int APP_CONFIG_IndicatorAlertInit(APP_CONFIG_IndicatorAlert_pt message)
{
    message->hasConfig = 0;
    message->enable = 0;
    message->max = 0;
    message->min = 0;
    message->threshold = 0;
    return APP_OK;
}

static void IndicatorAlertSerialize(uint8_t bytes[EEPROM_BLOCK_SIZE], APP_CONFIG_IndicatorAlert_pt message)
{
    write_uint8_t(message->hasConfig, &bytes[APP_CONFIG_EEPROM_INDICATOR_ALERT_HAS_CONFIG_OFFSET]); 
    write_uint8_t(message->enable, &bytes[APP_CONFIG_EEPROM_INDICATOR_ALERT_ENABLE_OFFSET]); 
    write_uint16_t(message->max, &bytes[APP_CONFIG_EEPROM_INDICATOR_ALERT_MAX_OFFSET]); 
    write_uint16_t(message->min, &bytes[APP_CONFIG_EEPROM_INDICATOR_ALERT_MIN_OFFSET]); 
    write_uint16_t(message->threshold, &bytes[APP_CONFIG_EEPROM_INDICATOR_ALERT_THRESHOLD_OFFSET]); 
}

APP_CONFIG_IndicatorAlert_t g_appConfigIndicatorAlert[PROBE_SIZE][INDICATOR_SIZE];
int APP_CONFIG_IndicatorAlertRead(uint8_t probeID, uint8_t indicatorID,  APP_CONFIG_IndicatorAlert_pt message)
{
    uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};

    if((probeID > PROBE_SIZE) || (indicatorID > INDICATOR_SIZE)){return APP_ERROR;}
    int rc = EEPROM_ReadBlock(APP_CONFIG_EEPROM_INDICATOR_ALERT_PAGE + (probeID * 16) + indicatorID, bytes);
    APP_CHECK_RC(rc)
    message->hasConfig = read_uint8_t(&bytes[APP_CONFIG_EEPROM_INDICATOR_ALERT_HAS_CONFIG_OFFSET]);
    message->enable = read_uint8_t(&bytes[APP_CONFIG_EEPROM_INDICATOR_ALERT_ENABLE_OFFSET]);
    message->max = read_uint16_t(&bytes[APP_CONFIG_EEPROM_INDICATOR_ALERT_MAX_OFFSET]);
    message->min = read_uint16_t(&bytes[APP_CONFIG_EEPROM_INDICATOR_ALERT_MIN_OFFSET]);
    message->threshold = read_uint16_t(&bytes[APP_CONFIG_EEPROM_INDICATOR_ALERT_THRESHOLD_OFFSET]);
    return APP_OK;
}

int APP_CONFIG_IndicatorAlertWrite(uint8_t probeID, uint8_t indicatorID,  APP_CONFIG_IndicatorAlert_pt message)
{
    uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};

    if((probeID > PROBE_SIZE) || (indicatorID > INDICATOR_SIZE)){return APP_ERROR;}
    IndicatorAlertSerialize(bytes, message);
    return EEPROM_WriteBlock(APP_CONFIG_EEPROM_INDICATOR_ALERT_PAGE + (probeID * 16) + indicatorID, bytes);
}

int APP_CONFIG_IndicatorAlertLoad(void)
{
    int rc = 0;
    uint8_t probeID;
    uint8_t indicatorID;

    for(probeID = 0; probeID < PROBE_SIZE; probeID++ ) 
    {
        for(indicatorID = 0; indicatorID < INDICATOR_SIZE; indicatorID++ ) 
        {
            APP_CONFIG_IndicatorAlertInit(&g_appConfigIndicatorAlert[probeID][indicatorID]);
            rc = APP_CONFIG_IndicatorAlertRead(probeID,indicatorID, &g_appConfigIndicatorAlert[probeID][indicatorID]);
            APP_CHECK_RC(rc)
        } 
    } 
    return APP_OK;
}

int APP_CONFIG_IndicatorAlertRecovery(void)
{
    int rc = 0;
    uint8_t probeID;
    uint8_t indicatorID;

    for(probeID = 0; probeID < PROBE_SIZE; probeID++ ) 
    {
        for(indicatorID = 0; indicatorID < INDICATOR_SIZE; indicatorID++ ) 
        {
            APP_CONFIG_IndicatorAlertInit(&g_appConfigIndicatorAlert[probeID][indicatorID]);
            rc = APP_CONFIG_IndicatorAlertWrite(probeID,indicatorID, &g_appConfigIndicatorAlert[probeID][indicatorID]);
            APP_CHECK_RC(rc)
        } 
    } 
    return APP_OK;
}
int APP_CONFIG_ConfigEnableInit(APP_CONFIG_ConfigEnable_pt message)
{
    message->hasConfig = 0;
    message->enable = 0;
    return APP_OK;
}

static void ConfigEnableSerialize(uint8_t bytes[EEPROM_BLOCK_SIZE], APP_CONFIG_ConfigEnable_pt message)
{
    write_uint8_t(message->hasConfig, &bytes[APP_CONFIG_EEPROM_CONFIG_ENABLE_HAS_CONFIG_OFFSET]); 
    write_uint8_t(message->enable, &bytes[APP_CONFIG_EEPROM_CONFIG_ENABLE_ENABLE_OFFSET]); 
}

static int ConfigEnableDeserialize(uint8_t *bytes, APP_CONFIG_ConfigEnable_pt message)
{
    message->hasConfig = read_uint8_t(&bytes[APP_CONFIG_EEPROM_CONFIG_ENABLE_HAS_CONFIG_OFFSET]); 
    message->enable = read_uint8_t(&bytes[APP_CONFIG_EEPROM_CONFIG_ENABLE_ENABLE_OFFSET]); 
    return APP_OK;
}

static void ConfigEnableCopy( APP_CONFIG_ConfigEnable_pt dist, APP_CONFIG_ConfigEnable_pt from)
{
    dist->hasConfig = from->hasConfig; 
    dist->enable = from->enable; 
}

int APP_CONFIG_LimitInit(APP_CONFIG_Limit_pt message)
{
    message->hasConfig = 0;
    message->enable = 0;
    message->relayID = 0;
    message->channel = 0;
    message->type = 0;
    message->upLimit = 0;
    message->downLimit = 0;
    return APP_OK;
}

static void LimitSerialeze(uint8_t bytes[EEPROM_BLOCK_SIZE], APP_CONFIG_Limit_pt message)
{
    write_uint8_t(message->hasConfig, &bytes[APP_CONFIG_EEPROM_LIMIT_HAS_CONFIG_OFFSET]); 
    write_uint8_t(message->enable, &bytes[APP_CONFIG_EEPROM_LIMIT_ENABLE_OFFSET]); 
    write_uint8_t(message->relayID, &bytes[APP_CONFIG_EEPROM_LIMIT_RELAY_ID_OFFSET]); 
    write_uint16_t(message->channel, &bytes[APP_CONFIG_EEPROM_LIMIT_CHANNEL_OFFSET]); 
    write_uint8_t(message->type, &bytes[APP_CONFIG_EEPROM_LIMIT_TYPE_OFFSET]); 
    write_uint16_t(message->upLimit, &bytes[APP_CONFIG_EEPROM_LIMIT_UP_LIMIT_OFFSET]); 
    write_uint16_t(message->downLimit, &bytes[APP_CONFIG_EEPROM_LIMIT_DOWN_LIMIT_OFFSET]); 
}

APP_CONFIG_Limit_t g_appConfigLimit[PROBE_SIZE][INDICATOR_SIZE][APP_CONFIG_MAX_LIMIT_TASK];
int APP_CONFIG_LimitRead(uint8_t probeID, uint8_t indicatorID, uint8_t taskID,  APP_CONFIG_Limit_pt message)
{
    uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};

    if((probeID > PROBE_SIZE) || (indicatorID > INDICATOR_SIZE) || (taskID > APP_CONFIG_MAX_LIMIT_TASK)){return APP_ERROR;}
    int rc = EEPROM_ReadBlock(APP_CONFIG_EEPROM_LIMIT_PAGE + (probeID * 16 * APP_CONFIG_MAX_LIMIT_TASK) + (indicatorID * APP_CONFIG_MAX_LIMIT_TASK) + taskID, bytes);
    APP_CHECK_RC(rc)
    message->hasConfig = read_uint8_t(&bytes[APP_CONFIG_EEPROM_LIMIT_HAS_CONFIG_OFFSET]);
    message->enable = read_uint8_t(&bytes[APP_CONFIG_EEPROM_LIMIT_ENABLE_OFFSET]);
    message->relayID = read_uint8_t(&bytes[APP_CONFIG_EEPROM_LIMIT_RELAY_ID_OFFSET]);
    message->channel = read_uint16_t(&bytes[APP_CONFIG_EEPROM_LIMIT_CHANNEL_OFFSET]);
    message->type = read_uint8_t(&bytes[APP_CONFIG_EEPROM_LIMIT_TYPE_OFFSET]);
    message->upLimit = read_uint16_t(&bytes[APP_CONFIG_EEPROM_LIMIT_UP_LIMIT_OFFSET]);
    message->downLimit = read_uint16_t(&bytes[APP_CONFIG_EEPROM_LIMIT_DOWN_LIMIT_OFFSET]);
    return APP_OK;
}

int APP_CONFIG_LimitWrite(uint8_t probeID, uint8_t indicatorID, uint8_t taskID,  APP_CONFIG_Limit_pt message)
{
    uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};

    if((probeID > PROBE_SIZE) || (indicatorID > INDICATOR_SIZE) || (taskID > APP_CONFIG_MAX_LIMIT_TASK)){return APP_ERROR;}
    LimitSerialeze(bytes, message);
    return EEPROM_WriteBlock(APP_CONFIG_EEPROM_LIMIT_PAGE + (probeID * 16 * APP_CONFIG_MAX_LIMIT_TASK) + (indicatorID * APP_CONFIG_MAX_LIMIT_TASK) + taskID, bytes);
}

int APP_CONFIG_LimitWriteEnable(uint8_t probeID, uint8_t indicatorID, uint8_t taskID,  APP_CONFIG_Limit_pt message, uint8_t hasConfig, uint8_t enable)
{
    uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};

    if((probeID > PROBE_SIZE) || (indicatorID > INDICATOR_SIZE) || (taskID > APP_CONFIG_MAX_LIMIT_TASK)){return APP_ERROR;}
    LimitSerialeze(bytes, message);
    write_uint8_t(hasConfig, &bytes[APP_CONFIG_EEPROM_LIMIT_HAS_CONFIG_OFFSET]);
    write_uint8_t(enable, &bytes[APP_CONFIG_EEPROM_LIMIT_ENABLE_OFFSET]);
    int rc = EEPROM_WriteBlock(APP_CONFIG_EEPROM_LIMIT_PAGE + (probeID * 16 * APP_CONFIG_MAX_LIMIT_TASK) + (indicatorID * APP_CONFIG_MAX_LIMIT_TASK) + taskID, bytes);
    APP_CHECK_RC(rc)
    message->hasConfig = hasConfig; 
    message->enable = enable; 
    return APP_OK;
}

int APP_CONFIG_LimitLoad(void)
{
    int rc = 0;
    uint8_t probeID;
    uint8_t indicatorID;
    uint8_t taskID;

    for(probeID = 0; probeID < PROBE_SIZE; probeID++ ) 
    {
        for(indicatorID = 0; indicatorID < INDICATOR_SIZE; indicatorID++ ) 
        {
            for(taskID = 0; taskID < APP_CONFIG_MAX_LIMIT_TASK; taskID++ ) 
            {
                APP_CONFIG_LimitInit(&g_appConfigLimit[probeID][indicatorID][taskID]);
                rc = APP_CONFIG_LimitRead(probeID,indicatorID,taskID, &g_appConfigLimit[probeID][indicatorID][taskID]);
                APP_CHECK_RC(rc)
            } 
        } 
    } 
    return APP_OK;
}

int APP_CONFIG_LimitRecovery(void)
{
    int rc = 0;
    uint8_t probeID;
    uint8_t indicatorID;
    uint8_t taskID;

    for(probeID = 0; probeID < PROBE_SIZE; probeID++ ) 
    {
        for(indicatorID = 0; indicatorID < INDICATOR_SIZE; indicatorID++ ) 
        {
            for(taskID = 0; taskID < APP_CONFIG_MAX_LIMIT_TASK; taskID++ ) 
            {
                APP_CONFIG_LimitInit(&g_appConfigLimit[probeID][indicatorID][taskID]);
                rc = APP_CONFIG_LimitWrite(probeID,indicatorID,taskID, &g_appConfigLimit[probeID][indicatorID][taskID]);
                APP_CHECK_RC(rc)
            } 
        } 
    } 
    return APP_OK;
}

int APP_CONFIG_RelayInit(APP_CONFIG_Relay_pt message)
{
    message->hasConfig = 0;
    message->enable = 0;
    message->warning = 0;
    return APP_OK;
}

static void RelaySerialeze(uint8_t bytes[EEPROM_BLOCK_SIZE], APP_CONFIG_Relay_pt message)
{
    write_uint8_t(message->hasConfig, &bytes[APP_CONFIG_EEPROM_RELAY_HAS_CONFIG_OFFSET]); 
    write_uint8_t(message->enable, &bytes[APP_CONFIG_EEPROM_RELAY_ENABLE_OFFSET]); 
    write_uint8_t(message->warning, &bytes[APP_CONFIG_EEPROM_RELAY_WARNING_OFFSET]); 
}

APP_CONFIG_Relay_t g_appConfigRelay[RELAY_SIZE];
int APP_CONFIG_RelayRead(uint8_t relayID,  APP_CONFIG_Relay_pt message)
{
    uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};

    if((relayID > RELAY_SIZE)){return APP_ERROR;}
    int rc = EEPROM_ReadBlock(APP_CONFIG_EEPROM_RELAY_PAGE + relayID, bytes);
    APP_CHECK_RC(rc)
    message->hasConfig = read_uint8_t(&bytes[APP_CONFIG_EEPROM_RELAY_HAS_CONFIG_OFFSET]);
    message->enable = read_uint8_t(&bytes[APP_CONFIG_EEPROM_RELAY_ENABLE_OFFSET]);
    message->warning = read_uint8_t(&bytes[APP_CONFIG_EEPROM_RELAY_WARNING_OFFSET]);
    return APP_OK;
}

int APP_CONFIG_RelayWrite(uint8_t relayID,  APP_CONFIG_Relay_pt message)
{
    uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};

    if((relayID > RELAY_SIZE)){return APP_ERROR;}
    RelaySerialeze(bytes, message);
    return EEPROM_WriteBlock(APP_CONFIG_EEPROM_RELAY_PAGE + relayID, bytes);
}

int APP_CONFIG_RelayWriteHasConfig(uint8_t relayID,  APP_CONFIG_Relay_pt message, uint8_t hasConfig)
{
    uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};

    if((relayID > RELAY_SIZE)){return APP_ERROR;}
    RelaySerialeze(bytes, message);
    write_uint8_t(hasConfig, &bytes[APP_CONFIG_EEPROM_RELAY_HAS_CONFIG_OFFSET]);
    int rc = EEPROM_WriteBlock(APP_CONFIG_EEPROM_RELAY_PAGE + relayID, bytes);
    APP_CHECK_RC(rc)
    message->hasConfig = hasConfig; 
    return APP_OK;
}

int APP_CONFIG_RelayWriteEnable(uint8_t relayID,  APP_CONFIG_Relay_pt message, uint8_t enable)
{
    uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};

    if((relayID > RELAY_SIZE)){return APP_ERROR;}
    RelaySerialeze(bytes, message);
    write_uint8_t(enable, &bytes[APP_CONFIG_EEPROM_RELAY_ENABLE_OFFSET]);
    int rc = EEPROM_WriteBlock(APP_CONFIG_EEPROM_RELAY_PAGE + relayID, bytes);
    APP_CHECK_RC(rc)
    message->enable = enable; 
    return APP_OK;
}

int APP_CONFIG_RelayWriteWarnning(uint8_t relayID,  APP_CONFIG_Relay_pt message, uint8_t warning)
{
    uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};

    if((relayID > RELAY_SIZE)){return APP_ERROR;}
    RelaySerialeze(bytes, message);
    write_uint8_t(warning, &bytes[APP_CONFIG_EEPROM_RELAY_WARNING_OFFSET]);
    int rc = EEPROM_WriteBlock(APP_CONFIG_EEPROM_RELAY_PAGE + relayID, bytes);
    APP_CHECK_RC(rc)
    message->warning = warning; 
    return APP_OK;
}

int APP_CONFIG_RelayLoad(void)
{
    int rc = 0;
    uint8_t relayID;

    for(relayID = 0; relayID < RELAY_SIZE; relayID++ ) 
    {
        APP_CONFIG_RelayInit(&g_appConfigRelay[relayID]);
        rc = APP_CONFIG_RelayRead(relayID, &g_appConfigRelay[relayID]);
        APP_CHECK_RC(rc)
    } 
    return APP_OK;
}

int APP_CONFIG_RelayRecovery(void)
{
    int rc = 0;
    uint8_t relayID;
    for(relayID = 0; relayID < RELAY_SIZE; relayID++ ) 
    {
        APP_CONFIG_RelayInit(&g_appConfigRelay[relayID]);
        rc = APP_CONFIG_RelayWrite(relayID, &g_appConfigRelay[relayID]);
        APP_CHECK_RC(rc)
    } 
    return APP_OK;
}
int APP_CONFIG_TimesEnableInit(APP_CONFIG_TimeEnable_pt message)
{
    APP_CONFIG_ConfigEnableInit(&message->task[0]);
    APP_CONFIG_ConfigEnableInit(&message->task[1]);
    APP_CONFIG_ConfigEnableInit(&message->task[2]);
    APP_CONFIG_ConfigEnableInit(&message->task[3]);
    APP_CONFIG_ConfigEnableInit(&message->task[4]);
    APP_CONFIG_ConfigEnableInit(&message->task[5]);
    APP_CONFIG_ConfigEnableInit(&message->task[6]);
    APP_CONFIG_ConfigEnableInit(&message->task[7]);
    return APP_OK;
}

static void TimesEnableSerialeze(uint8_t bytes[EEPROM_BLOCK_SIZE], APP_CONFIG_TimeEnable_pt message)
{
    ConfigEnableSerialize(&bytes[APP_CONFIG_EEPROM_TIMES_ENABLE_TASK_OFFSET + 0], &message->task[0]);
    ConfigEnableSerialize(&bytes[APP_CONFIG_EEPROM_TIMES_ENABLE_TASK_OFFSET + 2], &message->task[1]);
    ConfigEnableSerialize(&bytes[APP_CONFIG_EEPROM_TIMES_ENABLE_TASK_OFFSET + 4], &message->task[2]);
    ConfigEnableSerialize(&bytes[APP_CONFIG_EEPROM_TIMES_ENABLE_TASK_OFFSET + 6], &message->task[3]);
    ConfigEnableSerialize(&bytes[APP_CONFIG_EEPROM_TIMES_ENABLE_TASK_OFFSET + 8], &message->task[4]);
    ConfigEnableSerialize(&bytes[APP_CONFIG_EEPROM_TIMES_ENABLE_TASK_OFFSET + 10], &message->task[5]);
    ConfigEnableSerialize(&bytes[APP_CONFIG_EEPROM_TIMES_ENABLE_TASK_OFFSET + 12], &message->task[6]);
    ConfigEnableSerialize(&bytes[APP_CONFIG_EEPROM_TIMES_ENABLE_TASK_OFFSET + 14], &message->task[7]);
}

APP_CONFIG_TimeEnable_t g_appConfigTimesEnable[RELAY_SIZE];
int APP_CONFIG_TimesEnableRead(uint8_t relayID,  APP_CONFIG_TimeEnable_pt message)
{
    uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};

    if((relayID > RELAY_SIZE)){return APP_ERROR;}
    int rc = EEPROM_ReadBlock(APP_CONFIG_EEPROM_TIMES_ENABLE_PAGE + relayID, bytes);
    APP_CHECK_RC(rc)
    ConfigEnableDeserialize(&bytes[APP_CONFIG_EEPROM_TIMES_ENABLE_TASK_OFFSET + 0], &message->task[0]);
    ConfigEnableDeserialize(&bytes[APP_CONFIG_EEPROM_TIMES_ENABLE_TASK_OFFSET + 2], &message->task[1]);
    ConfigEnableDeserialize(&bytes[APP_CONFIG_EEPROM_TIMES_ENABLE_TASK_OFFSET + 4], &message->task[2]);
    ConfigEnableDeserialize(&bytes[APP_CONFIG_EEPROM_TIMES_ENABLE_TASK_OFFSET + 6], &message->task[3]);
    ConfigEnableDeserialize(&bytes[APP_CONFIG_EEPROM_TIMES_ENABLE_TASK_OFFSET + 8], &message->task[4]);
    ConfigEnableDeserialize(&bytes[APP_CONFIG_EEPROM_TIMES_ENABLE_TASK_OFFSET + 10], &message->task[5]);
    ConfigEnableDeserialize(&bytes[APP_CONFIG_EEPROM_TIMES_ENABLE_TASK_OFFSET + 12], &message->task[6]);
    ConfigEnableDeserialize(&bytes[APP_CONFIG_EEPROM_TIMES_ENABLE_TASK_OFFSET + 14], &message->task[7]);
    return APP_OK;
}

int APP_CONFIG_TimesEnableWrite(uint8_t relayID,  APP_CONFIG_TimeEnable_pt message)
{
    uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};

    if((relayID > RELAY_SIZE)){return APP_ERROR;}
    TimesEnableSerialeze(bytes, message);
    return EEPROM_WriteBlock(APP_CONFIG_EEPROM_TIMES_ENABLE_PAGE + relayID, bytes);
}

int APP_CONFIG_TimesEnableWriteConfigEnable(uint8_t relayID,  APP_CONFIG_TimeEnable_pt message, uint8_t taskIndex, APP_CONFIG_ConfigEnable_t task)
{
    uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};

    if((relayID > RELAY_SIZE)){return APP_ERROR;}
    TimesEnableSerialeze(bytes, message);
    ConfigEnableSerialize(&bytes[APP_CONFIG_EEPROM_TIMES_ENABLE_TASK_OFFSET + 2 * taskIndex], &task);
    int rc = EEPROM_WriteBlock(APP_CONFIG_EEPROM_TIMES_ENABLE_PAGE + relayID, bytes);
    APP_CHECK_RC(rc)
    ConfigEnableCopy(&message->task[taskIndex], &task); 
    return APP_OK;
}

int APP_CONFIG_TimesEnableLoad(void)
{
    int rc = 0;
    uint8_t relayID;

    for(relayID = 0; relayID < RELAY_SIZE; relayID++ ) 
    {
        APP_CONFIG_TimesEnableInit(&g_appConfigTimesEnable[relayID]);
        rc = APP_CONFIG_TimesEnableRead(relayID, &g_appConfigTimesEnable[relayID]);
        APP_CHECK_RC(rc)
    } 
    return APP_OK;
}

int APP_CONFIG_TimesEnableRecovery(void)
{
    int rc = 0;
    uint8_t relayID;

    for(relayID = 0; relayID < RELAY_SIZE; relayID++ ) 
    {
        APP_CONFIG_TimesEnableInit(&g_appConfigTimesEnable[relayID]);
        rc = APP_CONFIG_TimesEnableWrite(relayID, &g_appConfigTimesEnable[relayID]);
        APP_CHECK_RC(rc)
    } 
    return APP_OK;
}
int APP_CONFIG_TimesJobInit(APP_CONFIG_TimesJob_pt message)
{
    message->type = 0;
    message->startHour = 0;
    message->startMinute = 0;
    message->endHour = 0;
    message->endMinute = 0;
    return APP_OK;
}

static void TimesJobSerialeze(uint8_t bytes[EEPROM_BLOCK_SIZE], APP_CONFIG_TimesJob_pt message)
{
    write_uint8_t(message->type, &bytes[APP_CONFIG_EEPROM_TIMES_JOB_TYPE_OFFSET]); 
    write_uint8_t(message->startHour, &bytes[APP_CONFIG_EEPROM_TIMES_JOB_START_HOUR_OFFSET]); 
    write_uint8_t(message->startMinute, &bytes[APP_CONFIG_EEPROM_TIMES_JOB_START_MINUTE_OFFSET]); 
    write_uint8_t(message->endHour, &bytes[APP_CONFIG_EEPROM_TIMES_JOB_END_HOUR_OFFSET]); 
    write_uint8_t(message->endMinute, &bytes[APP_CONFIG_EEPROM_TIMES_JOB_END_MINUTE_OFFSET]); 
}

static int TimesJobDeserialeze(uint8_t *bytes, APP_CONFIG_TimesJob_pt message)
{
    message->type = read_uint8_t(&bytes[APP_CONFIG_EEPROM_TIMES_JOB_TYPE_OFFSET]); 
    message->startHour = read_uint8_t(&bytes[APP_CONFIG_EEPROM_TIMES_JOB_START_HOUR_OFFSET]); 
    message->startMinute = read_uint8_t(&bytes[APP_CONFIG_EEPROM_TIMES_JOB_START_MINUTE_OFFSET]); 
    message->endHour = read_uint8_t(&bytes[APP_CONFIG_EEPROM_TIMES_JOB_END_HOUR_OFFSET]); 
    message->endMinute = read_uint8_t(&bytes[APP_CONFIG_EEPROM_TIMES_JOB_END_MINUTE_OFFSET]); 
    return APP_OK;
}

static void TimesJobCopy( APP_CONFIG_TimesJob_pt dist, APP_CONFIG_TimesJob_pt from)
{
    dist->type = from->type; 
    dist->startHour = from->startHour; 
    dist->startMinute = from->startMinute; 
    dist->endHour = from->endHour; 
    dist->endMinute = from->endMinute; 
}

int APP_CONFIG_TimesInit(APP_CONFIG_Times_pt message)
{
    message->hasLimit = 0;
    message->channel = 0;
    message->probeID = 0;
    message->indicatorID = 0;
    message->value = 0;
    message->threshold = 0;
    message->executeType = 0;
    APP_CONFIG_TimesJobInit(&message->times[0]);
    APP_CONFIG_TimesJobInit(&message->times[1]);
    APP_CONFIG_TimesJobInit(&message->times[2]);
    APP_CONFIG_TimesJobInit(&message->times[3]);
    return APP_OK;
}

static void TimesSerialize(uint8_t bytes[EEPROM_BLOCK_SIZE], APP_CONFIG_Times_pt message)
{
    write_uint8_t(message->hasLimit, &bytes[APP_CONFIG_EEPROM_TIMES_HAS_LIMIT_OFFSET]); 
    write_uint16_t(message->channel, &bytes[APP_CONFIG_EEPROM_TIMES_CHANNEL_OFFSET]); 
    write_uint8_t(message->probeID, &bytes[APP_CONFIG_EEPROM_TIMES_PROBE_ID_OFFSET]); 
    write_uint8_t(message->indicatorID, &bytes[APP_CONFIG_EEPROM_TIMES_INDICATOR_ID_OFFSET]); 
    write_uint16_t(message->value, &bytes[APP_CONFIG_EEPROM_TIMES_VALUE_OFFSET]); 
    write_uint16_t(message->threshold, &bytes[APP_CONFIG_EEPROM_TIMES_THRESHOLD_OFFSET]); 
    write_uint8_t(message->executeType, &bytes[APP_CONFIG_EEPROM_TIMES_EXECUTE_TYPE_OFFSET]); 
    TimesJobSerialeze(&bytes[APP_CONFIG_EEPROM_TIMES_TIMES_OFFSET + 0], &message->times[0]);
    TimesJobSerialeze(&bytes[APP_CONFIG_EEPROM_TIMES_TIMES_OFFSET + 5], &message->times[1]);
    TimesJobSerialeze(&bytes[APP_CONFIG_EEPROM_TIMES_TIMES_OFFSET + 10], &message->times[2]);
    TimesJobSerialeze(&bytes[APP_CONFIG_EEPROM_TIMES_TIMES_OFFSET + 15], &message->times[3]);
}

APP_CONFIG_Times_t g_appConfigTimes[RELAY_SIZE][APP_CONFIG_MAX_TIMES_TASK];
int APP_CONFIG_TimesRead(uint8_t relayID, uint8_t taskID,  APP_CONFIG_Times_pt message)
{
    uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};

    if((relayID > RELAY_SIZE) || (taskID > APP_CONFIG_MAX_TIMES_TASK)){return APP_ERROR;}
    int rc = EEPROM_ReadBlock(APP_CONFIG_EEPROM_TIMES_PAGE + (relayID * APP_CONFIG_MAX_TIMES_TASK) + taskID, bytes);
    APP_CHECK_RC(rc)
    message->hasLimit = read_uint8_t(&bytes[APP_CONFIG_EEPROM_TIMES_HAS_LIMIT_OFFSET]);
    message->channel = read_uint16_t(&bytes[APP_CONFIG_EEPROM_TIMES_CHANNEL_OFFSET]);
    message->probeID = read_uint8_t(&bytes[APP_CONFIG_EEPROM_TIMES_PROBE_ID_OFFSET]);
    message->indicatorID = read_uint8_t(&bytes[APP_CONFIG_EEPROM_TIMES_INDICATOR_ID_OFFSET]);
    message->value = read_uint16_t(&bytes[APP_CONFIG_EEPROM_TIMES_VALUE_OFFSET]);
    message->threshold = read_uint16_t(&bytes[APP_CONFIG_EEPROM_TIMES_THRESHOLD_OFFSET]);
    message->executeType = read_uint8_t(&bytes[APP_CONFIG_EEPROM_TIMES_EXECUTE_TYPE_OFFSET]);
    TimesJobDeserialeze(&bytes[APP_CONFIG_EEPROM_TIMES_TIMES_OFFSET + 0], &message->times[0]);
    TimesJobDeserialeze(&bytes[APP_CONFIG_EEPROM_TIMES_TIMES_OFFSET + 5], &message->times[1]);
    TimesJobDeserialeze(&bytes[APP_CONFIG_EEPROM_TIMES_TIMES_OFFSET + 10], &message->times[2]);
    TimesJobDeserialeze(&bytes[APP_CONFIG_EEPROM_TIMES_TIMES_OFFSET + 15], &message->times[3]);
    return APP_OK;
}

int APP_CONFIG_TimesWrite(uint8_t relayID, uint8_t taskID,  APP_CONFIG_Times_pt message)
{
    uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};

    if((relayID > RELAY_SIZE) || (taskID > APP_CONFIG_MAX_TIMES_TASK)){return APP_ERROR;}
    TimesSerialize(bytes, message);
    return EEPROM_WriteBlock(APP_CONFIG_EEPROM_TIMES_PAGE + (relayID * APP_CONFIG_MAX_TIMES_TASK) + taskID, bytes);
}

int APP_CONFIG_TimesLoad(void)
{
    int rc = 0;
    uint8_t relayID;
    uint8_t taskID;

    for(relayID = 0; relayID < RELAY_SIZE; relayID++ ) 
    {
        for(taskID = 0; taskID < APP_CONFIG_MAX_TIMES_TASK; taskID++ ) 
        {
            APP_CONFIG_TimesInit(&g_appConfigTimes[relayID][taskID]);
            rc = APP_CONFIG_TimesRead(relayID,taskID, &g_appConfigTimes[relayID][taskID]);
            APP_CHECK_RC(rc)
        } 
    } 
    return APP_OK;
}

int APP_CONFIG_TimesRecovery(void)
{
    int rc = 0;
    uint8_t relayID;
    uint8_t taskID;

    for(relayID = 0; relayID < RELAY_SIZE; relayID++ ) 
    {
        for(taskID = 0; taskID < APP_CONFIG_MAX_TIMES_TASK; taskID++ ) 
        {
            APP_CONFIG_TimesInit(&g_appConfigTimes[relayID][taskID]);
            rc = APP_CONFIG_TimesWrite(relayID,taskID, &g_appConfigTimes[relayID][taskID]);
            APP_CHECK_RC(rc)
        } 
    } 
    return APP_OK;
}
int APP_CONFIG_ManualRelayInit(APP_CONFIG_ManualRelay_pt message)
{
    message->hasConfig = 0;
    message->enable = 0;
    message->type = 0;
    APP_CONFIG_TimeInit(&message->start);
    APP_CONFIG_TimeInit(&message->end);
    return APP_OK;
}

static void ManualRelaySerialize(uint8_t bytes[EEPROM_BLOCK_SIZE], APP_CONFIG_ManualRelay_pt message)
{
    write_uint8_t(message->hasConfig, &bytes[APP_CONFIG_EEPROM_MANUAL_RELAY_HAS_CONFIG_OFFSET]); 
    write_uint8_t(message->enable, &bytes[APP_CONFIG_EEPROM_MANUAL_RELAY_ENABLE_OFFSET]); 
    write_uint8_t(message->type, &bytes[APP_CONFIG_EEPROM_MANUAL_RELAY_TYPE_OFFSET]); 
    TimeSerialize(&bytes[APP_CONFIG_EEPROM_MANUAL_RELAY_START_OFFSET], &message->start); 
    TimeSerialize(&bytes[APP_CONFIG_EEPROM_MANUAL_RELAY_END_OFFSET], &message->end); 
}

APP_CONFIG_ManualRelay_t g_appConfigManualRelay[RELAY_SIZE][RELAY_CHANNEL_SIZE];
int APP_CONFIG_ManualRelayRead(uint8_t relayID, uint8_t channelID,  APP_CONFIG_ManualRelay_pt message)
{
    uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};

    if((relayID > RELAY_SIZE) || (channelID > RELAY_CHANNEL_SIZE)){return APP_ERROR;}
    int rc = EEPROM_ReadBlock(APP_CONFIG_EEPROM_MANUAL_RELAY_PAGE + (relayID * 16) + channelID, bytes);
    APP_CHECK_RC(rc)
    message->hasConfig = read_uint8_t(&bytes[APP_CONFIG_EEPROM_MANUAL_RELAY_HAS_CONFIG_OFFSET]);
    message->enable = read_uint8_t(&bytes[APP_CONFIG_EEPROM_MANUAL_RELAY_ENABLE_OFFSET]);
    message->type = read_uint8_t(&bytes[APP_CONFIG_EEPROM_MANUAL_RELAY_TYPE_OFFSET]);
    TimeDeserialize(&bytes[APP_CONFIG_EEPROM_MANUAL_RELAY_START_OFFSET], &message->start);
    TimeDeserialize(&bytes[APP_CONFIG_EEPROM_MANUAL_RELAY_END_OFFSET], &message->end);
    return APP_OK;
}

int APP_CONFIG_ManualRelayWrite(uint8_t relayID, uint8_t channelID,  APP_CONFIG_ManualRelay_pt message)
{
    uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};

    if((relayID > RELAY_SIZE) || (channelID > RELAY_CHANNEL_SIZE)){return APP_ERROR;}
    ManualRelaySerialize(bytes, message);
    return EEPROM_WriteBlock(APP_CONFIG_EEPROM_MANUAL_RELAY_PAGE + (relayID * 16) + channelID, bytes);
}

int APP_CONFIG_ManualRelayWriteEnable(uint8_t relayID, uint8_t channelID,  APP_CONFIG_ManualRelay_pt message, uint8_t enable)
{
    uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};

    if((relayID > RELAY_SIZE) || (channelID > RELAY_CHANNEL_SIZE)){return APP_ERROR;}
    ManualRelaySerialize(bytes, message);
    write_uint8_t(enable, &bytes[APP_CONFIG_EEPROM_MANUAL_RELAY_ENABLE_OFFSET]);
    int rc = EEPROM_WriteBlock(APP_CONFIG_EEPROM_MANUAL_RELAY_PAGE + (relayID * 16) + channelID, bytes);
    APP_CHECK_RC(rc)
    message->enable = enable; 
    return APP_OK;
}

int APP_CONFIG_ManualRelayWriteTime(uint8_t relayID, uint8_t channelID,  APP_CONFIG_ManualRelay_pt message, uint8_t hasConfig, uint8_t enable, uint8_t type, APP_CONFIG_Time_t start, APP_CONFIG_Time_t end)
{
    uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};

    if((relayID > RELAY_SIZE) || (channelID > RELAY_CHANNEL_SIZE)){return APP_ERROR;}
    ManualRelaySerialize(bytes, message);
    write_uint8_t(hasConfig, &bytes[APP_CONFIG_EEPROM_MANUAL_RELAY_HAS_CONFIG_OFFSET]);
    write_uint8_t(enable, &bytes[APP_CONFIG_EEPROM_MANUAL_RELAY_ENABLE_OFFSET]);
    write_uint8_t(type, &bytes[APP_CONFIG_EEPROM_MANUAL_RELAY_TYPE_OFFSET]);
    TimeSerialize(&bytes[APP_CONFIG_EEPROM_MANUAL_RELAY_START_OFFSET], &start);
    TimeSerialize(&bytes[APP_CONFIG_EEPROM_MANUAL_RELAY_END_OFFSET], &end);
    int rc = EEPROM_WriteBlock(APP_CONFIG_EEPROM_MANUAL_RELAY_PAGE + (relayID * 16) + channelID, bytes);
    APP_CHECK_RC(rc)
    message->hasConfig = hasConfig; 
    message->enable = enable; 
    message->type = type; 
    TimeCopy(&message->start, &start); 
    TimeCopy(&message->end, &end); 
    return APP_OK;
}

int APP_CONFIG_ManualRelayLoad(void)
{
    int rc = 0;
    uint8_t relayID;
    uint8_t channelID;

    for(relayID = 0; relayID < RELAY_SIZE; relayID++ ) 
    {
        for(channelID = 0; channelID < RELAY_CHANNEL_SIZE; channelID++ ) 
        {
            APP_CONFIG_ManualRelayInit(&g_appConfigManualRelay[relayID][channelID]);
            rc = APP_CONFIG_ManualRelayRead(relayID,channelID, &g_appConfigManualRelay[relayID][channelID]);
            APP_CHECK_RC(rc)
        } 
    } 
    return APP_OK;
}

int APP_CONFIG_ManualRelayRecovery(void)
{
    int rc = 0;
    uint8_t relayID;
    uint8_t channelID;

    for(relayID = 0; relayID < RELAY_SIZE; relayID++ ) 
    {
        for(channelID = 0; channelID < RELAY_CHANNEL_SIZE; channelID++ ) 
        {
            APP_CONFIG_ManualRelayInit(&g_appConfigManualRelay[relayID][channelID]);
            rc = APP_CONFIG_ManualRelayWrite(relayID,channelID, &g_appConfigManualRelay[relayID][channelID]);
            APP_CHECK_RC(rc)
        } 
    } 
    return APP_OK;
}
APP_CONFIG_L6_t g_appConfigL6;
int APP_CONFIG_L6Init(APP_CONFIG_L6_pt message)
{
	message->hasConfig=0XF8;
	message->release=20;
	message->readWait=95;
	message->read=50;
	message->charge=15;
	message->chargeWait=7;
	return APP_OK;
}

static void L6Serialize(uint8_t bytes[EEPROM_BLOCK_SIZE], APP_CONFIG_L6_pt message)
{
    write_uint8_t(message->hasConfig, &bytes[APP_CONFIG_EEPROM_L6_HAS_CONFIG_OFFSET]); 
    write_uint8_t(message->release, &bytes[APP_CONFIG_EEPROM_L6_REALEASE_OFFSET]); 
    write_uint8_t(message->readWait, &bytes[APP_CONFIG_EEPROM_L6_READ_WAIT_OFFSET]); 
    write_uint8_t(message->read, &bytes[APP_CONFIG_EEPROM_L6_READ_OFFSET]); 
    write_uint8_t(message->charge, &bytes[APP_CONFIG_EEPROM_L6_CHARGE_OFFSET]); 
    write_uint8_t(message->chargeWait, &bytes[APP_CONFIG_EEPROM_L6_CHARGE_WAIT_OFFSET]); 

}

int APP_CONFIG_L6Write(uint8_t probeID,APP_CONFIG_L6_pt message)
{
	uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};

	if((probeID > PROBE_SIZE)){return APP_ERROR;}
	L6Serialize(bytes, message);
	int rc = EEPROM_WriteBlock(APP_CONFIG_EEPROM_L6_PAGE + probeID, bytes);
    APP_CHECK_RC(rc)
	return APP_OK;
}

int APP_CONFIG_L6Read(uint8_t probeID,APP_CONFIG_L6_pt message)
{
	uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};

	if((probeID > PROBE_SIZE)){return APP_ERROR;}
	int rc=EEPROM_ReadBlock(APP_CONFIG_EEPROM_L6_PAGE + probeID, bytes);
	APP_CHECK_RC(rc)
	message->hasConfig=read_uint8_t(&bytes[APP_CONFIG_EEPROM_L6_HAS_CONFIG_OFFSET]);
	message->release=read_uint8_t(&bytes[APP_CONFIG_EEPROM_L6_REALEASE_OFFSET]);
	message->readWait=read_uint8_t(&bytes[APP_CONFIG_EEPROM_L6_READ_WAIT_OFFSET]);
	message->read=read_uint8_t(&bytes[APP_CONFIG_EEPROM_L6_READ_OFFSET]);
	message->charge=read_uint8_t(&bytes[APP_CONFIG_EEPROM_L6_CHARGE_OFFSET]);
	message->chargeWait=read_uint8_t(&bytes[APP_CONFIG_EEPROM_L6_CHARGE_WAIT_OFFSET]);
	return APP_OK;
}

int APP_CONFIG_L6Load(void)
{
	uint8_t probeID;

    for(probeID = 0; probeID < PROBE_SIZE; probeID++ ) 
	{
        APP_CONFIG_L6Init(&g_appConfigL6);
        int rc=APP_CONFIG_L6Read(probeID, &g_appConfigL6);
        APP_CHECK_RC(rc)
	}
	return APP_OK;
}

int APP_CONFIG_L6Recovery(void)
{
	int rc=0;
    uint8_t probeID;

    for(probeID = 0; probeID < PROBE_SIZE; probeID++ ) 
	{
        APP_CONFIG_L6Init(&g_appConfigL6);
        rc=APP_CONFIG_L6Write(probeID, &g_appConfigL6);
        APP_CHECK_RC(rc)
	}
	return APP_OK;
}




APP_CONFIG_FirmwareInfo_t g_appFirmwareInfo = {0};
APP_CONFIG_FirmwareInfo_t g_downloadFirmwareInfo = {0};
static int FirmwareInfoInit(FirmwareInfo_pt message)
{
	message->deviceType = 0;
	message->versionH = 0;
	message->versionM = 0;
	message->versionL = 0;
	message->fileLen = 0;
	message->fileCrc = 0;
	
	return APP_OK;
}


static int FirmwareInfoSerialize(uint8_t *bytes, FirmwareInfo_pt message)
{
	write_uint8_t(message->deviceType, &bytes[APP_CONFIG_EEPROM_FIRMWARE_INFO_DEVICE_TYPE_OFFSET]); 
	write_uint8_t(message->versionH, &bytes[APP_CONFIG_EEPROM_FIRMWARE_INFO_VERSION_H_OFFSET]); 
	write_uint8_t(message->versionM, &bytes[APP_CONFIG_EEPROM_FIRMWARE_INFO_VERSION_M_OFFSET]); 
	write_uint8_t(message->versionL, &bytes[APP_CONFIG_EEPROM_FIRMWARE_INFO_VERSION_L_OFFSET]);
	write_uint32_t(message->fileLen, &bytes[APP_CONFIG_EEPROM_FIRMWARE_INFO_FILE_LEN_OFFSET]); 
	write_uint32_t(message->fileCrc, &bytes[APP_CONFIG_EEPROM_FIRMWARE_INFO_FILE_CRC_OFFSET]); 

	return APP_OK;
}

static int FirmwareInfoDeserialize(uint8_t *bytes, FirmwareInfo_pt message)
{
	message->deviceType = read_uint8_t(&bytes[APP_CONFIG_EEPROM_FIRMWARE_INFO_DEVICE_TYPE_OFFSET]); 
	message->versionH = read_uint8_t(&bytes[APP_CONFIG_EEPROM_FIRMWARE_INFO_VERSION_H_OFFSET]); 
	message->versionM = read_uint8_t(&bytes[APP_CONFIG_EEPROM_FIRMWARE_INFO_VERSION_M_OFFSET]); 
	message->versionL = read_uint8_t(&bytes[APP_CONFIG_EEPROM_FIRMWARE_INFO_VERSION_L_OFFSET]);
	message->fileLen = read_uint32_t(&bytes[APP_CONFIG_EEPROM_FIRMWARE_INFO_FILE_LEN_OFFSET]); 
	message->fileCrc = read_uint32_t(&bytes[APP_CONFIG_EEPROM_FIRMWARE_INFO_FILE_CRC_OFFSET]);
	
	return APP_OK;
}


int APP_CONFIG_FirmwareInfoInit(APP_CONFIG_FirmwareInfo_pt message)
{
	message->downloadLen = 0;
    message->updateResult = IAP_UPDATE_UNKNOW;
    message->reserved[0] = 0;
    message->reserved[1] = 0;
    message->firmwareType = UNKONW_FIRMWARE;
	FirmwareInfoInit(&message->firmwareInfo);
	
	return APP_OK;
}


int APP_CONFIG_FirmwareInfoRead(uint8_t infotype, APP_CONFIG_FirmwareInfo_pt message)
{
	int rc = 0;
	uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};
	uint16_t page = 0;
		
	if(APP_FIRMWARE_INFO == infotype)
	{
		page = APP_CONFIG_EEPROM_FIRMWARE_SETTING_INFO_PAGE;
	}
	else
	{
		page = APP_CONFIG_EEPROM_FIRMWARE_SETTING_INFO_BACK_PAGE;
	}

	rc = EEPROM_ReadBlock(page, bytes);
	APP_CHECK_RC(rc)
	message->downloadLen = read_uint32_t(&bytes[APP_CONFIG_EEPROM_FIRMWARE_INFO_DOWNLOAD_LEN_OFFSET]);
    message->updateResult = read_uint8_t(&bytes[APP_CONFIG_EEPROM_FIRMWARE_INFO_UPDATE_RESULT_OFFSET]);
	message->reserved[0] = read_uint8_t(&bytes[APP_CONFIG_EEPROM_FIRMWARE_INFO_RESERVED_OFFSET]);
	message->reserved[1] = read_uint8_t(&bytes[APP_CONFIG_EEPROM_FIRMWARE_INFO_RESERVED_OFFSET+1]);
	message->firmwareType = read_uint8_t(&bytes[APP_CONFIG_EEPROM_FIRMWARE_INFO_FIRMWARE_TYPE_OFFSET]);
	FirmwareInfoDeserialize(bytes, &message->firmwareInfo);
	
	return APP_OK;
}


int APP_CONFIG_FirmwareInfoWrite(uint8_t infotype, APP_CONFIG_FirmwareInfo_pt message)
{
	uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};
	uint16_t page = 0;
	
	if(APP_FIRMWARE_INFO == infotype)
	{
		page = APP_CONFIG_EEPROM_FIRMWARE_SETTING_INFO_PAGE;
	}
	else
	{
		page = APP_CONFIG_EEPROM_FIRMWARE_SETTING_INFO_BACK_PAGE;
	}
	write_uint32_t(message->downloadLen, &bytes[APP_CONFIG_EEPROM_FIRMWARE_INFO_DOWNLOAD_LEN_OFFSET]);
    write_uint8_t(message->updateResult,  &bytes[APP_CONFIG_EEPROM_FIRMWARE_INFO_UPDATE_RESULT_OFFSET]);
    write_uint8_t(message->reserved[0],  &bytes[APP_CONFIG_EEPROM_FIRMWARE_INFO_RESERVED_OFFSET]);
	write_uint8_t(message->reserved[1],  &bytes[APP_CONFIG_EEPROM_FIRMWARE_INFO_RESERVED_OFFSET+1]);
	write_uint8_t(message->firmwareType,  &bytes[APP_CONFIG_EEPROM_FIRMWARE_INFO_FIRMWARE_TYPE_OFFSET]);
	FirmwareInfoSerialize(bytes, &message->firmwareInfo);
	return EEPROM_WriteBlock(page, bytes);
}


int APP_CONFIG_FirmwareSettingInfoOverwrite(void)
{
	int rc = 0;
	rc = APP_CONFIG_FirmwareInfoWrite(DOWNLOAD_FIRMWARE_INFO, &g_downloadFirmwareInfo);
	APP_CHECK_RC(rc)
	memcpy((uint8_t *)&g_appFirmwareInfo, (uint8_t *)&g_downloadFirmwareInfo, sizeof(APP_CONFIG_FirmwareInfo_t));
	rc = APP_CONFIG_FirmwareInfoWrite(APP_FIRMWARE_INFO, &g_appFirmwareInfo);
	APP_CHECK_RC(rc)
	return APP_OK;
}

// int APP_CONFIG_firmware_setting_info_recoverywrite(void)
// {
// 	int rc = 0;
// 	memcpy((uint8_t *)&g_downloadFirmwareInfo, (uint8_t *)&g_appFirmwareInfo, sizeof(APP_CONFIG_FirmwareInfo_t));
// 	rc = APP_CONFIG_FirmwareInfoWrite(DOWNLOAD_FIRMWARE_INFO, &g_downloadFirmwareInfo);
// 	APP_CHECK_RC(rc)
// }

// int APP_CONFIG_firmware_setting_info_overwrite1(void)
// {
// 	int rc = 0;
// 	rc = APP_CONFIG_FirmwareInfoWrite(DOWNLOAD_FIRMWARE_INFO, &g_downloadFirmwareInfo);
// 	APP_CHECK_RC(rc)
// }


int APP_CONFIG_FirmwareInfoLoad(void)
{
	int rc = 0;
	APP_CONFIG_FirmwareInfoInit(&g_appFirmwareInfo);
	APP_CONFIG_FirmwareInfoInit(&g_downloadFirmwareInfo);
	rc = APP_CONFIG_FirmwareInfoRead(APP_FIRMWARE_INFO, &g_appFirmwareInfo);
	APP_CHECK_RC(rc)
	rc = APP_CONFIG_FirmwareInfoRead(DOWNLOAD_FIRMWARE_INFO, &g_downloadFirmwareInfo);
	APP_CHECK_RC(rc)
	return APP_OK;
}

int APP_CONFIG_AppFirmwareInfoClearResult(void)
{
	int rc = 0;
	g_appFirmwareInfo.updateResult = IAP_UPDATE_UNKNOW;
	rc = APP_CONFIG_FirmwareInfoWrite(APP_FIRMWARE_INFO, &g_appFirmwareInfo);
	APP_CHECK_RC(rc)
	return APP_OK;
}


int APP_CONFIG_DownloadFirmwareInfoClearResult(void)
{
	int rc = 0;
	g_downloadFirmwareInfo.updateResult = IAP_UPDATE_UNKNOW;
    g_downloadFirmwareInfo.firmwareType = MCU_APP_FIRMWARE;
	rc = APP_CONFIG_FirmwareInfoWrite(DOWNLOAD_FIRMWARE_INFO, &g_downloadFirmwareInfo);
	APP_CHECK_RC(rc)
	return APP_OK;
}

int APP_CONFIG_AppFirmwareInfoRecovery(void)
{
	int rc = 0;
	APP_CONFIG_FirmwareInfo_pt info = &g_appFirmwareInfo;
	info->downloadLen = 0;
    info->updateResult = IAP_UPDATE_UNKNOW;
	info->reserved[0] = 0;
	info->reserved[1] = 0;
    info->firmwareType = MCU_APP_FIRMWARE;
	info->firmwareInfo.deviceType = MASTER_CONTROL_DEV;
	info->firmwareInfo.versionH = APP_FIRMWARE_MAJOR_DEFAULT;
	info->firmwareInfo.versionM = APP_FIRMWARE_MINOR_DEFAULT;	
	info->firmwareInfo.versionL = APP_FIRMWARE_REVISION_DEFAULT;
	info->firmwareInfo.fileLen = (0x20000);
	info->firmwareInfo.fileCrc = DEFAULT_CRC32_VALUE;
	rc = APP_CONFIG_FirmwareInfoWrite(APP_FIRMWARE_INFO, info);
	APP_CHECK_RC(rc)
	return APP_OK;
}


int APP_CONFIG_DownloadFirmwareInfoRecovery(void)
{
	int rc = 0;
	APP_CONFIG_FirmwareInfo_pt info = &g_downloadFirmwareInfo;
	info->downloadLen = 0;
    info->updateResult = IAP_UPDATE_UNKNOW;
	info->reserved[0] = 0;
	info->reserved[1] = 0;
    info->firmwareType = UNKONW_FIRMWARE;
	info->firmwareInfo.deviceType = UNKONW_DEV;
	info->firmwareInfo.versionH = APP_FIRMWARE_MAJOR_DEFAULT;
	info->firmwareInfo.versionM = APP_FIRMWARE_MINOR_DEFAULT;	
	info->firmwareInfo.versionL = APP_FIRMWARE_REVISION_DEFAULT;
	info->firmwareInfo.fileLen = (0x20000);
	info->firmwareInfo.fileCrc = DEFAULT_CRC32_VALUE;
	rc = APP_CONFIG_FirmwareInfoWrite(DOWNLOAD_FIRMWARE_INFO, info);
	APP_CHECK_RC(rc)
	return APP_OK;
}


int APP_CONFIG_AllFirmwareInfoRecovery(void)
{
	int rc = 0;
	APP_CONFIG_FirmwareInfo_pt info = &g_appFirmwareInfo;
	info->downloadLen = 0;
    info->updateResult = IAP_UPDATE_UNKNOW;
	info->reserved[0] = 0;
	info->reserved[1] = 0;
    info->firmwareType = MCU_APP_FIRMWARE;
	info->firmwareInfo.deviceType = MASTER_CONTROL_DEV;
	info->firmwareInfo.versionH = APP_FIRMWARE_MAJOR_DEFAULT;
	info->firmwareInfo.versionM = APP_FIRMWARE_MINOR_DEFAULT;	
	info->firmwareInfo.versionL = APP_FIRMWARE_REVISION_DEFAULT;
	info->firmwareInfo.fileLen = (0x20000);
	info->firmwareInfo.fileCrc = DEFAULT_CRC32_VALUE;
	rc = APP_CONFIG_FirmwareInfoWrite(APP_FIRMWARE_INFO, &g_appFirmwareInfo);
	APP_CHECK_RC(rc)
	memcpy((uint8_t *)&g_downloadFirmwareInfo, (uint8_t *)&g_appFirmwareInfo, sizeof(APP_CONFIG_FirmwareInfo_t));
    info = &g_downloadFirmwareInfo;
    info->firmwareType = UNKONW_FIRMWARE;
    info->firmwareInfo.deviceType = UNKONW_DEV;
	rc = APP_CONFIG_FirmwareInfoWrite(DOWNLOAD_FIRMWARE_INFO, &g_downloadFirmwareInfo);
	APP_CHECK_RC(rc)
	return APP_OK;
}

// info 区的数据是否有效 
int APP_CONFIG_FirmwareCheckInfoValid(APP_CONFIG_FirmwareInfo_pt info)
{
    if((0 == info->downloadLen) || (UNKONW_FIRMWARE == info->firmwareType) || (DEFAULT_CRC32_VALUE == info->firmwareInfo.fileCrc))
    {
        return APP_ERROR;
    }
    return APP_OK;
}

/* 继电器通道接线、功率、故障使能配置相关 */
static int RelayPowerConfigSerialize(uint8_t *bytes, APP_RELAY_PowerConfig_pt message)
{
	write_uint8_t(message->enable, &bytes[APP_CONFIG_EEPROM_RELAY_POWER_CONFIG_ENABLE_OFFSET]); 
	write_uint8_t(message->phaseConfig, &bytes[APP_CONFIG_EEPROM_RELAY_POWER_CONFIG_PHASE_OFFSET]); 
	write_uint8_t(message->power, &bytes[APP_CONFIG_EEPROM_RELAY_POWER_CONFIG_POWER_OFFSET]); 
	write_uint8_t(message->lossPhaseProtectEnable, &bytes[APP_CONFIG_EEPROM_RELAY_POWER_CONFIG_LOSSPHASE_PROTECT_ENABLE_OFFSET]); 
	write_uint8_t(message->overloadProtectEnable, &bytes[APP_CONFIG_EEPROM_RELAY_POWER_CONFIG_OVERLOAD_PROTECT_ENABLE_OFFSET]); 
	write_uint8_t(message->ouvProtectEnable, &bytes[APP_CONFIG_EEPROM_RELAY_POWER_CONFIG_OUV_PROTECT_ENABLE_OFFSET]); 
	write_uint8_t(message->loaderType, &bytes[APP_CONFIG_EEPROM_RELAY_POWER_CONFIG_LOADER_TYPE]); 

	return APP_OK;
}

APP_RELAY_PowerConfig_t g_appRelayPowerConfig[RELAY_SIZE][RELAY_CHANNEL_SIZE];
static int RelayPowerConfigDeserialize(uint8_t *bytes, APP_RELAY_PowerConfig_pt message)
{
	message->enable = read_uint8_t(&bytes[APP_CONFIG_EEPROM_RELAY_POWER_CONFIG_ENABLE_OFFSET]); 
	message->phaseConfig = read_uint8_t(&bytes[APP_CONFIG_EEPROM_RELAY_POWER_CONFIG_PHASE_OFFSET]); 
	message->power = read_uint8_t(&bytes[APP_CONFIG_EEPROM_RELAY_POWER_CONFIG_POWER_OFFSET]); 
	message->lossPhaseProtectEnable = read_uint8_t(&bytes[APP_CONFIG_EEPROM_RELAY_POWER_CONFIG_LOSSPHASE_PROTECT_ENABLE_OFFSET]); 
	message->overloadProtectEnable = read_uint8_t(&bytes[APP_CONFIG_EEPROM_RELAY_POWER_CONFIG_OVERLOAD_PROTECT_ENABLE_OFFSET]);
	message->ouvProtectEnable = read_uint8_t(&bytes[APP_CONFIG_EEPROM_RELAY_POWER_CONFIG_OUV_PROTECT_ENABLE_OFFSET]); 
	message->loaderType = read_uint8_t(&bytes[APP_CONFIG_EEPROM_RELAY_POWER_CONFIG_LOADER_TYPE]); 
	return APP_OK;
}


int APP_CONFIG_RelayPowerConfigInit(uint8_t channelID, APP_RELAY_PowerConfig_pt message)
{
    message->enable = ENABLE;
    if(4 != channelID)
    {
        message->phaseConfig = APP_RELAY_PHASE_3V3;
        message->power = 30;
    }
	else
    {
        message->phaseConfig = APP_RELAY_PHASE_1V1;
        message->power = 10;
    }
	message->lossPhaseProtectEnable = APP_RELAY_PROTECT_DISABLE;
	message->overloadProtectEnable = APP_RELAY_PROTECT_DISABLE;
	message->ouvProtectEnable = APP_RELAY_PROTECT_DISABLE;
    message->loaderType = APP_RELAY_LOADER_TYPE_AERATOR;
	
	return APP_OK;
}


int APP_CONFIG_RelayPowerConfigWrite(uint8_t channelID, APP_RELAY_PowerConfig_pt message)
{
	uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};

	if(channelID > RELAY_CHANNEL_SIZE)
    {
        return APP_ERROR;
    }
    RelayPowerConfigSerialize(bytes, message);
	int rc = EEPROM_WriteBlock(APP_CONFIG_EEPROM_RELAY_POWER_CONFIG_PAGE + channelID, bytes);
    APP_CHECK_RC(rc)
    return APP_OK;
}


int APP_CONFIG_RelayPowerConfigRead(uint8_t channelID, APP_RELAY_PowerConfig_pt message)
{
	uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};

    if(channelID > RELAY_CHANNEL_SIZE)
    {
        return APP_ERROR;
    }
	int rc = EEPROM_ReadBlock(APP_CONFIG_EEPROM_RELAY_POWER_CONFIG_PAGE + channelID, bytes);
    APP_CHECK_RC(rc)
    RelayPowerConfigDeserialize(bytes, message);
    return APP_OK;
}

int APP_CONFIG_RelayPowerConfigCheck(uint8_t channelID, APP_RELAY_PowerConfig_pt message)
{
    if(ENABLE != message->enable)
    {
        APP_CONFIG_RelayPowerConfigInit(channelID, message);
        APP_CONFIG_RelayPowerConfigWrite(channelID, message);
        return APP_ERROR;
    }
#if 0
    if((APP_RELAY_PHASE_1V1 != message->phaseConfig) && (APP_RELAY_PHASE_3V3 != message->phaseConfig))   message->phaseConfig = APP_RELAY_PHASE_UNKNOW;
    if(APP_RELAY_PROTECT_ENABLE != message->lossPhaseProtectEnable)  message->lossPhaseProtectEnable = APP_RELAY_PROTECT_DISABLE;
    if(APP_RELAY_PROTECT_ENABLE != message->overloadProtectEnable)  message->overloadProtectEnable = APP_RELAY_PROTECT_DISABLE;
    if(APP_RELAY_PROTECT_ENABLE != message->ouvProtectEnable)  message->ouvProtectEnable = APP_RELAY_PROTECT_DISABLE;
#endif
}

int APP_CONFIG_RelayPowerConfigLoad(void)
{
	int rc = APP_OK;
    uint8_t channelID;
    APP_RELAY_PowerConfig_pt pconfig = NULL;
    
    for(channelID = 0; channelID < RELAY_CHANNEL_SIZE; channelID++) 
    {
        pconfig = &g_appRelayPowerConfig[0][channelID];
        rc = APP_CONFIG_RelayPowerConfigRead(channelID, pconfig);
        APP_CONFIG_RelayPowerConfigCheck(channelID, pconfig);
        APP_CHECK_RC(rc)
    }
    return APP_OK;
}

int APP_CONFIG_RelayPowerConfigRecovery(void)
{
    int rc = 0;
    uint8_t channelID;
    APP_RELAY_PowerConfig_pt pconfig = NULL;

    for(channelID = 0; channelID < RELAY_CHANNEL_SIZE; channelID++) 
    {
        pconfig = &g_appRelayPowerConfig[0][channelID];
        APP_CONFIG_RelayPowerConfigInit(channelID, pconfig);
        rc = APP_CONFIG_RelayPowerConfigWrite(channelID, pconfig);
        APP_CHECK_RC(rc)
    }
    return APP_OK;
}

APP_CONFIG_DiagnosisMessage_t g_diagnosisMsg;

int APP_CONFIG_DiagnosisMessageInit(APP_CONFIG_DiagnosisMessage_pt message)
{
    message->msgTime.year = 0;
    message->msgTime.month = 1;
    message->msgTime.day = 1;
    message->msgTime.hour = 0;
    message->msgTime.minute = 0;
    message->msgTime.second = 0;
    message->msgType = 0;
    message->msgStatus = 0;
}

int APP_CONFIG_DiagnosisMessageRead(APP_CONFIG_DiagnosisMessage_pt message)
{
	uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};
    int rc = EEPROM_ReadBlock(APP_CONFIG_EEPROM_DIAGNOSIS_MESSAGE_PAGE, bytes);
    APP_CHECK_RC(rc)
    message->msgTime.year = read_uint8_t(&bytes[APP_CONFIG_EEPROM_DIAGNOSIS_TIME_YEAR_OFFSET]);
    message->msgTime.month = read_uint8_t(&bytes[APP_CONFIG_EEPROM_DIAGNOSIS_TIME_MONTH_OFFSET]);
    message->msgTime.day = read_uint8_t(&bytes[APP_CONFIG_EEPROM_DIAGNOSIS_TIME_DAY_OFFSET]);
    message->msgTime.hour = read_uint8_t(&bytes[APP_CONFIG_EEPROM_DIAGNOSIS_TIME_HOUR_OFFSET]);
    message->msgTime.minute = read_uint8_t(&bytes[APP_CONFIG_EEPROM_DIAGNOSIS_TIME_MINUTE_OFFSET]);
    message->msgTime.second = read_uint8_t(&bytes[APP_CONFIG_EEPROM_DIAGNOSIS_TIME_SECOND_OFFSET]);
    message->msgType = read_uint8_t(&bytes[APP_CONFIG_EEPROM_DIAGNOSIS_TYPE]);
    message->msgStatus = read_uint8_t(&bytes[APP_CONFIG_EEPROM_DIAGNOSIS_STATUS]);
    return APP_OK;
}

int APP_CONFIG_DiagnosisMessageWrite(APP_CONFIG_DiagnosisMessage_pt message)
{
	uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};
    write_uint8_t(message->msgTime.year,  &bytes[APP_CONFIG_EEPROM_DIAGNOSIS_TIME_YEAR_OFFSET]);
    write_uint8_t(message->msgTime.month,  &bytes[APP_CONFIG_EEPROM_DIAGNOSIS_TIME_MONTH_OFFSET]);
    write_uint8_t(message->msgTime.day,  &bytes[APP_CONFIG_EEPROM_DIAGNOSIS_TIME_DAY_OFFSET]);
    write_uint8_t(message->msgTime.hour,  &bytes[APP_CONFIG_EEPROM_DIAGNOSIS_TIME_HOUR_OFFSET]);
    write_uint8_t(message->msgTime.minute,  &bytes[APP_CONFIG_EEPROM_DIAGNOSIS_TIME_MINUTE_OFFSET]);
    write_uint8_t(message->msgTime.second,  &bytes[APP_CONFIG_EEPROM_DIAGNOSIS_TIME_SECOND_OFFSET]);
    write_uint8_t(message->msgType,  &bytes[APP_CONFIG_EEPROM_DIAGNOSIS_TYPE]);
    write_uint8_t(message->msgStatus,  &bytes[APP_CONFIG_EEPROM_DIAGNOSIS_STATUS]);
    int rc = EEPROM_WriteBlock(APP_CONFIG_EEPROM_DIAGNOSIS_MESSAGE_PAGE, bytes);
    return rc;
}

int APP_CONFIG_DiagnosisMessageLoad(void)
{
    int rc = 0;
    APP_CONFIG_DiagnosisMessageInit(&g_diagnosisMsg);
    APP_CONFIG_DiagnosisMessageRead(&g_diagnosisMsg);
    if((0 == g_diagnosisMsg.msgTime.year) && (0 == g_diagnosisMsg.msgTime.month) && (0 == g_diagnosisMsg.msgTime.day))
    {   // 用于处理之前没有使用该BLOCK区，整片格式化时全部赋值为0的情况，重新将数据校正  
        APP_CONFIG_DiagnosisMessageRecovery();
    }
    return APP_OK;
}

int APP_CONFIG_DiagnosisMessageRecovery(void)
{
    int rc = 0;
    APP_CONFIG_DiagnosisMessageInit(&g_diagnosisMsg);
    APP_CONFIG_DiagnosisMessageWrite(&g_diagnosisMsg);
    return APP_OK;
}

#define RAND_OPT_VALID_VALUE    (0xA55A) 
#define RAND_MAX_DEFAULT_VALUE  (50)
#define RAND_MIN_DEFAULT_VALUE  (10)
APP_CONFIG_RandOpt_t g_appRandOpt = 
{
    .valid = RAND_OPT_VALID_VALUE, 
    .max = RAND_MAX_DEFAULT_VALUE, 
    .min = RAND_MIN_DEFAULT_VALUE
};

#ifdef ENABLE_INPUT_RAND_MAX_MIN
int APP_CONFIG_RandValueInit(APP_CONFIG_RandOpt_pt opt)
{
    opt->valid = RAND_OPT_VALID_VALUE;
    opt->max = RAND_MAX_DEFAULT_VALUE;
    opt->min = RAND_MIN_DEFAULT_VALUE;
}

int APP_CONFIG_RandValueRead(APP_CONFIG_RandOpt_pt opt)
{
	uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};
    int rc = EEPROM_ReadBlock(APP_CONFIG_EEPROM_RAND_VALUE_PAGE, bytes);
    APP_CHECK_RC(rc)
    opt->valid = read_uint16_t(&bytes[APP_CONFIG_EEPROM_RAND_VALID_OFFSET]);
    opt->max = read_uint16_t(&bytes[APP_CONFIG_EEPROM_RAND_MAX_VALUE_OFFSET]);
    opt->min = read_uint16_t(&bytes[APP_CONFIG_EEPROM_RAND_MIN_VALUE_OFFSET]);
    return APP_OK;
}

int APP_CONFIG_RandValueWrite(APP_CONFIG_RandOpt_pt opt)
{
	uint8_t bytes[EEPROM_BLOCK_SIZE] = {0};
    write_uint16_t(opt->valid,  &bytes[APP_CONFIG_EEPROM_RAND_VALID_OFFSET]);
    write_uint16_t(opt->max,  &bytes[APP_CONFIG_EEPROM_RAND_MAX_VALUE_OFFSET]);
    write_uint16_t(opt->min,  &bytes[APP_CONFIG_EEPROM_RAND_MIN_VALUE_OFFSET]);
    int rc = EEPROM_WriteBlock(APP_CONFIG_EEPROM_RAND_VALUE_PAGE, bytes);
    return rc;
}

int APP_CONFIG_RandValueLoad(void)
{
    int rc = 0;
    APP_CONFIG_RandValueInit(&g_appRandOpt);
    rc = APP_CONFIG_RandValueRead(&g_appRandOpt);
    APP_CHECK_RC(rc)
    if(RAND_OPT_VALID_VALUE != g_appRandOpt.valid)
    {
        rc = APP_ERROR;
    }
    return rc;
}

int APP_CONFIG_RandValueRecovery(void)
{
    int rc = 0;
    APP_CONFIG_RandValueInit(&g_appRandOpt);
    rc = APP_CONFIG_RandValueWrite(&g_appRandOpt);
    return rc;
}

#endif

int APP_CONFIG_EepromLoadAll(void)
{
    int rc = 0;

    rc = EEPROM_ReadID();
    BSP_LOG_trace("eeprom_id read rc = %d\r\n", rc);

    rc = APP_CONFIG_SystemLoad();
    BSP_LOG_trace("system load rc: %d\r\n", rc);
    APP_CHECK_RC(rc)
    rc = APP_CONFIG_DeviceEnableLoad();
    BSP_LOG_trace("device_enable load rc: %d\r\n", rc);
    APP_CHECK_RC(rc)
    rc = APP_CONFIG_DeviceConnectLoad();
    BSP_LOG_trace("device_connect load rc: %d\r\n", rc);
    APP_CHECK_RC(rc)
    rc = APP_CONFIG_ProbeEnableLoad();
    BSP_LOG_trace("probe_enable load rc: %d\r\n", rc);
    APP_CHECK_RC(rc)
    rc = APP_CONFIG_PumpStatusLoad();
    BSP_LOG_trace("pump_status load rc: %d\r\n", rc);
    APP_CHECK_RC(rc)
    rc = APP_CONFIG_SensorLoad();
    BSP_LOG_trace("sensor load rc: %d\r\n", rc);
    APP_CHECK_RC(rc)
    rc = APP_CONFIG_CompensationLoad();
    BSP_LOG_trace("compensation load rc: %d\r\n", rc);
    APP_CHECK_RC(rc)
    rc = APP_CONFIG_IndicatorAlertLoad();
    BSP_LOG_trace("indicator_alert load rc: %d\r\n", rc);
    APP_CHECK_RC(rc)
    BSP_WDG_feed();
    rc = APP_CONFIG_LimitLoad();
    BSP_LOG_trace("limit load rc: %d\r\n", rc);
    APP_CHECK_RC(rc)
    BSP_WDG_feed();
    rc = APP_CONFIG_RelayLoad();
    BSP_LOG_trace("relay load rc: %d\r\n", rc);
    APP_CHECK_RC(rc)
    rc = APP_CONFIG_TimesEnableLoad();
    BSP_LOG_trace("times_enable load rc: %d\r\n", rc);
    APP_CHECK_RC(rc)
    rc = APP_CONFIG_TimesLoad();
    BSP_LOG_trace("times load rc: %d\r\n", rc);
    APP_CHECK_RC(rc)
    rc = APP_CONFIG_ManualRelayLoad();
    BSP_LOG_trace("manual_relay load rc: %d\r\n", rc);
    APP_CHECK_RC(rc)
#ifdef SUPPORT_L6_ENABLE
    rc = APP_CONFIG_L6Load();
    BSP_LOG_trace("l6 load rc: %d\r\n", rc);
    APP_CHECK_RC(rc)
#endif
    rc = APP_CONFIG_RelayPowerConfigLoad();
    BSP_LOG_trace("relay power config load rc: %d\r\n", rc);
    APP_CHECK_RC(rc)
    rc = APP_CONFIG_DiagnosisMessageLoad();
    BSP_LOG_trace("diagnosis message load rc: %d\r\n", rc);
    APP_CHECK_RC(rc)
#ifdef ENABLE_INPUT_RAND_MAX_MIN
    rc = APP_CONFIG_RandValueLoad();
    if(APP_OK != rc)
    {
        APP_CONFIG_RandValueRecovery();
    }
#endif    
    rc = APP_CONFIG_FirmwareInfoLoad();
    BSP_LOG_trace("firmware info load rc: %d\r\n", rc);
    APP_CHECK_RC(rc)
    return APP_OK;
}

// SGM706 的看门狗复位周期为 1.6s，此函数耗时较长，需插入喂狗操作
int APP_CONFIG_EepromRecoveryAll(void)
{
    int rc = 0;

    rc = APP_CONFIG_DeviceEnableRecovery();
    BSP_LOG_trace("device_enable recovery rc: %d\r\n", rc);
    APP_CHECK_RC(rc)
    rc = APP_CONFIG_DeviceConnectRecovery();
    BSP_LOG_trace("device_connect recovery rc: %d\r\n", rc);
    APP_CHECK_RC(rc)
    rc = APP_CONFIG_ProbeEnableRecovery();
    BSP_LOG_trace("probe_enable recovery rc: %d\r\n", rc);
    APP_CHECK_RC(rc)
    rc = APP_CONFIG_PumpStatusRecovery();
    BSP_LOG_trace("pump_status recovery rc: %d\r\n", rc);
    APP_CHECK_RC(rc)
    rc = APP_CONFIG_SensorRecovery();
    BSP_LOG_trace("sensor recovery rc: %d\r\n", rc);
    APP_CHECK_RC(rc)
    rc = APP_CONFIG_CompensationRecovery();
    BSP_LOG_trace("compensation recovery rc: %d\r\n", rc);
    APP_CHECK_RC(rc)
    rc = APP_CONFIG_IndicatorAlertRecovery();
    BSP_LOG_trace("indicator_alert recovery rc: %d\r\n", rc);
    APP_CHECK_RC(rc)
    BSP_WDG_feed();
    rc = APP_CONFIG_LimitRecovery();
    BSP_LOG_trace("limit recovery rc: %d\r\n", rc);
    APP_CHECK_RC(rc)
    BSP_WDG_feed();
    rc = APP_CONFIG_RelayRecovery();
    BSP_LOG_trace("relay recovery rc: %d\r\n", rc);
    APP_CHECK_RC(rc)
    rc = APP_CONFIG_TimesEnableRecovery();
    BSP_LOG_trace("times_enable recovery rc: %d\r\n", rc);
    APP_CHECK_RC(rc)
    rc = APP_CONFIG_TimesRecovery();
    BSP_LOG_trace("times recovery rc: %d\r\n", rc);
    APP_CHECK_RC(rc)
    rc = APP_CONFIG_ManualRelayRecovery();
    BSP_LOG_trace("manual_relay recovery rc: %d\r\n", rc);
    APP_CHECK_RC(rc)
#ifdef SUPPORT_L6_ENABLE
    rc = APP_CONFIG_L6Recovery();
    BSP_LOG_trace("l6 recovery rc: %d\r\n", rc);
    APP_CHECK_RC(rc)
#endif
    rc = APP_CONFIG_RelayPowerConfigRecovery();
    BSP_LOG_trace("relay power config rc: %d\r\n", rc);
    APP_CHECK_RC(rc)
    rc = APP_CONFIG_DiagnosisMessageRecovery();
    BSP_LOG_trace("diagnosis message recovery rc: %d\r\n", rc);
    APP_CHECK_RC(rc)
#ifdef ENABLE_INPUT_RAND_MAX_MIN
    APP_CONFIG_RandValueRecovery();
    BSP_LOG_trace("rand value recovery rc: %d\r\n", rc);
    APP_CHECK_RC(rc)
#endif
    rc = APP_CONFIG_AllFirmwareInfoRecovery();
    BSP_LOG_trace("firmware info recovery rc: %d\r\n", rc);
    APP_CHECK_RC(rc)
    return APP_OK;
}


int APP_CONFIG_EepromRecoveryFreePage(void)
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
    rc = EEPROM_WriteBlock(page, bytes);
    APP_CHECK_RC(rc)
  }
  BSP_WDG_feed();
  for(page=APP_CONFIG_EEPROM_DEVICE_CONNECT_PAGE+1; page<APP_CONFIG_EEPROM_PROBE_ENABLE_PAGE; page++)
  {
    if(0 == (page & 0xF)) // every 16 pages, feed wdg 
    {
      BSP_WDG_feed();
    }
    rc = EEPROM_WriteBlock(page, bytes);
    APP_CHECK_RC(rc)
  }
  BSP_WDG_feed();
  for(page=APP_CONFIG_EEPROM_PROBE_ENABLE_PAGE+PROBE_SIZE; page<APP_CONFIG_EEPROM_PUMP_STATUS_PAGE; page++)
  {
    if(0 == (page & 0xF)) // every 16 pages, feed wdg 
    {
      BSP_WDG_feed();
    }
    rc = EEPROM_WriteBlock(page, bytes);
    APP_CHECK_RC(rc)
  }
  BSP_WDG_feed();
  for(page=APP_CONFIG_EEPROM_PUMP_STATUS_PAGE+PROBE_SIZE; page<APP_CONFIG_EEPROM_SENSOR_PAGE; page++)
  {
    if(0 == (page & 0xF)) // every 16 pages, feed wdg 
    {
      BSP_WDG_feed();
    }
    rc = EEPROM_WriteBlock(page, bytes);
    APP_CHECK_RC(rc)
  }
  BSP_WDG_feed();
  for(page=APP_CONFIG_EEPROM_SENSOR_PAGE+(PROBE_SIZE*PROBE_SENSOR_SIZE); page<APP_CONFIG_EEPROM_COMPENSATION_PAGE; page++)
  {
    if(0 == (page & 0xF)) // every 16 pages, feed wdg 
    {
      BSP_WDG_feed();
    }
    rc = EEPROM_WriteBlock(page, bytes);
    APP_CHECK_RC(rc)
  }
  BSP_WDG_feed();
  for(page=APP_CONFIG_EEPROM_COMPENSATION_PAGE+(PROBE_SIZE*INDICATOR_SIZE); page<APP_CONFIG_EEPROM_INDICATOR_ALERT_PAGE; page++)
  {
    if(0 == (page & 0xF)) // every 16 pages, feed wdg 
    {
      BSP_WDG_feed();
    }
    rc = EEPROM_WriteBlock(page, bytes);
    APP_CHECK_RC(rc)
  }
  BSP_WDG_feed();
  for(page=APP_CONFIG_EEPROM_INDICATOR_ALERT_PAGE+(PROBE_SIZE*INDICATOR_SIZE); page<APP_CONFIG_EEPROM_LIMIT_PAGE; page++)
  {
    if(0 == (page & 0xF)) // every 16 pages, feed wdg 
    {
      BSP_WDG_feed();
    }
    rc = EEPROM_WriteBlock(page, bytes);
    APP_CHECK_RC(rc)
  }
  BSP_WDG_feed();
  for(page=APP_CONFIG_EEPROM_LIMIT_PAGE+(PROBE_SIZE*INDICATOR_SIZE*APP_CONFIG_MAX_LIMIT_TASK); page<APP_CONFIG_EEPROM_RELAY_PAGE; page++)
  {
    if(0 == (page & 0xF)) // every 16 pages, feed wdg 
    {
      BSP_WDG_feed();
    }
    rc = EEPROM_WriteBlock(page, bytes);
    APP_CHECK_RC(rc)
  }
  BSP_WDG_feed();
  for(page=APP_CONFIG_EEPROM_RELAY_PAGE+RELAY_SIZE; page<APP_CONFIG_EEPROM_TIMES_ENABLE_PAGE; page++)
  {
    if(0 == (page & 0xF)) // every 16 pages, feed wdg 
    {
      BSP_WDG_feed();
    }
    rc = EEPROM_WriteBlock(page, bytes);
    APP_CHECK_RC(rc)
  }
  BSP_WDG_feed();
  for(page=APP_CONFIG_EEPROM_TIMES_ENABLE_PAGE+RELAY_SIZE; page<APP_CONFIG_EEPROM_TIMES_PAGE; page++)
  {
    if(0 == (page & 0xF)) // every 16 pages, feed wdg 
    {
      BSP_WDG_feed();
    }
    rc = EEPROM_WriteBlock(page, bytes);
    APP_CHECK_RC(rc)
  }
  BSP_WDG_feed();
  for(page=APP_CONFIG_EEPROM_TIMES_PAGE+(RELAY_SIZE*APP_CONFIG_MAX_TIMES_TASK); page<APP_CONFIG_EEPROM_MANUAL_RELAY_PAGE; page++)
  {
    if(0 == (page & 0xF)) // every 16 pages, feed wdg 
    {
      BSP_WDG_feed();
    }
    rc = EEPROM_WriteBlock(page, bytes);
    APP_CHECK_RC(rc)
  }
  BSP_WDG_feed();
  for(page=APP_CONFIG_EEPROM_MANUAL_RELAY_PAGE+(RELAY_SIZE*RELAY_CHANNEL_SIZE); page<APP_CONFIG_EEPROM_L6_PAGE; page++)
  {
    if(0 == (page & 0xF)) // every 16 pages, feed wdg 
    {
      BSP_WDG_feed();
    }
    rc = EEPROM_WriteBlock(page, bytes);
    APP_CHECK_RC(rc)
  }
  BSP_WDG_feed();
  for(page=APP_CONFIG_EEPROM_L6_PAGE+PROBE_SIZE; page<APP_CONFIG_EEPROM_RELAY_POWER_CONFIG_PAGE; page++)
  {
    if(0 == (page & 0xF)) // every 16 pages, feed wdg 
    {
      BSP_WDG_feed();
    }
    rc = EEPROM_WriteBlock(page, bytes);
    APP_CHECK_RC(rc)
  }
  BSP_WDG_feed();
  for(page=APP_CONFIG_EEPROM_RELAY_POWER_CONFIG_PAGE+(RELAY_SIZE*RELAY_CHANNEL_SIZE); page<APP_CONFIG_EEPROM_DIAGNOSIS_MESSAGE_PAGE; page++)
  {
    if(0 == (page & 0xF)) // every 16 pages, feed wdg 
    {
      BSP_WDG_feed();
    }
    rc = EEPROM_WriteBlock(page, bytes);
    APP_CHECK_RC(rc)
  }
  BSP_WDG_feed();
  
#ifndef ENABLE_INPUT_RAND_MAX_MIN
  for(page=APP_CONFIG_EEPROM_DIAGNOSIS_MESSAGE_PAGE+1; page<APP_CONFIG_EEPROM_FIRMWARE_SETTING_INFO_PAGE; page++)
#else
    for(page=APP_CONFIG_EEPROM_DIAGNOSIS_MESSAGE_PAGE+1; page<APP_CONFIG_EEPROM_RAND_VALUE_PAGE; page++)
#endif
  {
    if(0 == (page & 0xF)) // every 16 pages, feed wdg 
    {
      BSP_WDG_feed();
    }
    rc = EEPROM_WriteBlock(page, bytes);
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
