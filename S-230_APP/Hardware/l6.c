#include "l6.h"
#include "utils.h"
#include "bsp_rs485.h"
#include "adc.h"
#include "rs485_l6.h"
#include "bsp_l6.h"

uint32_t ADC_Value[ADC_DMA_num];

//获取指定通道的转换值，取times次,然后平均 
//times:获取次数
//返回值:通道ch的times次转换结果平均值
uint16_t Get_Adc_Average(uint32_t ch,uint16_t times)
{
	uint32_t temp_val=0;
	uint16_t t;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc(ch);
		HARDWARE_OS_DELAY_MS(5);
	}
	return temp_val/times;
} 
