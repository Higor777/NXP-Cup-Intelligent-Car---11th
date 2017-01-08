#include "common.h"
#include "gpio.h"
#include "dma.h"
#include "UMC.h"
#include "Eagle.h"
#include "sccb.h"
#include "uart.h"
#define OV7725_EAGLE_Delay_ms(time)  DelayMs(time)
#define Thre 30
float Threshold=Thre;//调节阈值使用

uint8   *ov7725_eagle_img_buff;
volatile IMG_STATUS_e      eagle_img_flag = IMG_FINISH;   //图像状态


//内部函数声明
static uint8 ov7725_eagle_reg_init(void);   //鹰眼 寄存器 初始化
static void ov7725_eagle_port_init(void);			//鹰眼 引脚 初始化
static void Eagle_dma_init(uint8_t chl);
GPIO_InitTypeDef GPIO_InitStruct1;
DMA_InitTypeDef DMA_InitStruct1;

void Eagle_Init(uint8 *imgaddr)
{
  ov7725_eagle_img_buff=imgaddr;
	while(ov7725_eagle_reg_init() == 0);
  ov7725_eagle_port_init();
}

/*!
 *  @brief      鹰眼ov7725管脚初始化（内部调用）
 *  @since      v5.0
 */
void ov7725_eagle_port_init(void)
{
		// 场中断 VSYNC 初始化
		PORT_PinMuxConfig(OV7725_VSYNC_PORT, OV7725_VSYNC_PIN, kPinAlt1);
		GPIO_InitStruct1.pinx = OV7725_VSYNC_PIN;
		GPIO_InitStruct1.mode = kGPIO_Mode_IPD;   
		GPIO_InitStruct1.instance = OV7725_VSYNC_PORT;
		GPIO_Init(&GPIO_InitStruct1);
		PORT_PinPassiveFilterConfig(OV7725_VSYNC_PORT, OV7725_VSYNC_PIN, ENABLE);
		GPIOPORT_IRQ_CLEAN(OV7725_VSYNC_PORT);
		PORT_PinPassiveFilterConfig(OV7725_VSYNC_PORT,OV7725_VSYNC_PIN,ENABLE);
		GPIO_ITDMAConfig(OV7725_VSYNC_PORT,OV7725_VSYNC_PIN,kGPIO_IT_RisingEdge,true);
		//注册中断服务函数
		GPIO_CallbackInstall(OV7725_VSYNC_PORT,Eagle_ISR);
		GPIO_IRQ_DIS(OV7725_VSYNC_PORT);
    //配置DMA初始化
    Eagle_dma_init(HW_DMA_CH0);
}

/*!
 *  @brief      鹰眼ov7725场中断服务函数
 *  @since      v5.0
 */
void Eagle_ISR(uint32_t index)
{

	  if((index&(1<<OV7725_VSYNC_PIN)))//场中断
		{		
			 //UART_printf("VSYNC\n");		
				//场中断需要判断是场结束还是场开始
				if(eagle_img_flag == IMG_START)                   //需要开始采集图像
				{
						eagle_img_flag = IMG_GATHER;                  //标记图像采集中
					  // UART_printf("IMG_GATHER\n");
						GPIO_IRQ_DIS(OV7725_VSYNC_PORT);  

						GPIOA_IRQ_CLEAN(OV7725_PCLK_PIN);             //清空PCLK标志位
						DMA_SetDestAddress(HW_DMA_CH0,(uint32)ov7725_eagle_img_buff);    //恢复地址
						DMA_IRQ_CLEAN(HW_DMA_CH0);
						DMA_EnableRequest(HW_DMA_CH0);
						DMA_EN(HW_DMA_CH0);                //使能通道CHn 硬件请求
					
				}
				else                                        //图像采集错误
				{
						GPIO_IRQ_DIS(OV7725_VSYNC_PORT);                       //关闭PTA的中断
						eagle_img_flag = IMG_FAIL;                    //标记图像采集失败
					//UART_printf("IMG_FAIL\n");
				}			
		}
} 



void DMA_ISR(void)
{
	    eagle_img_flag = IMG_FINISH ;
	    DMA_IRQ_CLEAN(HW_DMA_CH0);           //清除通道传输中断标志位
	   // UART_printf("DMAFINISH\n");
	    
}

