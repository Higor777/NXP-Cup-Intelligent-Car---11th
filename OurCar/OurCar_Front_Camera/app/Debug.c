#include "common.h"
#include "Debug.h"
#include "KEY.h"
#include "gpio.h"
#include "Motor.h"
#include "OLED.h"
#include "filter.h"
#include "adc.h"
#include "Eagle.h"
#include "sccb.h"
#include "anglesensor.h"
#include <stdio.h>
#include <string.h>
#include "can_communication.h"
#include "onchipflash.h"
#include "servo.h"
#include "ftm.h"
#include "findroute.h"

#define Dirset_Num  6//绕标方式个数

float Temp_Para=450;

extern reg_s ov7725_eagle_reg[];
extern float Threshold;
float Flash;
ParameterNode_Type P_Ctrl[20];//常调参数
ParameterNode_Type P_Ctrl1[20];//不常调参数
ParameterNode_Type P_Ctrl2[35];//调试和避障参数
void ParameterList_Init(void)
{
	ParameterNode_Type node;
	ParameterNode_Type *blank_node;
	ParameterList_Type insert_place=0;
	blank_node=P_Ctrl;
	
//	extern float Servo_PWM;
//	node.exterdata=&(Servo_PWM);
//	node.step=1;
//	sprintf(node.name,"Servo_PWM"); 
//	Insert_ParameterNode(&node,&insert_place,blank_node++);
	
//	extern float Diff_PWM;
//	node.exterdata=&(Diff_PWM);
//	node.step=100;
//	sprintf(node.name,"Diff_PWM"); 
//	Insert_ParameterNode(&node,&insert_place,blank_node++);
//	
	extern float L_ex_mid;
	node.exterdata=&(L_ex_mid);
	node.step=1;
	sprintf(node.name,"L_ex_mid"); 
	Insert_ParameterNode(&node,&insert_place,blank_node++);
	
	extern float R_ex_mid;
	node.exterdata=&(R_ex_mid);
	node.step=1;
	sprintf(node.name,"R_ex_mid"); 
	Insert_ParameterNode(&node,&insert_place,blank_node++);
	
	extern float Speed_EX_H;
	node.exterdata=&(Speed_EX_H);
	node.step=0.1;
	sprintf(node.name,"Speed_EXH");
	Insert_ParameterNode(&node,&insert_place,blank_node++);

	extern float Speed_EX_L;
	node.exterdata=&(Speed_EX_L);
	node.step=0.1;
	sprintf(node.name,"Speed_EXL");
	Insert_ParameterNode(&node,&insert_place,blank_node++);
	
	extern float DiffSpeedH;
	node.exterdata=&(DiffSpeedH);
	node.step=0.1;
	sprintf(node.name,"DiffSpH");
	Insert_ParameterNode(&node,&insert_place,blank_node++);

	extern float DiffSpeedL;
	node.exterdata=&(DiffSpeedL);
	node.step=0.1;
	sprintf(node.name,"DiffSpL");
	Insert_ParameterNode(&node,&insert_place,blank_node++);
	
	extern float X_Thre;
	node.exterdata=&(X_Thre);
	node.step=1.0;
	sprintf(node.name,"X_Thre");
	Insert_ParameterNode(&node,&insert_place,blank_node++);

	extern float X_Thre_adj;
	node.exterdata=&(X_Thre_adj);
	node.step=1.0;
	sprintf(node.name,"X_adj");
	Insert_ParameterNode(&node,&insert_place,blank_node++);
	
	extern float Cnt_X_Thre;
	node.exterdata=&(Cnt_X_Thre);
	node.step=1.0;
	sprintf(node.name,"Cnt_X");
	Insert_ParameterNode(&node,&insert_place,blank_node++);

	extern float Flash;
	node.exterdata=&(Flash);
	node.step=1;
	sprintf(node.name,"Flash"); 
	Insert_ParameterNode(&node,&insert_place,blank_node++);

	//另开一个循环链表留下调一些不常调的参数，以免出现意外，相当于是另一页页菜单
	blank_node=P_Ctrl1;
	insert_place=0;

	extern float Threshold;
	node.exterdata=&(Threshold);
	node.step=1;
	sprintf(node.name,"Threshold"); 
	Insert_ParameterNode(&node,&insert_place,blank_node++);
	
	extern float Lose_MAX;
	node.exterdata=&(Lose_MAX);
	node.step=1;
	sprintf(node.name,"Lose_MAX"); 
	Insert_ParameterNode(&node,&insert_place,blank_node++);
	
	extern float Obstacle_MAXtime;
	node.exterdata=&(Obstacle_MAXtime);
	node.step=1;
	sprintf(node.name,"Obst_MAX"); 
	Insert_ParameterNode(&node,&insert_place,blank_node++);
	
	extern float Obstacle_Straight_time;
	node.exterdata=&(Obstacle_Straight_time);
	node.step=1;
	sprintf(node.name,"Obst_Str"); 
	Insert_ParameterNode(&node,&insert_place,blank_node++);

	extern float Obstacle_X_Thre;
	node.exterdata=&(Obstacle_X_Thre);
	node.step=1;
	sprintf(node.name,"Obst_X"); 
	Insert_ParameterNode(&node,&insert_place,blank_node++);
	
	extern float Direct_P;
	node.exterdata=&(Direct_P);
	node.step=0.1;
	sprintf(node.name,"Direct_P");
	Insert_ParameterNode(&node,&insert_place,blank_node++);
	
	extern float Direct_D;
	node.exterdata=&(Direct_D);
	node.step=0.1;
	sprintf(node.name,"Direct_D");
	Insert_ParameterNode(&node,&insert_place,blank_node++);
	
	extern float Ar_max;
	node.exterdata=&(Ar_max);
	node.step=0.05;
	sprintf(node.name,"Ar_max");
	Insert_ParameterNode(&node,&insert_place,blank_node++);
	
	extern float Ar_min;
	node.exterdata=&(Ar_min);
	node.step=0.05;
	sprintf(node.name,"Ar_min");
	Insert_ParameterNode(&node,&insert_place,blank_node++);
	
	extern float Wr_min;
	node.exterdata=&(Wr_min);
	node.step=0.05;
	sprintf(node.name,"Wr_min");
	Insert_ParameterNode(&node,&insert_place,blank_node++);

	extern float White_Min;
	node.exterdata=&(White_Min);
	node.step=1;
	sprintf(node.name,"WP_Min");
	Insert_ParameterNode(&node,&insert_place,blank_node++);

	extern float Cnt_Max;
	node.exterdata=&(Cnt_Max);
	node.step=1;
	sprintf(node.name,"Cnt_Max"); 
	Insert_ParameterNode(&node,&insert_place,blank_node++);

	extern float Stop_flag;
	node.exterdata=&(Stop_flag);
	node.step=1;
	sprintf(node.name,"Stop"); 
	Insert_ParameterNode(&node,&insert_place,blank_node++);
	
	extern float X_Start;
	node.exterdata=&(X_Start);
	node.step=1;
	sprintf(node.name,"X_Start"); 
	Insert_ParameterNode(&node,&insert_place,blank_node++);
	
	extern float Beacon_N;
	node.exterdata=&(Beacon_N);
	node.step=1;
	sprintf(node.name,"Beacon_N"); 
	Insert_ParameterNode(&node,&insert_place,blank_node++);
	
	extern float Servo_MAX;
	node.exterdata=&(Servo_MAX);
	node.step=1;
	sprintf(node.name,"Servo_MAX"); 
	Insert_ParameterNode(&node,&insert_place,blank_node++);
	
	extern float Servo_MIN;
	node.exterdata=&(Servo_MIN);
	node.step=1;
	sprintf(node.name,"Servo_MIN"); 
	Insert_ParameterNode(&node,&insert_place,blank_node++);

	extern float Servo_MID;
	node.exterdata=&(Servo_MID);
	node.step=1;
	sprintf(node.name,"Servo_MID"); 
	Insert_ParameterNode(&node,&insert_place,blank_node++);

	extern float ptest;
	node.exterdata=&(ptest);
	node.step=1;
	sprintf(node.name,"ptest"); 
	Insert_ParameterNode(&node,&insert_place,blank_node++);


	//另开一个循环链表，用于调试时使用，可以开启调试模式，设定调试时间等
	blank_node=P_Ctrl2;
	insert_place=0;
	
	extern float Obs_preset[30];
	node.exterdata=&Obs_preset[5];
	node.step=0.07;
	sprintf(node.name,"Obs6"); 
	Insert_ParameterNode(&node,&insert_place,blank_node++);
	
	node.exterdata=&Obs_preset[6];
	node.step=0.07;
	sprintf(node.name,"Obs7"); 
	Insert_ParameterNode(&node,&insert_place,blank_node++);

	node.exterdata=&Obs_preset[7];
	node.step=0.07;
	sprintf(node.name,"Obs8"); 
	Insert_ParameterNode(&node,&insert_place,blank_node++);
	
	node.exterdata=&Obs_preset[10];
	node.step=0.07;
	sprintf(node.name,"Obs11"); 
	Insert_ParameterNode(&node,&insert_place,blank_node++);
	
	node.exterdata=&Obs_preset[12];
	node.step=0.07;
	sprintf(node.name,"Obs13"); 
	Insert_ParameterNode(&node,&insert_place,blank_node++);
	
	extern float Debug;
	node.exterdata=&(Debug);
	node.step=1;
	sprintf(node.name,"Debug"); 
	Insert_ParameterNode(&node,&insert_place,blank_node++);
	
	extern float Debug_time;
	node.exterdata=&(Debug_time);
	node.step=1;
	sprintf(node.name,"De_time"); 
	Insert_ParameterNode(&node,&insert_place,blank_node++);
	
	OLED_Fill(0x00);
	Show_Parameter(P_Ctrl,0x01|0x02|0x04);
}

