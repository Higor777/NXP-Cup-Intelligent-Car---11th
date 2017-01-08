#ifndef __MOTOR_H
#define __MOTOR_H

#include <stdint.h>

#define PWM_MAX 10000
#define PWM_MIN 0//-10000

void Motor_Init(uint32_t freq);
void Servo_Init(uint32_t freq);
void Motor_PWM(int32_t Left_PWM,int32_t Right_PWM);

#endif
