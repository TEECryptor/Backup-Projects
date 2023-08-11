/****************************************************************************
* File name    :	Spi.c
* Function     :	the driver of SPI Module
* Author       : 	coins
* Date         :	2012/10/26
* Version      :    v1.0
* Description  :
* ModifyRecord :
****************************************************************************/
/***************************************************************************
* Include Header Files
***************************************************************************/
#include "SY03.h"


/**************************************************************************
* Local Macro Definition
***************************************************************************/


/**************************************************************************
* Local Type Definition
***************************************************************************/


/**************************************************************************
* Local static Variable Declaration
***************************************************************************/
#if(SPI_SLAVE)
static STRUCT_FIFO gSpiTxFIFO[SPI_NUM];	// the global for SPI transmitting
static STRUCT_FIFO gSpiRxFIFO[SPI_NUM];	// the global for SPI receiving
#endif

/**************************************************************************
* Global Variable Declaration
***************************************************************************/


/**************************************************************************
* Local Functon Declaration
***************************************************************************/
#if(SPI_SLAVE)
static void SpiInitTxFIFO(UINT8 bSpiNum, STRUCT_DATA_BUF *pTxBuf);
static void SpiInitRxFIFO(UINT8 bSpiNum, STRUCT_DATA_BUF *pRxBuf);
static UINT16 SpiGetTxFIFOValidSize(UINT8 bSpiNum);
static void SpiSetTxFIFOValidSize(UINT8 bSpiNum, UINT16 offset_size);
static UINT16 SpiGetRxFIFOValidSize(UINT8 bSpiNum);
static void SpiSetRxFIFOValidSize(UINT8 bSpiNum, UINT16 offset_size);
#endif

/**************************************************************************
* Functon
***************************************************************************/

/****************************************************************************
 * Subroutine:	SPI_PortInit
 * Function:	SPI Port initialization  
 * Input:       SPI_MAP mPortMap-- Select which group of GPIO port as SPI
 *                                 SPI0_MAP_A7_A9--GPIA[7]&GPIOB[8]&GPIOB[9]
 *                                 SPI1_MAP_B19_B21--GPIOB[19]&GPIOA[20]&GPIOA[21]
 *                                 SPI1_MAP_B3_B5--GPIOB[3]&GPIOB[4]&GPIOB[5]
 * Output:		RT_OK or RT_FAIL	
 * Description:	 
 * Date:		2013/08/28
 * ModifyRecord:
***************************************************************************/
UINT8 SPI_PortInit(SPI_MAP mPortMap)
{
    switch(mPortMap)
	{
		case SPI0_MAP_A7_A9:
			// select Pins
			GpioRegs.GPAUSE0.bit.GPA7 = MUX_SPI;	// GPA7 - Spi0_CLK
			GpioRegs.GPAUSE1.bit.GPA8 = MUX_SPI;	// GPA8 - Spi0_MISO
			GpioRegs.GPAUSE1.bit.GPA9 = MUX_SPI;	// GPA8 - Spi0_MOSI	
			break;
		case SPI1_MAP_B19_B21:
			// select Pins
			GpioRegs.GPBUSE2.bit.GPB19 =  MUX_SPI;	  // GPB19 - Spi1_CLK
			GpioRegs.GPBUSE2.bit.GPB20 =  MUX_SPI;	  // GPB20 - Spi1_MISO
			GpioRegs.GPBUSE2.bit.GPB21 =  MUX_SPI;	  // GPB21 - Spi1_MOSI
			break;
		case SPI1_MAP_B3_B5: 
			GpioRegs.GPBUSE0.bit.GPB4 =  MUX_SPI;	  // GPB4 - Spi1_CLK
			GpioRegs.GPBUSE0.bit.GPB3 =  MUX_SPI;	  // GPB3 - Spi1_MISO
			GpioRegs.GPBUSE0.bit.GPB5 =  MUX_SPI;	  // GPB5 - Spi1_MOSI
			break;
		default:
			return RT_FAIL;
	}
	return RT_OK;	
}

