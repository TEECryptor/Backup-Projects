/**********************************************************************************
* File name    :	SD_SystemApp.h
* Function     :	The SD card System Application 
* Author       : 	coins
* Date         :	2014/01/25
* Version      :    v1.0
* Description  :	
* ModifyRecord :
**********************************************************************************/
#ifndef SD_SYSTEM_APP_H_
#define SD_SYSTEM_APP_H_

/***************************************************************************
* Include Header Files
***************************************************************************/
#include "SYglobal.h"

/**************************************************************************
* Global Macro Definition
***************************************************************************/
#define SD_NULL_SYSTEM			0//
#define SD_VIRTUAL_SYSTEM		1//虚拟固定文件（44k）
#define SD_EFC_FLASH_SYSTEM		2//容量在内部flash（44k）
#define SD_SQI_FLASH_SYSTEM		3//容量在SQI flash（）
#define SD_TF_SYSTEM			4//容量在实际TF
#define SD_MULT_SYSTEM			10//容量根据实际存储空间

#define SD_SYSTEM_MODE 			SD_TF_SYSTEM//SD_VIRTUAL_SYSTEM

#define SQI_FLASH_CAP			2*1024*1024//
#define SQI_FLASH_ADDR_OFF		0//

#define	FILE_NUM	3
#define	CMD_FILE	0
#define	DEC0_FILE	1
#define	ENC0_FILE	2
#define	DEC1_FILE	3
#define	ENC1_FILE	4
#define	DEC2_FILE	5
#define	ENC2_FILE	6
/**************************************************************************
* Global Type Definition
***************************************************************************/

/**************************************************************************
* Global Variable Declaration
***************************************************************************/
extern UINT32 gSDICapacity;
extern UINT8 gSDISysMode;
extern SDSI_FILE SdsiFile;
/**************************************************************************
* Global Functon Declaration
***************************************************************************/
//
UINT8 SD_Init(void *p);
UINT8 SD_MsgProcess(UINT32 nAddr, UINT8 nMsg);
UINT8 SD_Read(UINT32 nAddr, UINT8 nBuffBank, UINT32 nSize);
UINT8 SD_Write(UINT32 nAddr, UINT8 nBuffBank, UINT32 nSize);
UINT8 SD_Erase(UINT32 nStartAddr, UINT32 nEndAddr);
void  SD_FileInit(void);
//
UINT8 TF_GetFreeSectors(char *path, UINT32* totalSectors, UINT32* freeSectors);
UINT8 TF_GetFilesAddr(const char *dir_name, const char files_name[FILE_NUM][16], UINT32* files_addr);
UINT8 TF_GetSysFileAddr(const char *path, const char *name, UINT32 size, UINT32* addr);
UINT8 TF_CreateDir(char *dir_name);

#endif
