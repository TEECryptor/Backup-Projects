/******************************************************************************

                  版权所有 (C), 2013-2023, 安徽云盾信息技术有限公司

 ******************************************************************************
  文 件 名   : SynoLog.h
  版 本 号   : 初稿
  作    者   : 张晓垒
  生成日期   : 2013年10月12日
  最近修改   : 2013年10月12日
  功能描述   : 公共文件
  函数列表   :
  修改历史   :
  1.日    期 : 2013年10月12日
    作    者 : 张晓垒
    修改内容 : 创建文件

******************************************************************************/

#ifndef _SYNO_LOG_H_
#define _SYNO_LOG_H_

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/

#ifdef WIN32
//#include <windows.h>
#else
#include <limits.h>
#include <stdarg.h>
#include <unistd.h>
#endif

#ifdef ANDROID
#include <android/log.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if defined(WIN32)
void ICA_LogFile(
    const char *pccFunc,
    const int   ciLine,
    const char *pccFmt,...);
#elif defined(ANDROID)
void ICA_LogFile(int iPrio,
    const char *pccFunc,
    const int   ciLine,
    const char *pccFmt, ...);
#elif defined(SYNO_ASIDE)
void ICA_LogFile(
    const char *pccFunc,
    const int   ciLine,
    const char *pccFmt,...);
#elif defined(LINUX)
void ICA_LogFile(
    const char *pccFunc,
    const int   ciLine,
    const char *pccFmt, ...);
#endif

char *GetErrorString(unsigned int ulError);

#ifdef __cplusplus
}
#endif

/*----------------------------------------------*
 * 关于日志宏                                   *
 *----------------------------------------------*/
#if defined(WIN32)
#define ICA_LogI(...)           ICA_LogFile(__FUNCTION__, __LINE__, __VA_ARGS__)

#define ICA_LogD(...)           ICA_LogFile(__FUNCTION__, __LINE__, __VA_ARGS__)

#define ICA_LogE(...)           ICA_LogFile(__FUNCTION__, __LINE__, __VA_ARGS__)
#elif defined(ANDROID)
#define ICA_LogI(...)           ICA_LogFile(ANDROID_LOG_INFO, \
    __FUNCTION__, __LINE__, __VA_ARGS__)

#define ICA_LogD(...)           ICA_LogFile(ANDROID_LOG_DEBUG, \
    __FUNCTION__, __LINE__, __VA_ARGS__)

#define ICA_LogE(...)           ICA_LogFile(ANDROID_LOG_ERROR, \
    __FUNCTION__, __LINE__, __VA_ARGS__)
#elif defined(SYNO_ASIDE)
#define ICA_LogI(...)           ICA_LogFile(__FUNCTION__, __LINE__, __VA_ARGS__)

#define ICA_LogD(...)           ICA_LogFile(__FUNCTION__, __LINE__, __VA_ARGS__)

#define ICA_LogE(...)           ICA_LogFile(__FUNCTION__, __LINE__, __VA_ARGS__)
#elif defined(LINUX)
#define ICA_LogI(...)           ICA_LogFile(__FUNCTION__, __LINE__, ##__VA_ARGS__)

#define ICA_LogD(...)           ICA_LogFile(__FUNCTION__, __LINE__, ##__VA_ARGS__)

#define ICA_LogE(...)           ICA_LogFile(__FUNCTION__, __LINE__, ##__VA_ARGS__)
#endif

#define FUNC_BEGIN              ICA_LogI("begin!");

#define FUNC_END                ICA_LogI("end!");

#define SAR_ErrToStr(Err)       GetErrorString(Err)

#define LOG_ErrNo(Err)          ICA_LogE("%s", SAR_ErrToStr(Err))

/*----------------------------------------------*
 * 关于内存操作宏                               *
 *----------------------------------------------*/
#define ICA_free(handle) do{if(NULL == (handle))break; free(handle); (handle) = NULL;}while(0)

#define ICA_calloc(nelem, elsize) calloc(nelem, elsize)

#define ICA_malloc(size) malloc(size)

#endif /* _SYNO_LOG_H_ */


