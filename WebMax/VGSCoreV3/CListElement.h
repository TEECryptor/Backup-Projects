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
	char						name[32];					//���ƣ�������32���ַ�
	//##ModelId=47EBC7490042
	int							index;						//���

	//##ModelId=47EBC7490046
	CListElement*				pPrevious;					//ǰһ������
	//##ModelId=47EBC7490051
	CListElement*				pNext;						//��һ������

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
