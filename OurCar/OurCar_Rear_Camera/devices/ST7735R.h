#ifndef __ST7735R_H__
#define __ST7735R_H__

#include <stdint.h>
#include "gpio.h"


/*
 * 定义坐标结构体
 */
typedef struct
{
    uint16_t x;
    uint16_t y;
} Site_t;

/*
 * 定义矩形大小结构体
 */
typedef struct
{
    uint16_t W;       //宽
    uint16_t H;       //高
} Size_t;

/***************  LCD初始化  ***************/

void LCD_init(void);        

extern void     LCD_ST7735R_init(void);
extern void     LCD_ST7735R_dir(uint8_t option);
extern void     LCD_ST7735R_ptlon(Site_t site, Size_t size);
extern uint16_t   ST7735R_get_h(void);
extern uint16_t   ST7735R_get_w(void);
extern uint8_t    ST7735R_get_dir(void);


extern void LCD_ST7735R_wr_data(uint8_t data);
extern void LCD_ST7735R_wr_cmd(uint8_t cmd);



/******常用颜色*****/
#define RED     0XF800    //红色
#define GREEN   0X07E0    //绿色
#define BLUE    0X001F    //蓝色
#define BRED    0XF81F
#define GRED    0XFFE0    //灰色
#define GBLUE   0X07FF    //
#define BLACK   0X0000    //黑色
#define WHITE   0XFFFF    //白色
#define YELLOW  0xFFE0    //黄色

#define FCOLOUR         BLUE    //定义字体颜色
#define BCOLOUR         RED     //定义背景颜色

//提取RGB的3元素，rgb565必须是16位
#define RGB565_R(rgb565)                ((uint8_t)(((uint16_t)(rgb565)>>11) &0x1F))
#define RGB565_G(rgb565)                ((uint8_t)(((uint16_t)(rgb565)>> 5) &0x3F))
#define RGB565_B(rgb565)                ((uint8_t)( (uint16_t)(rgb565)      &0x1F))


#define GRAY_2_RGB565(gray)             ((uint16_t)((((uint8_t)(gray)>>3)<<11)|(((uint8_t)(gray)>>2)<<5)|((uint8_t)(gray)>>3)))

#define RGB565_2_GRAY(rgb565)           ((uint8_t)(((RGB565_R(rgb565)*235+RGB565_G(rgb565)*613+RGB565_B(rgb565)*625)+1)>>8))  //  31*235+63*613+31*625+1  = 255*256


#define RGB565_H(rgb565)                ((uint8_t)(((uint16_t)(rgb565))>>8))
#define RGB565_L(rgb565)                (uint8_t)(rgb565))

#define RGB24_RGB565(R,G,B)             ((uint16_t)((((uint8_t)(R)>>3)<<11)|(((uint8_t)(G)>>2)<<5)|((uint8_t)(B)>>3)))
#define RGB555_RGB565(rgb555)           (((((uint16_t)(rgb555)) & (uint16_t)(~0x1F)) << 1) |  (((uint16_t)(rgb555)) & 0x1F))
#define BGR555_RGB565(bgr555)           ((((uint16_t)(bgr555)<<11) & (uint16_t)(0x1F<<11)) | (((uint16_t)(bgr555)) & (uint16_t)(0x1F<<5)) | (((uint16_t)(bgr555)>>10) & (uint16_t)(0x1F)))


//用户配置
#define ST7735R_DIR_DEFAULT     0       //定义默认方向 (0~3)



//定义所用到的 IO管脚
#define LCD_RST    13                //复位

#define ST7735R_H       128
#define ST7735R_W       128


/********************* ST7735R内部实现 ***********************/
//LCD 的管脚定义
#define     LCD_ST7735R_WR      9
#define     LCD_ST7735R_RD      10
#define     LCD_ST7735R_CS      11
#define     LCD_ST7735R_RS      12
#define     LCD_ST7735R_RST     13




#define ST7735R_DELAY()                         //do{asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");}while(0)
#define ST7735R_DELAY_MS(ms)    DelayMs(ms)



#define     LCD_ST7735R_WR_OUT      PCout(LCD_ST7735R_WR)
#define     LCD_ST7735R_RD_OUT      PCout(LCD_ST7735R_RD)
#define     LCD_ST7735R_CS_OUT      PCout(LCD_ST7735R_CS)
#define     LCD_ST7735R_RS_OUT      PCout(LCD_ST7735R_RS)
#define     LCD_ST7735R_RST_OUT     PCout(LCD_ST7735R_RST)

