#ifndef __APP_SCREEN_PAGED_MESSAGE_H
#define __APP_SCREEN_PAGED_MESSAGE_H

#define APP_SCREEN_PAGED_MESSAGE_NODE_END \
    {                                     \
        NULL, NULL, NULL                  \
    }

typedef struct
{
    char *name;
    char *(*handler)();
    int (*visibility)();
} app_screen_paged_message_node_t, *app_screen_paged_message_node_pt;

typedef struct
{
    uint8_t start; //从第N个开始显示
    uint8_t max;   // 共有多少行
    uint8_t timer;
    app_screen_paged_message_node_pt nodes[30];
} app_screen_paged_message_params_t;

#define _CONVERT_PAGED_MESSAGE_VARIABLES(page)                                                     \
    if (page == NULL || page->type != PAGE_TYPE_PAGED_MESSAGE)                                     \
    {                                                                                              \
        return;                                                                                    \
    }                                                                                              \
    app_screen_paged_message_params_t *params = (app_screen_paged_message_params_t *)page->params; \
    app_screen_paged_message_node_t *content = (app_screen_paged_message_node_t *)page->content;

void paged_message_page_calculate_visibility(app_screen_page_pt page)
{
    _CONVERT_PAGED_MESSAGE_VARIABLES(page)
    uint8_t max = 0;
    while (content->name != NULL)
    {
        if (content->visibility == NULL || content->visibility())
        {
            params->max = max;
            params->nodes[max] = content;
            max++;
        }
        content++;
    }
    params->nodes[max] = content;
}

void paged_message_page_init(app_screen_page_pt page)
{
    _CONVERT_PAGED_MESSAGE_VARIABLES(page)
    UNUSED(content);
    params->start = 0;
    paged_message_page_calculate_visibility(page);
}

void paged_message_page_render(app_screen_page_pt page)
{
    _CONVERT_PAGED_MESSAGE_VARIABLES(page)
    UNUSED(content);
    int idx = 0, row = 0;
    char msg[20] = {0};
    app_screen_paged_message_node_pt node = params->nodes[0];
    while (node->name != NULL)
    {
        if (idx >= params->start)
        {
            if (node->handler == NULL)
            {
                BSP_LCD12864_show_row(row, 0, node->name);
            }
            else
            {
                snprintf(msg, 19, "%s%s", node->name, node->handler());
                BSP_LCD12864_show_row(row, 0, msg);
            }
            row++;
            if (row > LCD_MAX_ROW)
            {
                break;
            }
        }
        idx++;
        node = params->nodes[idx];
    }
    for (; row <= LCD_MAX_ROW; row++)
    {
        BSP_LCD12864_show_row(row, 0, "  ");
    }
}

void paged_message_page_event_handler(app_screen_page_pt page, uint8_t key)
{
    if (!check_page_valid(page))
    {
        return;
    }
    _CONVERT_PAGED_MESSAGE_VARIABLES(page)
    UNUSED(content);
    paged_message_page_calculate_visibility(page);
    if (key == APP_KEY_DOWN)
    {
        params->start += LCD_MAX_ROW;
        if (params->start > params->max)
        {
            params->start = 0;
        }
        params->timer = 0;
        page->render(page);
    }
    if (key == APP_KEY_UP && params->start > 0)
    {
        params->start -= LCD_MAX_ROW;
        if (params->start > params->max)
        {
            params->start = 0;
        }
        params->timer = 0;
        page->render(page);
    }
    if (key == APP_KEY_BACK)
    {
        backward_page(page);
    }
}

void paged_message_page_timer_handler(app_screen_page_pt page)
{
    _CONVERT_PAGED_MESSAGE_VARIABLES(page)
    UNUSED(content);
    paged_message_page_calculate_visibility(page);
    params->timer++;
    if (params->timer % 5 == 0)
    {
        params->start += LCD_MAX_ROW;
    }
    if (params->start > params->max)
    {
        params->start = 0;
    }
    page->render(page);
}

void paged_message_page_before_render(app_screen_page_pt page)
{
    _CONVERT_PAGED_MESSAGE_VARIABLES(page)
    UNUSED(content);
    params->timer = 0;
    params->start = 0;
}

#define NEW_PAGED_MESSAGE_PAGE(_var, _paged_messages, _check, _before, _after)           \
    static app_screen_paged_message_params_t _var##_params;                              \
    static app_screen_page_t _var = {.type = PAGE_TYPE_PAGED_MESSAGE,                    \
                                     .initialized = 0,                                   \
                                     .save_history = 1,                                  \
                                     .params = &_var##_params,                           \
                                     .content = _paged_messages,                         \
                                     .user_initialized = 0,                              \
                                     .user_init = NULL,                                  \
                                     .init = &paged_message_page_init,                   \
                                     .check = _check,                                    \
                                     .render = &paged_message_page_render,               \
                                     .before_render = &paged_message_page_before_render, \
                                     .before = _before,                                  \
                                     .after = _after,                                    \
                                     .timer_handler = &paged_message_page_timer_handler, \
                                     .key_handler = &paged_message_page_event_handler};

#endif
