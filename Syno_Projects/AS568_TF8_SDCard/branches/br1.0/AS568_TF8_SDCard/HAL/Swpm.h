/***************************************************************************
* File name    :	Swpm.h
* Function     :	Header of SWP Master controller
* Author       : 	coins
* Date         :	2012/12/01
* Version      :    v1.0
* Description  :    
* ModifyRecord :
****************************************************************************/
#ifndef SWPM_H_
#define SWPM_H_

/***************************************************************************
* Include Header Files
***************************************************************************/



/**************************************************************************
* Global Macro Definition
***************************************************************************/

	

/**************************************************************************
* Global Type Definition
***************************************************************************/
// define the return of SWPM operation
typedef	enum	{
	SWPM_OK 	= 0,	// SWPM operation ok
	SWPM_FAIL	= 1,   	// SWPM error but unspecified
	SWPM_PARAM_ERR =2,
	SWPM_TX_INHIBIT = 3,	
	SWPM_TX_UNDERFLOW = 4,
	SWPM_TX_FRAME_ERR = 5,
	SWPM_TX_TIMEOUT = 6,
	SWPM_RX_SOF_ABSENT = 7,
	SWPM_RX_CRC_ERR = 8,
	SWPM_RX_EOF_ABSENT = 9,
	SWPM_RX_OVERFLOW = 10,
	SWPM_RX_LEN_UNMATCH = 11,
	SWPM_RX_TIMEOUT = 12,
}SWPM_ERR;

// define the SWP State
#ifndef	_TYPE_SWP_STATE_
#define	_TYPE_SWP_STATE_
typedef	enum	{
	SWP_DEACTIVATED 	= 0,	// SWPS operation ok
	SWP_CLF_START = 1,
	SWP_SUSPENDED = 2,
	SWP_ACTIVATED = 3,	
}SWP_STATE;
#endif

/**************************************************************************
* Global Variable Declaration
***************************************************************************/


/**************************************************************************
* Global Functon Declaration
***************************************************************************/
SWPM_ERR SWPM_Init(void);
SWPM_ERR SWPM_SendFrame(UINT8 *pData, UINT8 nLen);
SWPM_ERR SWPM_GetFrame(UINT8 *pData, UINT8 *pLen, UINT32 nLoops);
SWPM_ERR SWPM_Active(UINT32 nBaudRate, UINT8 *pData, UINT8 *pLen, UINT32 nLoops);



#endif