/*!
 *  @brief      鹰眼ov7725采集图像（采集到的数据存储在 初始化时配置的地址上）
 *  @since      v5.0
 */
 void eagle_get_img()
 {
	 //等待上一次图像采集完成，这一段本来是在eagle_get_img内紧跟在开始采集图像后的，现在把它放在采集之前检测上一次图像是否采集完成，这样可以提高采集效率，即DMA采集与主循环并行执行
	 while(eagle_img_flag != IMG_FINISH)               //等待图像采集完毕
	 {
		 if(eagle_img_flag == IMG_FAIL)                  //假如图像采集错误，则重新开始采集
		 {
			 eagle_img_flag = IMG_START;                   //开始采集图像
			 GPIOPORT_IRQ_CLEAN(OV7725_VSYNC_PORT);        //写1清中断标志位(必须的，不然回导致一开中断就马上触发中断)
			 GPIO_IRQ_EN(OV7725_VSYNC_PORT);               //允许PTA的中断
		 }
	 }
	 //开始采集下一帧图像
	 eagle_img_flag = IMG_START;                 				   //开始采集图像
	 GPIOPORT_IRQ_CLEAN(OV7725_VSYNC_PORT);                //写1清中断标志位(必须的，不然回导致一开中断就马上触发中断)
	 GPIO_IRQ_EN(OV7725_VSYNC_PORT);                       //允许PTA的中断
 }



