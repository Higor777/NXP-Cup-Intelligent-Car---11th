#ifndef __KEY_H_
#define	__KEY_H_

#include <stdint.h>
#include <stdbool.h>


#define Key_1 PDin(10)
#define Key_2 PDin(11)
#define Key_3 PDin(8)
#define Key_4 PDin(9)
#define M_Encoder_A   PBin(18)
#define M_Encoder_B   PBin(16)


#define Key_1_MASK		 0x01U
#define Key_1_SHIFT    0
#define Key_2_MASK 	   0x02U
#define Key_2_SHIFT 	 1
#define Key_3_MASK 	 	 0x04U
#define Key_3_SHIFT 	 2
#define Key_4_MASK 	   0x08U
#define Key_4_SHIFT  	 3


#define M_Encoder_CW_MASK 		0x01U	//顺时针
#define M_Encoder_CW_SHIFT 		0
#define M_Encoder_CCW_MASK 		0x02U	//逆时针	
#define M_Encoder_CCW_SHIFT 	1

#define M_Encoder_A_MASK   	0x01U
#define M_Encoder_A_SHIFT	0
#define M_Encoder_B_MASK   	0x02U
#define M_Encoder_B_SHIFT	1

#define KEY_Scan_STA_Update_MASK	0x80U
#define M_Encoder_STA_Update_MASK	0x80U


//连按等待时间
#define Key_KeepTime	30
//连按输出的时间间隔
#define Key_KeepStep	5

//是否在定时器中断里面扫描按键IO状态
//扫描按键IO状态应每10ms一次
#define USE_PIT_KEYSCAN 1

extern volatile uint8_t M_Encoder_STA;

void Key_Init(void);
void Key_Scan(void);
uint8_t Key_STA(void);
void M_Encoder_Init(void);
void M_Encoder_Scan(void);
#endif
