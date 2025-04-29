#ifndef _BSP_LCD_H
#define _BSP_LCD_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "app.h"

// Page0 显示项目位置、大小定义 

#define LCD_PN_START_X          160 
#define LCD_PN_VALUE_START_X    224 
#define LCD_PN_START_Y          64 
#define LCD_PN_FONT_SIZE        FONT_16X32 

#define LCD_SN_START_X          160 
#define LCD_SN_VALUE_START_X    224 
#define LCD_SN_START_Y          (LCD_PN_START_Y + LCD_PN_FONT_SIZE + 8) 
#define LCD_SN_FONT_SIZE        FONT_16X32 

#define LCD_HW_START_X          160 
#define LCD_HW_VALUE_START_X    224 
#define LCD_HW_START_Y          (LCD_SN_START_Y + LCD_SN_FONT_SIZE + 8) 
#define LCD_HW_FONT_SIZE        FONT_16X32 

#define LCD_SW_START_X          160 
#define LCD_SW_VALUE_START_X    224 
#define LCD_SW_START_Y          (LCD_HW_START_Y + LCD_HW_FONT_SIZE + 8) 
#define LCD_SW_FONT_SIZE        FONT_16X32 


// Page1 显示项目位置、大小定义 

#define LCD_DO_INDEX            0
#define LCD_TMP_INDEX           1
#define LCD_PH_INDEX            2
#define LCD_MAX_ITEM_CNT        3

#define LCD_PAGE_HEADER_SIZE    8   // 页眉边距大小，留出边距，放置上盖遮挡到屏幕 
// Logo 图标 
#define LCD_LOGO_START_X        (LCD_PAGE_HEADER_SIZE + 0) 
#define LCD_LOGO_START_Y        (LCD_PAGE_HEADER_SIZE + 0) 
#define LCD_LOGO1_WIDTH         48 
#define LCD_LOGO1_HEIGH         48 
// 中英文图标 
#define LCD_ZYIOT_START_X       (LCD_LOGO_START_X + LCD_LOGO1_WIDTH + 4)
#define LCD_ZYIOT_START_Y       (LCD_LOGO_START_Y + 4) 
#define LCD_ZYIOT_FONT_SIZE     FONT_12X24
#define LCD_ZYIOT_EN_START_X    LCD_ZYIOT_START_X 
#define LCD_ZYIOT_EN_START_Y    (LCD_ZYIOT_START_Y + LCD_ZYIOT_FONT_SIZE + 4) 
#define LCD_ZYIOT_EN_FONT_SIZE  FONT_8X16 

// 信号强度图标 
#define LCD_SIGNAL_RANGE0       0
#define LCD_SIGNAL_RANGE1       1
#define LCD_SIGNAL_RANGE2       2
#define LCD_SIGNAL_RANGE3       3
#define LCD_SIGNAL_RANGE_NON    255

#define LCD_SIGNAL_START_X      428 
#define LCD_SIGNAL_START_Y      (LCD_PAGE_HEADER_SIZE + 0) 
#define LCD_SIGNAL_WIDTH        36 
#define LCD_SIGNAL_HEIGH        36 

// 公司主题 
#define LCD_TITLE_FONT_SIZE     FONT_32X64
#define LCD_TITLE_LINE1_START_X (LCD_PAGE_HEADER_SIZE + 80)
#define LCD_TITLE_LINE1_START_Y (LCD_PAGE_HEADER_SIZE + 56)
#define LCD_TITLE_LINE2_START_X (LCD_PAGE_HEADER_SIZE + 80 + FONT_32X64)
#define LCD_TITLE_LINE2_START_Y (LCD_PAGE_HEADER_SIZE + LCD_TITLE_LINE1_START_Y + LCD_TITLE_FONT_SIZE + 4)

// 传感器项目内容 
#define LCD_ITEM_FONT_SIZE      FONT_32X64
#define LCD_ITEM_HEIGH          LCD_ITEM_FONT_SIZE
#define LCD_ITEM_START_X        (LCD_PAGE_HEADER_SIZE + 32)
#define LCD_ITEM_START_Y        (LCD_PAGE_HEADER_SIZE + 56)
#define LCD_ITEM_X              LCD_ITEM_START_X
#define LCD_ITEM0_Y             LCD_ITEM_START_Y
#define LCD_ITEM1_Y             (LCD_ITEM0_Y + LCD_ITEM_HEIGH)
#define LCD_ITEM2_Y             (LCD_ITEM1_Y + LCD_ITEM_HEIGH)
// 冒号 
#define LCD_ITEM_DOT_X          (LCD_ITEM_X + LCD_ITEM_FONT_SIZE * 3)
// 数值 
#define LCD_ITEM_VALUE_X        (LCD_ITEM_X + (LCD_ITEM_FONT_SIZE /2 * 7))
#define LCD_ITEM0_VALUE_Y       LCD_ITEM_START_Y
#define LCD_ITEM1_VALUE_Y       (LCD_ITEM0_Y + LCD_ITEM_HEIGH)
#define LCD_ITEM2_VALUE_Y       (LCD_ITEM1_Y + LCD_ITEM_HEIGH)
// 告警图标 
#define LCD_ITEM_WARN_X         (LCD_ITEM_DOT_X + LCD_ITEM_FONT_SIZE)
#define LCD_ITEM0_WARN_Y        (LCD_ITEM_START_Y + 24)
#define LCD_ITEM1_WARN_Y        (LCD_ITEM0_WARN_Y + LCD_ITEM_HEIGH)
#define LCD_ITEM2_WARN_Y        (LCD_ITEM1_WARN_Y + LCD_ITEM_HEIGH)
#define LCD_WARNNING_LOGO_WIDTH 32
#define LCD_WARNNING_LOGO_HEITH 32

typedef struct 
{
    uint16_t x;
    uint16_t y;
}itemLocaltion_t, *itemLocaltion_pt;

#include "lcd_driver.h"
#define BSP_LCD_CLR_SCREEN()    LcdClrScreen()
#define BSP_LCD_LIGHT_ON()      LCD_BL_ON()
#define BSP_LCD_LIGHT_OFF()     LCD_BL_OFF()

#include "board_config.h"
#ifdef SUPPORT_IRED  
#define IRED_CHECK_INVALID  0
#define IRED_CHECK_VALID    1
#define BSP_IRED_CHECK_STATUS() (HARDWARE_GPIO_READ_PIN(IRED_CHECK_GPIO_Port, IRED_CHECK_Pin) == GPIO_PIN_SET ? IRED_CHECK_VALID : IRED_CHECK_INVALID)
#endif

int BspLcdInit(void);
int BspLcdShowTopic(void);
int BspLcdShowItem(uint8_t itemIndex, uint8_t popIndex);
int BspLcdClearItem(uint8_t itemIndex);
int BspLcdShowValue(float value, uint8_t valueIndex, uint8_t popIndex, uint8_t overFlow);
int BspLcdShowUnknowValue(uint8_t valueIndex, uint8_t popIndex);
int BspLcdShowWarnning(uint8_t itemIndex, uint8_t popIndex);
int BspLcdClearValue(uint8_t valueIndex);
int BspLcdShowSignal(uint8_t range);
int BspLcdShowTitle(void);



#ifdef __cplusplus
}
#endif

#endif

