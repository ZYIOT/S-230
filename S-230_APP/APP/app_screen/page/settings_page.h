
app_screen_menu_node_t settings_menus[] = {
    {.name = "�豸��Ϣ", .type = MENU_NODE_PAGE, .click = NULL, .next = {.page = &device_info_page}, .visibility = NULL},
    {.name = "����״̬", .type = MENU_NODE_PAGE, .click = NULL, .next = {.page = &network_info_page}, .visibility = NULL},
    {.name = "�豸ʱ��", .type = MENU_NODE_PAGE, .click = NULL, .next = {.page = &time_info_page}, .visibility = NULL},
    {.name = "������", .type = MENU_NODE_PAGE, .click = NULL, .next = {.page = &sensor_page}, .visibility = &has_sensors_visibility},
    APP_SCREEN_MENU_NODE_END};

NEW_MENU_PAGE(settings_page, settings_menus, NULL, NULL, NULL)
