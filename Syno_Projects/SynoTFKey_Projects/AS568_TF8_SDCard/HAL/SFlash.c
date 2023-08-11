 /**********************************************************************************
* File name    :	SFlash.c
* Function     :	Safe flash operate.
* Author       : 	Howard
* Date         :	2013/03/14
* Version      :    v1.0
* Description  :
* The structure of Save Flash Area:
*
*   |<------ SF_Init:	nEndBlock - nStartBlock	 ------>|
*									|<-- BK_PAGE_NUM -->|
* 	-----------------------------------------------------
*	|*********���ݴ洢��************|***д����������****|
* 	-------------------------------------------------------------------------
*  /|\							   /|\				   /|\
*	|  								|					|
* gSF_Start_Block				   gSF_End_Block	  nStartBlock
*								   BK_START_PAGE
* ModifyRecord :
* 1. ��STRUCT_BK_STATUS�ṹ���ԱlStatus�������һ���������д״̬ҳʱ������ڵı�־
*    �Ѿ�д�룬����������δ��ȷд���������	2013.04.08	Coins
* 2. �޸�д�����ݷ�ΧԽ��ıȽϷ�ʽ����>=�Ƚϸ�Ϊ>�Ƚϣ����д���ݴ洢����
*	 ���һҳʱ���ز��������bug��	2013/05/13	coins
* 3. ����ǰ�����ж��Ƿ��Ѿ��������Լ��ٶ��Ѳ���ҳ��д��ʱ�䡣2013/05/13	coins 
* 4. �޸ı��ݷ�ʽ����ԭ������״̬����־������ÿ������ҳ�У���дһ�εĲ���������ԭ����3��
*	 ��Ϊ2�Σ�ʱ����89ms��Ϊ69ms��	2013/05/14	coins
* 5. �޸ĺ���SF_WriteData�������д�����ݳ���С�ڵ�ҳƫ�ƺ󲿷ֵĳ���ʱ��
*	 д���������RT_PARAM_ERR��bug�� 2013/05/15	coins
* 6. �޸ı��ݻָ�bug, ���ݻָ�ʱ��ԭ���ķ�ʽ��ÿҳ������Ƿ���Ҫ�ָ����ݣ�
*	 �������ܴ������⣬��������д����ڲ�������ҳʱ���磬����ֻ�����˱��ݵ���ʼҳ��
*	 �����ϵ�ָ�ʱ����ָ�����ı���ҳ������д��������ݱ������ݸ��ǡ�
*	 �����޸ĳɰ�����ָ����ָ������ݱ�־��ЧҳΪֹ���Ժ���ҳ�����ټ�顣
*	 2015/01/16	coins
**********************************************************************************/
/**********************************************************************************
* Include Header Files
**********************************************************************************/
#include "SY03.h" 
/**********************************************************************************
* Local Macro Definition
**********************************************************************************/ 
#define BK_PAGE_NUM   	3 //������ҳ�� 
#define	BK_START_PAGE	(gSF_End_Block)	// ��������ʼҳ���Ǵ洢������ҳ
#define	BK_STATUS_ADDR	(FLASH_PAGE_SIZE-sizeof(STRUCT_BK_STATUS))	// ״̬��ŵ�ַ 
#define STATUS_BK 		0x9876 		//˵���Ǳ���״̬. 
#define	SF_PAGE_SIZE	BK_STATUS_ADDR		// ��ȫ�洢����ҳ��С��1020�ֽڣ�

/**********************************************************************************
* Local Type Definition
**********************************************************************************/
typedef struct { 	// 2013.04.08	Coins
    UINT16 lStartPage;	//��������ҳ��ַ
	UINT16 lStatus;		//״̬ 
}STRUCT_BK_STATUS; 	

