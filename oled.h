#ifndef __OLED_H
#define __OLED_H	

#include "stdlib.h"	    

#include <bcm2835.h>


#define	OLED_SCLK(a)	if(a)\
			bcm2835_gpio_write(39, HIGH);\
			else\
			bcm2835_gpio_write(39, LOW)\

#define	OLED_SDIN(a)	if(a)\
			bcm2835_gpio_write(38, HIGH);\
			else\
			bcm2835_gpio_write(38, LOW)\

#define	OLED_RS(a)	if(a)\
			bcm2835_gpio_write(37, HIGH);\
			else\
			bcm2835_gpio_write(37, LOW)\

#define	OLED_CS(a)	if(a)\
			bcm2835_gpio_write(36, HIGH);\
			else\
			bcm2835_gpio_write(36, LOW)\


#define OLED_CMD  0
#define OLED_DATA 1

void OLED_WR_Byte(uint8_t dat,uint8_t cmd);	    
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Refresh_Gram(void);  		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(uint8_t x,uint8_t y,uint8_t t);
void OLED_Fill(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t dot);
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t size,uint8_t mode);
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size);
void OLED_ShowString(uint8_t x,uint8_t y,const char *p,uint8_t size);	 
#endif  
	 







 

