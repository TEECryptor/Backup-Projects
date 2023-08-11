// CList.cpp: implementation of the CLists class.
//
//////////////////////////////////////////////////////////////////////

#include "CList.h"
#include "windows.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CList::CList()
{
	pCurrent = NULL;
	pFirst = NULL;
	pLast = NULL;

	count = 0;
}

CList::~CList()
{

}


//-----------------------------------------------------------------------------
// 方法名: GetAt()
// 参  数: INT uIndex
// 用  途: 得到第uIndex个Material的指针
//-----------------------------------------------------------------------------
CListElement* CList::GetAt(int uIndex)
{
	if (count <= 0 || uIndex >= count || pCurrent == NULL) return NULL;

	if (uIndex == 0) 
	{
		pCurrent = pFirst;
		return pFirst;
	}
	if (uIndex == count -1) return pLast;
	if (pCurrent->index == uIndex) return pCurrent;

	if (pCurrent->index < uIndex )
	{
		while (pCurrent->index < uIndex)
		{
			if (pCurrent == NULL) return 0;
			pCurrent = pCurrent->pNext;
		}
		return pCurrent;
	}
	else
	{
		while (pCurrent->index > uIndex)
		{
			pCurrent = pCurrent->pPrevious;
		}
		return pCurrent;
	}

	return NULL;
}

//-----------------------------------------------------------------------------
// 方法名: GetByName()
// 参  数: char* strName
// 用  途: 通过Element的名称得到CListElement的指针
//-----------------------------------------------------------------------------
CListElement* CList::GetByName(char* strName)
{
	if (count == 0) return NULL;

	if (strcmp(pCurrent->name, strName) == 0) 
		return pCurrent;

	int tLen = GetCount();
	CListElement* tElement = pCurrent;
	int tIndex = pCurrent->index;

	while (pCurrent->pNext != NULL)
	{
		pCurrent = pCurrent->pNext;
		if (strcmp(pCurrent->name, strName) == 0) 
			return pCurrent;
	}

	pCurrent = tElement;
	while (pCurrent->pPrevious != NULL)
	{
		pCurrent = pCurrent->pPrevious;
		if (strcmp(pCurrent->name, strName) == 0)
			return pCurrent;
	}

	return NULL;
}

//-----------------------------------------------------------------------------
// 方法名: GetFirst()
// 参  数: 
// 用  途: 得到第一个Element的指针
//-----------------------------------------------------------------------------
CListElement* CList::GetFirst()
{
	return pFirst;
}	

//-----------------------------------------------------------------------------
// 方法名: GetLast()
// 参  数: 
// 用  途: 得到最后一个Element的指针
//-----------------------------------------------------------------------------
CListElement* CList::GetLast()	
{
	return pLast;
}	

//-----------------------------------------------------------------------------
// 方法名: GetCurrent()
// 参  数: 
// 用  途: 得到当前指向的Element的指针
//-----------------------------------------------------------------------------
CListElement* CList::GetCurrent()
{
	return pCurrent;
}

//-----------------------------------------------------------------------------
// 方法名: GetCount()
// 参  数: 
// 用  途: 得到当前Element序列的长度
//-----------------------------------------------------------------------------
unsigned int CList::GetCount()
{
	return count;
}

//-----------------------------------------------------------------------------
// 方法名: GetCurrentIndex()
// 参  数: 
// 用  途: 得到当前Element的指针编号
//-----------------------------------------------------------------------------
int CList::GetCurrentIndex()
{
	if (pCurrent == NULL) 
		return -1;
	return pCurrent->index;
}						

//-----------------------------------------------------------------------------
// 方法名: PushAt()
// 参  数: int uIndex, CListElement* pElement
// 用  途: 将pElement放在固定的位置
//-----------------------------------------------------------------------------
VOID CList::PushAt(int uIndex, CListElement* pElement)
{
	//如果将其放在列表现有区域之外，则什么也不做，直接返回
	if (uIndex < 0 || uIndex >= count) return;

	CListElement* pTElement = GetAt(uIndex);
	pTElement->pPrevious = pElement;
	pTElement->index = uIndex + 1;
	
	pElement->index = uIndex;
	pElement->pPrevious = pTElement->pPrevious;
	pElement->pNext = pTElement;

	if (pTElement->pPrevious != NULL)
		pTElement->pPrevious->pNext = pElement;

	pCurrent = pElement;

	if (uIndex == 0)
		pFirst = pCurrent;
}

