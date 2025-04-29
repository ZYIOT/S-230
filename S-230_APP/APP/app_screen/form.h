#ifndef __APP_SCREEN_FORM_H
#define __APP_SCREEN_FORM_H

#define APP_SCREEN_FORM_NODE_END            \
    {                                       \
        NULL, FORM_NODE_MESSAGE, NULL, NULL \
    }

typedef enum
{
    FORM_NODE_MESSAGE,
    FORM_NODE_INPUT,
    FORM_NODE_SELECT
} app_screen_form_type_t;

typedef struct app_screen_form_node
{
    char *name;
    app_screen_form_type_t type;
    union {
        app_screen_page_pt page;
        char *(*handler)();
    } next;
    int (*click)(struct app_screen_form_node *node);
} app_screen_form_node_t;

typedef struct
{
    uint8_t current; //当前选择项
    uint8_t start;   //从第N个开始显示
    uint8_t max;     // 共有多少个选择
} app_screen_form_params_t;

#define _CONVERT_FORM_VARIABLES(page)                                            \
    if (page == NULL || page->type != PAGE_TYPE_FORM)                            \
    {                                                                            \
        return;                                                                  \
    }                                                                            \
    app_screen_form_params_t *params = (app_screen_form_params_t *)page->params; \
    app_screen_form_node_t *content = (app_screen_form_node_t *)page->content;

void form_page_init(app_screen_page_pt page)
{
    _CONVERT_FORM_VARIABLES(page)
    params->current = 0;
    params->start = 0;
    uint8_t max = 0;
    while (content->name != NULL)
    {
        params->max = max;
        max++;
        content++;
    }
}

void form_page_render(app_screen_page_pt page)
{
    if (!check_page_valid(page))
    {
        return;
    }
    _CONVERT_FORM_VARIABLES(page)
    int idx = 0, row = 0;
    char msg[20] = {0};
    while (content->name != NULL)
    {
        if (params->current < params->start)
        {
            params->start = params->current;
        }
        if (params->current - params->start > LCD_MAX_ROW)
        {
            params->start = idx;
        }
        if (params->current - params->start <= LCD_MAX_ROW && idx >= params->start)
        {
            if (content->type == FORM_NODE_PAGE)
            {
                BSP_LCD12864_show_row(row, 1, content->name);
                if (params->current == idx)
                {
                    BSP_LCD12864_show_string(row, 0, "●");
                    //	LCD_reverse(row);
                }
                else
                {
                    BSP_LCD12864_show_string(row, 0, "　");
                }
            }
            else if (content->type == FORM_NODE_FUNCTION && content->next.handler != NULL)
            {
                snprintf(msg, 19, "%s%s", content->name, content->next.handler());
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

void forward_form_page(app_screen_page_pt page)
{
    _CONVERT_FORM_VARIABLES(page)
    int i = 0;
    int rc = 0;
    app_screen_page_pt next_page;
    while (content->name != NULL)
    {
        if (params->current == i && content->type == FORM_NODE_PAGE)
        {
            next_page = content->next.page;
            if (next_page == NULL)
            {
                return;
            }
            if (content->click != NULL)
            {
                rc = content->click(content);
                if (rc != APP_OK)
                {
                    return;
                }
            }
            forward_page(next_page, page);
            return;
        }
        i++;
        content++;
    }
}

void form_page_event_handler(app_screen_page_pt page, uint8_t key)
{
    _CONVERT_FORM_VARIABLES(page)
    UNUSED(content);
    if (key == APP_KEY_DOWN && (params->current < params->max))
    {
        (params->current)++;
        page->render(page);
    }
    if (key == APP_KEY_UP && (params->current > 0))
    {
        (params->current)--;
        page->render(page);
    }
    if (key == APP_KEY_OK)
    {
        forward_form_page(page);
    }
    if (key == APP_KEY_BACK)
    {
        backward_page(page);
    }
}

void form_page_before_render(app_screen_page_pt page)
{
    _CONVERT_FORM_VARIABLES(page)
    UNUSED(content);
    params->start = 0;
    params->current = 0;
}

#define NEW_FORM_PAGE(_var, _forms, _check, _before, _after)                    \
    static app_screen_form_params_t _var##_params;                              \
    static app_screen_page_t _var = {.type = PAGE_TYPE_FORM,                    \
                                     .initialized = 0,                          \
                                     .save_history = 1,                         \
                                     .params = &_var##_params,                  \
                                     .content = _forms,                         \
                                     .user_initialized = 0,                     \
                                     .user_init = NULL,                         \
                                     .init = &form_page_init,                   \
                                     .check = _check,                           \
                                     .render = &form_page_render,               \
                                     .before_render = &form_page_before_render, \
                                     .before = _before,                         \
                                     .after = _after,                           \
                                     .timer_handler = NULL,                     \
                                     .key_handler = &form_page_event_handler};

#endif
