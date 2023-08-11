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
 * ����ͷ�ļ�                                   *
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
#define MAX_DATA_BUF                   (1024*15) /* �ļ�IO����󻺳��� */
#endif
#endif

/*----------------------------------------------*
 * ������붨��                                 *
 *----------------------------------------------*/
#define SAR_COS_OK									0x9000	//�ɹ�
#define	SAR_COS_PIN_INVALID							0x63C0	//��֤ʧ��,��ʣX�λ���(XΪ���һλֵ���趯̬���)
#define	SAR_COS_WRITEFILEERR						0x6581	//д�ļ�ʧ��
#define	SAR_COS_INDATALENERR						0x6700	//LC���ȴ���
#define SAR_COS_GENRSAKEYERR                        0x6915  //������Կ��ʧ��
#define	SAR_COS_SASTATUSERR							0x6982	//�����㰲ȫ״̬ 
#define	SAR_COS_PIN_LOCKED							0x6983	//��֤��������
#define	SAR_COS_INDATAERR							0x6984	//����������Ч
#define	SAR_COS_INVCONDERR							0x6985	//ʹ������������
#define	SAR_COS_INVALID_CMD							0x6986	//���������
#define	SAR_COS_INVALIDSM_OBJECT					0x6988	//SM���ݶ�����ȷ
#define	SAR_COS_APPLICATION_NOT_OPEN				0x698A	//û�д�Ӧ��
#define	SAR_COS_INVALIDDATA							0x6A80	//��������ȷ(��PIN����ܵĲ������) 
#define	SAR_COS_UNSUPPORTFUNC						0x6A81	//���ܲ���֧��
#define	SAR_COS_NO_ROOM								0x6A84	//�豸�ռ䲻��   
#define	SAR_COS_INVALIDPARAMERR						0x6A86	//P1P2��������ȷ 
#define	SAR_COS_APPLICATION_NOT_EXISTS				0x6A88	//ָ����Ӧ�ò�����
#define	SAR_COS_APPLICATION_EXISTS					0x6A89	//Ӧ���Ѿ����� 
#define	SAR_COS_APPLICATION_ALREADY_OPEN			0x6A8A	//Ӧ���Ѵ�  	  
#define	SAR_COS_APPLICATIONNAME_NOT_EXISTS			0x6A8B	//���õ�Ӧ����δ�ҵ� 
#define	SAR_COS_SYMM_KEY_NOT_EXISTS					0x6A8C	//�Գ���Կ������
#define	SAR_COS_DATA_ERR							0x6A8D	//���ݴ���
#define	SAR_COS_UNSUPPORT_MULTI_APP					0x6A90	//�Ѵ�Ӧ�ã���֧�ֶ��Ӧ��ͬʱ��
#define	SAR_COS_CONTAINERNAME_NOT_EXISTS			0x6A91	//ָ����������������
#define	SAR_COS_FILE_ALREADY_EXIST					0x6A92	//�ļ��Ѵ���
#define	SAR_COS_FILE_NOT_EXIST						0x6A93	//�ļ�������
#define	SAR_COS_CONTAINERID_NOT_EXISTS				0x6A94	//ָ��������ID������ 
#define	SAR_COS_KEYNOTFOUNTERR						0x6A95	//������û�ж�Ӧ����Կ���ļ�
#define	SAR_COS_CERTNOTFOUNTERR						0x6A96	//ָ�����͵�֤�鲻����
#define	SAR_COS_WRITEDATAERR						0x6A97	//����д��ʧ��
#define	SAR_COS_SIGNVERYFIERR						0x6A98	//��֤ǩ��ʧ��/��Կ����ʧ��
#define	SAR_COS_UNSUPPORT_SESSION_KEY				0x6A99	//��֧�ֵĻỰ��Կ�㷨��ʶ
#define	SAR_COS_ASYMM_ENC_FAIL						0x6A9A	//�ǶԳƼ���ʧ��
#define	SAR_COS_ASYMM_DEC_FAIL						0x6A9B	//�ǶԳƽ���ʧ��
#define	SAR_COS_PRI_KEY_SIGN_FAIL					0x6A9C	//˽Կǩ��ʧ��
#define	SAR_COS_UNSUPPORT_HASH_ALG					0x6A9D	//��֧��ժҪ�㷨
#define	SAR_COS_NEED_MORE_DATA						0x6A9E	//���и��������ϴ�
#define	SAR_COS_KEY_NOT_EXISTS						0x6A9F	//���õ���Կδ�ҵ�
#define	SAR_COS_FILEOFFSETERR						0x6B00	//ƫ���������ļ�����
#define	SAR_COS_KEY_EXCHANGE_DATA_FAIL				0x6B01	//������ԿЭ������ʧ��
#define	SAR_COS_GEN_KEY_EXCHANGE_FAIL				0x6B02	//����Э����Կʧ��
#define	SAR_COS_OUTDATALENERR						0x6C00	//LE����ȷ
#define	SAR_COS_CLAERR								0x6E00	//CLA����ȷ
#define	SAR_COS_READSIZE_OVERFLOW					0x6E01	//�����ص����ݳ��ȳ�����󻺳������� 
#define	SAR_COS_CONTAINER_ALREADY_EXISTS			0x6E02	//ָ���������Ѿ�����
//
#define SAR_OK					      0x00000000 /* �ɹ� */
#define SAR_FAIL				      0x0B000001 /* ʧ�� */
#define SAR_NOTSUPPORT_SYS	          0x0B000002 /* ��֧�ֵ�ϵͳƽ̨ */
#define SAR_FILEERR				      0x0B000003 /* �ļ��������� */
#define SAR_INVALIDPARAMERR		      0x0B000004 /* ��Ч�Ĳ��� */
#define SAR_NAMELENERR			      0x0B000005 /* ���Ƴ��ȴ��� */
#define SAR_OBJERR                    0x0B000006 /* ������� */
#define SAR_MEMORYERR		          0x0B000007 /* �ڴ���� */
#define SAR_INDATALENERR		      0x0B000008 /* �������ݳ��ȴ��� */
#define SAR_INDATAERR			      0x0B000009 /* �������ݴ��� */
#define SAR_BUFFER_TOO_SMALL	      0x0B00000A /* ���������� */
#define SAR_PIN_LEN_RANGE             0x0B00000B /* PIN���ȴ��� */
#define SAR_DEVICEAUTHERR             0x0B00000C /* �豸��֤ʧ�� */
#define SAR_INVALIDDEVNAME            0x0B00000D /* ��Ч���豸�� */
#define SAR_INVALIDDEVPATH            0x0B00000E /* ��Ч���豸·�� */
#define SAR_USER_NOT_LOGGED_IN        0x0A00002D /* �û�û�е�¼ */
#define SAR_PIN_INVALID				  0x0A000026 /* PIN��Ч */
#define SAR_MODULUSLENERR	          0x0A00000B /* ģ�ĳ��ȴ��� */
#define SAR_OPENFILEERR				  0x0B000101 /* ���ļ����� */
#define SAR_WRITEFILEERR			  0x0B000102 /* д�ļ����� */
#define SAR_READFILEERR				  0x0B000103 /* ���ļ����� */
#define SAR_DELFILEERR				  0x0B000104 /* ɾ���ļ����� */
#define SAR_RENAMEFILEERR			  0x0B000105 /* �ļ����������� */
#define SAR_FILEENCRPYTED			  0x0B000106 /* �ļ��Ѽ��� */
#define SAR_FILENOENCRPYT			  0x0B000107 /* �ļ�δ���� */
#define SAR_FILESIZEERR				  0x0B000108 /* �ļ���С���� */
#define SAR_NOFINDKEYERR			  0x0B000109 /* δ�ҵ�����KEY */
#define SAR_FILE_ENCRYPT_CANCEL	      0x0B000110  /*���ܹ�����ȡ������*/
#define SAR_FILE_DENCRYPT_CANCEL	  0x0B000110  /*���ܹ�����ȡ������*/
#define SAR_DEVICEDISCONNECT		  0x0B00010C /* �豸�ѶϿ� */
#define SAR_NOTLOGIN                  0x0B00010D /* �û�û�е�¼ */
#define SAR_CREATEDIRFAIL             0x0B00010E /* �����ļ���ʧ�� */
#define SAR_COPYFILEFAIL              0x0B00010F /* �����ļ�ʧ�� */
#define SAR_OPERATION_CANCEL	      0x0B000110 /* ����ȡ�� */
#define SAR_DEV_STATE_ERR		      0x0B000111 /* �豸״̬���� */
#define SAR_NOFIND_LICENSE            0x0B000112 /* δ�ҵ���Ȩ�ļ� */
#define SAR_VERIFY_ACTIVATION_FAIL    0x0B000113 /* ��֤����ʧ�� */
#define SAR_SOFT_VERSION_ERR          0x0B000114 /* ����汾��֤���� */
#define SAR_LOSE_EFFICACY_ERR         0x0B000114 /* �������ʧЧ */

#if 0
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
 * ������־��                                   *
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
 * �����ڴ������                               *
 *----------------------------------------------*/
#define ICA_free(handle) do{if(NULL == (handle))break; free(handle); (handle) = NULL;}while(0)

#define ICA_calloc(nelem, elsize) calloc(nelem, elsize)

#define ICA_malloc(size) malloc(size)
#endif//0
#endif /* _COMMON_H_ */


