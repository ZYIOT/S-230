#ifndef __HARDWARE_LCD12864_H
#define __HARDWARE_LCD12864_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "bsp_gpio.h"
#include "hardware_wrapper.h"
    typedef struct
    {
        GPIO_TypeDef *rst_gpio;
        uint16_t rst_pin;
        GPIO_TypeDef *cs_gpio;
        uint16_t cs_pin;
        GPIO_TypeDef *clk_gpio;
        uint16_t clk_pin;
        GPIO_TypeDef *sdi_gpio;
        uint16_t sdi_pin;
        HARDWARE_SEMAPHORE_TYPE_T sem;
    } LCD12864_TypeDef;

    void LCD12864_init(LCD12864_TypeDef *LCDx);
    void LCD12864_reset(LCD12864_TypeDef *LCDx);
    void LCD12864_clear_txt(LCD12864_TypeDef *LCDx);
    void LCD12864_reverse_row(LCD12864_TypeDef *LCDx, uint8_t row);
    void LCD12864_show_row(LCD12864_TypeDef *LCDx, uint8_t row, uint8_t col, char *bytes);
    void LCD12864_show_string(LCD12864_TypeDef *LCDx, uint8_t row, uint8_t col, char *bytes);

#ifdef __cplusplus
}
#endif

#endif
