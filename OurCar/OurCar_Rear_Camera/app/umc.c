#include "common.h"
#include "UMC.h"
#include "uart.h"
#include "usmart.h"
#include <string.h>




Queue UART_RX_Queue;	//串口接收队列
Queue UART_TX_Queue;	//串口发送队列

uint8_t UART_RX_DATA[UART_REC_LEN]; //串口接收的数据
uint16_t UART_RX_STA[2];	//串口数据状态，第一个记录数据长度，第二个记录数据归属

//复位状态机函数
//void Reset_Receiver(ReceiverSTA_Type *pReceiver_STA);



//上位机数据接收状态机
static void UM_DATA_Receiver(ReceiverSTA_Type *pReceiver_STA, uint8_t data)
{
	static Recstate_Type RECSTA=RECOP1;
	if(*pReceiver_STA==ResetReceiver)
	{
		RECSTA=RECOP1;
	}
		
	//处理队列数据
	static uint8_t checksum=0;
	static uint16_t datalen=0;
	
	switch(RECSTA)
	{
		case RECOP1:
		{
			if(data==OP1_BYTE)
			{
				RECSTA=RECOP2;
				checksum=0;
			}
			break;
		}
		case RECOP2:
		{
			if(data==OP2_BYTE)
			{
				RECSTA=RECOP3;
			}
			else if(data==OP1_BYTE) RECSTA=RECOP2;
			else RECSTA=RECOP1;
			break;
		}
		case RECOP3:
		{
			if(data==OP3_BYTE)
			{
				RECSTA=RECLEN1;
				*pReceiver_STA=UM_Receiver;
				Reset_Receiver(pReceiver_STA);	//复位其他状态机
			}
			else if(data==OP1_BYTE) RECSTA=RECOP2;
			else RECSTA=RECOP1;
			break;
		}
		case RECLEN1:
		{
			checksum+=data;
			datalen=(uint16_t)data<<8;
			RECSTA=RECLEN2;
			break;
		}
		case RECLEN2:
		{
			checksum+=data;
			datalen|=data;
			RECSTA=RECSEL;
			UART_RX_STA[0]=0;
			if(datalen>UART_REC_LEN)	//数据接收错误
			{
				RECSTA=RECOP1;
				*pReceiver_STA=USMART_Receiver;
				UART_RX_STA[0]=0;
			}
			break;
		}
		case RECSEL:
		{
			UART_RX_DATA[UART_RX_STA[0]++]=data;
			checksum+=data;
			if(UART_RX_STA[0]>=datalen)		//接收完数据
			{
				RECSTA=RECCHECK;
			}
			break;
		}
		case RECCHECK:
		{
			if(checksum==data)
			{
				RECSTA=RECEND1;
			}
			else
			{
				//数据校验错误
				if(data==OP1_BYTE) RECSTA=RECOP2;
				else RECSTA=RECOP1;
				*pReceiver_STA=USMART_Receiver;
				UART_RX_STA[0]=0;
			}
			break;
		}
		case RECEND1:
		{
			if(data==END1_BYTE) RECSTA=RECEND2;
			else
			{
				//数据接收错误
				if(data==OP1_BYTE) RECSTA=RECOP2;
				else RECSTA=RECOP1;
				*pReceiver_STA=USMART_Receiver;
				UART_RX_STA[0]=0;
			}
			break;
		}
		case RECEND2:
		{
			if(data==END2_BYTE)
			{
				//数据接收成功
				UART_RX_STA[1]=UM_UART_DATA;	//标记数据归属
				RECSTA=RECOP1;
				*pReceiver_STA=USMART_Receiver;
			}
			else
			{
				//数据接收错误
				if(data==OP1_BYTE) RECSTA=RECOP2;
				else RECSTA=RECOP1;
				*pReceiver_STA=USMART_Receiver;
				UART_RX_STA[0]=0;
			}
		}
	}
}

//USMART数据接收状态机
static void USMART_DATA_Receiver(ReceiverSTA_Type *pReceiver_STA, uint8_t data)
{
	static Recstate_Type RECSTA=RECSEL;
	if(*pReceiver_STA==ResetReceiver)
	{
		RECSTA=RECSEL;
	}
	
	switch(RECSTA)
	{
		case RECSEL:
		{
			if(data==0x0DU) RECSTA=RECEND1;
			else 
			{
				if(UART_RX_STA[0]>=UART_REC_LEN-1) //留一个空余
				{
					//数据已满,接收错误
					UART_RX_STA[0]=0;
				}
				UART_RX_DATA[UART_RX_STA[0]++]=data;
			}
			break;
		}
		case RECEND1:
		{
			if(data==0x0AU)
			{
				//接收完成
				UART_RX_STA[1]=USMART_UART_DATA;	//标记数据归属
				RECSTA=RECSEL;
			}
			else
			{
				//接收错误
				UART_RX_STA[0]=0;
				UART_RX_DATA[UART_RX_STA[0]++]=data;
				RECSTA=RECSEL;
			}
		}
		default :
			;
	}
}

//复位状态机函数
static void Reset_Receiver(ReceiverSTA_Type *pReceiver_STA)
{
	ReceiverSTA_Type RESET=ResetReceiver;
	if(*pReceiver_STA!=UM_Receiver)
	{
		UM_DATA_Receiver(&RESET,0x00U);
	}
	if(*pReceiver_STA!=USMART_Receiver)
	{
		USMART_DATA_Receiver(&RESET,0x00U);
	}
}

