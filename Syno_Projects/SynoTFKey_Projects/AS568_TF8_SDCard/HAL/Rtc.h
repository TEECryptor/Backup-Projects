/***************************************************************************************
* File name    :	RTC.h
* Function     :	The header of RTC.c
* Author       : 	Hongjz
* Date         :	2012/07/26
* Version      :    v1.0
* Description  :    
* ModifyRecord :
*****************************************************************************************/
#ifndef RTC_H_
#define RTC_H_

/***************************************************************************
* Include Header Files
***************************************************************************/

/**************************************************************************
* Global Macro Definition
***************************************************************************/
// define the RTC interrupt type
typedef enum
{
	NO_INT			=0,		//Not open RTC interrupt
	SEC_INT			=1,		// each second interupt
	HSEC_INT		=2,		// each half second interrupt
	MIN_INT			=4,	 	// each minute interrupt
}RTC_INT;

/**************************************************************************
* Global Type Definition
***************************************************************************/

typedef struct{ 
    UINT16 year;
    UINT8 month;
    UINT8 day;
    UINT8 week;
    UINT8 hour;
    UINT8 minute;
    UINT8 second;
}DATA_FORM;


/**************************************************************************
* Global Variable Declaration
***************************************************************************/


/**************************************************************************
* Global Functon Declaration
***************************************************************************/
void RTC_Init(UINT32 nInitVal, RTC_INT nIntType);
void RTC_SetIntType(RTC_INT nIntType);
UINT32 RTC_DataToCV(DATA_FORM sDataForm);
DATA_FORM RTC_CVToData(UINT32 nCCVR);
__INLINE void RTC_WrInitVal(UINT32 nLoad);
__INLINE UINT32 RTC_ReadData(void);

/**************************************************************************
* Functon
***************************************************************************/

/****************************************************************************
 * Subroutine:	RTC_WrInitVal
 * Function:	to write RTC initialization value
 * Input:		UINT32 nLoad -- the initialization value
 * Output:		None	
 * Description:	 
 * Date:		2012.07.27
 * ModifyRecord:
 * *************************************************************************/
__INLINE void RTC_WrInitVal(UINT32 nLoad)
{
	RtcRegs.LOAD = nLoad;	
}


/****************************************************************************
 * Subroutine:	RTC_ReadData
 * Function:	to read RTC current value
 * Input:		UINT32 nLoad -- the initialization value
 * Output:		None	
 * Description:	 
 * Date:		2012.07.27
 * ModifyRecord:
 * *************************************************************************/
__INLINE UINT32 RTC_ReadData(void)
{
	return RtcRegs.CCVR;
}


#endif 	 /* RTC_H_ */
