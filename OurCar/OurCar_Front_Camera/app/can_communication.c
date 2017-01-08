#include "can.h"
#include "can_communication.h"
#include "motor.h"
#include "servo.h"
#include "ftm.h"

void CAN_ISR(void);
void CAN_Message_Handle(CAN_Message_Type* CAN_Mes);

/*********************************************
* CAN初始化
*********************************************/
void Can_Init(void)
{
    CAN_QuickInit(CAN1_TX_PE24_RX_PE25, 125*1000);
    /* FIFO deep is 0+6 = 6 
    Once FIFO is enabled, MB0-5 cannot used as normal MB, MB0-5 used as RxFIFO and they automaically configed as Rx MB 
    */
    CAN_SetRxFIFO(HW_CAN1);
    CAN_CallbackInstall(HW_CAN1, CAN_ISR);
    CAN_ITDMAConfig(HW_CAN1, CAN_RX_FIFO_MB, kCAN_IT_RX);
}

/*********************************************
* CAN发送一帧数据
*ID : 数据帧的内容ID
*data : 数据内容
*********************************************/
void Can_Send(uint8_t ID,float data)
{
	extern float Speed_EX;
	uint8_t str[8];
	str[0] = ID;
	str[4] = *((uint8_t *)(&data));
	str[5] = *((uint8_t *)(&data)+1);
	str[6] = *((uint8_t *)(&data)+2);
	str[7] = *((uint8_t *)(&data)+3);
	CAN_WriteData(HW_CAN1, CAN_TX_MB, CAN_TX_ID, (uint8_t *)str, 8); 
	if(ID==Speed_ID)Speed_EX=data;
}

/*********************************************
* CAN发送主动差速信息
*ID : 数据帧的内容ID
*data  : 左轮速度
*data1 : 右轮速度
*********************************************/
void Can_SendDiffSpeed(uint8_t ID,float data,float data1)
{
	extern float Speed_EX;
	uint8_t str[8];
	str[0] = ID;
	str[4] = (uint8_t)data;
	str[5] = ((uint8_t)data*10)%10;
	str[6] = (uint8_t)data1;
	str[7] = ((uint8_t)data1*10)%10;
	CAN_WriteData(HW_CAN1, CAN_TX_MB, CAN_TX_ID, (uint8_t *)str, 8); 
	Speed_EX=2.0;
}

/*********************************************
* CAN接收中断
*********************************************/
CAN_Message_Type CAN_Message;

void CAN_ISR(void)
{
    /* FIFO */
    if(CAN_IsRxFIFOEnable(HW_CAN1))
    {
        if(CAN1->IFLAG1 & (1 << CAN_RX_FIFO_MB))
        {
            if(CAN_ReadFIFO(HW_CAN1, &CAN_Message.id, CAN_Message.data, &CAN_Message.len) == 0)
            {

               CAN_Message_Handle(&CAN_Message);
            }
        }
    }
}


extern float Speed_EX;
extern float Obstacle_Flag;
extern uint8_t Obst_Dir_flag;
extern float X_Thre;
extern float CarSpeed;
extern struct system System;//系统相关变量
float Diff_PWM=0,Diff_PWM1;
uint8_t Obst_Dir=0;//记录方向，用于之后转向
/*********************************************
* CAN消息处理
*********************************************/
void CAN_Message_Handle(CAN_Message_Type* CAN_Mes)
{
	float MOTOR_PWM_t;
	if(CAN_Mes->id != ((((uint32_t)(((uint32_t)(CAN_RX_ID))<<CAN_ID_STD_SHIFT))&CAN_ID_STD_MASK)))return;
	switch (CAN_Mes->data[0])
	{
		case Speed_ID :
			MOTOR_PWM_t =  *((float *)(CAN_Mes->data+4));
		if(Speed_EX>=3.8)Motor_PWM(PWM_MAX,PWM_MAX);
		else Motor_PWM((int32_t)MOTOR_PWM_t,(int32_t)MOTOR_PWM_t);
			break;
		case Obstacle_ID :
			Obstacle_Flag =  *((float *)(CAN_Mes->data+4));
			if(Obstacle_Flag>64)
			{
				//Motor_PWM(0,0);
				//Can_Send(Speed_ID,0);
				//FTM_PWM_ChangeDuty(HW_FTM1,1,Servo_MIN);
				Obst_Dir=1;
				Obst_Dir_flag = 0;
			}else if (Obstacle_Flag<64&&Obstacle_Flag>0)
			{
				//Motor_PWM(0,0);				
				//Can_Send(Speed_ID,0);
				//FTM_PWM_ChangeDuty(HW_FTM1,1,Servo_MAX);
				Obst_Dir=2;
				Obst_Dir_flag = 1;
			}else
			{
				//Can_Send(Speed_ID,Speed_EX);
				Obst_Dir=0;
			}
			break;
		case X_Thre_ID://接收由后摄像头计算的X_Thre
			X_Thre = *((float *)(CAN_Mes->data+4));
			break;
		case R_Speed_ID://接收后摄像头采集的实时速度
			CarSpeed = *((float *)(CAN_Mes->data+4));
			Choose_X_Thre(CarSpeed);
			break;
		case distance_ID://接收积分的里程长度
			System.distance = *((float *)(CAN_Mes->data+4));
			break;
		case DiffSpeed_ID://接收主动差速PWM
			if(Speed_EX>=4.0)Motor_PWM(PWM_MAX,PWM_MAX);
			else Motor_PWM(*((uint16_t *)(CAN_Mes->data+4)),*((uint16_t *)(CAN_Mes->data+6)));
			break;
		default:
			break;
	}
}

