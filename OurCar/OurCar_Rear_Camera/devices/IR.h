#ifndef __IR_H_
#define	__IR_H_

#include <stdint.h>
#include <stdbool.h>

extern volatile uint8_t Beacon_STA;

void IR_Init(void);
void Beacon_Check(void);
static void IR_ISR(uint32_t index);

#endif

