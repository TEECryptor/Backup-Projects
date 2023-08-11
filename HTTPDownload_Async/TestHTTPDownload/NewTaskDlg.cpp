//--------------------------------------------
//	File Name:NewTaskDlg.cpp
//	Introduce:The implement file of class CNewTaskDlg
//	Date:2010-05-10
//--------------------------------------------
#include "stdafx.h"
#include "TestHTTPDownload.h"
#include "NewTaskDlg.h"
//--------------------------------------------
// CNewTaskDlg dialog
//--------------------------------------------
IMPLEMENT_DYNAMIC(CNewTaskDlg, CDialog)
//--------------------------------------------
CNewTaskDlg::CNewTaskDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNewTaskDlg::IDD, pParent)
{
	m_dwThreadCnt = 5;
	m_bIsStreamTask = FALSE;
	m_bForceDownload = FALSE;
}
//--------------------------------------------
CNewTaskDlg::~CNewTaskDlg()
{
}
//--------------------------------------------
void CNewTaskDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}
//--------------------------------------------
BEGIN_MESSAGE_MAP(CNewTaskDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_BROWSE, OnBnClickedBtnBrowse)
	ON_CBN_SELCHANGE(IDC_COMBO_THREADCNT, OnCbnSelchangeComboThreadcnt)
	ON_BN_CLICKED(IDC_CHECK_FORCED, OnBnClickedCheckForced)
	ON_CBN_SELCHANGE(IDC_COMBO_TASKTYPE, OnCbnSelchangeComboTasktype)
	ON_BN_CLICKED(IDC_BTN_ADDTHREAD, OnBnClickedBtnAddthread)
	ON_BN_CLICKED(IDC_BTN_DELTHREAD, OnBnClickedBtnDelthread)
END_MESSAGE_MAP()
//--------------------------------------------


//--------------------------------------------
BOOL CNewTaskDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

    _tcscpy(m_tcNetFileURL, _T("\0"));
	_tcscpy(m_tcTargetFile, _T("\0"));
	
	CComboBox* pTaskTypeCombo = (CComboBox*)GetDlgItem(IDC_COMBO_TASKTYPE);
	pTaskTypeCombo->AddString(_T("Normal Task"));
	pTaskTypeCombo->AddString(_T("Stream Task"));
	pTaskTypeCombo->SetCurSel(0);
	OnCbnSelchangeComboTasktype();

	CComboBox* pThreadCntCom = (CComboBox*)GetDlgItem(IDC_COMBO_THREADCNT);
	pThreadCntCom->ResetContent();
	for (int iIndex = 0; iIndex < 10; iIndex++)
	{
		CString strCount;
		strCount.Format(_T("%d"), iIndex+1);
		pThreadCntCom->AddString(strCount);
	}
	pThreadCntCom->SetCurSel(4);	//5 threads in default

	CListCtrl* pThreadList = (CListCtrl*)GetDlgItem(IDC_LIST_THREADINFO);
	pThreadList->SetExtendedStyle(pThreadList->GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_SHOWSELALWAYS);
	pThreadList->InsertColumn(0, _T("Thread ID"), LVCFMT_LEFT, 80);
	pThreadList->InsertColumn(1, _T("Start Position"), LVCFMT_LEFT, 100);
	pThreadList->InsertColumn(2, _T("Data length"), LVCFMT_LEFT, 100);

	GetDlgItem(IDC_EDIT_URL)->SetFocus();

	return FALSE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
