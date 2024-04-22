#ifndef __APP_SCREEN_MENU_H
#define __APP_SCREEN_MENU_H

#define APP_SCREEN_MENU_NODE_END         \
    {                                    \
        NULL, MENU_NODE_PAGE, NULL, NULL \
    }

typedef enum
{
    MENU_NODE_FUNCTION,
    MENU_NODE_PAGE
} app_screen_menu_type_t;

typedef struct app_screen_menu_node
{
    char *name;
    app_screen_menu_type_t type;
    union {
        app_screen_page_pt page;
        char *(*handler)();

    } next;
    int (*click)(struct app_screen_menu_node *node);
    int (*visibility)();
} app_screen_menu_node_t, *app_screen_menu_node_pt;

typedef struct
{
    uint8_t current; //当前选择项
    uint8_t start;   //从第N个开始显示
    uint8_t max;     // 共有多少个选择
    app_screen_menu_node_pt nodes[30];
} app_screen_menu_params_t;

#define _CONVERT_MENU_VARIABLES(page)                                            \
    if (page == NULL || page->type != PAGE_TYPE_MENU)                            \
    {                                                                            \
        return;                                                                  \
    }                                                                            \
    app_screen_menu_params_t *params = (app_screen_menu_params_t *)page->params; \
    app_screen_menu_node_t *content = (app_screen_menu_node_t *)page->content;

void menu_page_calculate_visibility(app_screen_page_pt page)
{
    _CONVERT_MENU_VARIABLES(page)
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
    if (params->current > params->max)
    {
        params->current = 0;
    }
}

void menu_page_init(app_screen_page_pt page)
{
    _CONVERT_MENU_VARIABLES(page)
    UNUSED(content);
    params->current = 0;
    params->start = 0;
    menu_page_calculate_visibility(page);
}

void menu_page_render(app_screen_page_pt page)
{
    if (!check_page_valid(page))
    {
        return;
    }
    _CONVERT_MENU_VARIABLES(page)
    UNUSED(content);
    int idx = 0, row = 0;
    char msg[20] = {0};
    app_screen_menu_node_pt node = params->nodes[0];
    while (node->name != NULL)
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
            if (node->type == MENU_NODE_PAGE)
            {
                BSP_LCD12864_show_row(row, 1, node->name);
                if (params->current == idx)
                {
                    BSP_LCD12864_show_string(row, 0, ">>");
                    //	LCD_reverse(row);
                }
                else
                {
                    BSP_LCD12864_show_string(row, 0, "　");
                }
            }
            else if (node->type == MENU_NODE_FUNCTION && node->next.handler != NULL)
            {
                snprintf(msg, 19, "%s%s", node->name, node->next.handler());
                BSP_LCD12864_show_row(row, 0, msg);
            }
            row++;
            if (row > LCD_MAX_ROW)
            {
                return;
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

void forward_menu_page(app_screen_page_pt page)
{
    _CONVERT_MENU_VARIABLES(page)
    UNUSED(content);
    int i = 0;
    int rc = 0;
    app_screen_page_pt next_page;
    app_screen_menu_node_pt node = params->nodes[0];
    while (node->name != NULL)
    {
        node = params->nodes[i];
        if (params->current == i && node->type == MENU_NODE_PAGE)
        {
            if (node->click != NULL)
            {
                rc = node->click(node);
                if (rc != APP_OK)
                {
                    return;
                }
            }
            next_page = node->next.page;
            if (next_page == NULL)
            {
                return;
            }

            forward_page(next_page, page);
            return;
        }
        i++;
    }
}

void menu_page_event_handler(app_screen_page_pt page, uint8_t key)
{
    _CONVERT_MENU_VARIABLES(page)
    UNUSED(content);
    menu_page_calculate_visibility(page);
    if (key == APP_KEY_DOWN)
    {
        if (params->current < params->max)
        {
            (params->current)++;
        }
        else
        {
            params->current = 0;
        }
        page->render(page);
    }
    if (key == APP_KEY_UP && (params->current > 0))
    {
        (params->current)--;
        page->render(page);
    }
    if (key == APP_KEY_OK)
    {
        forward_menu_page(page);
    }
    if (key == APP_KEY_BACK)
    {
        backward_page(page);
    }
}

void menu_page_timer_handler(app_screen_page_pt page)
{
    menu_page_calculate_visibility(page);
    page->render(page);
}

void menu_page_before_render(app_screen_page_pt page)
{
    _CONVERT_MENU_VARIABLES(page)
    UNUSED(content);
    params->start = 0;
    params->current = 0;
}

#define NEW_MENU_PAGE(_var, _menus, _check, _before, _after)                    \
    static app_screen_menu_params_t _var##_params;                              \
    static app_screen_page_t _var = {.type = PAGE_TYPE_MENU,                    \
                                     .initialized = 0,                          \
                                     .save_history = 1,                         \
                                     .params = &_var##_params,                  \
                                     .content = _menus,                         \
                                     .user_initialized = 0,                     \
                                     .user_init = NULL,                         \
                                     .init = &menu_page_init,                   \
                                     .check = _check,                           \
                                     .render = &menu_page_render,               \
                                     .before_render = &menu_page_before_render, \
                                     .before = _before,                         \
                                     .after = _after,                           \
                                     .timer_handler = &menu_page_timer_handler, \
                                     .key_handler = &menu_page_event_handler};

#endif
