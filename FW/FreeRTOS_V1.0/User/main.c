#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"

#include "./BSP/LED/led.h"
#include "./BSP/LCD/lcd.h"
#include "./BSP/KEY/key.h"
#include "./BSP/SPI/spi.h"
#include "./BSP/ADC/adc.h"
#include "./BSP/DAC8550/DAC8550.h"

#include "./MALLOC/malloc.h"
#include "freertos_demo.h"

int main(void)
{
    HAL_Init();                         /* 初始化HAL库 */
    sys_stm32_clock_init(RCC_PLL_MUL9); /* 设置时钟, 72Mhz */
    delay_init(72);                     /* 延时初始化 */
    usart_init(115200);                 /* 串口初始化为115200 */
    led_init();                         /* 初始化LED */
    lcd_init();                         /* 初始化LCD */
    key_init();                         /* 初始化按键 */
		adc_init();
		spi1_init();
		DAC8550_GPIO_Init();
    my_mem_init(SRAMIN);                /* 初始化内部SRAM内存池 */
    
    freertos_demo();                    /* 运行FreeRTOS例程 */
}
