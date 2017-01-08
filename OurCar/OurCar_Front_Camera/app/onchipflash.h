#ifndef __ONCHIP_FLASH_H
#define __ONCHIP_FLASH_H
#include <stdint.h>
#include <stdbool.h>
/* chip's Flash size and sector size can be found in RM */
#define DEV_SIZE            (0x80000)
#define Sector_Base          224
#define Addr_Base            Secter_Base*SECTOR_SIZE     //458752->0x70000

/*
!!!
参数存储扇区从224开始，一个扇区只存储一个参数，最大到255，所以这种存储方案只能存储32个参数 
!!!
*/

//片内资源足够，所以采用一个变量单独使用一个扇区的方式，一个扇区为2Kbyte->2048byte
//定义变量存储区相对于扇区基址的偏移
#define Dir_Preset_Offset  			 0
#define Speed_EX_H_Offset    		 1
#define Speed_EX_L_Offset 			 2
#define L_ex_mid_Offset   			 3
#define R_ex_mid_Offset   			 4
#define X_Thre_Offset     			 5
#define Ar_max_Offset      			 6
#define Ar_min_Offset     			 7
#define Wr_min_Offset     			 8
#define Beacon_N_Offset  			   9
#define Obstacle_X_Thre_Offset   10
#define Lose_MAX_Offset          11
#define Threshold_Offset         12
#define Cnt_X_Thre_Offset        13
#define Servo_MID_Offset         14
#define Servo_MIN_Offset         15
#define Servo_MAX_Offset         16
#define Obs_preset_Offset        17

void Read_Var(uint8_t offset);
void Write_All_Var(void);
void Write_Var(uint8_t offset);
void Read_All_Var(void);
void FlashData_Init(void);
	

#endif







