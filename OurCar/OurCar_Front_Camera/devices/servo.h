#ifndef __SERVO_H__
#define __SERVO_H__


#include <stdint.h>

#define EX_MID 40 

#define C11 1


 //舵机，位置式PID
  struct dPID            //定义数法核心数据
  {
    int16_t SetPoint;      //设定目标 desired value
    int16_t NowPoint;      //当前点数据
    
    double Kp;           //比例常数
    double Kd;           //微分常数
    
    int16_t LastError;      //Error[-1]
    
    int32_t PreD;           //输出位置
  };
 
	
void dPIDInit( struct dPID *rPID );
void AngleCtl( struct dPID *pp, int16_t rMid_Line );
int16_t dPIDCalc( struct dPID *d_pp ) ;


#endif

