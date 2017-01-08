#ifndef __UMC_H_
#define	__UMC_H_

#include <stdint.h>
#include <stdbool.h>

#define QUEUE_SIZE		9600     //队列长度
#define UART_REC_LEN	200		//串口接收数据长度

//串口数据归属标记
#define UM_UART_DATA		0x01U
#define USMART_UART_DATA	0x02U
#define NONE_UART_DATA		0x00U

//上下位机通讯协议
#define OP1_BYTE	0xFFU
#define OP2_BYTE	0x01U
#define OP3_BYTE	0xCCU
#define END1_BYTE	0xEEU
#define END2_BYTE	0xDDU
#define CCD_DATA_SEL		0x01U
#define CCD_DATA_SEL_LEN	128

//构造队列
typedef struct{
	uint8_t arr[QUEUE_SIZE];
	uint16_t head, tail;
}Queue;

//队列状态
typedef enum
{
    Queue_Empty,
    Queue_Full,
    Queue_NotFull
}Queue_State_Type;

//接收状态
typedef enum
{
    RECOP1,		//起始字节1
	RECOP2,		//起始字节2
	RECOP3,		//起始字节3
	RECLEN1,	//长度字节1
	RECLEN2,	//长度字节2
	RECSEL,		//数据字节
	RECCHECK,	//校验字节
	RECEND1,	//结束字节1
	RECEND2		//结束字节2
}Recstate_Type;

//数据接收状态
typedef enum
{
    USMART_Receiver,	//在处理USMART数据
	UM_Receiver,		//在处理上位机数据
	ResetReceiver		//复位状态机
}ReceiverSTA_Type;

////串口发送数据指针
//typedef struct
//{
//	uint8_t *head;
//	uint8_t *tail;
//}UART_TX_DATA_Type;

//extern Queue UART_RX_Queue;		//串口接收队列
extern uint8_t UART_RX_DATA[UART_REC_LEN];	//串口接收的数据
extern uint16_t UART_RX_STA[2];		//串口数据状态，第一个记录数据长度，第二个记录数据归属
//extern UART_TX_DATA_Type UART_TXDATA;


static void Queue_Init(Queue * pQueue);
static uint8_t Queue_Get(Queue * pQueue);
static void Queue_Put(Queue * pQueue, uint8_t ch);
static Queue_State_Type Queue_State(Queue * pQueue);
static void UART_RX_ISR(uint16_t byteRec);
static void UART_TX_ISR(uint16_t * byteToSend);
//void UM_DATA_Receiver(ReceiverSTA_Type *pReceiver_STA, uint8_t data);
//void USMART_DATA_Receiver(ReceiverSTA_Type *pReceiver_STA, uint8_t data);
static void Reset_Receiver(ReceiverSTA_Type *pReceiver_STA);

void UART_Send_DATA(uint32_t instance,uint8_t *data, uint16_t len);
void UART_DATA_Processor(void);
void UMC_Init(void);
void UM_Upload_Data(uint32_t instance);
void SendImageData(uint8_t * ImageData);
void UART_Printf(uint8_t *data);

void sendimg(uint8_t *imgaddr,uint16_t len);
void sendccd(unsigned char * ImageData);
#endif
