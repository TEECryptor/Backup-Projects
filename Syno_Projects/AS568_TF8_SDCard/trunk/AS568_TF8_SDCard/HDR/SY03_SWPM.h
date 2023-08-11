/***************************************************************************
* File name    :	SY03_SWPM.h
* Function     :	Header of SY03 SWP Master Controller registers
* Author       : 	coins
* Date         :	2012/11/29
* Version      :    v1.0
* Description  :    the SWPM register header for SY03.
* ModifyRecord :
****************************************************************************/
#ifndef SY03_SWPM_H_
#define SY03_SWPM_H_

/***************************************************************************
* Include Header Files
***************************************************************************/

/**************************************************************************
* Global Macro Definition
***************************************************************************/

/**************************************************************************
* Global Type Definition
***************************************************************************/
// SWPM Interrupt (Enable) Register Bit Definition
struct	SWPM_INT_BITS	{	//	bits	description
	UINT32	TxDone:1;  		//	0		transmit over
	UINT32	Rsvd0:1;		//	1		reserved
	UINT32	TxFifoNotFull:1;// 	2		transmit FIFO not full
	UINT32	Rsvd1:5;
	UINT32	RxSof:1;		// 	8		SOF received
	UINT32	RxDone:1;		// 	9		receive over
	UINT32	Rsvd2:3;		//	12:10	reserved
	UINT32	RxFifoNotEmpty:1;//	13		receive FIFO not empty
	UINT32	Rsvd3:10;		// 	23:14	reserved
	UINT32	Resume:1;		// 	24		Resume received
	UINT32	Rsvd:7; 		//	31:25 	Reserved
};
union	SWPM_INT_REG	{
	UINT32	all;
	struct SWPM_INT_BITS	bit;
};

// SWPM Status Register Bit Definition
struct	SWPM_STATUS_BITS	{	//	bits	description
	UINT32	TxFifoEmpty:1;  			
	UINT32	TxFifoFull:1;				
	UINT32	RxFifoEmpty:1;			
	UINT32	RxFifoFull:1;
	UINT32	Rsvd0:5;				
	UINT32	TxFifoUnderFlow:1;	
	UINT32	Rsvd1:7;			
	UINT32	RxCrcErr:1;			
	UINT32	RxEofNotFoundErr:1;	
	UINT32	RxFifoOverFlow:1;			
	UINT32	Rsvd2:12; 			
};
union	SWPM_STATUS_REG	{
	UINT32	all;
	struct SWPM_STATUS_BITS	bit;
};

// SWPM Control Register Bit Definition
struct	SWPM_CTRL_BITS	{	//	bits	description
	UINT32	TxEn:1;  		//	0		transmit enable
	UINT32	Rsvd1:6;		//	6:1		reserved
	UINT32	TxFifoClr:1;	//	7		Tx FIFO clear
	UINT32	RxEn:1;			// 	8		receive	enable
	UINT32	SuspendIdleBit:3;// 11:9	Suspend idle bit number
	UINT32	Rsvd2:3;		// 14:12	Reserved
	UINT32	RxFifoClr:1;	//	15		Rx FIFO clear
	UINT32	Suspend:1;		// 	16		Suspend
	UINT32	Deactive:1;		// 	17		Deactive
	UINT32	Rsvd:14; 		//	31:18 	Reserved
};
union	SWPM_CTRL_REG	{
	UINT32	all;
	struct SWPM_CTRL_BITS	bit;
};

// SWPM Length Register Bit Definition
struct	SWPM_LEN_BITS	{	//	bits	description
	UINT32	TxLen:6;  		//	5:0		transmit length
	UINT32	Rsvd1:2; 		//	7:6 	Reserved
	UINT32	RxLen:8;		//	15:8	receive length
	UINT32	Rsvd2:16; 		//	31:16 	Reserved
};
union	SWPM_LEN_REG	{
	UINT32	all;
	struct SWPM_LEN_BITS	bit;
};




// SWPM regs
struct	SWPM_REGS	{
	union SWPM_INT_REG	INT;
	union SWPM_INT_REG	IE;
	union SWPM_STATUS_REG	STATUS;
	union SWPM_CTRL_REG	CTRL;
	union SWPM_LEN_REG 	LEN;
	UINT32	TX_FIFO;
	UINT32	RX_FIFO;
	UINT32	RSVD1;		
	UINT32	RSVD2;	
	UINT32	RESUME_TM;	   	// 23bit
	UINT32	S1_PERIOD; 		// 13bit
};


/**************************************************************************
* Global Variable Declaration
***************************************************************************/
// Definition of SWPM register variable
extern volatile struct SWPM_REGS SwpmRegs;

/**************************************************************************
* Global Functon Declaration
***************************************************************************/








#endif
