/***************************************************************************
* File name    :	SY03_ROM.h
* Function     :	Header of SY03 ROM Function 
* Author       : 	coins
* Date         :	2013/02/02
* Version      :    v1.0
* Description  :    the ROM Function header for SY03
* ModifyRecord :
****************************************************************************/
#ifndef SY03_ROM_H_
#define SY03_ROM_H_

/***************************************************************************
* Include Header Files
***************************************************************************/

/**************************************************************************
* Global Macro Definition
***************************************************************************/
//#define	_SCM_SetSysClk 		((FP_SCM_SET_SYSCLK)0x00001111)
//#define	_SCM_LoadHFRoscTrim	((FP_SCM_LOAD_HFROSC_TRIM)0x00001195)
//#define	_EFC_Program		((FP_EFC_PROGRAM)0x00000809)
//#define	_EFC_IsEraseOK		((FP_EFC_ISERASEOK)0x0000098d)
//#define	_EFC_Erase			((FP_EFC_ERASE)0x000008a1) //
//#define	_EFC_PageProg		((FP_EFC_PAGE_PROG)0x000009c5)
//#define	_EFC_ConfigTiming	((FP_EFC_CONFIG_TIMING)0x00000a3f)
//#define	_EFC_WriteInfo	((FP_FLASH_WRITE_INFO)0x00000a59)
//#define	_EFC_Chip_Erase	((EFC_CHIP_ERASE)0x00000953)
//
//#define	_SQI_PageProg		((FP_SQI_PAGE_PROG)0x00000d73)
//#define	_SQI_Init			((SQI_INIT)0x00000eb7)
//#define	_SQI_ReadInfo		((SQI_READ_INFO)0x000010af)
//#define	_SQI_WriteInfo		((SQI_WRITE_INFO)0x00001019)

/**************************************************************************
* Global Type Definition
***************************************************************************/
// define the ROM function type
typedef void (*FP_EFC_PROGRAM)(UINT32 *pData, UINT32 addr_align, UINT32 nLen);
typedef void (*FP_EFC_ERASE)(UINT32 addr_offset, UINT32 page_num);
typedef BOOL (*FP_EFC_ISERASEOK)(UINT32 StrAddr, UINT32 EndAddr);
typedef void (*FP_EFC_PAGE_PROG)(UINT32 addr_offset, UINT32 *pData);
typedef void (*FP_EFC_CONFIG_TIMING)(UINT8 nReadMode, UINT32 nTC0, UINT32 nTC1);
typedef UINT8 (*FP_SCM_SET_SYSCLK)(UINT8 nClk); 
typedef	UINT8 (*FP_SCM_LOAD_HFROSC_TRIM)(void);
typedef UINT8 (*FP_FLASH_WRITE_INFO)(UINT8 *pDataBuf,UINT32 lAddress,UINT32 lLen);
typedef void (*EFC_CHIP_ERASE)(UINT32 StrAddr, UINT32 EndAddr);

typedef UINT8 (*FP_SQI_PAGE_PROG)(UINT32 byte_addr, UINT32 *pData);
typedef UINT8 (*SQI_INIT)(UINT8 clk_div, UINT8 read_mode);
typedef UINT8 (*SQI_READ_INFO)(UINT8* pDataBuf,UINT32 nOffset,UINT16 nDataNum);
typedef UINT8 (*SQI_WRITE_INFO)(UINT8 *pDataBuf, UINT32 lAddress, UINT32 lLen);
	  
/**************************************************************************
* Global Variable Declaration
***************************************************************************/
//extern FP_SCM_SET_SYSCLK		_SCM_SetSysClk; 		
//extern FP_SCM_LOAD_HFROSC_TRIM	_SCM_LoadHFRoscTrim;	
//extern FP_EFC_PROGRAM 			_EFC_Program;
//extern FP_EFC_ISERASEOK			_EFC_IsEraseOK;	
//extern FP_EFC_ERASE				_EFC_Erase;	
//extern FP_EFC_PAGE_PROG			_EFC_PageProg;		
//extern FP_EFC_CONFIG_TIMING		_EFC_ConfigTiming;
//extern FP_FLASH_WRITE_INFO		_EFC_WriteInfo;	
//extern EFC_CHIP_ERASE			_EFC_Chip_Erase;
//extern FP_SQI_PAGE_PROG			_SQI_PageProg;
//extern SQI_INIT					_SQI_Init;		
//extern SQI_READ_INFO			_SQI_ReadInfo;		
//extern SQI_WRITE_INFO			_SQI_WriteInfo;	

/**************************************************************************
* Global Functon Declaration
***************************************************************************/

#endif
