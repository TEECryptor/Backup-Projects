//--------------------------------------------
//	File Name:NewThreadDlg.cpp
//	Introduce:The implement file of class CNewThreadDlg
//	Date:2010-06-06
//--------------------------------------------
#include "stdafx.h"
#include "TestHTTPDownload.h"
#include "NewThreadDlg.h"
#include ".\newthreaddlg.h"
//--------------------------------------------
// CNewThreadDlg dialog
//--------------------------------------------
IMPLEMENT_DYNAMIC(CNewThreadDlg, CDialog)
CNewThreadDlg::CNewThreadDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNewThreadDlg::IDD, pParent)
{
	m_dwThreadID = 0;
	m_dwStartPos = 0;
	m_dwLength = 0;
}
//--------------------------------------------
CNewThreadDlg::~CNewThreadDlg()
{
}
//--------------------------------------------
void CNewThreadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}
//--------------------------------------------
BEGIN_MESSAGE_MAP(CNewThreadDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()
//--------------------------------------------
BOOL CNewThreadDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	GetDlgItem(IDC_EDIT_THREADID)->SetFocus();

	return FALSE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
//--------------------------------------------



//--------------------------------------------
// CNewThreadDlg message handlers
//--------------------------------------------
void CNewThreadDlg::OnBnClickedOk()
{
	TCHAR tcTemp[32] = {_T("\0")};
	GetDlgItemText(IDC_EDIT_THREADID, tcTemp, 32);
	m_dwThreadID = _ttol(tcTemp);
	//
	GetDlgItemText(IDC_EDIT_STARTPOS, tcTemp, 32);
	m_dwStartPos = _ttol(tcTemp);
	//
	GetDlgItemText(IDC_EDIT_LENGTH, tcTemp, 32);
	m_dwLength = _ttol(tcTemp);

	if (m_dwThreadID <= 0)
	{
		MessageBox(_T("Thread ID is wrong, thread ID should > 0!"));
		GetDlgItem(IDC_EDIT_THREADID)->SetFocus();
		return;
	}

	if (m_dwStartPos < 0)
	{
		MessageBox(_T("Start position is wrong!"));
		GetDlgItem(IDC_EDIT_STARTPOS)->SetFocus();
		return;
	}

	if (m_dwLength <= 0)
	{
		MessageBox(_T("Data length is wrong!"));
		GetDlgItem(IDC_EDIT_LENGTH)->SetFocus();
		return;
	}

	OnOK();
}
//--------------------------------------------