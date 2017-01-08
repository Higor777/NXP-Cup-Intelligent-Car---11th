#ifndef __ENCODER_H
#define __ENCODER_H

#include <stdint.h>

//两次编码器之间的最大差值
#define  MAX_DIFF  1111


typedef enum
{
	LeftMotor,
	RightMotor,
}Encoder_WhichMotor_Type;

typedef struct
{
	int value;
	int last_value;
	uint8_t direction;
}Encoder_Speed_Type;

void Encoder_Init(void);
void Encoder_GetPulse(Encoder_WhichMotor_Type dir);

#endif
