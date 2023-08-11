// EffectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MyMidiSample.h"
#include "EffectDlg.h"


// CEffectDlg dialog

IMPLEMENT_DYNAMIC(CEffectDlg, CDialog)
CEffectDlg::CEffectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEffectDlg::IDD, pParent)
{
}

CEffectDlg::~CEffectDlg()
{
}

void CEffectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CEffectDlg, CDialog)
END_MESSAGE_MAP()


// CEffectDlg message handlers
