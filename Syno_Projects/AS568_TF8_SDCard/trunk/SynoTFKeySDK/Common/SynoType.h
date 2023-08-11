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
 * ������������                                 *
 *----------------------------------------------*/

#ifndef INT8
typedef signed char                        INT8; /* �з���8λ���� */
#endif

#ifndef INT16
typedef signed short                      INT16; /* �з���16λ���� */
#endif

#ifndef INT32
typedef signed int                        INT32; /* �з���32λ���� */
#endif

#ifndef UINT8
typedef unsigned char                     UINT8; /* �޷���8λ���� */
#endif

#ifndef UINT16
typedef unsigned short                   UINT16; /* �޷���16λ���� */
#endif

#ifndef UINT32
typedef unsigned int                     UINT32; /* �޷���32λ���� */
#endif

#ifndef BOOL
typedef signed int                         BOOL; /* ��������,ȡֵΪTRUE��FALSE */
#endif

#ifndef BYTE
typedef UINT8                              BYTE; /* �ֽ�����,�޷���8λ���� */
#endif

#ifndef CHAR
typedef char                         CHAR; /* �ַ�����,�з���8λ���� */
#endif

#ifndef UCHAR
typedef unsigned char                     UCHAR; /* �ַ�����,�޷���8λ���� */
#endif

#ifndef SHORT
typedef INT16                             SHORT; /* ������,�з���16λ */
#endif

#ifndef SHORT
//typedef INT16                             SHORT; /* ������,�з���16λ */
#endif

#ifndef USHORT
typedef UINT16                           USHORT; /* ������,�޷���16λ���� */
#endif

#ifndef LONG
//typedef INT32                              LONG; /* ������,�з���32λ���� */
#endif

#ifndef ULONG
#ifdef _X64
typedef unsigned int                      ULONG; /* ������,�޷���32λ���� */
#else
typedef unsigned long                     ULONG; /* ������,�޷���32λ���� */
#endif
#endif

#ifndef UINT
typedef UINT32                             UINT; /* �޷���32λ���� */
#endif

#ifndef WORD
typedef UINT16                             WORD; /* ������,�޷���16λ���� */
#endif

#ifndef DWORD
typedef unsigned long                     DWORD; /* ˫������,�޷���32λ���� */
#endif

//#ifndef WCHAR
//typedef UINT16                            WCHAR; /* ˫������,�޷���32λ���� */
//#endif

#ifndef FLAGS
typedef UINT32                            FLAGS; /* ��־����,�޷���32λ���� */
#endif

#ifndef LPSTR
typedef CHAR *                            LPSTR; /* 8λ�ַ���ָ��,����UTF8��ʽ�洢������ */
#endif

#ifndef HANDLE
typedef void *                           HANDLE; /* ���,ָ���������ݶ������ʼ��ַ */
#endif

#ifndef NULL
#define NULL		                 0x00000000
#endif

#ifndef TRUE
#define TRUE		                  0x00000001 /* ����ֵΪ�� */
#endif

#ifndef FALSE
#define FALSE		                  0x00000000 /* ����ֵΪ�� */
#endif

#ifndef PATH_MAX
#define PATH_MAX                             512 /* ���·������ */
#endif


#ifndef MAX_BUF
#define MAX_BUF                            2048  /* ��󻺳��� */
#endif

#ifndef MIN_BUF
#define MIN_BUF                             512  /* ��С������ */
#endif

#ifndef MAX_CMD_BUF
#define MAX_CMD_BUF                     (1024*4) /* �ļ�IO����󻺳��� */
#endif

#if defined(SYNO_ASIDE)
#ifndef MAX_DATA_BUF
#define MAX_DATA_BUF                   (1024*2) /* �ļ�IO����󻺳��� */
#endif
#else
#ifndef MAX_DATA_BUF
#ifdef USE_4K_DATAFILE
#define MAX_DATA_BUF                   (1024*4) /* �ļ�IO����󻺳��� */
#else
#define MAX_DATA_BUF                   (1024*15) /* �ļ�IO����󻺳��� */
#endif	//USE_4K_DATAFILE
#endif
#endif

#endif /* _SYNO_TYPE_H_ */


