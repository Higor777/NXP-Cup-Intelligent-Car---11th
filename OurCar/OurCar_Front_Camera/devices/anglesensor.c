#include "anglesensor.h"
#include "common.h"
#include "gpio.h"
#include "i2c.h"
#include "umc.h"
#include "uart.h"
#include  <math.h>    //Keil library  
unsigned char BUF[8];                         //接收数据缓存区
char  test=0; 
int   x,y;
float angle;
uint8_t ge,shi,bai,qian,wan,shiwan;           //显示变量
int A_X,A_Y,A_Z;
short T_X,T_Y,T_Z;

short data_xyz[3];
float Roll,Pitch,Q,T,K;


//***BMP085使用
short ac1;
short ac2; 
short ac3; 
unsigned short ac4;
unsigned short ac5;
unsigned short ac6;
short b1; 
short b2;
short mb;
short mc;
short md;

long  temperature;
long  pressure;


//***************************************************
void conversion(long temp_data)  
{     
    shiwan=temp_data/100000+0x30 ;
    temp_data=temp_data%100000;   //取余运算 
    wan=temp_data/10000+0x30 ;
    temp_data=temp_data%10000;   //取余运算
	 qian=temp_data/1000+0x30 ;
    temp_data=temp_data%1000;    //取余运算
    bai=temp_data/100+0x30   ;
    temp_data=temp_data%100;     //取余运算
    shi=temp_data/10+0x30    ;
    temp_data=temp_data%10;      //取余运算
    ge=temp_data+0x30; 	
}


/*
********************************************************************************
** 函数名称 ： Delay(uint32_t nCount)
** 函数功能 ： 延时函数
** 输    入	： 无
** 输    出	： 无
** 返    回	： 无
********************************************************************************
*/
 void Delay(uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}

/*
********************************************************************************
** 函数名称 ： void Delayms(uint32_t m)
** 函数功能 ： 长延时函数	 m=1,延时1ms
** 输    入	： 无
** 输    出	： 无
** 返    回	： 无
********************************************************************************
*/
 void Delayms(uint32_t m)
{
  uint32_t i;
  
  for(; m != 0; m--)	
       for (i=0; i<50000; i++);
}



void IMU_Init(void)
{

	I2C_QuickInit(I2C0_SCL_PD08_SDA_PD09,1000);
	
	
	DelayMs(10);
	Init_HMC5883L();
  Init_ADXL345();
  Init_BMP085();
  Init_L3G4200D();		     //初始化L3G4200D
}

//********************************************************************
long bmp085ReadTemp(void)
{   short  temp_ut;
	I2C_WriteSingleRegister(HW_I2C0,BMP085_Addr,0xF4,0x2E);
	Delayms(5);	// max time is 4.5ms
	I2C_ReadSingleRegister(HW_I2C0,BMP085_Addr,0xF6,(uint8_t *)&temp_ut);
	temp_ut |= (temp_ut>>8);
	I2C_ReadSingleRegister(HW_I2C0,BMP085_Addr,0xF7,(uint8_t *)&temp_ut);		
	
	return (long) temp_ut ;
}
//*************************************************************

long bmp085ReadPressure(void)
{
	long pressure = 0;
	I2C_WriteSingleRegister(HW_I2C0,BMP085_Addr,0xF4,0x34);
	Delayms(5);	// max time is 4.5ms
	I2C_ReadSingleRegister(HW_I2C0,BMP085_Addr,0xF6,(uint8_t *)&pressure);
	pressure |= (pressure>>8);
	I2C_ReadSingleRegister(HW_I2C0,BMP085_Addr,0xF7,(uint8_t *)&pressure);	
  
	pressure &= 0x0000FFFF;	
	return pressure;	
}

 //******************
