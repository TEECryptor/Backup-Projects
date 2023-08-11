/**********************************************************************************
* File name    :	Efc.c
* Function     :	The base function for embedded flash controller
* Author       : 	coins
* Date         :	2012/09/13
* Version      :    v1.0
* Description  :	
* ModifyRecord :
**********************************************************************************/

/**********************************************************************************
* Include Header Files
**********************************************************************************/
#include "SY03.h"


/**********************************************************************************
* Local Macro Definition
**********************************************************************************/
#define	EFC_ROM_FUNC_USED

/**********************************************************************************
* Local Type Definition
**********************************************************************************/


/**********************************************************************************
* Local static Variable Declaration
**********************************************************************************/


/**********************************************************************************
* Global Variable Declaration
**********************************************************************************/



/**********************************************************************************
* Local Functon Declaration
**********************************************************************************/


/**********************************************************************************
* Functon
**********************************************************************************/


/***************************************************************************
* Subroutine:	EFC_ConfigTiming
* Function:		to config EFC timing.
* Input:		UINT8 nReadMode -- 	to set read mode.
*						 			0/3 -- for Txa Timing;
*									1 -- for one cycle read;
*									2 -- for two cycle read;		
*				UINT32 nTC0	-- 	to specify the TC0 timing register value;
* 				UINT32 nTC1 --	to specify the TC1 timing register value;
* Output:		None;
* Description:	if EFC_ROM_FUNC_USED is defined, ROM function will be called
* Date:			2012.09.21
* ModifyRecord:
* *************************************************************************/
void EFC_ConfigTiming(UINT8 nReadMode, UINT32 nTC0, UINT32 nTC1)
{
	#ifndef	EFC_ROM_FUNC_USED
	EfcRegs.CTRL.bit.ReadMode = nReadMode;
	EfcRegs.TC0.all = nTC0;
	EfcRegs.TC1.all = nTC1;
	#else
	_EFC_ConfigTiming(nReadMode, nTC0, nTC1);
	if(nReadMode > 1)
		EfcRegs.CTRL.bit.ReadMode = nReadMode;
	#endif
}

/***************************************************************************
* Subroutine:	EFC_Program
* Function:		flash program.
* Input:		UINT32 *pData	-- 	the data to be programmed, word aligned;
* 				UINT32 addr_align --	the offset address word-aligned;
* 				UINT32 nLen --		the length of data, size in word.
* Output:		None;
* Description:	Program nLen word into flash address addr_align.
* 				if EFC_ROM_FUNC_USED is defined, ROM function will be called
* Date:			2012.09.21
* ModifyRecord:
* *************************************************************************/
void EFC_Program(UINT32 *pData, UINT32 addr_align, UINT32 nLen)
{
	#ifndef	EFC_ROM_FUNC_USED
    UINT32 lRegStatus;
	volatile UINT32 * pSysAddr = (volatile UINT32 *)(FLASH_BASE_ADDR+addr_align);
    lRegStatus = __get_PRIMASK();	// save global interrput
    __disable_irq();	// close interrupt
	while(nLen--)
	{
		*pSysAddr++ = *pData++;		// program a word (32 bits)
		while(EfcRegs.STA);
	}
	__set_PRIMASK(lRegStatus); 	// recover interrupt
	#else
	UINT32 nCtrl = EfcRegs.CTRL.all;	// cannot be programed in multi-cycle-read mode, 2013.03.28 
	EfcRegs.CTRL.bit.ReadMode = 0;
	__nop();__nop();
	_EFC_Program((UINT32 *)pData, addr_align, nLen);
	EfcRegs.CTRL.all = nCtrl; 
	#endif
}

#ifndef	EFC_ROM_FUNC_USED
/***************************************************************************
* Subroutine:	EFC_Erase1
* Function:		Embedded flash page erase. A page is 1K-byte aligned.
* Input:		UINT32 addr_offset	-- 	the flash offset address of byte;
* 				UINT32 page_num --	page number n;
* Output:		None;
* Description:	to erase n pages from the page that the addr_offset included.
* 		
* Date:			2012.09.21
* ModifyRecord:
* *************************************************************************/
void EFC_Erase1(UINT32 addr_offset, UINT32 page_num)
{
 
    UINT32 lRegStatus;
	UINT32 i;
    lRegStatus = __get_PRIMASK();	// save global interrput
    __disable_irq();	// close interrupt
	for(i=0;i<page_num;i++)
	{
		EfcRegs.WREN.all = (addr_offset+i*FLASH_PAGE_SIZE) | 0x80000000;
		EfcRegs.PEBA = addr_offset+i*FLASH_PAGE_SIZE;
		EfcRegs.CTRL.all |= EFC_CTRL_PECMD;
		while(EfcRegs.STA);
	}
	__set_PRIMASK(lRegStatus); 	// recover interrupt
 
}
#endif

