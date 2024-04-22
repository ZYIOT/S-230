
app_screen_menu_node_t settings_menus[] = {
    {.name = "设备信息", .type = MENU_NODE_PAGE, .click = NULL, .next = {.page = &device_info_page}, .visibility = NULL},
    {.name = "网络状态", .type = MENU_NODE_PAGE, .click = NULL, .next = {.page = &network_info_page}, .visibility = NULL},
    {.name = "设备时间", .type = MENU_NODE_PAGE, .click = NULL, .next = {.page = &time_info_page}, .visibility = NULL},
    {.name = "传感器", .type = MENU_NODE_PAGE, .click = NULL, .next = {.page = &sensor_page}, .visibility = &has_sensors_visibility},
    APP_SCREEN_MENU_NODE_END};

NEW_MENU_PAGE(settings_page, settings_menus, NULL, NULL, NULL)
