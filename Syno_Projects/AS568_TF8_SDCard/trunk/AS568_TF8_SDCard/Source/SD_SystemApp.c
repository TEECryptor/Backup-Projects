/**********************************************************************************
* File name    :	SD_SystemApp.c
* Function     :	The SD card System Application 
* Author       : 	coins
* Date         :	2014/01/25
* Version      :    v1.0
* Description  :	
* ModifyRecord :
**********************************************************************************/

/**********************************************************************************
* Include Header Files
**********************************************************************************/
#include "SY03.h"
#include "SdMemAcc.h"   
#include "SD_SystemApp.h"  
#include "ff.h"
#include "log.h"

/**********************************************************************************
* Local Macro Definition
**********************************************************************************/

/**********************************************************************************
* Local Type Definition
**********************************************************************************/

/**********************************************************************************
* Local static Variable Declaration
**********************************************************************************/
static UINT32 gCmdAddr = 0;
static UINT8 gCmd = 0;
SDSI_FILE SdsiFile;
//static const char mTFSystemFiles[FILE_NUM][16] = {"command.sys", "datadec0.sys", "dataenc0.sys"};
static const char mTFSystemFile[16] = "command.sys";
UINT32 gSDICapacity = 0;
UINT8 gSDISysMode = SD_NULL_SYSTEM;

#if((SD_SYSTEM_MODE == SD_VIRTUAL_SYSTEM)||(SD_SYSTEM_MODE == SD_EFC_FLASH_SYSTEM))
static SDSI_DEV SdsiDev = 
{
	SD_PHY_V2_00,//SD_PHY_V2_00,//,//SD_PHY_V1_10,// V2.00,SD_PHY_V1_00
#if(SD_SYSTEM_MODE == SD_VIRTUAL_SYSTEM)	
#ifdef USE_FAT32
    0x766800,
#else  
	0x88,//0x762800,// 0x766800,//0x762800,//0x200000,      //SDSC,SDHC
#endif  // USE_FAT32
#else
	0x88,//0x762800,// 0x766800,//0x762800,//0x200000,      //SDSC,SDHC
#endif
	NULL,
	NULL,
	SD_Init,
	SD_Read,
	SD_Write,
	SD_MsgProcess,
	SD_Erase,
	SD_FileInit,	
};
#endif
#if((SD_SYSTEM_MODE == SD_SQI_FLASH_SYSTEM)||(SD_SYSTEM_MODE == SD_TF_SYSTEM)||(SD_SYSTEM_MODE == SD_MULT_SYSTEM))
static SDSI_DEV SdsiDev = 
{
	SD_PHY_V2_00,//,//SD_PHY_V1_10,// V2.00,SD_PHY_V1_00	  
	0x766800,// 0x766800,//0x762800,//0x200000,      //SDSC,SDHC
	NULL,
	NULL,
	SD_Init,
	SD_Read,
	SD_Write,
	SD_MsgProcess,
	SD_Erase,
	SD_FileInit,	
};
#endif
/**********************************************************************************
* Global Variable Declaration
**********************************************************************************/

/**********************************************************************************
* Local Functon Declaration
**********************************************************************************/
static BOOL SD_ReadDataFromMemory(UINT32 nAddr, UINT8 nBuffBank, UINT32 nSize); 
static BOOL SD_WriteDataToMemory(UINT32 nAddr, UINT8 nBuffBank, UINT32 nSize);


