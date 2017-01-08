#include "findroute.h"
#include "can_communication.h"
#include "servo.h"
#include "ftm.h"
#include "common.h"

#define DiffSpeed 1

float Beacon_N=10;//亮几次信标
//int8_t Dir_Preset[30]={1,0,1,1,1,0,1,1,1            ,1,1,1,1,1,1,1,1,1,1,1};//预赛
//int8_t Dir_Preset[30]={1,0,1,1,0,0,0,0,1,0,0,0,          0,0,0,0,0,0,0,0};//决赛 
//int8_t Dir_Preset[30]={3,0,3,3,3,2,3,3,3           ,1,1,1,1,1,1,1,1,1,1,1};//预赛
//int8_t Dir_Preset[30]={3,2,3,2,2, 2,2,0,3,2 ,2,2              ,1,1,1,1,1,1,1,1};//决赛
//	int8_t Dir_Preset[30]={3,2,3,2,3,2,3,2,3,2,3,3              ,1,1,1,1,1,1,1,1};//决赛
//mint8_t Dir_Preset[30]={1,1,1,1,1,1,1,1,1,1,1,1              ,1,1,1,1,1,1,1,1};//决赛

//int8_t Dir_Preset[30]={13,12,3,2,2,2,2,12,3,2,2,2            ,1,1,1,1,1,1,1,1};
//int8_t Dir_Preset[30]={5,3,3,5,3,  5,2,2,3,3,  2,2,2,4,2,  2,2,2,2,2};
//float Obs_preset[30] = {0,0,0,0,0   ,0,0,0,0,0     ,0,0,0,0,0,    0,0,0,0,0};
//	int8_t Dir_Preset[30]={5,3,4,2,2,2,2,2,2,2,2,2,                 1,1,1,1,1,1,1,1};//


//32422
//int8_t Dir_Preset[30]={3,2,4,2,2,2,2,2,2,2,2,3,3,3,3,                1,1,1,1,1};//
//float Obs_preset[30] = {0,0,0,0,0   ,0,0,0,0,0     ,0,0,0,0,0,    0,0,0,0,0};


//15432
//int8_t Dir_Preset[30]={2,4,2,2,2,     3,3,3,3,3,         4,2,0,3,3,     4,2,5,    0,0,     0,0,0,0,0,     0,0,0,0,0};//
int8_t Dir_Preset[30]={2,4,2,2,2,     3,1,3,3,2,         5,2,2,3,3,     12,2,5,    0,0,     0,0,0,0,0,     0,0,0,0,0};//国赛决赛

//int8_t Dir_Preset[30]={3,3,3,3,3,     3,3,3,3,3,         3,3,3,3,3,     3,3,3,    0,0,     0,0,0,0,0,     0,0,0,0,0};//
//int8_t Dir_Preset[30]={3,2,2,2,2,     2,2,2,2,2,         2,2,2,2,2,     2,2,2,    0,0,     0,0,0,0,0,     0,0,0,0,0};//
float Obs_preset[30] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
//int8_t Dir_Preset[30]={2,2,2,2,2,  2,2,2,2,2,  2,2,2,2,2  ,3,3,3,3,3        ,3,3,3,3,3   , 3,3,3,3,3};//

//14253
//int8_t Dir_Preset[30]={3,2,2,2,2,2,2,5,2,2,2,2,3,2,2,                1,1,1,1,1};//
//float Obs_preset[30] = {0,0,0,0,0,1.77,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
//float Obs_preset[30] = {0,0,0.78,0.78,0.78,0,0,0,0,0.78,0,0,0,0,0,0,0,0,0,0};
extern float L_ex_mid;
extern float M_ex_mid;
extern float R_ex_mid;
extern float ex_mid;
extern uint8_t Beacon_cnt;
extern float Speed_EX_H;
extern float Speed_EX_L;
extern float DiffSpeedH;
extern float DiffSpeedL;
extern struct dPID d_PID;//舵机PD
extern int16_t Mid_Line ;
extern float Servo_MID;
extern float Servo_MAX;
extern float Servo_MIN;

