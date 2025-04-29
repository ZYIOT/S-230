#include "bsp_rtc.h"

static char *get_time_date()
{
    uint8_t year = 0, month = 0, day = 0, hour = 0, minute = 0, second = 0, week = 0;
    int rc = BSP_RTC_get(&year, &month, &day, &hour, &minute, &second, &week);
    if (rc != APP_OK)
        return "����";
    snprintf(app_screen_msg, APP_SCREEN_MSG_BUFFER_SIZE - 1, "20%02d��%02d��%02d��", year, month, day);
    return app_screen_msg;
}

static char *get_time_time()
{
    uint8_t year = 0, month = 0, day = 0, hour = 0, minute = 0, second = 0, week = 0;
    int rc = BSP_RTC_get(&year, &month, &day, &hour, &minute, &second, &week);
    if (rc != APP_OK)
        return "����";
    snprintf(app_screen_msg, APP_SCREEN_MSG_BUFFER_SIZE - 1, "%02dʱ%02d��%02d��", hour, minute, second);
    return app_screen_msg;
}

app_screen_message_node_t time_info_messages[] = {
    {.name = "����: ", .handler = NULL},
    {.name = "  ", .handler = &get_time_date},
    {.name = "ʱ��: ", .handler = NULL},
    {.name = "  ", .handler = &get_time_time},
    APP_SCREEN_MESSAGE_NODE_END};
NEW_MESSAGE_PAGE(time_info_page, time_info_messages, NULL, NULL, NULL)
