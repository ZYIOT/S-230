

#ifndef __COMMON_WATER_INDICATOR_H__
#define __COMMON_WATER_INDICATOR_H__

#ifdef __cplusplus
extern "C"
{
#endif
#include "stdint.h"

#define WATER_INDICATOR_INDEX_TEMPERATURE 0		// 温度 
#define WATER_INDICATOR_INDEX_DO 1				// 溶氧 
#define WATER_INDICATOR_INDEX_PH 2				// PH 
// #define WATER_INDICATOR_INDEX_CONDUCTIVITY 3	// 导电度 ORP 
#define WATER_INDICATOR_INDEX_SALINITY 3		// 盐浓度 
#define WATER_INDICATOR_INDEX_CHLOROPHYLL 4		// 叶绿素 
#define WATER_INDICATOR_INDEX_COD 5				// 化学需氧量 COD 
#define WATER_INDICATOR_INDEX_NH3 6				// 氨含量 
#define WATER_INDICATOR_INDEX_LEVEL    7        // 液位 

    int water_indicator_to_uint16_t(uint8_t index, float f, uint16_t *u);
    int water_indicator_from_uint16_t(uint8_t index, uint16_t u, float *f);
    char *water_indicator_name_by_id(uint8_t id);
#ifdef __cplusplus
}
#endif
#endif
