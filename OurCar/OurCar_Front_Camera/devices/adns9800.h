#ifndef __ADNS9800_H__
#define __ADNS9800_H__
#include <stdint.h>
#include <stdbool.h>
#define SPI1_PORT HW_GPIOB
#define PCS1 9
#define PCS0 10
#define SCK  11
#define SOUT 16
#define SIN  17
#define ADNS9800_spi_cs(x) PBout(PCS0)=x
#define ADNS9800_SPI_WRITE 0x80
// Registers address
#define ADNS9800_SPIREGISTER_PRODUCTID        0x00
#define ADNS9800_SPIREGISTER_RevisionID       0x01
#define ADNS9800_SPIREGISTER_MOTION           0x02
#define ADNS9800_SPIREGISTER_DELTAXL          0x03
#define ADNS9800_SPIREGISTER_DELTAXH          0x04
#define ADNS9800_SPIREGISTER_DELTAYL          0x05
#define ADNS9800_SPIREGISTER_DELTAYH          0x06
#define ADNS9800_SPIREGISTER_SQUAL            0x07
#define ADNS9800_SPIREGISTER_CONFIGURATION1   0x0F
#define ADNS9800_SPIREGISTER_Frame_Capture    0x12
#define ADNS9800_SPIREGISTER_SROMENABLE       0x13
#define ADNS9800_SPIREGISTER_LASERCTRL0       0x20
#define ADNS9800_SPIREGISTER_OBSERVATION      0x24
#define ADNS9800_SPIREGISTER_DOUTLOW          0x25
#define ADNS9800_SPIREGISTER_DOUTHI           0x26
#define ADNS9800_SPIREGISTER_SROMID           0x2A
#define ADNS9800_SPIREGISTER_CONFIGURATION4   0x39
#define ADNS9800_SPIREGISTER_CONFIGURATION5   0x2F
#define ADNS9800_SPIREGISTER_POWERUPRESET     0x3A
#define ADNS9800_SPIREGISTER_INVPRODUCTID     0x3F
#define ADNS9800_SPIREGISTER_SROMLOAD         0x62
#define ADNS9800_SPIREGISTER_Pixel_Burst      0x64
// Registers values
#define ADNS9800_CONFIGURATION4_LOAD1   0x02
#define ADNS9800_SROMENABLE_LOAD1       0x1D
#define ADNS9800_SROMENABLE_LOAD2       0x18
#define ADNS9800_SROMENABLE_CRC         0x15
#define ADNS9800_POWERUPRESET_POWERON   0x5A
#define ADNS9800_SROMENABLE_Frame_Capture1       0x93
#define ADNS9800_SROMENABLE_Frame_Capture2       0xC5
// Registers bits
#define ADNS9800_MOTION_BIT_LPVALID 5
#define ADNS9800_MOTION_BIT_FAULT   1
// 0x01 = 50, minimum
// 0x44 = 3400, default
// 0x8e = 7100
// 0xA4 = 8200, maximum
#define ADNS9800_CONFIGURATION1_RESMAX   0xA4
#define ADNS9800_OBSERVATION_BIT_OB6 6
#define ADNS9800_OBSERVATION_BIT_OB5 5
/// Initialize ADNS9800
void ADNS9800_init(void);
/// Launch ADNS9800s boot sequence
void ADNS9800_boot(void);
void ADNS9800_upload_firmware(void);
void Frame_Capture_Prepare(void);
void Frame_Capture(uint8_t * pic);
void Get_Motion(int16_t * X,int16_t * Y);
#define ADNS9800_FIRMWARE_CRCHI (0xBE)
#define ADNS9800_FIRMWARE_CRCLO (0xEF)
#endif

/*ADNS9800_H_*/


