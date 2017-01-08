#include "ftm.h"
#include "encoder.h"
#include "common.h"

Encoder_Speed_Type LeftSpeed={0,0},RightSpeed={0,0};

void Encoder_Init()
{
	FTM_QD_QuickInit(FTM1_QD_PHA_PA12_PHB_PA13,kFTM_QD_NormalPolarity,kQD_PHABEncoding);
	FTM_QD_QuickInit(FTM2_QD_PHA_PA10_PHB_PA11,kFTM_QD_NormalPolarity,kQD_PHABEncoding);
	FTM_QD_ClearCount(HW_FTM1);
	FTM_QD_ClearCount(HW_FTM2);
}

void Encoder_GetPulse(Encoder_WhichMotor_Type dir)
{
	if(dir==LeftMotor)
	{
		FTM_QD_GetData(HW_FTM1,&(LeftSpeed.value),&(LeftSpeed.direction));
		FTM_QD_ClearCount(HW_FTM1);
		if(LeftSpeed.direction==0 && LeftSpeed.value>0)
		{
			LeftSpeed.value=0xFFFF-LeftSpeed.value;
		}
		else if(LeftSpeed.direction==1 && LeftSpeed.value>0)
		{
			LeftSpeed.value=-LeftSpeed.value;
		}
	}
	else
	{
		FTM_QD_GetData(HW_FTM2,&(RightSpeed.value),&(RightSpeed.direction));
		FTM_QD_ClearCount(HW_FTM2);
		if(RightSpeed.direction==0 && RightSpeed.value>0)
		{
			RightSpeed.value=RightSpeed.value-0xFFFF;
		}
	}
}
