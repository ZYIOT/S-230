app_screen_menu_node_t sensor_menus[] = {
    {.name = "����������", .type = MENU_NODE_PAGE, .next = {.page = &do_sensor_page}, .visibility = &get_sensor_do_visibility},
    {.name = "pH������", .type = MENU_NODE_PAGE, .next = {.page = &ph_sensor_page}, .visibility = &get_sensor_ph_visibility},
    APP_SCREEN_MENU_NODE_END};

NEW_MENU_PAGE(sensor_page, sensor_menus, &has_sensor_check, NULL, NULL)
