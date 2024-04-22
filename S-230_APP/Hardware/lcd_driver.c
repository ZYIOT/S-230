
#include "spi.h"
#include "main.h"
#include "board_config.h"
#include "lcd_driver.h"
#include "lcd_font.h"
#include "hardware_wrapper.h"

#define LCD_SPI_RW_TIMEOUT  (0xFFFFF)
#define LCD_SPI_HANDLE      (&hspi2)
#define LCD_SPI_CS_Pin      (LCD_CS_Pin)
#define LCD_SPI_CS_Port     (LCD_CS_GPIO_Port)
#define LCD_SPI_DC_Pin      (LCD_DC_Pin)
#define LCD_SPI_DC_Port     (LCD_DC_GPIO_Port)
#define LCD_SPI_CS_ENABLE()     HARDWARE_GPIO_WRITE_PIN_L(LCD_SPI_CS_Port, LCD_SPI_CS_Pin)
#define LCD_SPI_CS_DISABLE()    HARDWARE_GPIO_WRITE_PIN_H(LCD_SPI_CS_Port, LCD_SPI_CS_Pin)
#define LCD_SPI_CMD_ENABLE()    HARDWARE_GPIO_WRITE_PIN_L(LCD_SPI_DC_Port, LCD_DC_Pin)
#define LCD_SPI_DAT_ENABLE()    HARDWARE_GPIO_WRITE_PIN_H(LCD_SPI_DC_Port, LCD_DC_Pin)
#define LCD_SPI_WRITE_BYTE(byte)    HAL_SPI_Transmit(LCD_SPI_HANDLE, byte, 1, LCD_SPI_RW_TIMEOUT)
#define LCD_SPI_WRITE_CMD(cmd, len) HAL_SPI_Transmit(LCD_SPI_HANDLE, cmd, len, LCD_SPI_RW_TIMEOUT)
#define LCD_SPI_WRITE_DAT(dat, len) HAL_SPI_Transmit(LCD_SPI_HANDLE, dat, len, LCD_SPI_RW_TIMEOUT)


static void lcdDriverGpioInit(void)
{
    LCD_BL_OFF();
    LCD_RST_ON();
}

static void lcdWriteCmd(uint8_t cmd)
{
    LCD_SPI_CMD_ENABLE();
    LCD_SPI_CS_ENABLE();
    LCD_SPI_WRITE_CMD(&cmd, 1);
    LCD_SPI_CS_DISABLE();
    LCD_SPI_DAT_ENABLE();
}

static void lcdWriteDat(uint8_t dat)
{
    LCD_SPI_DAT_ENABLE();
    LCD_SPI_CS_ENABLE();
    LCD_SPI_WRITE_DAT(&dat, 1);
    LCD_SPI_CS_DISABLE();
}

static void lcdWriteData16(uint16_t data)
{
    uint8_t bytes[2] = {0};
    bytes[0] = (data >> 8);
    bytes[1] = data;
    LCD_SPI_DAT_ENABLE();
    LCD_SPI_CS_ENABLE();
    LCD_SPI_WRITE_DAT(bytes, 2);
    LCD_SPI_CS_DISABLE();
}

static void lcdAddressSet(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    lcdWriteCmd(0x2A);      // 列地址设置 
    lcdWriteData16(x1);
    lcdWriteData16(x2);
    lcdWriteCmd(0x2B);      // 行地址设置 
    lcdWriteData16(y1);
    lcdWriteData16(y2);
    lcdWriteCmd(0x2C);      // 存储器写 
}

