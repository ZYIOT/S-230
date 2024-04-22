
char *get_device_firmware()
{
    snprintf(app_screen_msg, APP_SCREEN_MSG_BUFFER_SIZE - 1, "%d.%d.%d", APP_FIRMWARE_MAJOR, APP_FIRMWARE_MINOR, APP_FIRMWARE_REVISION);
    return app_screen_msg;
}

char *get_device_hardware()
{
    snprintf(app_screen_msg, APP_SCREEN_MSG_BUFFER_SIZE - 1, "%d.%d.%d", app_config_system.hardware[0], app_config_system.hardware[1], app_config_system.hardware[2]);
    return app_screen_msg;
}

char *get_device_PN()
{
    return PN_CODE;
}

char *get_device_SN()
{
    snprintf(app_screen_msg, APP_SCREEN_MSG_BUFFER_SIZE - 1, "%04u%07u%02u%02u%04u",
             SN_CODE,
             APP_CONFIG_device_id(),
             app_config_system.SN[0],
             app_config_system.SN[1],
             read_uint16_t(&(app_config_system.SN[2])));
    return app_screen_msg;
}

char *get_device_ID()
{
    snprintf(app_screen_msg, APP_SCREEN_MSG_BUFFER_SIZE - 1, "%u", APP_CONFIG_device_id());
    return app_screen_msg;
}

app_screen_message_node_t device_info_messages[] = {
    {.name = "PN:", .handler = &get_device_PN},
    {.name = "ID:", .handler = &get_device_ID},
    // {.name = "SN:", .handler = NULL},
    // {.name = "", .handler = &get_device_SN},
    {.name = "Ӳ��:", .handler = &get_device_hardware},
    {.name = "����:", .handler = &get_device_firmware},
    APP_SCREEN_MESSAGE_NODE_END};

NEW_MESSAGE_PAGE(device_info_page, device_info_messages, NULL, NULL, NULL)
