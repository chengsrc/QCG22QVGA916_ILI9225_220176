#include <stm32f10x.h>

#ifdef RGB16BIT
//#define RGB24TORGB16(R,G,B) ((unsigned short int)((((R)>>3)<<11) | (((G)>>2)<<5)	| ((B)>>3)	))
#define RGB(R,G,B) ((unsigned short int)((((R)>>3)<<11) | (((G)>>2)<<5)	| ((B)>>3)	))
#else
//#define RGB24TORGB16(R,G,B) ((unsigned int)((((R))<<16) | (((G))<<8)	| ((B))	))
#define RGB(r,g,b)          ((uint32_t)(((uint8_t)(b)|((uint16_t)((uint8_t)(g))<<8))|(((uint32_t)(uint8_t)(r))<<16)))
#endif

void lcd_send_data(unsigned int dat);
void lcd_send_cmd(unsigned int regid);
void lcd_init(void);
void lcd_display_color_bar(void);
void lcd_display_gray_bar(void);
void lcd_fill_rect(int x, int y, int cx, int cy, unsigned int clr);