/***************************************************************************
* Subroutine:	EFC_Erase
* Function:		Embedded flash page erase. A page is 1K-byte aligned.
* Input:		UINT32 addr_offset	-- 	the flash offset address of byte;
* 				UINT32 page_num --	page number n;
* Output:		None;
* Description:	to erase n pages from the page that the addr_offset included.
* 				if EFC_ROM_FUNC_USED is defined, ROM function will be called
* Date:			2012.09.21
* ModifyRecord:
* *************************************************************************/
UINT8 EFC_Erase(UINT32 addr_offset, UINT32 page_num)
{
	// cannot be programed in multi-cycle-read mode, 2013.03.28 
	// to change flash read mode by calling ROM function, 2103.12.31
	UINT8 cnt, err = 0;
	UINT32 page;
	UINT32 nCtrl = EfcRegs.CTRL.all;	// cannot be programed in multi-cycle-read mode, 2013.03.28 
	EfcRegs.CTRL.bit.ReadMode = 0;
	__nop();__nop();
	addr_offset &= ~(FLASH_PAGE_SIZE-1);	// align to FLASH_PAGE_SIZE 
	for(page=0;page<page_num;page++)
	{
		cnt = ERASE_MAX_TIMES;	// erase times
		while(cnt--)
		{
			_EFC_Erase(addr_offset, 1);
			err = 0;
			if(EFC_IsEraseOK(addr_offset/1024, addr_offset/1024) != true)
				err = 1;

			if(err==0)
			{
				addr_offset += FLASH_PAGE_SIZE;
				break;
			}
		}
		if(err==1) break;	
	} 
	EfcRegs.WREN.all = 0;
	EfcRegs.CTRL.all = nCtrl; 
	if(err) return RT_FAIL;
	else return RT_OK;
}
/***************************************************************************
* Subroutine:	EFC_IsEraseOK
* Function:		Check flash page erase is successful. A page is 1K-byte aligned.
* Input:		UINT32 StrAddr	-- 	the start block(page)
* 				UINT32 EndAddr --	the end block(page)
* Output:		true
*               false 
* Description:	shuold call ROM function 
* Date:			2014.07.21
* ModifyRecord:
* *************************************************************************/
BOOL EFC_IsEraseOK(UINT32 StrAddr, UINT32 EndAddr)
{
	#ifndef	EFC_ROM_FUNC_USED
    UINT32 i,j;
    UINT32 *pAddr;
    UINT32 lStartAddr;
	UINT32 Enctrydata = EfcRegs.CTRL.all;
	EfcRegs.CTRL.all &= ~EFC_CTRL_ENCRYPT;	 
	for(i=StrAddr;i<=EndAddr;i++)
	{
	    lStartAddr=i*FLASH_PAGE_SIZE;   
	    // first read back the data	
		pAddr=(UINT32*)(FLASH_BASE_ADDR+lStartAddr);     	 
		for(j=0;j<FLASH_PAGE_SIZE/4;j++)
		{
		    if(pAddr[j]!=0xffffffff)
			{
				EfcRegs.CTRL.all = Enctrydata;
			   return false;				
			}
		}
	}
	EfcRegs.CTRL.all = Enctrydata;
	return true;
	#else
	UINT32 lRegStatus;
	BOOL ret ;
    lRegStatus = __get_PRIMASK();	// save global interrput
    __disable_irq();
	ret = _EFC_IsEraseOK(StrAddr, EndAddr);
	__set_PRIMASK(lRegStatus); 	// recover interrupt
	return ret;
	#endif	
}

