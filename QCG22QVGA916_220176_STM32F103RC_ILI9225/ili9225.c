#include <stm32f10x.h>
#include "delay.h"
#include "ili9225.h"

#define LCD_WIDTH  (220) 
#define LCD_HEIGHT (176) 

#define CS_CLR  GPIO_ResetBits(GPIOA, GPIO_Pin_0)
#define CS_SET  GPIO_SetBits(GPIOA, GPIO_Pin_0)

#define DC_CLR  GPIO_ResetBits(GPIOA, GPIO_Pin_1)
#define DC_SET  GPIO_SetBits(GPIOA, GPIO_Pin_1)

#define RD_CLR  GPIO_ResetBits(GPIOA, GPIO_Pin_2)
#define RD_SET  GPIO_SetBits(GPIOA, GPIO_Pin_2)

#define WR_CLR  GPIO_ResetBits(GPIOA, GPIO_Pin_3)
#define WR_SET  GPIO_SetBits(GPIOA, GPIO_Pin_3)

#define RST_CLR  GPIO_ResetBits(GPIOA, GPIO_Pin_4)
#define RST_SET  GPIO_SetBits(GPIOA, GPIO_Pin_4)

//void lcd_send_cmd(unsigned int regid)
//{
//	CS_CLR;
//	DC_CLR;
//	RD_SET;
//	
//	WR_CLR;
//	//high 8bit transfer to I0-I7
//	GPIOB->ODR= (GPIOB->ODR & 0x00FF) | (regid & 0xFF00);
//	WR_SET;
//    
//    WR_CLR;
//	GPIOB->ODR= (GPIOB->ODR & 0x00FF) | (regid << 8 & 0xFF00);
//	WR_SET;
//}

void lcd_send_cmd(unsigned int regid)
{
	CS_CLR;
	DC_CLR;
	RD_SET;
	
	WR_CLR;
	GPIOB->ODR= (regid & 0xFFFF);
	WR_SET;
    
}

//void lcd_send_data(unsigned int dat)
//{
//	CS_CLR;
//	DC_SET;
//	RD_SET;
//    
//	WR_CLR;
//    GPIOB->ODR= (GPIOB->ODR & 0x00FF) | (dat & 0xFF00);
//	WR_SET;
//    
//    WR_CLR;
//    GPIOB->ODR= (GPIOB->ODR & 0x00FF) | (dat << 8 & 0xFF00);
//	WR_SET;
//}

void lcd_send_data(unsigned int dat)
{
	CS_CLR;
	DC_SET;
	RD_SET;
    
	WR_CLR;
    GPIOB->ODR= (dat & 0xFFFF);
	WR_SET;

}

void lcd_set_register_data(unsigned int regid, unsigned int dat)
{
	lcd_send_cmd(regid); 
    lcd_send_data(dat);
}

void lcd_reset(void)
{
	RST_SET;
	RST_CLR;	//keep low level to perform reset
	delay_ms(30);
	RST_SET;
	delay_ms(120);
}

void lcd_init_registers(void)
{
	
//************* Start Initial Sequence **********// 
	lcd_set_register_data(0x00D0, 0x0003);
	lcd_set_register_data(0x00EB, 0x0B00);
	lcd_set_register_data(0x00EC, 0x004F); //0f
	lcd_set_register_data(0x00C7, 0x003F); //030f

	lcd_set_register_data(0x0001, 0x011C);
	lcd_set_register_data(0x0002, 0x0100); //0100
	lcd_set_register_data(0x0003, 0x1030);
	lcd_set_register_data(0x0007, 0x0000);
	lcd_set_register_data(0x0008, 0x0808);
	lcd_set_register_data(0x000F, 0x0A01);
	delay_ms(10);

	lcd_set_register_data(0x0010, 0x0000);
	lcd_set_register_data(0x0011, 0x1B41); //The register setting is suitable for VCI=2.8V
	delay_ms(120);

	lcd_set_register_data(0x0012, 0x300E); //The register setting is suitable for VCI=2.8V
	lcd_set_register_data(0x0013, 0x0061); //The register setting is suitable for VCI=2.8V
	lcd_set_register_data(0x0014, 0x5569); //The register51etting is suitable for VCI=2.8V   5569  4e65  00-7F65  vcomh
//-------------- Set GRAM area -----------------//
	lcd_set_register_data(0x0030, 0x0000);
	lcd_set_register_data(0x0031, 0x00DB);
	lcd_set_register_data(0x0032, 0x0000);
	lcd_set_register_data(0x0033, 0x0000);
	lcd_set_register_data(0x0034, 0x00DB);
	lcd_set_register_data(0x0035, 0x0000);
	lcd_set_register_data(0x0036, 0x00AF);
	lcd_set_register_data(0x0037, 0x0000);
	lcd_set_register_data(0x0038, 0x00DB);
	lcd_set_register_data(0x0039, 0x0000);
// ----------- Adjust the Gamma Curve ----------//     
	lcd_set_register_data(0x0050, 0x0100);
	lcd_set_register_data(0x0051, 0x0609);
	lcd_set_register_data(0x0052, 0x0c09);
	lcd_set_register_data(0x0053, 0x0205);
	lcd_set_register_data(0x0054, 0x090c);
	lcd_set_register_data(0x0055, 0x0906);
	lcd_set_register_data(0x0056, 0x0001);
	lcd_set_register_data(0x0057, 0x0502);
	lcd_set_register_data(0x0058, 0x0003);
	lcd_set_register_data(0x0059, 0x0300);
	lcd_set_register_data(0x0020, 0x0000);
	lcd_set_register_data(0x0021, 0x0000);
	lcd_set_register_data(0x0007, 0x1017);

}