#define LCD_ST7735R_WR_8CMD(cmd)    do\
                            {\
                                LCD_ST7735R_RD_OUT=1;\
                                LCD_ST7735R_RS_OUT=0;\
                                LCD_ST7735R_CS_OUT=0;\
																PCout(0)=(cmd&0x01);\
																PCout(1)=(cmd>>1&0x01);\
																PCout(2)=(cmd>>2&0x01);\
																PCout(3)=(cmd>>3&0x01);\
																PCout(4)=(cmd>>4&0x01);\
																PCout(5)=(cmd>>5&0x01);\
																PCout(6)=(cmd>>6&0x01);\
																PCout(7)=(cmd>>7&0x01);\
                                LCD_ST7735R_WR_OUT=0;\
                                LCD_ST7735R_WR_OUT=1;\
                                LCD_ST7735R_CS_OUT=1;\
                            }while(0)   //LCD_WR=0;LCD_WR=1;产生一个上升沿

#define LCD_ST7735R_WR_8DATA(data)  do\
                            {\
                                LCD_ST7735R_RD_OUT=1;\
                                LCD_ST7735R_RS_OUT=1;\
                                LCD_ST7735R_CS_OUT=0;\
																PCout(0)=(data&0x01);\
																PCout(1)=(data>>1&0x01);\
																PCout(2)=(data>>2&0x01);\
																PCout(3)=(data>>3&0x01);\
																PCout(4)=(data>>4&0x01);\
																PCout(5)=(data>>5&0x01);\
																PCout(6)=(data>>6&0x01);\
																PCout(7)=(data>>7&0x01);\
                                LCD_ST7735R_WR_OUT=0;\
                                LCD_ST7735R_WR_OUT=1;\
                                LCD_ST7735R_CS_OUT=1;\
                            }while(0)   //LCD_WR=0;在这里写入数据到RAM



#define LCD_EN_W        8
#define LCD_EN_H        16


                                                                            //LCD初始化

/***************  LCD绘画  ***************/

extern void LCD_point          (Site_t,                                        uint16_t rgb565);                 //画点
extern void LCD_rectangle      (Site_t, Size_t,                                uint16_t rgb565);                 //画矩形
extern void LCD_char           (Site_t,        uint8_t ascii,                    uint16_t Color, uint16_t bkColor);  //显示8*16字符
extern void LCD_str            (Site_t,        uint8_t *Str,                     uint16_t Color, uint16_t bkColor);  //显示8*16字符串

extern void LCD_num            (Site_t,        uint32_t num,                     uint16_t Color, uint16_t bkColor);  //显示数字
#define MAX_NUM_BIT 5                                                                                   //数字的最大位数（用于清掉多余的屏幕残留数字）
#define LCD_num_C(site,num,color,bkColor)           LCD_num_BC(site, num,MAX_NUM_BIT,color,bkColor)     //显示数字（清掉多余的屏幕残留数字）
extern void LCD_num_BC         (Site_t,        uint32_t num, uint8_t max_num_bit,  uint16_t Color, uint16_t bkColor);  //显示数字（清掉多余的屏幕残留数字）

extern void LCD_Img_gray       (Site_t site, Size_t size, uint8_t *img);                   //显示灰度图像
extern void LCD_Img_gray_Z     (Site_t site, Size_t size, uint8_t *img, Size_t imgsize);   //显示灰度图像(可缩放)

#define BINARY_BGCOLOR  WHITE       //定义二值化图像背景颜色
#define BINARY_COLOR    BLACK       //定义二值化图像前景颜色
extern void LCD_Img_Binary     (Site_t site, Size_t size, uint8_t *img);                   //显示二值化图像
extern void LCD_Img_Binary_Z   (Site_t site, Size_t size, uint8_t *img, Size_t imgsize);   //显示二值化图像(可缩放)

extern void LCD_wave(Site_t site,Size_t size,uint8_t *img,uint8_t maxval,uint16_t Color ,uint16_t bkColor); //波形显示，带背景颜色（maxval 最大值表示超过此值的数都当作最大值处理）
extern void LCD_wave_display(Site_t site,Size_t size,uint8_t *img,uint8_t maxval,uint16_t Color);        //波形显示，不带背景颜色





/*********************** API接口 ******************************/
//提供API接口给LCD调用


#define LCD_PTLON(site,size)    LCD_ST7735R_ptlon(site,size)    //开窗
#define LCD_RAMWR()             LCD_WR_CMD(0x2C)                //写模式
#define LCD_WR_DATA(data)       do{LCD_ST7735R_WR_8DATA((uint8_t)((data)>>8) );LCD_ST7735R_WR_8DATA( (uint8_t)(data) );}while(0)       //写数据
#define LCD_WR_CMD(cmd)         LCD_ST7735R_WR_8CMD(cmd)        //命令


#endif  //__ST7735R_H__