//--------------------------------------------
void CNewTaskDlg::OnBnClickedBtnBrowse()
{
	// Check URL is set or not
	GetDlgItemText(IDC_EDIT_URL, m_tcNetFileURL, 1024);
	if (_tcslen(m_tcNetFileURL) == 0)
	{
		MessageBox(_T("Please set net file URL string!"));
		GetDlgItem(IDC_EDIT_URL)->SetFocus();
		return;
	}

	// Get net file name
	TCHAR tcFileName[MAX_PATH] = {_T("\0")};
	LPCTSTR lp = m_tcNetFileURL;
	while (*lp != _T('\0')) {lp++;}
	while (*lp != _T('/')) {lp--;}
	_tcscpy(tcFileName, lp+1);

	// Use file dialog to select full save file name
	CFileDialog dlgSave(FALSE, 0, tcFileName);
	if (dlgSave.DoModal() == IDOK)
	{
		CString strSaveFile = dlgSave.GetPathName();
		SetDlgItemText(IDC_EDIT_SAVEFILE, strSaveFile);
		_tcscpy(m_tcTargetFile, (LPCTSTR)strSaveFile);
	}
}
//--------------------------------------------
void CNewTaskDlg::OnCbnSelchangeComboThreadcnt()
{
	CComboBox* pThreadCntCom = (CComboBox*)GetDlgItem(IDC_COMBO_THREADCNT);
	m_dwThreadCnt = pThreadCntCom->GetCurSel() + 1;
}
//--------------------------------------------
void CNewTaskDlg::OnBnClickedCheckForced()
{
	CButton* pForceCheckBtn = (CButton*)GetDlgItem(IDC_CHECK_FORCED);
	if (BST_CHECKED == pForceCheckBtn->GetCheck())
		m_bForceDownload = TRUE;
	else
		m_bForceDownload = FALSE;
}
//--------------------------------------------
void CNewTaskDlg::OnCbnSelchangeComboTasktype()
{
	CComboBox* pTaskTypeCombo = (CComboBox*)GetDlgItem(IDC_COMBO_TASKTYPE);
	int iCurSel = pTaskTypeCombo->GetCurSel();
	
	if (0 == iCurSel)
	{
		m_bIsStreamTask = FALSE;
		GetDlgItem(IDC_COMBO_THREADCNT)->EnableWindow(TRUE);
		GetDlgItem(IDC_LIST_THREADINFO)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_ADDTHREAD)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_DELTHREAD)->EnableWindow(FALSE);
	}
	else
	{
		m_bIsStreamTask = TRUE;
		GetDlgItem(IDC_COMBO_THREADCNT)->EnableWindow(FALSE);
		GetDlgItem(IDC_LIST_THREADINFO)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_ADDTHREAD)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_DELTHREAD)->EnableWindow(TRUE);
	}
}
//--------------------------------------------
void CNewTaskDlg::OnBnClickedBtnAddthread()
{
	CNewThreadDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		StreamTaskThreadInfo ThreadInfo;
		ThreadInfo.dwThreadID = dlg.m_dwThreadID;
		ThreadInfo.dwStartPos = dlg.m_dwStartPos;
		ThreadInfo.dwDataLength = dlg.m_dwLength;
		m_lstThreadInfo.push_back(ThreadInfo);

		CListCtrl* pThreadList = (CListCtrl*)GetDlgItem(IDC_LIST_THREADINFO);
		DWORD dwCurCount = pThreadList->GetItemCount();

		TCHAR tcTempStr[32] = {_T("\0")};
		_stprintf(tcTempStr, _T("%d"), ThreadInfo.dwThreadID);
		int iIndex = pThreadList->InsertItem(dwCurCount, tcTempStr);
		pThreadList->SetItemData(iIndex, ThreadInfo.dwThreadID);
		//
		_stprintf(tcTempStr, _T("%d"), ThreadInfo.dwStartPos);
		pThreadList->SetItemText(dwCurCount, 1, tcTempStr);
		//
		_stprintf(tcTempStr, _T("%d"), ThreadInfo.dwDataLength);
		pThreadList->SetItemText(dwCurCount, 2, tcTempStr);
	}
}
//--------------------------------------------
void CNewTaskDlg::OnBnClickedBtnDelthread()
{
	DWORD dwSelThreadID = -1;
	CListCtrl* pThreadList = (CListCtrl*)GetDlgItem(IDC_LIST_THREADINFO);
	DWORD dwCurCount = pThreadList->GetItemCount();
	for (DWORD dwIndex = 0; dwIndex < dwCurCount; dwIndex++)
	{		
		if (LVIS_SELECTED == pThreadList->GetItemState(dwIndex, LVIS_SELECTED))
		{
			dwSelThreadID = (DWORD)pThreadList->GetItemData(dwIndex);
			pThreadList->DeleteItem(dwIndex);
			break;	
		}
	}

	if (-1 == dwSelThreadID)
	{
		MessageBox(_T("Please select the thread item you want to delete!"));
		return;
	}

	for (ThreadInfoList::iterator it  = m_lstThreadInfo.begin();
		 it  != m_lstThreadInfo.end();
		 it++)
	{
		if ((*it).dwThreadID == dwSelThreadID)
		{
			m_lstThreadInfo.erase(it);
			break;
		}
	}
}
//--------------------------------------------
