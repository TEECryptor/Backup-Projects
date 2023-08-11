/***************************************************************************************
* File name    :	SY03_USART.h
* Function     :	Header of SY03 UART/SPI/I2C registers
* Author       : 	coins
* Date         :	2012/06/15
* Version      :    v1.0
* Description  :    the UART/SPI/I2C register header for SY03
* ModifyRecord :
*****************************************************************************************/
#ifndef SY03_USART_H_
#define SY03_USART_H_

/***************************************************************************
* Include Header Files
***************************************************************************/

/**************************************************************************
* Global Macro Definition
***************************************************************************/

/**************************************************************************
* Global Type Definition
***************************************************************************/
// USART Current Status Register Bit Definitions
struct	USART_CSTAT_BITS	{//	bits	description
	UINT32	TxEpt:1;		//  0		Transmitter empty
	UINT32	RxAvl:1;		//	1		Receive available byte data message
	UINT32	TxFull:1;		//	2		Transmitter FIFO full status
	UINT32	Rsvd1:9;		//	11:3	Reserved
	UINT32	Si2cDa:1;		//	12		Data/address (I2C slave only)
	UINT32	Si2cRw:1;		// 	13		Read/Write info. (I2C slave only)
	UINT32	Si2cNack:1;		//	14		I2C acknowledge (I2C slave only)
	UINT32	Si2cBusy:1;		//  15		I2C bus busy (I2C slave only)
	UINT32	Rsvd2:16;		//	31:16	Reserved
};
union	USART_CSTAT_REG	{
	UINT32	all;
	struct USART_CSTAT_BITS	bit;
};

// USART Interrupt Status Register Bit Definitions
struct	USART_INTSTAT_BITS	{//	bits	dedcription
	UINT32	TxIntf:1;		//	0		Transmitter FIFO empty interrupt flag
	UINT32	RxIntf:1;		//	1		Receiver data available interrupt flag
	UINT32	UnderRunIntf:1;	//	2		SPI slave transmitter underrun flag (SPI slave only)
	UINT32	RxOerrIntf:1;	//  3		Receiver overrun error interrupt flag
	UINT32	RxPerrIntf:1;	//	4		Parity check error interrupt flag (UART only)
	UINT32	RxFerrIntf:1;	//  5		Framing error interrupt flag (UART only)
	UINT32	RxBrkIntf:1;	//  6		Break condition interrupt flag (UART only)
	UINT32	NackIntf:1;		//  7		No acknowledge interupt flag (I2C master only)
	UINT32	RtsIntf:1;		//	8		Request to send data flag (I2C slave transmitter only)
	UINT32	Rsvd:23;		//  31:9	Reserved
};
union	USART_INTSTAT_REG	{
	UINT32	all;
	struct USART_INTSTAT_BITS	bit;
};

// USART Interrupt Enable Register Bits Definitions
struct	USART_INTEN_BITS	{//	bits	description
	UINT32	TxiEn:1;		//	0		Transmitter FIFO empty interrupt enable
	UINT32	RxiEn:1;		//	1		Receiver data available interrupt enable
	UINT32	UnderRuniEn:1;	//	2		SPI slave transmitter underrun enable (SPI slave only)
	UINT32	RxOerriIEn:1;	//  3		Receiver overrun error interrupt enable
	UINT32	RxPerriEn:1;	//	4		Parity check error interrupt enable (UART only)
	UINT32	RxFerriEn:1;	//  5		Framing error interrupt enable (UART only)
	UINT32	RxBrkiEn:1;		//  6		Break condition interrupt enable (UART only)
	UINT32	NackiEn:1;		//  7		No acknowledge interupt enable (I2C master only)
	UINT32	RtsiEn:1;		//	8		Request to send data enable (I2C slave transmitter only)
	UINT32	Rsvd:23;		//  31:9	Reserved
};
union	USART_INTEN_REG	{
	UINT32	all;
	struct USART_INTEN_BITS	bit;
};