void lcd_gpio_config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
   
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
	GPIO_InitStructure.GPIO_Pin = ((uint16_t)0xFFFF);
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void lcd_init(void)
{
    lcd_gpio_config();
    lcd_reset();
	lcd_init_registers();
}


void lcd_open_window(int x, int y, int cx, int cy)
{
	lcd_set_register_data(0x39, x & 0xFF);
	lcd_set_register_data(0x38, cx & 0xFF);
		
	lcd_set_register_data(0x37, y & 0xFF);
	lcd_set_register_data(0x36, cy & 0xFF);

	lcd_set_register_data(0x21, x & 0xFF);
	lcd_set_register_data(0x20, y & 0xFF);
    
    lcd_send_cmd(0x22);
}

void lcd_fill_rect(int x, int y, int cx, int cy, unsigned int clr)
{
	uint32_t n, temp;
	lcd_open_window(x, y, cx, cy);
	temp = (uint32_t)cx*cy;
	for(n=0; n<temp; n++)
	{
		//lcd_send_data( clr>>8 );
		lcd_send_data( clr );
	}
}

void lcd_display_gray_bar(void)
{
    static unsigned int gray[]=
    {
        RGB(0x00, 0x00, 0x00),
        RGB(0x10, 0x10, 0x10),
        RGB(0x20, 0x20, 0x20),
        RGB(0x30, 0x30, 0x30),
        RGB(0x40, 0x40, 0x40),
        RGB(0x50, 0x50, 0x50),
        RGB(0x60, 0x60, 0x60),
        RGB(0x70, 0x70, 0x70),
        RGB(0x80, 0x80, 0x80),
        RGB(0x90, 0x90, 0x90),
        RGB(0xA0, 0xA0, 0xA0),
        RGB(0xB0, 0xB0, 0xB0),
        RGB(0xC0, 0xC0, 0xC0),
        RGB(0xD0, 0xD0, 0xD0),
        RGB(0xE0, 0xE0, 0xE0),
        RGB(0xF0, 0xF0, 0xF0),
    };
	int i=0,j;
	uint8_t step;
	step = LCD_WIDTH / 16;
	lcd_open_window(0, 0, LCD_WIDTH, LCD_HEIGHT); 
	
	CS_CLR;
	DC_SET;
	RD_SET;
	
	for(j=0; j<LCD_WIDTH; j++)
	{
		for(i=0; i< LCD_HEIGHT; i++)
		{
//			WR_CLR;
//			GPIOB->ODR= (GPIOB->ODR & 0x00FF) | (gray[j/step]  & 0xFF00);
//			WR_SET;
//	
//			WR_CLR;
//			GPIOB->ODR= (GPIOB->ODR & 0x00FF) | (gray[j/step] << 8 & 0xFF00);
//			WR_SET;
            
            WR_CLR;
			GPIOB->ODR=  (gray[j/step%16] & 0xFFFF);
			WR_SET;
		}
	}
	
	CS_SET;	
}


void lcd_display_color_bar(void)
{
    static unsigned int colorbar[]=
    {
        RGB(0xFF, 0xFF, 0x00),
        RGB(0xFF, 0x00, 0x00),
        RGB(0x00, 0xFF, 0x00),
        RGB(0x00, 0x00, 0xFF),

        RGB(0xFF, 0x00, 0xFF),
        RGB(0x00, 0xFF, 0xFF),
        RGB(0x00, 0x00, 0x00),		//there can not be 0x00 or 0xFF
        RGB(0xFF, 0xFF, 0xFF),

    };
	int i=0, j/*, index=0*/;
	uint8_t step;

	step = LCD_WIDTH / 8;
	lcd_open_window(0, 0, LCD_WIDTH, LCD_HEIGHT); 
	
	CS_CLR;
	DC_SET;
	RD_SET;

	for(j=0; j<LCD_WIDTH; j++)
	{
		for(i=0; i< LCD_HEIGHT; i++)
		{
//			WR_CLR;
//			GPIOB->ODR= (GPIOB->ODR & 0x00FF) | (colorbar[j/step]  & 0xFF00);
//			WR_SET;
//	
//			WR_CLR;
//			GPIOB->ODR= (GPIOB->ODR & 0x00FF) | (colorbar[j/step] << 8 & 0xFF00);
//			WR_SET;
            
            WR_CLR;
			GPIOB->ODR= (colorbar[j/step] & 0xFFFF);
			WR_SET;
		}
	}
	
	CS_SET;	
}
