#include "common.h"
#include "SR04.h"
#include "gpio.h"
#include <stdio.h>


void SR04_Init(void)
{
	  GPIO_QuickInit(HW_GPIOE,8, kGPIO_Mode_OPP);
	  GPIO_WriteBit(HW_GPIOE,8,0);
		GPIO_QuickInit(HW_GPIOE,9, kGPIO_Mode_IPD);
	  GPIO_QuickInit(HW_GPIOC,17, kGPIO_Mode_OPP);
	  GPIO_WriteBit(HW_GPIOC,17,0);
		GPIO_QuickInit(HW_GPIOC,16, kGPIO_Mode_IPD);
}

float Get_Distance(uint8_t num)
{
	    uint32_t SR04_cnt=0;
      float Distance=100;
	    static float Last_Distance=100;
	    switch(num)
		 {
				case 1:
          PCout(17)=1;
					DelayUs(11);	
					PCout(17)=0;
					SR04_cnt=0;		
					while(PCin(16)==0&&SR04_cnt<10000){SR04_cnt++;}
					if(SR04_cnt>=10000)
					{
						return Distance;
					}
					SR04_cnt=0;
					while(PCin(16)==1&&SR04_cnt<10000){SR04_cnt++;}
					if(SR04_cnt>=10000)
					{
						Distance=100;
					}
					else 
					{
						Distance=(float)SR04_cnt/230.0;
					}	
					break;
				case 2:
					PEout(8)=1;
					DelayUs(11);	
					PEout(8)=0;
					SR04_cnt=0;			
					while(PEin(9)==0&&SR04_cnt<10000){SR04_cnt++;}
					if(SR04_cnt>=10000)
					{
						return Distance;
					}
					SR04_cnt=0;
					while(PEin(9)==1&&SR04_cnt<10000){SR04_cnt++;}
					if(SR04_cnt==10000)
					{
						Distance=100;;
					}
					else 
					{
						Distance=(float)SR04_cnt/230.0;
					}						
						
					break;
        default:
					break;
			}
		 if(Distance<7&&Last_Distance-Distance>50)Distance=Last_Distance;
		 Last_Distance=Distance;
		 return Distance;
}

