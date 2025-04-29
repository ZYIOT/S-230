#include "rs485_sensor.h"
#include "drivers/sensors/sensors.h"

static size_t fake_buffer_index = 0;
uint8_t fake_buffer[] = {0x02, 0x03, 0x04, 0x01, 0x01, 0x00, 0x02, 0x18, 0xce};
static int _rs485_read(uint8_t buffer[RS485_BUFFER_MAX_SIZE], size_t len, size_t tick)
{
    printf("read: i=%d, len=%d, data=", fake_buffer_index, len);
    for (int i = 0; i < len; i++)
    {
        printf("%x ", fake_buffer[fake_buffer_index + i]);
    }
    printf("\n");
    memcpy(buffer, &fake_buffer[fake_buffer_index], len);
    fake_buffer_index += len;
    return len;
}

static int _rs485_write(uint8_t buffer[RS485_BUFFER_MAX_SIZE], size_t len, size_t tick)
{
    printf("write: len=%d, data=", len);
    for (int i = 0; i < len; i++)
    {
        printf("%x ", buffer[i]);
    }
    printf("\n");
    return 1;
}

static int _rs485_clear(void)
{
    fake_buffer_index = 0;
    printf("clear!\n");
    return 1;
}

int main(int argc, char **argv)
{
    rs485_sensor_t sensors[] = {
        {.manufacturer = RS485_DRIVER_DO_MANUFACTURER_ZYIOT, .model = 2, .protocol = 1, .id = RS485_DRIVER_SENSOR_ID_DO, .action = NULL},
        {.manufacturer = RS485_DRIVER_DO_MANUFACTURER_ZYIOT, .model = 1, .protocol = 1, .id = RS485_DRIVER_SENSOR_ID_PH, .action = NULL},
        {.manufacturer = RS485_DRIVER_LEVEL_MANUFACTURER_ANBULEILA, .model = 1, .protocol = 1, .id = RS485_DRIVER_SENSOR_ID_LEVEL, .action = NULL},
        {NULL}};
    // int rc = RS485_SENSOR_SCAN(rs485_port1, 1, &sensor)
    rs485_port_t rs485_port1 = {
        .read = _rs485_read,
        .write = _rs485_write,
        .clear = _rs485_clear,
    };

    rs485_sensor_pt sensor = &sensors[0];
    RS485_DRIVER_init();
    int rc = 0;
    while (sensor != NULL && sensor->id > 0)
    {
        rc = RS485_DRIVER_match(sensor);
        // check rc
        sensor++;
    }
    sensor = &sensors[0];
    while (sensor != NULL && sensor->id > 0)
    {
        if (sensor->action != NULL)
        {
            printf("matched: manufacturer=%d, model=%d, protocol=%d, id=%d, action=%p\n", sensor->manufacturer, sensor->model, sensor->protocol, sensor->id, sensor->action);
        }
        sensor++;
    }
    sensor = &sensors[1];
    sensor->port = &rs485_port1;
    NEW_RS485_SENSOR_INDICATOR(indicator);
    while (1)
    {
        rc = sensor->action->read_value1(sensor, &indicator);
        if (rc != 0)
        {
            printf("rc=%d", rc);
        }
        else
        {
            printf("indicator: value1=%f, vm1=%d, status=%d\n", indicator.value1, indicator.vm1, indicator.status);
        }
    }
}
