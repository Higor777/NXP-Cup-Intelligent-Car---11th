#include "common.h"
#include "adns9800.h"
#include "spi.h"
#include  "gpio.h"

void adns9500_init(void)
{   

  	SPI_InitTypeDef SPI_InitStruct1;
    SPI_InitStruct1.baudrate = 750000;
    SPI_InitStruct1.frameFormat = kSPI_CPOL0_CPHA0;
    SPI_InitStruct1.dataSize = 8;
    SPI_InitStruct1.instance = HW_SPI1;
    SPI_InitStruct1.mode = kSPI_Master;
    SPI_InitStruct1.bitOrder = kSPI_MSB;
    SPI_InitStruct1.ctar = HW_CTAR0;
    /* init pinmux */
   
    PORT_PinMuxConfig(HW_GPIOB, 15, kPinAlt2); 
   
    /* init moudle */
    SPI_Init(&SPI_InitStruct1);
	
	
//  // Initialize SPI
//  adns9500_spi_init();

//  // Default configuration
//  ADNS9500_LOCK = 0;
//  ADNS9500_ENABLE = 0;
}


