/* blink.c
 * you can build this with something like:
 * gcc -Wall blink.c -o blink -lbcm2835
 * sudo ./blink
*/

#include <stdio.h>
#include <bcm2835.h>
#include "oled.h"
#include "oledfont.h"

uint8_t OLED_GRAM[129][9];

void OLED_Clear(void)  
{  
	uint8_t i,n;  
	for(i=0;i<8;i++)for(n=0;n<128;n++)OLED_GRAM[n][i]=0X00;  
	OLED_Refresh_Gram();
}
				   
void OLED_DrawPoint(uint8_t x,uint8_t y,uint8_t t)
{
    uint8_t pos,bx,temp=0;
    if(x>129||y>64)	return;
    pos = 7-y/8;
    bx = y%8;
    temp = 1<<(7-bx);
    if(t)	OLED_GRAM[x][pos] |= temp;
    else 	OLED_GRAM[x][pos] &= ~temp;	
}

void OLED_Refresh_Gram(void)
{
    uint8_t i,n;		    
    for(i=0;i<=8;i++)  
    {
	OLED_WR_Byte (0xb0+i,OLED_CMD);
	OLED_WR_Byte (0x02,OLED_CMD);
	OLED_WR_Byte (0x10,OLED_CMD);   
	for(n=0;n<=128;n++)OLED_WR_Byte(OLED_GRAM[n][i],OLED_DATA);
    }   
}

void OLED_WR_Byte(uint8_t dat,uint8_t cmd)
{
    uint8_t i;			  
    OLED_RS(cmd);
    OLED_CS(0);		  
    for(i=0;i<8;i++)
    {
	OLED_SCLK(0);
	if(dat&0x80)OLED_SDIN(1);
	else OLED_SDIN(0);
	OLED_SCLK(1);
	dat<<=1;
    }			 
    OLED_CS(1);	  
    OLED_RS(1);
}

void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t size,uint8_t mode)
{      			    
    uint8_t temp,t,t1;
    uint8_t y0=y;
    uint8_t csize=(size/8+((size%8)?1:0))*(size/2);
    chr=chr-' ';		 
    for(t=0;t<csize;t++)
    {
	if(size==12)temp=asc2_1206[chr][t];
	else if(size==16)temp=asc2_1608[chr][t];
	else if(size==24)temp=asc2_2412[chr][t];
	else return;
	for(t1=0;t1<8;t1++)
	{
	    if(temp&0x80)OLED_DrawPoint(x,y,mode);
	    else OLED_DrawPoint(x,y,!mode);
	    temp<<=1;
	    y++;
	    if((y-y0)==size)
	    {
		y=y0;
		x++;
		break;
	    }
	}
    }
}

void OLED_ShowString(uint8_t x,uint8_t y,const char *p,uint8_t size)
{	
    while((*p<='~')&&(*p>=' '))
    {       
        if(x>(128-(size/2))){x=0;y+=size;}
        if(y>(64-size)){y=x=0;OLED_Clear();}
        OLED_ShowChar(x,y,*p,size,1);	 
        x+=size/2;
        p++;
    }  
	
}