/***************************************************************************
 * Subroutine:	SPI_GetRegs
 * Function:	Get SPI register file address
 * Input:		UINT8 nSpiNum -- 	SPI number, should be less than SPI_NUM;
 * 								if it is out of range, dead loops.
 * Output:		volatile struct SPI_REGS * --	Return SPIx register file
 * 												address
 * Description:	Get SPI register file address.
 * Date:		2012.10.26
 * ModifyRecord:
 * *************************************************************************/
volatile struct USART_REGS * SPI_GetRegs(UINT8 nSpiNum)
{
	switch(nSpiNum)
	{
		case SPI0:
			return &Spi0Regs;
		case SPI1:
			return &Spi1Regs;
		//case SPI2:
		//	return &Spi2Regs;
		default:
			return NULL;
	}
}

/***************************************************************************
 * Subroutine:	SPI_Init
 * Function:	SPI initialization
 * Input:		UINT8 nSpiNum -- 	SPI number, Spi0 or Spi1;
 *				SPI_MAP mPortMap--  Select which group of GPIO port as SPI
 *                                  SPI0_MAP_A7_A9--GPIA[7]&GPIOB[8]&GPIOB[9]
 *                                  SPI1_MAP_B19_B21--GPIOB[19]&GPIOA[20]&GPIOA[21]
 *                                  SPI1_MAP_B3_B5--GPIOB[3]&GPIOB[4]&GPIOB[5]
 * 				STRUCT_SPI_FORMAT sFrame --
 * 									SPI frame format;
 * 				STRUCT_DATA_BUF sRxBuf --
 * 									SPI receive buffer, to specify the buffer and size for slave receive;
 * 				STRUCT_DATA_BUF sTxBuf --
 * 									SPI transmit buffer, to specify the buffer and size for slave transmit;
 * Output:		UINT8	--			Return result, RT_OK or RT_FAIL	or RT_PARAM_ERR
 * Description:
 * Date:		2012.10.26
 * ModifyRecord:
 * *************************************************************************/
