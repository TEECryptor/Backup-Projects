/***************************************************************************
 *	File name:Device.c
 *	Introduce:The implmenet file for device functions
 *	Author:Syno common
 *	Date:2015/07/18
 *  Version:v1.0
 **************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include "device.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(WIN32)
#include <windows.h>
#include <fcntl.h>
#include <io.h>
#elif defined(LINUX) || defined(ANDROID)
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#endif

#if defined(LINUX)
#include <sys/dir.h>
#endif

#ifdef SYNO_ASIDE
#include "ff.h"
#endif


/*----------------------------------------------*
 * 全局变量                                     *
 *----------------------------------------------*/
BOOL g_bDefinedPath = FALSE;                /* 自定义设备路径 */

/*----------------------------------------------*
 * 模块级变量                                   *
 *----------------------------------------------*/
static CHAR s_acDevPath[PATH_MAX] = {0};    /* 设备路径 */

#ifndef O_RDWR
#define O_RDWR 0
#endif

/* 定义宏:替换不同平台文件操作函数 */
#if defined(ANDROID) || defined(LINUX)

#ifndef O_DIRECT
#define O_DIRECT 040000
#endif

#if defined(LINUX)
#ifndef _OPEN
#define _OPEN(pathname, flags)	open(pathname,  O_CREAT | O_RDWR | O_DIRECT | O_SYNC, S_IRWXU)
#endif
#else
#ifndef _OPEN
#define _OPEN(pathname, flags)	open(pathname,  O_RDWR | O_DIRECT)
#endif
#endif

#ifndef _WRITE
#define _WRITE(handle, buf, len)	write(handle, buf, len)
#endif

#ifndef _READ
#define _READ(handle, buf, len)	read(handle, buf, len)
#endif

#ifndef _LSEEK
#define _LSEEK(handle, offset)	lseek(handle, offset, SEEK_SET)
#endif

#ifndef _CLOSE
#define _CLOSE(handle)	close(handle)
#endif

#elif defined(WIN32)

INT32 WinWriteF(INT32 handle, void *buf, INT32 len)
{
    INT32 iSize = 0;
    WriteFile((HANDLE)handle, buf, len, (LPDWORD)&iSize, NULL);
    return iSize;
}

INT32 WinReadF(INT32 handle, void *buf, INT32 len)
{
    INT32 iSize = 0;
    ReadFile((HANDLE)handle, buf, len, (LPDWORD)&iSize, NULL);
    return iSize;
}

#ifndef _OPEN
#define _OPEN(pathname, flags)  (INT32 )CreateFile(	\
	(LPCTSTR)pathname, \
	((flags & (O_RDONLY | O_RDWR))? GENERIC_READ : 0) \
	| ((flags & (O_WRONLY | O_RDWR))? GENERIC_WRITE : 0), \
	FILE_SHARE_READ|FILE_SHARE_WRITE, \
	NULL, \
	OPEN_EXISTING, \
	FILE_FLAG_NO_BUFFERING, \
	NULL);
#endif

#ifndef _WRITE
#define _WRITE(handle, buf, len) WinWriteF(handle, buf, len)
#endif

#ifndef _READ
#define _READ(handle, buf, len)  WinReadF(handle, buf, len)
#endif

#ifndef _LSEEK
#define _LSEEK(handle, offset)	SetFilePointer((HANDLE)handle, offset, 0, FILE_BEGIN)
#endif

#ifndef _CLOSE
#define _CLOSE(handle)	CloseHandle((HANDLE)handle)
#endif

#elif defined(SYNO_ASIDE)

INT32 SYNO_Open(CHAR *pcPathName)
{
    INT32 iRet = SAR_OK;
    FIL *pHandle = NULL;

    /* 参数检查 */
    if (NULL == pcPathName)
    {
        LOG_ErrNo(SAR_INVALIDPARAMERR);
        return -1;
    }

    pHandle = ICA_malloc(sizeof(FIL));
    if (NULL == pHandle)
    {
        LOG_ErrNo(SAR_MEMORYERR);
        return -1;
    }

    iRet = f_open(pHandle, pcPathName, FA_READ | FA_WRITE);
    if (SAR_OK != iRet)
    {
        ICA_LogE("f_open err! iRet=<%d>", iRet);
        return -1;
    }

    return (INT32)pHandle;
}