void Data_Init(void)
{
	/*不可调参数*/
    ;
}

void Parameter_Change(void)
{
	int temp=0;
	extern float Beacon_N;
	extern uint8_t start_flag;
	extern int8_t Dir_Preset[30];	
	static ParameterNode_Type* para=P_Ctrl;
	static uint8_t Dir_Preset_cnt=0;
	static char paratype=0; 
	uint8_t keysta,m_encoder_sta=0;
	keysta=Key_STA();
	if(M_Encoder_STA&M_Encoder_STA_Update_MASK)
	{
		m_encoder_sta=M_Encoder_STA&(~M_Encoder_STA_Update_MASK);
		M_Encoder_STA=0;
	}
	
	if(keysta&Key_1_MASK)
	{
		if(paratype==2)
		{
			Dir_Preset_cnt=((Dir_Preset_cnt+1)%(int)Beacon_N);
			Show_Dir_Preset(Dir_Preset_cnt+1);
		}else
		{
			para=para->prepara;
			Show_Parameter(para,0x01|0x02|0x04);
		}
	}
	
	if(keysta&Key_2_MASK)
	{
		if(paratype==2)
		{
			if(Dir_Preset_cnt>0)Dir_Preset_cnt=((Dir_Preset_cnt-1)%(int)Beacon_N);
			else Dir_Preset_cnt=Beacon_N-1;
			Show_Dir_Preset(Dir_Preset_cnt+1);
		}else
		{
			para=para->nextpara;
			Show_Parameter(para,0x01|0x02|0x04);
		}
	}
	
	if(keysta&Key_3_MASK)
	{
		//(para->step)++;
		//Show_Parameter(para,0);
		
		
//		extern uint8_t uart_img_flag;
//		uart_img_flag=1-uart_img_flag;
//		if(uart_img_flag)OLED_Print(80,4,"ON ");
//		else OLED_Print(80,4,"OFF");
		Car_Start();
	}
	
	if(keysta&Key_4_MASK)
	{
		extern uint8_t oled_img_flag;
		extern uint8_t ShowPos_flag;
		paratype=(paratype+1)%5;
		if(paratype==0)
		{
			para=P_Ctrl;
			OLED_Fill(0x00); 
			Show_Parameter(para,0x01|0x02|0x04);
			ShowPos_flag=1;
			oled_img_flag=0;
		}else if(paratype==1)
		{
			para=P_Ctrl1;
			OLED_Fill(0x00); 
			Show_Parameter(para,0x01|0x02|0x04);
			ShowPos_flag=1;
			oled_img_flag=0;
		}else if(paratype==2)
		{
			OLED_Fill(0x00); 
			Show_Dir_Preset(Dir_Preset_cnt+1);
			ShowPos_flag=0;
			oled_img_flag=0;
		}else if(paratype==3)
		{
			para=P_Ctrl1;
			oled_img_flag=1-oled_img_flag;
			if(oled_img_flag==0)
			{
				OLED_Fill(0x00); 
				Show_Parameter(para,0x01|0x02|0x04);
			}	
			ShowPos_flag=1;
		}else if(paratype==4)
		{
			para=P_Ctrl2;
			OLED_Fill(0x00); 
			Show_Parameter(para,0x01|0x02|0x04);
			ShowPos_flag=1;
			oled_img_flag=0;
		}
	}
	if(keysta&Key_5_MASK)
	{
			if(!strcmp(para->name,"Flash"))
			{
				if(*(para->exterdata)<=0)
				{
					Read_All_Var();
					OLED_Print(0,4,(uint8_t *)"R Finish!");
				}else
				{
					Write_All_Var();
					OLED_Print(0,4,(uint8_t *)"W Finish!");
				}
				
			}else if(paratype==2)
			{
				for(temp=0;temp<30;temp++)
				{
					if(Dir_Preset[temp]==2||Dir_Preset[temp]==12)
					{
						Dir_Preset[temp]=0;
					}else if(Dir_Preset[temp]==3||Dir_Preset[temp]==12)
					{
						Dir_Preset[temp]=1;
					}
				}
				Show_Dir_Preset(Dir_Preset_cnt);
			}
	}
	
	//if(start_flag==1)return;//如果比赛进行中，则直接返回避免参数被修改
	if(m_encoder_sta&M_Encoder_CCW_MASK)
	{
		if(paratype==2)
		{
			if(Dir_Preset[Dir_Preset_cnt]==0)Dir_Preset[Dir_Preset_cnt]=Dirset_Num-1;
			else Dir_Preset[Dir_Preset_cnt]=(Dir_Preset[Dir_Preset_cnt]-1)%Dirset_Num;
			Show_Dir_Preset(Dir_Preset_cnt+1);
		}else
		{
			*(para->exterdata)-=para->step;
			Show_Parameter(para,0x02);
			if(!strcmp(para->name,"Threshold"))
			{
				while(0==SCCB_WriteByte(OV7725_CNST,(uint8_t)Threshold));
			}else if(strstr(para->name,"Servo"))
			{
					FTM_PWM_ChangeDuty(HW_FTM1,1,*(para->exterdata));
			}
		}
	}
	
	if(m_encoder_sta&M_Encoder_CW_MASK)
	{
		if(paratype==2)
		{
			Dir_Preset[Dir_Preset_cnt]=(Dir_Preset[Dir_Preset_cnt]+1)%Dirset_Num;
			Show_Dir_Preset(Dir_Preset_cnt+1);
		}else
		{
			*(para->exterdata)+=para->step;
			Show_Parameter(para,0x02);
			if(!strcmp(para->name,"Threshold"))
			{
				while(0==SCCB_WriteByte(OV7725_CNST,(uint8_t)Threshold));
			}else if(strstr (para->name,"Servo"))
			{
				 FTM_PWM_ChangeDuty(HW_FTM1,1,*(para->exterdata));
			}
		}
	}
}

