#ifndef __APP_SCREEN_TIMER_H
#define __APP_SCREEN_TIMER_H

#define APP_SCREEN_TIMER_NODE_END \
    {                             \
        NULL, NULL                \
    }
#define _CONVERT_TIMER_VARIABLES(page)                                             \
    if (page == NULL || page->type != PAGE_TYPE_TIMER)                             \
    {                                                                              \
        return;                                                                    \
    }                                                                              \
    app_screen_timer_params_t *params = (app_screen_timer_params_t *)page->params; \
    app_screen_timer_node_t *content = (app_screen_timer_node_t *)page->content;

#define _CONVERT_TIMER_PARAMS(page)                    \
    if (page == NULL || page->type != PAGE_TYPE_TIMER) \
    {                                                  \
        return;                                        \
    }                                                  \
    app_screen_timer_params_t *params = (app_screen_timer_params_t *)page->params;

#define _CONVERT_TIMER_PARAMS_AND_RETURN(page, _ret)   \
    if (page == NULL || page->type != PAGE_TYPE_TIMER) \
    {                                                  \
        return _ret;                                   \
    }                                                  \
    app_screen_timer_params_t *params = (app_screen_timer_params_t *)page->params;
#define TIMER_PAGE_IS_TIMEOUT(params) \
    (params->is_finished == 1 && params->finished_rc == APP_SCREEN_TIMER_RC_TIMEOUT)

#define TIMER_PAGE_IS_OK(params) \
    (params->is_finished == 1 && params->finished_rc == APP_SCREEN_TIMER_RC_OK)

#define TIMER_PAGE_IS_ERROR(params) \
    (params->is_finished == 1 && params->finished_rc == APP_SCREEN_TIMER_RC_ERROR)

#define APP_SCREEN_TIMER_RC_OK 0
#define APP_SCREEN_TIMER_RC_ERROR 1
#define APP_SCREEN_TIMER_RC_TIMEOUT 4
typedef struct
{
    char *name;
    char *(*handler)(app_screen_page_pt page);
} app_screen_timer_node_t;

typedef int (*app_screen_timer_check_handler)(app_screen_page_s_pt page);
typedef struct
{
    uint8_t start;       //从第N个开始显示
    uint8_t max;         // 共有多少行
    uint8_t is_finished; //是否已经完成
    uint8_t finished_rc; //完成的状态
    uint16_t timeout;    //timer超时数
    uint16_t timer;      //timer计数
    app_screen_timer_check_handler check;
    app_screen_handler confirm_handler;
    app_screen_page_pt next_page;
} app_screen_timer_params_t;

void timer_page_init(app_screen_page_pt page)
{
    _CONVERT_TIMER_VARIABLES(page)
    params->start = 0;
    params->timeout = 60;
    params->timer = 0;
    params->is_finished = 0;
    params->finished_rc = 0;
    params->check = NULL;
    params->confirm_handler = NULL;
    params->next_page = NULL;
    uint8_t max = 0;
    while (content->name != NULL)
    {
        params->max = max;
        max++;
        content++;
    }
}

void timer_page_before_render(app_screen_page_pt page)
{
    _CONVERT_TIMER_PARAMS(page)
    params->start = 0;
    params->timer = 0;
    params->is_finished = 0;
    params->finished_rc = 0;
}

void timer_page_render(app_screen_page_pt page)
{
    if (!check_page_valid(page))
    {
        return;
    }
    _CONVERT_TIMER_VARIABLES(page)
    int idx = 0, row = 0;
    char msg[20] = {0};
    int max_row = LCD_MAX_ROW;
    if (params->confirm_handler != NULL)
    {
        params->confirm_handler(page);
        max_row--;
    }
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
}

void timer_page_key_handler(app_screen_page_pt page, uint8_t key)
{
    _CONVERT_TIMER_PARAMS(page)
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
    if (key == APP_KEY_BACK)
    {
        backward_page(page);
    }

    if (key == APP_KEY_OK && params->is_finished == 1 && params->next_page != NULL)
    {
        forward_page(params->next_page, page);
    }
}

void timer_page_timer_handler(app_screen_page_pt page)
{
    _CONVERT_TIMER_PARAMS(page)
    if (params->timer < 0xFFFF)
    {
        params->timer++;
    }
    if (params->is_finished != 1 && params->timer >= params->timeout)
    {
        params->is_finished = 1;
        params->finished_rc = APP_SCREEN_TIMER_RC_TIMEOUT;
    }
    if (params->check != NULL && params->check(page) != APP_OK)
    {
        return;
    }
    else
    {
        page->render(page);
    }
}

void timer_page_set_timeout(app_screen_page_pt page, uint16_t timeout)
{
    _CONVERT_TIMER_PARAMS(page)
    params->timeout = timeout;
}

void timer_page_set_on_check(app_screen_page_pt page, app_screen_timer_check_handler handler)
{
    _CONVERT_TIMER_PARAMS(page)
    params->check = handler;
}
void timer_page_set_confirm_handler(app_screen_page_pt page, app_screen_handler handler)
{
    _CONVERT_TIMER_PARAMS(page)
    params->confirm_handler = handler;
}

void timer_default_confirm_handler(app_screen_page_pt page)
{
    _CONVERT_TIMER_PARAMS(page)
    if (params->is_finished == 1)
    {
        BSP_LCD12864_show_row(LCD_MAX_ROW, 6, "返回");
    }
    else
    {
        BSP_LCD12864_show_row(LCD_MAX_ROW, 6, "取消");
    }
    if (TIMER_PAGE_IS_OK(params) && params->next_page != NULL)
    {
        BSP_LCD12864_show_string(LCD_MAX_ROW, 0, "确认");
    }
}

#define NEW_TIMER_PAGE(_var, _timers, _check, _before, _after)                   \
    static app_screen_timer_params_t _var##_params;                              \
    static app_screen_page_t _var = {.type = PAGE_TYPE_TIMER,                    \
                                     .initialized = 0,                           \
                                     .save_history = 0,                          \
                                     .params = &_var##_params,                   \
                                     .content = _timers,                         \
                                     .user_initialized = 0,                      \
                                     .user_init = NULL,                          \
                                     .init = &timer_page_init,                   \
                                     .check = _check,                            \
                                     .render = &timer_page_render,               \
                                     .before_render = &timer_page_before_render, \
                                     .before = _before,                          \
                                     .after = _after,                            \
                                     .timer_handler = &timer_page_timer_handler, \
                                     .key_handler = &timer_page_key_handler};

#endif
