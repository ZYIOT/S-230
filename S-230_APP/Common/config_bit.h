

#ifndef __COMMON_CONFIG_BIT_H__
#define __COMMON_CONFIG_BIT_H__

#ifdef __cplusplus
extern "C"
{
#endif
#include "stdio.h"
#include "stdint.h"
    uint8_t config_bit_to_index(uint16_t u);
    uint16_t config_bit_from_index(uint8_t idx);
    uint8_t config_bit_1_bits(uint16_t value, uint8_t *bits);
    uint8_t config_bit_1_bits_count(uint16_t value);
    int config_bit_is_set(uint16_t value, uint8_t idx);
#ifdef __cplusplus
}
#endif
#endif