//config
//bit0:	显示参数名
//bit1:	显示参数
//bit2:	显示步进值
void Show_Parameter(ParameterNode_Type *para, uint8_t config)
{
	char str[10];
	
	//显示参数名
	if(config&0x01)
	{
		sprintf(str,"%-9s",para->name);
		OLED_Print(0,0,(uint8_t*)str);
	}
	//显示参数
	if(config&0x02)
	{
		sprintf(str,"%-9.2f",*(para->exterdata));
		OLED_Print(0,2,(uint8_t*)str);
	}
	//显示步进值
	if(config&0x04)
	{
		sprintf(str,"%-9.2f",para->step);
		OLED_Print(0,4,(uint8_t*)str);
	}
}

//config  第几个信标
void Show_Dir_Preset(uint8_t config)
{
	extern int8_t Dir_Preset[30];
	static char str[10];
	sprintf(str,"%-2d",config);
	OLED_Print(0,0,(uint8_t*)str);
	if(config>15)
	{
		sprintf(str,"%-2d %-2d %-2d %-2d %-2d",Dir_Preset[15],Dir_Preset[16],Dir_Preset[17],Dir_Preset[18],Dir_Preset[19]);
		OLED_Print(0,2,(uint8_t*)str);
		sprintf(str,"%-2d %-2d %-2d %-2d %-2d",Dir_Preset[20],Dir_Preset[21],Dir_Preset[22],Dir_Preset[23],Dir_Preset[24]);
		OLED_Print(0,4,(uint8_t*)str);
		sprintf(str,"%-2d %-2d %-2d %-2d %-2d",Dir_Preset[25],Dir_Preset[26],Dir_Preset[27],Dir_Preset[28],Dir_Preset[29]);
		OLED_Print(0,6,(uint8_t*)str);
	}else
	{
		sprintf(str,"%-2d %-2d %-2d %-2d %-2d",Dir_Preset[0],Dir_Preset[1],Dir_Preset[2],Dir_Preset[3],Dir_Preset[4]);
		OLED_Print(0,2,(uint8_t*)str);
		sprintf(str,"%-2d %-2d %-2d %-2d %-2d",Dir_Preset[5],Dir_Preset[6],Dir_Preset[7],Dir_Preset[8],Dir_Preset[9]);
		OLED_Print(0,4,(uint8_t*)str);
		sprintf(str,"%-2d %-2d %-2d %-2d %-2d",Dir_Preset[10],Dir_Preset[11],Dir_Preset[12],Dir_Preset[13],Dir_Preset[14]);
		OLED_Print(0,6,(uint8_t*)str);
	}
	
}