/***************************************************************************
* Subroutine:	EFC_ChipErase
* Function:		Embedded flash all page(0~256) erase. 
* Input:		
* Output:		None;
* Description:	
* 				
* Date:			2012.09.21
* ModifyRecord:
* *************************************************************************/
void EFC_ChipErase(void)
{
	#ifndef	EFC_ROM_FUNC_USED	

	#else
	UINT32 nEoStConfigAddr = 0x407F8;
	UINT32 EndPage = 255;
		
	EFC_ClrProgFlag(AREA_PARA_WE);
	EFC_WrProtect(AREA_PROG_WE); 
	EfcRegs.DATAPRO=0x10040000;	
	if(*((UINT32 *)(FLASH_BASE_ADDR+nEoStConfigAddr)) != 0xFFFFFFFF)
	{
		EfcRegs.DATAPRO=0x1003e000;
		EndPage = 247;
	}
	_EFC_Chip_Erase(0,EndPage);
	#endif	
}
/***************************************************************************
* Subroutine:	EFC_PageProg
* Function:		Embedded flash program a page.
* Input:		UINT32 addr_offset --	the flash offset address of byte;
* 				UINT32 *pData	-- 	the data to be programmed;
* Output:		None;
* Description:	to program a page that the addr_offset included.
* Date:			2012.09.21
* ModifyRecord:
* *************************************************************************/
void EFC_PageProg(UINT32 addr_offset, UINT32 *pData)
{
	#ifndef	EFC_ROM_FUNC_USED
	EfcRegs.WREN.all = addr_offset | 0x80000000;
	EFC_Program(pData, addr_offset&~(FLASH_PAGE_SIZE-1), FLASH_PAGE_SIZE/4);
	#else
	UINT32 nCtrl = EfcRegs.CTRL.all;	// cannot be programed in multi-cycle-read mode, 2013.03.28 
	EfcRegs.CTRL.bit.ReadMode = 0;
	__nop();__nop();
	_EFC_PageProg(addr_offset, pData);
	EfcRegs.CTRL.all = nCtrl; 
	#endif
	EfcRegs.WREN.all = 0;
}

/***************************************************************************
* Subroutine:	FlashWriteBlock
* Function:		to write a Flash Block(page), Block(page) is 1K-byte aligned.
* Input:		UINT8 *pBuf	-- 	to specify the data to be written;
* 				UINT32 lBlock -- to specify the block (page) to be written;
* 				UINT32 lOffset -- to specify the offset in block for the data
* 								  to be written start;
* 				UINT32 lLen --	to specify the length data to be written
* Output:		UINT8 --	return the result;
* Description:
* Date:			2010.09.02
* ModifyRecord:
* *************************************************************************/
//#ifndef	EFC_ROM_FUNC_USED
static UINT8 FlashWriteBlock(UINT8 *pBuf, UINT32 lBlock, UINT32 lOffset, UINT32 lLen)
{
	UINT32 i;
	UINT8 *pTmp;
	UINT8 Buf[FLASH_PAGE_SIZE];
	UINT8 bMustErase=false;
	UINT32 lStartAddr;

 	if((lOffset+lLen)>FLASH_PAGE_SIZE)
 	return RT_PARAM_ERR;

 	lStartAddr=lBlock*FLASH_PAGE_SIZE;
 	// first read back the data
 	pTmp=Buf;//(UINT8*)TEMP_BUF;

 	memcpy(pTmp,(void *)(FLASH_BASE_ADDR+lStartAddr),FLASH_PAGE_SIZE);

	for(i=0;i<lLen;i++)
	{
	   	*(Buf+lOffset+i)=*(pBuf+i);
	}

	if(EFC_IsEraseOK(lBlock, lBlock) != true)
		bMustErase=true;

    //erase the block
	if(bMustErase == true)
	    EFC_Erase(lStartAddr,1);

    //write to flash
    EFC_PageProg(lStartAddr, (UINT32 *)Buf);
    return RT_OK;
}
//#endif