void Route_Init(void)
{
	if(Dir_Preset[0]==0||Dir_Preset[0]==5)
	{
		ex_mid=R_ex_mid;
	}else if(Dir_Preset[0]==1||Dir_Preset[0]==4)
	{
		ex_mid=L_ex_mid;
	}else
	{
		ex_mid=M_ex_mid;
	}	
	Can_Send(Speed_ID,0);
}

void Path_Finding(void)
{
	if(Dir_Preset[Beacon_cnt]==0||Dir_Preset[Beacon_cnt]==5||Dir_Preset[0]==6)
	{
		ex_mid=R_ex_mid;
	}else if(Dir_Preset[Beacon_cnt]==1||Dir_Preset[Beacon_cnt]==4||Dir_Preset[0]==7)
	{
		ex_mid=L_ex_mid;
	}
	if(Dir_Preset[Beacon_cnt]==0)
	{
#if DiffSpeed
		Can_SendDiffSpeed(DiffSpeed_ID,DiffSpeedL,DiffSpeedH);
#else
		Can_Send(Speed_ID,Speed	_EX_L);
#endif
		AngleCtl(&d_PID,Mid_Line);
	}else if(Dir_Preset[Beacon_cnt]==1)
	{
#if DiffSpeed
		Can_SendDiffSpeed(DiffSpeed_ID,DiffSpeedH,DiffSpeedL);
#else
		Can_Send(Speed_ID,Speed_EX_L);
#endif
		AngleCtl(&d_PID,Mid_Line);
	}else if(Dir_Preset[Beacon_cnt]==2)
	{
#if DiffSpeed
		Can_SendDiffSpeed(DiffSpeed_ID,DiffSpeedL,DiffSpeedH);
#else
		Can_Send(Speed_ID,Speed_EX_L);
#endif
		FTM_PWM_ChangeDuty(HW_FTM1,1,Servo_MIN);
	}else if(Dir_Preset[Beacon_cnt]==3)
	{
#if DiffSpeed
		Can_SendDiffSpeed(DiffSpeed_ID,DiffSpeedH,DiffSpeedL);
#else
		Can_Send(Speed_ID,Speed_EX_L);
#endif
		FTM_PWM_ChangeDuty(HW_FTM1,1,Servo_MAX);
	}else if(Dir_Preset[Beacon_cnt]==12)
	{
		FTM_PWM_ChangeDuty(HW_FTM1,1,Servo_MIN);
	}else if(Dir_Preset[Beacon_cnt]==13)
	{
		FTM_PWM_ChangeDuty(HW_FTM1,1,Servo_MAX);
	}else if(Dir_Preset[Beacon_cnt]==4)
	{
		Can_Send(Speed_ID,Speed_EX_H);
		AngleCtl(&d_PID,Mid_Line);
	}else if(Dir_Preset[Beacon_cnt]==5)
	{
		Can_Send(Speed_ID,Speed_EX_H);
		AngleCtl(&d_PID,Mid_Line);
	}
}


float ptest = 0;
void Path_Runing(void)
{
	if(Dir_Preset[Beacon_cnt]==0||Dir_Preset[Beacon_cnt]==5)
	{
		ex_mid=R_ex_mid;
	}else if(Dir_Preset[Beacon_cnt]==1||Dir_Preset[Beacon_cnt]==4)
	{
		ex_mid=L_ex_mid;
	}else
	{
		if(Dir_Preset[Beacon_cnt]==2||Dir_Preset[0]==7||Dir_Preset[Beacon_cnt]==12)ex_mid=M_ex_mid+(uint8_t)ptest;
		else if(Dir_Preset[Beacon_cnt]==3||Dir_Preset[0]==6||Dir_Preset[Beacon_cnt]==13)ex_mid=M_ex_mid-(uint8_t)ptest;
	}
	Can_Send(Speed_ID,Speed_EX_H);//Send Speed_EX
	AngleCtl(&d_PID,Mid_Line);
}