void LcdDriverInit(void)
{
    lcdDriverGpioInit();
    LCD_RST_ON();           // 复位 
    HARDWARE_OS_DELAY_MS(100);
    LCD_RST_OFF();
    HARDWARE_OS_DELAY_MS(100);
    LCD_BL_ON();            // 打开背光 
    HARDWARE_OS_DELAY_MS(100);

    lcdWriteCmd(0x11);
    HARDWARE_OS_DELAY_MS(120);
    lcdWriteCmd(0xF0);
    lcdWriteDat(0xC3);
    lcdWriteCmd(0xF0);
    lcdWriteDat(0x96);
    lcdWriteCmd(0x36);
#if (LCD_SHOW_DIR == LCD_SHOW_DIR_HORIZONTAL)
    lcdWriteDat(0X28);
#else
    lcdWriteDat(0X48);
#endif
    lcdWriteCmd(0x3A);
	lcdWriteDat(0x05);
    lcdWriteCmd(0xE6);
	lcdWriteDat(0x0F);
	lcdWriteDat(0xF2);
	lcdWriteDat(0x3F);
	lcdWriteDat(0x4F);
	lcdWriteDat(0x4F);
	lcdWriteDat(0x28);
	lcdWriteDat(0x0E);
	lcdWriteDat(0x00);
    lcdWriteCmd(0xC5);
	lcdWriteDat(0x2A);
    lcdWriteCmd(0xE0);
	lcdWriteDat(0xF0);
	lcdWriteDat(0x03);
	lcdWriteDat(0x0A);
	lcdWriteDat(0x11);
	lcdWriteDat(0x14);
	lcdWriteDat(0x1C);
	lcdWriteDat(0x3B);
	lcdWriteDat(0x55);
	lcdWriteDat(0x4A);
	lcdWriteDat(0x0A);
	lcdWriteDat(0x13);
	lcdWriteDat(0x14);
	lcdWriteDat(0x1C);
	lcdWriteDat(0x1F);
    lcdWriteCmd(0xE1);
	lcdWriteDat(0xF0);
	lcdWriteDat(0x03);
	lcdWriteDat(0x0A);
	lcdWriteDat(0x0C);
	lcdWriteDat(0x0C);
	lcdWriteDat(0x09);
	lcdWriteDat(0x36);
	lcdWriteDat(0x54);
	lcdWriteDat(0x49);
	lcdWriteDat(0x0F);
	lcdWriteDat(0x1B);
	lcdWriteDat(0x18);
	lcdWriteDat(0x1B);
	lcdWriteDat(0x1F);
    lcdWriteCmd(0xF0);
	lcdWriteDat(0x3C);
    lcdWriteCmd(0xF0);
	lcdWriteDat(0x69);
    lcdWriteCmd(0x29);
    HARDWARE_OS_DELAY_MS(100);
}



/******************************************************************************
      函数说明：在指定区域填充颜色
      入口数据：xsta,ysta   起始坐标
                xend,yend   终止坐标
								color       要填充的颜色
      返回值：  无
******************************************************************************/
void LcdFill(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint16_t color)
{
    uint16_t i, j;
    lcdAddressSet(sx, sy, ex, ey);
    uint8_t bytes[2] = {0};
    bytes[0] = (color >> 8);
    bytes[1] = color;
    LCD_SPI_DAT_ENABLE();
    LCD_SPI_CS_ENABLE();

    for(i=sy; i<ey; i++)
    {
        for(j=sx; j<ex; j++)
        {
            LCD_SPI_WRITE_DAT(bytes, 2);
        }
    }
    LCD_SPI_CS_DISABLE();
}

void LcdClrScreen(void)
{
    LcdFill(0, 0, LCD_WIDTH - 1, LCD_HEITH, WHITE);
}

void LcdClrScreenRegion(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey)
{
    LcdFill(sx, sy, ex, ey, WHITE );
}

/******************************************************************************
      函数说明：在指定位置画点
      入口数据：x,y 画点坐标
                color 点的颜色
      返回值：  无
******************************************************************************/
void LcdDrawPoint(uint16_t x, uint16_t y, uint16_t color)
{
    lcdAddressSet(x, y, x, y);
    lcdWriteData16(color);
}

/******************************************************************************
      函数说明：画线
      入口数据：x1,y1   起始坐标
                x2,y2   终止坐标
                color   线的颜色
      返回值：  无
******************************************************************************/
void LcdDrawLine(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint16_t color)
{
    uint8_t t = 0;
    int xErr = 0, yErr = 0, xDelta = 0, yDelta = 0, distance = 0;
    int xInc = 0, yInc = 0, uRow = 0, uCol = 0;

    xDelta = ex - sx;
    yDelta = ey - sy;
    uRow = sx;
    uCol = sy;

    if(xDelta > 0) {xInc = 1;}
    else if(xDelta == 0) {xInc = 0;}
    else {xInc = -1;xDelta = -xDelta;}

    if(yDelta > 0) {yInc = 1;}
    else if(yDelta == 0) {yInc = 0;}
    else {yInc = -1;yDelta = -yDelta;}

    if(xDelta > yDelta) {distance = xDelta;}
    else {distance = yDelta;}

    for(t=0; t<distance+1; t++)
    {
        LcdDrawPoint(uRow, uCol, color);
        xErr += xDelta;
        yErr += yDelta;
        if(xErr > distance) {xErr -= distance;uRow += xInc;}
        if(yErr > distance) {yErr -= distance;uCol += yInc;}
    }
}

