#include "config_bit.h"

uint8_t config_bit_1_bits(uint16_t value, uint8_t *bits)
{
    uint8_t count = 0;
    uint16_t mask = 0x8000;
    uint8_t i = 0;
    for (i = 1; i <= 16; i++)
    {
        if ((value & mask) == mask)
        {
            if (bits != NULL)
            {
                bits[count] = i;
            }
            count++;
        }
        mask = mask >> 1;
    }
    return count;
}

uint8_t config_bit_1_bit_count(uint16_t value)
{
    return config_bit_1_bits(value, NULL);
}

uint8_t config_bit_to_index(uint16_t u)
{
    uint16_t mask = 0x8000;
    uint8_t i = 0;
    for (i = 1; i <= 16; i++)
    {
        if ((u & mask) == mask)
        {
            return i;
        }
        mask = mask >> 1;
    }
    return 0;
}

uint16_t config_bit_from_index(uint8_t idx)
{
    return 0x8000 >> (idx - 1);
}

int config_bit_is_set(uint16_t value, uint8_t idx)
{
    uint16_t mask = 0x8000 >> (idx - 1);
    return (value & mask) == mask;
}
