char *show_operation_alert_title(app_screen_page_pt page)
{
    _CONVERT_ALERT_PARAMS_AND_RETURN(page, "")
    switch (params->status)
    {
    case APP_SCREEN_ALERT_SUCCESS:
        snprintf(app_screen_msg, APP_SCREEN_MSG_BUFFER_SIZE - 1, "    操作成功");
        break;
    case APP_SCREEN_ALERT_FAURE:
        snprintf(app_screen_msg, APP_SCREEN_MSG_BUFFER_SIZE - 1, "    操作失败");
        break;
    default:
        snprintf(app_screen_msg, APP_SCREEN_MSG_BUFFER_SIZE - 1, "    未知结果");
        break;
    }

    return app_screen_msg;
}

app_screen_alert_node_t operation_alert_process[] = {
    {"", .handler = NULL},
    {"", .handler = &show_operation_alert_title},
    APP_SCREEN_ALERT_NODE_END};

NEW_ALERT_PAGE(operation_alert_page, operation_alert_process, NULL, NULL, NULL)

void show_operation_page(uint16_t status)
{
    operation_alert_page_params.status = status;
    forward_page(&operation_alert_page, current_page);
}
