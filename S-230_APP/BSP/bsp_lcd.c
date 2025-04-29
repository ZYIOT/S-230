#include "bsp_lcd.h"
#include "board_config.h"
#include "lcd_driver.h"
#include "lcd_chinese_string.h"
#include "lcd_logo.h"
#include "app_config.h"

uint8_t bspFontSize = LCD_ITEM_FONT_SIZE;
uint16_t bspBackColor = LCD_BACKGROUND_COLOR;
uint16_t bspItemColor = BLACK;
uint16_t bspItemValueColor = BLACK;

const itemLocaltion_t itemLocations[LCD_MAX_ITEM_CNT] = 
{
    [LCD_DO_INDEX] = {LCD_ITEM_X, LCD_ITEM0_Y},
    [LCD_TMP_INDEX] = {LCD_ITEM_X, LCD_ITEM1_Y},
    [LCD_PH_INDEX] = {LCD_ITEM_X, LCD_ITEM2_Y},
};

const itemLocaltion_t itemVauleLocations[LCD_MAX_ITEM_CNT] = 
{
    [LCD_DO_INDEX] = {LCD_ITEM_VALUE_X, LCD_ITEM0_VALUE_Y},
    [LCD_TMP_INDEX] = {LCD_ITEM_VALUE_X, LCD_ITEM1_VALUE_Y},
    [LCD_PH_INDEX] = {LCD_ITEM_VALUE_X, LCD_ITEM2_VALUE_Y},
};

const itemLocaltion_t itemWarnningLocations[LCD_MAX_ITEM_CNT] = 
{
    [LCD_DO_INDEX] = {LCD_ITEM_WARN_X, LCD_ITEM0_WARN_Y},
    [LCD_TMP_INDEX] = {LCD_ITEM_WARN_X, LCD_ITEM1_WARN_Y},
    [LCD_PH_INDEX] = {LCD_ITEM_WARN_X, LCD_ITEM2_WARN_Y},
};

int BspLcdInit(void)
{
    LcdDriverInit();
    BSP_LCD_CLR_SCREEN();
    return APP_OK;
}


int BspLcdShowHomePage(void)
{
    uint8_t string[32] = {0};
    LcdShowString(LCD_PN_START_X, LCD_PN_START_Y, "PN :", BLACK, LCD_BACKGROUND_COLOR, LCD_PN_FONT_SIZE, LCD_SHOW_MODE);
    LcdShowString(LCD_PN_VALUE_START_X, LCD_PN_START_Y, PN_CODE, BLACK, LCD_BACKGROUND_COLOR, LCD_PN_FONT_SIZE, LCD_SHOW_MODE);

    LcdShowString(LCD_SN_START_X, LCD_SN_START_Y, "SN :", BLACK, LCD_BACKGROUND_COLOR, LCD_SN_FONT_SIZE, LCD_SHOW_MODE);
    snprintf(string, 8, "%d", APP_CONFIG_DeviceID());
    LcdShowString(LCD_SN_VALUE_START_X, LCD_SN_START_Y, string, BLACK, LCD_BACKGROUND_COLOR, LCD_SN_FONT_SIZE, LCD_SHOW_MODE);

    LcdShowString(LCD_HW_START_X, LCD_HW_START_Y, "HW :", BLACK, LCD_BACKGROUND_COLOR, LCD_HW_FONT_SIZE, LCD_SHOW_MODE);
    memset(string, 0, sizeof(string));
    snprintf(string, 10, "V%d.%d.%d.%d", APP_HARDWARE_MAJOR, APP_HARDWARE_MINOR, APP_HARDWARE_REVISION, APP_HARDWARE_BUILD);
    LcdShowString(LCD_HW_VALUE_START_X, LCD_HW_START_Y, string, BLACK, LCD_BACKGROUND_COLOR, LCD_HW_FONT_SIZE, LCD_SHOW_MODE);
    
    LcdShowString(LCD_SW_START_X, LCD_SW_START_Y, "SW :", BLACK, LCD_BACKGROUND_COLOR, LCD_SW_FONT_SIZE, LCD_SHOW_MODE);
    memset(string, 0, sizeof(string));
    snprintf(string, 10, "V%d.%d.%d.%d", APP_FIRMWARE_MAJOR, APP_FIRMWARE_MINOR, APP_FIRMWARE_REVISION, APP_FIRMWARE_BUILD);
    LcdShowString(LCD_SW_VALUE_START_X, LCD_SW_START_Y, string, BLACK, LCD_BACKGROUND_COLOR, LCD_SW_FONT_SIZE, LCD_SHOW_MODE);
}

