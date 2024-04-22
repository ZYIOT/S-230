#ifndef __L6_H
#define __L6_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "gpio.h"
#include "bsp_l6.h"
	
uint16_t  GET_adc_vaul(uint8_t adc_ch,uint8_t ch_num,uint8_t ch_times);

#ifdef __cplusplus
}
#endif

#endif
