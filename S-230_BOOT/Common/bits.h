
#ifndef __COMMON_BITS_H__
#define __COMMON_BITS_H__

#ifdef __cplusplus
extern "C"
{
#endif
#include "stdint.h"

#define __DECLARE_BITS_MASK_CONVERTER(type)                          \
    type bits_##type##_set(type data, uint8_t start, uint8_t len);   \
    type bits_##type##_reset(type data, uint8_t start, uint8_t len); \
    type bits_##type##_reverse(type data, uint8_t start, uint8_t len);

    __DECLARE_BITS_MASK_CONVERTER(uint8_t)
    __DECLARE_BITS_MASK_CONVERTER(uint16_t)
    __DECLARE_BITS_MASK_CONVERTER(uint32_t)
#ifdef __cplusplus
}
#endif
#endif