/***************************************************************************
* Subroutine:	EFC_WriteInfo
* Function:		to write Embedded Flash data area
* Input:		UINT8 *pDataBuf	-- 	to specify the data to be written;
* 				UINT32 lAddress -- to specify the Flash address;
* 				UINT32 lLen --	to specify the length data to be written
* Output:		UINT8 --	return the result;
* Description:	this function is used to write data area , the address from 
* 				(ASA_DATAPRO-ASA_EO).
* 				If the address out of the range, RT_PARAM_ERR will be returned.
* Date:			2012.09.21
* ModifyRecord:
* *************************************************************************/
UINT8 EFC_WriteInfo(UINT8 *pDataBuf, UINT32 lAddress, UINT32 lLen)
{
//	#ifndef	EFC_ROM_FUNC_USED
	UINT32 nStart;
	UINT16 nNum;
	UINT16 nStartBlock,nStartOffset;

	UINT8 *pBuf;
	UINT8 nRet;	   

	//check the data range;
	if(lLen<=0)
		return RT_PARAM_ERR;
	nStart=lAddress;
	nStartBlock=(UINT16)(nStart/FLASH_PAGE_SIZE);
	pBuf=pDataBuf;
	nNum=lLen;
	//loop to write data to flash
	do{
		nStartOffset=(UINT16)(nStart%FLASH_PAGE_SIZE);
		if(nStartOffset)
		{
			
			if(FLASH_PAGE_SIZE-nStartOffset>lLen)
			{
					 
				return FlashWriteBlock(pBuf,nStartBlock,nStartOffset,lLen);
			}
			nRet=FlashWriteBlock(pBuf,nStartBlock,nStartOffset,FLASH_PAGE_SIZE-nStartOffset);
			   
			if(nRet!=RT_OK) return nRet;
			    
			pBuf+=FLASH_PAGE_SIZE-nStartOffset;
			nStart+=FLASH_PAGE_SIZE-nStartOffset;
			nNum-=FLASH_PAGE_SIZE-nStartOffset;
			nStartBlock++;
		}
		else if(nNum>=FLASH_PAGE_SIZE)
		{
			nRet=FlashWriteBlock(pBuf,nStartBlock,0,FLASH_PAGE_SIZE);
				
			if(nRet!=RT_OK) return nRet;
						    
		    pBuf+=FLASH_PAGE_SIZE;
		    nStart+=FLASH_PAGE_SIZE;
		    nNum-=FLASH_PAGE_SIZE;
			nStartBlock++;			  	
		 }			 
		 else
		 {
			nRet=FlashWriteBlock(pBuf,nStartBlock,0,nNum);			 
	   	    if(nRet!=RT_OK) return nRet;
			nNum=0;
		 }
	}while(nNum>0);
			    
	return RT_OK;
//	#else
//	UINT8 nRet;
//	UINT32 nCtrl = EfcRegs.CTRL.all;	// cannot be programed in multi-cycle-read mode, 2013.03.28 
//	EfcRegs.CTRL.bit.ReadMode = 0;
//	__nop();__nop();
//	nRet = _EFC_WriteInfo(pDataBuf, lAddress, lLen);
//	EfcRegs.CTRL.all = nCtrl; 
//	return nRet;
//	#endif
}

/***************************************************************************
* Subroutine:	EFC_ReadInfo
* Function:		to Read Embedded Flash data area
* Input:		UINT8 *pDataBuf	-- 	to specify the data to be Read;
* 				UINT32 lAddress -- to specify the Flash address;
* 				UINT32 lLen --	to specify the length data to be read
* Output:		UINT8 --	return the result;
* Description:	
* Date:			2013.01.29
* ModifyRecord:
* *************************************************************************/
UINT8 EFC_ReadInfo(UINT8 *pDataBuf, UINT32 lAddress, UINT32 lLen)
{ 
	VUINT8* pAddr;
	pAddr=(VUINT8*)(lAddress+FLASH_BASE_ADDR);
	memcpy(pDataBuf,(void*)pAddr,lLen);
	return RT_OK;
}


/***************************************************************************
* Subroutine:	EFC_OtpWrite
* Function:		to write OTP user field.
* Input:		UINT32 nAddress	-- 	to specify the start address of the OTP user field
* 									from 0 to 240;
* 				UINT32 nNum --	the number of data to be written, size of word;
* 				UINT32* pBuf -- to specify the data 
* Output:		UINT8 --	return the result;
* Description:
* Date:			2013.01.29
* ModifyRecord:
* *************************************************************************/
UINT8 EFC_OtpWrite(UINT32 nAddress, UINT32 nNum, UINT32* pBuf)
{
	if(nAddress>240) return RT_FAIL;
	if((nAddress+nNum)>241) return RT_FAIL;

	EFC_WrProtect(AREA_OTP_USER_WR);

	while(nNum--)
	{
	   EfcRegs.WREN.all = 0x80000000 + nAddress*4 + 0x40430;
	   EFC_Program(pBuf, nAddress*4+0x40430, 1);
	   nAddress++;
	   pBuf++;         
	}	
	EFC_WrProtect(AREA_PROT_DI);
	return RT_OK;
}

/***************************************************************************
* Subroutine:	EFC_OtpRead
* Function:		to read OTP user field.
* Input:		UINT32 nAddress	-- 	to specify the start address of the OTP user field
* 									from 0 to 240;
* 				UINT32 nNum --	the number of data to be read;
* 				UINT8* pBuf -- to return the data
* Output:		UINT8 --	return the result;
* Description:
* Date:			2013.01.29
* ModifyRecord:
* *************************************************************************/
UINT8 EFC_OtpRead(UINT32 nAddress, UINT32 nNum, UINT32 *pBuf)
{
	if(nAddress>240) return RT_FAIL;
	if((nAddress+nNum)>241) return RT_FAIL;

	memcpy((void *)pBuf, (void *)(nAddress*4+FLASH_BASE_ADDR+0x40430), nNum*4);
	return RT_OK;
}

