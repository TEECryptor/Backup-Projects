/****************************************************************************
* File name    :	SY03_RTC.h
* Function     :	Header of SY03 Real Time Controller Registers
* Author       : 	coins
* Date         :	2012/06/28
* Version      :    v1.0
* Description  :    the RTC register header for SY03
* ModifyRecord :
****************************************************************************/
#ifndef SY03_RTC_H_
#define SY03_RTC_H_

/***************************************************************************
* Include Header Files
***************************************************************************/

/**************************************************************************
* Global Macro Definition
***************************************************************************/

/**************************************************************************
* Global Type Definition
***************************************************************************/
// RTC CTRL Bits Definition
struct	RTC_CTRL_BITS	{
	UINT32	En:1;
	UINT32	Rsvd0:7;
	UINT32	SecIntEn:1;
	UINT32	HSecIntEn:1;
	UINT32	MinIntEn:1;
	UINT32	Rsvd1:4;
	UINT32	IntEn:1;	
	UINT32	Rsvd3:16;
};
union	RTC_CTRL_REG	{
	UINT32	all;
	struct RTC_CTRL_BITS	bit;
};

struct	RTC_STA_BITS{
	UINT32	STAT:1;
	UINT32	Rsvd1:31;
};
union	RTC_STA_REG{
	UINT32	all;
	struct	RTC_STA_BITS	bit;
};
// RTC Registers
struct	RTC_REGS{
	UINT32	LOAD;
	union 	RTC_CTRL_REG	CTRL;
	union	RTC_STA_REG		STA;
	UINT32	CCVR;
};

/**************************************************************************
* Global Variable Declaration
***************************************************************************/
// Definition of RTC register variable
extern 	volatile struct RTC_REGS  RtcRegs;

/**************************************************************************
* Global Functon Declaration
***************************************************************************/

#endif /*SY03_RTC_H_*/
