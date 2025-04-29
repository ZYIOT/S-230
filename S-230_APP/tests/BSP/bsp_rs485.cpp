#include "bsp_rng.h"
#ifdef __cplusplus
extern "C"
{
#endif

#include "bsp_rs485.h"
#include "bsp_log.h"

    int BSP_RS485_init(void)
    {
        return 0;
    }

    static int _read(uint8_t buffer[RS485_BUFFER_MAX_SIZE], size_t len, size_t tick)
    {
        return 0;
    }

    static int _write(uint8_t buffer[RS485_BUFFER_MAX_SIZE], size_t len, size_t tick)
    {
        return len;
    }

    static int _clear(void)
    {
        return 0;
    }

    rs485_port_t rs485_port = {
        .read = _read,
        .write = _write,
        .clear = _clear,
    };

    int RS485_DRIVER_match_by_model(rs485_sensor_pt sensor, uint8_t model)
    {
        return APP_ERROR;
    }
#ifdef __cplusplus
}
#endif
