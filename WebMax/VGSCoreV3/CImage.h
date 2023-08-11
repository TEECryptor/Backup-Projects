/////////////////////////////////////////////////////////////////////////
// ����: CImage.h--CImage class
// ����: ͼ������࣬����VGS��ʹ�õ�ͼ������ṹ
// ����: lixin	
// ����: 2006-08-23
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
	CHAR				pathAndName[256];	//ͼ���ļ���ȫ·��
	//##ModelId=47EBC7490159
	_D3DXIMAGE_INFO		imageInfo;			//ͼ���ļ���ʽ

	//##ModelId=47EBC749015D
	SHORT				width;				//map width
	//##ModelId=47EBC749015E
	SHORT				height;				//map height
	//##ModelId=47EBC7490160
	D3DFORMAT			format;				//ͼ�����ݸ�ʽ
	//##ModelId=47EBC7490164
	UINT				dataSize;			//ͼ�����ݳ���
	//##ModelId=47EBC7490165
	BYTE*				data;				//ͼ������

	
	//##ModelId=47EBC749016A
	CImage*				pPrevious;			//ǰһ������
	//##ModelId=47EBC749016F
	CImage*				pNext;				//��һ������

public:
	//##ModelId=47EBC7490171
	CImage();
	//##ModelId=47EBC7490172
	virtual ~CImage();

	//##ModelId=47EBC7490174
	VOID			Release();

};

#endif // !defined(AFX_CIMAGE_H__D6FC75AF_B55D_4350_AF0E_A416DACCF5A2__INCLUDED_)