UINT8 SPI_Init(UINT8 nSpiNum, SPI_MAP mPortMap,STRUCT_SPI_FORMAT sFrame, STRUCT_DATA_BUF *pRxBuf, STRUCT_DATA_BUF *pTxBuf)
{
	volatile struct USART_REGS * pSpi;
	switch(nSpiNum)
	{
		case SPI0 :
			if((mPortMap == SPI1_MAP_B3_B5)||(mPortMap == SPI1_MAP_B19_B21))
				return RT_PARAM_ERR;
			// 1. to open global control
			SCM_DisableModClk(ID_SPI0);// disable SPI0 clock
			SCM_KeepModRst(ID_SPI0);// reset
			SCM_EnableModClk(ID_SPI0);// enable SPI0 clock
			SCM_ReleaseModRst(ID_SPI0);// Relinquish from reset
			break;	
		case SPI1 :
			if(mPortMap == SPI0_MAP_A7_A9)
				return RT_PARAM_ERR;
			// 1. to open global control
			SCM_DisableModClk(ID_SPI1);// disable SPI0 clock
			SCM_KeepModRst(ID_SPI1);// reset
			SCM_EnableModClk(ID_SPI1);// enable SPI0 clock
			SCM_ReleaseModRst(ID_SPI1);// Relinquish from reset
			break;
		default:
			return RT_PARAM_ERR;
	}
	//2 to select Pins
	if(SPI_PortInit(mPortMap) != RT_OK) return RT_FAIL;
	// 3. select SPI register file
	pSpi = SPI_GetRegs(nSpiNum);
	// 4. disable SPI transfer and receive and set USART to SPI mode
	pSpi->GCTL.all = 0x0002;
	// 5. disable all SPI interrupts and clear all interrupt flags
	pSpi->INTEN.all = 0x0000;
	pSpi->INTCLR.all = 0xffff;

	// 6. config character length bit
	if((sFrame.nBits-7)<2)
		pSpi->CCTL.bit.SpiLen = sFrame.nBits - 7;
	else
		return RT_FAIL;
	// 7. config data mode
	switch(sFrame.nMode)
	{
		case MODE0:
			pSpi->CCTL.all &= ~(1<<1);	// ckpl
			pSpi->CCTL.all |= (1<<0);	// ckph
			break;
		case MODE1:
			pSpi->CCTL.all &= ~(1<<1);
			pSpi->CCTL.all &= ~(1<<0);
			break;
		case MODE2:
			pSpi->CCTL.all |= (1<<1);
			pSpi->CCTL.all |= (1<<0);
			break;
		case MODE3:
			pSpi->CCTL.all |= (1<<1);
			pSpi->CCTL.all &= ~(1<<0);
			break;
		default:
			return RT_FAIL;
	}
	// 8. config frame format : Lsb or Msb
	pSpi->CCTL.bit.Lsbfe = sFrame.nFrame;

	// 9. Master or Slave Configure
	switch(sFrame.nMasterEn)
	{
		case MASTER:
			// 10. config SPI to master or slave
			pSpi->GCTL.bit.Mm = sFrame.nMasterEn;
			// 11. Config SPI speed in Spi master
			if(sFrame.nSckFreq == 0)
				return RT_FAIL;
			pSpi->SPBREG = SCM_GetSysClk() / sFrame.nSckFreq;
			if(pSpi->SPBREG < 2) pSpi->SPBREG = 2;	// must be >=2
			// 12. enable SPI transmit and receive
			pSpi->GCTL.bit.TxEn = 1;		// enable transmit
			pSpi->GCTL.bit.RxEn = 1;		// enable receive
			break;
		#if(SPI_SLAVE)
		case SLAVE:
			// 10. Config SPI Interrupt & FIFO in Spi Slave
			pSpi->GCTL.bit.Mm = sFrame.nMasterEn;

			SpiInitTxFIFO(nSpiNum, pTxBuf);
			SpiInitRxFIFO(nSpiNum, pRxBuf);

			// 11. to initialize RX FIFO, enable RX interrupt and RX
			if(pRxBuf != NULL)		// no buffer, disable receive
			{
				pSpi->INTEN.bit.RxiEn = 1;				// added by coins	2009.09.17
				pSpi->INTEN.bit.RxOerriIEn = 1;
				pSpi->INTEN.bit.UnderRuniEn = 1;
			}

			// 12. to initial TX FIFO, enable TX interrupt and TX
			if(pTxBuf != NULL)				// no buffer for transmit interrupt
			{
				pSpi->INTEN.bit.TxiEn = 1;
				pSpi->GCTL.bit.TxEn = 1;		// enable transmit
			}

			pSpi->GCTL.bit.TxEn = 1;				// enable transmit
			pSpi->GCTL.bit.RxEn = 1;				// enable receive
			break;
		#endif
		default:
			return RT_FAIL;
	}
	return RT_OK;
}

/***************************************************************************
 * Subroutine:	SPI_MasterTransceive
 * Function:	SPI Master transmit byte and receive byte at the same time
 * Input:		UINT8 nSpiNum -- 	SPI number, SPI 0/1/2
 * 				UINT8 *pData --		the point of transmit data and
 * 									received data;
 *				UINT32 nLen -- 		to transceive nLen bytes;
 * Output:		UINT8	--			Return the result, RT_OK or RT_FAIL
 * Description:
 * Date:		2012.10.26
 * ModifyRecord:
 * *************************************************************************/
