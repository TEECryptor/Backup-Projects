/****************************************************************************
* File name    :	Swpm.c
* Function     :	the driver of SWP Master controller
* Author       : 	coins
* Date         :	2012/12/01
* Version      :    v1.0
* Description  :  	this file implements SWP Master MAC layer driver
* ModifyRecord :
****************************************************************************/
/***************************************************************************
* Include Header Files
***************************************************************************/
#include "SY03.h"

/**************************************************************************
* Local Macro Definition
***************************************************************************/
//#define	SWPM_RX_INT

/**************************************************************************
* Local Type Definition
***************************************************************************/
#ifdef SWPM_RX_INT
typedef	struct	{
	UINT8 nData[50];
	UINT8 nLen;
	SWPM_ERR nStat;
}SWPM_RX_FRAME;
#endif

/**************************************************************************
* Local static Variable Declaration
***************************************************************************/
#ifdef SWPM_RX_INT
static SWPM_RX_FRAME RxFrame = {0};
static UINT8 gMutex = 0; 	// the mutex semaphore for interrupt receive and application process
#endif


/**************************************************************************
* Global Variable Declaration
***************************************************************************/
 UINT8	buffr[10]={0},len;

/**************************************************************************
* Local Functon Declaration
***************************************************************************/


/**************************************************************************
* Functon
***************************************************************************/

/****************************************************************************
 * Subroutine:	SWPM_Init
 * Function:	to initialization SWPM 
 * Input:		None
 * Output:		SWPM_ERR	
 * Description:	 
 * Date:		2012.12.01
 * ModifyRecord:
 * *************************************************************************/
SWPM_ERR SWPM_Init(void)
{
	UINT32 nSysClk = SCM_GetSysClk()/1000000;
	//GpioRegs.GPAUSE1.bit.GPA11 = MUX_SWPM;	
	
	SCM_KeepModRst(ID_SWPM);
	SCM_EnableModClk(ID_SWPM);
	SCM_ReleaseModRst(ID_SWPM);

	// Config S1 period
	SwpmRegs.S1_PERIOD = 15;
	SwpmRegs.RESUME_TM = nSysClk * 3;	// S2_RESUME状态下，当S2高电平持续时间等于该值，将S1拉低

	// Suspend bit number
	SwpmRegs.CTRL.bit.SuspendIdleBit = 7;	
	#ifdef SWPM_RX_INT
		SwpmRegs.IE.bit.Resume = 1;
		SwpmRegs.IE.bit.RxSof = 1;
		SwpmRegs.IE.bit.RxFifoNotEmpty = 1;
		SwpmRegs.IE.bit.RxDone = 1;
		NVIC_EnableIRQ(SWPS_IRQn);	// enable SWPS interrupt
	#endif	
	return SWPM_OK;
}

/****************************************************************************
 * Subroutine:	SWPM_SendFrame
 * Function:	to send a frame with SWP Master 
 * Input:		UINT8 *pData --	to specify the data for sending. 
 *				UINT8 nLen -- to specify the length of data
 * Output:		SWPM_ERR	
 * Description:	The data for sending is only the payload, excluding the CRC bytes,
 *				SOF and EOF. The length for data in limited to 30 
 * Date:		2012.12.01
 * ModifyRecord:
 * *************************************************************************/
