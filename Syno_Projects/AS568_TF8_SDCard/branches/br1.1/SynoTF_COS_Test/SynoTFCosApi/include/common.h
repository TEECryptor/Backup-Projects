/***************************************************************************
 *	File name:Common.h
 *	Introduce:Head file for common definition
 *	Author:Syno common
 *	Date:2015/07/18
 *  Version:v1.0
 **************************************************************************/

#ifndef _COMMON_H_
#define _COMMON_H_

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


/*----------------------------------------------*
 * 基本数据类型                                 *
 *----------------------------------------------*/

#ifndef INT8
typedef signed char                        INT8; /* 有符号8位整数 */
#endif

#ifndef INT16
typedef signed short                      INT16; /* 有符号16位整数 */
#endif

#ifndef INT32
typedef signed int                        INT32; /* 有符号32位整数 */
#endif

#ifndef UINT8
typedef unsigned char                     UINT8; /* 无符号8位整数 */
#endif

#ifndef UINT16
typedef unsigned short                   UINT16; /* 无符号16位整数 */
#endif

#ifndef UINT32
typedef unsigned int                     UINT32; /* 无符号32位整数 */
#endif

#ifndef BOOL
typedef signed int                         BOOL; /* 布尔类型,取值为TRUE或FALSE */
#endif

#ifndef BYTE
typedef UINT8                              BYTE; /* 字节类型,无符号8位整数 */
#endif

#ifndef CHAR
typedef char                         CHAR; /* 字符类型,有符号8位整数 */
#endif

#ifndef UCHAR
typedef unsigned char                     UCHAR; /* 字符类型,无符号8位整数 */
#endif

#ifndef SHORT
typedef INT16                             SHORT; /* 短整数,有符号16位 */
#endif

#ifndef SHORT
//typedef INT16                             SHORT; /* 短整数,有符号16位 */
#endif

#ifndef USHORT
typedef UINT16                           USHORT; /* 短整数,无符号16位整数 */
#endif

#ifndef LONG
//typedef INT32                              LONG; /* 长整数,有符号32位整数 */
#endif

#ifndef ULONG
#ifdef _X64
typedef unsigned int                      ULONG; /* 长整数,无符号32位整数 */
#else
typedef unsigned long                     ULONG; /* 长整数,无符号32位整数 */
#endif
#endif

#ifndef UINT
typedef UINT32                             UINT; /* 无符号32位整数 */
#endif

#ifndef WORD
typedef UINT16                             WORD; /* 字类型,无符号16位整数 */
#endif

#ifndef DWORD
typedef unsigned long                     DWORD; /* 双字类型,无符号32位整数 */
#endif

//#ifndef WCHAR
//typedef UINT16                            WCHAR; /* 双字类型,无符号32位整数 */
//#endif

#ifndef FLAGS
typedef UINT32                            FLAGS; /* 标志类型,无符号32位整数 */
#endif

#ifndef LPSTR
typedef CHAR *                            LPSTR; /* 8位字符串指针,按照UTF8格式存储及交换 */
#endif

#ifndef HANDLE
typedef void *                           HANDLE; /* 句柄,指向任意数据对象的起始地址 */
#endif

#ifndef NULL
#define NULL		                 0x00000000
#endif

#ifndef TRUE
#define TRUE		                  0x00000001 /* 布尔值为真 */
#endif

#ifndef FALSE
#define FALSE		                  0x00000000 /* 布尔值为假 */
#endif

#ifndef PATH_MAX
#define PATH_MAX                             512 /* 最大路径长度 */
#endif


#ifndef MAX_BUF
#define MAX_BUF                            2048  /* 最大缓冲区 */
#endif

#ifndef MIN_BUF
#define MIN_BUF                             512  /* 最小缓冲区 */
#endif

#ifndef MAX_CMD_BUF
#define MAX_CMD_BUF                     (1024*4) /* 文件IO的最大缓冲区 */
#endif