/**********************************************************************************
* Local static Variable Declaration
**********************************************************************************/
static UINT16 gSF_Start_Block, gSF_End_Block;

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
* Subroutine:	SF_IsBufAllOne
* Function:		Check buffer all 1 
* Input:	 	void *pData -- the buffer
*				UINT32 nSize -- the buffer size (word number)
* Output:		RT_OK
* Description:	 
* Date:			2013.05.14
* ModifyRecord:
* *************************************************************************/
static UINT8 SF_IsBufAllOne(void *pData, UINT32 nSize)
{
	UINT32 i;
	UINT32 *p = pData;
	for(i=0;i<nSize;i++) 
	{
		if(p[i] != 0xFFFFFFFF) return FALSE;
	}
	return TRUE;
}

/***************************************************************************
* Subroutine:	SF_BackupPage
* Function:		Move several pages from data area to backup area.
* Input:	 	UINT32 *lSrcPage -- the source flash page number;
* 				UINT32 lDstPage --	the destination flash page number;
* 				UINT32 lPageNum  --	the numbers to move;
*				UINT32 *pPageBuf -- to specify a page buffer (FLASH_PAGE_SIZE size).
* Output:		RT_OK.
* Description:	the destination pages and source pages must be not overlapped.	 
* Date:			2013.05.14
* ModifyRecord: 
* *************************************************************************/
static UINT8 SF_BackupPage(UINT32 lSrcPage, UINT32 lDstPage, UINT32 lPageNum, UINT32 *pPageBuf)
{
	UINT8 i;
	STRUCT_BK_STATUS tBk;
	for(i=0;i<lPageNum;i++)
	{
		EFC_ReadInfo((UINT8 *)pPageBuf,(lSrcPage+i)*FLASH_PAGE_SIZE, SF_PAGE_SIZE);	
		// �ж��Ƿ��Ѳ��� 2013.05.13 coins
		//if(!SF_IsBufAllOne((void *)((lDstPage+i)*FLASH_PAGE_SIZE+FLASH_BASE_ADDR), FLASH_PAGE_SIZE/4))
		if(!EFC_IsEraseOK((lDstPage+i), (lDstPage+i)))			
			EFC_Erase((lDstPage+i)*FLASH_PAGE_SIZE,1);
		tBk.lStartPage = lSrcPage+i;
		tBk.lStatus = STATUS_BK; 
		memcpy((UINT8 *)pPageBuf+BK_STATUS_ADDR, (UINT8 *)&tBk, sizeof(STRUCT_BK_STATUS));
		EFC_PageProg((lDstPage+i)*FLASH_PAGE_SIZE, pPageBuf);	
	}	
	return RT_OK;
}

/***************************************************************************
* Subroutine:	SF_RecoverPage
* Function:		Move several pages from backup area to data area.
* Input:	 	UINT32 *lSrcPage -- the source flash page number;
* 				UINT32 lDstPage --	the destination flash page number;
* 				UINT32 lPageNum  --	the numbers to move;
*				UINT32 *pPageBuf -- to specify a page buffer (FLASH_PAGE_SIZE size).
* Output:		RT_OK.
* Description:	the destination pages and source pages must be not overlapped.	 
* Date:			2013.05.14
* ModifyRecord: 
* *************************************************************************/
static UINT8 SF_RecoverPage(UINT32 lSrcPage, UINT32 lDstPage, UINT32 lPageNum, UINT32 *pPageBuf)
{
	UINT8 i;
	for(i=0;i<lPageNum;i++)
	{
		EFC_ReadInfo((UINT8 *)pPageBuf,(lSrcPage+i)*FLASH_PAGE_SIZE, SF_PAGE_SIZE);	
		// �ж��Ƿ��Ѳ��� 2013.05.13 coins
		//if(!SF_IsBufAllOne((void *)((lDstPage+i)*FLASH_PAGE_SIZE+FLASH_BASE_ADDR), FLASH_PAGE_SIZE/4))
		if(!EFC_IsEraseOK((lDstPage+i), (lDstPage+i)))		
			EFC_Erase((lDstPage+i)*FLASH_PAGE_SIZE,1);
		memset((UINT8 *)pPageBuf+BK_STATUS_ADDR, 0xFF, sizeof(STRUCT_BK_STATUS));
		EFC_PageProg((lDstPage+i)*FLASH_PAGE_SIZE, pPageBuf);	
	}	
	return RT_OK;
}


