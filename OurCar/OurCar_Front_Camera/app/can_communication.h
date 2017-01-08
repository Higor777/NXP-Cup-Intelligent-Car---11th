#ifndef __CAN_COMMUNICATION_H__
#define __CAN_COMMUNICATION_H__

#include "stdlib.h"
#include "stdint.h"
#define CAN_TX_MB   12
#define CAN_TX_ID   0x01      //发送ID
#define CAN_RX_ID   0x02      //接收ID

#define Speed_ID     0x01//发送接收控速信息
#define Obstacle_ID  0x02//接收障碍信息
#define X_Thre_ID    0x03//接收计算的X_Thre
#define R_Speed_ID   0x04//接收采集的实时速度
#define distance_ID  0x05//接收积分的里程长度
#define System_ID    0x06//系统相关操作 data<0数据清零
#define DiffSpeed_ID 0x07//两个轮子单独闭环主动差速

typedef struct 
{
	  uint8_t data[8];
    uint8_t len;
    uint32_t id;
}CAN_Message_Type;




void Can_Init(void);
void Can_Send(uint8_t ID,float data);
void Choose_X_Thre(float speed);
void Can_SendDiffSpeed(uint8_t ID,float data,float data1);
#endif



