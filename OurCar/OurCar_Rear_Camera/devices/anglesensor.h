#ifndef __ANGLESENSOR_H
#define __ANGLESENSOR_H

#include <stdint.h>
/*MMA8451Q内部寄存器
  STATUS=0,               // 0x00
  OUT_X_MSB,              // 0x01
  OUT_X_LSB,              // 0x02
  OUT_Y_MSB,              // 0x03
  OUT_Y_LSB,              // 0x04
  OUT_Z_MSB,              // 0x05
  OUT_Z_LSB,              // 0x06
  F_STATUS,               // 0x08
  F_SETUP,                // 0x09
  TRIG_CFG,               // 0x0A
  SYSMOD,                 // 0x0B
  INT_SOURCE,             // 0x0C
  WHO_AM_I,               // 0x0D
  XYZ_DATA_CFG,           // 0x0E
  HP_FILTER_CUTOFF,       // 0x0F
  PL_STATUS,              // 0x10
  PL_CFG,                 // 0x11
  PL_COUNT,               // 0x12
  PL_BF_ZCOMP,            // 0x13
  PL_P_L_THS_REG,         // 0x14
  FF_MT_CFG,              // 0x15
  FF_MT_SRC,              // 0x16
  FF_MT_THS,              // 0x17
  FF_MT_COUNT,            // 0x18
  TRANSIENT_CFG,          // 0x1D
  TRANSIENT_SRC,          // 0x1E
  TRANSIENT_THS,          // 0x1F
  TRANSIENT_COUNT,        // 0x20
  PULSE_CFG,              // 0x21
  PULSE_SRC,              // 0x22
  PULSE_THSX,             // 0x23
  PULSE_THSY,             // 0x24
  PULSE_THSZ,             // 0x25
  PULSE_TMLT,             // 0x26
  PULSE_LTCY,             // 0x27
  PULSE_WIND,             // 0x28
  ASLP_COUNT,             // 0x29
  CTRL_REG1,              // 0x2A
  CTRL_REG2,              // 0x2B
  CTRL_REG3,              // 0x2C
  CTRL_REG4,              // 0x2D
  CTRL_REG5,              // 0x2E
  OFF_X,                  // 0x2F
  OFF_Y,                  // 0x30
  OFF_Z                   // 0x31
*/

/*设备地址*///修改
#define MMA8451_ADDRESS 0x1C //SA=0
#define MMA8451_SCL_SDA (I2C0_SCL_PB02_SDA_PB03)//对应关系不对
#define MMA8451_I2Cx HW_I2C0
/*MMA8451寄存器地址*/
#define CTRL_REG1 0x2A
#define XYZ_DATA_CFG_REG 0x0E
#define HP_FILTER_CUTOFF_REG 0x0F
#define OUT_X_MSB_REG 0x01
#define OUT_Y_MSB_REG 0x03
#define OUT_Z_MSB_REG 0x05
/*MMA7361*/
#define MMA7361_XOUT ADC0_SE9_PB1
#define MMA7361_YOUT ADC0_SE12_PB2
#define MMA7361_ZOUT ADC0_SE13_PB3
/*ENC03*/
#define ENC03_AR1 ADC1_SE10_PB04
#define ENC03_AR2 ADC1_SE11_PB05
#define ENC03_AR3 ADC1_SE12_PB06

void AngleSensor_WriteRegister(uint32_t instance,uint8_t devAddr,uint8_t regAddr,uint8_t data);
uint8_t AngleSensor_ReadRegister(uint32_t instance,uint8_t devAddr,uint8_t regAddr);
uint16_t AngleSensor_GetData(uint32_t instance,uint8_t devAddr,uint8_t regAddr);
void MMA8451_Init(void);

void ENC03_Init(void);
void MMA7361_Init(void);

#endif