//在insert_place节点后插入新节点,并使insert_place指向新插入的节点
static void Insert_ParameterNode(ParameterNode_Type *insert_node, ParameterList_Type *insert_place, ParameterNode_Type *blank_node)
{
	//复制数据
	blank_node->exterdata=insert_node->exterdata;
	for(uint8_t i=0;i<ParameterName_Size;i++)
	{
		blank_node->name[i]=insert_node->name[i];
	}
	blank_node->step=insert_node->step;
	
	//插入节点
	if((*insert_place)!=0)
	{
		blank_node->nextpara=(*insert_place)->nextpara;
		((*insert_place)->nextpara)->prepara=blank_node;
		(*insert_place)->nextpara=blank_node;
		blank_node->prepara=(*insert_place);
	}
	else
	{
		blank_node->nextpara=blank_node;
		blank_node->prepara=blank_node;
	}
	(*insert_place)=blank_node;
}


//Debug 用于定时停车等，放在识别之后，控制之前
void Debug_Check(void)
{
	extern float Debug;
	extern float Debug_time;
	extern uint8_t start_flag;
	extern uint8_t F_Mid_line_flag;
	extern struct system System;
	
	static char Debug_start=0;
	static float Debug_start_time=0;
	if(Debug<1)return;//不是Debug模式，直接退出
	if(start_flag==1&&Debug_start==0)
	{
		Debug_start=1;
		Debug_start_time=System.time;
	}else if(start_flag==0&&Debug_start==1)
	{
		Debug_start=0;
	}else if(start_flag>=1&&Debug_start==1)
	{
		if(System.time-Debug_start_time>Debug_time)
		{
			start_flag=2;
			F_Mid_line_flag=0;
			Can_Send(Speed_ID,0);
		}
	}
}


