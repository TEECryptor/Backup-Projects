/***************************************************************************
* File name    :	SY03_Device.h
* Function     :	Header of SY03 device to include all register struct
* 					and data type header
* Author       : 	coins
* Date         :	2012/06/15
* Version      :    v1.0
* Description  :
* ModifyRecord :
****************************************************************************/
#ifndef SY03_DEVICE_H_
#define SY03_DEVICE_H_



/**************************************************************************
* Global Macro Definition
***************************************************************************/
//#define	SIMULATE_FOR_IC

/* Configuration of the Cortex-M0 Processor and Core Peripherals */
#define __MPU_PRESENT           0       /*!< cm0ikmcu does not provide a MPU present or not       */
#define __NVIC_PRIO_BITS        2       /*!< cm0ikmcu Supports 2 Bits for the Priority Levels     */
#define __Vendor_SysTickConfig  0       /*!< Set to 1 if different SysTick Config is used         */

// define the HFROSC clock frequcency
#define		F_HFROSC	48000000UL	

// define the LFROSC clock frequency
#define		F_LFROSC	32000UL	


//--------------------------------------------------------------------------
// define the peripheral ID
//--------------------------------------------------------------------------
#define	ID_SCM		0
#define	ID_GPIO		1
#define	ID_T0		2
#define	ID_SEA		3
#define	ID_AEA		4
#define	ID_SPI0	 	5
#define	ID_TRNG		6
#define	ID_USB		7
#define	ID_T1		8
#define	ID_T2		9
#define	ID_SSI		10
#define	ID_UART0	11
#define	ID_I2C0		12
#define	ID_SWPS		13
#define	ID_SWPM		14
#define	ID_ISO7816	15
#define	ID_SCI		16
#define	ID_ACMP		17
#define	ID_VPWM		18
#define	ID_SDSI		19
#define	ID_RTC		20
#define	ID_EFC		21
#define	ID_SQI		22
#define	ID_CRC		23
#define	ID_BVD		24
#define	ID_RSVD		25
#define	ID_SDI		26
#define	ID_SPI1		27


// -------------------------------------------------------------------------
// define the memory-map-register base address
#define	MMR_BASE		0x40000000
#define	APB_BASE		MMR_BASE
#define	AHB_BASE		(MMR_BASE + 0x10000000)
#define	APB_SPACE		0x4000
#define	AHB_SPACE		0x10000

// APB
#define	SCM_BASE		(APB_BASE + APB_SPACE * ID_SCM)
#define	GPIO_BASE		(APB_BASE + APB_SPACE * ID_GPIO)
#define	T0_BASE			(APB_BASE + APB_SPACE * ID_T0)
#define	SEA_BASE		(APB_BASE + APB_SPACE * ID_SEA)
#define	AEA_BASE		(APB_BASE + APB_SPACE * ID_AEA)
#define	SPI0_BASE		(APB_BASE + APB_SPACE * ID_SPI0)
#define	TRNG_BASE		(APB_BASE + APB_SPACE * ID_TRNG)

#define	T1_BASE			(APB_BASE + APB_SPACE * ID_T1)
#define	T2_BASE			(APB_BASE + APB_SPACE * ID_T2)
#define	SSI_BASE		(APB_BASE + APB_SPACE * ID_SSI)
#define	UART0_BASE		(APB_BASE + APB_SPACE * ID_UART0)
#define	I2C0_BASE		(APB_BASE + APB_SPACE * ID_I2C0)
#define	SWPS_BASE		(APB_BASE + APB_SPACE * ID_SWPS)
#define	SWPM_BASE		(APB_BASE + APB_SPACE * ID_SWPM)
#define	ISO7816_BASE	(APB_BASE + APB_SPACE * ID_ISO7816)
#define	SCI_BASE		(APB_BASE + APB_SPACE * ID_SCI)
#define	ACMP_BASE		(APB_BASE + APB_SPACE * ID_ACMP)
#define	VPWM_BASE		(APB_BASE + APB_SPACE * ID_VPWM)

