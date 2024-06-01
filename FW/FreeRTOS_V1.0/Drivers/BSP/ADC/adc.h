#ifndef __ADC_H
#define __ADC_H

#include "./SYSTEM/sys/sys.h"

// ADC引脚及定义
#define ADC_ADCX_CHY_GPIO_PORT					GPIOC
#define ADC_ADCX_CHY_GPIO_PIN						GPIO_PIN_4
#define ADC_ADCX_CHY_GPIO_CLK_ENABLE()	do{__HAL_RCC_GPIOC_CLK_ENABLE();}while(0)

#define ADC_ADCX												ADC1
#define ADC_ADCX_CHY										ADC_CHANNEL_14
#define ADC_ADCX_CHY_CLK_ENABLE()				do{__HAL_RCC_ADC1_CLK_ENABLE();}while(0)


// function 
void adc_init(void);
void adc_channel_set(ADC_HandleTypeDef *adc_handle, uint32_t ch,uint32_t rank, uint32_t stime); 

uint32_t adc_get_result_average(uint32_t ch, uint8_t times); 

#endif

