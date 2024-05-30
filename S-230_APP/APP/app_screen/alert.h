#ifndef __APP_SCREEN_ALERT_H
#define __APP_SCREEN_ALERT_H

#define APP_SCREEN_ALERT_NODE_END \
    {                             \
        NULL, NULL                \
    }
#define _CONVERT_ALERT_VARIABLES(page)                                             \
    if (page == NULL || page->type != PAGE_TYPE_ALERT)                             \
    {                                                                              \
        return;                                                                    \
    }                                                                              \
    app_screen_alert_params_t *params = (app_screen_alert_params_t *)page->params; \
    app_screen_alert_node_t *content = (app_screen_alert_node_t *)page->content;

#define _CONVERT_ALERT_PARAMS(page)                    \
    if (page == NULL || page->type != PAGE_TYPE_ALERT) \
    {                                                  \
        return;                                        \
    }                                                  \
    app_screen_alert_params_t *params = (app_screen_alert_params_t *)page->params;

#define _CONVERT_ALERT_PARAMS_AND_RETURN(page, _ret)   \
    if (page == NULL || page->type != PAGE_TYPE_ALERT) \
    {                                                  \
        return _ret;                                   \
    }                                                  \
    app_screen_alert_params_t *params = (app_screen_alert_params_t *)page->params;

#define APP_SCREEN_ALERT_SUCCESS 1
#define APP_SCREEN_ALERT_FAURE 2

typedef struct
{
    char *name;
    char *(*handler)(app_screen_page_pt page);
} app_screen_alert_node_t;

typedef struct
{
    uint8_t start;    //从第N个开始显示
    uint8_t max;      //共有多少行
    uint16_t timer;   //timer计数
    uint16_t timeout; //timer超时数
    uint16_t status;  //alert状态
} app_screen_alert_params_t;

void alert_page_init(app_screen_page_pt page)
{
    _CONVERT_ALERT_VARIABLES(page)
    params->timeout = 5;
    params->timer = 0;
    params->start = 0;
    uint8_t max = 0;
    while (content->name != NULL)
    {
        params->max = max;
        max++;
        content++;
    }
}

void alert_page_before_render(app_screen_page_pt page)
{
    _CONVERT_ALERT_PARAMS(page)
    params->timeout = 5;
    params->start = 0;
    params->timer = 0;
}

void alert_page_render(app_screen_page_pt page)
{
    if (!check_page_valid(page))
    {
        return;
    }
    _CONVERT_ALERT_VARIABLES(page)
    int idx = 0, row = 0;
    char msg[20] = {0};
    int max_row = LCD_MAX_ROW - 1;
    while (content->name != NULL)
    {
        if (idx >= params->start)
        {
            if (content->handler == NULL)
            {
                BSP_LCD12864_show_row(row, 0, content->name);
            }
            else
            {
                snprintf(msg, 19, "%s%s", content->name, content->handler(page));
                BSP_LCD12864_show_row(row, 0, msg);
            }
            row++;
            if (row > max_row)
            {
                return;
            }
        }
        idx++;
        content++;
    }
    BSP_LCD12864_show_row(LCD_MAX_ROW, 0, "返回");
}

void alert_page_key_handler(app_screen_page_pt page, uint8_t key)
{
    _CONVERT_ALERT_PARAMS(page)
    if (key == APP_KEY_DOWN && params->max > params->start + 3)
    {
        (params->start)++;
        page->render(page);
    }
    if (key == APP_KEY_UP && params->start > 0)
    {
        (params->start)--;
        page->render(page);
    }
    if (key == APP_KEY_BACK || key == APP_KEY_OK)
    {
        backward_page(page);
    }
}

void alert_page_timer_handler(app_screen_page_pt page)
{
    _CONVERT_ALERT_PARAMS(page)
    params->timer++;
    if (params->timer >= params->timeout)
    {
        backward_page(page);
    }
    else
    {
        page->render(page);
    }
}

void alert_page_set_timeout(app_screen_page_pt page, uint16_t timeout)
{
    _CONVERT_ALERT_PARAMS(page)
    params->timeout = timeout;
}

#define NEW_ALERT_PAGE(_var, _alerts, _check, _before, _after)                   \
    static app_screen_alert_params_t _var##_params;                              \
    static app_screen_page_t _var = {.type = PAGE_TYPE_ALERT,                    \
                                     .initialized = 0,                           \
                                     .save_history = 0,                          \
                                     .params = &_var##_params,                   \
                                     .content = _alerts,                         \
                                     .user_initialized = 0,                      \
                                     .user_init = NULL,                          \
                                     .init = &alert_page_init,                   \
                                     .check = _check,                            \
                                     .render = &alert_page_render,               \
                                     .before_render = &alert_page_before_render, \
                                     .before = _before,                          \
                                     .after = _after,                            \
                                     .timer_handler = &alert_page_timer_handler, \
                                     .key_handler = &alert_page_key_handler};

#endif
