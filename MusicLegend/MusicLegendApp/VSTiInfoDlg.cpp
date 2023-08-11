//--------------------------------------------------
// VSTiInfoDlg.cpp : implementation file
//
//--------------------------------------------------
#include "stdafx.h"
#include "MusicLegendApp.h"
#include "VSTiInfoDlg.h"
#include ".\vstiinfodlg.h"
//--------------------------------------------------
// CVSTiInfoDlg dialog
//--------------------------------------------------
IMPLEMENT_DYNAMIC(CVSTiInfoDlg, CDialog)
CVSTiInfoDlg::CVSTiInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVSTiInfoDlg::IDD, pParent)
{
	m_pVSTiEffect = NULL;
}
//--------------------------------------------------
CVSTiInfoDlg::~CVSTiInfoDlg()
{
}
//--------------------------------------------------
void CVSTiInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}
//--------------------------------------------------
BEGIN_MESSAGE_MAP(CVSTiInfoDlg, CDialog)
	ON_WM_CLOSE()
END_MESSAGE_MAP()
//--------------------------------------------------


//--------------------------------------------------
// CVSTiInfoDlg message handlers
//--------------------------------------------------
BOOL CVSTiInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	if (NULL == m_pVSTiEffect)
	{
		MessageBox(_T("VSTi Effect pointer is null!"), _T("Error"), MB_OK);
		OnOK();
	}

	CString strTitle;
	VSTiEffectInfo Info;
	m_pVSTiEffect->GetEffectInfo(Info);
	strTitle = Info.csEffectName;
	strTitle += _T(" Edit Window");
	SetWindowText(strTitle);

	RECT rcEffect;
	m_pVSTiEffect->GetEditWindowRect(rcEffect);

	CRect rcWindow;
	rcWindow.left = 0;
	rcWindow.top = 0;
	rcWindow.right = rcEffect.right - rcEffect.left + 6;
	rcWindow.bottom = rcEffect.bottom - rcEffect.top + 32;
	SetWindowPos(NULL, 0, 0, rcWindow.Width(), rcWindow.Height(), SWP_SHOWWINDOW);
	CenterWindow(NULL);

	m_pVSTiEffect->ShowEditWindow(m_hWnd);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
//--------------------------------------------------
void CVSTiInfoDlg::OnClose()
{
	if (NULL == m_pVSTiEffect)
	{
		m_pVSTiEffect->CloseEditWindow();
		m_pVSTiEffect = NULL;
	}

	CDialog::OnClose();
}
//--------------------------------------------------
