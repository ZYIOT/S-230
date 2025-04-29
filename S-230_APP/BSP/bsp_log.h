#ifndef __BSP_LOG_H
#define __BSP_LOG_H

#ifdef __cplusplus
extern "C"
{
#endif
#include "app.h"

/* DO NOT USE log methods in ISR */
#define BSP_LOG_LEVEL_NONE 0
#define BSP_LOG_LEVEL_ERROR 1
#define BSP_LOG_LEVEL_WARN 2
#define BSP_LOG_LEVEL_INFO 3
#define BSP_LOG_LEVEL_DEBUG 4
#define BSP_LOG_LEVEL_TRACE 5

#define BSP_LOG_show(fmt, ...) BSP_LOG_print(BSP_LOG_LEVEL_NONE, fmt, ##__VA_ARGS__)
#define BSP_LOG_error(fmt, ...) BSP_LOG_print(BSP_LOG_LEVEL_ERROR, fmt, ##__VA_ARGS__)
#define BSP_LOG_warn(fmt, ...) BSP_LOG_print(BSP_LOG_LEVEL_WARN, fmt, ##__VA_ARGS__)
#define BSP_LOG_info(fmt, ...) BSP_LOG_print(BSP_LOG_LEVEL_INFO, fmt, ##__VA_ARGS__)
#define BSP_LOG_debug(fmt, ...) BSP_LOG_print(BSP_LOG_LEVEL_DEBUG, fmt, ##__VA_ARGS__)

#define BSP_LOG_show_hex(buf, len) BSP_LOG_print_hex(BSP_LOG_LEVEL_NONE, buf, len)
#define BSP_LOG_error_hex(buf, len) BSP_LOG_print_hex(BSP_LOG_LEVEL_ERROR, buf, len)
#define BSP_LOG_warn_hex(buf, len) BSP_LOG_print_hex(BSP_LOG_LEVEL_WARN, buf, len)
#define BSP_LOG_info_hex(buf, len) BSP_LOG_print_hex(BSP_LOG_LEVEL_INFO, buf, len)
#define BSP_LOG_debug_hex(buf, len) BSP_LOG_print_hex(BSP_LOG_LEVEL_DEBUG, buf, len)

#ifdef ENABLE_LOG_TRACE
#define BSP_LOG_trace(fmt, ...) BSP_LOG_print(BSP_LOG_LEVEL_TRACE, fmt, ##__VA_ARGS__)
#define BSP_LOG_trace_hex(buf, len) BSP_LOG_print_hex(BSP_LOG_LEVEL_TRACE, buf, len)
#ifndef BSP_LOG_DEFAULT_LEVEL
#define BSP_LOG_DEFAULT_LEVEL BSP_LOG_LEVEL_TRACE
#endif
#else
#define BSP_LOG_trace(...)
#define BSP_LOG_trace_hex(...)
#ifndef BSP_LOG_DEFAULT_LEVEL
#define BSP_LOG_DEFAULT_LEVEL BSP_LOG_LEVEL_DEBUG
#endif
#endif

    int BSP_LOG_init(void);
    void BSP_LOG_print(uint8_t level, const char *fmt, ...);
    void BSP_LOG_print_hex(uint8_t level, uint8_t *buf, size_t len);
    void BSP_LOG_level_show(void);

#ifdef __cplusplus
}
#endif

#endif
