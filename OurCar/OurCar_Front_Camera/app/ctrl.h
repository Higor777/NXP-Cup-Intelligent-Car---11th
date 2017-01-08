#ifndef __CTRL_H
#define __CTRL_H

#include <stdint.h>


#define SPEED_RATIO (1000.0/(500.0*20))

typedef enum
{
	RSTA_blackline1,
	RSTA_blackline2,
	RSTA_findgap,
	RSTA_turning,
	RSTA_delay1,
	RSTA_delay2,
}RightAngle_State_Type;




void SetSpeedPI(int p,int i);
void SetMotorDead(int lval,int rval);


void SpeedCalculate(void);
void SpeedCtrl(void);
void DirectCtrl(void);


#endif
