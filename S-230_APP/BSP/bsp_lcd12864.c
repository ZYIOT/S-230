 #include "main.h"
#include "bsp_lcd12864.h"
 
 LCD12864_TypeDef lcd12864 = {
    .rst_gpio = LCD_RST_GPIO_Port,
    .rst_pin = LCD_RST_Pin,
    .cs_gpio = LCD_CS_GPIO_Port,
    .cs_pin = LCD_CS_Pin,
    .clk_gpio = LCD_CLK_GPIO_Port,
    .clk_pin = LCD_CLK_Pin,
    .sdi_gpio = LCD_SDI_GPIO_Port,
    .sdi_pin = LCD_SDI_Pin,
};

