#include "i2c.h"
#include "anglesensor.h"
#include "uart.h"
#include "adc.h"

/*
void judgeAck(uint8_t ack)
{
	switch(ack)
	{
		case 0:
			printf("收到应答\n");
			break;
		case 1:
			printf("没有应答\n");
			break;
		case 2:
			printf("超时\n");
			break;
	}
}

void AngleSensor_WriteRegister(uint32_t instance,uint8_t devAddr,uint8_t regAddr,uint8_t data)
{
	uint8_t ack;
	
	I2C_GenerateSTART(instance);
	I2C_Send7bitAddress(instance,devAddr,kI2C_Write);
	ack=I2C_WaitAck(instance);
	judgeAck(ack);
	
	I2C_SendData(instance,regAddr);
	ack=I2C_WaitAck(instance);
	judgeAck(ack);
	
	I2C_SendData(instance,data);
	ack=I2C_WaitAck(instance);
	judgeAck(ack);
	
	I2C_GenerateSTOP(instance);
	while(I2C_IsBusy(instance)==0);//等待传输完毕
	//DelayMs(50);
}

uint8_t AngleSensor_ReadRegister(uint32_t instance,uint8_t devAddr,uint8_t regAddr)
{
	uint8_t result=0;
	
	I2C_GenerateSTART(instance);
	I2C_Send7bitAddress(instance,devAddr,kI2C_Write);
	I2C_WaitAck(instance);
	
	I2C_SendData(instance,regAddr);
	I2C_WaitAck(instance);
	
	I2C_GenerateRESTART(instance);
	I2C_Send7bitAddress(instance,devAddr,kI2C_Read);
	I2C_WaitAck(instance);
	
	I2C_SetMasterMode(instance,kI2C_Read);
	I2C_GenerateAck(instance);
	I2C_ReadData(instance);
	I2C_WaitAck(instance);
	
	I2C_GenerateSTOP(instance);
	result=I2C_ReadData(instance);
	
	return result;
}

//x=AngleSensor_GetData(MMA8451_I2Cx,MMA8451_ADDRESS,OUT_X_MSB_REG);
uint16_t AngleSensor_GetData(uint32_t instance,uint8_t devAddr,uint8_t regAddr)
{
	uint8_t H,L;
	
	H=AngleSensor_ReadRegister(instance,devAddr,regAddr);
	L=AngleSensor_ReadRegister(instance,devAddr,regAddr+1);
	
	return ((H<<8)+L);
}

void MMA8451_Init()//
{
	I2C_QuickInit(MMA8451_SCL_SDA,376000);
	
	AngleSensor_WriteRegister(MMA8451_I2Cx,MMA8451_ADDRESS,CTRL_REG1,0x00+0x00);
	AngleSensor_WriteRegister(MMA8451_I2Cx,MMA8451_ADDRESS,XYZ_DATA_CFG_REG,0x00);//2g
	AngleSensor_WriteRegister(MMA8451_I2Cx,MMA8451_ADDRESS,HP_FILTER_CUTOFF_REG,0x10);//低通滤波
	AngleSensor_WriteRegister(MMA8451_I2Cx,MMA8451_ADDRESS,CTRL_REG1,0x01);//激活状态
}
*/
void ENC03_Init()
{
	ADC_QuickInit(ENC03_AR1,kADC_SingleDiff10or11);
	ADC_QuickInit(ENC03_AR2,kADC_SingleDiff10or11);
	ADC_QuickInit(ENC03_AR3,kADC_SingleDiff10or11);//ADC1_SE12_PB06需要验证地址
}

void MMA7361_Init()
{
	ADC_QuickInit(MMA7361_XOUT,kADC_SingleDiff10or11);
	ADC_QuickInit(MMA7361_YOUT,kADC_SingleDiff10or11);
	ADC_QuickInit(MMA7361_ZOUT,kADC_SingleDiff10or11);
}
