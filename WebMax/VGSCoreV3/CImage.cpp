// CImage.cpp: implementation of the CImage class.
//
//////////////////////////////////////////////////////////////////////

#include "CImage.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//##ModelId=47EBC7490171
CImage::CImage()
{
	data = NULL;
	ZeroMemory(name, sizeof(name));
	ZeroMemory(name, sizeof(fullPath));
	ZeroMemory(name, sizeof(pathAndName));
}

//##ModelId=47EBC7490172
CImage::~CImage()
{

}

//ÊÍ·ÅÍ¼ÏñÄÚ´æ
//##ModelId=47EBC7490174
VOID CImage::Release()
{
	SAFE_DELETE_ARRAY(data);	
}
