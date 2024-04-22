#ifndef __BSP_L6_H
#define __BSP_L6_H

#ifdef __cplusplus
extern "C"
{
#endif
#include "hardware_wrapper.h"
#include "adc.h"
	
#define ADC_DMA_num  120		
#define MAX_PUMP_num  3000
#define MIN_PUMP_num  300
#define MAX_SOLENOID_num  3000
#define MIN_SOLENOID_num  300	
#define OILPUMP_ADC_CHANNEL  ADC_CHANNEL_9
#define Solenoid1_ADC_CHANNEL  ADC_CHANNEL_10
#define Solenoid2_ADC_CHANNEL  ADC_CHANNEL_11	
	
extern uint32_t ADC_Value[ADC_DMA_num];

uint16_t  GET_adc_vaul(uint8_t adc_ch,uint8_t ch_num,uint8_t ch_times);
uint16_t Get_Adc_Average(uint32_t ch,uint16_t times)	;
	
#define BSP_L6_init()

#ifdef  SUPPORT_L6_ENABLE
#define BSP_AIRPUMP_off() HARDWARE_GPIO_WRITE_PIN_L(AIRPUMP_CTRL_GPIO_Port, AIRPUMP_CTRL_Pin)
#define BSP_AIRPUMP_on() HARDWARE_GPIO_WRITE_PIN_H(AIRPUMP_CTRL_GPIO_Port, AIRPUMP_CTRL_Pin)
#define BSP_SOLENOID1_off() HARDWARE_GPIO_WRITE_PIN_L(SOLENOID1_CTRL_GPIO_Port, SOLENOID1_CTRL_Pin)
#define BSP_SOLENOID1_on() HARDWARE_GPIO_WRITE_PIN_H(SOLENOID1_CTRL_GPIO_Port, SOLENOID1_CTRL_Pin)
#define BSP_SOLENOID2_off() HARDWARE_GPIO_WRITE_PIN_L(SOLENOID2_CTRL_GPIO_Port, SOLENOID2_CTRL_Pin)
#define BSP_SOLENOID2_on() HARDWARE_GPIO_WRITE_PIN_H(SOLENOID2_CTRL_GPIO_Port, SOLENOID2_CTRL_Pin)
#define BSP_BUZZ_off() HARDWARE_GPIO_WRITE_PIN_L(BUZZ_PWRKEY_GPIO_Port, BUZZ_PWRKEY_Pin)
#define BSP_BUZZ_on() HARDWARE_GPIO_WRITE_PIN_H(BUZZ_PWRKEY_GPIO_Port, BUZZ_PWRKEY_Pin)
	
	
#else
#define BSP_AIRPUMP_on()
#define BSP_AIRPUMP_off()
#define BSP_SOLENOID1_on()
#define BSP_SOLENOID1_off()
#define BSP_SOLENOID2_on()
#define BSP_SOLENOID2_off()
#define BSP_BUZZ_off()
#define BSP_BUZZ_on()

#endif

#ifdef __cplusplus
}
#endif

#endif
