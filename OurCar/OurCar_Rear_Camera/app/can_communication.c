#include "can.h"
#include "can_communication.h"

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



/*********************************************
* CAN发送一帧数据
*ID : 数据帧的内容ID
*data : 数据内容
*********************************************/
void Can_Send(uint8_t ID,float data)
{
	uint8_t str[8];
	str[0] = ID;
	str[4] = *((uint8_t *)(&data));
	str[5] = *((uint8_t *)(&data)+1);
	str[6] = *((uint8_t *)(&data)+2);
	str[7] = *((uint8_t *)(&data)+3);
	CAN_WriteData(HW_CAN1, CAN_TX_MB, CAN_TX_ID, (uint8_t *)str, 8); 
}


extern float Speed_EX;
extern struct system System;//系统相关变量

extern float Speed_EXL;
extern float Speed_EXR;
extern float DiffSpeed_flag;
/*********************************************
* CAN消息处理
*********************************************/
void CAN_Message_Handle(CAN_Message_Type* CAN_Mes)
{
	if(CAN_Mes->id != ((((uint32_t)(((uint32_t)(CAN_RX_ID))<<CAN_ID_STD_SHIFT))&CAN_ID_STD_MASK)))return;
	switch (CAN_Mes->data[0])
	{
		case Speed_ID :
			Speed_EX = *((float *)(CAN_Mes->data+4));
			Speed_EXL = Speed_EX;
			Speed_EXR = Speed_EX;
			//DiffSpeed_flag=0;
			break;
		case System_ID :
			if(*((float *)(CAN_Mes->data+4))<0)
			{
				System.distance=0;
				System.time=0;
			}
			break;
		case DiffSpeed_ID :
			Speed_EXL=((float)*(CAN_Mes->data+4))+0.1*((float)*(CAN_Mes->data+5));
			Speed_EXR=((float)*(CAN_Mes->data+6))+0.1*((float)*(CAN_Mes->data+7));
			//DiffSpeed_flag=1;
			break;
		default:
			break;
	}
}