INT32 SYNO_Write(INT32 iHandle, void *pvBuf, INT32 iLen)
{
    INT32 iRet = SAR_OK;
    UINT32 iSize = 0;

    /* 参数检查 */
    if (NULL == pvBuf)
    {
        LOG_ErrNo(SAR_INVALIDPARAMERR);
        return -1;
    }

    iRet = f_write((FIL *)iHandle, pvBuf, (UINT32)iLen, &iSize);
    if (SAR_OK != iRet)
    {
        ICA_LogE("f_write err! iRet=<%d>", iRet);
        return -1;
    }

    return iSize;
}

INT32 SYNO_Read(INT32 iHandle, void *pvBuf, INT32 iLen)
{
    INT32 iRet = SAR_OK;
    UINT32 iSize = 0;

    iRet = f_read((FIL *)iHandle, pvBuf, (UINT32)iLen, &iSize);
    if (SAR_OK != iRet)
    {
        ICA_LogE("f_read err! iRet=<%d>", iRet);
        return -1;
    }

    return iSize;
}

INT32 SYNO_Close(INT32 handle)
{
    (void)f_close((FIL *)handle);
    ICA_free((FIL *)handle);
    return SAR_OK;
}

#ifndef _OPEN
#define _OPEN(pathname, flags) SYNO_Open(pathname)
#endif

#ifndef _WRITE
#define _WRITE(handle, buf, len) SYNO_Write(handle, buf, len)
#endif

#ifndef _READ
#define _READ(handle, buf, len)	SYNO_Read(handle, buf, len)
#endif

#ifndef _LSEEK
#define _LSEEK(handle, offset)	f_lseek((FIL *)handle, offset)
#endif

#ifndef _CLOSE
#define _CLOSE(handle)	SYNO_Close(handle)
#endif

#endif

ULONG CreateIOBuf(UCHAR **ppIOBuf, ULONG ulBufLen)
{
    FUNC_BEGIN;

#if defined(WIN32) || defined(SYNO_ASIDE)
    ICA_LogD("system is WIN32 or SYNO_ASIDE!");
    *ppIOBuf = (UCHAR *)ICA_malloc(ulBufLen);
#elif defined(LINUX)
    ICA_LogD("system is Linux!");
	if(0 != posix_memalign((void **)ppIOBuf, (size_t)512, (size_t)ulBufLen))
	{
		LOG_ErrNo(SAR_MEMORYERR);
        return SAR_MEMORYERR;
	}
#elif defined(ANDROID)
    ICA_LogD("system is Android!");
    *ppIOBuf = (UCHAR *)memalign(512, ulBufLen);
#endif
    if (NULL == *ppIOBuf)
    {
        LOG_ErrNo(SAR_MEMORYERR);
        return SAR_MEMORYERR;
    }

    FUNC_END;

    return SAR_OK;
}

void CloseDevFile(VIRFILE_S *pstVirFile)
{
    /* 参数检查 */
    if(NULL == pstVirFile)
	{
        LOG_ErrNo(SAR_INVALIDPARAMERR);
		return;
	}

    if (0 != pstVirFile->iCmdFd)
    {
        _CLOSE(pstVirFile->iCmdFd);
        pstVirFile->iCmdFd = 0;
    }

    if (0 != pstVirFile->iEncFd)
    {
        _CLOSE(pstVirFile->iEncFd);
        pstVirFile->iEncFd = 0;
    }

    if (0 != pstVirFile->iDecFd)
    {
        _CLOSE(pstVirFile->iDecFd);
        pstVirFile->iDecFd = 0;
    }

    return;
}

