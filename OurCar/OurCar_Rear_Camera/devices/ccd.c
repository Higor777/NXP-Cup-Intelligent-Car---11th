#include "common.h"
#include "gpio.h"
#include "adc.h"
#include "ccd.h"
#include "can.h"
#include "can_communication.h"
#include "oled.h"
#include "stdio.h"
uint32_t IntegTime=10;
uint8_t CCD_Data[128]={0};

float CCD_Threshold = 106;

void Delay100Ns(uint8_t ns)
{
	for(uint8_t i=0;i<ns;i++)
	{
		//asm("nop");
		__nop();
	}
}

void CCD_Init()
{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.instance=HW_GPIOE;
	GPIO_InitStruct.mode=kGPIO_Mode_OPP;
	GPIO_InitStruct.pinx=5;
	GPIO_Init(&GPIO_InitStruct);	//CCD1_CLK
	GPIO_InitStruct.pinx=7;
	GPIO_Init(&GPIO_InitStruct);	//CCD1_SI
	
	ADC_QuickInit(ADC1_SE7_PE3,kADC_SingleDiff8or9);	//CCD1_AO;
	CCD_Start();
}

void CCD_Start()
{
	CCD_SI_Set();//SI=1;
	Delay100Ns(2);
	CCD_CLK_Set();//CLK=1;
	Delay100Ns(2);
	CCD_SI_Reset();//SI=0;
	Delay100Ns(2);
	CCD_CLK_Reset();//CLK=0;
	
	for(uint8_t i=0;i<127;i++)
	{
		Delay100Ns(4);
		CCD_CLK_Set();
		Delay100Ns(4);
		CCD_CLK_Reset();
	}
	Delay100Ns(4);
	CCD_CLK_Set();
	Delay100Ns(4);
	CCD_CLK_Reset();
}

void CCD_ImageCapture(uint8_t ImageData[])
{
	uint8_t cnt=0;
	
	CCD_SI_Set();
	Delay100Ns(2);
	
	CCD_CLK_Set();
	Delay100Ns(2);
	
	CCD_SI_Reset();
	Delay100Ns(2);
	
	DelayUs(10);//第一个像素点样本，修改延时时间平滑图像
	
	ImageData[cnt]=(uint8_t)(ADC_QuickReadValue(ADC1_SE7_PE3));
	cnt++;
	CCD_CLK_Reset();
	for(uint8_t i=0;i<127;i++)
	{
		Delay100Ns(4);
		CCD_CLK_Set();
		Delay100Ns(4);
		
		ImageData[cnt]=(uint8_t)(ADC_QuickReadValue(ADC1_SE7_PE3));
		cnt++;
		CCD_CLK_Reset();
	}
	
	Delay100Ns(4);
	CCD_CLK_Set();
	Delay100Ns(4);
	
	CCD_CLK_Reset();
}

void CCD_IntegTime(const uint8_t ImageData[])
{
	uint32_t AverGreyLevel=0;
	uint32_t AverGreyLevel_EX=100;//期望采集均值，需要调节
	int32_t error=0;
	int32_t error_EX=3;//期望离差，需要调节
	
	for(int32_t i=0;i<128;i++)
	{
		AverGreyLevel+=(uint32_t)ImageData[i];
	}
	AverGreyLevel>>=7;
	
	error=AverGreyLevel_EX-AverGreyLevel;
	if(error<-error_EX || error>error_EX)
	{
		if(error<-10)
			error=-10;
		if(error>10)
			error=10;
		
		IntegTime+=(error/2);//P调节
	}
}

void CCD_Diff(uint8_t ImageData[],int16_t ImageDataDiff[])
{
	for(uint8_t i=1;i<128;i++)
	{
		ImageDataDiff[i-1]=ImageData[i]-ImageData[i-1];
	}
}


float CCD_RMAX=115;
float CCD_LMIN=13;
//检测障碍物
extern float Speed_EX;
void  Check_Obstacle(uint8 * ccd_data)
{
	char str[5];
	static uint8_t New_flag = 3;//用于没有扫到障碍时只发三次-1.0，减小通信压力
	float sum = 0;
	uint8_t cnt = 0;
	uint8_t i=0;
	for(i=10;i<118;i++)
	{
		if(*(ccd_data+i)>CCD_Threshold)
		{
			cnt++;
			sum+=i;
		}
	}
	if(cnt>0)
	{
		sum/=cnt;
	}
	sprintf(str,"%-3d",cnt);
	OLED_Print(80,0,(uint8_t*)str);
	if (sum>CCD_LMIN&&sum<CCD_RMAX&&cnt<15&&cnt>1)
	{
		New_flag = 3;
		//Speed_EX = 0;
		Can_Send(Obstacle_ID,sum);
	}else if(New_flag > 0)
	{
		New_flag --;
		Can_Send(Obstacle_ID,-1.0);
	}
}