#define	RTC_BASE		(APB_BASE + APB_SPACE * ID_RTC)
#define	EFC_BASE		(APB_BASE + APB_SPACE * ID_EFC)
#define	SQI_BASE		(APB_BASE + APB_SPACE * ID_SQI)
#define	CRC_BASE		(APB_BASE + APB_SPACE * ID_CRC)
#define	BVD_BASE		(APB_BASE + APB_SPACE * ID_BVD)
#define	RSVD_BASE		(APB_BASE + APB_SPACE * ID_RSVD)
#define	SPI1_BASE		(APB_BASE + APB_SPACE * ID_SPI1)
// AHB
#define	SDSI_BASE		(AHB_BASE + AHB_SPACE * 0)
#define	USB_BASE		(AHB_BASE + AHB_SPACE * 1)	
#define	SDI_BASE		(AHB_BASE + AHB_SPACE * 2)	

/**************************************************************************
* Global Type Definition
***************************************************************************/
/*
 * ==========================================================================
 * ---------- Interrupt Number Definition -----------------------------------
 * ==========================================================================
 */

typedef enum IRQn
{
/******  Cortex-M0 Processor Exceptions Numbers ***************************************************/
  NonMaskableInt_IRQn       = -14,    /*!< 2 Non Maskable Interrupt                             */
  HardFault_IRQn	      	= -13,    /*!< 3 Cortex-M0 Hard Fault Interrupt                     */
  SVCall_IRQn              	= -5,     /*!< 11 Cortex-M0 SV Call Interrupt                       */
  PendSV_IRQn               = -2,     /*!< 14 Cortex-M0 Pend SV Interrupt                       */
  SysTick_IRQn              = -1,     /*!< 15 Cortex-M0 System Tick Interrupt                   */

/******  CM0IKMCU Cortex-M0 specific Interrupt Numbers ********************************************/
  SCM_IRQn                  = ID_SCM,      /*!< SCM Interrupt                                       */
                                      /*!< maximum of 32 Interrupts are possible                */
  GPIO_IRQn         		= ID_GPIO,
  T0_IRQn         			= ID_T0,
  SEA_IRQn         			= ID_SEA,
  AEA_IRQn         			= ID_AEA,
  SPI0_IRQn      			= ID_SPI0,
  TRNG_IRQn         		= ID_TRNG,
  USB_IRQn         			= ID_USB,
  T1_IRQn         			= ID_T1,
  T2_IRQn         			= ID_T2,
  SSI_IRQn         			= ID_SSI,
  UART0_IRQn         		= ID_UART0,
  I2C0_IRQn        		 	= ID_I2C0, 
  SWPS_IRQn         		= ID_SWPS,
  SWPM_IRQn         		= ID_SWPM,
  ISO7816_IRQn         		= ID_ISO7816,
  SCI_IRQn         			= ID_SCI,
  ACMP_IRQn         		= ID_ACMP,
  VPWM_IRQn         		= ID_VPWM,
  SDSI_IRQn         		= ID_SDSI,
  RTC_IRQn         			= ID_RTC,
  RSVD_IRQn         		= ID_RSVD,
  SDI_IRQn         			= ID_SDI,
  SPI1_IRQn         		= ID_SPI1,
} IRQn_Type;


/***************************************************************************
* Include Header Files
***************************************************************************/
// all peripheral header files
#include "Type.h"
#include "core_cm0.h"
#include "SY03_SCM.h"
#include "SY03_TMR.h"
#include "SY03_USART.h"
#include "SY03_GPIO.h"
#include "SY03_SCI.h"
#include "SY03_ACMP.h"
#include "SY03_SWPS.h"
#include "SY03_SWPM.h"
#include "SY03_EFC.h"
#include "SY03_SDSI.h"
#include "SY03_SQI.h"
#include "SY03_SSI.h"
#include "SY03_RNG.h"
#include "SY03_USB.h"
#include "SY03_ISO7816.h"
#include "SY03_VPWM.h"
#include "SY03_RTC.h"
#include "SY03_CRC.h"
#include "SY03_BVD.h"
#include "SY03_SDI.h"

#include "SY03_ROM.h"


/**************************************************************************
* Global Variable Declaration
***************************************************************************/


/**************************************************************************
* Global Functon Declaration
***************************************************************************/

#endif /*SY03_DEVICE_H_*/
