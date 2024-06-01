#include "./BSP/SPI/spi.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/ADS1118/ADS1118.h"

void ADS1118_GPIO_Init(void){
	GPIO_InitTypeDef gpio_init_struct;
	
	ADC_CS_GPIO_CLK_ENABLE();
	gpio_init_struct.Pin = ADC_CS_GPIO_PIN;                  
  gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;          
  gpio_init_struct.Pull = GPIO_PULLUP;                   
    
  gpio_init_struct.Speed = GPIO_SPEED_FREQ_LOW;          
  HAL_GPIO_Init(ADC_CS_GPIO_PORT, &gpio_init_struct); 

	ADS1118_CS_H;
}

int16_t ADS1118(uint16_t mode)
{
	uint8_t fd_data[2] = {0x00, 0x00};
	
	ADS1118_CS_L;
	spi1_read_write_byte((mode >> 8) & 0x00FF);
	spi1_read_write_byte(mode & 0x00FF);
	ADS1118_CS_H;
	delay_ms(50);
	ADS1118_CS_L;
	fd_data[0] = spi1_read_write_byte((mode >> 8) & 0x00FF);
	fd_data[1] = spi1_read_write_byte(mode & 0x00FF);
	ADS1118_CS_H;
	
	return (fd_data[0] << 8) | fd_data[1];
}

