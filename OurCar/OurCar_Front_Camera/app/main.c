#include <stdio.h>
#include <math.h>
/*Drivers*/
#include "common.h"
#include "gpio.h"
#include "uart.h"
#include "ftm.h"//更改PWM通道
#include "i2c.h"
#include "adc.h"
#include "pit.h"
#include "sd.h"//更改寄存器
#include "can.h"
#include "deviceio.h"//重载底层函数体
#include "flash.h"
#include "uart.h"
/*Devices*/
#include "motor.h"
#include "oled.h"
#include "led.h"
#include "sccb.h"
#include "Eagle.h"
#include "servo.h"
#include "BatteryMon.h"
#include "encoder.h"
#include "anglesensor.h"
#include "key.h"
#include "bluetooth.h"
#include "IR.h"
#include "SR04.h"
#include "adns9800.h"
/*app*/
#include "umc.h"
#include "ctrl.h"
#include "Debug.h"
#include "filter.h"
#include "Get_Beacon.h"
#include "can_communication.h"
#include "onchipflash.h"
#include "findroute.h"

#define Flow_RATIO 331.2    //光流数值与距离转换比例
static void PIT0_CallBack(void);
static void PIT1_CallBack(void);

/*****extern 区****************/
extern struct dPID d_PID;//舵机PD
extern float CarSpeed;
extern uint8 mat[60][80];//标记连通域之后的图像
extern float Speed_EX_H;
extern float Speed_EX_L;
extern uint8   *ov7725_eagle_img_buff;
extern IMG_STATUS_e eagle_img_flag;   //图像状态
extern uint8_t Beacon_cnt;//记录第几个信标
extern float Servo_MID;
extern float Servo_MAX;
extern float Servo_MIN;
extern float Beacon_N;//亮几次信标
//Debug 专用变量
float Debug=0;
float Debug_time=99.7;
uint8 imgbuff[80*60/8];  //二值化图像，1 byte--->8 pixel
uint8 pic[60][80];			 //解压后二值化图像，1 byte--->1 pixel 
uint8_t Frame_Capture_Pic[900];//光流图像
struct system System;//系统相关变量
struct Beacon DstBeacon;//目标信标位置
int16_t Mid_Line = EX_MID;
uint8_t ShowPos_flag=1;			//OLED显示位置相关参数标志，0不显示，1显示
uint8_t Out_Put_Data_flag=0;//串口发送数据标志，0不发送，1发送
uint8_t Obst_Dir_flag = 0;	//障碍标志，记录方向，用于之后转向，0左转避障，1右转避障
float Obstacle_Flag = -1;
uint8_t oled_img_flag=0;		//OLED显示图像标志，0不显示，1显示
uint8_t uart_img_flag=0;		//串口传图像标志，0不发送，1发送
uint8_t start_flag=0;				//比赛开始标志，0表示等待比赛开始，1表示比赛进行中，2表示比赛结束
float Stop_flag=1;					//停车标志，0不停车，1停车
uint8_t TIME0flag_20ms=0;
uint8_t TIME1flag_5ms=0;
uint8_t TIME1flag_10ms=0;
uint8_t TIME1flag_20ms=0;
uint8_t TIME1flag_100ms=0;
uint8_t TIME1flag_1000ms=0;
float L_ex_mid = 58;
float M_ex_mid = 40;
float R_ex_mid = 22;
float ex_mid = 40;
float Servo_PWM=1300;
uint8_t B_Mid_line_flag=0;//后摄像头检测到信标标志
uint8_t B_Mid_line=0;//后摄像头检测到信标位置
uint8_t F_Mid_line_flag=0;//前摄像头检测到信标标志
int Lose_cnt=0;//丢线计数
float Lose_MAX=3.0;
float Obstacle_MAXtime = 8.0;//打角最大转向避障时间
float Obstacle_Straight_time = 0.0;//不打角时间
uint8_t Obstacle_time=1;
float Obstacle_X_Thre = 29.0;
uint8_t Lose_flag=0;//丢线标志
uint8_t Turn_flag=0;
float X_Thre = 26;
//float SR04_Distance1=100.0;
//float SR04_Distance2=100.0;
int16_t Motion_X=0,Motion_Y=0;//360--1mm
float X=0.0,Y=0.0;//单位mm
float ADNS_ANGLE=0,ADNS_ANGLE1;//测试通过adns测角度
//记录转弯时刻的瞬时速度与X阈值，用于调试
float X_temp=0;
float S_temp=0;
double  voltage;//电源电压

