#ifndef __BATTERYMON_H_
#define	__BATTERYMON_H_

#include <stdint.h>
#include <stdbool.h>

void BatteryMon_Init(void);
double Get_BatVol(void);
void Show_BatVol(void);

#endif
