#ifndef __BSP_LCD12864_H
#define __BSP_LCD12864_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "main.h"
#include "lcd12864.h"
#include "hardware_wrapper.h"
    extern LCD12864_TypeDef lcd12864;

#define BSP_LCD12864_led_on() HARDWARE_GPIO_WRITE_PIN_H(LCD_BL_GPIO_Port, LCD_BL_Pin);
#define BSP_LCD12864_led_off() HARDWARE_GPIO_WRITE_PIN_L(LCD_BL_GPIO_Port, LCD_BL_Pin);
#define BSP_LCD12864_init(void) LCD12864_init(&lcd12864)
#define BSP_LCD12864_reset(void) LCD12864_reset(&lcd12864)
#define BSP_LCD12864_show_row(row, col, buffer) LCD12864_show_row(&lcd12864, row, col, buffer)
#define BSP_LCD12864_show_string(row, col, buffer) LCD12864_show_string(&lcd12864, row, col, buffer)
#define BSP_LCD12864_clear_txt() LCD12864_clear_txt(&lcd12864)

#ifdef __cplusplus
}
#endif

#endif
