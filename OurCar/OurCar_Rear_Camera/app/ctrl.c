#include <math.h>
#include "ctrl.h"
#include "encoder.h"
#include "motor.h"
#include "debug.h"
#include "filter.h"
#include "can.h"
#include "can_communication.h"

float CarSpeed=0;
float CarSpeedL=0;
float CarSpeedR=0;
float Speed_Integ=0;
float SpeedCtrlOutNew=0;
float Speed_P=3045;//3337.0;//7000;//5000;//2500;//50;
float Speed_D=47;//331.0;//150;//50;
float Speed_I=187.0;//230;//1777.0;//10;//250;
float Speed_EX=0.0;//2.1;//1.8;
float SpeedCtrl_Limit;

float DirectCtrlOut=0;
float Direct_P=50;//40;//34;
float Direct_I=0;
float Direct_D=280;//180;//102; 
float LeftPWM=0,RightPWM=0;

float DiffSpeed_flag = 0;
float MOTOR_OUT_DEAD_VAL=777;

void SetSpeedPI(int p,int i)
{
	Speed_P=p;
	Speed_I=i;
}

void SetMotorDead(int val)
{
	MOTOR_OUT_DEAD_VAL=val;
}




uint32_t pluse_sum = 0;
extern struct system System;//系统相关变量

void SpeedCalculate(void)
{
	extern Encoder_Speed_Type LeftSpeed,RightSpeed;
	extern LP1Filter_Type LP1Filter_CarSpeed;
	
	Encoder_GetPulse(LeftMotor);
	Encoder_GetPulse(RightMotor);
	pluse_sum+=RightSpeed.value;
	if(LeftSpeed.value>-50000 && LeftSpeed.value<50000 && RightSpeed.value>-50000 && RightSpeed.value<50000)
	{
		CarSpeed=(LeftSpeed.value+RightSpeed.value)>>1;
		CarSpeed*=SPEED_RATIO;
		CarSpeedL=LeftSpeed.value*SPEED_RATIO;
		CarSpeedR=RightSpeed.value*SPEED_RATIO;
		CarSpeed=LP1Filter_getVal(&LP1Filter_CarSpeed,CarSpeed,0.7);
		System.distance+=CarSpeed*Ts;
	}
}


/*********************************************
*速度控制
*增量式PID
*运算结果通过CAN发送给主控控制电机
*********************************************/
float I_Domin=0.97;
void SpeedCtrl(void)
{
	float speed_err=0;
	static float speed_err_last=0;
	static float speed_err_pre=0;	
	speed_err=Speed_EX-CarSpeed;

	
	  if(speed_err>Speed_EX*I_Domin)
	{		
		SpeedCtrlOutNew = MOTOR_OUT_DEAD_VAL+Speed_P*(speed_err)+Speed_I*speed_err+Speed_D*(speed_err - 2*speed_err_last + speed_err_pre);
	}else
	{
		SpeedCtrlOutNew += Speed_P*(speed_err-speed_err_last)+Speed_I*speed_err+Speed_D*(speed_err - 2*speed_err_last + speed_err_pre);
	}
	
	if(SpeedCtrlOutNew>PWM_MAX)SpeedCtrlOutNew=PWM_MAX;
	
	
//  if(speed_err>Speed_EX*0.97)
//	{		
//		SpeedCtrlOutNew = MOTOR_OUT_DEAD_VAL+Speed_P*(speed_err)+Speed_I*speed_err+Speed_D*(speed_err - 2*speed_err_last + speed_err_pre);
//	}else
//	{
//		SpeedCtrlOutNew += Speed_P*(speed_err)+Speed_I*speed_err+Speed_D*(speed_err - 2*speed_err_last + speed_err_pre);
//	}
//	
	speed_err_pre = speed_err_last;
	speed_err_last = speed_err;
	
	Can_Send(Speed_ID,SpeedCtrlOutNew);
}


void Choose_X_Thre(float speed)
{
	if(speed<2.0)
	{
		Can_Send(X_Thre_ID,27.0);
	}else if(speed<4.0)
	{
		Can_Send(X_Thre_ID,26.0);
	}else if(speed<4.2)
	{
		Can_Send(X_Thre_ID,25.0);
	}else
	{
		Can_Send(X_Thre_ID,24.0);
	}
}


float Speed_EXL=0;
float Speed_EXR=0;
void DiffSpeedCtrl(void)
{
	float speed_errl=0,speed_errr=0;
	static float speed_err_lastl=0,speed_err_lastr=0;
	static float speed_err_prel=0,speed_err_prer=0;
	static float SpeedCtrlOutNewl=0,SpeedCtrlOutNewr=0;
	uint16_t pwm[2]={0,0};
	speed_errl=Speed_EXL-CarSpeedL;
	if(speed_errl>Speed_EXL*I_Domin)
	{		
		SpeedCtrlOutNewl = (MOTOR_OUT_DEAD_VAL+Speed_P*(speed_errl)+Speed_I*speed_errl+Speed_D*(speed_errl - 2*speed_err_lastl + speed_err_prel));
	}else
	{
		SpeedCtrlOutNewl += (Speed_P*(speed_errl-speed_err_lastl)+Speed_I*speed_errl+Speed_D*(speed_errl - 2*speed_err_lastl + speed_err_prel));
	}
	if(SpeedCtrlOutNewl>PWM_MAX)SpeedCtrlOutNewl=PWM_MAX;
	if(SpeedCtrlOutNewl<PWM_MIN)SpeedCtrlOutNewl=PWM_MIN;
	speed_err_prel = speed_err_lastl;
	speed_err_lastl = speed_errl;
	

	speed_errr=Speed_EXR-CarSpeedR;
	if(speed_errr>Speed_EXR*I_Domin)
	{		
		SpeedCtrlOutNewr = (MOTOR_OUT_DEAD_VAL+Speed_P*(speed_errr)+Speed_I*speed_errr+Speed_D*(speed_errr - 2*speed_err_lastr + speed_err_prer));
	}else
	{
		SpeedCtrlOutNewr += (Speed_P*(speed_errr-speed_err_lastr)+Speed_I*speed_errr+Speed_D*(speed_errr - 2*speed_err_lastr + speed_err_prer));
	}
	if(SpeedCtrlOutNewr>PWM_MAX)SpeedCtrlOutNewr=PWM_MAX;
	if(SpeedCtrlOutNewr<PWM_MIN)SpeedCtrlOutNewr=PWM_MIN;
	speed_err_prer = speed_err_lastr;
	speed_err_lastr = speed_errr;	
	

	pwm[0]=(uint16_t)SpeedCtrlOutNewl;	
	pwm[1]=(uint16_t)SpeedCtrlOutNewr;	
	Can_Send(DiffSpeed_ID,*((float *)pwm));
}





