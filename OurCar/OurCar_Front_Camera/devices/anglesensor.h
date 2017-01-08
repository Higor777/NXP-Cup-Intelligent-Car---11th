#ifndef __ANGLESENSOR_H
#define __ANGLESENSOR_H

#include <stdint.h>

#define   OSS 0	 //	BMP085使用

//L3G4200D内部寄存器
#define CTRL_REG1 0x20
#define CTRL_REG2 0x21
#define CTRL_REG3 0x22
#define CTRL_REG4 0x23
#define CTRL_REG5 0x24
#define OUT_X_L 0x28
#define OUT_X_H 0x29
#define OUT_Y_L 0x2A
#define OUT_Y_H 0x2B
#define OUT_Z_L 0x2C
#define OUT_Z_H 0x2D

//定义器件在IIC总线中的从地址,根据ALT  ADDRESS地址引脚不同修改
#define	HMC5883L_Addr   0x3C	//磁场传感器器件地址
#define	ADXL345_Addr    0xA6	//加速度传感器器件地址
#define	BMP085_Addr     0xee	//气压传感器器件地址
#define	L3G4200_Addr    0xD2	//陀螺仪传感器器件地址




/* 函数申明 -----------------------------------------------*/
void Delay(uint32_t nTime);
void Delayms(uint32_t m);  
void conversion(long temp_data);
void adxl345_angle(void);

void IMU_Init(void);

void  Init_ADXL345(void);
void  Init_BMP085(void);
void  Init_HMC5883L(void);
void Init_L3G4200D(void);
void read_L3G4200D(void);
void read_BMP085(void);
void read_ADXL345(void);
void read_hmc5883l(void);
void Send_L3G4200D(void);
void  Send_ADXL345(void);
void  Send_BMP085(void);
void Send_HMC5883L(void);
void  Send_L3G420D_data(short dis_data);
void Send_ADXL345_data(int dis_data);
long bmp085ReadPressure(void);
long bmp085ReadTemp(void);

	


#endif