int main()
{
	DisableInterrupts;
	/*================延时初始化=================*/
	DelayInit();
	/*=================上位机通信=================*/
  UMC_Init();
	DelayMs(10);
	UART_printf("\nUMC_Init Success!\n");
	/*================LED初始化=================*/
	UART_printf("LED_Init...\n");
  LED_Init();
	/*=================OLED初始化=================*/
	UART_printf("OLED_Init...\n");
	OLED_Init();
	/*=================电池电压初始化=================*/
	UART_printf("BatteryMon_Init...\n");
	BatteryMon_Init();
	/*=================电机初始化,频率10k=================*/
	UART_printf("Motor_Init...\n");
	Motor_Init(10000);
	/*=================舵机初始化=================*/
	UART_printf("Servo_Init...\n");
	Servo_Init(100);
	/*=================CAN通信初始化=================*/
	UART_printf("Can_Init...\n");
	Can_Init();
	/*=================鹰眼初始化=================*/
	UART_printf("Eagle_Init...\n");
  Eagle_Init(imgbuff);
	/*=================PIT0初始化,2ms定时中断=================*/
	UART_printf("PIT_QuickInit...\n");
	PIT_QuickInit(HW_PIT_CH0,2000);
  PIT_CallbackInstall(HW_PIT_CH0,PIT0_CallBack);
	PIT_ITDMAConfig(HW_PIT_CH0,kPIT_IT_TOF,ENABLE);
	/*=================PIT1初始化,5ms定时中断=================*/	
	PIT_QuickInit(HW_PIT_CH1,5000);
	PIT_CallbackInstall(HW_PIT_CH1,PIT1_CallBack);
	PIT_ITDMAConfig(HW_PIT_CH1,kPIT_IT_TOF,ENABLE);
	/*=================红外接收初始化=================*/	
//	UART_printf("IR_Init...\n");	
//  IR_Init();
	/*=================超声波初始化=================*/
//	UART_printf("SR04_Init...\n");	
//	SR04_Init();
	/*=================光流初始化=================*/
//	UART_printf("ADNS9800_init...\n");
//	ADNS9800_init();
	/*====================键盘初始化=======================*/
	UART_printf("Key_Init...\n");
	Key_Init();
	/*=================鼠标滚轮初始化=================*/
	UART_printf("M_Encoder_Init...\n");
	M_Encoder_Init();
	/*================片内flash初始化=============*/
	UART_printf("FLASH_Init...\n");
	FLASH_Init();
	/*================片内flash数据初始化=============*/
	UART_printf("FlashData_Init...\n");
	FlashData_Init();
	/*=================参数链表初始化=================*/
	UART_printf("ParameterList_Init...\n");
	ParameterList_Init();
	/*================准备比赛开始=================*/
	UART_printf("Car_Start...\n");
	Car_Start();
	/*================开启中断=================*/
	UART_printf("EnableInterrupts!\n");
	EnableInterrupts;
	UART_printf("MajorLoop Start!\n");
//单独调试速度与舵机使用
//while(1)
//{
//if(TIME1flag_20ms==1)
//{
//	DelayMs(5);
//	//Can_Send(Speed_ID,Speed_EX_H);
//	FTM_PWM_ChangeDuty(HW_FTM1,1,Servo_PWM);
//	Motor_PWM(0,5000);
//	//;Can_Send(DiffSpeed_ID,0);
//	Key_Scan();
//	Parameter_Change();
//} 
//}
	while(1)
	{
		/*================================================================*/
		if(TIME0flag_20ms==1)
		{
			TIME0flag_20ms=0;
		}
		if(TIME1flag_5ms==1)
		{
			TIME1flag_5ms=0;
		}
		if(TIME1flag_10ms==1)
		{
			TIME1flag_10ms=0;
//最初计划使用光流定位，之后放弃			
//			Get_Motion(&Motion_X,&Motion_Y);
//			X+=(float)Motion_X/Flow_RATIO;
//			Y+=(float)Motion_Y/Flow_RATIO;
//		  sprintf(str,"%-9.0f",Y);
//	    OLED_Print(0,6,(uint8_t*)str);
//			ADNS_ANGLE= atan2(Motion_Y,Motion_X) * (180 / 3.14159265);
//			ADNS_ANGLE1= atan2(Y,X) * (180 / 3.14159265);
			Key_Scan();
		}
		if(TIME1flag_20ms==1)
		{
			TIME1flag_20ms=0;
			//开始采集下一帧图像
		  eagle_get_img();
			//解压图像600=80*60/8
			img_extract(&pic[0][0],imgbuff,600);
			//扫描识别信标
			GetBeacon(mat,&DstBeacon);
			//LED指示状态
			LED_Switch(F_Mid_line_flag);
			//Debug  用于定时停车等，放在识别之后，控制之前
			Debug_Check();
			//控制部分
			if(F_Mid_line_flag==1&&start_flag!=2)//前摄像头找到信标且比赛没有结束
			{
				Lose_flag=0;
				Lose_cnt=0;
				start_flag=1;	
				Mid_Line  = DstBeacon.y;
				Beacon_CNT();
				if(DstBeacon.x<=Obstacle_X_Thre&&Obstacle_time>1)
				{
					if(Obstacle_time<=Obstacle_Straight_time)FTM_PWM_ChangeDuty(HW_FTM1,1,Servo_MID);
					if(Obstacle_time>1)Obstacle_time--;
				}else if(DstBeacon.x>X_Thre)
				{
					if(Turn_flag>3)
					{
						X_temp=X_Thre;
						S_temp=CarSpeed;
						//sprintf(str,"%d,%3.2f\n",(int)X_temp,S_temp);
						//UART_putstr(HW_UART0,str);
					}
					Turn_flag=1;
					Path_Finding();
				}
				else{
					Turn_flag++;
					if(Turn_flag>3)Turn_flag=4;
					Path_Runing();
					Check_Obstacle();
				}	
			}else if(start_flag==1)//没有识别到信标且比赛进行中
			{ 
				Lose_cnt++;
				if(DstBeacon.x<=Obstacle_X_Thre&&Obstacle_time>1)
				{
					if(Obstacle_time<=Obstacle_Straight_time)FTM_PWM_ChangeDuty(HW_FTM1,1,Servo_MID);
					if(Obstacle_time>1)Obstacle_time--;
				}else if(Lose_cnt>=Lose_MAX)
				{	
					Lose_cnt=Lose_MAX+1;
					Path_Circling();
					if(Stop_flag>=1&&Beacon_cnt>=Beacon_N-1&&Turn_flag==1)
					{
						Car_Stop();
					}
				}
			}
			//发送数据给上位机	
			if(Out_Put_Data_flag)
			{
				Out_Data[0]=(int)((X_Thre-23)*100);
				Out_Data[1]=(int)(CarSpeed*100);
				Out_Data[2]=(int)(0);
				Out_Data[3]=(int)(Beacon_cnt*50);
				Out_Put_Data();		
			}
			if(oled_img_flag)//按键4按下改变oled_img_flag，为1时通过Oled显示二值化图像
			{
			   Oled_Img(&pic[0][0]);
			}
			if(uart_img_flag)//按键3按下改变uart_img_flag，为1时通过串口0发送二值化图像
			{
				//sendimg(&mat[0][0],80*60);//标记连通域后的图像
				sendimg(imgbuff,600);//二值化图像600=80*60/8
			}
			Show_Pos(ShowPos_flag);		
		}
		if(TIME1flag_100ms==1)
		{
			TIME1flag_100ms=0;	
		}
		if(TIME1flag_1000ms==1)
		{
			TIME1flag_1000ms=0;
      //采集电池电压
			voltage=Get_BatVol();
		}
		//按键与滚轮事件处理
		Parameter_Change();
	}
}

static void PIT0_CallBack()
{
	static uint8_t pit0_cnt=0;	
	pit0_cnt++;
	//鼠标滚轮扫描
  M_Encoder_Scan();
	System.time+=0.002;
	if(pit0_cnt>=10)
	{
		pit0_cnt=0;
		TIME0flag_20ms=1;
	}	
}

static void PIT1_CallBack()
{
	static uint8_t pit1_cnt=0;
	
	pit1_cnt++;
	if(pit1_cnt%1==0)
	{
		TIME1flag_5ms=1;
	}
	if(pit1_cnt%2==0)
	{
		TIME1flag_10ms=1;
	}
	if(pit1_cnt%4==0)
	{
		TIME1flag_20ms=1;
	}
	if(pit1_cnt%20==0)
	{
		TIME1flag_100ms=1;
	}
	if(pit1_cnt%200==0)
	{
		TIME1flag_1000ms=1;
	}
	if(pit1_cnt>=200)
	{
		pit1_cnt=0;
	}
}


