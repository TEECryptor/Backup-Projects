/******************************************************************************

                  版权所有 (C), 2013-2020, 安徽云盾信息技术有限公司

 ******************************************************************************
  文 件 名   : SynoLog.c
  版 本 号   : 初稿
  作    者   : 张晓垒
  生成日期   : 2013年10月12日
  最近修改   : 2013年10月12日
  功能描述   :
  函数列表   :
  修改历史   :
  1.日    期 : 2013年10月12日
    作    者 : 张晓垒
    修改内容 : 创建文件

******************************************************************************/

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

static char s_acLogFilePath[MAX_PATH] = "mnt/sdcard/ica_log.txt";


/*----------------------------------------------*
 * 接口实现                                     *
 *----------------------------------------------*/

#if defined(ANDROID)
void ICA_LogFile(int iPrio,
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

void ICA_LogFile(
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
void ICA_LogFile(
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

void ICA_LogFile(
    const char *pccFunc,
    const int   ciLine,
    const char *pccFmt,...)
{
    char acOutPut[MAX_LOG] = {0};
    va_list stAp = {0};
	int ciThreadID = GetCurrentThreadId();

#ifndef _DEBUG
        return;
#endif

    if (!s_iLogOutPut)
    {
#ifndef _DEBUG
        return;
#endif
    }

    va_start(stAp, pccFmt);
    vsnprintf(acOutPut, sizeof(acOutPut), pccFmt, stAp);

    printf("ThreadID:%d %s %d  %s\n", ciThreadID, pccFunc, ciLine, acOutPut);

    return;
}
#endif

