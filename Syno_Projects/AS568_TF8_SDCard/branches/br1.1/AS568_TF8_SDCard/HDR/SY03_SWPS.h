/***************************************************************************
* File name    :	SY03_SWPS.h
* Function     :	Header of SY03 SWP Slave Controller registers
* Author       : 	coins
* Date         :	2012/08/22
* Version      :    v1.0
* Description  :    the SWPS register header for SY03.
* ModifyRecord :
****************************************************************************/
#ifndef SY03_SWPS_H_
#define SY03_SWPS_H_

/***************************************************************************
* Include Header Files
***************************************************************************/

/**************************************************************************
* Global Macro Definition
***************************************************************************/
#define	SWPS_INT_TX_DONE	(1<<0)
#define	SWPS_INT_RX_SOF		(1<<8)
#define	SWPS_INT_RX_DONE	(1<<9)
#define	SWPS_INT_RESUME		(1<<24)
#define	SWPS_INT_SUSPEND	(1<<25)
#define	SWPS_INT_DEACTIVE	(1<<26)
#define	SWPS_INT_CLF_START	(1<<27)

/**************************************************************************
* Global Type Definition
***************************************************************************/
// SWPS Interrupt (Enable) Register Bit Definition
struct	SWPS_INT_BITS	{	//	bits	description
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
	UINT32	Suspend:1;		//	25		Suspend received
	UINT32	Deactive:1;		//	26		Deactive received
	UINT32	ClfStart:1;		// 	27		
	UINT32	Rsvd:4; 		//	31:28 	Reserved
};
union	SWPS_INT_REG	{
	UINT32	all;
	struct SWPS_INT_BITS	bit;
};

// SWPS Status Register Bit Definition
struct	SWPS_STATUS_BITS	{	//	bits	description
	UINT32	TxFifoEmpty:1;  			
	UINT32	TxFifoFull:1;				
	UINT32	RxFifoEmpty:1;			
	UINT32	RxFifoFull:1;
	UINT32	Rsvd0:4;				
	UINT32	TxInhibit:1;		
	UINT32	TxFifoUnderFlow:1;	
	UINT32	TxFrameErr:1;		
	UINT32	Rsvd1:5;			
	UINT32	RxFrameErr:1;		
	UINT32	RxCrcErr:1;			
	UINT32	RxEofNotFoundErr:1;	
	UINT32	RxFifoOverFlow:1;			
	UINT32	Rsvd2:12; 			
};
union	SWPS_STATUS_REG	{
	UINT32	all;
	struct SWPS_STATUS_BITS	bit;
};

// SWPS Control Register Bit Definition
struct	SWPS_CTRL_BITS	{	//	bits	description
	UINT32	TxEn:1;  		//	0		transmit enable
	UINT32	Rsvd1:3;		//	3:1		reserved
	UINT32	TxResumeBit:3;	// 	6:4		the bit 1 befor transmit SOF(reserved)	
	UINT32	TxFifoClr:1;	//	7		Tx FIFO clear
	UINT32	RxEn:1;			// 	8		receive	enable
	UINT32	SuspendIdleBit:3;// 11:9	Suspend idle bit number
	UINT32	Rsvd2:3;		// 14:12	Reserved
	UINT32	RxFifoClr:1;	//	15		Rx FIFO clear
	UINT32	Rsvd:16; 		//	31:13 	Reserved
};
union	SWPS_CTRL_REG	{
	UINT32	all;
	struct SWPS_CTRL_BITS	bit;
};

// SWPS Length Register Bit Definition
struct	SWPS_LEN_BITS	{	//	bits	description
	UINT32	TxLen:8;  		//	7:0		transmit length
	UINT32	RxLen:8;		//	15:8	receive length
	UINT32	Rsvd:16; 		//	31:16 	Reserved
};
union	SWPS_LEN_REG	{
	UINT32	all;
	struct SWPS_LEN_BITS	bit;
};

// SWPS S1 Period Register Bit Definition
struct	SWPS_S1_PERIOD_BITS	{	//	bits	description
	UINT32	MaxHighTime:16;  	//	9:0		S1 maximum high level duration time
	UINT32	MaxLowTime:16;		//	25:16	S1 maximum low level duration time
};
union	SWPS_S1_PERIOD_REG	{
	UINT32	all;
	struct SWPS_S1_PERIOD_BITS	bit;
};


// SWPS regs
struct	SWPS_REGS	{
	union SWPS_INT_REG	INT;
	union SWPS_INT_REG	IE;
	union SWPS_STATUS_REG	STATUS;
	union SWPS_CTRL_REG	CTRL;
	union SWPS_LEN_REG 	LEN;
	UINT32	TX_FIFO;
	UINT32	RX_FIFO;
	UINT32	SUSPEND_TM;		// 13bit
	UINT32	DEACTIVATED_TM;	// 14bit
	UINT32	INHIBIT_TM;	   	// 23bit
	union SWPS_S1_PERIOD_REG	S1_PERIOD; 	// 11bit
};


/**************************************************************************
* Global Variable Declaration
***************************************************************************/
// Definition of SWPS register variable
extern volatile struct SWPS_REGS SwpsRegs;

/**************************************************************************
* Global Functon Declaration
***************************************************************************/


#endif
