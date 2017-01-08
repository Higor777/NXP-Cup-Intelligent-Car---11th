#ifndef   __GET_BEACON_H__
#define   __GET_BEACON_H__

#include <stdint.h>


struct Beacon{
	uint8_t x;
	uint8_t y;
	uint16_t pixelcnt;
};




uint8_t Get_Beacon(void);


void NoLost(int temp1, int temp2);
void Lost(int temp1, int temp2);
void GetLine(void );
void img_XOR(uint8 * img0,uint8 * img1,uint16 Pixelcnt);
void Mark_Connect(uint8  pic[60][80],int x, int y);
void Connect(int x, int y, uint8 isMarked);
void GetBeacon(uint8 Mat[60][80],struct Beacon * DstBeacon);
#endif    



