#include <stdio.h>
#include <stdarg.h>
#include "usart.h"

#include "bsp_log.h"
#include "hardware_wrapper.h"

#define LOG_UART_HANDLE (&huart2)
#define LOG_RECV_BUFFER (NULL)

#pragma import(__use_no_semihosting)  
struct __FILE
{
	int handle;
};

FILE __stdout;

void _sys_exit(int x) 
{ 
    x = x; 
} 

//__use_no_semihosting was requested, but _ttywrch was
void _ttywrch(int ch)
{
    ch = ch;
}

int fputc(int ch, FILE *f)
{
    HAL_UART_Transmit(LOG_UART_HANDLE, (uint8_t *)&ch, 1, 0xffff);
    return ch;
}

int fgetc(FILE *f)
{
    uint8_t ch = 0;
    HAL_UART_Receive(LOG_UART_HANDLE, &ch, 1, HAL_MAX_DELAY);
    return ch;
}

int BSP_LOG_init(void)
{
    return 0;
}

void BSP_LOG_print(uint8_t level, const char *fmt, ...)
{
    if (level > BSP_LOG_DEFAULT_LEVEL)
    {
        return;
    }
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
}

void BSP_LOG_print_hex(uint8_t level, uint8_t *buf, size_t len)
{
    if (level > BSP_LOG_DEFAULT_LEVEL)
    {
        return;
    }
    for (int i = 0; i < len; i++)
    {
        printf("%02x ", buf[i]);
    }
}

static char *log_levels[] = {
    "NONE",
    "ERROR",
    "WARN",
    "INFO",
    "DEBUG",
    "TRACE"};

void BSP_LOG_level_show(void)
{
    uint8_t l = BSP_LOG_DEFAULT_LEVEL;
    BSP_LOG_show("Log level: %s\r\n", log_levels[l]);
}
