/****************************************************************************
* File name    :	SY03_ACMP.h
* Function     :	Header of SY03 ACMP Registers
* Author       : 	Leixj
* Date         :	2012/06/29
* Version      :    v1.0
* Description  :    the ACMP register header for SY03
* ModifyRecord :
****************************************************************************/
#ifndef SY03_ACMP_H_
#define SY03_ACMP_H_

/***************************************************************************
* Include Header Files
***************************************************************************/

/**************************************************************************
* Global Macro Definition
***************************************************************************/


/**************************************************************************
* Global Type Definition
***************************************************************************/
// ACMP CTRL Register Bit Definition
struct	ACMP_CTRL_BITS	{// bits	description
	UINT32	En:1;		 // 0		Enable ACMP	
	UINT32	IntEdge:1;	 // 1		Interrupt edge
	UINT32	IEn:1;		 // 2		Enable interrupt
	UINT32	RefSel:1;	 // 3		Reference voltage select
	UINT32	Rsvd:28;	 // 31:4		Reserved	
};
union 	ACMP_CTRL_REG	{
	UINT32	all;
	struct ACMP_CTRL_BITS	bit;
};

// ACMP STAT Register Bit Definition
struct	ACMP_STAT_BITS	{// bits	description
	UINT32	SyncOut:1;	 // 0		Result 	
	UINT32	IntFlag:1;	 // 1		Interrupt flag		
	UINT32	Rsvd:30;	 //	31:2	Reserved
};
union 	ACMP_STAT_REG	{
	UINT32	all;
	struct ACMP_STAT_BITS	bit;
};

// ACMP Registers
struct ACMP_REGS{
	union	ACMP_CTRL_REG CTRL;
	union	ACMP_STAT_REG STAT;
};

/**************************************************************************
* Global Variable Declaration
***************************************************************************/
extern volatile struct ACMP_REGS AcmpRegs;

/**************************************************************************
* Global Functon Declaration
***************************************************************************/

#endif

