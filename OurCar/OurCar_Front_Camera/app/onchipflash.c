#include "flash.h"
#include "onchipflash.h"
#include "string.h"
#include "KEY.h"
#include "gpio.h"
#include "oled.h"
extern int8_t Dir_Preset[30];
extern float Speed_EX_H;
extern float Speed_EX_L;
extern float L_ex_mid;
extern float R_ex_mid;
extern float X_Thre;
extern float Ar_max;
extern float Ar_min;
extern float Wr_min;
extern float Beacon_N;
extern float Obstacle_X_Thre;
extern float Lose_MAX;
extern float Threshold;
extern float Cnt_X_Thre;
extern float Servo_MID;
extern float Servo_MAX;
extern float Servo_MIN;
extern float Obs_preset[30];
void Write_Var(uint8_t offset)
{
	int sector_size;
	int addr;
	sector_size=FLASH_GetSectorSize();
	addr = ((Sector_Base+offset)*sector_size);
	FLASH_EraseSector(addr);
	switch(offset)
	{
		case Dir_Preset_Offset:
			FLASH_WriteSector(addr,(uint8_t *)Dir_Preset,30);
			break;
		case Speed_EX_H_Offset:
			FLASH_WriteSector(addr,(uint8_t *)&Speed_EX_H,4);
			break;
		case Speed_EX_L_Offset:
			FLASH_WriteSector(addr,(uint8_t *)&Speed_EX_L,4);
			break;
		case L_ex_mid_Offset:
			FLASH_WriteSector(addr,(uint8_t *)&L_ex_mid,4);
			break;
		case R_ex_mid_Offset:
			FLASH_WriteSector(addr,(uint8_t *)&R_ex_mid,4);
			break;
		case X_Thre_Offset:
			FLASH_WriteSector(addr,(uint8_t *)&X_Thre,4);
			break;
		case Ar_max_Offset:
			FLASH_WriteSector(addr,(uint8_t *)&Ar_max,4);
			break;
		case Ar_min_Offset:
			FLASH_WriteSector(addr,(uint8_t *)&Ar_min,4);
			break;
		case Wr_min_Offset:
			FLASH_WriteSector(addr,(uint8_t *)&Wr_min,4);
			break;
		case Beacon_N_Offset:
			FLASH_WriteSector(addr,(uint8_t *)&Beacon_N,4);
			break;
		case Obstacle_X_Thre_Offset:
			FLASH_WriteSector(addr,(uint8_t *)&Obstacle_X_Thre,4);
			break;
		case Lose_MAX_Offset:
			FLASH_WriteSector(addr,(uint8_t *)&Lose_MAX,4);
			break;
		case Threshold_Offset:
			FLASH_WriteSector(addr,(uint8_t *)&Threshold,4);
			break;
		case Cnt_X_Thre_Offset:
			FLASH_WriteSector(addr,(uint8_t *)&Cnt_X_Thre,4);
			break;
		case Servo_MID_Offset:
			FLASH_WriteSector(addr,(uint8_t *)&Servo_MID,4);
			break;
		case Servo_MIN_Offset:
			FLASH_WriteSector(addr,(uint8_t *)&Servo_MIN,4);
			break;
		case Servo_MAX_Offset:
			FLASH_WriteSector(addr,(uint8_t *)&Servo_MAX,4);
			break;
		case Obs_preset_Offset:
			FLASH_WriteSector(addr,(uint8_t *)&Obs_preset,120);
			break;
		default :
			break;
	}
}


