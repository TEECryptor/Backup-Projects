/***************************************************************************
 *	File name:Device.h
 *	Introduce:Head file for device object functions definition
 *	Author:Syno common
 *	Date:2015/07/18
 *  Version:v1.0
 **************************************************************************/

#ifndef _DEVICE_H_
#define _DEVICE_H_

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include "common.h"

/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/
#define MAX_DEV_CNT                            8 /* 同时支持最大的设备个数 */

/*----------------------------------------------*
 * 外部变量说明                                 *
 *----------------------------------------------*/
extern BOOL g_bDefinedPath;                      /* 自定义设备路径 */

/*----------------------------------------------*
 * 复合数据类型定义                             *
 *----------------------------------------------*/

/* 设备虚拟文件 */
typedef struct tagVirFile
{
    INT32 iCmdFd;
    INT32 iEncFd;
    INT32 iDecFd;
} VIRFILE_S;

/* 设备读写 */
typedef struct tagRWDevice
{
    INT32 iRWFd;
    UCHAR *pucIOBuf;
} RWDEVICE_S;

/* 设备IO驱动 */
typedef struct tagIODriver
{
    VIRFILE_S stVirFile;
    UCHAR *pucCmdBuf;
    UCHAR *pucDataBuf;
} IODRIVER_S;

#ifdef __cplusplus
extern "C" {
#endif

ULONG DEV_SetDevicePath(CHAR *pcDevPath);

void DEV_ClearDevicePath();

ULONG CreateIOBuf(UCHAR **ppIOBuf, ULONG ulBufLen);

ULONG DEV_OpenDevice(INT32 iDevNum, HANDLE* pHandle);

void DEV_CloseDevice(HANDLE pHandle);

ULONG DEV_Write(RWDEVICE_S *pstRWDev, ULONG ulOffset, UCHAR *pucBuf, ULONG *pulDataLen);

ULONG DEV_Read(RWDEVICE_S *pstRWDev, ULONG ulOffset, UCHAR *pucBuf, ULONG *pulDataLen);

//ULONG DEV_CmdWrite(RWDEVICE_S *pstRWDev, UCHAR *pucCmd, ULONG ulCmdLen,
//    UCHAR *pucData, ULONG ulDataLen);
//
//ULONG DEV_CmdRead(RWDEVICE_S *pstRWDev, UCHAR *pucData, ULONG *pulDataLen);

#ifdef __cplusplus
}
#endif

#endif /* _DEVICE_H_ */

