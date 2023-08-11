/***************************************************************************************
* File name    :	RomFunc.h
* Function     :	The header of RomFunc.c
* Author       : 	Emil
* Date         :	2015/07/03
* Version      :    v1.0
* Description  :    
* ModifyRecord :
*****************************************************************************************/
#ifndef _ROMFUNC_H_
#define _ROMFUNC_H_

/***************************************************************************
* Include Header Files
***************************************************************************/



/**************************************************************************
* Global Macro Definition
***************************************************************************/

	

/**************************************************************************
* Global Type Definition
***************************************************************************/


/**************************************************************************
* Global Variable Declaration
***************************************************************************/
extern FP_SCM_SET_SYSCLK		_SCM_SetSysClk; 		
extern FP_SCM_LOAD_HFROSC_TRIM	_SCM_LoadHFRoscTrim;	
extern FP_EFC_PROGRAM 			_EFC_Program;
extern FP_EFC_ISERASEOK			_EFC_IsEraseOK;	
extern FP_EFC_ERASE				_EFC_Erase;	
extern FP_EFC_PAGE_PROG			_EFC_PageProg;		
extern FP_EFC_CONFIG_TIMING		_EFC_ConfigTiming;
extern FP_FLASH_WRITE_INFO		_EFC_WriteInfo;	
extern EFC_CHIP_ERASE			_EFC_Chip_Erase;
extern FP_SQI_PAGE_PROG			_SQI_PageProg;
extern SQI_INIT					_SQI_Init;		
extern SQI_READ_INFO			_SQI_ReadInfo;		
extern SQI_WRITE_INFO			_SQI_WriteInfo;	

/**************************************************************************
* Global Functon Declaration
***************************************************************************/
/****************************************************************************
 * Subroutine:	RomFuncInit
 * Function:	to initialize the ROM Func
 * Input:		UINT8 Type(0:AS568 chip  l:TF Card)
 * Output:		None
 * Description:	to initialize the ROM Func 
 * Date:		2015.06.10
 * ModifyRecord:
 * *************************************************************************/
void RomFuncInit (UINT8 Type);

#endif
