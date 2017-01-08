#ifndef   __GETLINE_H__
#define   __GETLINE_H__

#include "stdlib.h"
#include "stdint.h"


struct Beacon{
	uint8_t x;
	uint8_t y;
};



uint8_t Get_Beacon(void);


void NoLost(int temp1, int temp2);
void Lost(int temp1, int temp2);
void GetLine(void );
void img_XOR(uint8_t * img0,uint8_t * img1,uint16_t Pixelcnt);
void Mark_Connect(uint8_t  pic[60][80],int x, int y);
void Connect(int x, int y, uint8_t isMarked);
void GetBeacon(uint8_t Mat[60][80],struct Beacon * DstBeacon);
void Beacon_CNT(void);
#endif    