/******************************************************************************
      函数说明：画矩形
      入口数据：x1,y1   起始坐标
                x2,y2   终止坐标
                color   矩形的颜色
      返回值：  无
******************************************************************************/
void LcdDrawRectangle(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint16_t color)
{
    LcdDrawLine(sx, sy, ex, sy, color);
    LcdDrawLine(sx, sy, sx, ey, color);
    LcdDrawLine(sx, ey, ex, ey, color);
    LcdDrawLine(ex, sy, ex, ey, color);
}

/******************************************************************************
      函数说明：画圆
      入口数据：x0,y0   圆心坐标
                r       半径
                color   圆的颜色
      返回值：  无
******************************************************************************/
void LcdDrawCircle(uint16_t x, uint16_t y, uint8_t r, uint16_t color)
{

}

/******************************************************************************
      函数说明：显示单个字符
      入口数据：x,y显示坐标
                num 要显示的字符
                fontColor 字的颜色
                bc 字的背景色
                sizey 字号
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
void LcdShowChar(uint16_t x, uint16_t y, uint8_t num, uint16_t fontColor, uint16_t backColor, uint8_t size, uint8_t mode)
{
    uint8_t temp, xSize, t, m = 0;
    uint16_t i, typefaceNum;
    uint16_t x0 = x;

    xSize = size / 2;
    typefaceNum = ((xSize >> 3) + ((xSize & 0x7) ? 1 : 0)) * size;
    num = num - ' ';
    lcdAddressSet(x, y, x + xSize - 1, y + size - 1);       // 设置光标位置 
    
    for(i=0; i< typefaceNum; i++)
    {
        if(FONT_6X12 == size) {temp = ascii_1206[num][i];}          // 调用 6X12 字体 
        else if(FONT_8X16 == size) {temp = ascii_1608[num][i];}     // 调用 8X16 字体 
        else if(FONT_12X24 == size) {temp = ascii_2412[num][i];}    // 调用 12X24 字体 
        else if(FONT_16X32 == size) {temp = ascii_3216[num][i];}    // 调用 16X32 字体 
        else if(FONT_32X64 == size) {temp = ascii_6432[num][i];}    // 调用 32X64 字体 
        else return;
        for(t=0; t<8; t++)
        {
            if(LCD_SHOW_MODE0 == mode)   // 非叠加模式 
            {
                if(temp & (0x1 << t)) {lcdWriteData16(fontColor);}
                else {lcdWriteData16(backColor);}
                m += 1;
                if(0 == m % xSize) {m = 0;break;}
            }
            else            // 叠加模式 
            {
                if(temp & (0x1 << t)) {LcdDrawPoint(x, y, fontColor);}
                x += 1;
                if((x - x0) == xSize) {x = x0;y += 1;break;}
            }
        }
    }
}

/******************************************************************************
      函数说明：显示字符串
      入口数据：x,y显示坐标
                *p 要显示的字符串
                fontColor 字的颜色
                bc 字的背景色
                sizey 字号
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
void LcdShowString(uint16_t x, uint16_t y, uint8_t *str, uint16_t fontColor, uint16_t backColor, uint8_t size, uint8_t mode)
{
    while(*str != '\0')
    {
        LcdShowChar(x, y, *str, fontColor, backColor, size, mode);
        x += size / 2;
        str += 1;
    }
}

// 计算 m^n (m的n次幂)的值 
static uint32_t myPow(uint8_t m, uint8_t n)
{
    uint32_t result = 1;
    while (n--)
    {
        result *= m;
    }
    return result;
}

/*
 获取浮点数的有效数据长度 
 data : 浮点数数值 
 len  : 小数位数 
 floatLen : 浮点数的位数 
*/
static uint8_t getFloatLen(float data, uint8_t len)
{
    uint8_t floatLen = 0;
    uint32_t intData = 0;

    data = (int)data;
    intData = (data > 0) ? data : (-data);
    len = len > 6 ? 6 : len;
    floatLen += len;
    while(intData > 0)
    {
        floatLen += 1;
        intData /= 10;
    }
    return floatLen;
}


