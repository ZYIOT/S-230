#ifndef __RS485_L6_H
#define __RS485_L6_H
#ifdef __cplusplus
extern "C"
{
#endif
#include "rs485.h"

#define RS485_L6_ADDRESS 0xFB
#ifdef L6_DEBUG_TEST
#define Saturability_max_capacity	 3
#else
#define Saturability_max_capacity	 144
#endif
    typedef struct
    {
      uint16_t device_status; // 设备状态 
      uint16_t data_status;   // 获取数据状态 
      uint16_t bump_status;   // 气泵状态 
      uint16_t solenoid_status;      // 电磁阀状态 
      uint16_t liquid_level_status;  // 液位传感器状态 
      uint16_t release;       // 泄气时间 
      uint16_t readWait;     // 入水后多长时间允许读取数据（传感器在水中等待时长）
      uint16_t read;          // 读取数据持续时间 
      uint16_t charge;        // 充气时间 
      uint16_t chargeWait;   // 气吹自动等待时长(传感器在空气中时长) 
      int status;             // 数据查询的状态 
      int error_code;         // 485 通信的错误状态 
      int initialized;
      uint16_t errorCount; // 连续读取错误次数 
      uint16_t reset_count; // 设备重启次数 
    
      uint16_t force_status;// 强制读数状态 
      uint16_t send_status; // 容许上传服务器状态 
    
      uint8_t release_status; // 泄气状态 
      uint8_t charge_status;  // 充气状态 
      uint8_t release_start;  // 泄气开始 
      uint8_t charge_start;   // 充气开始 
      
      uint8_t start_status;   // 开启状态 
      
      uint8_t saturability_status;      // 可以检查空气中的 DO 传感器的饱和度值 
      uint8_t saturability_alet_status; // DO 传感器表面附着告警状态 
      uint16_t saturability_data;       // 空气中的 DO 传感器的饱和度值 
      uint16_t saturability_count;      // 空气中 DO 传感器的饱和度值异常次数 
      uint16_t saturability[Saturability_max_capacity]; // 饱和度数据 
      uint8_t air_alet_status;  // DO 传感器长时间处于空气中告警状态 
    
    } rs485_l6_status_t, *rs485_l6_status_pt;

    int RS485_L6_start(rs485_port_pt rs485);
    int RS485_L6_read(rs485_port_pt rs485, rs485_l6_status_pt l6_status);
    int RS485_L6_set_register(rs485_port_pt rs485, uint16_t release, uint16_t readWait, uint16_t read, uint16_t charge, uint16_t chargeWait);

#ifdef __cplusplus
}
#endif
#endif