void Path_Circling(void)
{
	if(Dir_Preset[Beacon_cnt]==0||Dir_Preset[Beacon_cnt]==2||Dir_Preset[Beacon_cnt]==4||Dir_Preset[Beacon_cnt]==12)
	{
#if DiffSpeed
		Can_SendDiffSpeed(DiffSpeed_ID,DiffSpeedL,DiffSpeedH);
#else
		Can_Send(Speed_ID,Speed_EX_L);
#endif
		FTM_PWM_ChangeDuty(HW_FTM1,1,Servo_MIN);
	}else if(Dir_Preset[Beacon_cnt]==1||Dir_Preset[Beacon_cnt]==3||Dir_Preset[Beacon_cnt]==5||Dir_Preset[Beacon_cnt]==13)
	{
#if DiffSpeed
		Can_SendDiffSpeed(DiffSpeed_ID,DiffSpeedH,DiffSpeedL);
#else
		Can_Send(Speed_ID,Speed_EX_L);
#endif
		FTM_PWM_ChangeDuty(HW_FTM1,1,Servo_MAX);
	}
}


extern struct system System;//系统相关变量
extern float Obstacle_MAXtime;//打角最大转向避障时间
extern float Obstacle_Straight_time;//不打角时间
extern uint8_t Obstacle_time;
int ObsBeacon_cnt=-1;
void Check_Obstacle(void)
{
	static float Distance = 0;
	static uint8_t Obs_flag;
	if(ObsBeacon_cnt!=Beacon_cnt)
	{
		ObsBeacon_cnt=Beacon_cnt;
		if(Obs_preset[Beacon_cnt]>0.05)
		{
			Distance=System.distance;
			Obs_flag=1;
		}else
		{
			Obs_flag=0;
		}
	}else if(Obs_flag==1)
	{
		if(System.distance-Distance>Obs_preset[Beacon_cnt])
		{
			Obs_flag=0;
			Obstacle_time=Obstacle_MAXtime+Obstacle_Straight_time;
			if(Dir_Preset[Beacon_cnt]==0||Dir_Preset[Beacon_cnt]==2||Dir_Preset[Beacon_cnt]==12)FTM_PWM_ChangeDuty(HW_FTM1,1,Servo_MAX);
			else if(Dir_Preset[Beacon_cnt]==1||Dir_Preset[Beacon_cnt]==3||Dir_Preset[Beacon_cnt]==13)FTM_PWM_ChangeDuty(HW_FTM1,1,Servo_MIN);
		}
	}
}

int Beacon_n=0;
Pos Postion[6];//把起点当作一个点，即Postion[0]
int order[20]={0,2,3,5,4,1,2,3,5,4,1};//第一个点设为0，其实把起点发车区当作一个点
int result[20]={5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5};
	
void Init_routedata(void)
{
    Postion[0].x=0;
    Postion[0].y=0;
    Postion[1].x=0.5;
    Postion[1].y=1;
    Postion[2].x=1;
    Postion[2].y=3.5;
    Postion[3].x=2;
    Postion[3].y=2;
    Postion[4].x=4;
    Postion[4].y=4;
    Postion[5].x=5;
    Postion[5].y=1;
    Beacon_n=10;
    order[Beacon_n+1]=order[1];

}

void Find_Route(void)
{
    int i=0;
    float k,point_value;
    for(i=0;i<Beacon_n;i++)
    {
        k=(Postion[order[i+1]].y-Postion[order[i]].y)/(Postion[order[i+1]].x-Postion[order[i]].x+0.0010101);
        if( (k>=0 && Postion[order[i+1]].y >= Postion[order[i]].y) || (k<0 && Postion[order[i+1]].y <= Postion[order[i]].y))
        {
            point_value=(Postion[order[i+2]].x-Postion[order[i]].x)*k+Postion[order[i]].y;
            if(point_value<Postion[order[i+2]].y)result[i]=0;
            else if(point_value>Postion[order[i+2]].y)result[i] = 1;
            else result[i]=2;
        }else if((k>=0 && Postion[order[i+1]] .y< Postion[order[i]].y) || (k<0 && Postion[order[i+1]].y > Postion[order[i]].y))
        {
            point_value=(Postion[order[i+2]].x-Postion[order[i]].x)*k+Postion[order[i]].y;
            if(point_value>Postion[order[i+2]].y)result[i]=0;
            else if(point_value<Postion[order[i+2]].y)result[i] = 1;
            else result[i]=2;
        }
    }
    if(result[Beacon_n-1]==0)result[Beacon_n-1]=-1;
    else if(result[Beacon_n-1]==1)result[Beacon_n-1]=-2;
}





