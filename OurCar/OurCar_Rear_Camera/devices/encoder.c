#include "ftm.h"
#include "encoder.h"
#include "common.h"

Encoder_Speed_Type LeftSpeed={0,0,0},RightSpeed={0,0,0};

int abs_int(int dat)
{
	return dat>0?dat:-dat;
}


void Encoder_Init()
{
	FTM_QD_QuickInit(FTM1_QD_PHA_PA08_PHB_PA09,kFTM_QD_NormalPolarity,kQD_PHABEncoding);
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
		if(LeftSpeed.value!=0)
		{
  		LeftSpeed.value=0xFFFF-LeftSpeed.value;
		}
    if(abs_int(LeftSpeed.value-LeftSpeed.last_value)>MAX_DIFF)
		{
			LeftSpeed.value = LeftSpeed.last_value;
		}
		LeftSpeed.last_value = LeftSpeed.value;
	}
	else
	{
		FTM_QD_GetData(HW_FTM2,&(RightSpeed.value),&(RightSpeed.direction));
		FTM_QD_ClearCount(HW_FTM2);
		if(RightSpeed.value!=0)
		{
      RightSpeed.value=0xFFFF - RightSpeed.value;
		}
		if(abs_int(RightSpeed.value-RightSpeed.last_value)>MAX_DIFF)
		{
			RightSpeed.value = RightSpeed.last_value;
		}
		RightSpeed.last_value = RightSpeed.value;
	}
}
