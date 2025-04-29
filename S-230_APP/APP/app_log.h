#ifndef __APP_LOG_H
#define __APP_LOG_H

#ifdef __cplusplus
extern "C"
{
#endif
#include "app.h"
#include "bsp_log.h"

/* DO NOT USE log methods in ISR */

#define APP_LOG_Show(...) BSP_LOG_show(##__VA_ARGS__)
#define APP_LOG_Error(...) BSP_LOG_error(##__VA_ARGS__)
#define APP_LOG_Warn(...) BSP_LOG_warn(##__VA_ARGS__)
#define APP_LOG_Info(...) BSP_LOG_info(##__VA_ARGS__)
#define APP_LOG_Debug(...) BSP_LOG_debug(##__VA_ARGS__)
#define APP_LOG_Trace(...) BSP_LOG_trace(##__VA_ARGS__)

#define APP_LOG_ShowHex(...) BSP_LOG_show_hex(##__VA_ARGS__)
#define APP_LOG_ErrorHex(...) BSP_LOG_error_hex(##__VA_ARGS__)
#define APP_LOG_WarnHex(...) BSP_LOG_warn_hex(##__VA_ARGS__)
#define APP_LOG_InfoHex(...) BSP_LOG_info_hex(##__VA_ARGS__)
#define APP_LOG_DebugHex(...) BSP_LOG_debug_hex(##__VA_ARGS__)
#define APP_LOG_TraceHex(...) BSP_LOG_trace_hex(##__VA_ARGS__)

#define APP_LOG_Init() BSP_LOG_init()
#define APP_LOG_LevelShow() BSP_LOG_level_show()


#ifdef __cplusplus
}
#endif

#endif