void OLED_Init()
{
    bcm2835_gpio_fsel(36, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(37, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(38, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(39, BCM2835_GPIO_FSEL_OUTP);
    
    OLED_SCLK(1);
    OLED_SDIN(1);
    OLED_RS(1);
    OLED_CS(1);
    	  
    OLED_WR_Byte(0xAE,OLED_CMD); //¹Ø±ÕÏÔÊ¾
    OLED_WR_Byte(0xD5,OLED_CMD); //ÉèÖÃÊ±ÖÓ·ÖÆµÒò×Ó,Õðµ´ÆµÂÊ
    OLED_WR_Byte(80,OLED_CMD);   //[3:0],·ÖÆµÒò×Ó;[7:4],Õðµ´ÆµÂÊ
    OLED_WR_Byte(0xA8,OLED_CMD); //ÉèÖÃÇý¶¯Â·Êý
    OLED_WR_Byte(0X3F,OLED_CMD); //Ä¬ÈÏ0X3F(1/64) 
    OLED_WR_Byte(0xD3,OLED_CMD); //ÉèÖÃÏÔÊ¾Æ«ÒÆ
    OLED_WR_Byte(0X00,OLED_CMD); //Ä¬ÈÏÎª0

    OLED_WR_Byte(0x40,OLED_CMD); //ÉèÖÃÏÔÊ¾¿ªÊ¼ÐÐ [5:0],ÐÐÊý.
    					
    OLED_WR_Byte(0x8D,OLED_CMD); //µçºÉ±ÃÉèÖÃ
    OLED_WR_Byte(0x14,OLED_CMD); //bit2£¬¿ªÆô/¹Ø±Õ
    OLED_WR_Byte(0x20,OLED_CMD); //ÉèÖÃÄÚ´æµØÖ·Ä£Ê½
    OLED_WR_Byte(0x02,OLED_CMD); //[1:0],00£¬ÁÐµØÖ·Ä£Ê½;01£¬ÐÐµØÖ·Ä£Ê½;10,Ò³µØÖ·Ä£Ê½;Ä¬ÈÏ10;
    OLED_WR_Byte(0xA1,OLED_CMD); //¶ÎÖØ¶¨ÒåÉèÖÃ,bit0:0,0->0;1,0->127;
    OLED_WR_Byte(0xC0,OLED_CMD); //ÉèÖÃCOMÉ¨Ãè·½Ïò;bit3:0,ÆÕÍ¨Ä£Ê½;1,ÖØ¶¨ÒåÄ£Ê½ COM[N-1]->COM0;N:Çý¶¯Â·Êý
    OLED_WR_Byte(0xDA,OLED_CMD); //ÉèÖÃCOMÓ²¼þÒý½ÅÅäÖÃ
    OLED_WR_Byte(0x12,OLED_CMD); //[5:4]ÅäÖÃ
    
    OLED_WR_Byte(0x81,OLED_CMD); //¶Ô±È¶ÈÉèÖÃ
    OLED_WR_Byte(0xEF,OLED_CMD); //1~255;Ä¬ÈÏ0X7F (ÁÁ¶ÈÉèÖÃ,Ô½´óÔ½ÁÁ)
    OLED_WR_Byte(0xD9,OLED_CMD); //ÉèÖÃÔ¤³äµçÖÜÆÚ
    OLED_WR_Byte(0xf1,OLED_CMD); //[3:0],PHASE 1;[7:4],PHASE 2;
    OLED_WR_Byte(0xDB,OLED_CMD); //ÉèÖÃVCOMH µçÑ¹±¶ÂÊ
    OLED_WR_Byte(0x30,OLED_CMD); //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;

    OLED_WR_Byte(0xA4,OLED_CMD); //È«¾ÖÏÔÊ¾¿ªÆô;bit0:1,¿ªÆô;0,¹Ø±Õ;(°×ÆÁ/ºÚÆÁ)
    OLED_WR_Byte(0xA6,OLED_CMD); //ÉèÖÃÏÔÊ¾·½Ê½;bit0:1,·´ÏàÏÔÊ¾;0,Õý³£ÏÔÊ¾	    						   
    OLED_WR_Byte(0xAF,OLED_CMD); //¿ªÆôÏÔÊ¾	 
	
    OLED_Clear();
}

int main(int argc, char **argv)
{
    if (!bcm2835_init())
    {
	printf("bcm2835_init failed. Are you running as root??\r\n");
	return 1;
    }
    OLED_Init();
    printf("OLED_init\r\n");
    
    OLED_ShowString(0,0,"ZIGBEE TX2",16);
    OLED_Refresh_Gram();
    printf("Run\r\n");
    
    while (1)
    {
	bcm2835_delay(1000);
    }
    bcm2835_close();
    return 0;
}