/***************************************************************************
* Subroutine:	EFC_ReadChipSN
* Function:		to read OTP chip Serial Number.
* Input:		UINT8 *pSN	-- 	to return SN data;
* 				UINT16* pLen -- to return the length of SN, unit in byte
* Output:		None
* Description:	if pSN is NULL, only the length of SN will return by pLen
* Date:			2013.01.29
* ModifyRecord:
* *************************************************************************/
void EFC_ReadChipSN(UINT8 *pSN, UINT16* pLen)
{
	if(pSN == NULL)
	{
		*pLen = 0x20;
		return;
	}
	memcpy((void *)pSN, (void *)(0x10040400), 0x20);
	*pLen = 0x20;
}

/***************************************************************************
* Subroutine:	EFC_LockJtag
* Function:		to lock JTAG
* Input:		None
* Output:		UINT8 --	return the result;
* Description:	
* Date:			2013.01.29
* ModifyRecord: 
* *************************************************************************/
UINT8 EFC_LockJtag(void)
{
	UINT32 nAddr = 0x407FC;
	UINT32 lock = 0;
	
    if(*(UINT32 *)(FLASH_BASE_ADDR+nAddr) == 0xFFFFFFFF)
	{
		EFC_WrProtect(AREA_OTP_JTAG_WR);
		EfcRegs.WREN.all = 0x80000000 | nAddr;
		EFC_Program((UINT32 *)&lock, nAddr, 1);
		EFC_WrProtect(AREA_PROT_DI);
		if(lock == *(UINT32 *)(FLASH_BASE_ADDR+nAddr)) return RT_OK;
		else return RT_FAIL;
	}
	else return RT_FAIL;
}

/***************************************************************************
* Subroutine:	EFC_SetEoSt
* Function:		to set execute only area start page
* Input:		UINT32 nPage  -- Page number;
* Output:		UINT8 --	return the result;
* Description:
* Date:			2012.10.17
* ModifyRecord:
* *************************************************************************/
UINT8 EFC_SetEoSt(UINT32 nPage)
{
	UINT32 nEoStConfigAddr = 0x407F8;

	if(*(UINT32 *)(FLASH_BASE_ADDR+nEoStConfigAddr) == 0xFFFFFFFF)
	{
		nPage *= FLASH_PAGE_SIZE;
		nPage += FLASH_BASE_ADDR;
		EFC_WrProtect(AREA_OTP_EO_ST);

		EfcRegs.WREN.all = 0x80000000 | nEoStConfigAddr;
		EFC_Program((UINT32 *)&nPage, nEoStConfigAddr, 1);
		EFC_WrProtect(AREA_PROT_DI);
		if(nPage == *(UINT32 *)(FLASH_BASE_ADDR+nEoStConfigAddr)) return RT_OK;
		else return RT_FAIL;
	}
	else return RT_FAIL;
}

/***********************************************************************************
 * Subroutine:	EFC_ClrProgFlag
 * Function:	Clear SY03 code programmed flag and parameter page
 * Input:		UINT32 Key -- 	AREA_PARA_WE(0x4444_5678);
 * Output:		None
 * Description: It is recommended the parameter Key is get from USB, UART or
 * 				other communication interface for parameter security.
 * Date:		2012.11.05
 * ModifyRecord:
 * ********************************************************************************/
void EFC_ClrProgFlag(UINT32 Key)
{
	EFC_WrProtect(Key); 
	EFC_Erase(256*FLASH_PAGE_SIZE, 1);
	EFC_WrProtect(AREA_PROT_DI);
}

/***************************************************************************
* Subroutine:	EFC_LockTest
* Function:		to lock test
* Input:		None
* Output:		UINT8 --	return the result;
* Description:
* Date:			2013.01.18
* ModifyRecord:
* *************************************************************************/
UINT8 EFC_LockTest(void)
{
	UINT32 nAddr = 0x407F4;
	UINT32 nVal = 0x20121023;

	if(*(UINT32 *)(FLASH_BASE_ADDR+nAddr) == 0xFFFFFFFF)
	{
		EFC_WrProtect(AREA_OTP_TEST_WR);
		EfcRegs.WREN.all = 0x80000000 | nAddr;
		EFC_Program((UINT32 *)&nVal, nAddr, 1);
		if(nVal == *(UINT32 *)(FLASH_BASE_ADDR+nAddr)) return RT_OK;
		else return RT_FAIL;
	}
	else return RT_FAIL;
}

