#ifndef __APP_RELAY_H
#define __APP_RELAY_H
#ifdef BOARD_HAS_RELAY
#ifdef __cplusplus
extern "C"
{
#endif
#include "app.h"
#include "bsp_protocol.h"

#define APP_RELAY_channel_size() RELAY_CHANNEL_SIZE;
// #define APP_REPLACE_RELAY_DEBUG     // 调试时用上位机替代继电器控制板 
    enum
    {
        APP_RELAY_STATUS_OK = 0,        // 正常
        APP_RELAY_STATUS_BROKEN,        // 无输出电流
        APP_RELAY_STATUS_LOSS_PHASE,    // 缺相
        APP_RELAY_STATUS_OVERLOAD,      // 过载
        APP_RELAY_STATUS_SHORT_CIRCUIT, // 短路
        APP_RELAY_STATUS_OUV,           // 过欠压
        // APP_RELAY_STATUS_LEAKAGE,           // 漏电
        APP_RELAY_STATUS_LOAD_ERROR,  // 负载类型错误
        APP_RELAY_STATUS_PHASE_ERROR, // 进线与负载不匹配
        APP_RELAY_STATUS_MAX,         // 状态种类个数
    };

    typedef struct
    {
        uint8_t calculate_status;    // 计算的开关状态，可能还未同步继电器
        uint8_t limit_status;        // 智能控制通道状态
        uint16_t limit_status_is_on; // 智能控制通道状态是开的状态
        uint8_t times_status;        // 定时控制通道状态
        uint8_t manual_status;       // 手动控制通道状态

        uint8_t remote_manual_status; // 远程手动控制通道状态
        uint8_t local_manual_status;  // 本地手动控制通道状态

        uint8_t all_off_manual_status; // 手动全关状态
        uint8_t output_status;         // 继电器开关状态(设置的IO输出状态)
        uint8_t relay_output_status;   // 继电器输出状态(回读到的电流状态)

        uint8_t relay_broken_status;     // 继电器故障状态
        uint8_t relay_leakage_status;    // 漏电检测状态
        uint8_t relay_loss_phase_status; // 缺相检测状态
        uint8_t relay_overload_status;   // 过载保护检测状态
        uint8_t relay_ouv_status;        // 过欠压保护检测状态

        uint8_t relay_loss_phase_protect_status; // 缺相保护状态
        uint8_t relay_overload_protect_status;   // 过载保护状态
        uint8_t relay_ouv_protect_status;        // 过欠压保护状态

        uint8_t changed;             // 改状态变，需要上传服务器
        uint32_t status_change_tick; // 改状态变时间
    } app_relay_channel_t, *app_relay_channel_pt;

    int APP_RELAY_init(void);
    int APP_RELAY_set_channel(void);
    int APP_RELAY_send_data_to_server(void);
    int APP_RELAY_read_status_protocol(uint8_t relay_id, g2_server_relay_data_message_pt message, int *changed);
    int APP_RELAY_send_relay_error_to_server(void);
    void APP_RELAY_task_run(void *argument);
    int G2_SERVER_write_relay_power_config_message_process(g2_server_packet_pt packet);
    int APP_RELAY_parse_power_config(uint8_t relay_id, uint8_t channel_id, uint8_t *buffer);
    int G2_SERVER_write_loader_config_message_process(g2_server_packet_pt packet);
    int G2_SERVER_read_loader_config_message_process(g2_server_packet_pt packet);
    int APP_RELAY_parse_load_type(uint8_t *laod_type);

#ifdef __cplusplus
}
#endif
#endif
#endif