//#if C1 

float X_Thre_adj=1;
void Choose_X_Thre(float speed)
{
	if(speed<1.62)
	{
		X_Thre=37.0;
	}else if(speed<1.75)
	{
		X_Thre=36.0;
	}else if(speed<1.93)
	{
		X_Thre=35.0;
	}else if(speed<2.19)
	{
		X_Thre=34.0;
	}else if(speed<2.47)
	{
		X_Thre=33.0;
	}else if(speed<2.75)
	{
		X_Thre=32.0;	
	}else if(speed<3.39)
	{
		X_Thre=31.0;
	}else if(speed<3.64)
	{
		X_Thre=30.0;
	}else if(speed<3.73)
	{
		X_Thre=29.0;
	}else if(speed<4.03)
	{
		X_Thre=28.0;
	}else if(speed<4.67)
	{
		X_Thre=27.0;
	}else if(speed<4.99)
	{
		X_Thre=26.0;
	}else if(speed<5.05)
	{
		X_Thre=25.0;
	}else if(speed<5.19)
	{
		X_Thre=24.0;
	}else if(speed<5.37)
	{
		X_Thre=23.0;
	}else
	{
		X_Thre=22.0;
	}
	X_Thre-=X_Thre_adj;
}


//void Choose_X_Thre(float speed)
//{
//	if(speed<1.0)
//	{
//		X_Thre=36.0;
//	}else if(speed<2.0)
//	{
//		X_Thre=35.0;
//	}else if(speed<2.3)
//	{
//		X_Thre=34.0;
//	}else if(speed<2.6)
//	{
//		X_Thre=33.0;
//	}else if(speed<3.1)
//	{
//		X_Thre=32.0;	
//	}else if(speed<3.2)
//	{
//		X_Thre=31.0;
//	}else if(speed<3.5)
//	{
//		X_Thre=30.0;
//	}else if(speed<3.6)
//	{
//		X_Thre=29.0;
//	}else if(speed<3.8)
//	{
//		X_Thre=28.0;
//	}else if(speed<4.0)
//	{
//		X_Thre=27.0;
//	}else if(speed<4.1)
//	{
//		X_Thre=26.0;
//	}else if(speed<4.3)
//	{
//		X_Thre=25.0;
//	}else if(speed<4.6)
//	{
//		X_Thre=24.0;
//	}else
//	{
//		X_Thre=23.0;
//	}
//}

//#else
//void Choose_X_Thre(float speed)
//{
//	if(speed<2.5)
//	{
//		X_Thre=30.0;
//	}else if(speed<2.9)
//	{
//		X_Thre=29.0;
//	}else if(speed<3.2)
//	{
//		X_Thre=28.0;
//	}else if(speed<3.6)
//	{
//		X_Thre=27.0;
//	}else if(speed<4.3)
//	{
//		X_Thre=26.0;
//	}else if(speed<5.0)
//	{
//		X_Thre=25.0;
//	}else if(speed<5.3)
//	{
//		X_Thre=24.0;
//	}else
//	{
//		X_Thre=23.0;
//	}
//}

//#endif