/******************************************************************************
      函数说明：显示整数变量
      入口数据：x,y显示坐标
                num 要显示整数变量
                len 要显示的位数
                fontColor 字的颜色
                bc 字的背景色
                sizey 字号
      返回值：  无
******************************************************************************/
void LcdShowIntNum(uint16_t x, uint16_t y, uint16_t num, uint8_t len, uint16_t fontColor, uint16_t backColor, uint8_t size)
{
    uint8_t t, temp;
    uint8_t enShow = 0;
    uint8_t xSize = size / 2;

    for(t=0; t<len; t++)
    {
        temp = (num / myPow(10, len - t - 1)) % 10;
        if((0 == enShow) && (t < len - 1))
        {
            if(0 == temp) {LcdShowChar(x + t *xSize, y, ' ', fontColor, backColor, size, 0);continue;}
            else {enShow = 1;}
        }
        LcdShowChar(x + t * xSize, y, temp + 48, fontColor, backColor, size, 0);
    }
}

/******************************************************************************
      函数说明：显示浮点数变量
      入口数据：x,y显示坐标
                num 要显示浮点数变量
                len 要显示的小数位数
                fontColor 字的颜色
                bc 字的背景色
                sizey 字号
      返回值：  无
******************************************************************************/
void LcdShowFloatNum(uint16_t x, uint16_t y, float num, uint8_t len, uint16_t fontColor, uint16_t backColor, uint8_t size)
{
    uint8_t t, xSize;
    uint16_t num1;
    uint32_t temp = 0;
    uint8_t floatLen = 0;
    int intData = 0;
    float dotData = 0.0;

    floatLen = getFloatLen(num, len);
    len = len > 6 ? 6 : len;
    xSize = size / 2;

    if(num < 0)
    {
        LcdShowChar(x, y, '-', fontColor, backColor, size, LCD_SHOW_MODE0);
        x += xSize;
        num = -num;
    }
    intData = (int)num;
    dotData = num - intData;
    for(t=floatLen-len; t>0; t--)
    {
        temp = myPow(10, t - 1);
        num1 = (intData / temp);
        LcdShowChar(x, y, num1 + '0', fontColor, backColor, size, LCD_SHOW_MODE0);
        x += xSize;
        intData -= num1 * temp;
    }
    LcdShowChar(x, y, '.', fontColor, backColor, size, LCD_SHOW_MODE0);
    x += xSize;
    for(t=0; t<len; t++)
    {
        dotData *= 10;
        num1 = (int)(dotData);
        LcdShowChar(x, y, num1 + '0', fontColor, backColor, size, LCD_SHOW_MODE0);
        x += xSize;
        dotData -= num1;
    }
}

/******************************************************************************
      函数说明：显示图片
      入口数据：x,y起点坐标
                length 图片长度
                width  图片宽度
                pic[]  图片数组    
      返回值：  无
******************************************************************************/
void LcdShowPicture(uint16_t x, uint16_t y, uint16_t len, uint16_t width, const uint8_t *pic)
{
    uint16_t i, j;
    uint32_t k = 0;
    uint8_t picL, picH;
    lcdAddressSet(x, y, x + len - 1, y + width - 1);
    for(i=0; i<len; i++)
    {
        for(j=0; j<width; j++)
        {
            picL = pic[k * 2];
            picH = pic[k * 2 + 1];
            lcdWriteData16((picH << 8) | picL);
            k += 1;
        }
    }
}


showChineseStringFunc lcdShowChineseStringFunc = NULL;


