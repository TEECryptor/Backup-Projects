/***************************************************************************************
* File name    :	Acmp.c
* Function     :	The functions for Acmp
* Author       : 	Leixj
* Date         :	2012/06/29
* Version      :    v1.0
* Description  :    Acmp Hardware Abstract Layer
* ModifyRecord :
*****************************************************************************************/
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


/**************************************************************************
* Global Variable Declaration
***************************************************************************/


/**************************************************************************
* Local Functon Declaration
***************************************************************************/


/**************************************************************************
* Functon
***************************************************************************/

/***********************************************************************************
 * Subroutine:	ACMP_Init
 * Function:	set ACMP work mode
 * Input:		UINT8 nIEn -- Enable interrupt, 0: disabled, 1: enabled
 * 				UINT8 nIntEdge -- Select interrupt edge
 *				UINT8 nRefSel -- Select the reference voltage
 *				UINT8 nAcmpOutEn --	to enable the result output from Pin, 0: disabled, 1: enabled
 * Output:		UINT8 --			RT_OK or RT_FAIL
 * Description:
 * Date:		2012.06.29
 * ModifyRecord:
 * ********************************************************************************/
UINT8 ACMP_Init(UINT8 nIEn, UINT8 nIntEdge, UINT8 nRefSel, UINT8 nAcmpOutEn)
{
	SCM_EnableModClk(ID_ACMP);
	SCM_ReleaseModRst(ID_ACMP);

	if(nAcmpOutEn)GpioRegs.GPAUSE2.bit.GPA22 = MUX_ACMP;	//A22	GPIO	ACMPO
	if(nRefSel == REF_VOL_EXT)GpioRegs.GPAUSE2.bit.GPA20 = MUX_ACMP;	//A20	GPIO	ACMP-										
	GpioRegs.GPAUSE2.bit.GPA21 = MUX_ACMP;	//A21	GPIO	ACMP+	

	AcmpRegs.CTRL.bit.En = 0;  //Disable ACMP
	AcmpRegs.CTRL.bit.RefSel = nRefSel;  //select the Reference Voltage Source

	switch(nIEn)			  
	{
		case 0:
			AcmpRegs.CTRL.bit.IEn = 0;
			break;
		case 1:
			AcmpRegs.STAT.bit.IntFlag = 1;  //clear interrupt flag
			AcmpRegs.CTRL.bit.IntEdge = nIntEdge;
			AcmpRegs.CTRL.bit.IEn = 1;  //enable interrupt
			break;
		default :
			return RT_FAIL;
	} 
	
	SCM_AnModPowerEnable(AN_ACMP);
	AcmpRegs.CTRL.bit.En = 1;  //Enable ACMP for digital part

	return RT_OK;		
}

 
