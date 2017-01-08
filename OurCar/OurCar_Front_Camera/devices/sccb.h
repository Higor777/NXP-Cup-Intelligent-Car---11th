/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外初学论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       VCAN_SCCB.h
 * @brief      OV摄像头配置总线SCCB函数库
 * @author     山外科技
 * @version    v5.0
 * @date       2013-09-01
 */


#ifndef __SCCB_H__
#define __SCCB_H__

#include <stdint.h>
#include "gpio.h"


//SCCB 管脚配置PTA25 26
#define SCCB_SCL        26
#define SCCB_SDA        25


#define SCCB_SCL_H()         PAout(SCCB_SCL) = 1
#define SCCB_SCL_L()         PAout(SCCB_SCL) = 0
#define SCCB_SCL_DDR_OUT()   PTA->PDDR|=(1<<SCCB_SCL) 
#define SCCB_SCL_DDR_IN()    PTA->PDDR&=~(1<<SCCB_SCL) 

#define SCCB_SDA_H()         PAout(SCCB_SDA) = 1
#define SCCB_SDA_L()         PAout(SCCB_SDA) = 0
#define SCCB_SDA_IN()        PAin(SCCB_SDA)
#define SCCB_SDA_DDR_OUT()   PTA->PDDR|=(1<<SCCB_SDA)
#define SCCB_SDA_DDR_IN()    PTA->PDDR&=~(1<<SCCB_SDA)

#define ADDR_OV7725   0x42

#define DEV_ADR  ADDR_OV7725             /*设备地址定义*/

#define SCCB_DELAY()    SCCB_delay(400)


void SCCB_GPIO_init(void);
int SCCB_WriteByte( uint16 WriteAddress , uint8 SendByte);
int SCCB_ReadByte(uint8 *pBuffer,   uint16 length,   uint8 ReadAddress);


#endif      //_VCAN_SCCB_H_