SWPM_ERR SWPM_SendFrame(UINT8 *pData, UINT8 nLen)
{
	if(nLen>30) nLen = 30;	// the payload size is limit to 30 bytes
	if(nLen==0) return SWPM_PARAM_ERR;
	SwpmRegs.LEN.bit.TxLen = nLen;
	
	while((SwpmRegs.INT.bit.TxFifoNotFull) && nLen)
	{
		SwpmRegs.TX_FIFO = *pData++;  	// write transmit FIFO
		nLen -- ;
	}
	SwpmRegs.INT.all = 1;			// clear done flag
	SwpmRegs.CTRL.bit.TxEn = 1; 	// enable transfer
	while(SwpmRegs.INT.bit.TxDone == 0)	// when TxDone is set, the TxEn cleared automatically
	{
		while((SwpmRegs.INT.bit.TxFifoNotFull) && nLen) 	// if TX fifo is not full
		{
			SwpmRegs.TX_FIFO = *pData++;  	// write fifo
			nLen -- ;	
		}
	}

	SwpmRegs.INT.all = 1;	// clear done flag
	
	if(SwpmRegs.STATUS.bit.TxFifoUnderFlow)	// under run
	{
		SwpmRegs.CTRL.bit.TxFifoClr = 1;	// clear transmit FIFO
		return SWPM_TX_UNDERFLOW;
	}
	else
	{
		return SWPM_OK;
	}	
}


/****************************************************************************
 * Subroutine:	SWPM_GetFrame
 * Function:	to get a frame from UICC 
 * Input:		UINT8 *pData --	to specify the a frame data buffer for read. 
 *				UINT8 *pLen -- to return the length of the frame
 *				UINT32 nLoops --  to specify the loops for waiting the frame
 * Output:		SWPM_ERR	
 * Description:	The frame data is only the payload, excluding the CRC bytes,
 *				SOF and EOF. The length for data in limited to 30 
 * Date:		2012.12.01
 * ModifyRecord:
 * *************************************************************************/
SWPM_ERR SWPM_GetFrame(UINT8 *pData, UINT8 *pLen, UINT32 nLoops)
{
	UINT32 i = nLoops;
	UINT8 RxDoneFlag = 0;
	#ifdef	SWPM_RX_INT
	SWPM_ERR mRet;
	#endif
	*pLen = 0;

	#ifdef SWPM_RX_INT	
	
	while((!nLoops) || (i--))
	{
		NVIC_DisableIRQ(SWPM_IRQn);	// disable SWPM interrupt
		if(gMutex)
		{
			if(RxFrame.nStat == SWPM_OK)
			{
				memcpy(pData, RxFrame.nData, RxFrame.nLen);
				*pLen = RxFrame.nLen;
				RxFrame.nLen = 0;	
			}
			mRet = RxFrame.nStat;
			gMutex = 0;
			NVIC_EnableIRQ(SWPM_IRQn);	// enable SWPM interrupt
			return mRet;
		}
		else
		{
			NVIC_EnableIRQ(SWPM_IRQn);	// enable SWPM interrupt
			continue;
		}	
	}
	return SWPM_RX_TIMEOUT;
	#else
	while((!nLoops) || (i--)) 	// wait RxSof
	{
		if(SwpmRegs.INT.bit.RxSof) 
			break;
	}
	if(SwpmRegs.INT.bit.RxSof==0) return SWPM_RX_SOF_ABSENT;	// no sof, time out
	SwpmRegs.INT.all = 0x1000100;	//clear resume、sof
	// ready to receive data and wait for receive done
	i = nLoops;	
	len = 0;
	while((!nLoops) || (i--))
	{
		if(SwpmRegs.INT.bit.RxDone) RxDoneFlag = 1;	// RxDone
		while(SwpmRegs.INT.bit.RxFifoNotEmpty)
		{
			*pData++ = SwpmRegs.RX_FIFO;
			(*pLen)++;
		}
		if(RxDoneFlag) break;
	}
	if(RxDoneFlag == 0) return SWPM_RX_TIMEOUT;
	SwpmRegs.INT.all = 0x200;	//Clear RxDone
	if((*pLen>30) || (SwpmRegs.LEN.bit.RxLen != *pLen)) 
	{
		__nop();
		return SWPM_RX_LEN_UNMATCH;
	}	
	// error check
	if(SwpmRegs.STATUS.bit.RxCrcErr) return SWPM_RX_CRC_ERR;
	else if(SwpmRegs.STATUS.bit.RxEofNotFoundErr) return SWPM_RX_EOF_ABSENT;
	else if(SwpmRegs.STATUS.bit.RxFifoOverFlow) return SWPM_RX_OVERFLOW;
	else return SWPM_OK;
	#endif
}

