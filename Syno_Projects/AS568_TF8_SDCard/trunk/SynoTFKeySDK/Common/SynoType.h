/***************************************************************************
 *	File name:SynoMemory.h
 *	Introduce:Syno common data type definition
 *	Author:Syno common
 *	Date:2015/11/04
 *  Version:v1.0
 **************************************************************************/

#ifndef _SYNO_TYPE_H_
#define _SYNO_TYPE_H_


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

#endif /* _SYNO_TYPE_H_ */


