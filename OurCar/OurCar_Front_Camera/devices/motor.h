#ifndef __MOTOR_H
#define __MOTOR_H

#include <stdint.h>

//理论上是10000，驱动要求不大于98%
#define PWM_MAX  9700
#define PWM_MIN  0//-9700

void Motor_Init(uint32_t freq);
void Servo_Init(uint32_t freq);
void Motor_PWM(int32_t Left_PWM,int32_t Right_PWM);

#endif
