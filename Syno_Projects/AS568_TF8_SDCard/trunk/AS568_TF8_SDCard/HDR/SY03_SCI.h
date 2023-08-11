/***************************************************************************
* File name    :	SY03_SCI.h
* Function     :	Header of SY03 Smart Card Interface registers
* Author       : 	coins
* Date         :	2012/07/21
* Version      :    v1.0
* Description  :    the SCI register header for SY03.
* ModifyRecord :
****************************************************************************/
#ifndef SY03_SCI_H_
#define SY03_SCI_H_


/***************************************************************************
* Include Header Files
***************************************************************************/

/**************************************************************************
* Global Macro Definition
***************************************************************************/

/**************************************************************************
* Global Type Definition
***************************************************************************/
// SCI Data Register Bit Definition
struct	SCI_DATA_BITS	{//	bits	description
	UINT32	Data:8;  	//	7:0		read or write data
	UINT32	ParityErr:1;//	8		parity error flag
	UINT32	Rsvd:23; 	//	31:9 	Reserved

};
union	SCI_DATA_REG	{
	UINT32	all;
	struct SCI_DATA_BITS	bit;
};

// SCI Control Register 0 Bit Definition
struct SCI_CR0_BITS {	//	bits	description
	UINT32  Sense :1;	//	0
	UINT32  Order :1;	// 	1
	UINT32  TxParity:1;	//	2
	UINT32  TxNak:1;	//	3
	UINT32  RxParity:1;	//	4
	UINT32  RxNak:1;	//	5
	UINT32  ClkDis:1;	//	6
	UINT32  ClkVal :1;	//	7
	UINT32  Rsvd:24;
};
union SCI_CR0_REG {
	UINT32 all;
	struct SCI_CR0_BITS bit;
};

// SCI Control Register 1 Bit Definition
struct SCI_CR1_BITS {
	UINT32  ATRDEn :1;
	UINT32  BlkEn :1;
	UINT32  Mode:1;
	UINT32  Clkz1:1;
	UINT32  BgtEn:1;
	UINT32  ExDbnce:1;
	UINT32  SyncCard:1;
	UINT32	CardSocket:1;
	UINT32  Rsvd:24;
};
union SCI_CR1_REG {
	UINT32 all;
	struct SCI_CR1_BITS bit;
};

// SCI Control Register 2 Bit Definition
struct SCI_CR2_BITS {
	UINT32  StartUp :1;
	UINT32  Finish :1;
	UINT32  WrReset:1;
	UINT32  Rsvd:29;
};
union SCI_CR2_REG {
	UINT32 all;
	struct SCI_CR2_BITS bit;
};

// SCI Transmit and Receive Tide Register Bit Definition
struct SCI_TIDE_BITS {
	UINT32  TxTide :4;
	UINT32  RxTide :4;
	UINT32  Rsvd:24;
};
union SCI_TIDE_REG {
	UINT32 all;
	struct SCI_TIDE_BITS bit;
};

// SCI Transmit and Receive Retry Register Bit Definition
struct SCI_RETRY_BITS {
	UINT32  TxRetry :3;
	UINT32  RxRetry :3;
	UINT32  Rsvd:26;
};
union SCI_RETRY_REG {
	UINT32 all;
	struct SCI_RETRY_BITS bit;
};

// SCI Transmit FIFO Count Register Bit Definition
struct SCI_FIFOSTATUS_BITS {
	UINT32  TxFF :1;
	UINT32  TxFE :1;
	UINT32  RxFF :1;
	UINT32  RxFE :1;
	UINT32  Rsvd:28;
};
union SCI_FIFOSTATUS_REG {
	UINT32 all;
	struct SCI_FIFOSTATUS_BITS bit;
};

// SCI Interrupt Mask/Status/Masked Status Register Bit Definition
struct SCI_INT_BITS {
	UINT32  Insert :1;
	UINT32  Remove :1;
	UINT32  PowerOn :1;
	UINT32  PowerOff :1;
	UINT32  TxErr :1;
	UINT32  ATRSTOut:1;
	UINT32  ATRDTOut:1;
	UINT32  BlkTout :1;
	UINT32  ChTOut :1;
	UINT32  RdTOut :1;
	UINT32  RxOv :1;
	UINT32  ClkStp :1;
	UINT32  ClkAct :1;
	UINT32  RxTide :1;
	UINT32  TxTide :1;
	UINT32  Rsvd:17;
};
union SCI_INT_REG {
	UINT32 all;
	struct SCI_INT_BITS bit;
};