/***************************************************************************
* Subroutine:	SF_CheckBKStatus
* Function:		Check Save Flash data in backup status. when in back up, to recover data. 
* Input:	 	None
* Output:		RT_OK
* Description:	 
* Date:			2013.03.14
* ModifyRecord:
*	1.�޸ı��ݻָ�bug, ���ݻָ�ʱ��ԭ���ķ�ʽ��ÿҳ������Ƿ���Ҫ�ָ����ݣ�
*	�������ܴ������⣬��������д����ڲ�������ҳʱ���磬����ֻ�����˱��ݵ���ʼҳ��
*	�����ϵ�ָ�ʱ����ָ�����ı���ҳ������д��������ݱ������ݸ��ǡ�
*	�����޸ĳɰ�����ָ����ָ������ݱ�־��ЧҳΪֹ���Ժ���ҳ�����ټ�顣
*	2015/01/16	Coins
* *************************************************************************/
static UINT8 SF_CheckBKStatus(void)
{
	UINT32 Buf[FLASH_PAGE_SIZE/sizeof(UINT32)];
	STRUCT_BK_STATUS tBK; 
	UINT32 i;
    
	for(i=0;i<BK_PAGE_NUM;i++)	// check all backup page 
	{
		// to read back up status from current backup page
		EFC_ReadInfo((UINT8*)&tBK,(BK_START_PAGE+i)*FLASH_PAGE_SIZE+BK_STATUS_ADDR,sizeof(tBK));	
		if(tBK.lStatus==STATUS_BK)	// the page have been backup, need to recover
		{
	        EFC_WrProtect(AREA_DATAPRO_WE);   
			SF_RecoverPage(BK_START_PAGE+i,tBK.lStartPage,1, Buf);	// copy back from backup pages	 			
			EFC_WrProtect(AREA_PROT_DI);	      	
		}
		else
		{
			break;	
		}		
	}
	for(i=0;i<BK_PAGE_NUM;i++)		// erase all backup page
	{	
		if(!EFC_IsEraseOK((BK_START_PAGE+i), (BK_START_PAGE+i)))
		{
			EFC_WrProtect(AREA_DATAPRO_WE);
			EFC_Erase((BK_START_PAGE+i)*FLASH_PAGE_SIZE,1);	// erase current backup page
			EFC_WrProtect(AREA_PROT_DI);
		}		
	}
	return RT_OK;
}


 

