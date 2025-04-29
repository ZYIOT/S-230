#include "bits.h"
#define __DEFINE_BITS_MASK_CONVERTER(type)                            \
    type bits_##type##_set(type data, uint8_t start, uint8_t len)     \
    {                                                                 \
        type mask = 0;                                                \
        uint8_t i = 0;                                                \
        for (i = 0; i < len; i++)                                     \
        {                                                             \
            mask <<= 1;                                               \
            mask |= 1;                                                \
        }                                                             \
        return data | (mask << start);                                \
    }                                                                 \
                                                                      \
    type bits_##type##_reset(type data, uint8_t start, uint8_t len)   \
    {                                                                 \
        type mask = 0;                                                \
        uint8_t i = 0;                                                \
        for (i = 0; i < len; i++)                                     \
        {                                                             \
            mask <<= 1;                                               \
            mask |= 1;                                                \
        }                                                             \
        return data & ~(mask << start);                               \
    }                                                                 \
                                                                      \
    type bits_##type##_reverse(type data, uint8_t start, uint8_t len) \
    {                                                                 \
        type mask = 0;                                                \
        uint8_t i = 0;                                                \
        for (i = 0; i < len; i++)                                     \
        {                                                             \
            mask <<= 1;                                               \
            mask |= 1;                                                \
        }                                                             \
        type reset = bits_##type##_reset(data, start, len);           \
        return reset | (data ^ (mask << start));                      \
    }

__DEFINE_BITS_MASK_CONVERTER(uint8_t)
__DEFINE_BITS_MASK_CONVERTER(uint16_t)
__DEFINE_BITS_MASK_CONVERTER(uint32_t)