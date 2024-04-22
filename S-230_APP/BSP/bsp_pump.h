#ifndef __BSP_PUMP_H
#define __BSP_PUMP_H

#ifdef __cplusplus
extern "C"
{
#endif
#include "hardware_wrapper.h"

#define BSP_PUMP_init()

#ifdef SUPPORT_PUMP
#define BSP_PUMP_on() HARDWARE_GPIO_WRITE_PIN_L(PUMP_PWRKEY_GPIO_Port, PUMP_PWRKEY_Pin)
#define BSP_PUMP_off() HARDWARE_GPIO_WRITE_PIN_H(PUMP_PWRKEY_GPIO_Port, PUMP_PWRKEY_Pin)
#define BSP_PUMP_set(status) HARDWARE_GPIO_WRITE_PIN(PUMP_PWRKEY_GPIO_Port, PUMP_PWRKEY_Pin, status)
#define BSP_PUMP_read_status() HARDWARE_GPIO_READ_PIN(PUMP_CHECK_GPIO_Port, PUMP_CHECK_Pin)
    extern void APP_PUMP_set_check_frequency(uint16_t fq);
#define BSP_PUMP_check_frequency() APP_PUMP_set_check_frequency(0)
#else
#define BSP_PUMP_on()
#define BSP_PUMP_off()
#define BSP_PUMP_set(status)
#define BSP_PUMP_read_status() 1
#define BSP_PUMP_check_frequency()
#endif

#ifdef __cplusplus
}
#endif

#endif