#if defined(SYNO_ASIDE)
#ifndef MAX_DATA_BUF
#define MAX_DATA_BUF                   (1024*2) /* 文件IO的最大缓冲区 */
#endif
#else
#ifndef MAX_DATA_BUF
#ifdef USE_4K_DATAFILE
#define MAX_DATA_BUF                   (1024*4) /* 文件IO的最大缓冲区 */
#else
#define MAX_DATA_BUF                   (1024*15) /* 文件IO的最大缓冲区 */
#endif	//USE_4K_DATAFILE
#endif
#endif

/*----------------------------------------------*
 * 错误代码定义                                 *
 *----------------------------------------------*/
#define SAR_OK					      0x00000000 /* 成功 */
#define SAR_FAIL				      0x0B000001 /* 失败 */
#define SAR_NOTSUPPORT_SYS	          0x0B000002 /* 不支持的系统平台 */
#define SAR_FILEERR				      0x0B000003 /* 文件操作错误 */
#define SAR_INVALIDPARAMERR		      0x0B000004 /* 无效的参数 */
#define SAR_NAMELENERR			      0x0B000005 /* 名称长度错误 */
#define SAR_OBJERR                    0x0B000006 /* 对象错误 */
#define SAR_MEMORYERR		          0x0B000007 /* 内存错误 */
#define SAR_INDATALENERR		      0x0B000008 /* 输入数据长度错误 */
#define SAR_INDATAERR			      0x0B000009 /* 输入数据错误 */
#define SAR_BUFFER_TOO_SMALL	      0x0B00000A /* 缓冲区不足 */
#define SAR_PIN_LEN_RANGE             0x0B00000B /* PIN长度错误 */
#define SAR_DEVICEAUTHERR             0x0B00000C /* 设备认证失败 */
#define SAR_INVALIDDEVNAME            0x0B00000D /* 无效的设备名 */
#define SAR_INVALIDDEVPATH            0x0B00000E /* 无效的设备路径 */
#define SAR_USER_NOT_LOGGED_IN        0x0A00002D /* 用户没有登录 */
#define SAR_PIN_INVALID				  0x0A000026 /* PIN无效 */
#define SAR_MODULUSLENERR	          0x0A00000B /* 模的长度错误 */
#define SAR_OPENFILEERR				  0x0B000101 /* 打开文件错误 */
#define SAR_WRITEFILEERR			  0x0B000102 /* 写文件错误 */
#define SAR_READFILEERR				  0x0B000103 /* 读文件错误 */
#define SAR_DELFILEERR				  0x0B000104 /* 删除文件错误 */
#define SAR_RENAMEFILEERR			  0x0B000105 /* 文件重命名错误 */
#define SAR_FILEENCRPYTED			  0x0B000106 /* 文件已加密 */
#define SAR_FILENOENCRPYT			  0x0B000107 /* 文件未加密 */
#define SAR_FILESIZEERR				  0x0B000108 /* 文件大小错误 */
#define SAR_NOFINDKEYERR			  0x0B000109 /* 未找到加密KEY */
#define SAR_FILE_ENCRYPT_CANCEL	      0x0B000110  /*加密过程中取消加密*/
#define SAR_FILE_DENCRYPT_CANCEL	  0x0B000110  /*解密过程中取消加密*/
#define SAR_DEVICEDISCONNECT		  0x0B00010C /* 设备已断开 */
#define SAR_NOTLOGIN                  0x0B00010D /* 用户没有登录 */
#define SAR_CREATEDIRFAIL             0x0B00010E /* 创建文件夹失败 */
#define SAR_COPYFILEFAIL              0x0B00010F /* 拷贝文件失败 */
#define SAR_OPERATION_CANCEL	      0x0B000110 /* 操作取消 */
#define SAR_DEV_STATE_ERR		      0x0B000111 /* 设备状态错误 */
#define SAR_NOFIND_LICENSE            0x0B000112 /* 未找到授权文件 */
#define SAR_VERIFY_ACTIVATION_FAIL    0x0B000113 /* 认证激活失败 */
#define SAR_SOFT_VERSION_ERR          0x0B000114 /* 软件版本认证错误 */
#define SAR_LOSE_EFFICACY_ERR         0x0B000114 /* 软件激活失效 */


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

#endif /* _COMMON_H_ */


