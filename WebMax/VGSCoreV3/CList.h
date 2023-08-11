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
	int								count;												//List��Ԫ�ص�����������
	CListElement*					pCurrent;											//��ǰ�����ָ��
	CListElement*					pFirst;												//��һ������
	CListElement*					pLast;												//���һ������

public:
	CListElement*					GetAt(int uIndex);									//�õ���uIndex��Element��ָ��
	CListElement*					GetByName(char* strName);							//ͨ��Element�����Ƶõ�Element��ָ��
	CListElement*					GetFirst();											//�õ���һ��Element��ָ��
	CListElement*					GetLast();											//�õ����һ��Element��ָ��
	CListElement*					GetCurrent();										//�õ���ǰָ���Element��ָ��
	int								GetCurrentIndex();									//�õ���ǰElement��ָ����
	unsigned int					GetCount();											//�õ���ǰElement���еĳ���

	void							PushAt(int uIndex, CListElement* pElement);		//��Element���ڹ̶���λ��
	void							PushFront(CListElement* pElement);					//��Element�������е���ǰ��
	void							PushBack(CListElement* pElement);					//��Element�������е������
	
	void							DelFirst();											//ɾ����һ��Element
	void							DelLast();											//ɾ�����һ��Element
	void							DelAt(int uIndex);									//ɾ����uIndex��Element

	void							Release();	

public:
	CList();
	virtual ~CList();

};

#endif // !defined(AFX_CLISTS_H__037AE677_06DA_428B_81BF_90D4C259A79E__INCLUDED_)