void Write_All_Var(void)
{
	Write_Var(Dir_Preset_Offset);
	Write_Var(Speed_EX_H_Offset);
	Write_Var(Speed_EX_L_Offset);
	Write_Var(L_ex_mid_Offset);
	Write_Var(R_ex_mid_Offset);
	Write_Var(X_Thre_Offset);
	Write_Var(Ar_max_Offset);
	Write_Var(Ar_min_Offset);
	Write_Var(Wr_min_Offset);
	Write_Var(Beacon_N_Offset);
	Write_Var(Obstacle_X_Thre_Offset);
	Write_Var(Lose_MAX_Offset);
	Write_Var(Threshold_Offset);
	Write_Var(Cnt_X_Thre_Offset);
	Write_Var(Servo_MID_Offset);
	Write_Var(Servo_MIN_Offset);
	Write_Var(Servo_MAX_Offset);
	Write_Var(Obs_preset_Offset);
}


void Read_Var(uint8_t offset)
{
	int sector_size;
	uint8_t *addr;
	sector_size=FLASH_GetSectorSize();
	addr = (uint8_t*)((Sector_Base+offset)*sector_size);
	switch(offset)
	{
		case Dir_Preset_Offset:
			memcpy(Dir_Preset,addr,30);
			break;
		case Speed_EX_H_Offset:
			memcpy(&Speed_EX_H,addr,4);
			break;
		case Speed_EX_L_Offset:
			memcpy(&Speed_EX_L,addr,4);
			break;
		case L_ex_mid_Offset:
			memcpy(&L_ex_mid,addr,4);
			break;
		case R_ex_mid_Offset:
			memcpy(&R_ex_mid,addr,4);
			break;
		case X_Thre_Offset:
			memcpy(&X_Thre,addr,4);
			break;
		case Ar_max_Offset:
			memcpy(&Ar_max,addr,4);
			break;
		case Ar_min_Offset:
			memcpy(&Ar_min,addr,4);
			break;
		case Wr_min_Offset:
			memcpy(&Wr_min,addr,4);
			break;
		case Beacon_N_Offset:
			memcpy(&Beacon_N,addr,4);
			break;
		case Obstacle_X_Thre_Offset:
			memcpy(&Obstacle_X_Thre,addr,4);
			break;
		case Lose_MAX_Offset:
			memcpy(&Lose_MAX,addr,4);
			break;
		case Threshold_Offset:
			memcpy(&Threshold,addr,4);
			break;
		case Cnt_X_Thre_Offset:
			memcpy(&Cnt_X_Thre,addr,4);
			break;
		case Servo_MID_Offset:
			memcpy(&Servo_MID,addr,4);
			break;
		case Servo_MIN_Offset:
			memcpy(&Servo_MIN,addr,4);
			break;
		case Servo_MAX_Offset:
			memcpy(&Servo_MAX,addr,4);
			break;
		case Obs_preset_Offset:
			memcpy(&Obs_preset,addr,120);
			break;
		default :		
			break;
	}
}

void Read_All_Var(void)
{
	Read_Var(Dir_Preset_Offset);
	Read_Var(Speed_EX_H_Offset);
	Read_Var(Speed_EX_L_Offset);
	Read_Var(L_ex_mid_Offset);
	Read_Var(R_ex_mid_Offset);
	Read_Var(X_Thre_Offset);
	Read_Var(Ar_max_Offset);
	Read_Var(Ar_min_Offset);
	Read_Var(Wr_min_Offset);
	Read_Var(Beacon_N_Offset);
	Read_Var(Obstacle_X_Thre_Offset);
	Read_Var(Lose_MAX_Offset);
	Read_Var(Threshold_Offset);
	Read_Var(Cnt_X_Thre_Offset);
	Read_Var(Servo_MID_Offset);
	Read_Var(Servo_MIN_Offset);
	Read_Var(Servo_MAX_Offset);
	Read_Var(Obs_preset_Offset);
}

void FlashData_Init(void)
{
	//开机时按住Key_5将Flash内的数据复位为程序初始化数据
	if(Key_3==0)
	{
		Read_All_Var();
		OLED_Print(0,0,(uint8_t *)"R Finish!");
		DelayMs(333);
	}
	if(Key_5==0)//核心板上独立按键
	{
		Write_All_Var();
		OLED_Print(0,0,(uint8_t *)"W Finish!");
		DelayMs(333);
	}

}


