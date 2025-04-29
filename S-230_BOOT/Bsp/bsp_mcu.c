#include "bsp_mcu.h"
#include "bsp_wdg.h"


int BSP_MCU_reset(void)
{
    __set_FAULTMASK(1);
    HAL_NVIC_SystemReset();
    return 0;
}

void BSP_MCU_run_led_flash(void)
{
    static uint8_t run_led = 0;
    HAL_GPIO_WritePin(LED_RUN_GPIO_Port, LED_RUN_Pin, run_led);
    run_led++;
    run_led &= 0x1;
}