/*OV7725初始化配置表*/
reg_s ov7725_eagle_reg[] =
{
    //寄存器，寄存器值次
    {OV7725_COM4         , 0xC1},
    {OV7725_CLKRC        , 0x02},//50hz
    {OV7725_COM2         , 0x03},
    {OV7725_COM3         , 0xD0},
    {OV7725_COM7         , 0x40},
    {OV7725_HSTART       , 0x3F},
    {OV7725_HSIZE        , 0x50},
    {OV7725_VSTRT        , 0x03},
    {OV7725_VSIZE        , 0x78},
    {OV7725_HREF         , 0x00},
    {OV7725_SCAL0        , 0x0A},
    {OV7725_AWB_Ctrl0    , 0xE0},
    {OV7725_DSPAuto      , 0xff},
    {OV7725_DSP_Ctrl2    , 0x0C},
    {OV7725_DSP_Ctrl3    , 0x00},
    {OV7725_DSP_Ctrl4    , 0x00},

#if (OV7725_W == 80)
    {OV7725_HOutSize     , 0x14},
#elif (OV7725_W == 160)
    {OV7725_HOutSize     , 0x28},
#elif (OV7725_W == 240)
    {OV7725_HOutSize     , 0x3c},
#elif (OV7725_W == 320)
    {OV7725_HOutSize     , 0x50},
#else

#endif

#if (OV7725_H == 60 )
    {OV7725_VOutSize     , 0x1E},
#elif (OV7725_H == 120 )
    {OV7725_VOutSize     , 0x3c},
#elif (OV7725_H == 180 )
    {OV7725_VOutSize     , 0x5a},
#elif (OV7725_H == 240 )
    {OV7725_VOutSize     , 0x78},
#else

#endif

    {OV7725_EXHCH        , 0x00},
    {OV7725_GAM1         , 0x0c},
    {OV7725_GAM2         , 0x16},
    {OV7725_GAM3         , 0x2a},
    {OV7725_GAM4         , 0x4e},
    {OV7725_GAM5         , 0x61},
    {OV7725_GAM6         , 0x6f},
    {OV7725_GAM7         , 0x7b},
    {OV7725_GAM8         , 0x86},
    {OV7725_GAM9         , 0x8e},
    {OV7725_GAM10        , 0x97},
    {OV7725_GAM11        , 0xa4},
    {OV7725_GAM12        , 0xaf},
    {OV7725_GAM13        , 0xc5},
    {OV7725_GAM14        , 0xd7},
    {OV7725_GAM15        , 0xe8},
    {OV7725_SLOP         , 0x20},
    {OV7725_LC_RADI      , 0x00},
    {OV7725_LC_COEF      , 0x13},
    {OV7725_LC_XC        , 0x08},
    {OV7725_LC_COEFB     , 0x14},
    {OV7725_LC_COEFR     , 0x17},
    {OV7725_LC_CTR       , 0x05},
    {OV7725_BDBase       , 0x99},
    {OV7725_BDMStep      , 0x03},
    {OV7725_SDE          , 0x04},
    {OV7725_BRIGHT       , 0x00},
    {OV7725_CNST         , Thre},
    {OV7725_SIGN         , 0x06},
    {OV7725_UVADJ0       , 0x11},
    {OV7725_UVADJ1       , 0x02},

};



 
void Eagle_dma_init(uint8_t chl)
{
	    uint8_t i=0;
			/*  配置输入源   */
			//数据IO口初始化
			for(i=0;i<8;i++)
			{
				PORT_PinMuxConfig(HW_GPIOB, i, kPinAlt1);
				GPIO_InitStruct1.pinx = i;
				GPIO_InitStruct1.mode = kGPIO_Mode_IPD;
				GPIO_InitStruct1.instance = HW_GPIOB;
				GPIO_Init(&GPIO_InitStruct1);
			}
	    //配置触发源（默认是 上升沿触发）
			//像素中断 PCLK 初始化
			PORT_PinMuxConfig(OV7725_PCLK_PORT, OV7725_PCLK_PIN, kPinAlt1);
			GPIO_InitStruct1.pinx = OV7725_PCLK_PIN;
			GPIO_InitStruct1.mode = kGPIO_Mode_IPU;
			GPIO_InitStruct1.instance = OV7725_PCLK_PORT;
			GPIO_Init(&GPIO_InitStruct1);
			GPIO_ITDMAConfig(OV7725_PCLK_PORT,OV7725_PCLK_PIN,kGPIO_DMA_FallingEdge,true);
			//DMA 寄存器 配置
			DMA_DIS(chl);     //失能通道CHn 硬件请求
			/* clear some register */
			DMA0->TCD[chl].ATTR  = 0;
			DMA0->TCD[chl].CSR   = 0;
			/* 开启时钟 */
			SIM->SCGC7 |= SIM_SCGC7_DMA_MASK;                        //打开DMA模块时钟
			SIM->SCGC6 |= SIM_SCGC6_DMAMUX_MASK;                     //打开DMA多路复用器时钟
			/* 配置 DMA 通道 的 传输控制块 TCD ( Transfer Control Descriptor ) */
			DMA0->TCD[chl].SADDR =  (uint32_t)&PTB->PDIR;                      // 设置  源地址
			DMA0->TCD[chl].DADDR =  (uint32_t)ov7725_eagle_img_buff;          // 设置目的地址
			DMA0->TCD[chl].SOFF  =    0;                              // 设置源地址偏移 = 0x0, 即不变
			DMA0->TCD[chl].DOFF  =    1;                              // 每次传输后，目的地址加 BYTEs
			DMA0->TCD[chl].ATTR  =    (0
													 | DMA_ATTR_SMOD(0x0u)               // 源地址模数禁止  Source address modulo feature is disabled
													 | DMA_ATTR_SSIZE(0)            // 源数据位宽 ：DMA_BYTEn  。    SSIZE = 0 -> 8-bit ，SSIZE = 1 -> 16-bit ，SSIZE = 2 -> 32-bit ，SSIZE = 4 -> 16-byte
													 | DMA_ATTR_DMOD(0x0u)               // 目标地址模数禁止
													 | DMA_ATTR_DSIZE(0)            // 目标数据位宽 ：DMA_BYTEn  。  设置参考  SSIZE
													);
			DMA0->TCD[chl].CITER_ELINKNO  = DMA_CITER_ELINKNO_CITER(OV7725_W*OV7725_H/8);   //当前主循环次数
			DMA0->TCD[chl].BITER_ELINKNO  = DMA_BITER_ELINKNO_BITER(OV7725_W*OV7725_H/8);   //起始主循环次数
			DMA0->CR &= ~DMA_CR_EMLM_MASK;                                // CR[EMLM] = 0
			//当CR[EMLM] = 0 时:
			DMA0->TCD[chl].NBYTES_MLNO =   DMA_NBYTES_MLNO_NBYTES(1); // 通道每次传输字节数，这里设置为BYTEs个字节。注：值为0表示传输4GB */
			//注册中断服务函数
			DMA_CallbackInstall(HW_DMA_CH0,DMA_ISR);
			/* 配置 DMA 传输结束后的操作 */
			DMA0->TCD[chl].SLAST      =   0;                              //调整  源地址的附加值,主循环结束后恢复  源地址
			DMA0->TCD[chl].DLAST_SGA  =   (uint32)(0); //调整目的地址的附加值,主循环结束后恢复目的地址或者保持地址//保持原地址
			DMA0->TCD[chl].CSR        =   (0
															 | DMA_CSR_BWC(3)               //带宽控制,每读一次，eDMA 引擎停止 8 个周期（0不停止；1保留；2停止4周期；3停止8周期）
															 | DMA_CSR_DREQ_MASK            //主循环结束后停止硬件请求
															 | DMA_CSR_INTMAJOR_MASK        //主循环结束后产生中断
															);

			/* 配置 DMA 触发源 */
			DMAMUX_InstanceTable[0]->CHCFG[chl] &= ~DMAMUX_CHCFG_TRIG_MASK;
			DMAMUX_InstanceTable[0]->CHCFG[chl] = (0
																									 | DMAMUX_CHCFG_ENBL_MASK                        /* Enable routing of DMA request */
																									 //| DMAMUX_CHCFG_TRIG_MASK                        /* Trigger Mode: Periodic   PIT周期触发传输模式   通道1对应PIT1，必须使能PIT1，且配置相应的PIT定时触发 */
																									 | DMAMUX_CHCFG_SOURCE( PORTA_DMAREQ) /* 通道触发传输源:     */
																							 );									
			/* enable DMAMUX */
	  	DMAMUX_InstanceTable[0]->CHCFG[chl] |= DMAMUX_CHCFG_ENBL_MASK;
			DMA_IRQ_CLEAN(chl);
			/* 开启中断 */
			DMA_EN(chl);                                    //使能通道CHn 硬件请求
			DMA_IRQ_EN(chl);                                //允许DMA通道传输
}




