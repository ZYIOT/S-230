static calibration_fun do_calibration_fun = NULL;

app_screen_timer_node_t do_sensor_calibrate_messages[] = {
    {"", .handler = &get_timer_calibrate_status},
    APP_SCREEN_TIMER_NODE_END};
app_screen_page_pt do_calibrate_sensor_data_page = NULL;

int do_sensor_calibrate_message_check_handler(app_screen_page_pt page)
{
    _CONVERT_TIMER_PARAMS_AND_RETURN(page, APP_OK)
    if (TIMER_PAGE_IS_TIMEOUT(params) && get_calibrate_status() == RS485_OK)
    {
        app_screen_page_pt p;
        app_screen_history_pop(&p); // 弹出校准菜单
        params->next_page = do_calibrate_sensor_data_page;
        params->is_finished = 1;
        params->finished_rc = APP_SCREEN_TIMER_RC_OK;
    }
    if (TIMER_PAGE_IS_OK(params) && params->timer > params->timeout + 2)
    {
        forward_page(do_calibrate_sensor_data_page, page);
        return APP_ERROR;
    }
    return APP_OK;
}

void do_sensor_calibrate_message_before_handler(app_screen_page_pt page, app_screen_page_pt parent);

NEW_TIMER_PAGE(do_sensor_calibrate_message_page, do_sensor_calibrate_messages, NULL, &do_sensor_calibrate_message_before_handler, NULL)
void do_sensor_calibrate_message_before_handler(app_screen_page_pt page, app_screen_page_pt parent)
{
    if (do_calibration_fun != NULL)
    {
        calibrate_status = do_calibration_fun();
    }
    timer_page_set_confirm_handler(&do_sensor_calibrate_message_page, &sensor_calibrate_confirm_handler);
    timer_page_set_on_check(&do_sensor_calibrate_message_page, &do_sensor_calibrate_message_check_handler);
    int timeout = 3;
    _CONVERT_TIMER_PARAMS(page)
    params->timeout = timeout;
}

char *get_do_sensor_calibrate_title(app_screen_page_pt page)
{
    snprintf(app_screen_msg, APP_SCREEN_MSG_BUFFER_SIZE - 1, "确认进行校准？");
    return app_screen_msg;
}

char *get_do_sensor_calibrate_tip(app_screen_page_pt page)
{
    snprintf(app_screen_msg, APP_SCREEN_MSG_BUFFER_SIZE - 1, "请把传感器放在空");
    return app_screen_msg;
}
char *get_do_sensor_calibrate_tip1(app_screen_page_pt page)
{
    snprintf(app_screen_msg, APP_SCREEN_MSG_BUFFER_SIZE - 1, "气中15分钟");
    return app_screen_msg;
}

app_screen_timer_node_t do_sensor_calibrate_process[] = {
    {"", .handler = &get_do_sensor_calibrate_title},
    {"", .handler = &get_do_sensor_calibrate_tip},
    {"", .handler = &get_do_sensor_calibrate_tip1},
    APP_SCREEN_TIMER_NODE_END};

void do_sensor_calibrate_before_handler(app_screen_page_pt page, app_screen_page_pt parent);

NEW_TIMER_PAGE(do_sensor_calibrate_page, do_sensor_calibrate_process, &has_do_sensor_check, &do_sensor_calibrate_before_handler, NULL)

void do_sensor_calibrate_before_handler(app_screen_page_pt page, app_screen_page_pt parent)
{
    _CONVERT_TIMER_PARAMS(page)
    timer_page_set_confirm_handler(&do_sensor_calibrate_page, &sensor_calibrate_confirm_handler);
    timer_page_set_on_check(&do_sensor_calibrate_page, &sensor_calibrate_check_handler);
    params->is_finished = 1;
    params->finished_rc = APP_SCREEN_TIMER_RC_OK;
    params->timeout = 3;
    params->next_page = &do_sensor_calibrate_message_page;
}

static int do_calibration_full(void)
{
    return APP_SENSOR_calibrate(0, RS485_DRIVER_SENSOR_ID_DO, 2, NULL);
}

static int set_do_sensor_calibrate_type_full(app_screen_menu_node_t *node)
{
    calibrate_status = 0;
    do_calibration_fun = &do_calibration_full;
    return APP_OK;
}

app_screen_menu_node_t do_sensor_calibration_menus[] = {
    {.name = "饱和氧校准", .type = MENU_NODE_PAGE, .click = &set_do_sensor_calibrate_type_full, .next = {.page = &do_sensor_calibrate_page}, .visibility = NULL},
    APP_SCREEN_MENU_NODE_END};

NEW_MENU_PAGE(do_sensor_calibration_page, do_sensor_calibration_menus, &has_do_sensor_check, NULL, NULL)
