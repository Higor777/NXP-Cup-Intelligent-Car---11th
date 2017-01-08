#ifndef __LED_H__
#define __LED_H__
#include "stdlib.h"
#define ON 0
#define OFF 1
#define LED_CNT 4
#define LED1(x) PBout(20) = x
#define LED2(x) PBout(21) = x
#define LED3(x) PBout(22) = x
#define LED4(x) PBout(23) = x



void LED_Init(void);
void LED_ON(uint8_t led);
void LED_OFF(uint8_t led);
void LED_Single(uint8_t led);
#endif

