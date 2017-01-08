#include "common.h"
#include "led.h"
#include "gpio.h"


/*********************************************
* LED初始化
*********************************************/
void LED_Init(void)
{
    //全灭
	GPIO_QuickInit(HW_GPIOB, 20, kGPIO_Mode_OPP);
	GPIO_WriteBit(HW_GPIOB, 20, 1);
	GPIO_QuickInit(HW_GPIOB, 21, kGPIO_Mode_OPP);
	GPIO_WriteBit(HW_GPIOB, 21, 1);
	GPIO_QuickInit(HW_GPIOB, 22, kGPIO_Mode_OPP);
	GPIO_WriteBit(HW_GPIOB, 22, 1);
	GPIO_QuickInit(HW_GPIOB, 23, kGPIO_Mode_OPP);
	GPIO_WriteBit(HW_GPIOB, 23, 1);
}


/*********************************************
* 点亮LED
*********************************************/
void LED_ON(uint8_t led)
{
	switch (led )
		{
			case 0:
				LED1(ON);
				break;
			case 1:
				LED2(ON);
				break;
			case 2:
				LED3(ON);
				break;
			case 3:
				LED4(ON);
				break;
			default :
				break;
	 }
}

/*********************************************
* 熄灭一个LED
*********************************************/
void LED_OFF(uint8_t led)
{
	switch (led )
		{
			case 0:
				LED1(OFF);
				break;
			case 1:
				LED2(OFF);
				break;
			case 2:
				LED3(OFF);
				break;
			case 3:
				LED4(OFF);
				break;
			default :
				break;
	 }
	
}

/*********************************************
* 单个LED亮
*********************************************/
void LED_Single(uint8_t led)
{
	switch (led )
		{
			case 0:
				LED1(ON);
				LED2(OFF);
				LED3(OFF);
				LED4(OFF);
				break;
			case 1:
				LED1(OFF);
				LED2(ON);
				LED3(OFF);
				LED4(OFF);
				break;
			case 2:
				LED1(OFF);
				LED2(OFF);
				LED3(ON);
				LED4(OFF);
				break;
			case 3:
				LED1(OFF);
				LED2(OFF);
				LED3(OFF);
				LED4(ON);
				break;
			default :
				break;
	 }
	
}

