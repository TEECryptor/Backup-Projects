// CListElement.h: interface for the CList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLISTELEMENT_H__0CA2EA4E_B370_4BB6_A9DE_BF26028A34F4__INCLUDED_)
#define AFX_CLISTELEMENT_H__0CA2EA4E_B370_4BB6_A9DE_BF26028A34F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "VGSDef.h"

//##ModelId=47EBC7490040
class VGS_EXPORT CListElement  
{
public:
	//##ModelId=47EBC7490041
	char						name[32];					//名称，不超过32个字符
	//##ModelId=47EBC7490042
	int							index;						//编号

	//##ModelId=47EBC7490046
	CListElement*				pPrevious;					//前一个对象
	//##ModelId=47EBC7490051
	CListElement*				pNext;						//后一个对象

public:
	//##ModelId=47EBC7490053
	virtual void Release();

public:
	//##ModelId=47EBC7490055
	CListElement();
	//##ModelId=47EBC7490056
	virtual ~CListElement();

};

#endif // !defined(AFX_CLIST_H__0CA2EA4E_B370_4BB6_A9DE_BF26028A34F4__INCLUDED_)
