/////////////////////////////////////////////////////////////////////////
// 名称: CImage.h--CImage class
// 描述: 图像对象类，包含VGS中使用的图像基本结构
// 作者: lixin	
// 日期: 2006-08-23
////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CIMAGE_H__D6FC75AF_B55D_4350_AF0E_A416DACCF5A2__INCLUDED_)
#define AFX_CIMAGE_H__D6FC75AF_B55D_4350_AF0E_A416DACCF5A2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CSysFunc.h"

//##ModelId=47EBC749014C
class VGS_EXPORT CImage
{
public:
	//##ModelId=47EBC749014D
	INT					index;
	//##ModelId=47EBC749014E
	CHAR				name[32];
	//##ModelId=47EBC749014F
	CHAR				fullPath[256];
	//##ModelId=47EBC7490157
	CHAR				pathAndName[256];	//图像文件的全路径
	//##ModelId=47EBC7490159
	_D3DXIMAGE_INFO		imageInfo;			//图像文件格式

	//##ModelId=47EBC749015D
	SHORT				width;				//map width
	//##ModelId=47EBC749015E
	SHORT				height;				//map height
	//##ModelId=47EBC7490160
	D3DFORMAT			format;				//图像数据格式
	//##ModelId=47EBC7490164
	UINT				dataSize;			//图像数据长度
	//##ModelId=47EBC7490165
	BYTE*				data;				//图像数据

	
	//##ModelId=47EBC749016A
	CImage*				pPrevious;			//前一个材质
	//##ModelId=47EBC749016F
	CImage*				pNext;				//后一个材质

public:
	//##ModelId=47EBC7490171
	CImage();
	//##ModelId=47EBC7490172
	virtual ~CImage();

	//##ModelId=47EBC7490174
	VOID			Release();

};

#endif // !defined(AFX_CIMAGE_H__D6FC75AF_B55D_4350_AF0E_A416DACCF5A2__INCLUDED_)
