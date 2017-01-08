#ifndef __OELD_H
#define __OELD_H

#include "gpio.h"

#define OLED_SCL  PDout(1)
#define OLED_SDA  PDout(3)
#define OLED_RST  PDout(4)
#define OLED_DC   PDout(2)
#define OLED_CS   PDout(5)

#define XLevelL		0x00
#define XLevelH		0x10
#define XLevel		((XLevelH&0x0F)*16+XLevelL)
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xCF

#define OLED_CMD  0	//Ð´ÃüÁî
#define OLED_DATA 1	//Ð´Êý¾Ý

#define u8 uint8_t
#define u16 uint16_t

 extern u8 lanzhou96x64[768];
 
 void OLED_Init(void);
 void OLED_CLS(void);
 void OLED_P6x8Str(u8 x,u8 y,u8 ch[]);
 void OLED_P8x16Str(u8 x,u8 y,u8 ch[]);
 void OLED_P14x16Str(u8 x,u8 y,u8 ch[]);
 void OLED_Print(u8 x, u8 y, u8 ch[]);
 void OLED_PutPixel(u8 x,u8 y);
 void OLED_Rectangle(u8 x1,u8 y1,u8 x2,u8 y2,u8 gif);
 void OLED_Set_Pos(u8 x, u8 y);
 void OLED_WrDat(u8 data);
void Draw_Pic(void);
 void Draw_BMP(u8 x0,u8 y0,u8 x1,u8 y1,u8 bmp[]);
 void OLED_Fill(u8 dat);
 void Delay_ms(u16 ms);
void Oled_Img(uint8 * img);
void Oled_Ccd(uint8_t base,uint8 * img);
#endif
