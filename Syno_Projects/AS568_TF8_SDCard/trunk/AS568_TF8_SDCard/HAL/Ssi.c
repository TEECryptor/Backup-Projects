/****************************************************************************
* File name    :	Ssi.c
* Function     :	the driver of SSI
* Author       : 	coins
* Date         :	2012/12/21
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
// define the SSI hardware FIFO depth
#define	SSI_FIFO_DEPTH		8	

/**************************************************************************
* Local static Variable Declaration
***************************************************************************/
static UINT8 *gRxBuf;		// the SSI receive buffer pointer
static UINT16 gRxBufSize;	// the SSI receive buffer size
static volatile UINT16 gRxSize;	// the SSI received byte number
	
// save for store buffer and load RX FIFO (bit 1) mode
// 0 -- load RX FIFO and store one byte
// 1 -- load RX FIFO and store two bytes
static UINT8 gRxLoadMod;


/**************************************************************************
* Global Variable Declaration
***************************************************************************/


/**************************************************************************
* Local Functon Declaration
***************************************************************************/


/**************************************************************************
* Functon
***************************************************************************/

/****************************************************************************
 * Subroutine:	SSI_PortInit
 * Function:	SSI Port initialization  
 * Input:		SSI_MAP mPortMap --	to specify the SSI port map to. 
 * Output:		RT_OK or RT_FAIL	
 * Description:	 
 * Date:		2012/12/21
 * ModifyRecord:
 * *************************************************************************/
UINT8 SSI_PortInit(SSI_MAP mPortMap)
{
	switch(mPortMap)
	{
		case SSI_MAP_B11_14:
			// to select Pins - GPIOB11/12/13/14 for SSI signals
			GpioRegs.GPBUSE1.bit.GPB14 = MUX_SSI;		// SSI_SSN	
			GpioRegs.GPBUSE1.bit.GPB12 = MUX_SSI;		// SSI_CLK	
			GpioRegs.GPBUSE1.bit.GPB11 = MUX_SSI;		// SSI_TXD	
			GpioRegs.GPBUSE1.bit.GPB13 = MUX_SSI;		// SSI_RXD
			break;
		default:
			return RT_FAIL;
	}
			
	// to enable SSI module and reset all logic
	SCM_EnableModClk(ID_SSI);
	SCM_ReleaseModRst(ID_SSI);
	SsiRegs.SSIENR = 0;		// disable SSI
	return RT_OK;	
}

/****************************************************************************
 * Subroutine:	SSI_SpiSlvInit
 * Function:	SSI initialization for SPI Slave mode 
 * Input:		SSI_MAP mPortMap --	to specify the SSI port map to.
 *				UINT8 nCPOL --	to specify the clock polarity, value 0 or 1
 *				UINT8 nCPHA --	to specify the clock phase, value 0 or 1
 *				UINT8 nDFS --  to specify the data frame size, value 4 ~ 16 
 *				UINT8 *pRxBuf -- to specify the buffer for receiving
 *				UINT16 nBufSize -- to specify the buffer size for receiving
 * Output:		RT_OK or RT_FAIL	
 * Description:	 
 * Date:		2012.12.21
 * ModifyRecord:
 * *************************************************************************/
UINT8 SSI_SpiSlvInit(SSI_MAP mPortMap, UINT8 nCPOL, UINT8 nCPHA, UINT8 nDFS, \
						UINT8 *pRxBuf, UINT16 nBufSize)
{	
	if(SSI_PortInit(mPortMap) != RT_OK) return RT_FAIL;
	SsiRegs.CTRLR0.bit.Frf = 0;	// Protocol: Motorola SPI
	SsiRegs.CTRLR0.bit.Scpol = nCPOL;  	// Clock Polarity
	SsiRegs.CTRLR0.bit.Scph = nCPHA; 	// Clock Phase
	SsiRegs.CTRLR0.bit.Dfs = nDFS-1;	// Data Frame Size
	SsiRegs.CTRLR0.bit.SlvOe = 0;
	SsiRegs.IMR.all = 0;	
	SsiRegs.CTRLR0.bit.Tmod = 0x0;	// transmit and receive

	if((pRxBuf != NULL) && (nBufSize != 0))	// Receive interrupt used
	{
		gRxBuf = pRxBuf;
		gRxBufSize = nBufSize;
		gRxSize = 0;

		if(SsiRegs.CTRLR0.bit.Dfs>>3)
		{
			gRxLoadMod = 0x1;	
		}
		else gRxLoadMod = 0;

		SsiRegs.RXFTLR = 0;		
	}

	return RT_OK;
}

