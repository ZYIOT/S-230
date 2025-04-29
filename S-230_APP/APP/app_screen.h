#ifndef __APP_SCREEN_H
#define __APP_SCREEN_H

#ifdef __cplusplus
extern "C"
{
#endif
#include "stdint.h"

#ifdef LCD12864_ENABLE
#define LCD_MAX_ROW 3

    int APP_SCREEN_init(void);
    int APP_SCREEN_show(void);
    int APP_SCREEN_on_key_press(uint8_t key);
    int APP_SCREEN_on_timer_up(void);
    void APP_SCREEN_task_run(void *argument);
    void APP_SCREEN_to_main_page(void);

#endif
#ifdef __cplusplus
}
#endif

#endif
