// MyEdit1.cpp : implementation file
//

#include "stdafx.h"
#include "TestEdit.h"
#include "MyEdit1.h"
#include ".\myedit1.h"


// CMyEdit1

IMPLEMENT_DYNAMIC(CMyEdit1, CEdit)
CMyEdit1::CMyEdit1()
:m_iCurLineCnt(0)
{
}

CMyEdit1::~CMyEdit1()
{
}


BEGIN_MESSAGE_MAP(CMyEdit1, CEdit)
	ON_CONTROL_REFLECT(EN_CHANGE, OnEnChange)
END_MESSAGE_MAP()



// CMyEdit1 message handlers


BOOL CMyEdit1::PreTranslateMessage(MSG* pMsg)
{
	if(	pMsg->message == WM_KEYDOWN &&
		pMsg->wParam == VK_RETURN)
	{
		LOGFONT lf;
		CFont *pFnt = GetFont();
		pFnt->GetLogFont(&lf);
		//
		int iLineCnt = GetLineCount();
		int iEditHeight = lf.lfHeight * ((iLineCnt + 1)*1.2f);
		iEditHeight = iEditHeight > 0 ? iEditHeight : -1*iEditHeight;
		//
		CRect rcEdit;
		GetWindowRect(&rcEdit);
		GetParent()->ScreenToClient(&rcEdit);
		rcEdit.bottom = rcEdit.top + iEditHeight;
		MoveWindow(&rcEdit);
	}

	return CEdit::PreTranslateMessage(pMsg);
}

void CMyEdit1::OnEnChange()
{
	int iLineCnt = GetLineCount();
	if(iLineCnt != m_iCurLineCnt)
	{
		LOGFONT lf;
		CFont *pFnt = GetFont();
		pFnt->GetLogFont(&lf);
		//
		int iEditHeight = lf.lfHeight * ((iLineCnt + 0)*1.2f);
		iEditHeight = iEditHeight > 0 ? iEditHeight : -1*iEditHeight;
		//
		CRect rcEdit;
		GetWindowRect(&rcEdit);
		GetParent()->ScreenToClient(&rcEdit);
		rcEdit.bottom = rcEdit.top + iEditHeight;
		MoveWindow(&rcEdit);
		//
		m_iCurLineCnt = iLineCnt;
	}	
}
