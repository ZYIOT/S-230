#ifndef __BSP_LOG_H
#define __BSP_LOG_H

#ifdef __cplusplus
extern "C"
{
#endif
#include "hardware_wrapper.h"

/* DO NOT USE log methods in ISR */
#define BSP_LOG_LEVEL_NONE 0
#define BSP_LOG_LEVEL_ERROR 1
#define BSP_LOG_LEVEL_WARN 2
#define BSP_LOG_LEVEL_INFO 3
#define BSP_LOG_LEVEL_DEBUG 4
#define BSP_LOG_LEVEL_TRACE 5

#define BSP_LOG_show(fmt, ...)
#define BSP_LOG_error(fmt, ...)
#define BSP_LOG_warn(fmt, ...)
#define BSP_LOG_info(fmt, ...)
#define BSP_LOG_debug(fmt, ...)

#define BSP_LOG_show_hex(buf, len)
#define BSP_LOG_error_hex(buf, len)
#define BSP_LOG_warn_hex(buf, len)
#define BSP_LOG_info_hex(buf, len)
#define BSP_LOG_debug_hex(buf, len)

#define BSP_LOG_trace(...)
#define BSP_LOG_trace_hex(...)
#ifndef BSP_LOG_DEFAULT_LEVEL
#define BSP_LOG_DEFAULT_LEVEL BSP_LOG_LEVEL_DEBUG
#endif

#define BSP_LOG_init()
#define BSP_LOG_level_show()

#ifdef __cplusplus
}
#endif

#endif
