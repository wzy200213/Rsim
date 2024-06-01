#include "./BSP/SPI/spi.h"
#include "./BSP/DAC8550/DAC8550.h"

int16_t ZeroCode_Error = 0;	

void DAC8550_GPIO_Init(void){
	GPIO_InitTypeDef gpio_init_struct;
	
	SYNC_GPIO_CLK_ENABLE();
	gpio_init_struct.Pin = SYNC_GPIO_PIN;                  
  gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;          
  gpio_init_struct.Pull = GPIO_PULLUP;                   
    
  gpio_init_struct.Speed = GPIO_SPEED_FREQ_LOW;          
  HAL_GPIO_Init(SYNC_GPIO_PORT, &gpio_init_struct); 

	DAC8550_SYNC_H;
}

void DAC8550_Write_Reg(uint16_t data)
{
	DAC8550_SYNC_L;
	spi1_read_write_byte(0x00);
	spi1_read_write_byte((data >> 8) & 0x00FF);
	spi1_read_write_byte(data & 0x00FF);
	DAC8550_SYNC_H;
}

void DAC8550_Set_Volatile(int16_t Vol)
{
	int16_t buff;
	int16_t Digital;
	
	buff = Vol + ZeroCode_Error;
	
	if(buff>5000)
			buff=5000;
	else if(buff<-5000)
			buff=-5000;
	
	Digital = buff*65535/10000;
	DAC8550_Write_Reg(Digital);
}

