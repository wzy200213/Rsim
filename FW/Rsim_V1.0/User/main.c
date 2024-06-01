#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"

#include "./BSP/LED/led.h"
#include "./BSP/LCD/lcd.h"
#include "./BSP/ADC/adc.h"
#include "./BSP/SPI/spi.h"

#include "./BSP/DAC8550/DAC8550.h"
#include "./BSP/ADS1118/ADS1118.h"


int main(void)
{
	int16_t test;
	uint32_t adcx;
	
  HAL_Init();                                         /* HAL库初始化 */
  sys_stm32_clock_init(RCC_PLL_MUL9);                 /* 设置时钟, 72Mhz */
  delay_init(72);                                     /* 延时初始化 */
  usart_init(115200);                                 /* 串口初始化为115200 */
    
	led_init();                                         /* 初始化LED */
  lcd_init();                                         /* 初始化LCD */
	adc_init();
	
	
	ADS1118_GPIO_Init();	
	DAC8550_GPIO_Init();
	DAC8550_Set_Volatile(3000);
	
	
	lcd_clear(WHITE);
	lcd_show_string(10, 40, 240, 32, 32, "STM32 HAL", RED);
  lcd_show_string(10, 80, 240, 24, 24, "Rsim V1.0", RED);
  lcd_show_string(10, 110, 240, 16, 16, "Rsim @ZeYu Wang", BLUE);
	LED0(0);
	
	lcd_show_string(10, 200, 240, 16, 16, "Rsim @test bench", BLACK);
	
  while (1)
  {
    
       
		adcx = adc_get_result_average(ADC_ADCX_CHY, 10);    /* 获取ADC通道的转换值，10次取平均 */
    lcd_show_xnum(10, 140, adcx, 5, 16, 0, BLUE);      /* 显示ADC采样后的原始值 */
		
		test = ADS1118(ADC_CH0_1);
		DAC8550_Set_Volatile(-5000);
		
		LED1_TOGGLE(); 
		delay_ms(1000);
  }
}