void Car_Start(void)
{
	extern uint8_t start_flag;
	extern uint8_t Beacon_cnt;
	extern uint8_t Pre_Beacon_cnt ;
	extern int ObsBeacon_cnt;
	start_flag=0;//为0表示准备开始比赛
	Can_Send(Speed_ID,0);
	Route_Init();
	System_data_reset();
	Pre_Beacon_cnt =0;
	Beacon_cnt=0;
	ObsBeacon_cnt=-1;
}

void Car_Stop(void)
{
	extern uint8_t start_flag;
	extern int8_t Dir_Preset[30];
	extern uint8_t Beacon_cnt;
	extern uint8_t Pre_Beacon_cnt ;
	extern float Servo_MAX;
	extern float Servo_MIN;;
	start_flag=2;//为2表示跑完设定信标数，比赛结束
	Can_Send(Speed_ID,0);
	if(Dir_Preset[Beacon_cnt]==0||Dir_Preset[Beacon_cnt]==2||Dir_Preset[Beacon_cnt]==4)FTM_PWM_ChangeDuty(HW_FTM1,1,Servo_MIN);
	else if(Dir_Preset[Beacon_cnt]==1||Dir_Preset[Beacon_cnt]==3||Dir_Preset[Beacon_cnt]==5)FTM_PWM_ChangeDuty(HW_FTM1,1,Servo_MAX);
	Pre_Beacon_cnt =0;
	Beacon_cnt=0;
}

extern struct system System;//系统相关变量
void System_data_reset(void)
{
	Can_Send(System_ID,-1);
	System.distance=0;
	System.time=0;
}


