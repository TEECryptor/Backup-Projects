/***************************************************************************
 *	File name:SynoLog.h
 *	Introduce:Syno common log functions and macro definition
 *	Author:Syno common
 *	Date:2015/11/04
 *  Version:v1.0
 **************************************************************************/

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
void SYNO_LogFile(
    const char *pccFunc,
    const int   ciLine,
    const char *pccFmt,...);
#elif defined(ANDROID)
void SYNO_LogFile(int iPrio,
    const char *pccFunc,
    const int   ciLine,
    const char *pccFmt, ...);
#elif defined(SYNO_ASIDE)
void SYNO_LogFile(
    const char *pccFunc,
    const int   ciLine,
    const char *pccFmt,...);
#elif defined(LINUX)
void SYNO_LogFile(
    const char *pccFunc,
    const int   ciLine,
    const char *pccFmt, ...);
#endif

#ifdef __cplusplus
}
#endif

/*----------------------------------------------*
 * 关于日志宏                                   *
 *----------------------------------------------*/
#if defined(WIN32)
#define SYNO_LogI(...)           SYNO_LogFile(__FUNCTION__, __LINE__, __VA_ARGS__)

#define SYNO_LogD(...)           SYNO_LogFile(__FUNCTION__, __LINE__, __VA_ARGS__)

#define SYNO_LogE(...)           SYNO_LogFile(__FUNCTION__, __LINE__, __VA_ARGS__)
#elif defined(ANDROID)
#define SYNO_LogI(...)           SYNO_LogFile(ANDROID_LOG_INFO, \
    __FUNCTION__, __LINE__, __VA_ARGS__)

#define SYNO_LogD(...)           SYNO_LogFile(ANDROID_LOG_DEBUG, \
    __FUNCTION__, __LINE__, __VA_ARGS__)

#define SYNO_LogE(...)           SYNO_LogFile(ANDROID_LOG_ERROR, \
    __FUNCTION__, __LINE__, __VA_ARGS__)
#elif defined(SYNO_ASIDE)
#define SYNO_LogI(...)           SYNO_LogFile(__FUNCTION__, __LINE__, __VA_ARGS__)

#define SYNO_LogD(...)           SYNO_LogFile(__FUNCTION__, __LINE__, __VA_ARGS__)

#define SYNO_LogE(...)           SYNO_LogFile(__FUNCTION__, __LINE__, __VA_ARGS__)
#elif defined(LINUX)
#define SYNO_LogI(...)           SYNO_LogFile(__FUNCTION__, __LINE__, ##__VA_ARGS__)

#define SYNO_LogD(...)           SYNO_LogFile(__FUNCTION__, __LINE__, ##__VA_ARGS__)

#define SYNO_LogE(...)           SYNO_LogFile(__FUNCTION__, __LINE__, ##__VA_ARGS__)
#endif

#define FUNC_BEGIN              SYNO_LogI("begin!");

#define FUNC_END                SYNO_LogI("end!");
  
#define LOG_ErrNo(Err)          SYNO_LogE("%s", SAR_ErrToStr(Err))

#endif	//_SYNO_LOG_H_

