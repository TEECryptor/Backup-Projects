/****************************************************************************
* File name    :	SY03_TMR.h
* Function     :	Header of SY03 timer Registers
* Author       : 	Leixj
* Date         :	2012/06/26
* Version      :    v0.1
* Description  :    the timer register header for SY03
* ModifyRecord :
****************************************************************************/
#ifndef SY03_TMR_H_
#define SY03_TMR_H_

/***************************************************************************
* Include Header Files
***************************************************************************/

/**************************************************************************
* Global Macro Definition
***************************************************************************/
// Define bits of TIMER_CONTROL
#define	CONTROL_WKEY	(0x67<<24)
#define CONTROL_WUFLAG	(0x55<<16)

#define	CONTROL_CLKSEL	6
#define CONTROL_EN		4
#define CONTROL_IP		3
#define CONTROL_WDT		2
#define CONTROL_IE		0

/**************************************************************************
* Global Type Definition
***************************************************************************/
// Timer Control Register Bit Definition
struct	TIMER_CONTROL_BITS	{// bits	description
	UINT32	Ie:1;			
	UINT32	Rsvd0:1;			
	UINT32	Wdt:1;		
	UINT32	Ip:1;		
	UINT32	En:1;		
	UINT32	Rsvd1:1;		
	UINT32	ClkSel:2;		
	UINT32	Rsvd2:8;
	UINT32  Wuflag:8;
	UINT32	Wkey:8;		
};
union 	TIMER_CONTROL_REG	{
	UINT32	all;
	struct TIMER_CONTROL_BITS	bit;
};

// Timer Registers
struct TMR_REGS{
	UINT32	COUNT;
	union	TIMER_CONTROL_REG CONTROL;
	UINT32	LIMIT;
};

/**************************************************************************
* Global Variable Declaration
***************************************************************************/
extern 	volatile struct TMR_REGS T0Regs;
extern	volatile struct TMR_REGS T1Regs;
extern	volatile struct TMR_REGS T2Regs;


/**************************************************************************
* Global Functon Declaration
***************************************************************************/


#endif

