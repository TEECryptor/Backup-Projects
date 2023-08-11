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
// ������: GetAt()
// ��  ��: INT uIndex
// ��  ;: �õ���uIndex��Material��ָ��
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
// ������: GetByName()
// ��  ��: char* strName
// ��  ;: ͨ��Element�����Ƶõ�CListElement��ָ��
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
// ������: GetFirst()
// ��  ��: 
// ��  ;: �õ���һ��Element��ָ��
//-----------------------------------------------------------------------------
CListElement* CList::GetFirst()
{
	return pFirst;
}	

//-----------------------------------------------------------------------------
// ������: GetLast()
// ��  ��: 
// ��  ;: �õ����һ��Element��ָ��
//-----------------------------------------------------------------------------
CListElement* CList::GetLast()	
{
	return pLast;
}	

//-----------------------------------------------------------------------------
// ������: GetCurrent()
// ��  ��: 
// ��  ;: �õ���ǰָ���Element��ָ��
//-----------------------------------------------------------------------------
CListElement* CList::GetCurrent()
{
	return pCurrent;
}

//-----------------------------------------------------------------------------
// ������: GetCount()
// ��  ��: 
// ��  ;: �õ���ǰElement���еĳ���
//-----------------------------------------------------------------------------
unsigned int CList::GetCount()
{
	return count;
}

//-----------------------------------------------------------------------------
// ������: GetCurrentIndex()
// ��  ��: 
// ��  ;: �õ���ǰElement��ָ����
//-----------------------------------------------------------------------------
int CList::GetCurrentIndex()
{
	if (pCurrent == NULL) 
		return -1;
	return pCurrent->index;
}						

//-----------------------------------------------------------------------------
// ������: PushAt()
// ��  ��: int uIndex, CListElement* pElement
// ��  ;: ��pElement���ڹ̶���λ��
//-----------------------------------------------------------------------------
VOID CList::PushAt(int uIndex, CListElement* pElement)
{
	//�����������б���������֮�⣬��ʲôҲ������ֱ�ӷ���
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
// ������: PushFront()
// ��  ��: CListElement* pElement
// ��  ;: ��pElement�������е���ǰ��
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
// ������: PushBack()
// ��  ��: CListElement* pElement
// ��  ;: ��pElement�������е������
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
// ������: DelFirst()
// ��  ��: 
// ��  ;: ɾ����һ��CListElement
//-----------------------------------------------------------------------------	 
VOID CList::DelFirst()
{
	DelAt(0);
}
	
//-----------------------------------------------------------------------------
// ������: DelLast()
// ��  ��: 
// ��  ;: ɾ�����һ��CListElement
//-----------------------------------------------------------------------------
VOID CList::DelLast()	
{
	DelAt(count -1);
}

//-----------------------------------------------------------------------------
// ������: DelAt()
// ��  ��: int uIndex
// ��  ;: ɾ����uIndex��CListElement
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
// ������: Release()
// ��  ��: 
// ��  ;: �ͷ��ڴ�
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