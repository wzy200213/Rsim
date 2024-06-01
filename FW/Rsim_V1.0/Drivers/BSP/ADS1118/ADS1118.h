#ifndef __ADS1118_H
#define __ADS1118_H
#include "./SYSTEM/sys/sys.h"

/*
tips: The SPI_CSK max rate is 4MHz

This ADS ic has two reg, one is data reg and another is config reg
data reg is a read-only reg, the last transfer data is here.
config reg can config many parameters:
bit 15: start consvertion, only useful in power-down mode
bit 14-12: choose input multiplexer 000: AIN_P = AIN0 and AIN_N = AIN1
																		011: AIN_P = AIN2 and AIN_N = AIN3
bit 11-9: These bits configure the programmable gain amplifier. No effect when in temperature sensor mode. 
																		000: +6.144V ~ - 6.144V
bit 8: conversion mode choose. 0: Continuous conversion mode		1: Power-down single-shot mode
bit 7-5: choose data transfer rate. 100: 128SPS
bit 4: choose ADC mode or Temperature sensor mode. 0: ADC mode	1: Temperature sensor mode
bit 3: wheter pull-up DOUT by a 400Kresistor	0: when cs is high, the DOUT is float	1: when cs is high, the DOUT is high
bit 2-1: only 01 can valid the data to config reg
bit 0: I think maybe is not useful!! 0: Data ready, no conversion in progress 1: Data not ready, conversion in progress
*/

// Reg Mode config
// ch0 and ch1 1000_0000_1000_1010
#define ADC_CH0_1													0x8085
// ch2 and ch3 1011_0000_1000_1010
#define ADC_CH2_3													0xB085
// temperature mode 1000_0000_1001_1010
#define ADC_Temp													0x8095

// Pin config
#define ADC_CS_GPIO_PORT                  GPIOA
#define ADC_CS_GPIO_PIN                   GPIO_PIN_11
#define ADC_CS_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)  

// Pin set
#define ADS1118_CS_L											HAL_GPIO_WritePin(ADC_CS_GPIO_PORT, ADC_CS_GPIO_PIN, GPIO_PIN_RESET)
#define ADS1118_CS_H											HAL_GPIO_WritePin(ADC_CS_GPIO_PORT, ADC_CS_GPIO_PIN, GPIO_PIN_SET)


// function
void ADS1118_GPIO_Init(void);
int16_t ADS1118(uint16_t mode);

#endif