/****************************************************************************
 * Subroutine:	SSI_WaitTxIdle
 * Function:	to wait SSI transfer idle 
 * Input:		UINT32 nTimeOut --	to specify the timeout, uint in us.
 * Output:		RT_OK or RT_FAIL	
 * Description:	 
 * Date:		2012.12.21
 * ModifyRecord:
 * *************************************************************************/
UINT8 SSI_WaitTxIdle(UINT32 nTimeOut)
{
	UINT32 nTimeCnt = 0;
	while((!SsiRegs.SR.bit.Tfe) || SsiRegs.SR.bit.Busy)	// wait transmit FIFO empty
	{
		if(nTimeOut != 0)	// timeout check
		{
			_delay_us(1);
			if(nTimeCnt++ == nTimeOut) 
			{
				return RT_OVERTIME;
			}
		}
	}
	return RT_OK;
}


/****************************************************************************
 * Subroutine:	SSI_SpiSlvWrite
 * Function:	SSI SPI Slave mode send data  
 * Input:		UINT8 *pData --	to specify the data to be sent
 *				UINT32 nLen --	to specify the size to be sent
 *				UINT32 nTimeOut -- to specify the timeout for each byte  
 * Output:		RT_OK or RT_OVERTIME	
 * Description:	The Data Frame Size is set by initialization 
 * Date:		2012.12.21
 * ModifyRecord:
 * *************************************************************************/
UINT8 SSI_SpiSlvWrite(UINT8 *pData, UINT16 nLen, UINT32 nTimeOut)
{
	UINT16 *pDat16 = (UINT16 *)pData;
	UINT32 nTimeCnt = 0;
	UINT8 nTxCnt;
	UINT8 nLoadMod;

	//SsiRegs.CTRLR0.bit.Tmod = 0x1;	// transmit only

	// check load buffer (bit 0) and fill TX FIFO (bit 1) mode
	// 2b'00 -- load one byte and fill
	// 2b'01 -- load one half-word and fill
	// 2b'11 -- load two bytes and fill
	if(SsiRegs.CTRLR0.bit.Dfs>>3)
	{
		nLoadMod = 0x1 + (((UINT32)pData)&0x1)<<1;
		if(nLen&0x1) return RT_PARAM_ERR;	
	}
	else nLoadMod = 0;

	SsiRegs.SSIENR = 1;		// enable SSI
	while(nLen)
	{
		nTimeCnt = 0;
		while(!SsiRegs.SR.bit.Tfnf)	// TX FIFO Full, wait and check timeout
		{
			if(nTimeOut)	// timeout check
			{
				_delay_us(1);
				if(nTimeCnt++ == nTimeOut) 
				{
					return RT_OVERTIME;
				}
			}
		}
		nTxCnt = SSI_FIFO_DEPTH - SsiRegs.TXFLR; 	// read TX FIFO valid data entries
		while(nTxCnt-- && nLen)		// Fill TX FIFO
		{
			if(nLoadMod == 0) 	// load one byte and fill	
			{
				SsiRegs.DR = *pData++;
				nLen--;	
			}
			else if (nLoadMod == 1)	// load one half-word and fill		
			{
				SsiRegs.DR = *pDat16++;	
				nLen -= 2;
			}
			else 	// load two bytes and fill
			{
				SsiRegs.DR = *pData | (*(pData+1))<<8;	// LSB stored
				pData += 2;
				nLen -= 2;
			}		
		}					
	}
	return RT_OK;			
}

