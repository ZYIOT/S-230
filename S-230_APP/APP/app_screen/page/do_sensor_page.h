
char *get_do_sensor_indicator_saturated()
{
    return get_sensor_indicator_value_string(RS485_DRIVER_SENSOR_ID_DO, "%.1f%%", get_sensor_indicator_value3);
}

char *get_do_sensor_indicator_status(void)
{
    GET_SENSOR(RS485_DRIVER_SENSOR_ID_DO, "", id);
    return get_sensor_indicator_status(sensor->indicator);
}

char *get_sensor_do()
{
    return get_sensor_indicator_value_string(RS485_DRIVER_SENSOR_ID_DO, "%.2f ", get_sensor_indicator_value2);
}

char *get_sensor_temperature()
{
    return get_sensor_indicator_value_string(RS485_DRIVER_SENSOR_ID_DO, "%.1f ", get_sensor_indicator_value1);
}

app_screen_message_node_t do_sensor_data[] = {
    {.name = "�ͺ�: ", .handler = &get_do_sensor_PN},
    {.name = "״̬: ", .handler = &get_do_sensor_indicator_status},
    {.name = "�¶�: ", .handler = &get_sensor_temperature},
    {.name = "����: ", .handler = &get_sensor_do},
    // {.name = "���Ͷ�:", .handler = &get_do_sensor_indicator_saturated},
    APP_SCREEN_MESSAGE_NODE_END};

NEW_MESSAGE_PAGE(do_sensor_data_page, do_sensor_data, &has_do_sensor_check, NULL, NULL)

app_screen_menu_node_t do_sensor_menus[] = {
    {.name = "����������", .type = MENU_NODE_PAGE, .click = NULL, .next = {.page = &do_sensor_data_page}, .visibility = NULL},
    // {.name = "��������Ϣ", .type = MENU_NODE_PAGE, .click = NULL, .next = {.page = &sensor_message_page}, .visibility = NULL},
    {.name = "������У׼", .type = MENU_NODE_PAGE, .click = NULL, .next = {.page = &do_sensor_calibration_page}, .visibility = NULL},
    APP_SCREEN_MENU_NODE_END};

NEW_MENU_PAGE(do_sensor_page, do_sensor_menus, &has_do_sensor_check, NULL, NULL)