/***************************************************************************
* Subroutine:	SF_WriteBlocks
* Function:		Multi-page save write, the page number should be less than
*				or equal to BK_PAGE_NUM. 
* Input:	 	UINT8 *pBuf -- the data to be written;
*             	UINT32 lStartBlock -- the start page;
*             	UINT32 lOffset -- the offset in the page; 
*				UINT32 lLen -- the length to be written.
* Output:		RT_OK or RT_PARAM_ERR;
* Description:	 
* Date:			2013.03.14
* ModifyRecord:	
*	1. �޸�д�����ݷ�ΧԽ��ıȽϷ�ʽ����>=�Ƚϸ�Ϊ>�Ƚϣ����д���ݴ洢����
*	���һҳʱ���ز��������bug��	2013/05/13	coins
* *************************************************************************/
UINT8 SF_WriteBlocks(UINT8 *pBuf, UINT32 lStartBlock, UINT32 lOffset, UINT32 lLen)
{
	UINT32 j;
	UINT8 *pAddr;
	UINT8 Buf[FLASH_PAGE_SIZE];
	//UINT8 bMustErase;
	UINT32 lStartAddr,lBlockNum,lWriteLen;
  
 	lStartAddr=lStartBlock*FLASH_PAGE_SIZE;
 	
 	//����Ҫ���ݵ�ҳ�� 
 	if(lOffset==0)
 	   lBlockNum=(lLen+SF_PAGE_SIZE-1)/SF_PAGE_SIZE;
 	else
 	   lBlockNum=(lOffset+SF_PAGE_SIZE-1)/SF_PAGE_SIZE+(lLen-(SF_PAGE_SIZE-lOffset)+SF_PAGE_SIZE-1)/SF_PAGE_SIZE;
 	
 	if(lBlockNum>BK_PAGE_NUM)//����������ܳ�����������ҳ�� 
 	    return RT_PARAM_ERR;
 	// 2013/05/13 coins �޸�bug����>=�Ƚϸ�Ϊ>�Ƚϣ�����д���ݴ洢�������һҳʱ���ز�������    
    if((lStartBlock+lBlockNum)>gSF_End_Block)	// ���ܳ�����ȫ�洢��.
 	    return RT_PARAM_ERR; 	
 	
 	//1---�����ݰ��Ƶ��������������ñ��ݱ�־
     SF_BackupPage(lStartBlock,BK_START_PAGE,lBlockNum, (UINT32 *)Buf) ;
      
    //2---д������  
    pAddr=pBuf;
    for(j=0;j<lBlockNum;j++)
    {       	 
		EFC_ReadInfo(Buf, lStartAddr, FLASH_PAGE_SIZE);// modify SF_PAGE_SIZE to FLASH_PAGE_SIZE, 2015/01/16, coins 
		//erase the block
		// �ж��Ƿ��Ѳ��� 2013.05.13 coins
		//if(!SF_IsBufAllOne((void *)Buf, SF_PAGE_SIZE/4))
		if(!EFC_IsEraseOK(lStartAddr/FLASH_PAGE_SIZE, lStartAddr/FLASH_PAGE_SIZE))// modify SF_PAGE_SIZE to FLASH_PAGE_SIZE, 2015/01/16, coins
        	EFC_Erase(lStartAddr,1);
    	// copy data to write buffer
    	if((lLen+lOffset)>SF_PAGE_SIZE)
    	     lWriteLen=SF_PAGE_SIZE-lOffset;    	    
    	else
    	     lWriteLen=lLen;
    	     
        memcpy((Buf+lOffset), pAddr, lWriteLen); 
    
        //write to flash
        EFC_PageProg(lStartAddr, (UINT32 *)Buf);
        
		// next page
        lStartAddr+=FLASH_PAGE_SIZE;
        pAddr+=lWriteLen;
        lOffset=0;
        lLen-=lWriteLen;
    }

    // 3--��������ҳ 
	EFC_Erase(BK_START_PAGE*FLASH_PAGE_SIZE,lBlockNum);	
  
    return RT_OK;
}
 
/***************************************************************************
* Subroutine:	SF_Init
* Function:		Init Safe flash block
* Input:		UINT16 nStartBlock --	the start block(page).
                UINT16 nEndBlock --	the end block(page).
* Output:		RT_OK or RT_PARAM_ERR;
* Description:	 
* Date:			2013.03.14
* ModifyRecord:
* *************************************************************************/
UINT8 SF_Init(UINT16 nStartBlock, UINT16 nEndBlock)
{
	if(nEndBlock>256) return RT_PARAM_ERR;
	if(nEndBlock<nStartBlock) return RT_PARAM_ERR; 
    if((nEndBlock-nStartBlock)>192 || (nEndBlock-nStartBlock)<=(2*BK_PAGE_NUM))
    	return RT_PARAM_ERR; 
     
    gSF_Start_Block=nStartBlock;
    gSF_End_Block=nEndBlock-BK_PAGE_NUM;   
 	// ��ȫFlash�洢������λ��MPU�ı����������ռ�
	if(EfcRegs.DATAPRO>FLASH_BASE_ADDR+nStartBlock*FLASH_PAGE_SIZE) 
 		EfcRegs.DATAPRO=FLASH_BASE_ADDR+nStartBlock*FLASH_PAGE_SIZE;

    return SF_CheckBKStatus();    
}
 