//SCI Interrupt Clear Register Bit Definition
struct SCI_ICR_BITS {
	UINT32  Insert :1;
	UINT32  Remove :1;
	UINT32  PowerOn :1;
	UINT32  PowerOff :1;
	UINT32  TxErr :1;
	UINT32  ATRSTOut:1;
	UINT32  ATRDTOut:1;
	UINT32  BlkTout :1;
	UINT32  ChTOut :1;
	UINT32  RdTOut :1;
	UINT32  RxOv :1;
	UINT32  ClkStp :1;
	UINT32  ClkAct :1;
	UINT32  Rsvd:19;
};
union SCI_ICR_REG {
	UINT32 all;
	struct SCI_ICR_BITS bit;
};

// SCI Synchronous Card Activation Control Register Bit Definition
struct SCI_SYNCACT_BITS {
	UINT32  Power :1;
	UINT32  CRERst :1;
	UINT32  ClkEn :1;
	UINT32  DataEn :1;
	UINT32  FCB :1;
	UINT32  ClkoutEn :1;
	UINT32	ExClkEn	:1;
	UINT32  Clkout :1;
	UINT32  DataOutEn :1;
	UINT32  ExDataEn:1;
	UINT32  Present :1;
	UINT32  Rsvd:21;
};
union SCI_SYNCACT_REG {
	UINT32 all;
	struct SCI_SYNCACT_BITS bit;
};


// SCI Synchronous Transmit Clock and Data Register Bit Definition
struct SCI_SYNCTX_BITS {
	UINT32  WData :1;
	UINT32  WClk :1;
	UINT32  WDataEn :1;
	UINT32  WClkEn :1;
	UINT32  WRst :1;
	UINT32  WFCB :1;
	UINT32  Rsvd:26;
};
union SCI_SYNCTX_REG {
	UINT32 all;
	struct SCI_SYNCTX_BITS bit;
};

// SCI Synchronous Receive Clock and Data register Bit Definition
struct SCI_SYNCRX_BITS {
	UINT32  RData :1;
	UINT32  RClk :1;
	UINT32  Rsvd:30;
};
union SCI_SYNCRX_REG {
	UINT32 all;
	struct SCI_SYNCRX_BITS bit;
};

//SCI regs
struct	SCI_REGS	{
	union SCI_DATA_REG	DATA;
	union SCI_CR0_REG	CR0;
	union SCI_CR1_REG	CR1;
	union SCI_CR2_REG	CR2;
	UINT32	CLKICC;
	UINT32  VALUE;
	UINT32	BAUD;
	union SCI_TIDE_REG	TIDE;
	UINT32 	Rsvd1;		// Reserved for DMA control
	UINT32  STABLE;
	UINT32	ATIME;
	UINT32	DTIME;
	UINT32 	ATRSTIME;
	UINT32	ATRDTIME;
	UINT32  STOPTIME;
	UINT32	STARTTIME;
	union SCI_RETRY_REG	RETRY;
	UINT32	CHTIMELS;
	UINT32	CHTIMEMS;
	UINT32  BLKTIMELS;
	UINT32  BLKTIMEMS;
	UINT32  CHGUARD;
	UINT32	BLKGUARD;
	UINT32  RXTIME;
	union SCI_FIFOSTATUS_REG 	FIFOSTATUS;
	UINT32  TXCOUNT;
    UINT32	RXCOUNT;
    union SCI_INT_REG	IMSC;
    union SCI_INT_REG   RIS;
    union SCI_INT_REG   MIS;
    union SCI_ICR_REG   ICR;
    union SCI_SYNCACT_REG  	SYNCACT;
    union SCI_SYNCTX_REG    SYNCTX;
    union SCI_SYNCRX_REG	SYNCRX;
};

/**************************************************************************
* Global Variable Declaration
***************************************************************************/
// Definition of SCI register variable
extern 	volatile struct SCI_REGS  SciRegs;
/**************************************************************************
* Global Functon Declaration
***************************************************************************/

#endif /*SY03_SCI_H_*/