/**********************************************************************************
* Functon
**********************************************************************************/
/***************************************************************************
* Subroutine:	SD_FileInit
* Function:		to initialize our SD FILE.
* Input:		None
* Output:		None
* Description:	
* Date:			2014/01/21
* ModifyRecord:
* *************************************************************************/
void SD_FileInit(void)
{      
    UINT32 nSysMode = 0;

    DBG_FUNC_BEGIN;
#if(SD_SYSTEM_MODE == SD_MULT_SYSTEM) 
    nSysMode = gSDISysMode;
#else                           
    nSysMode = SD_SYSTEM_MODE;
#endif     
    if ((nSysMode == SD_VIRTUAL_SYSTEM) || (nSysMode == SD_EFC_FLASH_SYSTEM))
    {    
    	SdsiFile.nFileNum = FILE_NUM;
    	SdsiFile.nFileAddr[CMD_FILE][0] = ADDR_FILE_COMM;
    	SdsiFile.nFileAddr[CMD_FILE][1] = ADDR_FILE_DEC0 - 1;
    	SdsiFile.nFileAddr[DEC0_FILE][0] = ADDR_FILE_DEC0;
    	SdsiFile.nFileAddr[DEC0_FILE][1] = ADDR_FILE_ENC0 - 1;
    	SdsiFile.nFileAddr[ENC0_FILE][0] = ADDR_FILE_ENC0;
    	SdsiFile.nFileAddr[ENC0_FILE][1] = ADDR_FILE_ENC0 + SD_DATFILE_SIZE/512 - 1;
    }                                                                              
    else if ((nSysMode == SD_TF_SYSTEM)||(nSysMode == SD_SQI_FLASH_SYSTEM))
    {
/*   Youfang 2015-09-07     
		UINT32 addrs[FILE_NUM] = {0};

        if(TF_GetFilesAddr("0:", mTFSystemFiles, addrs) == 0)//文件存在或创建成功
		{ 
	    	SdsiFile.nFileNum = FILE_NUM;
	    	SdsiFile.nFileAddr[CMD_FILE][0] = addrs[0];
	    	SdsiFile.nFileAddr[CMD_FILE][1] = addrs[1] - 1;
	    	SdsiFile.nFileAddr[DEC0_FILE][0] = addrs[1];
	    	SdsiFile.nFileAddr[DEC0_FILE][1] = addrs[2] - 1;
	    	SdsiFile.nFileAddr[ENC0_FILE][0] = addrs[2];
	    	SdsiFile.nFileAddr[ENC0_FILE][1] = addrs[2] + SD_DATFILE_SIZE/512 - 1;
		}
*/ 
		UINT32 addr = 0;
		
		// 获取系统文件地址,如不存在则创建
		if (TF_GetSysFileAddr("0:", mTFSystemFile, SD_CMDFILE_SIZE + 2*SD_DATFILE_SIZE, &addr) == 0)
		{
	    	SdsiFile.nFileNum = FILE_NUM;
	    	SdsiFile.nFileAddr[CMD_FILE][0] = addr;
	    	SdsiFile.nFileAddr[CMD_FILE][1] = (addr + SD_CMDFILE_SIZE/SDSI_BUFF_BANK_SIZE) - 1;
	    	SdsiFile.nFileAddr[DEC0_FILE][0] = (addr + SD_CMDFILE_SIZE/SDSI_BUFF_BANK_SIZE);
	    	SdsiFile.nFileAddr[DEC0_FILE][1] = (addr + SD_CMDFILE_SIZE/SDSI_BUFF_BANK_SIZE) + SD_DATFILE_SIZE/SDSI_BUFF_BANK_SIZE - 1;
	    	SdsiFile.nFileAddr[ENC0_FILE][0] = (addr + SD_CMDFILE_SIZE/SDSI_BUFF_BANK_SIZE) + SD_DATFILE_SIZE/SDSI_BUFF_BANK_SIZE;
	    	SdsiFile.nFileAddr[ENC0_FILE][1] = (addr + SD_CMDFILE_SIZE/SDSI_BUFF_BANK_SIZE) + 2*SD_DATFILE_SIZE/SDSI_BUFF_BANK_SIZE - 1;
		}
		else //失败，只是普通TF卡
		{
	    	SdsiFile.nFileNum = FILE_NUM;
	    	SdsiFile.nFileAddr[CMD_FILE][0] = 0x7ffffff0;
	    	SdsiFile.nFileAddr[CMD_FILE][1] = 0x7ffffff1;
	    	SdsiFile.nFileAddr[DEC0_FILE][0] = 0x7ffffff2;
	    	SdsiFile.nFileAddr[DEC0_FILE][1] = 0x7ffffff3;
	    	SdsiFile.nFileAddr[ENC0_FILE][0] = 0x7ffffff4;
	    	SdsiFile.nFileAddr[ENC0_FILE][1] = 0x7ffffff5;
		}

        if (gSDICapacity > 0)
        {
            SdsiDev.nCapacity = gSDICapacity; 
        }   
    }
    else if(nSysMode == SD_NULL_SYSTEM)   //SD_FLASH_SYSTEM
    {
        //TBD:
    }
	else
	{
	}          

    printf("File addr:0x%x, 0x%x, 0x%x\n", SdsiFile.nFileAddr[CMD_FILE][0], SdsiFile.nFileAddr[DEC0_FILE][0], SdsiFile.nFileAddr[ENC0_FILE][0]);
    printf("File addr:0x%x, 0x%x, 0x%x\n", SdsiFile.nFileAddr[CMD_FILE][1], SdsiFile.nFileAddr[DEC0_FILE][1], SdsiFile.nFileAddr[ENC0_FILE][1]);
    
    DBG_FUNC_END;
}
/***************************************************************************
* Subroutine:	SD_Init
* Function:		to initialize SD.
* Input:		None
* Output:		None
* Description:	
* Date:			2014/08/19
* ModifyRecord:
* *************************************************************************/
UINT8 SD_Init(void *p)
{                
    DBG_FUNC_BEGIN;

	SCM_EnableModClk(ID_SEA);
	SCM_ReleaseModRst(ID_SEA);

	if(gSDISysMode == SD_TF_SYSTEM)
	{
		if(SDI_Init(1)!= 0)
		  printf("SDI INIT ERR\n");
		gSDICapacity = gSdmcPara.Capacity*2024;
	}
	else if(gSDISysMode == SD_SQI_FLASH_SYSTEM)
	{
		if(SQI_Init(1, DEFAULT_RD_MODE) != 0)
		  printf("SQI INIT ERR\n");
		gSDICapacity = SQI_FLASH_CAP/512;
	}
	else
	{
		gSDICapacity = 0;
	}
    DBG_FUNC_END;
	return RT_OK;
}
/***************************************************************************
* Subroutine:	SD_MsgProcess
* Function:		interrupt message process function.
* Input:		nAddr:SDSI address, in blocks
*               nMsg:interrupt type
* Output:		None
* Description:	
* Date:			2014/08/19
* ModifyRecord:
* *************************************************************************/
UINT8 SD_MsgProcess(UINT32 nAddr, UINT8 nMsg)
{                   
//	DBG_INFO("nAddr < SdsiDev.nCapacity, nAddr = 0x%x, SdsiDev.nCapacity = 0x%x", nAddr, SdsiDev.nCapacity);
	if (nAddr < SdsiDev.nCapacity)
	{						
		if (nMsg & (SDSI_MSG_READ_STT | SDSI_MSG_WRITE_STT)) 
		{
			gCmd = 1;   
			gCmdAddr = nAddr;
			DBG_INFO("Message is Start!");
		}
		else if (nMsg & (SDSI_MSG_READ_END | SDSI_MSG_WRITE_END))
		{
			if ((gCmdAddr >= SdsiFile.nFileAddr[CMD_FILE][0]) && (gCmdAddr <= SdsiFile.nFileAddr[CMD_FILE][1]))
			{		
                if (nMsg & SDSI_MSG_WRITE_END)
                {       
                    //处理命令数据(数据从SDSI的起始地址偏移1k)    
                    SD_CmdProc(0, SDSI_BUFF_BANK(2), DIR_WR);
                }	
			}
            else if ((gCmdAddr >= SdsiFile.nFileAddr[DEC0_FILE][0]) && (gCmdAddr <= SdsiFile.nFileAddr[ENC0_FILE][1]))
            {
                //  Do nothing
            }
			else
			{
	    		#if(SD_SYSTEM_MODE == SD_TF_SYSTEM)
	    			if(gCmd==0)SDI_StopTransmission();
	    		#endif
	    		#if(SD_SYSTEM_MODE == SD_MULT_SYSTEM)
    				if(gSDISysMode == SD_TF_SYSTEM)
    				{
    	    			if(gCmd==0)SDI_StopTransmission();
    				}
	    		#endif
			}	
			DBG_INFO("Message is end!");
		}
	}
	else
	{	       
		if (nMsg & (SDSI_MSG_READ_STT | SDSI_MSG_WRITE_STT)) 
		{
			gCmd = 1;
			DBG_INFO("S->0x%x, ", nAddr);
		}
		else if (nMsg&(SDSI_MSG_READ_END | SDSI_MSG_WRITE_END))
		{
			if ((gCmdAddr >= SdsiFile.nFileAddr[CMD_FILE][0]) && (gCmdAddr <= SdsiFile.nFileAddr[ENC0_FILE][1]))
			{			
			}
			else
			{
	    		#if(SD_SYSTEM_MODE == SD_TF_SYSTEM)
	    			if(gCmd==0)SDI_StopTransmission();
	    		#endif
	    		#if(SD_SYSTEM_MODE == SD_MULT_SYSTEM)
    				if(gSDISysMode == SD_TF_SYSTEM)
    				{
    	    			if(gCmd==0)SDI_StopTransmission();
    				}
	    		#endif
			}
			DBG_INFO(("E\n"));
		}
	}
	return RT_OK;
}
/***************************************************************************
* Subroutine:	SD_Read
* Function:		read interrupt process function.
* Input:		nAddr:SDSI address, in blocks
*               nBuffBank:buffer bank index
*				nSize:blocks count
* Output:		None
* Description:	
* Date:			2014/08/19
* ModifyRecord:
* *************************************************************************/
UINT8 SD_Read(UINT32 nAddr, UINT8 nBuffBank, UINT32 nSize)
{                 
	DBG_INFO("gCmd = 0x%x, gCmdAddr = 0x%x, nAddr = 0x%x, nBuffBank = 0x%x, nSize = 0x%x",gCmd, gCmdAddr, nAddr, nBuffBank, nSize);
	if (nAddr < SdsiDev.nCapacity)
	{      
		if (gCmd)
		{
			if ((gCmdAddr >= SdsiFile.nFileAddr[CMD_FILE][0]) && (gCmdAddr <= SdsiFile.nFileAddr[ENC0_FILE][1]))
			{			
				if (gCmdAddr <= SdsiFile.nFileAddr[CMD_FILE][1]) 	// security response returns
				{		               
                    // 返回全部结果数据
					SD_CmdProc(0, SDSI_BUFF_BANK(nBuffBank), DIR_RD);
				}
				else
				{		                  
	                //  Do nothing
				}
			}
			else
			{                         
                //DBG_INFO("Read data from gCmdAddr = 0x%x", gCmdAddr);
                //SD_ReadDataFromMemory(nAddr, nBuffBank, nSize);
				#if(SD_SYSTEM_MODE == SD_VIRTUAL_SYSTEM)
				    SD_MemReadFromFlash(nAddr, SDSI_BUFF_BANK(nBuffBank));
				#endif
				#if(SD_SYSTEM_MODE == SD_EFC_FLASH_SYSTEM)
					DBG_INFO(("CMD->R->"));
		    		EFC_ReadInfo(SDSI_BUFF_BANK(nBuffBank), (0x30000+nAddr*0x200), nSize*0x200);
	    		#endif
				#if(SD_SYSTEM_MODE == SD_SQI_FLASH_SYSTEM)
					DBG_INFO(("CMD->R->"));
		    		SQI_ReadData(SDSI_BUFF_BANK(nBuffBank), (SQI_FLASH_ADDR_OFF+nAddr*0x200), nSize*0x200);
	    		#endif
				#if(SD_SYSTEM_MODE == SD_TF_SYSTEM)
					DBG_INFO(("CMD->R->"));
	    			SDI_ReadMultiBlockWithCmd(nAddr, nBuffBank, nSize);
	    		#endif
				#if(SD_SYSTEM_MODE == SD_MULT_SYSTEM)				
					DBG_INFO(("CMD->R->"));
					if(gSDISysMode == SD_TF_SYSTEM)
						SDI_ReadMultiBlockWithCmd(nAddr, nBuffBank, nSize);
					else if(gSDISysMode == SD_SQI_FLASH_SYSTEM)
						SQI_ReadData(SDSI_BUFF_BANK(nBuffBank), (SQI_FLASH_ADDR_OFF+nAddr*0x200), nSize*0x200);
					else if(gSDISysMode == SD_VIRTUAL_SYSTEM)
				    	SD_MemReadFromFlash(nAddr, SDSI_BUFF_BANK(nBuffBank));
					else if(gSDISysMode == SD_EFC_FLASH_SYSTEM)
				    	EFC_ReadInfo(SDSI_BUFF_BANK(nBuffBank), (0x30000+nAddr*0x200), nSize*0x200);
	    			else
					{
					}
	    		#endif
			}
			gCmd = 0;
		}
		else
		{ 
			if ((gCmdAddr >= SdsiFile.nFileAddr[CMD_FILE][0]) && (gCmdAddr <= SdsiFile.nFileAddr[ENC0_FILE][1]))
			{
				if (gCmdAddr <= SdsiFile.nFileAddr[CMD_FILE][1]) 	// security response returns
				{				       
	                //  Do nothing
				}
				else 	// data response up
				{			                         
	                //  Do nothing
				}
			}
			else
			{             
                //DBG_INFO("Read data from gCmdAddr = 0x%x", gCmdAddr);                
                //SD_ReadDataFromMemory(nAddr, nBuffBank, nSize);
				#if(SD_SYSTEM_MODE == SD_VIRTUAL_SYSTEM)
				    SD_MemReadFromFlash(nAddr, SDSI_BUFF_BANK(nBuffBank));
				#endif
				#if(SD_SYSTEM_MODE == SD_EFC_FLASH_SYSTEM)
					DBG_INFO(("R->"));
		    		EFC_ReadInfo(SDSI_BUFF_BANK(nBuffBank), (0x30000+nAddr*0x200), nSize*0x200);
	    		#endif
				#if(SD_SYSTEM_MODE == SD_SQI_FLASH_SYSTEM)
					DBG_INFO(("R->"));
		    		SQI_ReadData(SDSI_BUFF_BANK(nBuffBank), (SQI_FLASH_ADDR_OFF+nAddr*0x200), nSize*0x200);
	    		#endif
				#if(SD_SYSTEM_MODE == SD_TF_SYSTEM)
					//DBG_INFO(("R->"));
	    			SDI_ReadMultiBlockNoCmd(nBuffBank, nSize);
	    		#endif
				#if(SD_SYSTEM_MODE == SD_MULT_SYSTEM)				
					DBG_INFO(("R->"));
					if(gSDISysMode == SD_TF_SYSTEM)
						SDI_ReadMultiBlockNoCmd(nBuffBank, nSize);
					else if(gSDISysMode == SD_SQI_FLASH_SYSTEM)
						SQI_ReadData(SDSI_BUFF_BANK(nBuffBank), (SQI_FLASH_ADDR_OFF+nAddr*0x200), nSize*0x200);
					else if(gSDISysMode == SD_VIRTUAL_SYSTEM)
				    	SD_MemReadFromFlash(nAddr, SDSI_BUFF_BANK(nBuffBank));
					else if(gSDISysMode == SD_EFC_FLASH_SYSTEM)
				    	EFC_ReadInfo(SDSI_BUFF_BANK(nBuffBank), (0x30000+nAddr*0x200), nSize*0x200);
	    			else
					{
					}
	    		#endif
			}
		}
	}
	else
	{ 
        DBG_INFO("nAddr > SdsiDev.nCapacity!!!");  		
	}

	return RT_OK;
}
/***************************************************************************
* Subroutine:	SD_Write
* Function:		write interrupt process function.
* Input:		nAddr:SDSI address, in blocks
*               nBuffBank:buffer bank index
*				nSize:blocks count
* Output:		None
* Description:	
* Date:			2014/08/19
* ModifyRecord:
* *************************************************************************/
UINT8 SD_Write(UINT32 nAddr, UINT8 nBuffBank, UINT32 nSize)
{			
	DBG_INFO("gCmd = 0x%x, gCmdAddr = 0x%x, nAddr = 0x%x, nBuffBank = 0x%x, nSize = 0x%x",gCmd, gCmdAddr, nAddr, nBuffBank, nSize);
	if (nAddr < SdsiDev.nCapacity)
	{         
        //DBG_INFO("gCmd = 0x%x", gCmd);
		if (gCmd)
		{
			if ((gCmdAddr >= SdsiFile.nFileAddr[CMD_FILE][0]) && (gCmdAddr <= SdsiFile.nFileAddr[ENC0_FILE][1]))
			{
				if (gCmdAddr <= SdsiFile.nFileAddr[CMD_FILE][1]) 	// security response returns
				{                     
	                //  Do nothing
				}
				else 	// data down
				{
                    SD_FlowProc(nAddr, nBuffBank, nSize, DIR_WR);//
				}
			}
			else
			{                            
                //DBG_INFO("Write data to gCmdAddr = 0x%x", gCmdAddr);                
                //SD_WriteDataToMemory(nAddr, nBuffBank, nSize);
				#if(SD_SYSTEM_MODE == SD_VIRTUAL_SYSTEM)
				    SD_MemWriteToFlash(nAddr, SDSI_BUFF_BANK(nBuffBank));
				#endif
				#if(SD_SYSTEM_MODE == SD_EFC_FLASH_SYSTEM)
					DBG_INFO(("CMD->W->"));
					EFC_WrProtect(AREA_DATAPRO_WE);
					EFC_WriteInfo(SDSI_BUFF_BANK(nBuffBank), (0x30000+nAddr*0x200), nSize*0x200);
					EFC_WrProtect(AREA_PROT_DI);
	    		#endif
				#if(SD_SYSTEM_MODE == SD_SQI_FLASH_SYSTEM)
					DBG_INFO(("CMD->W->"));
		    		SQI_WriteData(SDSI_BUFF_BANK(nBuffBank), (SQI_FLASH_ADDR_OFF+nAddr*0x200), nSize*0x200);
	    		#endif
				#if(SD_SYSTEM_MODE == SD_TF_SYSTEM)
					DBG_INFO(("CMD->W->"));
	    			SDI_WriteMultiBlockWithCmd(nAddr, nBuffBank, nSize);
	    		#endif
				#if(SD_SYSTEM_MODE == SD_MULT_SYSTEM)				
					DBG_INFO(("CMD->W->"));
					if(gSDISysMode == SD_TF_SYSTEM)
						SDI_WriteMultiBlockWithCmd(nAddr, nBuffBank, nSize);
					else if(gSDISysMode == SD_SQI_FLASH_SYSTEM)
						SQI_WriteData(SDSI_BUFF_BANK(nBuffBank), (SQI_FLASH_ADDR_OFF+nAddr*0x200), nSize*0x200);
					else if(gSDISysMode == SD_VIRTUAL_SYSTEM)
				    	SD_MemWriteToFlash(nAddr, SDSI_BUFF_BANK(nBuffBank));
					else if(gSDISysMode == SD_EFC_FLASH_SYSTEM)
					{
						EFC_WrProtect(AREA_DATAPRO_WE);
						EFC_WriteInfo(SDSI_BUFF_BANK(nBuffBank), (0x30000+nAddr*0x200), nSize*0x200);
						EFC_WrProtect(AREA_PROT_DI);
					}
	    			else
					{
					}
	    		#endif
			}
			gCmd = 0;
		}
		else
		{
			if ((gCmdAddr >= SdsiFile.nFileAddr[CMD_FILE][0]) && (gCmdAddr <= SdsiFile.nFileAddr[ENC0_FILE][1]))
			{
				if (gCmdAddr <= SdsiFile.nFileAddr[CMD_FILE][1]) 	// security command receive and processing
				{                
	                //Do nothing
				}
				else
				{
                    SD_FlowProc(nAddr, nBuffBank, nSize, DIR_WR);// 
				}
			}
			else
			{                                        
                //DBG_INFO("Write data to gCmdAddr = 0x%x", gCmdAddr);
                //SD_WriteDataToMemory(nAddr, nBuffBank, nSize);
				#if(SD_SYSTEM_MODE == SD_VIRTUAL_SYSTEM)
				    SD_MemWriteToFlash(nAddr, SDSI_BUFF_BANK(nBuffBank));
				#endif
				#if(SD_SYSTEM_MODE == SD_EFC_FLASH_SYSTEM)
					DBG_INFO(("W->"));
					EFC_WrProtect(AREA_DATAPRO_WE);
					EFC_WriteInfo(SDSI_BUFF_BANK(nBuffBank), (0x30000+nAddr*0x200), nSize*0x200);
					EFC_WrProtect(AREA_PROT_DI);
	    		#endif
				#if(SD_SYSTEM_MODE == SD_SQI_FLASH_SYSTEM)
					DBG_INFO(("W->"));
		    		SQI_WriteData(SDSI_BUFF_BANK(nBuffBank), (SQI_FLASH_ADDR_OFF+nAddr*0x200), nSize*0x200);
	    		#endif
				#if(SD_SYSTEM_MODE == SD_TF_SYSTEM)
					DBG_INFO(("W->"));
	    			SDI_WriteMultiBlockNoCmd(nBuffBank, nSize);
	    		#endif
				#if(SD_SYSTEM_MODE == SD_MULT_SYSTEM)				
					DBG_INFO(("W->"));
					if(gSDISysMode == SD_TF_SYSTEM)
						SDI_WriteMultiBlockNoCmd(nBuffBank, nSize);
					else if(gSDISysMode == SD_SQI_FLASH_SYSTEM)
						SQI_WriteData(SDSI_BUFF_BANK(nBuffBank), (SQI_FLASH_ADDR_OFF+nAddr*0x200), nSize*0x200);
					else if(gSDISysMode == SD_VIRTUAL_SYSTEM)
				    	SD_MemWriteToFlash(nAddr, SDSI_BUFF_BANK(nBuffBank));
					else if(gSDISysMode == SD_EFC_FLASH_SYSTEM)
					{
						EFC_WrProtect(AREA_DATAPRO_WE);
						EFC_WriteInfo(SDSI_BUFF_BANK(nBuffBank), (0x30000+nAddr*0x200), nSize*0x200);
						EFC_WrProtect(AREA_PROT_DI);
					}
	    			else
					{
					}
	    		#endif
			}
		}
	}
	else
	{ 
        DBG_INFO("nAddr > SdsiDev.nCapacity!!!");
	}
	return RT_OK;
}
/***************************************************************************
* Subroutine:	SD_Erase
* Function:		to erase SDSI address.
* Input:		nStartAddr:Start address
* 				nEndAddr:End address
* Output:		None
* Description:	
* Date:			2014/01/21
* ModifyRecord:
* *************************************************************************/
UINT8 SD_Erase(UINT32 nStartAddr, UINT32 nEndAddr)
{
    DBG_FUNC_BEGIN;

	SDI_EraseBlock(nStartAddr, nEndAddr);

    DBG_FUNC_END;
	return RT_OK;	
}
/***************************************************************************
* Subroutine:	SD_AppInit
* Function:		to initialize our SD application.
* Input:		None
* Output:		None
* Description:	
* Date:			2014/01/21
* ModifyRecord:
* *************************************************************************/
void SD_AppInit(void)
{       
    DBG_FUNC_BEGIN;

	GpioRegs.GPAUSE2.bit.GPA19=INPUT;		
	gSDISysMode =  SD_SYSTEM_MODE;
	#if(SD_SYSTEM_MODE == SD_MULT_SYSTEM)					
	if(GpioRegs.GPADATA.bit.GPA19 == 0)
		gSDISysMode = SD_TF_SYSTEM;
	else
		gSDISysMode = SD_SQI_FLASH_SYSTEM;
	#endif
	SdsiDev.pSdsiFile = (P_SDSI_FILE)(&SdsiFile);
	SDSI_Init(&SdsiDev);
    DBG_FUNC_END;
}
/***************************************************************************
* Subroutine:	TF_GetFreeSectors
* Function:		返回磁盘当前可用扇区数和总扇区数
* Input:		path:[IN]:磁盘路径
*               totalSectors:[OUT]:可用扇区数
*               freeSectors:[OUT]:总扇区数
* Output:		None
* Description:	
* Date:			2015/02/03
* ModifyRecord:
* *************************************************************************/
UINT8 TF_GetFreeSectors(char *path, UINT32* totalSectors, UINT32* freeSectors)
{       
    UINT32 res = 0; 
    FATFS fs_test = {0}, *fs_get = NULL;  
    DWORD fre_clust = 0, fre_sect = 0, tot_sect = 0;

    DBG_FUNC_BEGIN;	
                   
    res = f_mount(&fs_test, "0:", 1);	
	if(res != FR_OK)
	{
        DBG_ERR("ERROR:f_mount() failed! res = 0x%x", res);
        return res; 
	}

    res = f_getfree(path, &fre_clust, &fs_get); 
	if(res != FR_OK)
	{
        DBG_ERR("ERROR:f_getfree() failed! res = 0x%x", res);
        //return res;
        goto UNMOUNT; 
	}

    tot_sect = (fs_get->n_fatent - 2) * fs_get->csize;
    fre_sect = fre_clust * fs_get->csize;
    *totalSectors = tot_sect;
    *freeSectors = fre_sect;

    DBG_FUNC_END;
UNMOUNT:        
    f_mount(NULL, "0:", 1);
    return res;  
}
/***************************************************************************
* Subroutine:	TF_GetFilesAddr
* Function:		获取指定路径下、TF卡内COS系统文件物理地址，如果文件不存在则创建.
* Input:		dir_name:[IN]:文件路径
*               files_name:[IN]:文件名
*               files_addr:[OUT]:返回的文件起始地址
* Output:		None
* Description:	
* Date:			2015/08/01
* ModifyRecord:
* *************************************************************************/ 
UINT8 TF_GetFilesAddr(const char *dir_name, const char files_name[FILE_NUM][16], UINT32* files_addr)
{    
    UINT32 res = 0;
    UINT bw = 0, i = 0;
    BOOL create_files = FALSE; 
    char file_name[256] = {0}, readbuf[2] = {0}; 
	FIL files[FILE_NUM] = {0}; 
    FATFS fs_test = {0};  
    DIR dir = {0};

    DBG_FUNC_BEGIN;	
                   
    // Mount TF卡
    res = f_mount(&fs_test, "0:", 1);	
	if(res != FR_OK)
	{
        DBG_ERR("ERROR:f_mount() failed! res = 0x%x", res);
        return res; 
	}

    // 尝试打开文件
    for (i = 0; i < FILE_NUM; i++)
    { 
        strcpy(file_name, dir_name);  
        strcat(file_name, files_name[i]);
    	res = f_open(&files[i], file_name, FA_WRITE|FA_READ);
        if (res != FR_OK)
        {
            DBG_INFO("File %s doesn't exist!Need to create all system files!!!", file_name); 
            create_files = TRUE;
            break;
        }        
    }

    // 文件不存在，需要创建
    if (create_files)
    {
        for (i = 0; i < FILE_NUM; i++)
        {
            if (files[i].fs != 0)   
	            f_close(&files[i]);
        }

        // 非根目录，需要创建目录
        if (strcmp(dir_name, "0:") != 0 &&
            strcmp(dir_name, "0:/") != 0)
        {
            memset(file_name , 0 , 256);       
            strcpy(file_name, dir_name); 
            res = f_opendir(&dir, file_name);
            if (res == FR_OK)
            {
                // 删除整个目录
                res = f_unlink(file_name);
                if (res != FR_OK)
                {
                    DBG_ERR("ERROR:file_name = %s, f_unlink() failed! res = 0x%x", file_name, res); 
                    goto UNMOUNT;
                }
            }
            // 重新创建目录 
            res = TF_CreateDir(file_name);
            if (res != 0)
            {            
                DBG_ERR("ERROR:Dir name:%s, TF_CreateDir() failed! res = 0x%x", file_name, res); 
                goto UNMOUNT;
            }
        }
        
        // 创建文件     
        for (i = 0; i < FILE_NUM; i++)   
        {
            strcpy(file_name, dir_name);  
            strcat(file_name, files_name[i]);
        	res = f_open(&files[i], file_name, FA_CREATE_ALWAYS|FA_WRITE|FA_READ);
            if (res != FR_OK)
            {              
                DBG_ERR("ERROR:file_name = %s, f_open() failed! res = 0x%x", file_name, res); 
                break;
            }
            DBG_INFO("%s is created!", file_name);       
        }
    }

    // 创建失败
    if (res != FR_OK) 
    {  
        for (i = 0; i < FILE_NUM; i++)   
	        f_close(&files[i]);
        goto UNMOUNT;
    }

    // 设置文件大小，并返回文件起始地址
    for (i = 0; i < FILE_NUM; i++)   
    {                          
        DBG_INFO("Set file %d size...", i);  
		res = f_lseek(&files[i], SD_DATFILE_SIZE); 
        if (res != FR_OK)
        {              
            DBG_ERR("ERROR:f_lseek() failed! res = 0x%x", res); 
            break;
        }           
        DBG_INFO("Read file %d size...", i); 
    	res = f_lseek(&files[i], 0);  
    	res = f_read(&files[i], readbuf, 1, &bw); 
    	if(res == FR_OK)
    	{
            files_addr[i] = files[i].dsect;  
            DBG_INFO("File %d addr:0x%x", i, files_addr[i]); 
            printf("File %d addr:0x%x\n", i, files_addr[i]); 
    	}
    	else
    	{      
            DBG_ERR("ERROR:f_lseek() failed! res = 0x%x", res); 
    		files_addr[i] = 0;
    	} 
        f_close(&files[i]);
    }

    DBG_FUNC_END;
UNMOUNT:        
    f_mount(NULL, "0:", 1);
    return res;        
}
/***************************************************************************
* Subroutine:	TF_GetSysFileAddr
* Function:		获取指定路径下、TF卡内COS系统文件物理地址，如果文件不存在则创建.
* Input:		path:[IN]:文件路径
*               name:[IN]:文件名
*				size:[IN]:文件大小,只有创建文件时才会用到该参数
*               addr:[OUT]:返回的文件起始地址
* Output:		None
* Description:	
* Date:			2015/09/07
* ModifyRecord:
* *************************************************************************/ 
UINT8 TF_GetSysFileAddr(const char *path, const char *name, UINT32 size, UINT32* addr)
{
	UINT32 res = 0;
    UINT bw = 0;
    BOOL create_files = FALSE; 
    char file_name[256] = {0}, readbuf[2] = {0}; 
	FIL file = {0}; 
    FATFS fs_test = {0};  
    DIR dir = {0};

    DBG_FUNC_BEGIN;	
                   
	// 参数检查
	if (!path || strlen(path)==0 || !name || strlen(name)==0 || size==0 || !addr)
	{
		return 1;
	}

    // Mount TF卡
    res = f_mount(&fs_test, "0:", 1);	
	if (FR_OK != res)
	{
        DBG_ERR("ERROR:f_mount() failed! res = 0x%x", res);
        return res; 
	}

    // 尝试打开文件
    strcpy(file_name, path);  
    strcat(file_name, name);
    res = f_open(&file, file_name, FA_WRITE|FA_READ);
    if (FR_OK != res)
    {
        DBG_INFO("File %s doesn't exist!Need to create all system files!!!", file_name); 
        create_files = TRUE;
    }        

    // 文件不存在，需要创建
    if (create_files)
    {
        // 非根目录，需要创建目录
        if (strcmp(path, "0:") != 0 &&
            strcmp(path, "0:/") != 0)
        {
            memset(file_name, 0, 256);       
            strcpy(file_name, path); 
            res = f_opendir(&dir, file_name);
			if (FR_OK != res)
			{
				res = TF_CreateDir(file_name);
				if (FR_OK != res)
				{            
					DBG_ERR("ERROR:Dir name:%s, TF_CreateDir() failed! res = 0x%x", file_name, res); 
					goto UNMOUNT;
				}
			}
        }
        
        // 创建文件 
        memset(file_name, 0, 256);  
        strcpy(file_name, path);  
        strcat(file_name, name);
        res = f_open(&file, file_name, FA_CREATE_ALWAYS|FA_WRITE|FA_READ);
        if (FR_OK != res)
        {              
            DBG_ERR("ERROR:file_name = %s, f_open() failed! res = 0x%x", file_name, res); 
            goto UNMOUNT;
        }
        DBG_INFO("%s is created!", file_name);    
    }

    // 设置文件大小，并返回文件起始地址
	res = f_lseek(&file, size); 
    if (FR_OK != res)
    {              
        DBG_ERR("ERROR:f_lseek() failed! res = 0x%x", res); 
        goto CLOSE_FILE;
    }
	res = f_lseek(&file, 0);  
    res = f_read(&file, readbuf, 1, &bw); 
    if (FR_OK == res)
    {
        *addr = file.dsect;  
        DBG_INFO("File %s addr:0x%x", file_name, *addr); 
    }
    else
    {      
        DBG_ERR("ERROR:f_lseek() failed! res = 0x%x", res); 
        *addr = 0;  
    } 

    DBG_FUNC_END;
CLOSE_FILE:
    f_close(&file); 
UNMOUNT:       
    f_mount(NULL, "0:", 1);
    return res;        
}
/***************************************************************************
* Subroutine:	TF_CreateDir
* Function:		to create a folder on the TF card.
* Input:		None
* Output:		None
* Description:	
* Date:			2015/02/03
* ModifyRecord:
* *************************************************************************/
UINT8 TF_CreateDir(char *dir_name)  
{                
	UINT8 res;
	UINT32 i = 0, len = strlen(dir_name); 
	DIR dir;

    DBG_FUNC_BEGIN;

    //在末尾加/  
	if (dir_name[len - 1] != '/')  
	{  
		dir_name[len] = '/';  
		dir_name[len + 1] = '\0';  
	} 
    
    DBG_INFO("Create dir:%s", dir_name); 
     
	// 创建目录  
	for (i = 0;i < len+1;i ++)  
	{  
		if (dir_name[i] == '/')  
		{   
			dir_name[i] = '\0';  
  
			//如果不存在,创建
            res = f_opendir(&dir, dir_name); 
			if (res != FR_OK)  
			{  
				res = f_mkdir(dir_name);  
				if (res != FR_OK)  
				{             
                    DBG_ERR("ERROR:f_mkdir() failed! res = 0x%x", res); 
					return res;  
				}   
			}  
			dir_name[i] = '/';  
		}   
	}  
  
    DBG_FUNC_END;
	return 0;  
}
/***************************************************************************
* Subroutine:	TF_CreateDir
* Function:		to create a folder on the TF card.
* Input:		None
* Output:		None
* Description:	
* Date:			2015/02/03
* ModifyRecord:
* *************************************************************************/
static BOOL SD_ReadDataFromMemory(UINT32 nAddr, UINT8 nBuffBank, UINT32 nSize)
{ 
	#if(SD_SYSTEM_MODE == SD_VIRTUAL_SYSTEM)
	    SD_MemReadFromFlash(nAddr, SDSI_BUFF_BANK(nBuffBank));
	#endif
	#if(SD_SYSTEM_MODE == SD_EFC_FLASH_SYSTEM)
		DBG_INFO(("CMD->R->"));
		EFC_ReadInfo(SDSI_BUFF_BANK(nBuffBank), (0x30000+nAddr*0x200), nSize*0x200);
	#endif
	#if(SD_SYSTEM_MODE == SD_SQI_FLASH_SYSTEM)
		DBG_INFO(("CMD->R->"));
		SQI_ReadData(SDSI_BUFF_BANK(nBuffBank), (SQI_FLASH_ADDR_OFF+nAddr*0x200), nSize*0x200);
	#endif
	#if(SD_SYSTEM_MODE == SD_TF_SYSTEM)
		DBG_INFO(("CMD->R->"));
		SDI_ReadMultiBlockWithCmd(nAddr, nBuffBank, nSize);
	#endif
	#if(SD_SYSTEM_MODE == SD_MULT_SYSTEM)				
		DBG_INFO(("CMD->R->"));
		if(gSDISysMode == SD_TF_SYSTEM)
			SDI_ReadMultiBlockWithCmd(nAddr, nBuffBank, nSize);
		else if(gSDISysMode == SD_SQI_FLASH_SYSTEM)
			SQI_ReadData(SDSI_BUFF_BANK(nBuffBank), (SQI_FLASH_ADDR_OFF+nAddr*0x200), nSize*0x200);
		else if(gSDISysMode == SD_VIRTUAL_SYSTEM)
	    	SD_MemReadFromFlash(nAddr, SDSI_BUFF_BANK(nBuffBank));
		else if(gSDISysMode == SD_EFC_FLASH_SYSTEM)
	    	EFC_ReadInfo(SDSI_BUFF_BANK(nBuffBank), (0x30000+nAddr*0x200), nSize*0x200);
		else
		{
		}
	#endif

    return TRUE;
}