ULONG OpenDevFile(CHAR *pcDevPath, VIRFILE_S *pstVirFile)
{
    ULONG ulRet = SAR_OK;
    VIRFILE_S stVirFile = {0};
    CHAR acFilePath[PATH_MAX] = {0};
    INT32 iFd = -1;
    INT32 i = 0;
	HANDLE hFile = NULL;

    FUNC_BEGIN;

    /* 参数检查 */
    if ((NULL == pcDevPath) || (NULL == pstVirFile))
    {
        LOG_ErrNo(SAR_INVALIDPARAMERR);
        return SAR_INVALIDPARAMERR;
    }

    if (0 == strlen(pcDevPath))
    {
        LOG_ErrNo(SAR_INVALIDDEVPATH);
        return SAR_INVALIDDEVPATH;
    }

    /* 打开设备命令通道文件 */
    sprintf(acFilePath, "%scommand.sys", pcDevPath);
    ICA_LogD("open acFilePath=<%s>", acFilePath);
    iFd = _OPEN(acFilePath, O_RDWR);
	if (-1 == iFd)
	{
        //ICA_LogE("open command.sys errno=<%d>,<%s>", errno, strerror(errno));
        return SAR_FILEERR;
	}

    stVirFile.iCmdFd = iFd;

 //	Youfang Yao 2015-09-07 (只用一个功能文件)
 //   /* 打开设备加密通道文件 */
 //   sprintf(acFilePath,"%sdataenc0.sys", pcDevPath);
 //   ICA_LogD("open acFilePath=<%s>", acFilePath);
 //   iFd = _OPEN(acFilePath, O_RDWR);
	//if (-1 == iFd)
	//{
 //       ICA_LogE("open dataenc.sys err!");
 //       CloseDevFile(&stVirFile);
 //       return SAR_FILEERR;
	//}

 //   stVirFile.iEncFd = iFd;

 //   /* 打开设备解密通道文件 */
 //   sprintf(acFilePath,"%sdatadec0.sys", pcDevPath);
 //   ICA_LogD("open acFilePath=<%s>", acFilePath);
 //   iFd = _OPEN(acFilePath, O_RDWR);
	//if (-1 == iFd)
	//{
 //       ICA_LogE("open datadec.sys err!");
 //       CloseDevFile(&stVirFile);
 //       return SAR_FILEERR;
	//}

 //   stVirFile.iDecFd = iFd;

    memcpy(pstVirFile, &stVirFile, sizeof(VIRFILE_S));

    FUNC_END;

    return SAR_OK;
}

ULONG OpenDefinedDev(HANDLE *pHandle)
{
    ULONG ulRet = SAR_OK;
    IODRIVER_S *pstIODriver = NULL;

    FUNC_BEGIN;

    pstIODriver = (IODRIVER_S *)ICA_calloc(1, sizeof(IODRIVER_S));
    if (NULL == pstIODriver)
    {
        LOG_ErrNo(SAR_MEMORYERR);
        return SAR_MEMORYERR;
    }

    /* 打开设备文件 */
    ulRet = OpenDevFile(s_acDevPath, &pstIODriver->stVirFile);
    if (SAR_OK != ulRet)
    {
        goto Err;
    }

    ulRet = CreateIOBuf(&pstIODriver->pucCmdBuf, MAX_CMD_BUF);
    if (SAR_OK != ulRet)
    {
        goto Err;
    }

	*pHandle = pstIODriver;

    FUNC_END;

    return SAR_OK;

Err:
    ICA_free(pstIODriver);
    return ulRet;
}

#if defined(WIN32)
ULONG OpenWin32Dev(INT32 iDevNum, HANDLE *pHandle)
{
    ULONG ulRet = SAR_OK;
    IODRIVER_S *pstIODriver = NULL;
    CHAR acPath[PATH_MAX] = {0};
	CHAR acDevPath[PATH_MAX] = {0};
    CHAR acSysName[PATH_MAX] = {0};
    INT32 iStatus = 0;
    INT32 iDrive = 0;
    INT32 iCnt = 0;
    INT32 iType = -1;

#ifndef DRIVE_REMOVABLE
#define DRIVE_REMOVABLE   2
#endif

#ifndef DRIVE_CDROM
#define DRIVE_CDROM       5
#endif

    /* 参数检查 */
    if ((0 > iDevNum) || (NULL == pHandle))
    {
        LOG_ErrNo(SAR_INVALIDPARAMERR);
        return SAR_INVALIDPARAMERR;
    }

    pstIODriver = (IODRIVER_S *)ICA_calloc(1, sizeof(IODRIVER_S));
    if (NULL == pstIODriver)
    {
        LOG_ErrNo(SAR_MEMORYERR);
        return SAR_MEMORYERR;
    }

    /* 通过循环检测A～Z查找可移动盘符来判断是否存在设备 */
	for (iDrive = 'Z'; iDrive >= 'C'; iDrive--)
	{
		sprintf(acPath, "%c:", iDrive);
		iType = GetDriveTypeA(acPath);

        /* 可移动类型磁盘 */
		if(iType == DRIVE_REMOVABLE)
		{
            /* 已经检查过的设备 */
			if (iCnt != iDevNum)
			{
				iCnt++;
				continue;
			}

            /* 检查设备是否存在 */
            sprintf(acDevPath,"%c:\\", iDrive);
			iStatus = GetVolumeInformationA(acDevPath,
				NULL,0,NULL,NULL,NULL, acSysName,PATH_MAX);
			if (0 == iStatus)
            {
				ICA_free(pstIODriver);
                return SAR_OK;
			}

            /* 检查是否为合法设备 */
            //sprintf(acDevPath,"%c:\\cloudshield\\", iDrive);
            sprintf(acDevPath,"%c:\\", iDrive);
			if(-1 == _access(acDevPath, 0))
			{
				ICA_free(pstIODriver);
                return SAR_OK;
			}

            /* 打开设备文件 */
            ulRet = OpenDevFile(acDevPath, &pstIODriver->stVirFile);
            if (SAR_OK != ulRet)
            {
                ICA_LogE("OpenDevFile err!");
                break;
            }

            ulRet = CreateIOBuf(&pstIODriver->pucCmdBuf, MAX_CMD_BUF);
            if (SAR_OK != ulRet)
            {
                ICA_LogE("CreateIOBuf err!");
                break;
            }

			*pHandle = (HANDLE)pstIODriver;

			return SAR_OK;
		}
	}

	ICA_free(pstIODriver);

	return ulRet;
}
#elif defined(LINUX)
int IS_DIR(const char* path)
{
    struct stat st;
    lstat(path, &st);
    return S_ISDIR(st.st_mode);
}