/***************************************************************************
* Subroutine:	SF_WriteData
* Function:		write data to save flash area.
* Input:		UINT8* pDataBuf --	the data to be written save flash area;
*				UINT32 nOffset --	the offset address in save flash area;
*				UINT16 nDataNum --	the length to be written.
* Output:		RT_OK or RT_PARAM_ERR;
* Description:	 
* Date:			2013.03.14
* ModifyRecord:
* 1. �޸ĵ�д�����ݳ���С�ڵ�ҳƫ�ƺ󲿷ֵĳ���ʱ��д���������RT_PARAM_ERR��bug��	2013/05/15	coins
* *************************************************************************/
UINT8 SF_WriteData(UINT8* pDataBuf, UINT32 nOffset, UINT16 nDataNum)
{
 
	UINT32 lWriteLen;
	UINT16 nNum;
	UINT16 nStartBlock,nStartOffset;

	UINT8 *pBuf;
	UINT8 nRet;
	
	//check the data range;
	if(nDataNum<=0)
		return RT_PARAM_ERR;

	pBuf=pDataBuf;
	nNum=nDataNum;
	
	EFC_WrProtect(AREA_DATAPRO_WE);
	//loop to write data to flash
	do{
		nStartOffset=(UINT16)(nOffset%SF_PAGE_SIZE);
		nStartBlock=(UINT16)(gSF_Start_Block+nOffset/SF_PAGE_SIZE);
		 	
    	if(nNum>(BK_PAGE_NUM*SF_PAGE_SIZE-nStartOffset)) 	// 201/05/15	coins
    	{
    		lWriteLen=BK_PAGE_NUM*SF_PAGE_SIZE-nStartOffset;
    	}
        else
           	lWriteLen=nNum;
 
        nRet=SF_WriteBlocks(pBuf,nStartBlock,nStartOffset,lWriteLen);    	
 
		if(nRet!=RT_OK) 
		{
			EFC_WrProtect(AREA_PROT_DI);     
            return nRet;
		}        

		pBuf+=lWriteLen;
		nOffset+=lWriteLen;
		nNum-=lWriteLen;
		   
	}while(nNum>0);
	
	EFC_WrProtect(AREA_PROT_DI);     
	return RT_OK;
}
 
/***************************************************************************
* Subroutine:	SF_ReadData
* Function:		read data from save flash area
* Input:		UINT8* pDataBuf -- 	the buffer to save data for read;
*				UINT32 nOffset --	the offset address in save flash area;
*				UINT16 nDataNum --	the length to be written.
* Output:		RT_OK;
* Description:	 
* Date:			2013.03.14
* ModifyRecord:
* *************************************************************************/
UINT8 SF_ReadData(UINT8* pDataBuf, UINT32 nOffset, UINT16 nDataNum)
{
	VUINT8* pAddr;
	UINT16 nStartBlock,nStartOffset; 

	if((nOffset+nDataNum)>(gSF_End_Block-gSF_Start_Block)*SF_PAGE_SIZE) return RT_PARAM_ERR;

	while(nDataNum)
	{
		nStartOffset=(UINT16)(nOffset%SF_PAGE_SIZE);
		nStartBlock=(UINT16)(gSF_Start_Block+nOffset/SF_PAGE_SIZE);
		pAddr = (VUINT8*)FLASH_BASE_ADDR+nStartBlock*FLASH_PAGE_SIZE+nStartOffset;
		if(nDataNum>SF_PAGE_SIZE-nStartOffset)
		{
			memcpy(pDataBuf,(void*)pAddr,SF_PAGE_SIZE-nStartOffset);
			nDataNum -= SF_PAGE_SIZE-nStartOffset;
			nOffset += SF_PAGE_SIZE-nStartOffset;
			pDataBuf += SF_PAGE_SIZE-nStartOffset;	
		}
		else
		{
			memcpy(pDataBuf,(void*)pAddr,nDataNum);
			nDataNum = 0;
		}
	}	
	return RT_OK;
}
