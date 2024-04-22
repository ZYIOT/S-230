#ifndef _LCD_DRIVER_H
#define _LCD_DRIVER_H

#ifdef __cplusplus
extern "C"
{
#endif


#if (LCD_SHOW_DIR == LCD_SHOW_DIR_HORIZONTAL)
    #define LCD_WIDTH   480
    #define LCD_HEITH   320 
#else 
    #define LCD_WIDTH   320
    #define LCD_HEITH   480 
#endif

#define FONT_6X12	12
#define FONT_8X16	16
#define FONT_12X24	24
#define FONT_16X32	32
#define FONT_32X64  64

#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE           	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 // 棕色 
#define BRRED 			 0XFC07 // 棕红色 
#define GRAY  			 0X8430 // 灰色 
#define DARKBLUE      	 0X01CF	// 深蓝色 
#define LIGHTBLUE      	 0X7D7C	// 浅蓝色 
#define GRAYBLUE       	 0X5458 // 灰蓝色 
#define LIGHTGREEN     	 0X841F // 浅绿色 
#define LGRAY 			 0XC618 // 浅灰色(PANNEL),窗体背景色 
#define LGRAYBLUE        0XA651 // 浅灰蓝色(中间层颜色) 
#define LBBLUE           0X2B12 // 浅棕蓝色(选择条目的反色) 

#define LCD_BL_ON()         HARDWARE_GPIO_WRITE_PIN_H(LCD_BL_GPIO_Port, LCD_BL_Pin)
#define LCD_BL_OFF()        HARDWARE_GPIO_WRITE_PIN_L(LCD_BL_GPIO_Port, LCD_BL_Pin)
#define LCD_RST_ON()        HARDWARE_GPIO_WRITE_PIN_L(LCD_RST_GPIO_Port, LCD_RST_Pin)
#define LCD_RST_OFF()       HARDWARE_GPIO_WRITE_PIN_H(LCD_RST_GPIO_Port, LCD_RST_Pin)

typedef void (*showChineseStringFunc)(uint16_t x, uint16_t y, uint8_t *str, uint16_t fontColor, uint16_t backColor, uint8_t size, uint8_t mode);

void LcdDriverInit(void);
void LcdFill(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint16_t color);
void LcdClrScreen(void);
void LcdClrScreenRegion(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey);
void LcdDrawPoint(uint16_t x, uint16_t y, uint16_t color);
void LcdDrawLine(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint16_t color);
void LcdDrawRectangle(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint16_t color);
void LcdDrawCircle(uint16_t x, uint16_t y, uint8_t r, uint16_t color);

void LcdShowChar(uint16_t x, uint16_t y, uint8_t num, uint16_t fontColor, uint16_t backColor, uint8_t size, uint8_t mode);
void LcdShowString(uint16_t x, uint16_t y, uint8_t *str, uint16_t fontColor, uint16_t backColor, uint8_t size, uint8_t mode);
void LcdShowIntNum(uint16_t x, uint16_t y, uint16_t num, uint8_t len, uint16_t fontColor, uint16_t backColor, uint8_t size);
void LcdShowFloatNum(uint16_t x, uint16_t y, float num, uint8_t len, uint16_t fontColor, uint16_t backColor, uint8_t size);
void LcdShowPicture(uint16_t x, uint16_t y, uint16_t len, uint16_t width, const uint8_t *pic);
void LcdShowChinese(uint16_t x, uint16_t y, uint8_t *str, uint16_t fontColor, uint16_t backColor, uint8_t size, uint8_t mode);


#ifdef __cplusplus
}
#endif

#endif