UINT8 SPI_MasterTransceive(UINT8 nSpiNum, UINT8 *pData, UINT32 nLen)
{
	volatile struct USART_REGS * pSpi;
	UINT32 i = 0;
	UINT32 lWaitTime;
	if(nSpiNum >= SPI_NUM)
		return RT_FAIL;
	pSpi = SPI_GetRegs(nSpiNum);
	lWaitTime = 8 * pSpi->SPBREG;	// 8 SPI clock transmit a byte
	while(nLen--)
	{
		i = 0;
		while(pSpi->CSTAT.bit.TxFull)	// wait for tx_fifo not full
		{
			if(++i == lWaitTime) return RT_FAIL;
		}
		pSpi->TXREG = *pData;

		i = 0;
		while(!pSpi->CSTAT.bit.RxAvl)	// receive available
		{
			if(++i == lWaitTime) return RT_FAIL;
		}
		*pData = pSpi->RXREG;
		pData++;	// next byte
	}
	return RT_OK;
}

/***************************************************************************
 * Subroutine:	SPI_MasterWrite
 * Function:	SPI master write bytes
 * Input:		UINT8 nSpiNum -- 	SPI number, SPI 0/1/2
 * 				UINT8 *pData --		The data to be written
 *				UINT32 nLen -- 		the length of data to be written
 * Output:		UINT8	--			Return the result, RT_OK or RT_FAIL
 * Description:
 * Date:		2012.10.26
 * ModifyRecord:
 * *************************************************************************/
UINT8 SPI_MasterWrite(UINT8 nSpiNum, UINT8 *pData, UINT32 nLen)
{
	volatile struct USART_REGS * pSpi;
	UINT32 i = 0;
	UINT32 lWaitTime;
	if(nSpiNum >= SPI_NUM)
		return RT_FAIL;
	pSpi = SPI_GetRegs(nSpiNum);
	lWaitTime = 8 * pSpi->SPBREG;	// 8 SPI clock transmit a byte
	while(nLen--)
	{
		i = 0;
		while(pSpi->CSTAT.bit.TxFull)	// wait for tx_fifo not full
		{
			if(++i == lWaitTime) return RT_FAIL;
		}
		pSpi->TXREG = *pData++;

		i = 0;
		while(!pSpi->CSTAT.bit.RxAvl)	// receive available
		{
			if(++i == lWaitTime) return RT_FAIL;
		}
		pSpi->RXREG;
	}
	return RT_OK;
}

/***************************************************************************
 * Subroutine:	SPI_MaterRead
 * Function:	SPI master receive bytes (not interrupt)
 * Input:		UINT8 bSpiNum -- 	SPI number, SPI 0/1/2
 * 				UINT8 *pData --		The value to be received (pointer)
 *				UINT32 nLen -- 		to specify the length to be read
 * Output:		UINT8	--			Return the result, RT_OK or RT_FAIL
 * Description:
 * Date:		2012.10.26
 * ModifyRecord:
 * *************************************************************************/
UINT8 SPI_MasterRead(UINT8 nSpiNum, UINT8 *pData, UINT32 nLen)
{
	volatile struct USART_REGS * pSpi;
	UINT32 i = 0;
	UINT32 lWaitTime;
	if(nSpiNum >= SPI_NUM)
		return RT_FAIL;
	pSpi = SPI_GetRegs(nSpiNum);
	lWaitTime = 8 * pSpi->SPBREG;	// 8 SPI clock transmit a byte
	while(nLen--)
	{	
		pSpi->TXREG = DUMMY;
		i = 0;
		while(!pSpi->CSTAT.bit.RxAvl)	// receive available
		{
			if(++i == lWaitTime) return RT_FAIL;
		}
		*pData++ = pSpi->RXREG;
	}
	return RT_OK;
}

#if(SPI_SLAVE)
/**************************************************************************
 * Subroutine:	SPI_SlaveWrite
 * Function:	SPI Slave send bytes
 * Input:		UINT8 bSpiNum -- 	SPI number, SPI 0/1/2
 * 				UINT8 *pBuf --		The buffer to be sent
 * 				UINT32 nLen --		The length of buffer
 * Output:		UINT8	--			Return the result, RT_OK or RT_FAIL
 * Description:
 * Date:		2012.10.26
 * ModifyRecord:
 * **********************************************************************/
