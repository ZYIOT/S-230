#include "bsp_rng.h"
#ifdef __cplusplus
extern "C"
{
#endif
    void BSP_RNG_init(void)
    {
    }

    void BSP_RNG_deinit(void)
    {
    }

    uint32_t BSP_RNG_get(void)
    {
        return (uint32_t)rand();
    }

#ifdef __cplusplus
}
#endif
