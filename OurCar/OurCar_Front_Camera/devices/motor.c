#include "ftm.h"
#include "motor.h"
#include "gpio.h"
#include "common.h"



void Motor_Init(uint32_t freq)
{
	FTM_PWM_QuickInit(FTM0_CH2_PA05, kPWM_EdgeAligned, freq);
	FTM_PWM_QuickInit(FTM0_CH3_PA06, kPWM_EdgeAligned, freq);
	FTM_PWM_QuickInit(FTM0_CH4_PA07, kPWM_EdgeAligned, freq);
	FTM_PWM_QuickInit(FTM0_CH6_PD06, kPWM_EdgeAligned, freq);
	//左电机
  FTM_PWM_ChangeDuty(HW_FTM0,4,0);
	FTM_PWM_ChangeDuty(HW_FTM0,6,0);
	//右电机
	FTM_PWM_ChangeDuty(HW_FTM0,2,0);
	FTM_PWM_ChangeDuty(HW_FTM0,3,0);

}



void Motor_PWM(int32_t Left_PWM,int32_t Right_PWM)
{
	if(Left_PWM>PWM_MAX)
		Left_PWM=PWM_MAX;
	else if(Left_PWM<PWM_MIN)
		Left_PWM=PWM_MIN;
	if(Right_PWM>PWM_MAX)
		Right_PWM=PWM_MAX;
	else if(Right_PWM<PWM_MIN)
		Right_PWM=PWM_MIN;
		
	if(Right_PWM>=0)
	{
		FTM_PWM_ChangeDuty(HW_FTM0,3,Right_PWM);
		FTM_PWM_ChangeDuty(HW_FTM0,2,0);
	}
	else
	{
		FTM_PWM_ChangeDuty(HW_FTM0,3,0);
		FTM_PWM_ChangeDuty(HW_FTM0,2,-Right_PWM);
	}
	if(Left_PWM>=0)
	{
		FTM_PWM_ChangeDuty(HW_FTM0,6,0);
		FTM_PWM_ChangeDuty(HW_FTM0,4,Left_PWM);
	}
	else
	{
		FTM_PWM_ChangeDuty(HW_FTM0,6,-Left_PWM);
		FTM_PWM_ChangeDuty(HW_FTM0,4,0);
	}
}