UINT8 SPI_SlaveWrite(UINT8 bSpiNum, UINT8 *pBuf, UINT32 nLen)
{
	volatile struct USART_REGS * pSpi;
	UINT16 spi_tx_buf;
	UINT32 status;
	if(bSpiNum >= SPI_NUM) return RT_FAIL;
	
	pSpi = SPI_GetRegs(bSpiNum);
	spi_tx_buf = SpiGetTxFIFOValidSize(bSpiNum);	// to get TX buffer valid size
	if(spi_tx_buf==0)				// TX buffer is empty /
	{
		status = NVIC->ISER[0]; 	// Save interrupt status
		NVIC_DisableIRQ(SPI0_IRQn); 	// disable SPI0 interrupt
		while(!pSpi->CSTAT.bit.TxFull)
		{
			if(nLen--)
				pSpi->TXREG = *pBuf++;
			else
			{
				NVIC->ISER[0] = status;		// recover interrupt status
				return RT_OK;							// iLen == 0, exit
			}
		}
		NVIC->ISER[0] = status;		// recover interrupt status
	}

	while(nLen--)
	{
		if(SpiGetTxFIFOValidSize(bSpiNum) < gSpiTxFIFO[bSpiNum].FFDepth)// buffer is not full
		{
			gSpiTxFIFO[bSpiNum].pFFData[gSpiTxFIFO[bSpiNum].FFInOffset++] = *pBuf++;
			if(gSpiTxFIFO[bSpiNum].FFInOffset >= gSpiTxFIFO[bSpiNum].FFDepth)
				gSpiTxFIFO[bSpiNum].FFInOffset = 0;
			SpiSetTxFIFOValidSize(bSpiNum, 1);	// valid size + 1 with interrupt disabled
		}
		else
		{
			// wait
			UINT32 times = pSpi->SPBREG * 256;
			while((SpiGetTxFIFOValidSize(bSpiNum) >= gSpiTxFIFO[bSpiNum].FFDepth) && (times--));
			if(times == 0) return RT_FAIL;
		}
	}
	return RT_OK;
}

/**************************************************************************
 * Subroutine:	SPI_SlaveRead
 * Function:	SPI slave receive bytes
 * Input:		UINT8 bSpiNum -- 	SPI number, SPI 0/1/2
 * 				UINT8 *pBuf --		The buffer used for storing received data
 * 				UINT32 nLen --		The length will be receive
 * Output:		UINT8	--			Return the result, RT_OK or RT_FAIL(no enough data)
 * Description:
 * Date:		2009.09.28
 * ModifyRecord:
 * ************************************************************************/
UINT8 SPI_SlaveRead(UINT8 bSpiNum, UINT8 *pBuf, UINT32 nLen)
{
	if(SpiGetRxFIFOValidSize(bSpiNum) >= nLen)		// have enough data in receive FIFO
	{
		while(nLen--)
		{
			*pBuf++ = gSpiRxFIFO[bSpiNum].pFFData[gSpiRxFIFO[bSpiNum].FFOutOffset++];
			if(gSpiRxFIFO[bSpiNum].FFOutOffset == gSpiRxFIFO[bSpiNum].FFDepth)
				gSpiRxFIFO[bSpiNum].FFOutOffset = 0;
			SpiSetRxFIFOValidSize(bSpiNum, 1);		// valid size - 1 with interrupt disabled
		}
		return RT_OK;
	}
	return RT_FAIL;
}

/***************************************************************************
 * Subroutine:	SpiInitTxFIFO
 * Function:	Initialize Spi transmit buffer
 * Input:		UINT8 bSpiNum -- 	SPI number, SPI 0/1/2
 * Output:		none
 * Description: clear memory of gSpiTxFIFO[bSpiNum], include FFValidSize, FFInOffset,
 * 				FFOutOffset and FIFO
 * Date:		2009.09.28
 * ModifyRecord:
 * ************************************************************************/
