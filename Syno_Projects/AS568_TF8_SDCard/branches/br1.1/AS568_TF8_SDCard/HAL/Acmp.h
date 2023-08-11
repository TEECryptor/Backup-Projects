/***************************************************************************************
* File name    :	Acmp.h
* Function     :	Header of Acmp.c
* Author       : 	Leixj
* Date         :	2012/06/29
* Version      :    v1.0
* Description  :    Header of ACMP Hardware Abstract Layer
* ModifyRecord :
*****************************************************************************************/
#ifndef ACMP_H_
#define ACMP_H_

/***************************************************************************
* Include Header Files
***************************************************************************/


/**************************************************************************
* Global Macro Definition
***************************************************************************/
#define	REF_VOL_INNER	0	 //0.7v
#define	REF_VOL_EXT		1	 //ACMP-

#define	ACMP_EDGE_NEG_INT	0 	// negative edge interrupt request
#define	ACMP_EDGE_POS_INT	1	// positive edge interrupt request


/**************************************************************************
* Global Type Definition
***************************************************************************/


/**************************************************************************
* Global Variable Declaration
***************************************************************************/


/**************************************************************************
* Global Functon Declaration
***************************************************************************/
UINT8 ACMP_Init(UINT8 nIEn, UINT8 nIntEdge, UINT8 nRefSel, UINT8 nAcmpOutEn);
__INLINE UINT8 ACMP_ReadResult(void);


/**************************************************************************
* Functon
***************************************************************************/
/***********************************************************************************
 * Subroutine:	ACMP_ReadResult
 * Function:	Read the ACMP result
 * Input:		none
 * Output:		UINT32 -- the result
 * Description:
 * Date:		2012.06.29
 * ModifyRecord:
 * ********************************************************************************/
__INLINE UINT8 ACMP_ReadResult(void)
{
	return	AcmpRegs.STAT.bit.SyncOut;
} 

#endif /*ACMP_H_*/

