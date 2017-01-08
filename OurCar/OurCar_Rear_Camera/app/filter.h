#ifndef __FILTER_H
#define __FILTER_H

#include <stdint.h>
#define FILTER_BUFSIZE 10

typedef struct
{
	int32_t buf[FILTER_BUFSIZE];
	uint8_t pos;
}AverFilter_Type;

typedef struct
{
	float data[2];
}LP1Filter_Type;

extern AverFilter_Type AverFilter_Gyro_AR2,AverFilter_Acc_Z;

float AverFilter_getVal(AverFilter_Type *filter,int32_t current);
float LP1Filter_getVal(LP1Filter_Type *filter,float current,float a);
void KalmanFliter(float z_inc, float u_gyro);

#endif
