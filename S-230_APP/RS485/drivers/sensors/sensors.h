#ifndef __RS485_DRIVER_SENSORS_H
#define __RS485_DRIVER_SENSORS_H
#ifdef __cplusplus
extern "C"
{
#endif
    #include "rs485_sensor.h"
    #include "water_indicator.h"

    // 传感器厂家定义     
    // DO 厂家编号 
    #define RS485_DRIVER_DO_MANUFACTURER_ZYIOT      0x00    // 中易 
    #define RS485_DRIVER_DO_MANUFACTURER_SHLX       0x01    // 上海联祥 
    #define RS485_DRIVER_DO_MANUFACTURER_PROBEST    0x02    // 福州普佩斯 
    #define RS485_DRIVER_DO_MANUFACTURER_SZKLT      0x03    // 深圳科雷特 
    #define RS485_DRIVER_DO_MANUFACTURER_SHYL       0x04    // 上海益伦 
    #define RS485_DRIVER_DO_MANUFACTURER_SZYS       0x05    // 苏州禹山 
    #define RS485_DRIVER_DO_MANUFACTURER_FIVE_PHASE 0x07    // 浙江五相 
    #define RS485_DRIVER_DO_MANUFACTURER_GZLY       0x07    // 广州隆宇 
    #define RS485_DRIVER_DO_MANUFACTURER_XT         0x08    // 仙桃(暂定) 

    // PH 厂家编号 
    #define RS485_DRIVER_PH_MANUFACTURER_ZYIOT      0x00    // 中易
    #define RS485_DRIVER_PH_MANUFACTURER_SHLX       0x01    // 上江联祥 
    #define RS485_DRIVER_PH_MANUFACTURER_SZKLT      0x03    // 深圳科雷特 
    #define RS485_DRIVER_PH_MANUFACTURER_CHEMINS    0x06    // 杭州凯米斯 
    #define RS485_DRIVER_PH_MANUFACTURER_SZSG       0x08    // 苏州松果 
    #define RS485_DRIVER_PH_MANUFACTURER_XT         0x09    // 仙桃(暂定) 

    // ORP 厂家编号 
    #define RS485_DRIVER_ORP_MANUFACTURER_SHLX      0x01    // 上海联祥 
    #define RS485_DRIVER_ORP_MANUFACTURER_WHBW      0x02    // 武汉博文 
    #define RS485_DRIVER_ORP_MANUFACTURER_SZKLT     0x03    // 深圳科雷特 
    #define RS485_DRIVER_ORP_MANUFACTURER_CHEMINS   0x06    // 烟台凯米斯 

    // 叶绿素A 厂家编号 
    #define RS485_DRIVER_CHLOROPHYLL_MANUFACTURER_PROBEST   0x02    // 福州普佩斯 

    // NH3 厂家编号 
    #define RS485_DRIVER_NH3_MANUFACTURER_CHEMINS   0x01    // 烟台凯米斯 
    #define RS485_DRIVER_NH3_MANUFACTURER_PROBEST   0x02    // 福州普佩斯 

    // COD 厂家编号 
    #define RS485_DRIVER_COD_MANUFACTURER_CHEMINS   0x06    // 烟台凯米斯 

    // 液位 厂家编号 
    #define RS485_DRIVER_LEVEL_MANUFACTURER_ANBULEILA   0x06    // 安布雷拉 
    #define RS485_DRIVER_LEVEL_MANUFACTURER_FE          0x09    // 菲尔斯特 

    // 盐度 厂家编号 
    #define RS485_DRIVER_SALINITY_MANUFACTURER_CHEMINS  0x01    // 烟台凯米斯 


    // 传感器设备地址定义 
    #define RS485_DRIVER_SENSOR_ID_DO             1     // DO 
    #define RS485_DRIVER_SENSOR_ID_PH             2     // PH 
    #define RS485_DRIVER_SENSOR_ID_ORP            3     // ORP 
    #define RS485_DRIVER_SENSOR_ID_CONDUCTIVITY   3     // 电导率 
    #define RS485_DRIVER_SENSOR_ID_CHLOROPHYLL    4     // 叶绿素 
    #define RS485_DRIVER_SENSOR_ID_COD            5     // 化学需氧量COD 
    #define RS485_DRIVER_SENSOR_ID_NH3            6     // NH3 
    #define RS485_DRIVER_SENSOR_ID_LEVEL          7     // 液位 
    #define RS485_DRIVER_SENSOR_ID_SALINITY       8     // 盐度 

    #define RS485_DRIVER_SENSOR_MAX_SIZE 20
    // 传感器 model_id 定义 
    #define RS485_SENSOR_MODEL_ID_DOY100          1
    #define RS485_SENSOR_MODEL_ID_PH201           2
    #define RS485_SENSOR_MODEL_ID_DOY121          3
    #define RS485_SENSOR_MODEL_ID_PH230           4
    #define RS485_SENSOR_MODEL_ID_DOY170          5
    #define RS485_SENSOR_MODEL_ID_WL991           6
    #define RS485_SENSOR_MODEL_ID_PH280           7
    #define RS485_SENSOR_MODEL_ID_SAL610          8 
    #define RS485_SENSOR_MODEL_ID_ORP320          9 
    #define RS485_SENSOR_MODEL_ID_ORP620          10 
    #define RS485_SENSOR_MODEL_ID_NHN710          11
    #define RS485_SENSOR_MODEL_ID_COD861          12 
    #define RS485_SENSOR_MODEL_ID_NHN720          13
    #define RS485_SENSOR_MODEL_ID_COD860          14 
    #define RS485_SENSOR_MODEL_ID_PH260           15
    #define RS485_SENSOR_MODEL_ID_DOY123          16
    #define RS485_SENSOR_MODEL_ID_NHN760          17
    #define RS485_SENSOR_MODEL_ID_DOY180          17
    #define RS485_SENSOR_MODEL_ID_DOY181          18
    #define RS485_SENSOR_MODEL_ID_PH291           19 


    // 传感器探头类型 theory 定义
    #define SENSOR_THEORY_OTHER     0   // 其他
    #define SENSOR_THEORY_MEMBRANE  1   // 膜法
    #define SENSOR_THEORY_OPTICS    2   // 荧光法
    #define SENSOR_THEORY_3         3 
    #define SENSOR_THEORY_4         4 
    #define SENSOR_THEORY_5         5 
    #define SENSOR_THEORY_6         6 
    #define SENSOR_THEORY_7         7
    #define SENSOR_THEORY_8         8 
    #define SENSOR_THEORY_9         9

    // 传感器 SN_CODE 定义
    #define SENSOR_SN_CODE_DO100    (3100)
    #define SENSOR_SN_CODE_DOY100   (3102)
    #define SENSOR_SN_CODE_DOY101   (3101)
    #define SENSOR_SN_CODE_DOY102   (3103)
    #define SENSOR_SN_CODE_DO110    (3110)
    #define SENSOR_SN_CODE_DOY120   (3120)
    #define SENSOR_SN_CODE_DOY121   (3121)
    #define SENSOR_SN_CODE_DOY122   (3122)
    #define SENSOR_SN_CODE_DOY123   (3123)
    #define SENSOR_SN_CODE_DOY150   (3150)
    #define SENSOR_SN_CODE_DOY170   (3170)
    #define SENSOR_SN_CODE_DOY180   (3180)
    #define SENSOR_SN_CODE_DOY181   (3181)
    #define SENSOR_SN_CODE_PH200    (3200)
    #define SENSOR_SN_CODE_PH201    (3201)
    #define SENSOR_SN_CODE_PH202    (3202)
    #define SENSOR_SN_CODE_PH230    (3230)
    #define SENSOR_SN_CODE_PH260    (3260)
    #define SENSOR_SN_CODE_PH280    (3280)   
    #define SENSOR_SN_CODE_PH291    (3291)   
    #define SENSOR_SN_CODE_ORP310   (3310)
    #define SENSOR_SN_CODE_ORP320   (3320)
    #define SENSOR_SN_CODE_SAL610   (3610)
    #define SENSOR_SN_CODE_ORP620   (3620)
    #define SENSOR_SN_CODE_NHN710   (3710)
    #define SENSOR_SN_CODE_NHN760   (3760)
    #define SENSOR_SN_CODE_COD861   (3861)
    #define SENSOR_SN_CODE_WL991    (3991)

    #define SENSOR_SN_ID_DEFAULT    (1)
    #define SENSOR_SN_ID_DO         (1)
    #define SENSOR_SN_ID_PH         (2)
    #define SENSOR_SN_ID_ORP        (1)
    #define SENSOR_SN_ID_CONDUCTIVITY   (1)
    #define SENSOR_SN_ID_CHLOROPHYLL    (1)
    #define SENSOR_SN_ID_COD        (1)
    #define SENSOR_SN_ID_NH3        (1)
    #define SENSOR_SN_ID_LEVEL      (1)
    #define SENSOR_SN_ID_SALINITY   (1)

    // ZYIOT 定义格式的 PN 寄存器的地址 
    #define COMMON_PN_REG       (0x14)

    #ifdef ENABLE_RS485_DRIVER_SENSORS_ZYIOT_DOY100
    #include "drivers/sensors/zyiot/doy100.h"
    #endif

    #ifdef ENABLE_RS485_DRIVER_SENSORS_ZYIOT_PH201
    #include "drivers/sensors/zyiot/ph201.h"
    #endif

    #ifdef ENABLE_RS485_DRIVER_SENSORS_PROBEST_DOY121
    #include "drivers/sensors/probest/doy121.h"
    #endif

    #ifdef ENABLE_RS485_DRIVER_SENSORS_SZKLT_PH230
    #include "drivers/sensors/szklt/ph230.h"
    #endif

    #ifdef ENABLE_RS485_DRIVER_SENSORS_FIVE_PHASE_DOY170
    #include "drivers/sensors/five_phase/doy170.h"
    #endif

    #ifdef ENABLE_RS485_DIRVER_SENSOR_ANBULEILA_WL991
    #include "drivers/sensors/anbuleila/wl991.h"
    #endif

    #ifdef ENABLE_RS485_DIRVER_SENSOR_CHEMINS_SAL610
    #include "drivers/sensors/chemins/sal610.h"
    #endif

    #ifdef ENABLE_RS485_DIRVER_SENSOR_ORP320
    #include "drivers/sensors/whbw/orp320.h"
    #endif

    #ifdef ENABLE_RS485_DIRVER_SENSOR_ORP620
    #include "drivers/sensors/chemins/orp620.h"
    #endif

    #ifdef ENABLE_RS485_DIRVER_SENSORS_CHEMINS_PH260
    #include "drivers/sensors/chemins/ph260.h"
    #endif

    #ifdef ENABLE_RS485_DRIVER_SENSOR_DO_DEFAULT
    #include "drivers/sensors/zyiot/do_default.h"
    #endif

    #ifdef ENABLE_RS485_DRIVER_SENSORS_CHEMINS_NHN760
    #include "drivers/sensors/chemins/nhn760.h"
    #endif

    #ifdef ENABLE_RS485_DRIVER_SENSOR_XT_DO 
    #include "drivers/sensors/xiantao/doy181.h"
    #endif

    #ifdef ENABLE_RS485_DRIVER_SENSOR_XT_TMP_PH_NH3
    #include "drivers/sensors/xiantao/ph291.h"
    #endif


    int RS485_DRIVER_init(void);
    int RS485_DRIVER_regsiter(rs485_sensor_driver_pt sensor);
    int RS485_DRIVER_scan(rs485_sensor_pt sensor);
    int RS485_DRIVER_match(rs485_sensor_pt sensor);
    int RS485_DRIVER_match_by_pn(rs485_sensor_pt sensor, char *pn);
    int RS485_DRIVER_match_by_model(rs485_sensor_pt sensor, uint8_t theory);
	int RS485_DRIVER_copy_buffer(uint8_t buffer[RS485_BUFFER_MAX_SIZE], size_t len, void *ret, void *args);

    // int RS485_DRIVER_check_sensor_manual(rs485_sensor_pt rs485, rs485_sensor_driver_pt sd);
#ifdef __cplusplus
}
#endif
#endif