void Send_ADXL345_data(int dis_data)
  { float temp ;
	if(dis_data>0x7fff)dis_data-=0xffff;
 	if(dis_data<0){
  	     dis_data=-dis_data;
	       UART_WriteByte(HW_UART0,(uint16_t)'-'); 
		 Delayms(2);
	}
	else
	  { 
	   UART_WriteByte(HW_UART0,(uint16_t)'+'); 
	   Delayms(2);
	   }
        temp=(float)dis_data*3.9;  //计算数据和显示,查考ADXL345快速入门第4页
        conversion(temp);          //转换出显示需要的数据	 
	    UART_WriteByte(HW_UART0,(uint16_t)qian);
		UART_WriteByte(HW_UART0,(uint16_t)'.');
	    UART_WriteByte(HW_UART0,(uint16_t)bai); 
	    UART_WriteByte(HW_UART0,(uint16_t)shi); 
	    UART_WriteByte(HW_UART0,(uint16_t)ge); 
		UART_WriteByte(HW_UART0,(uint16_t)'g'); 
}
//***************************************
 //******************
void  Send_L3G420D_data(short dis_data)
  { float temp ;
 	if(dis_data<0){
  	     dis_data=-dis_data;
	     UART_WriteByte(HW_UART0,(uint16_t)'-');
		 Delayms(2);
	}
	else
	  { 
	   UART_WriteByte(HW_UART0,(uint16_t)'+');
	   Delayms(2);
	   }
        temp=(float)dis_data*0.07;  //计算数据和显示,查考ADXL345快速入门第4页
        conversion(temp);           //转换出显示需要的数据	 
	   UART_WriteByte(HW_UART0,(uint16_t)bai); 
	    UART_WriteByte(HW_UART0,(uint16_t)shi); 
	   UART_WriteByte(HW_UART0,(uint16_t)ge); 
 
}
//***************************************


void Send_HMC5883L(void)
{
UART_WriteByte(HW_UART0,(uint16_t)'H');
UART_WriteByte(HW_UART0,(uint16_t)'M');
UART_WriteByte(HW_UART0,(uint16_t)'C');
UART_WriteByte(HW_UART0,(uint16_t)'5');
UART_WriteByte(HW_UART0,(uint16_t)'8');
UART_WriteByte(HW_UART0,(uint16_t)'8');
UART_WriteByte(HW_UART0,(uint16_t)'3');
UART_WriteByte(HW_UART0,(uint16_t)'L');
UART_WriteByte(HW_UART0,(uint16_t)':');
conversion(angle);
UART_WriteByte(HW_UART0,(uint16_t)bai); 
UART_WriteByte(HW_UART0,(uint16_t)shi); 
UART_WriteByte(HW_UART0,(uint16_t)ge); 
UART_WriteByte(HW_UART0,(uint16_t)'`');  
UART_WriteByte(HW_UART0,(uint16_t)0x0d);
UART_WriteByte(HW_UART0,(uint16_t)0x0a);
}
//*************************************************
void  Send_BMP085(void)
{
UART_WriteByte(HW_UART0,(uint16_t)'B');
UART_WriteByte(HW_UART0,(uint16_t)'M');
UART_WriteByte(HW_UART0,(uint16_t)'P');
UART_WriteByte(HW_UART0,(uint16_t)'0');
UART_WriteByte(HW_UART0,(uint16_t)'8');
UART_WriteByte(HW_UART0,(uint16_t)'5');
UART_WriteByte(HW_UART0,(uint16_t)':');

UART_WriteByte(HW_UART0,(uint16_t)'t');
UART_WriteByte(HW_UART0,(uint16_t)'=');
conversion(temperature);
UART_WriteByte(HW_UART0,(uint16_t)bai); 
UART_WriteByte(HW_UART0,(uint16_t)shi); 
UART_WriteByte(HW_UART0,(uint16_t)'.');
UART_WriteByte(HW_UART0,(uint16_t)ge);
UART_WriteByte(HW_UART0,(uint16_t)'`'); 
UART_WriteByte(HW_UART0,(uint16_t)'C'); 

UART_WriteByte(HW_UART0,(uint16_t)' ');
UART_WriteByte(HW_UART0,(uint16_t)'p');
UART_WriteByte(HW_UART0,(uint16_t)'=');
conversion(pressure);

UART_WriteByte(HW_UART0,(uint16_t)shiwan);
UART_WriteByte(HW_UART0,(uint16_t)wan);
UART_WriteByte(HW_UART0,(uint16_t)qian);
UART_WriteByte(HW_UART0,(uint16_t)'.');
UART_WriteByte(HW_UART0,(uint16_t)bai); 
UART_WriteByte(HW_UART0,(uint16_t)shi); 
UART_WriteByte(HW_UART0,(uint16_t)ge); 
UART_WriteByte(HW_UART0,(uint16_t)'K'); 
UART_WriteByte(HW_UART0,(uint16_t)'p'); 
UART_WriteByte(HW_UART0,(uint16_t)'a');  				
UART_WriteByte(HW_UART0,(uint16_t)0x0d);
UART_WriteByte(HW_UART0,(uint16_t)0x0a);

}
//*****************************************************

