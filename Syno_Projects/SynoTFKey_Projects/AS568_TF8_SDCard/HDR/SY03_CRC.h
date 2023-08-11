/***************************************************************************************
* File name    :	SY03_CRC.h
* Function     :	Header of SY03 CRC registers
* Author       : 	coins
* Date         :	2012/06/15
* Version      :    v1.0
* Description  :    the CRC generator register header for SY03
* ModifyRecord :
*****************************************************************************************/
#ifndef SY03_CRC_H_
#define SY03_CRC_H_

/***************************************************************************
* Include Header Files
***************************************************************************/


/**************************************************************************
* Global Macro Definition
***************************************************************************/


/**************************************************************************
* Global Type Definition
***************************************************************************/
// CRC control register Bit Definitions
struct	CRC_CTRL_BITS	{//	bits	description
	UINT32	ArithSel:1;		//  0		Algorithm Selection
	UINT32	DatRev:1;		//	1		Input data reverse
	UINT32	ResRev:1;		//	2		Output Descending
	UINT32	Rsvd1:29;		//	31:3	Reserved
};
union	CRC_CTRL_REG{
	UINT32	all;
	struct CRC_CTRL_BITS	bit;
};

// USART Registers
struct	CRC_REGS	{
	union 	CRC_CTRL_REG	CTRL;		//CRC control register					
	UINT32	DATA;			//CRC data registers
	UINT32	INITVAL;		//CRC initial value register			
	UINT32	RESULT;			//CRC results output register
};

/**************************************************************************
* Global Variable Declaration
***************************************************************************/
extern	volatile	struct CRC_REGS	CrcRegs;

/**************************************************************************
* Global Functon Declaration
***************************************************************************/

#endif /*SY03_CRC_H_*/
