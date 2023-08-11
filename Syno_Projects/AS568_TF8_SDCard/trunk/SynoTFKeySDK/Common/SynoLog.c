/***************************************************************************
 *	File name:SynoError.c
 *	Introduce:The implement file for Syno log functions
 *	Author:Syno common
 *	Date:2015/11/04
 *  Version:v1.0
 **************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include "SynoLog.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>

#ifdef ANDROID
#include <android/log.h>
#endif

#ifdef WIN32
#include <windows.h>
#endif


/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/
#define MAX_LOG 512
#define MAX_CTIME 30

static int s_iLogOutPut = 0;

static int s_iWriteLogFile = 0;

#ifdef ANDROID
static char s_acLogFilePath[MAX_PATH] = "mnt/sdcard/syno_log.txt";
#elif defined(WIN32)
#define	SYNO_REGKEY_PATH "SOFTWARE\\Syno"
#define	SYNO_REGKEY_LOGTYPE "LogType"
#define	SYNO_REGKEY_LOGPATH "LogPath"
static int s_iLogType = -1;	//-1:unknow;0:No log;1:print log in console;2:print log in file
static char s_acLogFilePath[MAX_PATH] = {0};
static FILE* s_pLogFile = NULL;
static CRITICAL_SECTION s_csLock = {0};
#endif	//


/*----------------------------------------------*
 * 接口实现                                     *
 *----------------------------------------------*/

#if defined(ANDROID)
void SYNO_LogFile(int iPrio,
    const char *pccFunc,
    const int   ciLine,
    const char *pccFmt,...)
{
    char acOutPut[MAX_LOG] = {0};
    char acLogFilePath[PATH_MAX] = {0};
    char acTimeStamped[MAX_LOG] = {0};
    char acTimeStamp[MAX_CTIME] = {0};
    FILE *pstLogf = NULL;
    struct tm stTimeInfo = {0};
    time_t lNow = 0;
    va_list stAp = {0};

    s_iLogOutPut = 1;
    s_iWriteLogFile = 1;

    if (!s_iLogOutPut)
    {
        return;
    }

    va_start(stAp, pccFmt);
    vsnprintf(acOutPut, sizeof(acOutPut), pccFmt, stAp);

    if (s_iWriteLogFile)
    {
        /* 输出时间 */
        lNow = time(NULL);
        localtime_r(&lNow, &stTimeInfo);
        strftime(acTimeStamp, sizeof(acTimeStamp), "%H:%M:%S", &stTimeInfo);

        snprintf(acTimeStamped, sizeof(acTimeStamped), "%s %s %d %d  %s",
            acTimeStamp, pccFunc, ciLine, iPrio, acOutPut);

        pstLogf = fopen(s_acLogFilePath, "a");
        if (pstLogf != NULL)
        {
            fprintf(pstLogf, "%s\n", acTimeStamped);
            fclose(pstLogf);
        }
    }
    else
    {
        __android_log_print(iPrio, "ICA", "%s %d  %s",
            pccFunc, ciLine, acOutPut);
    }

    return;
}

#elif defined(SYNO_ASIDE)

extern print(const char * __format, ...);

void SYNO_LogFile(
    const char *pccFunc,
    const int   ciLine,
    const char *pccFmt,...)
{
#if 0
    char acOutPut[MAX_LOG];
    va_list stAp;

    if (!s_iLogOutPut)
    {
        //return;
    }

    va_start(stAp, pccFmt);
    vsprintf(acOutPut, pccFmt, stAp);

    print("%s %d  %s\r\n", pccFunc, ciLine, acOutPut);
#endif
    return;
}

#elif defined(LINUX)
void SYNO_LogFile(
    const char *pccFunc,
    const int   ciLine,
    const char *pccFmt,...)
{
#if 0

    char acOutPut[MAX_LOG] = {0};
    va_list stAp = {0};

    if (!s_iLogOutPut)
    {
        return;
    }

    va_start(stAp, pccFmt);
    vsnprintf(acOutPut, sizeof(acOutPut), pccFmt, stAp);

    printf("%s %d  %s\n", pccFunc, ciLine,  acOutPut);
#endif
    return;
}

#elif defined(WIN32)

void SYNO_LogFile(
    const char *pccFunc,
    const int   ciLine,
    const char *pccFmt,...)
{
    char acOutPut[MAX_LOG] = {0};
    va_list stAp = {0};

	// read log setting from registry
	if (s_iLogType == -1)
	{
		HKEY hSynoKey = NULL;
		ULONG ulValue = 0;
		ULONG ulValueLen = MAX_PATH;

		s_iLogType = 0;
		if (ERROR_SUCCESS == RegOpenKeyExA(HKEY_LOCAL_MACHINE, SYNO_REGKEY_PATH, 0, KEY_READ, &hSynoKey))
		{
			ulValueLen = 4;
			RegQueryValueExA(hSynoKey, SYNO_REGKEY_LOGTYPE, NULL, NULL, (LPBYTE)&s_iLogType, &ulValueLen);
			ulValueLen = MAX_PATH;
			if (ERROR_SUCCESS != RegQueryValueExA(hSynoKey, SYNO_REGKEY_LOGPATH, NULL, NULL, (LPBYTE)s_acLogFilePath, &ulValueLen) ||
				strlen(s_acLogFilePath) == 0)
			{
				s_iLogType = 0;
			}
			else
			{
				strcat_s(s_acLogFilePath, MAX_PATH, "\\SynoLog.txt");
			}
			RegCloseKey(hSynoKey);
			hSynoKey = NULL;
		}
	}

	// no log
	if (s_iLogType == 0)
	{
		return;
	}

    va_start(stAp, pccFmt);
    vsnprintf_s(acOutPut, 512, sizeof(acOutPut), pccFmt, stAp);

	// print out
	if (s_iLogType == 1)
	{
		printf("[%s()][%d]  %s\n", pccFunc, ciLine, acOutPut);
	}
	// print to file
	else if (s_iLogType == 2)
	{
		SYSTEMTIME st;
		char* pcsLog = NULL;
		int iLogLen = strlen(acOutPut) + strlen(pccFunc) + 64;
		pcsLog = (char*)malloc(iLogLen);
		memset(pcsLog, 0, iLogLen);
		GetLocalTime(&st);
		sprintf_s(pcsLog, iLogLen, "[%02d:%02d:%02d][%s()][%d] %s\n", st.wHour, st.wMinute, st.wSecond, pccFunc, ciLine, acOutPut);
		if (!s_pLogFile)
		{
			fopen_s(&s_pLogFile, s_acLogFilePath, "a+");
			if (s_pLogFile)
			{
				const char startLine1[] = {"/****************************************************************/\n"};
				const char startLine2[] = {"/*                        Syno Log Start                        */\n"};
				const char startLine3[] = {"/****************************************************************/\n"};
				fwrite(startLine1, 1, strlen(startLine1), s_pLogFile);
				fwrite(startLine2, 1, strlen(startLine2), s_pLogFile);
				fwrite(startLine3, 1, strlen(startLine3), s_pLogFile);
				fflush(s_pLogFile);
				InitializeCriticalSection(&s_csLock);
			}
		}
		if (s_pLogFile)
		{
			EnterCriticalSection(&s_csLock);
			fwrite(pcsLog, 1, strlen(pcsLog), s_pLogFile);
			fflush(s_pLogFile);
			LeaveCriticalSection(&s_csLock);
		}
		free(pcsLog);
		pcsLog = NULL;
	}

    return;
}
#endif