void  Send_ADXL345(void)
{
UART_WriteByte(HW_UART0,(uint16_t)'A');
UART_WriteByte(HW_UART0,(uint16_t)'D');
UART_WriteByte(HW_UART0,(uint16_t)'X');
UART_WriteByte(HW_UART0,(uint16_t)'L');
UART_WriteByte(HW_UART0,(uint16_t)'3');
UART_WriteByte(HW_UART0,(uint16_t)'4');
UART_WriteByte(HW_UART0,(uint16_t)'5');
UART_WriteByte(HW_UART0,(uint16_t)':');
	   UART_WriteByte(HW_UART0,(uint16_t)'X');
	   UART_WriteByte(HW_UART0,(uint16_t)'=');
	   Send_ADXL345_data(A_X);

	   UART_WriteByte(HW_UART0,(uint16_t)'Y');
	   UART_WriteByte(HW_UART0,(uint16_t)'=');
	   Send_ADXL345_data(A_Y);

	   UART_WriteByte(HW_UART0,(uint16_t)'Z');
	   UART_WriteByte(HW_UART0,(uint16_t)'=');
	   Send_ADXL345_data(A_Z);

	   adxl345_angle();


UART_WriteByte(HW_UART0,(uint16_t)0x0d);
UART_WriteByte(HW_UART0,(uint16_t)0x0a);
}
 //*****************************************************
 void Send_L3G4200D(void)
 {
   	UART_WriteByte(HW_UART0,(uint16_t)'L');
	UART_WriteByte(HW_UART0,(uint16_t)'3');
	UART_WriteByte(HW_UART0,(uint16_t)'G');
	UART_WriteByte(HW_UART0,(uint16_t)'4');
	UART_WriteByte(HW_UART0,(uint16_t)'2');
	UART_WriteByte(HW_UART0,(uint16_t)'0');
	UART_WriteByte(HW_UART0,(uint16_t)'0');
	UART_WriteByte(HW_UART0,(uint16_t)'D');
	UART_WriteByte(HW_UART0,(uint16_t)':');

	   UART_WriteByte(HW_UART0,(uint16_t)'X');
	   UART_WriteByte(HW_UART0,(uint16_t)'=');
	   Send_L3G420D_data(T_X);

	   UART_WriteByte(HW_UART0,(uint16_t)'Y');
	   UART_WriteByte(HW_UART0,(uint16_t)'=');
	   Send_L3G420D_data(T_Y);

	   UART_WriteByte(HW_UART0,(uint16_t)'Z');
	   UART_WriteByte(HW_UART0,(uint16_t)'=');
	   Send_L3G420D_data(T_Z);

	  UART_WriteByte(HW_UART0,(uint16_t)0x0d);
     UART_WriteByte(HW_UART0,(uint16_t)0x0a);
 }
