/***************************************************************************
 *  File name:SdMemAcc.h
 *  Function:Header of SdMemAcc.c
 *  Author:Syno Common
 *  Date:2015/06/20
 *  Version:v1.0
****************************************************************************/
#ifndef SD_MEM_ACC_H_
#define SD_MEM_ACC_H_

/***************************************************************************
* Include Header Files
***************************************************************************/
#include "SYglobal.h"


/**************************************************************************
* Global Macro Definition
***************************************************************************/
#define	SD_FILES_ADDR		SDSI_BUFF_BANK(0)

#define	DIR_WR		0
#define	DIR_RD		1

#define	SD_CMDFILE_SIZE		0x1000
#define	SD_DATFILE_SIZE		0x3C00
#define	SD_CMDBUF_LEN		SD_CMDFILE_SIZE			//Max CMD data length

#ifdef  USE_FAT32
#define	ADDR_DBR		    0x0
#define	ADDR_FAT1		    0x1012  //0x202400
#define	ADDR_FAT2		    0x1809  //0x301200
#define	ADDR_ROOT_DIR	    0x2000  //0x400000
#define	ADDR_FILE_COMM	    0x2002  //0x400400
#define	ADDR_FILE_DEC0	    0x200A  //0x401400
#define	ADDR_FILE_ENC0	    0x2028  //0x405000
#else
#ifdef USE_ONE_FILE 
#define	ADDR_DBR		    0x0
#define	ADDR_FAT1		    0x8     //0x1000
#define	ADDR_FAT2		    0x14    //0x2800
#define	ADDR_ROOT_DIR	    0x20    //0x4000
#define	ADDR_FILE_COMM	    0x40    //0x8000
#define	ADDR_FILE_DEC0	    (ADDR_FILE_COMM + SD_CMDFILE_SIZE / SDSI_BUFF_BANK_SIZE) 
#define	ADDR_FILE_ENC0	    (ADDR_FILE_COMM + (SD_CMDFILE_SIZE+SD_DATFILE_SIZE) / SDSI_BUFF_BANK_SIZE)
#else  
#define	ADDR_DBR		    0x0
#define	ADDR_FAT1		    0x8     //0x1000
#define	ADDR_FAT2		    0x14    //0x2800
#define	ADDR_ROOT_DIR	    0x20    //0x4000
#define	ADDR_FILE_COMM	    0x40    //0x8000
#define	ADDR_FILE_DEC0	    0x48    //0x9000
#define	ADDR_FILE_ENC0	    0x68    //0xD000
#endif  //USE_ONE_FILE
#endif  //FAT32 


/**************************************************************************
* Global Type Definition
***************************************************************************/

/**************************************************************************
* Global Variable Declaration
***************************************************************************/
extern BOOL     gSystemInited;
extern UINT8    gRespDataBuf[SD_CMDBUF_LEN];
extern UINT8*   gSDSIBuffAddr;
extern APDUHEADER_S        gApduHeader;
extern APDUBODY_S          gApduBody;
extern APDURESP_S          gApduResp;   

/**************************************************************************
* Global Functon Declaration
***************************************************************************/
UINT8 SD_MemReadFromFlash(UINT32 nAddr, UINT8 *pBuf);
UINT8 SD_MemWriteToFlash(UINT32 nAddr, UINT8 *pBuf);
void SD_CmdProc(UINT32 nArg, UINT8 *pBuf, UINT8 nFlgDir);
void SD_FlowProc(UINT32 nArg, UINT8 nBufBank,UINT32 nSize, UINT8 nFlgDir);


#endif