int BspLcdShowTopic(void)
{
    uint8_t string[32] = {0};
    strcpy(string, CompanyChinese);
    LcdShowChinese(LCD_ZYIOT_START_X, LCD_ZYIOT_START_Y, string, LIGHTBLUE, LCD_BACKGROUND_COLOR, LCD_ZYIOT_FONT_SIZE, LCD_SHOW_MODE);
    LcdShowString(LCD_ZYIOT_EN_START_X, LCD_ZYIOT_EN_START_Y, "ZHONGYIIOT", LIGHTBLUE, LCD_BACKGROUND_COLOR, LCD_ZYIOT_EN_FONT_SIZE, LCD_SHOW_MODE);
    LcdShowPicture(LCD_LOGO_START_X, LCD_LOGO_START_Y, LCD_LOGO1_WIDTH, LCD_LOGO1_HEIGH, gZYIOT_logo1);
}


int BspLcdShowItem(uint8_t itemIndex, uint8_t popIndex)
{
    uint8_t itemString[32] = {0};
    uint16_t x, y;

    x = itemLocations[popIndex].x;
    y = itemLocations[popIndex].y;
    if(LCD_TMP_INDEX == itemIndex)
    {
        strcpy(itemString, TmpItemChinese);
        LcdShowChinese(x, y, itemString, bspItemColor, bspBackColor, bspFontSize, LCD_SHOW_MODE);
        x += strlen(itemString) * bspFontSize;
        memset(itemString, 0, sizeof(itemString));
        strcpy(itemString, ":");
        LcdShowString(LCD_ITEM_DOT_X, y, itemString, bspItemColor, bspBackColor, bspFontSize, LCD_SHOW_MODE);
    }
    else if(LCD_DO_INDEX == itemIndex)
    {
        strcpy(itemString, DoItemChinese);
        LcdShowChinese(x, y, itemString, bspItemColor, bspBackColor, bspFontSize, LCD_SHOW_MODE);
        x += strlen(itemString) * bspFontSize;
        memset(itemString, 0, sizeof(itemString));
        strcpy(itemString, ":");
        LcdShowString(LCD_ITEM_DOT_X, y, itemString, bspItemColor, bspBackColor, bspFontSize, LCD_SHOW_MODE);
    }
    else if(LCD_PH_INDEX == itemIndex)
    {
        strcpy(itemString, "pH  ");
        LcdShowString(x, y, itemString, bspItemColor, bspBackColor, bspFontSize, LCD_SHOW_MODE);
        x += strlen(itemString) * bspFontSize / 2;
        memset(itemString, 0, sizeof(itemString));
        strcpy(itemString, PhItemChinese);
        LcdShowChinese(x, y, itemString, bspItemColor, bspBackColor, bspFontSize, LCD_SHOW_MODE);
        x += strlen(itemString) * bspFontSize;
        memset(itemString, 0, sizeof(itemString));
        strcpy(itemString, ":");
        LcdShowString(LCD_ITEM_DOT_X, y, itemString, bspItemColor, bspBackColor, bspFontSize, LCD_SHOW_MODE);
    }
    return APP_OK;
}

int BspLcdClearItem(uint8_t itemIndex)
{
    uint8_t itemString[32] = "       ";
    uint16_t x, y;

    x = itemLocations[itemIndex].x;
    y = itemLocations[itemIndex].y;
    LcdShowString(x, y, itemString, bspItemColor, bspBackColor, bspFontSize, LCD_SHOW_MODE);
    return APP_OK;
}

int BspLcdShowValue(float value, uint8_t valueIndex, uint8_t popIndex, uint8_t overFlow)
{
    char valueString[20] = {0};
    uint16_t x, y;
    uint8_t stringLen = 0;

    x = itemVauleLocations[popIndex].x;
    y = itemVauleLocations[popIndex].y;
    if(LCD_TMP_INDEX == valueIndex)
    {
        snprintf(valueString, 19, "%4.1f0", value); // 温度值的精度为 0.1 
    }
    else
    {
        snprintf(valueString, 19, "%5.2f ", value);
    }
    stringLen = strlen(valueString);
    if(stringLen < 5)
    {
        valueString[stringLen] = ' ';
    }
    if(0 == overFlow)
    {
        LcdShowString(x, y, valueString, bspItemValueColor, bspBackColor, LCD_ITEM_FONT_SIZE, LCD_SHOW_MODE);
    }
    else
    {
        LcdShowString(x, y, valueString, LCD_ERROR_COLOR, bspBackColor, LCD_ITEM_FONT_SIZE, LCD_SHOW_MODE);
    }
    return APP_OK;
}



