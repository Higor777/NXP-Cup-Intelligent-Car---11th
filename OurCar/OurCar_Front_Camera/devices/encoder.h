#ifndef __ENCODER_H
#define __ENCODER_H

#include <stdint.h>

typedef enum
{
	LeftMotor,
	RightMotor,
}Encoder_WhichMotor_Type;

typedef struct
{
	int value;
	uint8_t direction;
}Encoder_Speed_Type;

void Encoder_Init(void);
void Encoder_GetPulse(Encoder_WhichMotor_Type dir);

#endif
