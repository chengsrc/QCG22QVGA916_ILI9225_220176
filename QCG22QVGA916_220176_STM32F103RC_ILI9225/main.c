#include <stm32f10x.h>
#include "delay.h"
#include <stdio.h>
#include "ili9225.h"

int main(void)
{
	delay_init(72);	//when overclock, max delay_ms is 1048.57
	lcd_init();

	
    while (1)
	{
		lcd_display_color_bar();
		delay_ms(1000);
		lcd_display_gray_bar();
		delay_ms(1000);
		lcd_fill_rect(0, 0, 220, 176, RGB(255, 0, 0));
		delay_ms(1000);
		lcd_fill_rect(0, 0, 220, 176, RGB(0, 255, 0));
		delay_ms(1000);
		lcd_fill_rect(0, 0, 220, 176, RGB(0, 0, 255));
		delay_ms(1000);
	}
}

#if 0
/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in milliseconds.
  * @retval None
  */
void Delay(__IO uint32_t nTime)
{ 
  uwTimingDelay = nTime;

  while(uwTimingDelay != 0);
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void)
{
  if (uwTimingDelay != 0x00)
  { 
    uwTimingDelay--;
  }
}
#endif