/******************************************************************************
      函数说明：显示单个12x12汉字
      入口数据：x,y显示坐标
                *s 要显示的汉字
                fontColor 字的颜色
                bc 字的背景色
                sizey 字号
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
void LcdShowChinese12X12(uint16_t x, uint16_t y, uint8_t *str, uint16_t fontColor, uint16_t backColor, uint8_t size, uint8_t mode)
{
    uint16_t i, j, m = 0;
    uint16_t k;
    uint16_t hzNum;
    uint16_t typefaceNum;
    uint16_t x0 = x;

    typefaceNum = ((size >> 3) + ((size & 0x7) ? 1 : 0)) * size;
    hzNum = sizeof(tfont12) / sizeof(typFNT_GB12);      // 统计汉字数目 

    for(k=0; k<hzNum; k++)
    {
        if((tfont12[k].Index[0] == *(str)) && (tfont12[k].Index[1] == *(str + 1)))
        {
            lcdAddressSet(x, y, x + size - 1, y + size - 1);
            for(i=0; i<typefaceNum; i++)
            {
                for(j=0; j<8; j++)
                {
                    if(LCD_SHOW_MODE0 == mode)   //非叠加模式 
                    {
                        if(tfont12[k].Msk[i] & (0x1 << j)) {lcdWriteData16(fontColor);}
                        else {lcdWriteData16(backColor);}
                        m += 1;
                        if(0 == m % size) {m = 0;break;}
                    }
                    else            // 叠加模式 
                    {
                        if(tfont12[k].Msk[i] & (0x1 << j)) {LcdDrawPoint(x, y, fontColor);}
                        x += 1;
                        if(x - x0 == size) {x = x0;y +=1;break;}
                    }
                }
            }
        }
        continue;
    }
}

/******************************************************************************
      函数说明：显示单个16x16汉字
      入口数据：x,y显示坐标
                *s 要显示的汉字
                fontColor 字的颜色
                bc 字的背景色
                sizey 字号
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
void LcdShowChinese16X16(uint16_t x, uint16_t y, uint8_t *str, uint16_t fontColor, uint16_t backColor, uint8_t size, uint8_t mode)
{
    uint16_t i, j, m = 0;
    uint16_t k;
    uint16_t hzNum;
    uint16_t typefaceNum;
    uint16_t x0 = x;

    typefaceNum = ((size >> 3) + ((size & 0x7) ? 1 : 0)) * size;
    hzNum = sizeof(tfont16) / sizeof(typFNT_GB16);      // 统计汉字数目 

    for(k=0; k<hzNum; k++)
    {
        if((tfont16[k].Index[0] == *(str)) && (tfont16[k].Index[1] == *(str + 1)))
        {
            lcdAddressSet(x, y, x + size - 1, y + size - 1);
            for(i=0; i<typefaceNum; i++)
            {
                for(j=0; j<8; j++)
                {
                    if(LCD_SHOW_MODE0 == mode)   //非叠加模式 
                    {
                        if(tfont16[k].Msk[i] & (0x1 << j)) {lcdWriteData16(fontColor);}
                        else {lcdWriteData16(backColor);}
                        m += 1;
                        if(0 == m % size) {m = 0;break;}
                    }
                    else            // 叠加模式 
                    {
                        if(tfont16[k].Msk[i] & (0x1 << j)) {LcdDrawPoint(x, y, fontColor);}
                        x += 1;
                        if(x - x0 == size) {x = x0;y +=1;break;}
                    }
                }
            }
        }
        continue;
    }
}

/******************************************************************************
      函数说明：显示单个24x24汉字
      入口数据：x,y显示坐标
                *s 要显示的汉字
                fontColor 字的颜色
                bc 字的背景色
                sizey 字号
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
void LcdShowChinese24X24(uint16_t x, uint16_t y, uint8_t *str, uint16_t fontColor, uint16_t backColor, uint8_t size, uint8_t mode)
{
    uint16_t i, j, m = 0;
    uint16_t k;
    uint16_t hzNum;
    uint16_t typefaceNum;
    uint16_t x0 = x;

    typefaceNum = ((size >> 3) + ((size & 0x7) ? 1 : 0)) * size;
    hzNum = sizeof(tfont24) / sizeof(typFNT_GB24);      // 统计汉字数目 

    for(k=0; k<hzNum; k++)
    {
        if((tfont24[k].Index[0] == *(str)) && (tfont24[k].Index[1] == *(str + 1)))
        {
            lcdAddressSet(x, y, x + size - 1, y + size - 1);
            for(i=0; i<typefaceNum; i++)
            {
                for(j=0; j<8; j++)
                {
                    if(LCD_SHOW_MODE0 == mode)   //非叠加模式 
                    {
                        if(tfont24[k].Msk[i] & (0x1 << j)) {lcdWriteData16(fontColor);}
                        else {lcdWriteData16(backColor);}
                        m += 1;
                        if(0 == m % size) {m = 0;break;}
                    }
                    else            // 叠加模式 
                    {
                        if(tfont24[k].Msk[i] & (0x1 << j)) {LcdDrawPoint(x, y, fontColor);}
                        x += 1;
                        if(x - x0 == size) {x = x0;y +=1;break;}
                    }
                }
            }
        }
        continue;
    }
}

/******************************************************************************
      函数说明：显示单个32x32汉字
      入口数据：x,y显示坐标
                *s 要显示的汉字
                fontColor 字的颜色
                bc 字的背景色
                sizey 字号
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
void LcdShowChinese32X32(uint16_t x, uint16_t y, uint8_t *str, uint16_t fontColor, uint16_t backColor, uint8_t size, uint8_t mode)
{
    uint16_t i, j, m = 0;
    uint16_t k;
    uint16_t hzNum;
    uint16_t typefaceNum;
    uint16_t x0 = x;

    typefaceNum = ((size >> 3) + ((size & 0x7) ? 1 : 0)) * size;
    hzNum = sizeof(tfont32) / sizeof(typFNT_GB32);      // 统计汉字数目 

    for(k=0; k<hzNum; k++)
    {
        if((tfont32[k].Index[0] == *(str)) && (tfont32[k].Index[1] == *(str + 1)))
        {
            lcdAddressSet(x, y, x + size - 1, y + size - 1);
            for(i=0; i<typefaceNum; i++)
            {
                for(j=0; j<8; j++)
                {
                    if(LCD_SHOW_MODE0 == mode)   //非叠加模式 
                    {
                        if(tfont32[k].Msk[i] & (0x1 << j)) {lcdWriteData16(fontColor);}
                        else {lcdWriteData16(backColor);}
                        m += 1;
                        if(0 == m % size) {m = 0;break;}
                    }
                    else            // 叠加模式 
                    {
                        if(tfont32[k].Msk[i] & (0x1 << j)) {LcdDrawPoint(x, y, fontColor);}
                        x += 1;
                        if(x - x0 == size) {x = x0;y +=1;break;}
                    }
                }
            }
        }
        continue;
    }
}

/******************************************************************************
      函数说明：显示单个64x64汉字
      入口数据：x,y显示坐标
                *s 要显示的汉字
                fontColor 字的颜色
                bc 字的背景色
                sizey 字号
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
void LcdShowChinese64X64(uint16_t x, uint16_t y, uint8_t *str, uint16_t fontColor, uint16_t backColor, uint8_t size, uint8_t mode)
{
    uint16_t i, j, m = 0;
    uint16_t k;
    uint16_t hzNum;
    uint16_t typefaceNum;
    uint16_t x0 = x;

    typefaceNum = ((size >> 3) + ((size & 0x7) ? 1 : 0)) * size;
    hzNum = sizeof(tfont64) / sizeof(typFNT_GB64);      // 统计汉字数目 

    for(k=0; k<hzNum; k++)
    {
        if((tfont64[k].Index[0] == *(str)) && (tfont64[k].Index[1] == *(str + 1)))
        {
            lcdAddressSet(x, y, x + size - 1, y + size - 1);
            for(i=0; i<typefaceNum; i++)
            {
                for(j=0; j<8; j++)
                {
                    if(LCD_SHOW_MODE0 == mode)   //非叠加模式 
                    {
                        if(tfont64[k].Msk[i] & (0x1 << j)) {lcdWriteData16(fontColor);}
                        else {lcdWriteData16(backColor);}
                        m += 1;
                        if(0 == m % size) {m = 0;break;}
                    }
                    else            // 叠加模式 
                    {
                        if(tfont64[k].Msk[i] & (0x1 << j)) {LcdDrawPoint(x, y, fontColor);}
                        x += 1;
                        if(x - x0 == size) {x = x0;y +=1;break;}
                    }
                }
            }
        }
        continue;
    }
}


/******************************************************************************
      函数说明：显示汉字串
      入口数据：x,y显示坐标
                *s 要显示的汉字串
                fontColor 字的颜色
                bc 字的背景色
                sizey 字号 可选 16 24 32
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
void LcdShowChinese(uint16_t x, uint16_t y, uint8_t *str, uint16_t fontColor, uint16_t backColor, uint8_t size, uint8_t mode)
{
    if(FONT_6X12 == size)
    {
        lcdShowChineseStringFunc = &LcdShowChinese12X12;
    }
    else if(FONT_8X16 == size)
    {
        lcdShowChineseStringFunc = &LcdShowChinese16X16;
    }
    else if(FONT_12X24 == size)
    {
        lcdShowChineseStringFunc = &LcdShowChinese24X24;
    }
    else if(FONT_16X32 == size)
    {
        lcdShowChineseStringFunc = &LcdShowChinese32X32;
    }
    else if(FONT_32X64 == size)
    {
        lcdShowChineseStringFunc = &LcdShowChinese64X64;
    }
    else 
    {
        return;
    }
    while(*str)
    {
        lcdShowChineseStringFunc(x, y, str, fontColor, backColor, size, mode);
        str += 2;
        x += size;
    }
}