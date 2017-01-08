#ifndef __CCD_H
#define __CCD_H

#include <stdint.h>
#include "gpio.h"

#define CCD_CLK_Set()   do{GPIO_WriteBit(HW_GPIOE,5,1);}while(0)
#define CCD_CLK_Reset() do{GPIO_WriteBit(HW_GPIOE,5,0);}while(0)
#define CCD_SI_Set()    do{GPIO_WriteBit(HW_GPIOE,7,1);}while(0)
#define CCD_SI_Reset()  do{GPIO_WriteBit(HW_GPIOE,7,0);}while(0)




extern uint8_t CCD_Data[128];

void Delay100Ns(uint8_t ns);
void CCD_Init(void);
void CCD_Start(void);
void CCD_ImageCapture(uint8_t ImageData[]);
void CCD_IntegTime(const uint8_t ImageData[]);
void Check_Obstacle(uint8_t * ccd_data);


#endif