uint8 ov7725_eagle_cfgnum = ARR_SIZE( ov7725_eagle_reg ) ; /*结构体数组成员数目*/





/*!
 *  @brief      鹰眼ov7725寄存器 初始化
 *  @return     初始化结果（0表示失败，1表示成功）
 *  @since      v5.0
 */
uint8 ov7725_eagle_reg_init(void)
{
    uint16 i = 0;
    uint8 Sensor_IDCode = 0;
    SCCB_GPIO_init();
    OV7725_EAGLE_Delay_ms(50);
    if( 0 == SCCB_WriteByte ( OV7725_COM7, 0x80 ) ) /*复位sensor */
    {
        //UART_printf("警告:SCCB写数据错误\n");
        return 0 ;
    }

    OV7725_EAGLE_Delay_ms(50);

    if( 0 == SCCB_ReadByte( &Sensor_IDCode, 1, OV7725_VER ) )    /* 读取sensor ID号*/
    {
        //UART_printf("警告:读取ID失败\n");
        return 0;
    }
    //UART_printf("Get ID success,SENSOR ID is 0x%x\n", Sensor_IDCode);
    //UART_printf("Config Register Number is %d \n" ,ov7725_eagle_cfgnum);
    if(Sensor_IDCode == OV7725_ID)
    {
        for( i = 0 ; i<ov7725_eagle_cfgnum ; i++ )
        {
            if( 0 == SCCB_WriteByte(ov7725_eagle_reg[i].addr, ov7725_eagle_reg[i].val) )
            {
                //UART_printf("警告:写寄存器0x%x失败\n", ov7725_eagle_reg[i].addr);
                return 0;
            }
        }
    }
    else
    {
        return 0;
    }
    //UART_printf("OV7725 Register Config Success!\n");
    return 1;
}





//压缩二值化图像解压（空间 换 时间 解压）
//srclen 是二值化图像的占用空间大小
void img_extract(uint8_t *dst, uint8_t *src, uint32_t srclen)
{
    uint8_t colour[2] = {255, 0}; //0 和 1 分别对应的颜色
    //注：山外的摄像头 0 表示 白色，1表示 黑色
    uint8_t tmpsrc;
		
		
		
		
		
		
    while(srclen --)
    {
        tmpsrc = *src++;
        *dst++ = colour[ (tmpsrc >> 7 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 6 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 5 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 4 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 3 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 2 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 1 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 0 ) & 0x01 ];
    }
		

}


