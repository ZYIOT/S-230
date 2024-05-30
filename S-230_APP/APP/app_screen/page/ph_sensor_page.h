char *get_ph_sensor_indicator_status(void)
{
    GET_SENSOR(RS485_DRIVER_SENSOR_ID_PH, "", id);
    return get_sensor_indicator_status(sensor->indicator);
}

char *get_sensor_ph()
{
    return get_sensor_indicator_value_string(RS485_DRIVER_SENSOR_ID_PH, "%.2f ", get_sensor_indicator_value1);
}

char *get_ph_sensor_indicator_vm()
{
    return get_sensor_indicator_vm_string(RS485_DRIVER_SENSOR_ID_PH, "%d", get_sensor_indicator_vm1);
}

app_screen_message_node_t ph_sensor_data[] = {
    {.name = "�ͺ�: ", .handler = &get_ph_sensor_PN},
    {.name = "״̬: ", .handler = &get_ph_sensor_indicator_status},
    {.name = "pHֵ: ", .handler = &get_sensor_ph},
    {.name = "mVֵ: ", .handler = &get_ph_sensor_indicator_vm},
    APP_SCREEN_MESSAGE_NODE_END};

NEW_MESSAGE_PAGE(ph_sensor_data_page, ph_sensor_data, &has_ph_sensor_check, NULL, NULL)

app_screen_menu_node_t ph_sensor_menus[] = {
    {.name = "����������", .type = MENU_NODE_PAGE, .click = NULL, .next = {.page = &ph_sensor_data_page}, .visibility = NULL},
    // {.name = "��������Ϣ", .type = MENU_NODE_PAGE, .click = NULL, .next = {.page = &sensor_message_page}, .visibility = NULL},
    {.name = "������У׼", .type = MENU_NODE_PAGE, .click = NULL, .next = {.page = &ph_sensor_calibration_page}, .visibility = NULL},
    APP_SCREEN_MENU_NODE_END};

NEW_MENU_PAGE(ph_sensor_page, ph_sensor_menus, &has_ph_sensor_check, NULL, NULL)
