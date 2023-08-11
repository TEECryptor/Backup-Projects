/***************************************************************************
* File name    :	SY03_SSI.h
* Function     :	Header of SY03 SSI (Synchronous Serial Interface) registers
* Author       : 	coins
* Date         :	2012/12/20
* Version      :    v1.0
* Description  :    the SSI register header for SY03.
* ModifyRecord :
****************************************************************************/
#ifndef SY03_SSI_H_
#define SY03_SSI_H_

/***************************************************************************
* Include Header Files
***************************************************************************/

/**************************************************************************
* Global Macro Definition
***************************************************************************/

/**************************************************************************
* Global Type Definition
***************************************************************************/
// SSI Control Register 0 Bit Definition
struct	SSI_CTRLR0_BITS	{	//	bits	description
	UINT32	Dfs:4;  		//	3:0		Data Frame Size
	UINT32	Frf:2;			//	5:4		Frame Format
	UINT32	Scph:1;			// 	6		Serial Clock Phase
	UINT32	Scpol:1;		//	7		Serial Clock Polarity
	UINT32	Tmod:2;			// 	9:8		Transfer Mode
	UINT32	SlvOe:1;		// 	10		Slave Output Enable
	UINT32	Srl:1;			//	11		Shift Register Loop
	UINT32	Cfs:4;			//	15:12	Control Frame Size
	UINT32	Rsvd:16; 		//	31:16 	Reserved
};
union	SSI_CTRLR0_REG	{
	UINT32	all;
	struct SSI_CTRLR0_BITS	bit;
};

// SSI Control Register 1 Bit Definition
struct	SSI_CTRLR1_BITS	{	//	bits	description
	UINT32	Ndf:16;  		//	15:0	Number of Data Frame
	UINT32	Rsvd:16; 		//	31:16 	Reserved
};
union	SSI_CTRLR1_REG	{
	UINT32	all;
	struct SSI_CTRLR1_BITS	bit;
};

// SSI Microwire Control Register Bit Definition
struct	SSI_MWCR_BITS	{	//	bits	description
	UINT32	MwMod:1;  		//	0		Microwire Transfer Mode
	UINT32	Mod:1;  		//	1		Microwire Control
	UINT32	Mhs:1;  		//	2		Microwire Handshaking
	UINT32	Rsvd:29; 		//	31:3 	Reserved
};
union	SSI_MWCR_REG	{
	UINT32	all;
	struct SSI_MWCR_BITS	bit;
};

// SSI Status Register Bit Definition
struct	SSI_SR_BITS	{	//	bits	description
	UINT32	Busy:1;  		//	0		SSI Busy Flag
	UINT32	Tfnf:1;			//	1		Transmit FIFO Not Full
	UINT32	Tfe:1;			// 	2		Transmit FIFO Empty
	UINT32	Rfne:1;			//	3		Receive FIFO Not Empty
	UINT32	Rff:1;			// 	4		Receive FIFO Full
	UINT32	Txe:1;			// 	5		Transmission Error
	UINT32	Dcol:1;			//	6		Data Collision Error
	UINT32	Rsvd:25; 		//	31:7 	Reserved
};
union	SSI_SR_REG	{
	UINT32	all;
	struct SSI_SR_BITS	bit;
};

// SSI Interrupt Register Bit Definition
struct	SSI_INT_BITS	{	//	bits	description
	UINT32	Txe:1;  		//	0		Transmit FIFO Empty 
	UINT32	Txo:1;			//	1		Transmit FIFO Overflow
	UINT32	Rxu:1;			// 	2		Receive FIFO Underflow
	UINT32	Rxo:1;			//	3		Receive FIFO Overflow
	UINT32	Rxf:1;			// 	4		Receive FIFO Full
	UINT32	Mst:1;			// 	5		Transmission Error
	UINT32	Rsvd:26; 		//	31:6 	Reserved
};
union	SSI_INT_REG	{
	UINT32	all;
	struct SSI_INT_BITS	bit;
};

// SSI DMA Control Register Bit Definition
struct	SSI_DMACR_BITS	{	//	bits	description
	UINT32	Rdmae:1;  		//	0		Receive DMA Enable
	UINT32	Tdmae:1;  		//	1		Transmit DMA Enable
	UINT32	Rsvd:30; 		//	31:2 	Reserved
};
union	SSI_DMACR_REG	{
	UINT32	all;
	struct SSI_DMACR_BITS	bit;
};


// SSI regs
struct	SSI_REGS	{
	union SSI_CTRLR0_REG	CTRLR0;		// Control Register 0
	union SSI_CTRLR1_REG	CTRLR1;		// Control Register 1
	UINT32	SSIENR;
	union SSI_MWCR_REG		MWCR;

	UINT32 	SER;
	UINT32	BAUDR;
	UINT32	TXFTLR;
	UINT32	RXFTLR;

	UINT32	TXFLR;	
	UINT32	RXFLR;
	union SSI_SR_REG 	SR;
	union SSI_INT_REG	IMR;

	union SSI_INT_REG	ISR;
	union SSI_INT_REG	RISR;
	UINT32	TXOICR;
	UINT32	RXOICR;

	UINT32	RXUICR;
	UINT32	MSTICR;
	UINT32	ICR;
	union SSI_DMACR_REG		DMACR;

	UINT32 	DMATDLR;
	UINT32	DMARDLR;
	UINT32	IDR;
	UINT32	COMP_VERSION;

	UINT32	DR;
	UINT32	RSVD1[35];

	UINT32	RX_SAMPLE_DLY;
	UINT32	RSVD2[3];
};


/**************************************************************************
* Global Variable Declaration
***************************************************************************/
// Definition of SSI register variable
extern volatile struct SSI_REGS SsiRegs;


/**************************************************************************
* Global Functon Declaration
***************************************************************************/








#endif


