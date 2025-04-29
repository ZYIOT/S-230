#ifndef __APP_KEY_H
#define __APP_KEY_H

#ifdef __cplusplus
extern "C"
{
#endif
#include "app.h"
#include "bsp_key.h"

#define APP_KEY_UP 100 //没有向上键
#define APP_KEY_DOWN 4
#define APP_KEY_BACK 3
#define APP_KEY_OK 2
#define APP_KEY_MENU 1

#define APP_KEY_Init() BSP_KEY_init()
#define APP_KEY_CheckKeys() BSP_KEY_check_keys()
#define APP_KEY_Press(key_mask, ms) BSP_KEY_press(key_mask, ms)

int APP_KEY_Get(void);
#ifdef __cplusplus
}
#endif

#endif