static void SpiInitTxFIFO(UINT8 bSpiNum, STRUCT_DATA_BUF *pTxBuf)
{
	gSpiTxFIFO[bSpiNum].pFFData = pTxBuf->pBuf;		// to initialize transmit FIFO
	gSpiTxFIFO[bSpiNum].FFDepth = pTxBuf->Size;
	gSpiTxFIFO[bSpiNum].FFValidSize = 0;
	gSpiTxFIFO[bSpiNum].FFInOffset = 0;
	gSpiTxFIFO[bSpiNum].FFOutOffset = 0;
}
/***************************************************************************
 * Subroutine:	SpiInitRxFIFO
 * Function:	Initialize Spi transmit buffer
 * Input:		UINT8 bSpiNum -- 	SPI number, SPI 0/1/2
 * Output:		none
 * Description: clear memory of gSpiRxFIFO[bSpiNum], include FFValidSize, FFInOffset,
 * 				FFOutOffset and FIFO
 * Date:		2009.09.28
 * ModifyRecord:
 * ************************************************************************/
static void SpiInitRxFIFO(UINT8 bSpiNum, STRUCT_DATA_BUF *pRxBuf)
{
	gSpiRxFIFO[bSpiNum].pFFData = pRxBuf->pBuf;		// to initialize transmit FIFO
	gSpiRxFIFO[bSpiNum].FFDepth = pRxBuf->Size;
	gSpiRxFIFO[bSpiNum].FFValidSize = 0;
	gSpiRxFIFO[bSpiNum].FFInOffset = 0;
	gSpiRxFIFO[bSpiNum].FFOutOffset = 0;
}
/***************************************************************************
 * Subroutine:	SpiGetTxFIFOValidSize
 * Function:	to get Spi transmit buffer valid data number
 * Input:		UINT8 bSpiNum -- 	SPI number, SPI 0/1/2
 * Output:		UINT16	-- the buffer valid data size;
 * Description:
 * Date:		2009.09.28
 * ModifyRecord:
 * ************************************************************************/
static UINT16 SpiGetTxFIFOValidSize(UINT8 bSpiNum)
{
	UINT32 size;
	UINT32 status;
	status = NVIC->ISER[0]; 	// Save interrupt status
	NVIC_DisableIRQ(SPI0_IRQn); 	// disable SPI0 interrupt
	size = gSpiTxFIFO[bSpiNum].FFValidSize;
	NVIC->ISER[0] = status;		// recover interrupt status
	return size;
}

/***************************************************************************
 * Subroutine: SpiSetTxFIFOValidSize
 * Function:	to set spi transmit buffer valid data number (increase current value)
 * Input:		UINT8 bSpiNum -- 	SPI number, Spi 0/1/2
 * 				UINT16 offset_size --	specifies the size offset;
 * Output:		none
 * Description:
 * Date:		2009.09.28
 * ModifyRecord:
 * ************************************************************************/
static void SpiSetTxFIFOValidSize(UINT8 bSpiNum, UINT16 offset_size)
{
	UINT32 status;
	status = NVIC->ISER[0]; 	// Save interrupt status
	NVIC_DisableIRQ(SPI0_IRQn); 	// disable SPI0 interrupt
	gSpiTxFIFO[bSpiNum].FFValidSize += offset_size;
	NVIC->ISER[0] = status;		// recover interrupt status
}

/***************************************************************************
 * Subroutine:	SpiGetRxFIFOValidSize
 * Function:	to get Spi receive buffer valid data number
 * Input:		UINT8 bSpiNum -- 	SPI number, SPI 0/1/2
 * Output:		UINT16	-- the buffer valid data size;
 * Description:
 * Date:		2009.09.28
 * ModifyRecord:
 * ************************************************************************/
