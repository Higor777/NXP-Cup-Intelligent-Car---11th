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

float Temp_Para=450;

extern reg_s ov7725_eagle_reg[];
extern float Threshold;

ParameterNode_Type P_Ctrl[15];

void ParameterList_Init(void)
{
	ParameterNode_Type node;
	ParameterNode_Type *blank_node;
	ParameterList_Type insert_place=0;
	
	blank_node=P_Ctrl;
	
//	extern float CCD_Threshold;
//	node.exterdata=&(CCD_Threshold);
//	node.step=1;
//	sprintf(node.name,"CCD_Thre"); 
//	Insert_ParameterNode(&node,&insert_place,blank_node++);
	
	extern float Threshold;
	node.exterdata=&(Threshold);
	node.step=1;
	sprintf(node.name,"Threshold"); 
	Insert_ParameterNode(&node,&insert_place,blank_node++);
	
	extern float Speed_P;
	node.exterdata=&(Speed_P);
	node.step=10;
	sprintf(node.name,"Speed_P");
	Insert_ParameterNode(&node,&insert_place,blank_node++);
	
	extern float Speed_I;
	node.exterdata=&(Speed_I);
	node.step=5;
	sprintf(node.name,"Speed_I");
	Insert_ParameterNode(&node,&insert_place,blank_node++);
	
	extern float Speed_D;
	node.exterdata=&(Speed_D);
	node.step=5;
	sprintf(node.name,"Speed_D");
	Insert_ParameterNode(&node,&insert_place,blank_node++);
	
	extern float I_Domin;
	node.exterdata=&(I_Domin);
	node.step=0.01;
	sprintf(node.name,"I_Domin");
	Insert_ParameterNode(&node,&insert_place,blank_node++);
	
	extern float CCD_RMAX;
	node.exterdata=&(CCD_RMAX);
	node.step=1;
	sprintf(node.name,"CCD_RMAX");
	Insert_ParameterNode(&node,&insert_place,blank_node++);
	
	extern float CCD_LMIN;
	node.exterdata=&(CCD_LMIN);
	node.step=1;
	sprintf(node.name,"CCD_LMIN");
	Insert_ParameterNode(&node,&insert_place,blank_node++);
	
	extern float CCD_ShowBase;
	node.exterdata=&(CCD_ShowBase);
	node.step=1;
	sprintf(node.name,"CCD_Base");
	Insert_ParameterNode(&node,&insert_place,blank_node++);


	
	
	
	Show_Parameter(P_Ctrl,0x01|0x02|0x04);
}

void Data_Init(void)
{
	/*不可调参数*/
    ;
}

void Parameter_Change(void)
{
	uint8_t keysta,m_encoder_sta=0;
	keysta=Key_STA();
	if(M_Encoder_STA&M_Encoder_STA_Update_MASK)
	{
		m_encoder_sta=M_Encoder_STA&(~M_Encoder_STA_Update_MASK);
		M_Encoder_STA=0;
	}
	static ParameterNode_Type* para=P_Ctrl;
	
	if(keysta&Key_1_MASK)
	{
		para=para->prepara;
		Show_Parameter(para,0x01|0x02|0x04);
		
	}
	
	if(keysta&Key_2_MASK)
	{
		para=para->nextpara;
		Show_Parameter(para,0x01|0x02|0x04);
	}
	
	if(keysta&Key_3_MASK)
	{
		//(para->step)++;
		//Show_Parameter(para,0);
		extern uint8_t uart_img_flag;
		uart_img_flag=1-uart_img_flag;
//		if(uart_img_flag)OLED_Print(80,4,"ON ");
//		else OLED_Print(80,4,"OFF");
	}
	
	if(keysta&Key_4_MASK)
	{
		//(para->step)--;
		//Show_Parameter(para,0);
//		extern uint8_t RightAngle_Mode;
//		RightAngle_Mode=0;
//		extern uint8_t Begin_Pos;
//		Begin_Pos=64;
//		extern uint8_t Track_Width;
//		Track_Width=100;
//		
		
//    extern uint8_t oled_ccd_flag;
//		oled_ccd_flag=1-oled_ccd_flag;
//		if(oled_ccd_flag==0)
//		{
//			OLED_Fill(0x00); 
//			Show_Parameter(para,0x01|0x02|0x04);
//		}
		
		extern uint8_t oled_img_flag;
		oled_img_flag=1-oled_img_flag;
		if(oled_img_flag==0)
		{
			OLED_Fill(0x00); 
			Show_Parameter(para,0x01|0x02|0x04);
		}
		
		
		
	}
	
	if(m_encoder_sta&M_Encoder_CCW_MASK)
	{
		*(para->exterdata)-=para->step;
		Show_Parameter(para,0x02);
		//调节摄像头阈值
//		if(!strcmp(para->name,"Threshold"))
//		{
//			while(0==SCCB_WriteByte(OV7725_CNST,(uint8_t)Threshold));
//		}
	}
	
	if(m_encoder_sta&M_Encoder_CW_MASK)
	{
		
		*(para->exterdata)+=para->step;
		Show_Parameter(para,0x02);
		//调节摄像头阈值
//		if(!strcmp(para->name,"Threshold"))
//		{
//			while(0==SCCB_WriteByte(OV7725_CNST,(uint8_t)Threshold));
//		}
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