//-----------------------------------------------------------------------------
// 方法名: PushFront()
// 参  数: CListElement* pElement
// 用  途: 将pElement放在序列的最前面
//-----------------------------------------------------------------------------
VOID CList::PushFront(CListElement* pElement)	
{	
	if (pCurrent == NULL) 
	{
		pElement->pPrevious = NULL;
		pElement->pNext = NULL;

		pCurrent = pElement;
		pFirst = pCurrent;
		pLast = pCurrent;

		count++;
	}
	else
	{
		CListElement* pTElement = GetFirst();
		pTElement->pPrevious = pElement;

		pElement->pPrevious = NULL;
		pElement->pNext = pTElement;

		pCurrent = pElement;
		pFirst = pCurrent;

		count++;
	}
}

//-----------------------------------------------------------------------------
// 方法名: PushBack()
// 参  数: CListElement* pElement
// 用  途: 将pElement放在序列的最后面
//-----------------------------------------------------------------------------	 
VOID CList::PushBack(CListElement* pElement)	
{	
	if (pCurrent == NULL ) 
	{
		pElement->pPrevious = NULL;
		pElement->pNext = NULL;

		pCurrent = pElement;
		pFirst = pCurrent;
		pLast = pCurrent;

		count++;
	}
	else
	{
		CListElement* pTElement = GetLast();
		pTElement->pNext = pElement;

		pElement->pPrevious = pTElement;
		pElement->pNext = NULL;

		pCurrent = pElement;
		pLast = pCurrent;

		count++;
	}
}	

//-----------------------------------------------------------------------------
// 方法名: DelFirst()
// 参  数: 
// 用  途: 删除第一个CListElement
//-----------------------------------------------------------------------------	 
VOID CList::DelFirst()
{
	DelAt(0);
}
	
//-----------------------------------------------------------------------------
// 方法名: DelLast()
// 参  数: 
// 用  途: 删除最后一个CListElement
//-----------------------------------------------------------------------------
VOID CList::DelLast()	
{
	DelAt(count -1);
}

//-----------------------------------------------------------------------------
// 方法名: DelAt()
// 参  数: int uIndex
// 用  途: 删除第uIndex个CListElement
//-----------------------------------------------------------------------------
VOID CList::DelAt(int uIndex)	
{
	if (count == 0 || uIndex < 0 || uIndex >= count) return;

	CListElement* pElement = GetAt(uIndex);
	if (pElement == NULL) return;

	CListElement* prepElement = pElement->pPrevious;
	CListElement* nextElement = pElement->pNext;

	if (prepElement == NULL && nextElement== NULL)
	{
		SAFE_RELEASE(pElement);
		SAFE_DELETE(pElement);
		pFirst = NULL;
		pLast = NULL;
		pCurrent = NULL;
		count = 0;
		return;
	}

	if (prepElement != NULL)
	{
		prepElement->pNext = nextElement;
		if (nextElement == NULL)
		{
			pCurrent = prepElement;
			pLast = pCurrent;
		}
		else
		{
			pCurrent = nextElement;
			nextElement->index = pElement->index;
		}

	}

	if (nextElement != NULL)
	{
		nextElement->pPrevious = prepElement;
		nextElement->index = pElement->index;
		pCurrent = nextElement;
	}
	
	SAFE_RELEASE(pElement);
	SAFE_DELETE(pElement);

	count--;
}

//-----------------------------------------------------------------------------
// 方法名: Release()
// 参  数: 
// 用  途: 释放内存
//-----------------------------------------------------------------------------
VOID CList::Release()
{
	int len = GetCount();
	for (int i = len-1; i >= 0; i--)
		DelAt(i);

	pCurrent = NULL;
	pFirst = NULL;
	pLast = NULL;

	count = 0;
}						