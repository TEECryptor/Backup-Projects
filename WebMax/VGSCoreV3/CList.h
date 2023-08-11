// CList.h: interface for the CLists class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLIST_H__037AE677_06DA_428B_81BF_90D4C259A79E__INCLUDED_)
#define AFX_CLIST_H__037AE677_06DA_428B_81BF_90D4C259A79E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CListElement.h"
#include "CSysFunc.h"
#include "VGSDef.h"

class VGS_EXPORT CList  
{
public:
	int								count;												//List中元素的数量的数量
	CListElement*					pCurrent;											//当前对象的指针
	CListElement*					pFirst;												//第一个对象
	CListElement*					pLast;												//最后一个对象

public:
	CListElement*					GetAt(int uIndex);									//得到第uIndex个Element的指针
	CListElement*					GetByName(char* strName);							//通过Element的名称得到Element的指针
	CListElement*					GetFirst();											//得到第一个Element的指针
	CListElement*					GetLast();											//得到最后一个Element的指针
	CListElement*					GetCurrent();										//得到当前指向的Element的指针
	int								GetCurrentIndex();									//得到当前Element的指针编号
	unsigned int					GetCount();											//得到当前Element序列的长度

	void							PushAt(int uIndex, CListElement* pElement);		//将Element放在固定的位置
	void							PushFront(CListElement* pElement);					//将Element放在序列的最前面
	void							PushBack(CListElement* pElement);					//将Element放在序列的最后面
	
	void							DelFirst();											//删除第一个Element
	void							DelLast();											//删除最后一个Element
	void							DelAt(int uIndex);									//删除第uIndex个Element

	void							Release();	

public:
	CList();
	virtual ~CList();

};

#endif // !defined(AFX_CLISTS_H__037AE677_06DA_428B_81BF_90D4C259A79E__INCLUDED_)
