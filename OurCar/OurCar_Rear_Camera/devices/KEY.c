#include "common.h"
#include "KEY.h"
#include "gpio.h"

volatile uint8_t KEY_Scan_STA=0;
//KEY_Scan_STA含义：
//Bit		  7		  6	  	5		  4		  3		  2	  	1		  0
//Key		更新标识	NC		NC		NC	KEY4	KEY3	KEY2	KEY1
//1表示有效，0表示无效

volatile uint8_t M_Encoder_STA=0;
//M_Encoder_STA含义：
//Bit	   	7		  6		  5	  	4	  	3		  2	  	1	   	0
//Key		更新标识	NC		NC		NC		NC		NC		CCW		CW
//1表示有效，0表示无效


/*********************************************
* 按键IO扫描
*********************************************/
void Key_Scan(void)
{
	static uint8_t keysta_old=0;
	uint8_t keysta_new=0x00;
	keysta_new=keysta_new<<1;
	keysta_new|=(~Key_4)&0x01;
	keysta_new=keysta_new<<1;
	keysta_new|=(~Key_3)&0x01;
	keysta_new=keysta_new<<1;
	keysta_new|=(~Key_2)&0x01;
	keysta_new=keysta_new<<1;
	keysta_new|=(~Key_1)&0x01;
	
#if USE_PIT_KEYSCAN==0
	
	keysta_old=keysta_new;
	keysta_new=0x00;
	
	DelayMs(10);	//消除抖动
	

	keysta_new|=(~Key_4)&0x01;
	keysta_new=keysta_new<<1;
	keysta_new|=(~Key_3)&0x01;
	keysta_new=keysta_new<<1;
	keysta_new|=(~Key_2)&0x01;
	keysta_new=keysta_new<<1;
	keysta_new|=(~Key_1)&0x01;
	
#endif
	

	KEY_Scan_STA=keysta_new&keysta_old;
	
	KEY_Scan_STA|=KEY_Scan_STA_Update_MASK;		//添加标识位
	
	keysta_old=keysta_new;
}


/*********************************************
* 按键IO初始化
*********************************************/
void Key_Init(void)
{
	GPIO_QuickInit(HW_GPIOD,10, kGPIO_Mode_IFT);
	GPIO_QuickInit(HW_GPIOD,11, kGPIO_Mode_IFT);
	GPIO_QuickInit(HW_GPIOD,8, kGPIO_Mode_IFT);
	GPIO_QuickInit(HW_GPIOD,9, kGPIO_Mode_IFT);

	Key_Scan();	//初始化KeyUp_old,KeyDown_old
	
#if USE_PIT_KEYSCAN!=0
	DelayMs(10);
	Key_Scan();
#endif
}



/******************************************************
*鼠标编码器扫描
*要每1~5ms运行一次,编码器质量好的运行间隔时间可更短
******************************************************/
void M_Encoder_Scan(void)
{
	//ab_sta为编码器AB相每一次扫描的状态记录变量
	//bit0为A相，bit1为B相
	static uint8_t ab_sta_old=0;
	//ab_scansta_old为编码器AB相稳定状态记录变量
	//bit0为A相，bit1为B相
	static uint8_t ab_scansta_old=0;
	
	uint8_t ab_sta_new=0;
	uint8_t ab_scansta_new=0;
	
	ab_sta_new|=M_Encoder_A&0x01;
	ab_sta_new=ab_sta_new<<1;
	ab_sta_new|=M_Encoder_B&0x01;
	
	//更新编码器AB相状态
	ab_scansta_new=(((ab_sta_new^ab_sta_old)&ab_scansta_old)|((~(ab_sta_new^ab_sta_old))&ab_sta_old))&0x03;
	ab_sta_old=ab_sta_new;
	
	if((ab_scansta_new^ab_scansta_old)&M_Encoder_A_MASK)
	{
		//编码器A相状态改变
		if(((ab_scansta_old&M_Encoder_A_MASK)>>M_Encoder_A_SHIFT)==((ab_scansta_old&M_Encoder_B_MASK)>>M_Encoder_B_SHIFT))
		{
			M_Encoder_STA|=M_Encoder_CW_MASK;
		}
		else
		{
			M_Encoder_STA|=M_Encoder_CCW_MASK;
		}
		M_Encoder_STA|=M_Encoder_STA_Update_MASK;	//添加标识位
	}
	ab_scansta_old=ab_scansta_new;
}

void M_Encoder_Init(void)
{
	GPIO_QuickInit(HW_GPIOB,18, kGPIO_Mode_IFT);
	GPIO_QuickInit(HW_GPIOB,16, kGPIO_Mode_IFT);
	
	for(uint8_t i=0; i<4; i++)
	{
		M_Encoder_Scan();
		DelayMs(5);
	}
	M_Encoder_STA=0;
}



/******************************************************
*按键状态
*这个函数用于实现多按键连按输出
******************************************************/
uint8_t Key_STA(void)
{
	static uint8_t keysta_mask=0xFF;
	static uint8_t keysta_mask_old=0xFF;
	static uint8_t keysta_cnt=0;
	
	uint8_t keysta;
//	uint8_t keysta_out;
	
#if USE_PIT_KEYSCAN==0
	
	Key_Scan();
	
#endif
	
	if(KEY_Scan_STA&0x80)
	{
		keysta=KEY_Scan_STA&0x3F;
		KEY_Scan_STA=0;
	}
	else 
		return 0;
	
	
	//解除MASK
	keysta_mask|=~keysta;
	keysta_mask_old|=~keysta;
	
	//cnt清零
 	if(keysta_mask&keysta)
	{
		keysta_cnt=0;
		keysta_mask_old=keysta_mask;
	}
	
	//输出keysta
	if(keysta_cnt>=Key_KeepTime)
	{
		keysta&=keysta_mask_old;
		keysta_cnt-=Key_KeepStep;
	}
	else
	{
		keysta&=keysta_mask;
		keysta_cnt++;
	}
	
	//设定MASK
	keysta_mask&=~keysta;
	
	return keysta;
}
