#include "bsp_wdg.h"
#include "bsp_board_debug.h"
#include "tim.h"
#include "hardware_wrapper.h"

#ifdef DISABLE_WDG
void BSP_WDG_init(void)
{
}
void BSP_WDG_feed(void)
{
}
void BSP_WDG_stop(void)
{
}
#else
static int sp706_wdi = GPIO_PIN_RESET;
void BSP_WDG_init(void)
{
    MX_IWDG_Init();
    HARDWARE_GPIO_SET_OUTPUT(WDI_GPIO_Port, WDI_Pin);
    HARDWARE_GPIO_WRITE_PIN(WDI_GPIO_Port, WDI_Pin, sp706_wdi);
}

void BSP_WDG_feed(void)
{
    HAL_IWDG_Refresh(&hiwdg);
    sp706_wdi++;
    sp706_wdi %= 2;
    HARDWARE_GPIO_WRITE_PIN(WDI_GPIO_Port, WDI_Pin, sp706_wdi);
}

void BSP_WDG_stop(void)
{
    __HAL_IWDG_RELOAD_COUNTER(&hiwdg);
}
#endif

#define WDG_TIMER_HANDLE    (&htim2)

void BSP_WDG_Timer_init(void)
{
    __HAL_TIM_SET_AUTORELOAD(WDG_TIMER_HANDLE, 5000);
}

