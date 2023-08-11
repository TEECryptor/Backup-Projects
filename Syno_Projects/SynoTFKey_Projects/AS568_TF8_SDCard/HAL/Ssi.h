/***************************************************************************
* File name    :	Ssi.h
* Function     :	Header of SSI driver
* Author       : 	coins
* Date         :	2012/12/21
* Version      :    v1.0
* Description  :    
* ModifyRecord :
****************************************************************************/
#ifndef SSI_H_
#define SSI_H_

/***************************************************************************
* Include Header Files
***************************************************************************/



/**************************************************************************
* Global Macro Definition
***************************************************************************/

	

/**************************************************************************
* Global Type Definition
***************************************************************************/
// Define the SSI Port Map to GPIO Port
typedef	enum {

	SSI_MAP_B11_14 = 0,
			//GPB14 -- SSI_SSN	
			//GPB12 -- SSI_CLK	
			//GPB11 -- SSI_TXD	
			//GPB13 -- SSI_RXD
}SSI_MAP;

/**************************************************************************
* Global Variable Declaration
***************************************************************************/


/**************************************************************************
* Global Functon Declaration
***************************************************************************/
UINT8 SSI_SpiSlvInit(SSI_MAP mPortMap, UINT8 nCPOL, UINT8 nCPHA, UINT8 nDFS, \
						UINT8 *pRxBuf, UINT16 nBufSize);
UINT8 SSI_WaitTxIdle(UINT32 nTimeOut);
UINT8 SSI_SpiSlvWrite(UINT8 *pData, UINT16 nLen, UINT32 nTimeOut);
void SSI_StopReceive(void);
void SSI_StartReceive(UINT16 nDummyByte);
UINT16 SSI_GetRxSize(void);
__INLINE void SSI_TransOnly(void);
__INLINE void SSI_RecevOnly(void);
__INLINE void SSI_TransceiveBoth(void);

/****************************************************************************
 * Subroutine:	SSI_TransOnly
 * Function:	to set SSI Transmit Only mode 
 * Input:		None 
 * Output:		None	
 * Description:	 
 * Date:		2012.12.28
 * ModifyRecord:
 * *************************************************************************/
__INLINE void SSI_TransOnly(void)
{
	SsiRegs.CTRLR0.bit.Tmod = 0x1;	// transmit only
}

/****************************************************************************
 * Subroutine:	SSI_RecevOnly
 * Function:	to set SSI Receive Only mode 
 * Input:		None 
 * Output:		None	
 * Description:	 
 * Date:		2012.12.28
 * ModifyRecord:
 * *************************************************************************/
__INLINE void SSI_RecevOnly(void)
{
	SsiRegs.CTRLR0.bit.Tmod = 0x2;	// receive only
}

/****************************************************************************
 * Subroutine:	SSI_TransceiveBoth
 * Function:	to set SSI both Transmit and Receive  mode 
 * Input:		None 
 * Output:		None	
 * Description:	 
 * Date:		2012.12.28
 * ModifyRecord:
 * *************************************************************************/
__INLINE void SSI_TransceiveBoth(void)
{
	SsiRegs.CTRLR0.bit.Tmod = 0x0;	// receive only
}

#endif

