#include <stdint.h>
#include <stdio.h>
#include "oled.h"
#include "adc.h"
#include "battery.h"

char BatteryVolt[]="7.20V";

float Battery_GetVolt()
{
	float volt=ADC_QuickReadValue(ADC0_SE21);
	
//	return (volt*9.9/0xFF);
	return volt;
}

void Battery_num2str(int32_t battery)
{
	BatteryVolt[3]='0'+battery%10;
	battery/=10;
	BatteryVolt[2]='0'+battery%10;
	battery/=10;
	BatteryVolt[0]='0'+battery%10;
}

void Battery_ShowVolt()
{
	static double volt;
	volt=(Battery_GetVolt()+vol)/2;
	sprintf(BatteryVolt,"%.2fV",volt);
	OLED_Print(0,2,(uint8_t*)BatteryVolt);
}