// USART Interrupt Clear Register Bit Definition
struct	USART_INTCLR_BITS	{//	bits	description
	UINT32	TxiClr:1;		//	0		Transmitter FIFO empty interrupt clear
	UINT32	RxiClr:1;		//	1		Receiver data available interrupt clear
	UINT32	UnderRunClr:1;	//	2		SPI slave transmitter underrun clear (SPI slave only)
	UINT32	RxOerrClr:1;	//  3		Receiver overrun error interrupt clear
	UINT32	RxPerrClr:1;	//	4		Parity check error interrupt clear (UART only)
	UINT32	RxFerrClr:1;	//  5		Framing error interrupt clear (UART only)
	UINT32	RxBrkClr:1;		//  6		Break condition interrupt clear (UART only)
	UINT32	NackiClr:1;		//  7		No acknowledge interupt clear (I2C master only)
	UINT32	RtsiClr:1;		//	8		Request to send data clear (I2C slave transmitter only)
	UINT32	Rsvd:23;		//  31:9	Reserved
};
union	USART_INTCLR_REG	{
	UINT32	all;
	struct USART_INTCLR_BITS	bit;
};

// USART Global Control Register Bits Definition
struct	USART_GCTL_BITS	{// bits	description
	UINT32	Usam:2;		//	1:0		USART mode select
	UINT32	Mm:1;		//  2		Master mode (SPI or I2C)
	UINT32	TxEn:1;		//  3		Transmit enable
	UINT32	RxEn:1;		//  4		Receive enable
	UINT32	Rxtlf:2;	//  6:5		Receive FIFO trigger level flag
	UINT32  Txtlf:2;	//  8:7		Transmit FIFO trigger level flag
	UINT32	Rsvd:23;	//	31:9	Reserved
};
union	USART_GCTL_REG	{
	UINT32	all;
	struct USART_GCTL_BITS	bit;
};

// USART Common Control Register Bits Definition
struct	USART_CCTL_BITS	{// bits	description
	UINT32	Ckph:1;		//	0		Clock phase select (SPI only)
	UINT32	Ckpl:1;		//	1		Clock polarity select (SPI only)
	UINT32	Lsbfe:1;	//	2		LSB first enable (SPI only)
	UINT32	SpiLen:1;	//	3		SPI character length (SPI only)
	UINT32	PEn:1;		//	4		Parity enable (UART only)
	UINT32	PSel:1;		//	5		Parity select (UART only)
	UINT32	Spb:1;		//  6		Stop bit select (UART only)
	UINT32	Char:2;		//	8:7		UART character length (UART only)
	UINT32	Brk:1;		//  9		UART break control (UART only)
	UINT32	I2cXa:1;	//	10		I2C extend addressing (I2C only)
	UINT32	I2cRstt:1;	//	11		Repeat start (I2C master only)
	UINT32	I2cStp:1;	//  12		STOP bit (I2C master only)
	UINT32	I2cStt:1;	//  13		Start bit (I2C master only)
	UINT32	NackDt:1;	//	14		Acknowledge data bit (I2C master receive only)
	UINT32	Rsvd:17;	//	31:15	Reserved
};
union	USART_CCTL_REG	{
	UINT32	all;
	struct USART_CCTL_BITS	bit;
};

// USART Registers
struct	USART_REGS	{
	UINT32	TXREG;						// USART transmit data register
	UINT32	RXREG;						// USART receive data register
	union USART_CSTAT_REG	CSTAT;		// USART current status register
	union USART_INTSTAT_REG	INTSTAT;	// USART interrupt status register
	union USART_INTEN_REG	INTEN;		// USART interrupt enable register
	union USART_INTCLR_REG	INTCLR;		// USART interrupt clear register
	union USART_GCTL_REG	GCTL;		// USART global control register
	union USART_CCTL_REG	CCTL;		// USART common control register
	UINT32	SPBREG;						// USART baud rate generator register
	UINT32	SI2CADD;					// Addrss value in I2C slave mode
};

/**************************************************************************
* Global Variable Declaration
***************************************************************************/
// Definition of UART/SPI/I2C register variable
extern volatile struct USART_REGS Uart0Regs;
extern volatile struct USART_REGS I2C0Regs;
extern volatile struct USART_REGS Spi0Regs;
extern volatile struct USART_REGS Spi1Regs;


/**************************************************************************
* Global Functon Declaration
***************************************************************************/


#endif /*SY03_USART_H_*/

