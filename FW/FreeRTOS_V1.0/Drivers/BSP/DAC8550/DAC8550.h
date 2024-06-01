#ifndef __DAC8550_H
#define __DAC8550_H
#include "./SYSTEM/sys/sys.h"

// Pin config
#define SYNC_GPIO_PORT                  GPIOA
#define SYNC_GPIO_PIN                   GPIO_PIN_3
#define SYNC_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)  

// Pin set
#define DAC8550_SYNC_L									HAL_GPIO_WritePin(SYNC_GPIO_PORT, SYNC_GPIO_PIN, GPIO_PIN_RESET)
#define DAC8550_SYNC_H									HAL_GPIO_WritePin(SYNC_GPIO_PORT, SYNC_GPIO_PIN, GPIO_PIN_SET)


// function
void DAC8550_GPIO_Init(void);
void DAC8550_Set_Volatile(int16_t Vol);

#endif

