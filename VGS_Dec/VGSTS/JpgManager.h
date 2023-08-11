//////////////////////////////////////////////////////////////////////
//	JpgManager.h: interface for the CJpgManager class.
//	类名：	CJpgManager
//	作者：	yaoyoufang
//	日期：	04-8-1
//	简介：	该类实现了对jpg文件的加载和保存
//
//------------------------------------------------------------
#if !defined(AFX_JPGMANAGER_H__CADB9767_047A_49F4_83FF_086623F3DF45__INCLUDED_)
#define AFX_JPGMANAGER_H__CADB9767_047A_49F4_83FF_086623F3DF45__INCLUDED_
//------------------------------------------------------------
#include <windows.h>
#include "ijl.h"
//------------------------------------------------------------
typedef enum _JPGPARAMS
{
	/*图片数据buff大小*/
	_JPGIMAGESIZE		= 0,
	/*图片宽度参数*/
	_JPGIMAGEWIDTH		= 1,
	/*图片高度参数*/
	_JPGIMAGEHEIGHT		= 2,
	/*图片通道参数*/
	_JPGIMAGECHANNELS	= 3
}JPGPARAMS;
//------------------------------------------------------------
class CJpgManager  
{
public:
	CJpgManager();
	virtual ~CJpgManager();
//公共方法
public:
	//--------------------------------------------------------
	static JPEG_CORE_PROPERTIES s_Image;	 
	//初始化ijl库
	static BOOL InitIjlLibrary();
	//释放ijl库
	static void FreeIjlLibrary();
	//加载
	static BOOL	LoadImageFromBuff(BYTE* pJpgData, int iJpgDataSize, BYTE*& pData, int &iWidth, int &iHeight);
	//--------------------------------------------------------
	/******************加载JPG文件的函数********************/
	//打开jpg文件，读取图片属性和数据到成员变量
	BOOL		jpgOpenFile(const char* pszFileName);
	//返回jpg图片数据
	BYTE*		jpgGetImageData();
	//返回图片参数
	int			jpgGetImageParams(JPGPARAMS eParams);
	//返回图片BMP的信息
	void		jpgGetImageBMPInfo(BITMAPINFO &stuBMPInfo);
	/**************加载JPG文件的函数_End******************/

	/*****************写JPG文件的函数*********************/
	//设置图片参数
	void		jpgSetImageParams(JPGPARAMS eParams, int iParam);
	//将数据保存为JPG格式的文件
	BOOL		jpgSaveFile(BYTE* pImageData,const char* pszFileName);

	//从内存里读取数据，然后转化为位图数据
	BOOL		jpgReadBuff(BYTE* pJpgData, int iJpgDataSize);

//私有方法
private:
	//格式转换
	void		jpgRGBA_FPX_to_BGRA(BYTE* data,int width,int height);
	//格式转换
	void		jpgBGRA_to_RGBA(BYTE* data,int width,int height);

//私有成员变量
private:
	int			m_iImageSize;		//图片数据Buff大小
	int			m_iImageWidth;		//图片像素宽
	int			m_iImageHeight;		//图片像素高
	int			m_iImageChannels;	//图片通道数
	BYTE*		m_pImageData;		//图片数据
	BITMAPINFO	m_stuBMPInfo;		//图片BMP信息数据
};

#endif // !defined(AFX_JPGMANAGER_H__CADB9767_047A_49F4_83FF_086623F3DF45__INCLUDED_)