int BspLcdShowUnknowValue(uint8_t valueIndex, uint8_t popIndex)
{
    uint8_t valueString[32] = "--    ";
    uint16_t x, y;

    x = itemVauleLocations[popIndex].x;
    y = itemVauleLocations[popIndex].y;
    LcdShowString(x, y, valueString, bspItemValueColor, bspBackColor, LCD_ITEM_FONT_SIZE, LCD_SHOW_MODE);
    return APP_OK;
}

#if 0
int BspLcdShowWarnning(uint8_t itemIndex, uint8_t popIndex)
{
    uint16_t x, y;
    x = itemWarnningLocations[popIndex].x;
    y = itemWarnningLocations[popIndex].y;
    LcdShowPicture(x, y, LCD_WARNNING_LOGO_WIDTH, LCD_WARNNING_LOGO_HEITH, gWarnning_logo);
    return APP_OK;
}
#else
int BspLcdShowWarnning(uint8_t itemIndex, uint8_t popIndex)
{
    uint16_t x, y;
    uint8_t string[10] = {0};
    uint8_t lineCnt = 5;
    uint8_t stringLen = strlen(WarnningChinese);
    strcpy(string, &WarnningChinese[0]);
    while(lineCnt > stringLen)
    {
        string[stringLen] = ' ';
        stringLen += 1;
    }
    x = itemVauleLocations[popIndex].x;
    y = itemVauleLocations[popIndex].y;
    LcdShowChinese(x, y, string, LCD_ERROR_COLOR, LCD_BACKGROUND_COLOR, LCD_TITLE_FONT_SIZE, LCD_SHOW_MODE);
    return APP_OK;
}
#endif

int BspLcdClearValue(uint8_t valueIndex)
{
    uint8_t valueString[32] = "      ";
    uint16_t x, y;

    x = itemVauleLocations[valueIndex].x;
    y = itemVauleLocations[valueIndex].y;
    LcdShowString(x, y, valueString, bspItemValueColor, bspBackColor, LCD_ITEM_FONT_SIZE, LCD_SHOW_MODE);
    return APP_OK;
}

int BspLcdShowSignal(uint8_t range)
{
    const char *pic = NULL;
    if(LCD_SIGNAL_RANGE1 == range)
    {
        pic = gSignal_rang1;
    }
    else if(LCD_SIGNAL_RANGE2 == range)
    {
        pic = gSignal_rang2;
    }
    else if(LCD_SIGNAL_RANGE3 == range)
    {
        pic = gSignal_rang3;
    }
    else
    {
        pic = gSignal_rang0;
    }
    if(NULL != pic)
    {
        LcdShowPicture(LCD_SIGNAL_START_X, LCD_SIGNAL_START_Y, LCD_SIGNAL_WIDTH, LCD_SIGNAL_HEIGH, pic);
    }
    else
    {
        LcdFill(LCD_SIGNAL_START_X, LCD_SIGNAL_START_Y, LCD_SIGNAL_START_X + LCD_SIGNAL_WIDTH, LCD_SIGNAL_START_Y + LCD_SIGNAL_HEIGH, LCD_BACKGROUND_COLOR);
    }
    return APP_OK;
}

int BspLcdShowTitle(void)
{
    uint8_t string[32] = {0};
    uint8_t total = strlen(TitleChinese);
    uint8_t line1Cnt = 10;
    LcdClrScreenRegion(0, LCD_TITLE_LINE1_START_Y, LCD_WIDTH - 1, LCD_HEITH);
    memcpy(string, &TitleChinese[0], line1Cnt);
    LcdShowChinese(LCD_TITLE_LINE1_START_X, LCD_TITLE_LINE1_START_Y, string, DARKBLUE, LCD_BACKGROUND_COLOR, LCD_TITLE_FONT_SIZE, LCD_SHOW_MODE);
    memset(string, 0, sizeof(string));
    memcpy(string, &TitleChinese[line1Cnt], total - line1Cnt);
    LcdShowChinese(LCD_TITLE_LINE2_START_X, LCD_TITLE_LINE2_START_Y, string, DARKBLUE, LCD_BACKGROUND_COLOR, LCD_TITLE_FONT_SIZE, LCD_SHOW_MODE);
    return APP_OK;
}

