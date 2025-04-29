
#define COMMON_VERSION_REG  (0x11)
#define COMMON_SN_REG       (0x18)

static int _parse_value_error(uint8_t error_code, void *ret, void *args)
{
    rs485_sensor_indicator_pt indicator = (rs485_sensor_indicator_pt)ret;
    indicator->error_code = error_code;
    indicator->status = RS485_OK;
    return RS485_OK;
}


static uint8_t *phardware = NULL;
static uint8_t *pfirmware = NULL;
static uint8_t *psn = NULL;

static int _parse_version(uint8_t buffer[RS485_BUFFER_MAX_SIZE], size_t len, void *ret, void *args)
{
    rs485_sensor_pt rs485 = (rs485_sensor_pt)ret;
    uint8_t cnt = 0;
    cnt = RS485_SENSOR_MAX_HW_LEN;
    for (int i = 0; i < cnt; i++)
    {
        phardware[i] = buffer[i];
    }
    cnt = RS485_SENSOR_MAX_FW_LEN;
    for (int i = 0; i < cnt; i++)
    {
        pfirmware[i] = buffer[i + cnt];
    }
    return RS485_OK;
}

static int _parse_sn(uint8_t buffer[RS485_BUFFER_MAX_SIZE], size_t len, void *ret, void *args)
{
    rs485_sensor_pt rs485 = (rs485_sensor_pt)ret;
    uint8_t cnt = 0;
    cnt = RS485_SENSOR_MAX_SN_LEN;
    for (int i = 0; i < cnt; i++)
    {
        psn[i] = buffer[i];
    }
    return RS485_OK;
}

static int _read_info(rs485_sensor_pt rs485)
{
    uint8_t retry_cnt = 0;
    phardware = rs485->hardware;
    pfirmware = rs485->firmware;
    psn = rs485->SN;
    int rc = RS485_OK;
    do
    {
        rc = RS485_read_register(rs485->port, rs485->id, COMMON_VERSION_REG, (RS485_SENSOR_MAX_HW_LEN+RS485_SENSOR_MAX_FW_LEN)/2, rs485, _parse_version, NULL, NULL);
        retry_cnt += 1;
    }while((RS485_OK != rc) && (retry_cnt <= 3));
    if(RS485_OK != rc)
    {
        memset(phardware, 0, RS485_SENSOR_MAX_HW_LEN);
        memset(pfirmware, 0, RS485_SENSOR_MAX_FW_LEN);
    }
    // HARDWARE_OS_DELAY_MS(10);
    retry_cnt = 0;
    do 
    {
        rc = RS485_read_register(rs485->port, rs485->id, COMMON_SN_REG, (RS485_SENSOR_MAX_SN_LEN)/2, rs485, _parse_sn, NULL, NULL);
        retry_cnt += 1;
    }while((RS485_OK != rc) && (retry_cnt <= 3));
    if(RS485_OK != rc)
    {
        memset(psn, 0, RS485_SENSOR_MAX_SN_LEN);
    }
    return RS485_OK;
}

// static int _parse_pn(uint8_t buffer[RS485_BUFFER_MAX_SIZE], size_t len, void *ret, void *args)
// {
//     rs485_sensor_pt rs485 = (rs485_sensor_pt)ret;
//     rs485_sensor_driver_pt sd = (rs485_sensor_driver_pt)args;
//     if (strncmp(sd->action->pn_match_name, (char *)buffer, strlen(sd->action->pn_match_name)) == 0)
//     {
//         rs485->status = RS485_SENSOR_STATUS_ONLINE;
//         strncpy(sd->action->PN, buffer, strlen(sd->action->pn_match_name) + 2);
//         rs485->PN = sd->action->PN;
//         return RS485_OK;
//     }
//     else
//     {
//         return RS485_ERROR;
//     }
// }

// static int _check_sensor(rs485_sensor_pt rs485, rs485_sensor_driver_pt sd)
// {
//     return RS485_read_register(rs485->port, rs485->id, 20, 4, rs485, _parse_pn, NULL, sd);
// }
