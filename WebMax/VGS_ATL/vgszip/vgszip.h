/*-------------------------------------------------------------------------
 *	名称：	vgszip.h
 *	
 *	简介：	定义该DLL的解码方法
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
//	名称：	vgszipDeCode
//	简介：	对vgszip的压缩文件进行解码
//	参数：	pszvgszipFileName:i:要解码的vgszip文件
//			pszDestFileName:i:解码后的文件名
//	返回：	成功返回0，否则返回-1
//-----------------------------------------------------------------------
extern "C" _declspec(dllexport) int vgszipDeCode(const char* pszvgszipFileName,
											   const char* pszDestFileName);
//------------------------------------------------------------------------
//	名称：	vgszipGetDeCodeFileSize
//	简介：	返回指定压缩文件解压后的大小
//	参数：	pszvgszipFileName:i:要解码的vgszip文件
//	返回：	成功返回解压后的文件大小，否则返回-1
//-----------------------------------------------------------------------
extern "C" _declspec(dllexport) long vgszipGetDeCodeFileSize(const char* pszvgszipFileName);
//-----------------------------------------------------------------------
//	名称：	vgszipGetDeCodeBuff
//	简介：	从指定的压缩文件里读取解压后的数据
//	参数：	pszvgszipFileName:i:要解码的vgszip文件
//			pDeCodeBuff:o:解码后的文件内容数据buff
//	返回：	成功返回解码后的文件内容数据buff的大小，否则返回-1
//-----------------------------------------------------------------------
extern "C" _declspec(dllexport) long vgszipGetDeCodeBuff(const char* pszvgszipFileName,
													   unsigned char *pDeCodeBuff);
//------------------------------------------------------------------------
//	名称：	vgszipGetDeCodeBuffSize
//	简介：	返回指定压缩文件解压后的大小
//	参数：	pSrcCodeBuff:i:要解码的vgszip文件格式数据
//          size:i:要解码的vgszip文件格式数据大小
//	返回：	成功返回解压后的数据大小，否则返回-1
//-----------------------------------------------------------------------
extern "C" _declspec(dllexport) long vgszipGetDeCodeBuffSize(unsigned char *pSrcCodeBuff, long size);
//-----------------------------------------------------------------------
//	名称：	vgszipDeCodeFrmBuffToBuff
//	简介：	从指定的压缩文件里读取解压后的数据
//	参数：	pSrcCodeBuff:i:要解码的vgszip文件数据buff   
//          size:i:原Buff大小
//			pDeCodeBuff:o:解码后的文件内容数据buff
//	返回：	成功返回解码后的文件内容数据buff的大小，否则返回-1
//-----------------------------------------------------------------------
extern "C" _declspec(dllexport) long vgszipDeCodeFrmBuffToBuff(unsigned char *pSrcCodeBuff, long size,
													         unsigned char *pDeCodeBuff);
//-------------------------------------------------------------------------
#endif //_VGSZIP_H_