/****************************************************************************
 * Subroutine:	SWPM_Active
 * Function:	to active SWP UICC 
 * Input:		UINT32 nBaudRate -- to specify the baud rate for SWP S1.
 *				UINT8 *pData --	to specify the a frame data buffer for read. 
 *				UINT8 *pLen -- to return the length of the frame.
 *				UINT32 nLoops --  to specify the loops for waiting the frame.
 * Output:		SWPM_ERR	
 * Description:	The frame data is only the payload, excluding the CRC bytes,
 *				SOF and EOF. The length for data in limited to 30 
 * Date:		2012.12.01
 * ModifyRecord:
 * *************************************************************************/
SWPM_ERR SWPM_Active(UINT32 nBaudRate, UINT8 *pData, UINT8 *pLen, UINT32 nLoops)
{
	UINT32 nSysClk = SCM_GetSysClk();
	if(nBaudRate*4>nSysClk)	return SWPM_PARAM_ERR;
	SwpmRegs.S1_PERIOD = nSysClk/4/nBaudRate; 	// set S1 period

#ifdef SWPM_RX_INT
	memset(RxFrame.nData,0,30);
	RxFrame.nLen = 0;
	RxFrame.nStat = SWPM_OK;
#endif

	SwpmRegs.CTRL.bit.RxEn = 1;	// enable receive
	SwpmRegs.CTRL.bit.Suspend = 1;	// enter to Suspend state from deactived state
	
	// then wait for ACT SYNC_ID
	return SWPM_GetFrame(pData, pLen, nLoops);
}

/****************************************************************************
 * Subroutine:	SWPM_Handler
 * Function:	to handle the SWPM interrupt for receive
 * Input:		None
 * Output:		None	
 * Description:	 
 * Date:		2012.12.03
 * ModifyRecord:
 * *************************************************************************/
#ifdef SWPM_RX_INT
void SWPM_Handler(void)
{
	union SWPM_INT_REG nInt;
	UINT8 i;

	nInt.all = SwpmRegs.INT.all;
	SwpmRegs.INT.all = nInt.all;	// clear 

	
	if(nInt.bit.Resume)	// Resume
	{
	
	}
	if(nInt.bit.RxSof) 	// SOF
	{
		gMutex = 0;
		RxFrame.nLen = 0;	// start to receive a new packet
		RxFrame.nStat = SWPM_OK;
	}	
	while(SwpmRegs.INT.bit.RxFifoNotEmpty)		// not empty
	{
		RxFrame.nData[RxFrame.nLen++] = SwpmRegs.RX_FIFO;
	}
	if(nInt.bit.RxDone)	 	// done
	{	
		if(SwpmRegs.STATUS.bit.RxFifoOverFlow) 	// receive FIFO is overflow
		{
			RxFrame.nStat = SWPM_RX_OVERFLOW;
		}
		else if(SwpmRegs.STATUS.bit.RxEofNotFoundErr) 	// no EOF but data length is 30 bytes
		{
			RxFrame.nStat = SWPM_RX_EOF_ABSENT;
		}
		else if(SwpmRegs.STATUS.bit.RxCrcErr)	// CRC error
		{
			RxFrame.nStat = SWPM_RX_CRC_ERR;
		}		
		else 	// received, but no error
		{
			if(RxFrame.nLen != 	SwpmRegs.LEN.bit.RxLen) RxFrame.nStat = SWPM_RX_LEN_UNMATCH;
		   	else RxFrame.nStat = SWPM_OK;
		}
		if(RxFrame.nStat!=SWPM_OK)SwpmRegs.CTRL.bit.RxFifoClr = 1;	// clear receive FIFO
		SwpmRegs.CTRL.bit.RxFifoClr = 1;	// clear receive FIFO
		gMutex = 1;		// post mutex semaphore
		SwpmRegs.CTRL.bit.RxEn = 0;	// enable receive
	}
}
#endif
