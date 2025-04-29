#ifndef __APP_SCREEN_MESSAGE_H
#define __APP_SCREEN_MESSAGE_H

#define APP_SCREEN_MESSAGE_NODE_END \
    {                               \
        NULL, NULL                  \
    }

typedef struct
{
    char *name;
    char *(*handler)();
} app_screen_message_node_t;

typedef struct
{
    uint8_t start; //从第N个开始显示
    uint8_t max;   // 共有多少行
} app_screen_message_params_t;

#define _CONVERT_MESSAGE_VARIABLES(page)                                               \
    if (page == NULL || page->type != PAGE_TYPE_MESSAGE)                               \
    {                                                                                  \
        return;                                                                        \
    }                                                                                  \
    app_screen_message_params_t *params = (app_screen_message_params_t *)page->params; \
    app_screen_message_node_t *content = (app_screen_message_node_t *)page->content;

void message_page_init(app_screen_page_pt page)
{
    _CONVERT_MESSAGE_VARIABLES(page)
    params->start = 0;
    uint8_t max = 0;
    while (content->name != NULL)
    {
        params->max = max;
        max++;
        content++;
    }
}

void message_page_render(app_screen_page_pt page)
{
    if (!check_page_valid(page))
    {
        return;
    }
    _CONVERT_MESSAGE_VARIABLES(page)
    int idx = 0, row = 0;
    char msg[20] = {0};
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
                snprintf(msg, 19, "%s%s", content->name, content->handler());
                BSP_LCD12864_show_row(row, 0, msg);
            }

            row++;
            if (row > LCD_MAX_ROW)
            {
                return;
            }
        }
        idx++;
        content++;
    }
}

void message_page_event_handler(app_screen_page_pt page, uint8_t key)
{
    _CONVERT_MESSAGE_VARIABLES(page)
    UNUSED(content);
    if (key == APP_KEY_DOWN)
    {
        if (params->max > params->start + 3)
        {
            (params->start)++;
        }
        else
        {
            params->start = 0;
        }
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
}

void message_page_timer_handler(app_screen_page_pt page)
{
    page->render(page);
}

void message_page_before_render(app_screen_page_pt page)
{
    _CONVERT_MESSAGE_VARIABLES(page)
    UNUSED(content);
    params->start = 0;
}

#define NEW_MESSAGE_PAGE(_var, _messages, _check, _before, _after)                 \
    static app_screen_message_params_t _var##_params;                              \
    static app_screen_page_t _var = {.type = PAGE_TYPE_MESSAGE,                    \
                                     .initialized = 0,                             \
                                     .save_history = 1,                            \
                                     .params = &_var##_params,                     \
                                     .content = _messages,                         \
                                     .user_initialized = 0,                        \
                                     .user_init = NULL,                            \
                                     .init = &message_page_init,                   \
                                     .check = _check,                              \
                                     .render = &message_page_render,               \
                                     .before_render = &message_page_before_render, \
                                     .before = _before,                            \
                                     .after = _after,                              \
                                     .timer_handler = &message_page_timer_handler, \
                                     .key_handler = &message_page_event_handler};

#endif
