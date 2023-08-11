/****************************************************************************
* File name    :	SY06_Regs.c
* Function     :	the peripheral registers viarible defined
* Author       : 	coins
* Date         :	2012/10/09
* Version      :    v1.0
* Description  :
* ModifyRecord :
****************************************************************************/
/***************************************************************************
* Include Header Files
***************************************************************************/
#include "SY03_Device.h"


/**************************************************************************
* Local Macro Definition
***************************************************************************/

/**************************************************************************
* Local Type Definition
***************************************************************************/


/**************************************************************************
* Local static Variable Declaration
***************************************************************************/


/**************************************************************************
* Global Variable Declaration
***************************************************************************/
volatile struct SCM_REGS ScmRegs 		__attribute__((at(SCM_BASE)));
volatile struct GPIO_REGS GpioRegs 		__attribute__((at(GPIO_BASE)));
volatile struct TMR_REGS T0Regs		__attribute__((at(T0_BASE)));
//volatile struct SEA_REGS SeaRegs		__attribute__((at(SEA_BASE)));
//volatile struct AEA_REGS RsaRegs		__attribute__((at(AEA_BASE)));
volatile struct USART_REGS Spi0Regs		__attribute__((at(SPI0_BASE)));
volatile struct RNG_REGS RngRegs		__attribute__((at(TRNG_BASE)));
//
volatile struct TMR_REGS T1Regs			__attribute__((at(T1_BASE)));
volatile struct TMR_REGS T2Regs			__attribute__((at(T2_BASE)));
volatile struct SSI_REGS SsiRegs		__attribute__((at(SSI_BASE)));
volatile struct USART_REGS Uart0Regs	__attribute__((at(UART0_BASE)));
volatile struct USART_REGS I2C0Regs		__attribute__((at(I2C0_BASE)));
volatile struct SWPS_REGS SwpsRegs		__attribute__((at(SWPS_BASE)));
volatile struct SWPM_REGS SwpmRegs		__attribute__((at(SWPM_BASE)));
volatile struct ISO7816_REGS Iso7816Regs	__attribute__((at(ISO7816_BASE)));
volatile struct SCI_REGS SciRegs		__attribute__((at(SCI_BASE)));
volatile struct ACMP_REGS AcmpRegs		__attribute__((at(ACMP_BASE)));
volatile struct VPWM_REGS VpwmRegs		__attribute__((at(VPWM_BASE)));

volatile struct RTC_REGS RtcRegs		__attribute__((at(RTC_BASE)));
volatile struct EFC_REGS EfcRegs		__attribute__((at(EFC_BASE)));
volatile struct SQI_REGS SqiRegs		__attribute__((at(SQI_BASE)));
volatile struct CRC_REGS CrcRegs		__attribute__((at(CRC_BASE)));
volatile struct BVD_REGS BvdRegs		__attribute__((at(BVD_BASE)));
volatile struct USART_REGS Spi1Regs		__attribute__((at(SPI1_BASE)));
// AHB
volatile struct SDSI_REGS SdsiRegs		__attribute__((at(SDSI_BASE)));
volatile struct USB_REGS UsbRegs		__attribute__((at(USB_BASE)));
volatile struct SDI_REGS SdiRegs		__attribute__((at(SDI_BASE)));

// SDSI ADMA2 Descriptor Table
volatile struct ADMA2_DESC_TBL SdsiADMA2DescTbl[32]	__attribute__((at(SDSI_ADMA2_DESC_TBL_ADDR)));


/**************************************************************************
* Local Functon Declaration
***************************************************************************/

/**************************************************************************
* Functon
***************************************************************************/


