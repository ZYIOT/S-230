#include "lcd12864.h"

#ifdef LCD12864_ENABLE
#define __CLK_H HARDWARE_GPIO_WRITE_PIN(LCDx->clk_gpio, LCDx->clk_pin, GPIO_PIN_SET);
#define __CLK_L HARDWARE_GPIO_WRITE_PIN(LCDx->clk_gpio, LCDx->clk_pin, GPIO_PIN_RESET);
#define __SDI_H HARDWARE_GPIO_WRITE_PIN(LCDx->sdi_gpio, LCDx->sdi_pin, GPIO_PIN_SET);
#define __SDI_L HARDWARE_GPIO_WRITE_PIN(LCDx->sdi_gpio, LCDx->sdi_pin, GPIO_PIN_RESET);
#define __RST_H HARDWARE_GPIO_WRITE_PIN(LCDx->rst_gpio, LCDx->rst_pin, GPIO_PIN_SET);
#define __RST_L HARDWARE_GPIO_WRITE_PIN(LCDx->rst_gpio, LCDx->rst_pin, GPIO_PIN_RESET);
#define __CS_H HARDWARE_GPIO_WRITE_PIN(LCDx->cs_gpio, LCDx->cs_pin, GPIO_PIN_SET);
#define __CS_L HARDWARE_GPIO_WRITE_PIN(LCDx->cs_gpio, LCDx->cs_pin, GPIO_PIN_RESET);

#define __WRITE_COMMAND(cmd) _write_command(LCDx, cmd)
#define __WRITE_DATA(byte) _write_data(LCDx, byte)
#define __SEND_BYTE(byte) __send_byte(LCDx, byte)

static uint8_t POSTIONS[] = {
    0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
    0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97,
    0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f,
    0x98, 0x99, 0x9a, 0x9b, 0x9c, 0x9d, 0x9e, 0x9f};

static void __send_byte(LCD12864_TypeDef *LCDx, uint8_t byte)
{
    uint8_t i;
    for (i = 0; i < 8; i++)
    {
        __CLK_L
        if (byte & 0x80)
        {
            __SDI_H
        }
        else
        {
            __SDI_L
        }
        byte = byte << 1;
        HARDWARE_HAL_DELAY_US(3);
        __CLK_H
        HARDWARE_HAL_DELAY_US(3);
    }
    __CLK_L
    HARDWARE_HAL_DELAY_US(1);
}

static void _write_command(LCD12864_TypeDef *LCDx, uint8_t cmd)
{
    __CS_H
    HARDWARE_HAL_DELAY_US(10);
    __SEND_BYTE(0xf8);
    __SEND_BYTE(0xf0 & cmd);
    __SEND_BYTE(0xf0 & (cmd << 4));
    __CS_L
    HARDWARE_HAL_DELAY_US(10);
}

//static int _busy(LCD12864_TypeDef *LCDx)
//{
//    return 0;
//}

static void _write_data(LCD12864_TypeDef *LCDx, uint8_t byte)
{
    __CS_H
    HARDWARE_HAL_DELAY_US(10);
    __SEND_BYTE(0xfa);
    __SEND_BYTE(0xf0 & byte);
    __SEND_BYTE(0xf0 & (byte << 4));
    __CS_L
    HARDWARE_HAL_DELAY_US(10);
}

static void _init(LCD12864_TypeDef *LCDx)
{
    __CS_L
    __CS_L
    __SDI_L
    __RST_L
}

void LCD12864_reset(LCD12864_TypeDef *LCDx)
{
    __RST_L
    HARDWARE_HAL_DELAY_MS(50);
    __RST_H
    HARDWARE_HAL_DELAY_MS(10);
}

void LCD12864_init(LCD12864_TypeDef *LCDx)
{
    _init(LCDx);
    LCDx->sem = HARDWARE_CREATE_SEMAPHORE();
    LCD12864_reset(LCDx);
    __WRITE_COMMAND(0x30);
    HARDWARE_HAL_DELAY_US(110);
    __WRITE_COMMAND(0x30);
    HARDWARE_HAL_DELAY_US(50);
    __WRITE_COMMAND(0x0C);
    HARDWARE_HAL_DELAY_US(110);
    LCD12864_clear_txt(LCDx);
    __WRITE_COMMAND(0x06);
}

void LCD12864_clear_txt(LCD12864_TypeDef *LCDx)
{
    __WRITE_COMMAND(0x01);
    HARDWARE_HAL_DELAY_MS(10);
}

void LCD12864_reverse_row(LCD12864_TypeDef *LCDx, uint8_t row)
{
    row = row & 0x03;
    __WRITE_COMMAND(0x34);
    __WRITE_COMMAND(0x14 | (row));
}

static void _show_string(LCD12864_TypeDef *LCDx, uint8_t row, uint8_t col, char *bytes, uint8_t fill)
{
    HARDWARE_TAKE_SEMAPHORE(LCDx->sem);
    row = row & 0x03;
    col %= 8;
    uint8_t e = 0;
    __WRITE_COMMAND(0x30);
    __WRITE_COMMAND(POSTIONS[row * 8 + col]);

    for (int i = 0; i < 16 - col * 2; i++)
    {
        if (bytes[i] == 0)
        {
            e = 1;
        }
        if (e)
        {
            if (fill)
            {
                __WRITE_DATA(0x20);
            }
            else
            {
                HARDWARE_GIVE_SEMAPHORE(LCDx->sem);
                return;
            }
        }
        else
        {
            __WRITE_DATA(bytes[i]);
        }
    }
    HARDWARE_GIVE_SEMAPHORE(LCDx->sem);
}

void LCD12864_show_row(LCD12864_TypeDef *LCDx, uint8_t row, uint8_t col, char *bytes)
{
    _show_string(LCDx, row, col, bytes, 1);
}

void LCD12864_show_string(LCD12864_TypeDef *LCDx, uint8_t row, uint8_t col, char *bytes)
{
    _show_string(LCDx, row, col, bytes, 0);
}
#else
void LCD12864_init(LCD12864_TypeDef *LCDx)
{;}
void LCD12864_reset(LCD12864_TypeDef *LCDx)
{;}
void LCD12864_clear_txt(LCD12864_TypeDef *LCDx)
{;}
void LCD12864_reverse_row(LCD12864_TypeDef *LCDx, uint8_t row)
{;}
void LCD12864_show_row(LCD12864_TypeDef *LCDx, uint8_t row, uint8_t col, char *bytes)
{;}
void LCD12864_show_string(LCD12864_TypeDef *LCDx, uint8_t row, uint8_t col, char *bytes)
{;}
#endif