//遍历文件夹de递归函数
void List_Files_Core(const char *path, int recursive)
{
    DIR *pdir;
    struct dirent *pdirent;
    char temp[256];
    pdir = opendir(path);

    if(pdir)
    {
        while(pdirent = readdir(pdir))
        {
            //跳过"."和".."
            if((strcmp(pdirent->d_name, ".") == 0)
                || (strcmp(pdirent->d_name, "..") == 0))
            {
                continue;
            }

            sprintf(temp, "%s/%s", path, pdirent->d_name);

            //当temp为目录并且recursive为1的时候递归处理子目录
            if(IS_DIR(temp) && recursive)
            {
                List_Files_Core(temp, recursive);
            }
        }
    }
    else
    {
        ICA_LogE("opendir error:%s", path);
    }
    closedir(pdir);
}

//遍历文件夹的驱动函数
void List_Files(const char *path, int recursive)
{
    int len;
    char temp[256];

    //去掉末尾的'/'
    len = strlen(path);
    strcpy(temp, path);

    if(temp[len - 1] == '/')
    {
        temp[len -1] = '\0';
    }

    if(IS_DIR(temp))
    {
        //处理目录
        List_Files_Core(temp, recursive);
    }
    else   //输出文件
    {
        ICA_LogD("%s\n", path);
    }
}

ULONG OpenLinuxDev(INT32 iDevNum, HANDLE *pHandle)
{
    ULONG ulRet = SAR_OK;
    IODRIVER_S *pstIODriver = NULL;
    CHAR  acPath[PATH_MAX] = {0};
	CHAR  acDriver[PATH_MAX] = {0};
	CHAR  path[PATH_MAX] = "/media";
    INT32 iDriver = 0;
    INT32 iCnt = 0;
    INT32 iType = -1;
	DIR *pdir= NULL, *pSubDir= NULL;
	int recursive = 0;
	struct dirent *pdirent;

    pstIODriver = (IODRIVER_S *)ICA_calloc(1, sizeof(IODRIVER_S));
    if (NULL == pstIODriver)
    {
        LOG_ErrNo(SAR_MEMORYERR);
        return SAR_MEMORYERR;
    }

	pdir = opendir(path);
    if(pdir)
	{
        while(pdirent = readdir(pdir))
        {
            if((strcmp(pdirent->d_name, ".") == 0)
                || (strcmp(pdirent->d_name, "..") == 0))
            {
                continue;
            }

            sprintf(acPath, "%s/%s", path, pdirent->d_name);

			if(IS_DIR(acPath))
			{
				/* 已经检查过的设备 */
				if (iCnt != iDevNum)
				{
					iCnt++;
					continue;
				}

				sprintf(acDriver,"%s/cloudshield/", acPath);
				pSubDir = opendir(acDriver);
				if(pSubDir == NULL)
				{
					ICA_LogD("%s not exist\n", acDriver);
					closedir(pdir);
	                ICA_free(pstIODriver);
                    return SAR_OK;
				}
				else
				{
					ICA_LogD("%s exist\n", acDriver);
					closedir(pSubDir);
				}

				/* 打开设备文件 */
                ulRet = OpenDevFile(acDriver, &pstIODriver->stVirFile);
				if (SAR_OK != ulRet)
				{
					ICA_LogE("OpenDevFile err!");
					break;
				}

                ulRet = CreateIOBuf(&pstIODriver->pucCmdBuf, MAX_CMD_BUF);
                if (SAR_OK != ulRet)
                {
                    ICA_LogE("CreateIOBuf err!");
                    break;
                }

				*pHandle = pstIODriver;
                closedir(pdir);

				return SAR_OK;
			}
            //当acPath为目录并且recursive为1的时候递归处理子目录
            if(IS_DIR(acPath) && recursive)
            {
                List_Files_Core(acPath, recursive);
            }
        }
    }
    else
    {
        ICA_LogE("opendir error:%s", path);
        ulRet = SAR_INVALIDDEVPATH;
    }

    closedir(pdir);

	ICA_free(pstIODriver);

	return ulRet;
}
#endif

