/***************************************************************************
* File name    :	SY03.h
* Function     :	Header of SY03
* Author       : 	coins
* Date         :	2012/06/18
* Version      :    v1.0
* Description  :	to include all header files about SY03, such as memory-map-
*					register header, drive
* ModifyRecord :
****************************************************************************/
#ifndef SY03_H_
#define SY03_H_

/***************************************************************************
* Include Header Files
***************************************************************************/

#include "SY03_Device.h"
#include "stdio.h"
#include "string.h"
#include <alloca.h>

#include "Efc.h"
#include "SFlash.h"
#include "XFlash.h"
#include "Sci.h"
#include "Scm.h"
#include "Sqi.h"
#include "Ssi.h"
#include "Swps.h"
#include "Swpm.h"
#include "Gpio.h"
#include "Tmr.h"
#include "Spi.h"
#include "Uart.h"
#include "I2c.h"
#include "Acmp.h"
#include "Sdsi.h"
#include "Iso7816.h"
#include "Vpwm.h"
#include "Rtc.h"
#include "Crc.h"
#include "Bvd.h"
#include "Trng.h" 
#include "Sdi.h" 
#include "Usb.h"
#include "Alg.h"
#include "RomFunc.h"
/**************************************************************************
* Global Macro Definition
***************************************************************************/

#define	FWLIB_VERSION		0x0112	// Version, BCD code, nn.mm	


// define the function name for old firmware library revision
#define print printf
#define _nop  __nop

/**************************************************************************
* Global Type Definition
***************************************************************************/


/**************************************************************************
* Global Variable Declaration
***************************************************************************/


/**************************************************************************
* Global Functon Declaration
***************************************************************************/
/****************************************************************************
 * Subroutine:	FWLIB_GetVersion
 * Function:	to get firmware lib version 
 * Input:		None 
 * Output:		unsigned short	
 * Description:	the version is BCD code, such as 0x0010, is version 0.10 
 * Date:		2013.03.19
 * ModifyRecord:
 * *************************************************************************/
__INLINE unsigned short FWLIB_GetVersion(void)
{
	return (FWLIB_VERSION);
}


#endif /*SY03_H_*/

