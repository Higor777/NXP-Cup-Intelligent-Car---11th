#include "common.h"
#include "BatteryMon.h"
#include "adc.h"
#include "OLED.h"
#include <stdio.h>

void BatteryMon_Init(void)
{
	ADC_QuickInit(ADC1_SE16, kADC_SingleDiff10or11);
	for(uint8_t i;i<20;i++)
	{
		Show_BatVol();
	}
}

double Get_BatVol(void)
{
	static int32_t pool[20]={0};
	static uint8_t pool_pos=0;
	int32_t value=0;
	
	pool[pool_pos++] = ADC_QuickReadValue(ADC1_SE16);
	pool_pos=pool_pos%20;
	
	for(uint8_t i=0;i<20;i++)
	{
		value+=pool[i];
	}
	value/=20;
	return (double)value/0x3FF*9.9;
}

void Show_BatVol(void)
{
	double  voltage;
	voltage=Get_BatVol();
	
	char str[10];
	sprintf(str,"%.2fV",voltage);
	OLED_Print(81,6,(uint8_t*)str);
}
