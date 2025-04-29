#ifndef __APP_CONFIG_EEPROM_COMMON_H
#define __APP_CONFIG_EEPROM_COMMON_H
#ifdef __cplusplus
extern "C"
{
#endif
#define EEPROM_BLOCK_SIZE 32
#define EEPROM_BLOCK_DATA_SIZE 30

// 此处定义 1Page = 32Bytes, 每个 Page 最后的 2Bytes 为该 Page 前面数据的 CRC16 值。buf 的有效使用空间大小为 30Bytes 
int EEPROM_ReadBlock(uint16_t page, uint8_t *buf)
{
    APP_LOG_Debug("eeprom read: %u page\r\n", page + BSP_AT24CM1_DEVICE_ID*EEPROM_HALF_MAX_PAGE);
    if (BSP_AT24CM1_read_bytes(page * EEPROM_BLOCK_SIZE, buf, EEPROM_BLOCK_SIZE) != HAL_OK)
    {
        return APP_ERROR;
    }
    APP_LOG_DebugHex(buf,EEPROM_BLOCK_SIZE);
    APP_LOG_Debug("\r\n");
    uint16_t crc = get_modbus_crc16(buf, EEPROM_BLOCK_DATA_SIZE);
    return (read_uint16_t(buf + EEPROM_BLOCK_DATA_SIZE) == crc) ? APP_OK : APP_ERROR;
}

// 此处定义 1Page = 32Bytes, 每个 Page 最后的 2Bytes 为该 Page 前面数据的 CRC16 值。buf 的有效使用空间大小为 30Bytes 
int EEPROM_WriteBlock(uint16_t page, uint8_t *buf)
{
    APP_LOG_Debug("eeprom write: %u page\r\n", page + BSP_AT24CM1_DEVICE_ID*EEPROM_HALF_MAX_PAGE);
    uint16_t crc = get_modbus_crc16(buf, EEPROM_BLOCK_DATA_SIZE);
    write_uint16_t(crc, buf + EEPROM_BLOCK_DATA_SIZE);
    APP_LOG_DebugHex(buf,EEPROM_BLOCK_SIZE);
    APP_LOG_Debug("\r\n");
    return (BSP_AT24CM1_write_bytes(page * EEPROM_BLOCK_SIZE, buf, EEPROM_BLOCK_SIZE) == HAL_OK) ? APP_OK : APP_ERROR;
}

int EEPROM_ReadID(void)
{
    uint8_t eeprom_id[EEPROM_ID_LEN] = {0};
    if (BSP_AT24CM1_read_id(eeprom_id, EEPROM_ID_LEN) != HAL_OK)
    {
        return APP_ERROR;
    }
    APP_LOG_Debug("eeprom id :\r\n[");
    APP_LOG_DebugHex(eeprom_id, EEPROM_ID_LEN);
    APP_LOG_Debug("]\r\n");
    return APP_OK;
}

#ifdef __cplusplus
}
#endif
#endif