ULONG DEV_SetDevicePath(CHAR *pcDevPath)
{
    INT32 iPathLen = 0;

    /* 参数检查 */
    if (NULL == pcDevPath)
    {
        LOG_ErrNo(SAR_INVALIDPARAMERR);
        return SAR_INVALIDPARAMERR;
    }

    iPathLen = strlen(pcDevPath);
    if (PATH_MAX < iPathLen)
    {
        LOG_ErrNo(SAR_INDATALENERR);
        return SAR_INDATALENERR;
    }

    memset(s_acDevPath, 0x00, PATH_MAX);
    memcpy(s_acDevPath, pcDevPath, iPathLen);
    g_bDefinedPath = TRUE;

    return SAR_OK;
}

void DEV_ClearDevicePath()
{
    memset(s_acDevPath, 0x00, PATH_MAX);
    g_bDefinedPath = FALSE;
    return;
}

ULONG DEV_OpenDevice(INT32 iDevNum, HANDLE *pHandle)
{
    ULONG ulRet = SAR_OK;

    /* 参数检查 */
    if ((0 > iDevNum) || (MAX_DEV_CNT <= iDevNum) || (NULL == pHandle))
    {
        LOG_ErrNo(SAR_INVALIDPARAMERR);
        return SAR_INVALIDPARAMERR;
    }

    if (g_bDefinedPath)
    {
        ulRet = OpenDefinedDev(pHandle);
    }
    else
    {
    #if defined(WIN32)
        ulRet = OpenWin32Dev(iDevNum, pHandle);
    #elif defined(LINUX)
        ulRet = OpenLinuxDev(iDevNum, pHandle);
    #elif defined(ANDROID) || defined(SYNO_ASIDE)
        ulRet = OpenDefinedDev(pHandle);
    #else
        LOG_ErrNo(SAR_NOTSUPPORT_SYS);
        ulRet = SAR_NOTSUPPORT_SYS;
    #endif
    }

    return ulRet;
}

void DEV_CloseDevice(HANDLE pHandle)
{
    IODRIVER_S *pstIODriver = NULL;

    /* 参数检查 */
    if(NULL == pHandle)
	{
        LOG_ErrNo(SAR_INVALIDPARAMERR);
		return;
	}

    pstIODriver = (IODRIVER_S *)pHandle;
    CloseDevFile(&pstIODriver->stVirFile);
    ICA_free(pstIODriver->pucCmdBuf);
    ICA_free(pstIODriver->pucDataBuf);
    ICA_free(pstIODriver);

    return;
}

ULONG DEV_Write(RWDEVICE_S *pstRWDev, ULONG ulOffset, UCHAR *pucBuf, ULONG *pulDataLen)
{
    INT32 iFd = 0;
    UCHAR *pucWBuf = NULL;
    INT32 iRetLen = 0;
	LARGE_INTEGER li = {0};

    FUNC_BEGIN;

    /* 参数检查 */
    if ((NULL == pstRWDev) || (NULL == pucBuf))
    {
        LOG_ErrNo(SAR_INVALIDPARAMERR);
        return SAR_INVALIDPARAMERR;
    }
	if (NULL == pulDataLen || 0 == *pulDataLen)
    {
        LOG_ErrNo(SAR_INVALIDPARAMERR);
        return SAR_INVALIDPARAMERR;
    }

    iFd = pstRWDev->iRWFd;
    pucWBuf = pstRWDev->pucIOBuf;
    if ((0 == iFd) || (NULL == pucWBuf))
    {
        LOG_ErrNo(SAR_INVALIDPARAMERR);
        return SAR_INVALIDPARAMERR;
    }

    memcpy(pucWBuf, pucBuf, *pulDataLen);

    //_LSEEK(iFd, ulOffset);
	li.QuadPart = ulOffset;
	SetFilePointer((HANDLE)iFd, li.LowPart, &li.HighPart, FILE_BEGIN);
    ICA_LogD("_WRITE ulBufLen = %d", *pulDataLen);
	iRetLen = _WRITE(iFd, pucWBuf, *pulDataLen);
	if (iRetLen <= 0)
	{
		*pulDataLen = 0;
        LOG_ErrNo(SAR_FILEERR);
        //ICA_LogE("write device file err errno=<%d>,<%s>", errno, strerror(errno));
		return SAR_FILEERR;
	}
	*pulDataLen = iRetLen;

    FUNC_END;

	return SAR_OK;
}

