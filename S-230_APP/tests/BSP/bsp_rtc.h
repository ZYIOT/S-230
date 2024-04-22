#ifndef __BSP_RTC_H
#define __BSP_RTC_H
#ifdef __cplusplus
extern "C"
{
#endif
#include "datetime_utils.h"
#include "hardware_wrapper.h"
    void BSP_RTC_init(void);
    int BSP_RTC_set(uint8_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second);
    int BSP_RTC_get(uint8_t *year, uint8_t *month, uint8_t *day, uint8_t *hour, uint8_t *minute, uint8_t *second, uint8_t *week);
    int BSP_RTC_get_datetime(datetime_pt datetime);
#ifdef __cplusplus
}
#endif

#endif
