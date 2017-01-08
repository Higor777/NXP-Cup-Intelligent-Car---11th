#include "common.h"
#include "IR.h"
#include "gpio.h"
#include "oled.h"
#include <stdio.h>
//灯塔亮时为1，灭时为0
volatile uint8_t Beacon_STA=0;
volatile uint8_t beacon_scan=0;

void IR_Init(void)
{
	//1号
	GPIO_QuickInit(HW_GPIOC,5, kGPIO_Mode_IPU);
	GPIO_CallbackInstall(HW_GPIOC, IR_ISR);
	GPIO_ITDMAConfig(HW_GPIOC, 5, kGPIO_IT_FallingEdge, true);
	//2号
	GPIO_QuickInit(HW_GPIOC,6, kGPIO_Mode_IPU);
	GPIO_CallbackInstall(HW_GPIOC, IR_ISR);
	GPIO_ITDMAConfig(HW_GPIOC, 6, kGPIO_IT_FallingEdge, true);
	//3号
	GPIO_QuickInit(HW_GPIOE,10, kGPIO_Mode_IPU);
	GPIO_CallbackInstall(HW_GPIOE, IR_ISR);
	GPIO_ITDMAConfig(HW_GPIOE, 10, kGPIO_IT_FallingEdge, true);
	//4号
	GPIO_QuickInit(HW_GPIOE,12, kGPIO_Mode_IPU);
	GPIO_CallbackInstall(HW_GPIOE, IR_ISR);
	GPIO_ITDMAConfig(HW_GPIOE, 12, kGPIO_IT_FallingEdge, true);
}

void Beacon_Check(void)
{
	//更新Beacon_STA
	if(beacon_scan==0&&Beacon_STA==1)
	{
		Beacon_STA=0;
	}
	else if(beacon_scan==1&&Beacon_STA==0)
	{
		Beacon_STA=1;
	}
	
	beacon_scan=0;
	if(GPIO_ReadBit(HW_GPIOE, 12)&0x01)
	{
		//引脚高电平，检测下降沿
		GPIO_ITDMAConfig(HW_GPIOE, 12, kGPIO_IT_FallingEdge, true);
	}
	else
	{
		//引脚低电平
		beacon_scan=1;
	}
}

#define IR1 5
#define IR2 6
#define IR3 10
#define IR4 12

uint16_t ir1=0,ir2=0,ir3=0,ir4=0;
char s[5];
//PTE,PTC的中断函数都是这，PTC5,PTC6,PTE10,PTE12
static void IR_ISR(uint32_t index)
{
	  //IR1发生中断
    if(index&(0x01<<IR1))
		{
		//	GPIO_ITDMAConfig(HW_GPIOE, 12, kGPIO_IT_FallingEdge, false);
			ir1++;
			beacon_scan=1;
//			sprintf(s,"%-3d",ir1);
//			OLED_Print(80,2,s);
		}
		  //IR1发生中断
    if(index&(0x01<<IR2))
		{
		//	GPIO_ITDMAConfig(HW_GPIOE, 12, kGPIO_IT_FallingEdge, false);
			ir2++;
			beacon_scan=1;
//			sprintf(s,"%-3d",ir2);
//			OLED_Print(90,2,s);
		}
		  //IR1发生中断
    if(index&(0x01<<IR3))
		{
		//	GPIO_ITDMAConfig(HW_GPIOE, 12, kGPIO_IT_FallingEdge, false);
			ir3++;
			beacon_scan=1;
//			sprintf(s,"%-3d",ir3);
//			OLED_Print(100,2,s);
		}
		  //IR1发生中断
    if(index&(0x01<<IR4))
		{
		//	GPIO_ITDMAConfig(HW_GPIOE, 12, kGPIO_IT_FallingEdge, false);
			ir4++;
			beacon_scan=1;
//			sprintf(s,"%-3d",ir4);
//			OLED_Print(110,2,s);
		}
}