static UINT16 SpiGetRxFIFOValidSize(UINT8 bSpiNum)
{
	UINT32 status;
	UINT32 size;
	status = NVIC->ISER[0]; 	// Save interrupt status
	NVIC_DisableIRQ(SPI0_IRQn); 	// disable SPI0 interrupt
	size = gSpiRxFIFO[bSpiNum].FFValidSize;
	NVIC->ISER[0] = status;		// recover interrupt status
	return size;
}

/***************************************************************************
 * Subroutine: SpiSetRxFIFOValidSize
 * Function:	to set spi receive buffer valid data number (increase current value)
 * Input:		UINT8 bSpiNum -- 	SPI number, Spi 0/1/2
 * 				UINT16 offset_size --	specifies the size offset;
 * Output:		none
 * Description:
 * Date:		2009.09.28
 * ModifyRecord:
 * ************************************************************************/
static void SpiSetRxFIFOValidSize(UINT8 bSpiNum, UINT16 offset_size)
{
	UINT32 status;
	status = NVIC->ISER[0]; 	// Save interrupt status
	NVIC_DisableIRQ(SPI0_IRQn); 	// disable SPI0 interrupt
	gSpiRxFIFO[bSpiNum].FFValidSize -= offset_size;
	NVIC->ISER[0] = status;		// recover interrupt status
}

/***************************************************************************
 * Subroutine:	SpiIntProc
 * Function:	SPI interrupt process
 * Input:		UINT8 bSpiNum -- SPI number, SPI 0/1/2
 * Output:		none
 * Description: to process all SPI interrupt
 * Date:		2010.05.27
 * ModifyRecord:
 * ************************************************************************/
void SpiIntProc(UINT8 bSpiNum)
{
	union USART_INTSTAT_REG IntFlag;
	volatile struct USART_REGS * pSpi;
	pSpi = SPI_GetRegs(bSpiNum);

	IntFlag = pSpi->INTSTAT;		// get interrupt flags
	pSpi->INTCLR.all = IntFlag.all;	// clear interrupt flags for next interrupt

	if(IntFlag.bit.RxIntf || IntFlag.bit.RxOerrIntf)	// receive interrupt
	{
		while(pSpi->CSTAT.bit.RxAvl)	// added by coins, 2009.09.16
		{
			if(gSpiRxFIFO[bSpiNum].FFValidSize < gSpiRxFIFO[bSpiNum].FFDepth)	// not full
			{
				gSpiRxFIFO[bSpiNum].pFFData[gSpiRxFIFO[bSpiNum].FFInOffset++] = pSpi->RXREG;
				if(gSpiRxFIFO[bSpiNum].FFInOffset == gSpiRxFIFO[bSpiNum].FFDepth)
					gSpiRxFIFO[bSpiNum].FFInOffset = 0;
				gSpiRxFIFO[bSpiNum].FFValidSize++;
			}
			else	// if USART RX FIFO is full, dummy read.	added by coins, 2009.09.17
			{
				pSpi->RXREG;
			}
		}
	}
	if(IntFlag.bit.TxIntf)		// transmit interrupt
	{
		while(!pSpi->CSTAT.bit.TxFull)	// till FIFO is full
		{
			if(gSpiTxFIFO[bSpiNum].FFValidSize != 0)	// not empty
			{
				pSpi->TXREG = gSpiTxFIFO[bSpiNum].pFFData[gSpiTxFIFO[bSpiNum].FFOutOffset++];
				if(gSpiTxFIFO[bSpiNum].FFOutOffset == gSpiTxFIFO[bSpiNum].FFDepth)
					gSpiTxFIFO[bSpiNum].FFOutOffset = 0;
				gSpiTxFIFO[bSpiNum].FFValidSize--;
			}
			else break;
		}
	}
}

/***************************************************************************
 * Subroutine:	SPI0_Handler
 * Function:	SPI0 Interrupt Service Routine
 * Input:		None
 * Output:		none
 * Description: SPI0 interrupt processing
 * Date:		2012.10.26
 * ModifyRecord:
 * ************************************************************************/
void SPI0_Handler(void)
{
	SpiIntProc(SPI0);
}

#endif

