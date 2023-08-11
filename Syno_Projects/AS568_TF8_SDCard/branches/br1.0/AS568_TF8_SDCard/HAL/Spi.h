/***************************************************************************
* File name    :	Spi.h
* Function     :	Header of SPI Module
* Author       : 	coins
* Date         :	2012/10/26
* Version      :    v1.0
* Description  :    
* ModifyRecord :
****************************************************************************/
#ifndef SPI_H_
#define SPI_H_

/***************************************************************************
* Include Header Files
***************************************************************************/


/**************************************************************************
* Global Macro Definition
***************************************************************************/
#define	SPI_SLAVE	0
//--------- Define UART Number -------------
#ifndef	_SPI_
#define	_SPI_
#define 	SPI_NUM	2
#define		SPI0		0
#define		SPI1		1
//#define		SPI2		2
#endif

//---------- Define SPI Format Parameter ----------
#ifndef	_SPI_FORMAT_
#define	_SPI_FORMAT_
#define		MASTER		1
#define		SLAVE		0
#define		MSB			0
#define		LSB			1

#define		MODE0		0
#define		MODE1		1
#define		MODE2		2
#define		MODE3		3


#endif

#ifndef	DUMMY
#define		DUMMY		0x00
#endif



// define the function name for old firmware library revision
#define		GetSpiRegs			SPI_GetRegs
#define		SpiInit				SPI_Init	


/**************************************************************************
* Global Type Definition
***************************************************************************/
typedef	enum {
	SPI0_MAP_A7_A9 = 0,	
			//GPA7 -- SPI0_CLK	
			//GPA8 -- SPI0_MISO	
			//GPA9 -- SPI0_MOSI	
	SPI1_MAP_B3_B5 = 1,	
			//GPB3 -- SPI1_CLK	
			//GPB4 -- SPI1_MISO	
			//GPB5 -- SPI1_MOSI	
	SPI1_MAP_B19_B21 = 2
			//GPB19 -- SPI1_CLK	
			//GPB20 -- SPI1_CLK	
			//GPB21 -- SPI1_CLK	
}SPI_MAP;


//------ Define Data Buffer Structure Type --------
#ifndef	_DATA_BUF_
#define _DATA_BUF_
typedef	struct	{
	UINT8 *	pBuf;		// pointer to buffer start address
	UINT16	Size;		// buffer size
}STRUCT_DATA_BUF;
#endif

#ifndef	_STRUCT_SPI_FRAME_FORMAT_
#define	_STRUCT_SPI_FRAME_FORMAT_
typedef	struct {
	UINT8	nMasterEn;		// SPI master mode or slave mode
	UINT8 	nBits;			// SPI character length bits
	UINT8 	nMode;			// SPI data mode, Mode 0/1/2/3
	UINT8 	nFrame;			// SPI frame format support, MSB or LSB
	UINT32 	nSckFreq;		// SPI speed
	UINT8 	n4Wires;		// SPI 4 wires
}STRUCT_SPI_FORMAT;
#endif

//------ Define FIFO Structure Type --------
#ifndef	_STRUCT_FIFO_
#define	_STRUCT_FIFO_
typedef	struct {
	UINT8 *	pFFData;		// data buffer pointer
	UINT16	FFDepth;		// FIFO depth
	UINT16 	FFValidSize;	// valid data size in FIFO
	UINT16 	FFInOffset;		// index of FIFO in
	UINT16 	FFOutOffset;	// index of FIFO out
}STRUCT_FIFO;
#endif

/**************************************************************************
* Global Variable Declaration
***************************************************************************/


/**************************************************************************
* Global Functon Declaration
***************************************************************************/
volatile struct USART_REGS * SPI_GetRegs(UINT8 nSpiNum);
UINT8 SPI_Init(UINT8 nSpiNum, SPI_MAP mPortMap, STRUCT_SPI_FORMAT sFrame, STRUCT_DATA_BUF *pRxBuf, STRUCT_DATA_BUF *pTxBuf);
UINT8 SPI_MasterTransceive(UINT8 nSpiNum, UINT8 *pData, UINT32 nLen);
UINT8 SPI_MasterWrite(UINT8 nSpiNum, UINT8 *pData, UINT32 nLen);
UINT8 SPI_MasterRead(UINT8 nSpiNum, UINT8 *pData, UINT32 nLen);
#if(SPI_SALVE)
UINT8 SPI_SlaveWrite(UINT8 bSpiNum, UINT8 *pBuf, UINT32 nLen);
UINT8 SPI_SlaveRead(UINT8 bSpiNum, UINT8 *pBuf, UINT32 nLen);
#endif

// Just for old firmware library revision
__INLINE UINT8 SpiTransceive(UINT8 bSpiNum, UINT8 *pData)
{
	return SPI_MasterTransceive(bSpiNum, pData, 1);	
}

__INLINE UINT8 SpiWriteByte(UINT8 bSpiNum, UINT8 bData)
{
	return SPI_MasterWrite(bSpiNum, &bData, 1);	
}

__INLINE UINT8 SpiReadByte(UINT8 bSpiNum, UINT8 *pData)
{
	return SPI_MasterRead(bSpiNum, pData, 1);
}


#endif