//******************************************************
void read_hmc5883l(void)
{
       I2C_WriteSingleRegister(HW_I2C0,HMC5883L_Addr,0x00,0x14);   //
       I2C_WriteSingleRegister(HW_I2C0,HMC5883L_Addr,0x02,0x00);   //
  	   Delayms(10);

       I2C_ReadSingleRegister(HW_I2C0,HMC5883L_Addr,0x03,&BUF[1]);//OUT_X_L_A
       I2C_ReadSingleRegister(HW_I2C0,HMC5883L_Addr,0x04,&BUF[2]);//OUT_X_H_A

	     I2C_ReadSingleRegister(HW_I2C0,HMC5883L_Addr,0x07,&BUF[3]);//OUT_Y_L_A
       I2C_ReadSingleRegister(HW_I2C0,HMC5883L_Addr,0x08,&BUF[4]);//OUT_Y_H_A

       x=(BUF[1] << 8) | BUF[2]; //Combine MSB and LSB of X Data output register
       y=(BUF[3] << 8) | BUF[4]; //Combine MSB and LSB of Z Data output register

       if(x>0x7fff)x-=0xffff;	  
       if(y>0x7fff)y-=0xffff;	  
       angle= atan2(y,x) * (180 / 3.14159265) + 180; // angle in degrees
}
//****************************************
void read_ADXL345(void)
{
       I2C_ReadSingleRegister(HW_I2C0,ADXL345_Addr,0x32,&BUF[0]);//OUT_X_L_A
       I2C_ReadSingleRegister(HW_I2C0,ADXL345_Addr,0x33,&BUF[1]);//OUT_X_H_A

	     I2C_ReadSingleRegister(HW_I2C0,ADXL345_Addr,0x34,&BUF[2]);//OUT_Y_L_A
       I2C_ReadSingleRegister(HW_I2C0,ADXL345_Addr,0x35,&BUF[3]);//OUT_Y_H_A

	     I2C_ReadSingleRegister(HW_I2C0,ADXL345_Addr,0x36,&BUF[4]);//OUT_Z_L_A
       I2C_ReadSingleRegister(HW_I2C0,ADXL345_Addr,0x37,&BUF[5]);//OUT_Z_H_A

	   A_X=(BUF[1]<<8)+BUF[0];  //合成数据  
	   A_Y=(BUF[3]<<8)+BUF[2];  //合成数据
	   A_Z=(BUF[5]<<8)+BUF[4];  //合成数据
}
//*****************************************
void read_BMP085(void)
{

   	long ut;
	long up;
	long x1, x2, b5, b6, x3, b3, p;
	unsigned long b4, b7;

	ut = bmp085ReadTemp();	   // 读取温度
//	ut = bmp085ReadTemp();	   // 读取温度
	up = bmp085ReadPressure();  // 读取压强
//	up = bmp085ReadPressure();  // 读取压强
	x1 = ((long)ut - ac6) * ac5 >> 15;
	x2 = ((long) mc << 11) / (x1 + md);
	b5 = x1 + x2;
	temperature = (b5 + 8) >> 4;
	
	 //****************

	b6 = b5 - 4000;
	x1 = (b2 * (b6 * b6 >> 12)) >> 11;
	x2 = ac2 * b6 >> 11;
	x3 = x1 + x2;
	b3 = (((long)ac1 * 4 + x3) + 2)/4;
	x1 = ac3 * b6 >> 13;
	x2 = (b1 * (b6 * b6 >> 12)) >> 16;
	x3 = ((x1 + x2) + 2) >> 2;
	b4 = (ac4 * (unsigned long) (x3 + 32768)) >> 15;
	b7 = ((unsigned long) up - b3) * (50000 >> OSS);
	if( b7 < 0x80000000)
	     p = (b7 * 2) / b4 ;
           else  
		    p = (b7 / b4) * 2;
	x1 = (p >> 8) * (p >> 8);
	x1 = (x1 * 3038) >> 16;
	x2 = (-7357 * p) >> 16;
	 pressure = p + ((x1 + x2 + 3791) >> 4);
}
//***************************
void  Init_ADXL345(void)
{
   I2C_WriteSingleRegister(HW_I2C0,ADXL345_Addr,0x31,0x0B);   //测量范围,正负16g，13位模式
  // I2C_WriteSingleRegister(HW_I2C0,ADXL345_Addr,0x2C,0x0e);   //速率设定为100hz 参考pdf13页
   I2C_WriteSingleRegister(HW_I2C0,ADXL345_Addr,0x2D,0x08);   //选择电源模式   参考pdf24页
   I2C_WriteSingleRegister(HW_I2C0,ADXL345_Addr,0x2E,0x80);   //使能 DATA_READY 中断
  // I2C_WriteSingleRegister(HW_I2C0,ADXL345_Addr,0x1E,0x00);   //X 偏移量 根据测试传感器的状态写入pdf29页
  // I2C_WriteSingleRegister(HW_I2C0,ADXL345_Addr,0x1F,0x00);   //Y 偏移量 根据测试传感器的状态写入pdf29页
  // I2C_WriteSingleRegister(HW_I2C0,ADXL345_Addr,0x20,0x05);   //Z 偏移量 根据测试传感器的状态写入pdf29页
}
//****************************
 void  Init_BMP085(void)
{
	I2C_ReadSingleRegister(HW_I2C0,BMP085_Addr,0xAA,(uint8_t *)&ac1);
	ac1 |= (ac1>>8);
  I2C_ReadSingleRegister(HW_I2C0,BMP085_Addr,0xAB,(uint8_t *)&ac1);
	

  ac2 = I2C_ReadSingleRegister(HW_I2C0,BMP085_Addr,0xAC,(uint8_t *)&ac2);
	ac2 |= (ac2>>8);
	I2C_ReadSingleRegister(HW_I2C0,BMP085_Addr,0xAD,(uint8_t *)&ac2);
	

	ac3 = I2C_ReadSingleRegister(HW_I2C0,BMP085_Addr,0xAE,(uint8_t *)&ac3);
	ac3 |= (ac3>>8);
	I2C_ReadSingleRegister(HW_I2C0,BMP085_Addr,0xAF,(uint8_t *)&ac3);
	

	ac4 = I2C_ReadSingleRegister(HW_I2C0,BMP085_Addr,0xB0,(uint8_t *)&ac4);
	ac4 |= (ac4>>8);
	I2C_ReadSingleRegister(HW_I2C0,BMP085_Addr,0xB1,(uint8_t *)&ac4);
	

	ac5 = I2C_ReadSingleRegister(HW_I2C0,BMP085_Addr,0xB2,(uint8_t *)&ac5);
	ac5 |= (ac5>>8);
	I2C_ReadSingleRegister(HW_I2C0,BMP085_Addr,0xB3,(uint8_t *)&ac5);
	

	ac6 = I2C_ReadSingleRegister(HW_I2C0,BMP085_Addr,0xB4,(uint8_t *)&ac6);
	ac6 |= (ac6>>8);
	I2C_ReadSingleRegister(HW_I2C0,BMP085_Addr,0xB5,(uint8_t *)&ac6);
	

	b1 = I2C_ReadSingleRegister(HW_I2C0,BMP085_Addr,0xB6,(uint8_t *)&b1);
	b1 |= (b1>>8);
	I2C_ReadSingleRegister(HW_I2C0,BMP085_Addr,0xB7,(uint8_t *)&b1);
	

	b2 = I2C_ReadSingleRegister(HW_I2C0,BMP085_Addr,0xB8,(uint8_t *)&b2);
	b2 |= (b2>>8);
	I2C_ReadSingleRegister(HW_I2C0,BMP085_Addr,0xB9,(uint8_t *)&b2);
	

	mb = I2C_ReadSingleRegister(HW_I2C0,BMP085_Addr,0xBA,(uint8_t *)&mb);
	mb |= (mb>>8);
	I2C_ReadSingleRegister(HW_I2C0,BMP085_Addr,0xBB,(uint8_t *)&mb);
	

	mc = I2C_ReadSingleRegister(HW_I2C0,BMP085_Addr,0xBC,(uint8_t *)&mc);
	mc |= (mc>>8);
	I2C_ReadSingleRegister(HW_I2C0,BMP085_Addr,0xBD,(uint8_t *)&mc);
	

	md = I2C_ReadSingleRegister(HW_I2C0,BMP085_Addr,0xBE,(uint8_t *)&md);
	md |= (md>>8);
	I2C_ReadSingleRegister(HW_I2C0,BMP085_Addr,0xBF,(uint8_t *)&md);
	

}
//****************************
 void  Init_HMC5883L(void)
{
   I2C_WriteSingleRegister(HW_I2C0,HMC5883L_Addr,0x00,0x14);   //
   I2C_WriteSingleRegister(HW_I2C0,HMC5883L_Addr,0x02,0x00);   //
}
//*****************************************

 //************初始化L3G4200D*********************************
