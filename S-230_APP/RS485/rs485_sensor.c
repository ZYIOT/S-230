#include "rs485_sensor.h"

// static int scan_callback(uint8_t buffer[RS485_BUFFER_MAX_SIZE], size_t len, void *ret, void * args)
// {
//     rs485_sensor_pt sensor = (rs485_sensor_pt)ret;
//     sensor->manufacturer = read_uint16_t(&buffer[0]);
//     sensor->model = buffer[2];
//     sensor->protocol = buffer[3];
//     return RS485_OK;
// }

// int RS485_SENSOR_scan(rs485_port_pt rs485, uint8_t id, rs485_sensor_pt sensor)
// {
//     int rc = RS485_read_register(rs485, id, RS485_SENSOR_MODEL_ADDR, 2, sensor, scan_callback, NULL,NULL);
//     //check rc
//     sensor->id = id;
//     return RS485_OK;
// }

static char *_online = "Online";
static char *_offline = "Offline";
static char *_initializing = "Initializing";
static char *_unkown = "Unkown";

char *RS485_SENSOR_status_name(uint8_t status)
{
    switch (status)
    {
    case RS485_SENSOR_STATUS_OFFLINE:
        return _offline;
    case RS485_SENSOR_STATUS_INITIALIZE:
        return _initializing;
    case RS485_SENSOR_STATUS_ONLINE:
        return _online;

    default:
        return _unkown;
    }
}

static char *_unidentified = "Unidentified";
static char *_auto = "Auto";
static char *_manual = "Manual";
static char *_compatible = "Compatible";

char *RS485_SENSOR_recognition_mode_name(rs485_recognition_mode_t recognition_mode)
{
    switch (recognition_mode)
    {
    case RECOGNITION_MODE_UNIDENTIFIED:
        return _unidentified;
    case RECOGNITION_MODE_AUTO:
        return _auto;
    case RECOGNITION_MODE_MANUAL:
        return _manual;
    case RECOGNITION_MODE_COMPATIBLE:
        return _compatible;
    default:
        return _unkown;
    }
}
