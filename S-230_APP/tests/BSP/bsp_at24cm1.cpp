#include "bsp_at24cm1.h"
#include "hardware_wrapper.h"

#ifdef __cplusplus
extern "C"
{
#endif
static uint8_t eeprom[65535] = {0};

int BSP_AT24CM1_read_bytes(uint16_t reg, uint8_t *data, uint16_t len)
{
    memcpy(data, &eeprom[reg], len);
    return HAL_OK;
}

int BSP_AT24CM1_write_bytes(uint16_t reg, uint8_t *data, uint16_t len)
{
    memcpy(&eeprom[reg], data, len);
    return HAL_OK;
}
#ifdef __cplusplus
}
#endif
