#ifndef __SERVO_H__
#define __SERVO_H__


#include <stdint.h>

#define mid 30 
#define Servo_MID  880          // 舵机中值
#define Servo_MAX  1040          // 舵机最大值
#define Servo_MIN  720          // 舵机最小值
#define Servo_Error 160


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

