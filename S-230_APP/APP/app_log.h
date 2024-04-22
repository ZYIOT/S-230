#ifndef __APP_LOG_H
#define __APP_LOG_H

#ifdef __cplusplus
extern "C"
{
#endif
#include "app.h"
#include "bsp_log.h"

/* DO NOT USE log methods in ISR */

#define APP_LOG_show(...) BSP_LOG_show(##__VA_ARGS__)
#define APP_LOG_error(...) BSP_LOG_error(##__VA_ARGS__)
#define APP_LOG_warn(...) BSP_LOG_warn(##__VA_ARGS__)
#define APP_LOG_info(...) BSP_LOG_info(##__VA_ARGS__)
#define APP_LOG_debug(...) BSP_LOG_debug(##__VA_ARGS__)
#define APP_LOG_trace(...) BSP_LOG_trace(##__VA_ARGS__)

#define APP_LOG_show_hex(...) BSP_LOG_show_hex(##__VA_ARGS__)
#define APP_LOG_error_hex(...) BSP_LOG_error_hex(##__VA_ARGS__)
#define APP_LOG_warn_hex(...) BSP_LOG_warn_hex(##__VA_ARGS__)
#define APP_LOG_info_hex(...) BSP_LOG_info_hex(##__VA_ARGS__)
#define APP_LOG_debug_hex(...) BSP_LOG_debug_hex(##__VA_ARGS__)
#define APP_LOG_trace_hex(...) BSP_LOG_trace_hex(##__VA_ARGS__)

#define APP_LOG_init() BSP_LOG_init()
#define APP_LOG_level_show() BSP_LOG_level_show()


#ifdef __cplusplus
}
#endif

#endif
