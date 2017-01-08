#include <math.h>
#include "ctrl.h"
#include "encoder.h"
#include "motor.h"
#include "debug.h"
#include "filter.h"


float CarSpeed=0;
float Speed_Integ=0;
float SpeedCtrlOutOld=0,SpeedCtrlOutNew=0,SpeedCtrlOutEach=0,SpeedCtrlNow=0;
float Speed_P=0;//5000;//2500;//50;
float Speed_D=0;//50;
float Speed_I=0;//10;//250;
float Speed_EX_H=4.0;//2.1;//1.8;
float Speed_EX_L=1.7;
float Speed_EX=1.0;
float DiffSpeedH = 2.7;
float DiffSpeedL = 1.7;
float SpeedCtrl_Limit;


float LeftPWM=0,RightPWM=0;

float LMOTOR_OUT_DEAD_VAL=0;
float RMOTOR_OUT_DEAD_VAL=0;




void SetSpeedPI(int p,int i)
{
	Speed_P=p;
	Speed_I=i;
}

void SetMotorDead(int lval,int rval)
{
	LMOTOR_OUT_DEAD_VAL=lval;
	RMOTOR_OUT_DEAD_VAL=rval;
}



void SpeedCalculate()
{
	extern Encoder_Speed_Type LeftSpeed,RightSpeed;
	extern LP1Filter_Type LP1Filter_CarSpeed;
	
	Encoder_GetPulse(LeftMotor);
	Encoder_GetPulse(RightMotor);
	
	if(LeftSpeed.value>-50000 && LeftSpeed.value<50000 && RightSpeed.value>-50000 && RightSpeed.value<50000)
	{
		CarSpeed=(LeftSpeed.value+RightSpeed.value)>>1;
		CarSpeed*=SPEED_RATIO;
		CarSpeed=LP1Filter_getVal(&LP1Filter_CarSpeed,CarSpeed,0.1);
	}
}

void SpeedCtrl()
{
	float speed_err=0;
	extern float Ts;
	static float speed_err_old=0;
	
	speed_err=CarSpeed-Speed_EX;
	Speed_Integ+=speed_err*Ts;
	//ÓöÏÞÏûÈõ
	if(Speed_Integ>=1000 || Speed_Integ<=-1000)
	{
		Speed_Integ-=speed_err*Ts;
	}
	
	SpeedCtrlOutOld=SpeedCtrlOutNew;
	SpeedCtrlOutNew=Speed_P*speed_err+Speed_I*Speed_Integ+Speed_D*(speed_err-speed_err_old);
	speed_err_old=speed_err;
	
	if(SpeedCtrlOutNew>SpeedCtrl_Limit)
	{
		SpeedCtrlOutNew=SpeedCtrl_Limit;
	}
	else if(SpeedCtrlOutNew<-SpeedCtrl_Limit)
	{
		SpeedCtrlOutNew=-SpeedCtrl_Limit;
	}
	SpeedCtrlOutEach=(SpeedCtrlOutNew-SpeedCtrlOutOld)/4.0;
}


