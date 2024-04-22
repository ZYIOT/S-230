#include "bsp_rng.h"
#include "rng.h"

void BSP_RNG_init(void)
{
}


uint32_t BSP_RNG_get(void)
{
    return HAL_RNG_GetRandomNumber(&hrng);
}

int BSP_RNG_get_range(int min, int max)
{
    return min + HAL_RNG_GetRandomNumber(&hrng)%(max - min + 1);
}
