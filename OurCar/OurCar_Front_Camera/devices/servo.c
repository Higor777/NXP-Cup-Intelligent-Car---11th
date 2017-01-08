#include "common.h"
#include "servo.h"
#include "ftm.h"

struct dPID d_PID;

#if C11
float Servo_MID = 1468;//1325;//1390          // 舵机中值
float Servo_MAX =1680  ;//1520;//1580          // 舵机最大值
float Servo_MIN = 1259;//1130;//1200          // 舵机最小值
float Servo_Error = 300;
#else
float Servo_MID = 1410;//1390          // 舵机中值
float Servo_MAX = 1640;//1580          // 舵机最大值
float Servo_MIN = 1170;//1200          // 舵机最小值
float Servo_Error = 240;
#endif

//舵机PID参数
float Direct_P=4.7;
float Direct_I=0.0;
float Direct_D=10.7;
	
void Servo_Init(uint32_t freq)
{
FTM_PWM_QuickInit(FTM1_CH1_PA09, kPWM_EdgeAligned, freq);
FTM_PWM_ChangeDuty(HW_FTM1,1,Servo_MID);
	

}

void Servo_MID_Set(void)
{
	
}

 
//PID初始化
void dPIDInit( struct dPID *rPID ) {

  rPID->SetPoint = 0 ;   
  rPID->NowPoint = 0 ;   
  
  rPID->Kp = 0 ;  
  rPID->Kd = 0 ;
    
  rPID->LastError = 0 ; //Error[-1]
     
  rPID->PreD = 0 ;      //位置式PID，输出计算后的位置值     
}






  //根据偏差设定PID
  void Set_ServoParameter(struct dPID *pp_d) {
      pp_d->Kp = Direct_P;
      pp_d->Kd = Direct_D;
  }

                    
	
	
extern float ex_mid;

    //角度控制
void AngleCtl( struct dPID *pp, int16_t rMid_Line ) {  
    uint16_t rDirection;
    //设定参数
    Set_ServoParameter( pp );
    //设定目标为中线值 修正后为0
    pp->SetPoint =ex_mid ;
    //实际方向对应的中线值
    pp->NowPoint = rMid_Line ; 
    //进行PID运算
    //加减根据正向或反向行驶确定
    rDirection = Servo_MID - dPIDCalc ( pp ) ;
    //舵机限幅
    if( rDirection > Servo_MAX )  rDirection = Servo_MAX ;
    else if( rDirection < Servo_MIN )  rDirection = Servo_MIN ; 
		
    FTM_PWM_ChangeDuty(HW_FTM1,1,rDirection);
  }
  
	
	
//----------------------------------------------------------------------------------
//舵机，位置式PID，输出总的PWM

int16_t dPIDCalc( struct dPID *d_pp ) 
{
  //定义当前偏差、偏差的微分
  int16_t dError,Error;
  //计算当前偏差
  Error = d_pp->SetPoint - d_pp->NowPoint;        //偏差
  //偏差微分
  dError = Error - d_pp->LastError;             //微分
  d_pp->LastError = Error;
  //计算位置PID
  d_pp->PreD = (int32_t) (d_pp->Kp * Error
                    + d_pp->Kd * dError) ;
  //返回位置PID 
  if(d_pp->PreD >  Servo_Error)d_pp->PreD =  Servo_Error;
  if(d_pp->PreD < 0 - Servo_Error)d_pp->PreD = 0 - Servo_Error;
  
  return  (int16_t)d_pp->PreD ;
}




