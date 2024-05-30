static calibration_fun ph_calibration_fun = NULL;

app_screen_timer_node_t ph_sensor_calibrate_messages[] = {
    {"", .handler = &get_timer_calibrate_status},
    APP_SCREEN_TIMER_NODE_END};

app_screen_page_pt ph_calibrate_sensor_data_page = NULL;

int ph_sensor_calibrate_message_check_handler(app_screen_page_pt page)
{
    _CONVERT_TIMER_PARAMS_AND_RETURN(page, APP_OK)
    if (TIMER_PAGE_IS_TIMEOUT(params) && get_calibrate_status() == RS485_OK)
    {
        app_screen_page_pt p;
        app_screen_history_pop(&p); // 弹出校准菜单
        params->next_page = ph_calibrate_sensor_data_page;
        params->is_finished = 1;
        params->finished_rc = APP_SCREEN_TIMER_RC_OK;
    }
    if (TIMER_PAGE_IS_OK(params) && params->timer > params->timeout + 2)
    {
        forward_page(ph_calibrate_sensor_data_page, page);
        return APP_ERROR;
    }
    return APP_OK;
}

void ph_sensor_calibrate_message_before_handler(app_screen_page_pt page, app_screen_page_pt parent);

NEW_TIMER_PAGE(ph_sensor_calibrate_message_page, ph_sensor_calibrate_messages, &has_ph_sensor_check, &ph_sensor_calibrate_message_before_handler, NULL)

void ph_sensor_calibrate_message_before_handler(app_screen_page_pt page, app_screen_page_pt parent)
{
    if (ph_calibration_fun != NULL)
    {
        calibrate_status = ph_calibration_fun();
    }
    timer_page_set_on_check(&ph_sensor_calibrate_message_page, &ph_sensor_calibrate_message_check_handler);
    timer_page_set_confirm_handler(&ph_sensor_calibrate_message_page, &sensor_calibrate_confirm_handler);
    _CONVERT_TIMER_PARAMS(page)
    params->timeout = 3;
}

char *get_ph_sensor_calibrate_title(app_screen_page_pt page)
{
    _CONVERT_TIMER_PARAMS_AND_RETURN(page, "程序出错了");
    if (TIMER_PAGE_IS_ERROR(params))
    {
        snprintf(app_screen_msg, APP_SCREEN_MSG_BUFFER_SIZE - 1, "无法校准该传感器");
    }
    else
    {
        snprintf(app_screen_msg, APP_SCREEN_MSG_BUFFER_SIZE - 1, "确认进行校准？");
    }
    return app_screen_msg;
}

char *get_ph_sensor_calibrate_tip(app_screen_page_pt page)
{
    _CONVERT_TIMER_PARAMS_AND_RETURN(page, "");
    if (TIMER_PAGE_IS_ERROR(params))
    {
        snprintf(app_screen_msg, APP_SCREEN_MSG_BUFFER_SIZE - 1, "是否把传感器放入");
    }
    else
    {
        snprintf(app_screen_msg, APP_SCREEN_MSG_BUFFER_SIZE - 1, "请把传感器放在缓");
    }
    return app_screen_msg;
}

char *get_ph_sensor_calibrate_tip1(app_screen_page_pt page)
{
    snprintf(app_screen_msg, APP_SCREEN_MSG_BUFFER_SIZE - 1, "冲液中");
    return app_screen_msg;
}

app_screen_timer_node_t ph_sensor_calibrate_process[] = {
    {"", .handler = &get_ph_sensor_calibrate_title},
    {"", .handler = &get_ph_sensor_calibrate_tip},
    {"", .handler = &get_ph_sensor_calibrate_tip1},
    APP_SCREEN_TIMER_NODE_END};

void ph_sensor_calibrate_before_handler(app_screen_page_pt page, app_screen_page_pt parent);

NEW_TIMER_PAGE(ph_sensor_calibrate_page, ph_sensor_calibrate_process, &has_ph_sensor_check, &ph_sensor_calibrate_before_handler, NULL)

#define CHECK_CALIBRATE_RANGE(value)                                                               \
    if (calibrate_type == RS485_SENSOR_PH_CALIBRATE_##value && (ph > value - 2 || ph < value + 2)) \
        return APP_OK;

int ph_sensor_calibrate_ph230(uint8_t calibrate_type, float ph)
{
    CHECK_CALIBRATE_RANGE(4)
    // CHECK_CALIBRATE_RANGE(7)
    CHECK_CALIBRATE_RANGE(10)
    return APP_ERROR;
}

void ph_sensor_calibrate_before_handler(app_screen_page_pt page, app_screen_page_pt parent)
{
    _CONVERT_TIMER_PARAMS(page)
    timer_page_set_confirm_handler(&ph_sensor_calibrate_page, &sensor_calibrate_confirm_handler);
    timer_page_set_on_check(&ph_sensor_calibrate_page, &sensor_calibrate_check_handler);
    params->is_finished = 1;
    params->finished_rc = APP_SCREEN_TIMER_RC_OK;
    params->timeout = 3;
    params->next_page = &ph_sensor_calibrate_message_page;
}

static int ph_calibration_4(void)
{
    return APP_SENSOR_calibrate(0, RS485_DRIVER_SENSOR_ID_PH, 2, NULL);
}

static int ph_calibration_7(void)
{
    return APP_SENSOR_calibrate(0, RS485_DRIVER_SENSOR_ID_PH, 1, NULL);
}

static int ph_calibration_10(void)
{
    return APP_SENSOR_calibrate(0, RS485_DRIVER_SENSOR_ID_PH, 3, NULL);
}

static int set_ph_sensor_calibrate_type_4(app_screen_menu_node_t *node)
{
    calibrate_status = 0;
    ph_calibration_fun = &ph_calibration_4;
    return APP_OK;
}

static int set_ph_sensor_calibrate_type_7(app_screen_menu_node_t *node)
{
    calibrate_status = 0;
    ph_calibration_fun = &ph_calibration_7;
    return APP_OK;
}

static int set_ph_sensor_calibrate_type_10(app_screen_menu_node_t *node)
{
    calibrate_status = 0;
    ph_calibration_fun = &ph_calibration_10;
    return APP_OK;
}

app_screen_menu_node_t ph_sensor_calibration_menus[] = {
    {.name = "pH7 校准", .type = MENU_NODE_PAGE, .click = &set_ph_sensor_calibrate_type_7, .next = {.page = &ph_sensor_calibrate_page}, .visibility = NULL},
    // {.name = "pH4 校准", .type = MENU_NODE_PAGE, .click = &set_ph_sensor_calibrate_type_4, .next = {.page = &ph_sensor_calibrate_page}, .visibility = NULL},
    {.name = "pH10校准", .type = MENU_NODE_PAGE, .click = &set_ph_sensor_calibrate_type_10, .next = {.page = &ph_sensor_calibrate_page}, .visibility = NULL},
    APP_SCREEN_MENU_NODE_END};

NEW_MENU_PAGE(ph_sensor_calibration_page, ph_sensor_calibration_menus, &has_ph_sensor_check, NULL, NULL)
