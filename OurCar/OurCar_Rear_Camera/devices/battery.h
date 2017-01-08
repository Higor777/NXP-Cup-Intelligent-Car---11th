#ifndef __BATTERY_H
#define __BATTERY_H

#include <stdint.h>

float Battery_GetVolt(void);
void Battery_num2str(int32_t battery);
void Battery_ShowVolt(void);

#endif
