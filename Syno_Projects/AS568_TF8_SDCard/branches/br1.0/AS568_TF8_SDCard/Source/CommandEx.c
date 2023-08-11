/***************************************************************************
 *	File name:CommandEx.c
 *	Introduce:Extended command implement file
 *	Author:Syno common
 *	Date:2015/07/6
 *  Version:v1.0
 **************************************************************************/

#include "CommandEx.h" 	  
#include "SdMemAcc.h"
#include "log.h"

extern void SetCmdResult(UINT16 nSW, UINT16 nLen);

void DoAS5xxCMDEx()
{            	
    if((gApduHeader.CLA&0xFE) != 0xFE)
    {
		SetCmdResult(SAR_CLAERR, 0);  
        DBG_ERR("gApduHeader.CLA is wrong! gApduHeader.CLA = 0x%x", gApduHeader.CLA);
		return;
	}

	switch(gApduHeader.INS)
    {
		case CMD_GET_DEVINFO:
	        CMD_GetDevInfo();break;		            
		case CMD_SET_DEVPARAM:
	        CMD_SetDevParam();break;    
		case CMD_SET_DEVSLEEP:
	        CMD_SetDevSleep();break;    
		case CMD_SET_BURNMODE:
	        CMD_SetBurnMode();break;    
		case CMD_GET_DEVSTATUS:
	        CMD_GetDevStatus();break;   
		case CMD_CLEAN_FLASH:
	        CMD_CleanFlash();break;     
		case CMD_WRITE_FLASH:
	        CMD_WriteFlash();break;     
		case CMD_SET_UPDATEPARAM:
	        CMD_SetUpdateParam();break; 
		case CMD_DEVINIT:
	        CMD_DevInit();break;			
		case CMD_FORMATCD:
	        CMD_FormatCD();break;                    
		case CMD_RAND_XN:         
	        CMD_RandXN();break;            
		case CMD_GENPRIME:
	        CMD_GenPrime();break;
	    default:   
	         SetCmdResult(SAR_EX_NOTSUPPORTYETERR,0);
	        break;
    }
}

void CMD_GetDevInfo(void)
{
    SetCmdResult(SAR_EX_FAIL, 0);
}
void CMD_SetDevParam(void)
{
    SetCmdResult(SAR_EX_FAIL, 0);
}
void CMD_SetDevSleep(void)
{
    SetCmdResult(SAR_EX_FAIL, 0);
}
void CMD_SetBurnMode(void)
{
    SetCmdResult(SAR_EX_FAIL, 0);
}
void CMD_GetDevStatus(void)
{
    SetCmdResult(SAR_EX_FAIL, 0);
}
void CMD_CleanFlash(void)
{
    SetCmdResult(SAR_EX_FAIL, 0);
}
void CMD_WriteFlash(void)
{
    SetCmdResult(SAR_EX_FAIL, 0);
}
void CMD_SetUpdateParam(void)
{
    SetCmdResult(SAR_EX_FAIL, 0);
}
void CMD_DevInit(void)
{
    SetCmdResult(SAR_EX_FAIL, 0);
}
/***************************************************************************
* Subroutine:	CMD_FormatCD()
* Function:		把ISO文件烧写到CD  
* Author:   Comon		 
* Date:		2010.5.30
* Version:  V1.21
* ModifyRecord:
* ************************************************************************/
void CMD_FormatCD()
{
	UINT32 lStartLBA;
	UINT16 nLen ;
		
	lStartLBA=(gApduHeader.P1<<8)+gApduHeader.P2;
	nLen=gApduBody.LC;
		
    XF_WriteData(gApduBody.DATA,lStartLBA*2048,nLen);
	
	SetCmdResult(RT_OK,0);
}
void CMD_RandXN(void)
{
    SetCmdResult(SAR_EX_FAIL, 0);
}
void CMD_GenPrime(void)
{
    SetCmdResult(SAR_EX_FAIL, 0);
}
