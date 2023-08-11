/***************************************************************************************
* File name    :	Efc.h
* Function     :	The header of Efc.c
* Author       : 	coins
* Date         :	2012/09/13
* Version      :    v1.0
* Description  :
* ModifyRecord :
*****************************************************************************************/
#ifndef EFC_H_
#define EFC_H_

/***************************************************************************
* Include Header Files
***************************************************************************/

/**************************************************************************
* Global Macro Definition
***************************************************************************/
#define		FLASH_BASE_ADDR			0x10000000
#define		FLASH_PAGE_SIZE			1024

//------ Define Check Code for Write/Erase Protect --------------------------
#define		AREA_OTP_TEST_WR	0x00005678
#define		AREA_PROG_WE		0x11115678
#define		AREA_DATAPRO_WE		0x33335678
#define		AREA_PARA_WE		0x44445678
#define		AREA_OTP_USER_WR	0x77775678
#define		AREA_OTP_JTAG_WR	0x88885678
#define		AREA_OTP_USER_DIS	0x66665678
#define		AREA_OTP_EO_ST		0x99995678
#define		AREA_PROT_DI		0x0

//------ Define the Different Area Start Address ---------------------------
#define		ASA_PARA		0x00040000		// size 1 page, fixed
#define		ASA_PROG		0x00000000		// size 184 pages by default, alterable
#define		ASA_DATAPRO		0x0002E000		// size 72 pages by default, alterable
#define		ASA_EO			0x00040000		// size 0 pages by default, alterable

//------ Define the para for TC0 and TC1
#define		TC0_6M		0x22111112
#define		TC0_12M		0x22111112
#define		TC0_24M		0x22111112
#define		TC0_48M		0x22211114

#define		TC1_6M		0x02020200
#define		TC1_12M		0x02020400
#define		TC1_24M		0x02020800
#define		TC1_48M		0x02040F00		

#define ERASE_MAX_TIMES	30
// define the function name for old firmware library revision
#define	Flash_TimConfig(TC0, TC1)	EFC_ConfigTiming(0, TC0, TC1)	
#define	Flash_Program		EFC_Program
#define	Flash_Erase			EFC_Erase
#define	Flash_WriteInfo		EFC_WriteInfo
#define	Flash_ReadInfo		EFC_ReadInfo
#define	OTPWrite			EFC_OtpWrite
#define	OTPRead				EFC_OtpRead
#define	ReadChipSN			EFC_ReadChipSN
#define	JtagLock			EFC_LockJtag
#define	SetCodeSt			EFC_SetEoSt
#define	ClrProgFlag			EFC_ClrProgFlag
#define	Flash_PageProg		EFC_PageProg		

/**************************************************************************
* Global Type Definition
***************************************************************************/


/**************************************************************************
* Global Variable Declaration
***************************************************************************/


/**************************************************************************
* Global Functon Declaration
***************************************************************************/
void EFC_ConfigTiming(UINT8 nReadMode, UINT32 nTC0, UINT32 nTC1);
void EFC_Program(UINT32 *pData, UINT32 addr_align, UINT32 nLen);
UINT8 EFC_Erase(UINT32 addr_offset, UINT32 page_num);
void EFC_ChipErase(void);
BOOL EFC_IsEraseOK(UINT32 StrAddr, UINT32 EndAddr);
void EFC_PageProg(UINT32 addr_offset, UINT32 *pData);
UINT8 EFC_WriteInfo(UINT8 *pDataBuf, UINT32 lAddress, UINT32 lLen);
UINT8 EFC_ReadInfo(UINT8 *pDataBuf, UINT32 lAddress, UINT32 lLen);
UINT8 EFC_OtpWrite(UINT32 nAddress, UINT32 nNum, UINT32* pBuf);
UINT8 EFC_OtpRead(UINT32 nAddress, UINT32 nNum, UINT32 *pBuf);
void EFC_ReadChipSN(UINT8 *pSN, UINT16* pLen);
UINT8 EFC_LockJtag(void);
UINT8 EFC_SetEoSt(UINT32 nPage);
void EFC_ClrProgFlag(UINT32 Key);
UINT8 EFC_LockTest(void);
__INLINE void EFC_WrProtect(UINT32 permission);
__INLINE void EFC_DisEncrypt(void);
__INLINE void EFC_ConfigCodeSz(UINT32 nPage);


/**************************************************************************
* Functon
***************************************************************************/

/***************************************************************************
* Subroutine:	EFC_WrProtect
* Function:		Embedded flash write and erase protect enable or disable with proper
* 				check code.
* Input:		UINT32 permission	-- 	the permission for an operation;
* Output:		None
* Description:	the permission value defined as following:
*				0x0000_5678 -- 	TEST locked field writable
* 				0x1111_5678 -- 	program area writable and erasable;
* 				0x3333_5678 -- 	protected data area writable and erasable;
* 				0x4444_5678 -- 	Parameter area writable and erasable;
*				0x6666_5678 -- 	OTP area un-writable;
*				0x7777_5678 --	User field writable;
*				0x8888_5678 --	JTAG locked field writable;
*				0x9999_5678 --	excutable only area start address field writable.
* Date:		2012.09.21
* ModifyRecord:
* *************************************************************************/
__INLINE void EFC_WrProtect(UINT32 permission)
{
	EfcRegs.CHECK = permission;
}

/**************************************************************************
* Subroutine:	EFC_DisEncrypt
* Function:		to disable flash encrypt operation
* Input:		None
* Output:		None
* Description:
* Date:			2012.09.21
* ModifyRecord:
* *************************************************************************/
__INLINE void EFC_DisEncrypt(void)
{
	EfcRegs.CTRL.all &= ~EFC_CTRL_ENCRYPT;
}

/**************************************************************************
* Subroutine:	EFC_ConfigCodeSz
* Function:		to config code area size
* Input:		UINT32 nPage --	to specify the page numbers for code area
* Output:		None
* Description:
* Date:			2012.09.21
* ModifyRecord:
* *************************************************************************/
__INLINE void EFC_ConfigCodeSz(UINT32 nPage)
{
	EfcRegs.DATAPRO = nPage*FLASH_PAGE_SIZE;
}

#endif /* EFC_H_ */