ULONG DEV_Read(RWDEVICE_S *pstRWDev, ULONG ulOffset, UCHAR *pucBuf, ULONG *pulDataLen)
{
    INT32 iFd = 0;
    UCHAR *pucWBuf = NULL;
    INT32 iRetLen = 0;

    FUNC_BEGIN;

    /* 参数检查 */
    if ((NULL == pstRWDev) || (NULL == pucBuf))
    {
        LOG_ErrNo(SAR_INVALIDPARAMERR);
        return SAR_INVALIDPARAMERR;
    }
	if (NULL == pulDataLen || 0 == *pulDataLen)
	{
        LOG_ErrNo(SAR_INVALIDPARAMERR);
        return SAR_INVALIDPARAMERR;
	}

    iFd = pstRWDev->iRWFd;
    pucWBuf = pstRWDev->pucIOBuf;
    if ((0 == iFd) || (NULL == pucWBuf))
    {
        LOG_ErrNo(SAR_INVALIDPARAMERR);
        return SAR_INVALIDPARAMERR;
    }

    _LSEEK(iFd, ulOffset);

    iRetLen = _READ(iFd, pucWBuf, *pulDataLen);
    if (0 < iRetLen)
    {
        memcpy(pucBuf, pucWBuf, iRetLen);
		*pulDataLen = iRetLen;
    }
    else
    {
		*pulDataLen = 0;
        LOG_ErrNo(SAR_FILEERR);
        //ICA_LogE("read device file err errno=<%d>,<%s>", errno, strerror(errno));
		return SAR_FILEERR;
    }

    FUNC_END;

    return SAR_OK;
}
#if 0
ULONG DEV_CmdWrite(RWDEVICE_S *pstRWDev, UCHAR *pucCmd, ULONG ulCmdLen,
    UCHAR *pucData, ULONG ulDataLen)
{
    ULONG ulRet = SAR_OK;
    UCHAR aucBuf[MAX_CMD_BUF] = {0};

    /* 参数检查 */
    if ((NULL == pstRWDev) || (NULL == pucCmd) || (NULL == pucData))
    {
        LOG_ErrNo(SAR_INVALIDPARAMERR);
        return SAR_INVALIDPARAMERR;
    }

    memcpy(aucBuf, pucCmd, ulCmdLen);

    if (0 < ulDataLen)
    {
        memcpy(aucBuf + ulCmdLen, pucData, ulDataLen);
    }

    ulRet = DEV_Write(pstRWDev, aucBuf, MAX_CMD_BUF);

	return ulRet==0? SAR_OK : SAR_FILEERR;
}

ULONG DEV_CmdRead(RWDEVICE_S *pstRWDev, UCHAR *pucData, ULONG *pulDataLen)
{
	ULONG ulRes = 0;
    ULONG ulDataLen = MAX_CMD_BUF;
    UCHAR aucBuf[MAX_CMD_BUF] = {0};

    /* 参数检查 */
    if ((NULL == pstRWDev) || (NULL == pucData) || (NULL == pulDataLen))
    {
        LOG_ErrNo(SAR_INVALIDPARAMERR);
        return SAR_INVALIDPARAMERR;
    }

    ulRes = DEV_Read(pstRWDev, aucBuf, &ulDataLen);
    if (SAR_OK != ulRes)
    {
        LOG_ErrNo(SAR_FILEERR);
        return SAR_FILEERR;
    }

	if (*pulDataLen < ulDataLen)
	{
        LOG_ErrNo(SAR_BUFFER_TOO_SMALL);
		return SAR_BUFFER_TOO_SMALL;
	}

    memcpy(pucData, aucBuf, ulDataLen);
    *pulDataLen = ulDataLen;

    return SAR_OK;
}
#endif

