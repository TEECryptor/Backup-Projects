/*-------------------------------------------------------------------------
 *	���ƣ�	vgszip.h
 *	
 *	��飺	�����DLL�Ľ��뷽��
--------------------------------------------------------------------------*/
#ifndef _VGSZIP_H_
#define _VGSZIP_H_
//-------------------------------------------------------------------------
#ifndef UInt32
#ifdef _VGSZIP_UINT32_IS_ULONG
#define UInt32 unsigned long
#else
#define UInt32 unsigned int
#endif
#endif

#ifdef _VGSZIP_PROB32
#define CProb UInt32
#else
#define CProb unsigned short
#endif

#define VGSZIP_RESULT_OK 0
#define VGSZIP_RESULT_DATA_ERROR 1
#define VGSZIP_RESULT_NOT_ENOUGH_MEM 2

#ifdef _VGSZIP_IN_CB
typedef struct _IVgszipInCallback
{
  int (*Read)(void *object, unsigned char **buffer, UInt32 *bufferSize);
} IVgszipInCallback;
#endif

#define VGSZIP_BASE_SIZE 1846
#define VGSZIP_LIT_SIZE 768
//------------------------------------------------------------------------
//	���ƣ�	vgszipDeCode
//	��飺	��vgszip��ѹ���ļ����н���
//	������	pszvgszipFileName:i:Ҫ�����vgszip�ļ�
//			pszDestFileName:i:�������ļ���
//	���أ�	�ɹ�����0�����򷵻�-1
//-----------------------------------------------------------------------
extern "C" _declspec(dllexport) int vgszipDeCode(const char* pszvgszipFileName,
											   const char* pszDestFileName);
//------------------------------------------------------------------------
//	���ƣ�	vgszipGetDeCodeFileSize
//	��飺	����ָ��ѹ���ļ���ѹ��Ĵ�С
//	������	pszvgszipFileName:i:Ҫ�����vgszip�ļ�
//	���أ�	�ɹ����ؽ�ѹ����ļ���С�����򷵻�-1
//-----------------------------------------------------------------------
extern "C" _declspec(dllexport) long vgszipGetDeCodeFileSize(const char* pszvgszipFileName);
//-----------------------------------------------------------------------
//	���ƣ�	vgszipGetDeCodeBuff
//	��飺	��ָ����ѹ���ļ����ȡ��ѹ�������
//	������	pszvgszipFileName:i:Ҫ�����vgszip�ļ�
//			pDeCodeBuff:o:�������ļ���������buff
//	���أ�	�ɹ����ؽ������ļ���������buff�Ĵ�С�����򷵻�-1
//-----------------------------------------------------------------------
extern "C" _declspec(dllexport) long vgszipGetDeCodeBuff(const char* pszvgszipFileName,
													   unsigned char *pDeCodeBuff);
//------------------------------------------------------------------------
//	���ƣ�	vgszipGetDeCodeBuffSize
//	��飺	����ָ��ѹ���ļ���ѹ��Ĵ�С
//	������	pSrcCodeBuff:i:Ҫ�����vgszip�ļ���ʽ����
//          size:i:Ҫ�����vgszip�ļ���ʽ���ݴ�С
//	���أ�	�ɹ����ؽ�ѹ������ݴ�С�����򷵻�-1
//-----------------------------------------------------------------------
extern "C" _declspec(dllexport) long vgszipGetDeCodeBuffSize(unsigned char *pSrcCodeBuff, long size);
//-----------------------------------------------------------------------
//	���ƣ�	vgszipDeCodeFrmBuffToBuff
//	��飺	��ָ����ѹ���ļ����ȡ��ѹ�������
//	������	pSrcCodeBuff:i:Ҫ�����vgszip�ļ�����buff   
//          size:i:ԭBuff��С
//			pDeCodeBuff:o:�������ļ���������buff
//	���أ�	�ɹ����ؽ������ļ���������buff�Ĵ�С�����򷵻�-1
//-----------------------------------------------------------------------
extern "C" _declspec(dllexport) long vgszipDeCodeFrmBuffToBuff(unsigned char *pSrcCodeBuff, long size,
													         unsigned char *pDeCodeBuff);
//-------------------------------------------------------------------------
#endif //_VGSZIP_H_