/***************************************************************************
 *	File name:Common.c
 *	Introduce:The implmenet file for common functions
 *	Author:Syno common
 *	Date:2015/07/18
 *  Version:v1.0
 **************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include "common.h"

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

#if 0
/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/
#define MAX_LOG 512
#define MAX_CTIME 30

static int s_iLogOutPut = 0;

static int s_iWriteLogFile = 0;

//static char s_acLogFilePath[PATH_MAX] = {0};

static char s_acLogFilePath[PATH_MAX] = "mnt/sdcard/ica_log.txt";


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

#ifndef _LOG_ENABLED
        return;
#endif

    if (!s_iLogOutPut)
    {
#ifndef _LOG_ENABLED
        return;
#endif
    }

    va_start(stAp, pccFmt);
    vsnprintf(acOutPut, sizeof(acOutPut), pccFmt, stAp);

    printf("ThreadID:%d %s %d  %s\n", ciThreadID, pccFunc, ciLine, acOutPut);

    return;
}
#endif
#endif 0
/*****************************************************************************
 函 数 名  : GetErrorString
 功能描述  : 获取错误码代表的具体含义
 输入参数  : ulError 错误码
 输出参数  : 无
 返 回 值  : 错误码的具体含义

 修改历史      :
  1.日    期   : 2014年01月16日
    作    者   : 张晓垒
    修改内容   : 新生成函数

*****************************************************************************/
char *GetErrorString(unsigned int ulError)
{
    static char acStrError[512] = {0};

	switch(ulError)
	{
    	case SAR_OK:
    		strcpy(acStrError,"command success!"); break;
    	case SAR_FAIL:
    		strcpy(acStrError,"command failed!");break;
    	case SAR_NOTSUPPORT_SYS:
    		strcpy(acStrError,"unsupport system!");break;
    	case SAR_FILEERR:
    		strcpy(acStrError,"file operation error!");break;
    	case SAR_INVALIDPARAMERR:
    		strcpy(acStrError,"invalide param!");break;
    	case SAR_NAMELENERR:
    		strcpy(acStrError,"name length error!");break;
    	case SAR_OBJERR:
    		strcpy(acStrError,"object error!");break;
    	case SAR_MEMORYERR:
    		strcpy(acStrError,"memroy error");break;
    	case SAR_INDATALENERR:
    		strcpy(acStrError,"input data len error");break;
    	case SAR_INDATAERR:
    		strcpy(acStrError,"input data error");break;
    	case SAR_BUFFER_TOO_SMALL:
    		strcpy(acStrError,"buffer is not enough");break;
    	case SAR_PIN_LEN_RANGE:
    		strcpy(acStrError,"PIN length error!");break;
        case SAR_DEVICEAUTHERR:
            strcpy(acStrError,"device auther error!");break;
        case SAR_INVALIDDEVNAME:
            strcpy(acStrError,"invalide device name error!");break;
        case SAR_INVALIDDEVPATH:
            strcpy(acStrError,"invalide device path error!");break;
    	default:
            sprintf(acStrError, "Unknown ErrNo(0x%x):", ulError);
	}

    return (char *)acStrError;
}