void Init_L3G4200D(void)
{
	I2C_WriteSingleRegister(HW_I2C0,L3G4200_Addr,CTRL_REG1, 0x0f);
	I2C_WriteSingleRegister(HW_I2C0,L3G4200_Addr,CTRL_REG2, 0x00);
	I2C_WriteSingleRegister(HW_I2C0,L3G4200_Addr,CTRL_REG3, 0x08);
	I2C_WriteSingleRegister(HW_I2C0,L3G4200_Addr,CTRL_REG4, 0x30);	//+-2000dps
	I2C_WriteSingleRegister(HW_I2C0,L3G4200_Addr,CTRL_REG5, 0x00);
}	
//******读取L3G4200D数据****************************************
void read_L3G4200D(void)
{
   I2C_ReadSingleRegister(HW_I2C0,L3G4200_Addr,OUT_X_L,&BUF[0]);
   I2C_ReadSingleRegister(HW_I2C0,L3G4200_Addr,OUT_X_H,&BUF[1]);
   T_X=	(BUF[1]<<8)|BUF[0];
 


   I2C_ReadSingleRegister(HW_I2C0,L3G4200_Addr,OUT_Y_L,&BUF[2]);
   I2C_ReadSingleRegister(HW_I2C0,L3G4200_Addr,OUT_Y_H,&BUF[3]);
   T_Y=	(BUF[3]<<8)|BUF[2];
  

   I2C_ReadSingleRegister(HW_I2C0,L3G4200_Addr,OUT_Z_L,&BUF[4]);
   I2C_ReadSingleRegister(HW_I2C0,L3G4200_Addr,OUT_Z_H,&BUF[5]);
   T_Z=	(BUF[5]<<8)|BUF[4];

}

 //******************ADXL345计算倾斜角度************
void adxl345_angle(void)
 {

data_xyz[0]=A_X;  //合成数据   
data_xyz[1]=A_Y;  //合成数据   
data_xyz[2]=A_Z;  //合成数据   

//分别是加速度X,Y,Z的原始数据，10位的
Q=(float)data_xyz[0]*3.9;
T=(float)data_xyz[1]*3.9;
K=(float)data_xyz[2]*3.9;
Q=-Q;

  Roll=(float)(((atan2(K,Q)*180)/3.14159265)+180);    //X轴角度值
  Pitch=(float)(((atan2(K,T)*180)/3.14159265)+180);  //Y轴角度值
  //conversion(Pitch);								   //需要显示时候请更换
  conversion(Roll);
        UART_WriteByte(HW_UART0,(uint16_t)' '); 
		UART_WriteByte(HW_UART0,(uint16_t)' '); 
  	    UART_WriteByte(HW_UART0,(uint16_t)bai); 
	    UART_WriteByte(HW_UART0,(uint16_t)shi); 
	    UART_WriteByte(HW_UART0,(uint16_t)ge);
}
