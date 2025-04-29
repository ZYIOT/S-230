#include "adc.h"
#include "bsp_mcu.h"
#include "bsp_wdg.h"

#define TEMP_REFRESH_PERIOD 1000  /* Internal temperature refresh period */
#define MAX_CONVERTED_VALUE 4095  /* Max converted value */
#define AMBIENT_TEMP 25           /* Ambient Temperature */
#define VSENS_AT_AMBIENT_TEMP 760 /* VSENSE value (mv) at ambient temperature */
#define AVG_SLOPE 25              /* Avg_Solpe multiply by 10 */
#define VREF 3300

int BSP_MCU_temperature(float *t)
{
    if (HAL_ADC_Start(&hadc1) == HAL_OK)
    {
        if (HAL_ADC_PollForConversion(&hadc1, 1000) == HAL_OK)
        {
            *t = ((((HAL_ADC_GetValue(&hadc1) * VREF) / MAX_CONVERTED_VALUE) - VSENS_AT_AMBIENT_TEMP) * 10 / AVG_SLOPE) + AMBIENT_TEMP;
            return APP_OK;
        }
    }
    return APP_ERROR;
}

int BSP_MCU_reset(void)
{
    __set_FAULTMASK(1);
    HAL_NVIC_SystemReset();
    return APP_OK;
}