static BOOL SD_WriteDataToMemory(UINT32 nAddr, UINT8 nBuffBank, UINT32 nSize)
{  
	#if(SD_SYSTEM_MODE == SD_VIRTUAL_SYSTEM)
	    SD_MemWriteToFlash(nAddr, SDSI_BUFF_BANK(nBuffBank));
	#endif
	#if(SD_SYSTEM_MODE == SD_EFC_FLASH_SYSTEM)
		DBG_INFO(("CMD->W->"));
		EFC_WrProtect(AREA_DATAPRO_WE);
		EFC_WriteInfo(SDSI_BUFF_BANK(nBuffBank), (0x30000+nAddr*0x200), nSize*0x200);
		EFC_WrProtect(AREA_PROT_DI);
	#endif
	#if(SD_SYSTEM_MODE == SD_SQI_FLASH_SYSTEM)
		DBG_INFO(("CMD->W->"));
		SQI_WriteData(SDSI_BUFF_BANK(nBuffBank), (SQI_FLASH_ADDR_OFF+nAddr*0x200), nSize*0x200);
	#endif
	#if(SD_SYSTEM_MODE == SD_TF_SYSTEM)
		DBG_INFO(("CMD->W->"));
		SDI_WriteMultiBlockWithCmd(nAddr, nBuffBank, nSize);
	#endif
	#if(SD_SYSTEM_MODE == SD_MULT_SYSTEM)				
		DBG_INFO(("CMD->W->"));
		if(gSDISysMode == SD_TF_SYSTEM)
			SDI_WriteMultiBlockWithCmd(nAddr, nBuffBank, nSize);
		else if(gSDISysMode == SD_SQI_FLASH_SYSTEM)
			SQI_WriteData(SDSI_BUFF_BANK(nBuffBank), (SQI_FLASH_ADDR_OFF+nAddr*0x200), nSize*0x200);
		else if(gSDISysMode == SD_VIRTUAL_SYSTEM)
	    	SD_MemWriteToFlash(nAddr, SDSI_BUFF_BANK(nBuffBank));
		else if(gSDISysMode == SD_EFC_FLASH_SYSTEM)
		{
			EFC_WrProtect(AREA_DATAPRO_WE);
			EFC_WriteInfo(SDSI_BUFF_BANK(nBuffBank), (0x30000+nAddr*0x200), nSize*0x200);
			EFC_WrProtect(AREA_PROT_DI);
		}
		else
		{
		}
	#endif

    return TRUE;
}
