#include "bsp_rs485.h"
#include "app_sensors.h"
#define APP_SCREEN_MSG_BUFFER_SIZE 20

static char app_screen_msg[APP_SCREEN_MSG_BUFFER_SIZE] = {0};
#define GET_SENSOR(addr, _ret, check_field)          \
    rs485_sensor_pt sensor = NULL;                   \
    int rc = APP_SENSORS_sensor_get(addr, &sensor);  \
    if (rc != APP_OK || sensor->check_field == NULL) \
    {                                                \
        return _ret;                                 \
    }

char *get_sensor_firmware(uint8_t addr)
{
    GET_SENSOR(addr, "--", firmware);
    snprintf(app_screen_msg, APP_SCREEN_MSG_BUFFER_SIZE - 1, "%u.%u.%u", sensor->firmware[0], sensor->firmware[1], sensor->firmware[2]);
    return app_screen_msg;
}

char *get_sensor_hardware(uint8_t addr)
{
    GET_SENSOR(addr, "--", hardware);
    snprintf(app_screen_msg, APP_SCREEN_MSG_BUFFER_SIZE - 1, "%u.%u.%u", sensor->hardware[0], sensor->hardware[1], sensor->hardware[2]);
    return app_screen_msg;
}

char *get_sensor_pn(uint8_t addr)
{
    GET_SENSOR(addr, "--", action);
    return sensor->action->PN;
}

char *get_sensor_sn(uint8_t addr)
{
    GET_SENSOR(addr, "--", SN);
    snprintf(app_screen_msg, APP_SCREEN_MSG_BUFFER_SIZE - 1, "%04u%02u%04u%04u", read_uint16_t_BE(&(sensor->SN[0])), sensor->SN[2], read_uint16_t_BE(&(sensor->SN[3])), read_uint16_t_BE(&(sensor->SN[5])));
    return app_screen_msg;
}

char *get_sensor_addr(uint8_t addr)
{
    GET_SENSOR(addr, "", id);
    snprintf(app_screen_msg, APP_SCREEN_MSG_BUFFER_SIZE - 1, "%d", sensor->id);
    return app_screen_msg;
}

#define DEFINE_GET_SENSOR_INDICATOR(idx)                                 \
    static float get_sensor_indicator_value##idx(rs485_sensor_pt sensor) \
    {                                                                    \
        return sensor->indicator->value##idx;                            \
    }                                                                    \
    static int16_t get_sensor_indicator_vm##idx(rs485_sensor_pt sensor)  \
    {                                                                    \
        return sensor->indicator->vm##idx;                               \
    }

DEFINE_GET_SENSOR_INDICATOR(1)
DEFINE_GET_SENSOR_INDICATOR(2)
DEFINE_GET_SENSOR_INDICATOR(3)
DEFINE_GET_SENSOR_INDICATOR(4)

typedef float (*get_sensor_indicator_value_fun)(rs485_sensor_pt sensor);
typedef int16_t (*get_sensor_indicator_vm_fun)(rs485_sensor_pt sensor);
typedef int (*calibration_fun)(void);
char *get_sensor_indicator_status(rs485_sensor_indicator_pt indicator)
{
    if (indicator == NULL)
    {
        return "暂无数据";
    }
    switch (indicator->status)
    {
    case RS485_OK:
        snprintf(app_screen_msg, APP_SCREEN_MSG_BUFFER_SIZE - 1, "正常");
        break;
    case RS485_READ_ERROR:
        snprintf(app_screen_msg, APP_SCREEN_MSG_BUFFER_SIZE - 1, "读取超时");
        break;
    case RS485_CRC_ERROR:
        snprintf(app_screen_msg, APP_SCREEN_MSG_BUFFER_SIZE - 1, "校验码错误");
        break;
    default:
        snprintf(app_screen_msg, APP_SCREEN_MSG_BUFFER_SIZE - 1, "读取错误");
        break;
    }

    return app_screen_msg;
}
static int calibrate_status = 0;
int get_calibrate_status()
{
    return calibrate_status;
}
char *get_ph_sensor_PN(void)
{
    return get_sensor_pn(RS485_DRIVER_SENSOR_ID_PH);
}
char *get_do_sensor_PN(void)
{
    return get_sensor_pn(RS485_DRIVER_SENSOR_ID_DO);
}
char *get_calibrate_status_name()
{
    switch (get_calibrate_status())
    {
    case -100:
        snprintf(app_screen_msg, APP_SCREEN_MSG_BUFFER_SIZE - 1, "正在校准");
        break;
    case -255:
        snprintf(app_screen_msg, APP_SCREEN_MSG_BUFFER_SIZE - 1, "不支持该操作");
        break;
    case RS485_OK:
        snprintf(app_screen_msg, APP_SCREEN_MSG_BUFFER_SIZE - 1, "校准成功");
        break;
    case RS485_PARAM_ERROR:
        snprintf(app_screen_msg, APP_SCREEN_MSG_BUFFER_SIZE - 1, "参数不正确");
        break;
    case RS485_READ_ERROR:
        snprintf(app_screen_msg, APP_SCREEN_MSG_BUFFER_SIZE - 1, "校准超时");
        break;
    case RS485_ERROR:
        snprintf(app_screen_msg, APP_SCREEN_MSG_BUFFER_SIZE - 1, "校准失败");
        break;
    default:
        break;
    }
    return app_screen_msg;
}

char *get_timer_calibrate_status(app_screen_page_pt page)
{
    _CONVERT_TIMER_PARAMS_AND_RETURN(page, "正在校准")
    if (params->is_finished != 1)
    {
        return "正在校准";
    }
    return get_calibrate_status_name();
}

app_screen_message_node_t sensor_messages[] = {
    {.name = "PN:", .handler = &get_sensor_pn},
    {.name = "地址:", .handler = &get_sensor_addr},
    {.name = "软件:", .handler = &get_sensor_firmware},
    {.name = "硬件:", .handler = &get_sensor_hardware},
    {.name = "SN:", .handler = NULL},
    {.name = "", .handler = &get_sensor_sn},
    APP_SCREEN_MESSAGE_NODE_END};

NEW_MESSAGE_PAGE(sensor_message_page, sensor_messages, NULL, NULL, NULL)

void sensor_calibrate_confirm_handler(app_screen_page_pt page)
{
    _CONVERT_TIMER_PARAMS(page)
    if (params->is_finished == 1)
    {
        BSP_LCD12864_show_row(LCD_MAX_ROW, 6, "返回");
        if (TIMER_PAGE_IS_OK(params))
        {
            BSP_LCD12864_show_string(LCD_MAX_ROW, 0, "确认");
        }
    }
    else
    {
        BSP_LCD12864_show_row(LCD_MAX_ROW, 6, "取消");
    }
}

int sensor_calibrate_check_handler(app_screen_page_pt page)
{
    _CONVERT_TIMER_PARAMS_AND_RETURN(page, APP_OK)
    if (TIMER_PAGE_IS_TIMEOUT(params))
    {
        params->finished_rc = APP_SCREEN_TIMER_RC_OK;
    }
    return APP_OK;
}