//接收队列处理函数
void UART_DATA_Processor(void)
{
	while((Queue_State(&UART_RX_Queue)!=Queue_Empty) &&
		  (UART_RX_STA[1]==NONE_UART_DATA))
	{
		static ReceiverSTA_Type Receiver_STA;
		uint8_t data;
		
		data=Queue_Get(&UART_RX_Queue);
		switch(Receiver_STA)
		{
			case USMART_Receiver:
				UM_DATA_Receiver(&Receiver_STA,data);
				USMART_DATA_Receiver(&Receiver_STA,data);
				break;
			case UM_Receiver:
				UM_DATA_Receiver(&Receiver_STA,data);
				break;
			default :
				;
		}
	}
}




/******************************************************
*串口初始化
******************************************************/
void UMC_Init(void)
{
	//此串口用于发送数据到上位机
	Queue_Init(&UART_RX_Queue);
	UART_QuickInit(UART4_RX_PC14_TX_PC15, 115200);
	UART_CallbackRxInstall(HW_UART4, UART_RX_ISR);
	Queue_Init(&UART_TX_Queue);
	UART_CallbackTxInstall(HW_UART4, UART_TX_ISR);
	//需要发送数据时开启
	//UART_ITDMAConfig(HW_UART0, kUART_IT_Tx, true);
	//usmart_init(0);
	
	//此串口用于发送后摄像头的相关数据
	//UART_QuickInit(UART4_RX_PC14_TX_PC15, 115200);


	
}




//以下是串口处理函数
/***************************************我是分割线***************************************/

static void Queue_Init(Queue * pQueue)
{
	memset(pQueue, 0, sizeof(Queue));
}

static uint8_t Queue_Get(Queue * pQueue)
{
	uint8_t data = pQueue->arr[pQueue->head];
	pQueue->head = (pQueue->head + 1) % QUEUE_SIZE;
	return data;
}

static void Queue_Put(Queue * pQueue, uint8_t ch)
{
	pQueue->arr[pQueue->tail] = ch;
	pQueue->tail = (pQueue->tail + 1) % QUEUE_SIZE;
}

static Queue_State_Type Queue_State(Queue * pQueue)
{
	if (pQueue->head == pQueue->tail)
		return Queue_Empty;
	else if ((pQueue->tail + 1) % QUEUE_SIZE == pQueue->head)
		return Queue_Full;
	else
		return Queue_NotFull;
}

//串口接收中断
static void UART_RX_ISR(uint16_t byteRec)
{
    if(Queue_State(&UART_RX_Queue)==Queue_Full)
	{
		//队列溢出处理
	}
	else
	{
		Queue_Put(&UART_RX_Queue,(uint8_t)byteRec);
	}
}

//串口发送中断
//串口发送中断
static void UART_TX_ISR(uint16_t * byteToSend)
{
	
	*byteToSend=(uint16_t)Queue_Get(&UART_TX_Queue);
	if(Queue_State(&UART_TX_Queue)==Queue_Empty)
	{
		UART_ITDMAConfig(HW_UART0, kUART_IT_Tx, false);
	}
}





//串口数据发送函数
void UART_Send_DATA(uint32_t instance,uint8_t *data, uint16_t len)
{
	uint16_t i;
	
//	for(i=0;i<len;i++)
//	{
//		Queue_Put(&UART_TX_Queue,data[i]);
//		if(Queue_State(&UART_TX_Queue)==Queue_Full)
//		{
//			break;
//		}
//	}
//	
	if(0)
	{
		//开启了DMA发送数据
	}
	else if(0)
	{
		//使用中断发送数据
		UART_ITDMAConfig(instance, kUART_IT_Tx, true);
	}
	else
	{
//		while(Queue_State(&UART_TX_Queue)!=Queue_Empty)
//		{
//			UART_WriteByte(instance,(uint16_t)Queue_Get(&UART_TX_Queue));
//		}
//	

		
	for(i=0;i<len;i++)
	{
	  UART_WriteByte(instance,(uint16_t)data[i]);
		
	}
	
	
	
	}
}


/*!
 *  @brief      发送图像到eSmartCameraCar上位机显示
 *  @param      imgaddr         图像地址
 *  @param      imgsize         图像占用空间大小
 *  @since      v5.0
 *  @note       不同的上位机，不同的命令，这里使用 eSmartCameraCar软件，
                如果使用其他上位机，则需要修改代码。
 *  Sample usage:   sendimg(imgbuff, CAMERA_W * CAMERA_H);                    //发送到上位机
 */

#define UART_PORT HW_UART4
void sendimg(uint8_t *imgaddr,uint16_t len)
{
 #define CMD_IMG     1
    uint8 cmdf[2] = {CMD_IMG, ~CMD_IMG};    //山外上位机 使用的命令
    uint8 cmdr[2] = {~CMD_IMG, CMD_IMG};    //山外上位机 使用的命令
    UART_Send_DATA(UART_PORT, cmdf, 2);      //发送帧头命令
    UART_Send_DATA(UART_PORT, imgaddr, len); //再发送图像
    UART_Send_DATA(UART_PORT, cmdr, 2);      //发送帧尾命令
}



void sendccd(unsigned char * ImageData) 
{

    unsigned char i;
     char xieyi = 0;
    
    /* Send Data */
    xieyi=0x02;
   UART_WriteByte(UART_PORT,xieyi);
   xieyi=0xFD;
   UART_WriteByte(UART_PORT,xieyi);

    for(i=0; i<128; i++) 
   {
      UART_WriteByte(UART_PORT,*ImageData++);
    }

   UART_WriteByte(UART_PORT,xieyi);
   xieyi=0x02;
   UART_WriteByte(UART_PORT,xieyi);
}


