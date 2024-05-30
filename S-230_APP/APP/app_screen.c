
#include "app.h"
#ifdef LCD12864_ENABLE
#include "app_key.h"
#include "app_config.h"
#include "bsp_lcd12864.h"
#include "bsp_log.h"
#include "hardware_wrapper.h"

static HARDWARE_SEMAPHORE_TYPE_T sem = NULL;
#include "app_screen.h"
#include "app_screen/common.h"
#include "app_screen/message.h"
#include "app_screen/paged_message.h"
#include "app_screen/menu.h"
#include "app_screen/timer.h"

#include "app_version.h"
#include "app_sensors.h"

#include "app_screen/page/page_common.h"
#include "app_screen/page/main_page.h"
#include "app_screen/page/do_sensor_calibrate_page.h"
#include "app_screen/page/do_sensor_page.h"
#include "app_screen/page/ph_sensor_calibrate_page.h"
#include "app_screen/page/ph_sensor_page.h"
#include "app_screen/page/sensor_page.h"
#include "app_screen/page/device_info_page.h"
#include "app_screen/page/network_info_page.h"
#include "app_screen/page/time_info_page.h"
#include "app_screen/page/settings_page.h"

static int _show_eeprom_tip(void)
{
    switch (eeprom_init_tip)
    {
    case EEPROM_INIT_TIP_FORMAT:
        BSP_LCD12864_show_row(1, 0, "  格式化成功");
        break;
    case EEPROM_INIT_TIP_RECOVERY:
        BSP_LCD12864_show_row(1, 0, "恢复出厂设置成功");
        break;
    default:
        return APP_ERROR;
    }
    osDelay(2000);
    return APP_OK;
}

static int _splash(void)
{
    char msg[20] = {0};
    BSP_LCD12864_clear_txt();
    BSP_LCD12864_show_row(0, 0, "中易智慧渔业系统");
    BSP_LCD12864_show_row(1, 0, "让水产养殖更容易");
    snprintf(msg, 19, "主控ID:%07u", APP_CONFIG_device_id());
    BSP_LCD12864_show_row(2, 0, msg);
    snprintf(msg, 19, "版本:V%u.%u.%u", APP_FIRMWARE_MAJOR, APP_FIRMWARE_MINOR, APP_FIRMWARE_REVISION);
    BSP_LCD12864_show_row(3, 0, msg);
    return APP_OK;
}

int APP_SCREEN_init(void)
{
    sem = HARDWARE_CREATE_SEMAPHORE();
    do_calibrate_sensor_data_page = &do_sensor_data_page;
    ph_calibrate_sensor_data_page = &ph_sensor_data_page;
    BSP_LCD12864_init();
    return APP_OK;
}

void APP_SCREEN_task_run(void *argument)
{
    int key = 0;
    uint8_t i = 0;
    APP_SCREEN_init();
    BSP_LCD12864_led_on();
    _show_eeprom_tip();
    _splash();
    osDelay(3000);
    APP_SCREEN_show();
    for (;;)
    {
        key = APP_KEY_get();
        if (key > 0)
        {
            if (key == APP_KEY_MENU)
            {
                if (current_page == &main_page)
                {
                    forward_page(&settings_page, current_page);
                }
                else
                {
                    APP_SCREEN_to_main_page();
                }
            }
            else
            {
                APP_SCREEN_on_key_press((uint8_t)key);
            }
        }
        osDelay(20);
        i++;
        if (i > 50)
        {
            APP_SCREEN_on_timer_up();
            i = 0;
        }
    }
}

static void go_to_main_page(void)
{
    if (current_page != &main_page)
    {
        app_screen_history_reset();
        forward_page(&main_page, NULL);
    }
}

void APP_SCREEN_to_main_page(void)
{
    HARDWARE_TAKE_SEMAPHORE(sem);
    if (current_page != &main_page)
    {
        app_screen_history_reset();
        forward_page(&main_page, NULL);
    }
    HARDWARE_GIVE_SEMAPHORE(sem);
}

int APP_SCREEN_show(void)
{
    forward_page(&main_page, NULL);
    return APP_OK;
}

int APP_SCREEN_on_key_press(uint8_t key)
{
    HARDWARE_TAKE_SEMAPHORE(sem);
    if (current_page != NULL && current_page->key_handler != NULL)
    {
        current_page->key_handler(current_page, key);
    }
    HARDWARE_GIVE_SEMAPHORE(sem);
    return APP_OK;
}

int APP_SCREEN_on_timer_up(void)
{
    HARDWARE_TAKE_SEMAPHORE(sem);
    if (current_page != NULL && current_page->timer_handler != NULL)
    {
        current_page->timer_handler(current_page);
    }
    HARDWARE_GIVE_SEMAPHORE(sem);
    return APP_OK;
}
#endif