/****************************************************************************
 * Subroutine:	SSI_StopReceive
 * Function:	to stop SSI receive.  
 * Input:		None  
 * Output:		None	
 * Description:	to disable SSI receive interrupt and clear received byte number  
 * Date:		2012.12.21
 * ModifyRecord:
 * *************************************************************************/
void SSI_StopReceive(void)
{
	NVIC_DisableIRQ(SSI_IRQn);	// disable SSI interrupt
	SsiRegs.IMR.bit.Rxf = 0;
	gRxSize = 0;
	NVIC_EnableIRQ(SSI_IRQn);	// enable SSI interrupt
}

/****************************************************************************
 * Subroutine:	SSI_StartReceive
 * Function:	to start to receive  
 * Input:		UINT8 nDummyByte -- to specify the dummy byte when receiving  
 * Output:		None	
 * Description:	  
 * Date:		2012.12.21
 * ModifyRecord:
 * *************************************************************************/
void SSI_StartReceive(UINT16 nDummyByte)
{
	NVIC_DisableIRQ(SSI_IRQn);	// disable SSI interrupt
	SsiRegs.SSIENR = 0;		// disable SSI
	SsiRegs.IMR.bit.Rxf = 1;
	gRxSize = 0;			// reset buffer index
	SsiRegs.SSIENR = 1;		// enable SSI
	SsiRegs.DR = nDummyByte;
	NVIC_EnableIRQ(SSI_IRQn);	// enable SSI interrupt
}

/****************************************************************************
 * Subroutine:	SSI_GetRxSize
 * Function:	to get received byte number  
 * Input:		None 
 * Output:		to return the number	
 * Description:	  
 * Date:		2012.12.21
 * ModifyRecord:
 * *************************************************************************/
UINT16 SSI_GetRxSize(void)
{
	return gRxSize;
}

/****************************************************************************
 * Subroutine:	SSI_Handler
 * Function:	to handle SSI interrupt 
 * Input:		None 
 * Output:		None	
 * Description:	  
 * Date:		2012.12.21
 * ModifyRecord:
 * *************************************************************************/
void SSI_Handler(void)
{
	UINT8 nRxCnt;
	UINT16 nDat16;
	
	if(SsiRegs.ISR.bit.Rxf)
	{
		// receive processing
		while(SsiRegs.RXFLR && SsiRegs.IMR.bit.Rxf) 	// receive data valid and not soft FIFO not overflow
		{
			nRxCnt = SsiRegs.RXFLR; 	// read RX FIFO valid data entries
			if(gRxLoadMod == 0) 	// load RX FIFO and store one byte	
			{
				if((gRxBufSize - gRxSize)<nRxCnt) 	// no enough buffer for store
				{
					nRxCnt = gRxBufSize - gRxSize;
					if(nRxCnt == 0)
					{ 
						SsiRegs.IMR.bit.Rxf = 0;	// overflow and disable receive interrupt
					}
				}				
				while(nRxCnt--)		// Load RX FIFO and Fill FIFO
				{
					gRxBuf[gRxSize++] = SsiRegs.DR;				
				}	
			}		
			else 	// load RX FIFO and store two bytes
			{
				if((gRxBufSize - gRxSize)<nRxCnt*2) 	// no enough buffer for store
				{
					nRxCnt = (gRxBufSize - gRxSize)/2;
					if(nRxCnt == 0)
					{ 
						SsiRegs.IMR.bit.Rxf = 0;	// overflow and disable receive interrupt
					}
				}
				while(nRxCnt--)		// Load RX FIFO
				{
					nDat16 = SsiRegs.DR;
					gRxBuf[gRxSize++] = nDat16;		// LSB stored
					gRxBuf[gRxSize++] = nDat16>>8;
				}
			}
		}	
	}				
}




