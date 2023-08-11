// TestHTTPDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TestHTTP.h"
#include "TestHTTPDlg.h"
#include ".\testhttpdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


//------------------------------------------------------------
//	Name:HttpCallbackFunc
//	Introduce:The download callback function
//------------------------------------------------------------
long HttpCallbackFunc(LONG lTaskID, LONG lThreadID, DWORD dwEvent, DWORD dwParam1, DWORD dwParam2, LPVOID lpUser)
{
	if (NULL != lpUser)
		return ((CTestHTTPDlg*)lpUser)->OnHttpDownloadEvent(lTaskID, lThreadID, dwEvent, dwParam1, dwParam2);

	return 0;
}
//------------------------------------------------------------


//------------------------------------------------------------
// CTestHTTPDlg dialog
//------------------------------------------------------------
CTestHTTPDlg::CTestHTTPDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestHTTPDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}
//------------------------------------------------------------
void CTestHTTPDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}
//------------------------------------------------------------
BEGIN_MESSAGE_MAP(CTestHTTPDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_SAVE, OnBnClickedBtnSave)
	ON_BN_CLICKED(IDC_BTN_DOWNLOAD, OnBnClickedBtnDownload)
	ON_BN_CLICKED(IDC_BTN_SAVELOG, OnBnClickedBtnSavelog)
	ON_BN_CLICKED(IDC_RADIO_SINGLE, OnBnClickedRadioSingle)
	ON_BN_CLICKED(IDC_RADIO_MULTI, OnBnClickedRadioMulti)
	ON_MESSAGE(WM_MSG_TASKSTATUSCHANGED, OnTaskStatusChange)
	ON_MESSAGE(WM_MSG_TASKPROGRESS, OnTaskProgress)
	ON_MESSAGE(WM_MSG_TASKSERROR, OnTaskError)
	ON_WM_DESTROY()
	ON_CBN_SELCHANGE(IDC_COMBO_THREADCNT, OnCbnSelchangeComboThreadcnt)
END_MESSAGE_MAP()
//------------------------------------------------------------
// CTestHTTPDlg message handlers
//------------------------------------------------------------
BOOL CTestHTTPDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	m_lFileSize = 0;
	GetDlgItem(IDC_EDIT_URL)->SetFocus();
	//
	CComboBox* pThreadCntCombo = (CComboBox*)GetDlgItem(IDC_COMBO_THREADCNT);
	int iIndex = pThreadCntCombo->AddString(_T("2"));
	pThreadCntCombo->SetItemData(iIndex, 2);
	iIndex = pThreadCntCombo->AddString(_T("3"));
	pThreadCntCombo->SetItemData(iIndex, 3);
	iIndex = pThreadCntCombo->AddString(_T("4"));
	pThreadCntCombo->SetItemData(iIndex, 4);
	iIndex = pThreadCntCombo->AddString(_T("5"));
	pThreadCntCombo->SetItemData(iIndex, 5);
	pThreadCntCombo->SetCurSel(3);
	m_dwThreadCnt = 5;
	//
	//((CButton*)GetDlgItem(IDC_RADIO_SINGLE))->SetCheck(BST_CHECKED);
	//OnBnClickedRadioSingle();
	((CButton*)GetDlgItem(IDC_RADIO_MULTI))->SetCheck(BST_CHECKED);
	OnBnClickedRadioMulti();
	//
	CProgressCtrl* pProgress = (CProgressCtrl*)GetDlgItem(IDC_PROGRESS);
	pProgress->SetRange(0, 100);
	//
	SetDlgItemText(IDC_STATIC_PERCENT, _T("0%"));
	SetDlgItemText(IDC_STATIC_PROGRESS, _T("0 KB"));
	//
	_tcscpy(m_tcLogFile, _T("C:\\HttpLog.txt"));
	_tcscpy(m_tcNetFileURL, _T("http://www.suntoward.com/webmax2/expo/china2/floor3_01.n3d"));
	_tcscpy(m_tcTargetFile, _T("C:\\floor3_01.n3d"));
	SetDlgItemText(IDC_EDIT_LOGFILE, m_tcLogFile);
	SetDlgItemText(IDC_EDIT_URL, m_tcNetFileURL);
	SetDlgItemText(IDC_EDIT_TAGERTFILE, m_tcTargetFile);

	LONG lRes = HTTP_E_NOERROR;
	m_pHttpDownloader = Create_HttpDownload();
	ASSERT(NULL != m_pHttpDownloader);

	// Call intialize function at first!
	lRes = m_pHttpDownloader->Initialize();
	if (lRes != HTTP_E_NOERROR)
	{
		CString strError;
		strError.Format(_T("Initialize http download failed!\nError code = %8x"), lRes);
		MessageBox(strError, NULL, MB_ICONERROR|MB_OK);
		return TRUE;
	}

	// Set configure data to downloader
	HttpDownloadCfg cfg;
	memset(&cfg, 0, sizeof(cfg));
	cfg.dwMaxTaskCnt = 5;
	cfg.dwMaxThreadCnt = 5;
	cfg.lpHttpCallbackFunc = HttpCallbackFunc;
	cfg.lpUser = this;
	lRes = m_pHttpDownloader->Configure(cfg);
	
	return FALSE;  // return TRUE  unless you set the focus to a control
}
//------------------------------------------------------------
void CTestHTTPDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}
//------------------------------------------------------------
// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.
//------------------------------------------------------------
void CTestHTTPDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}
//------------------------------------------------------------
// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
//------------------------------------------------------------
HCURSOR CTestHTTPDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
//------------------------------------------------------------
void CTestHTTPDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// Stop all task before windows close
	m_pHttpDownloader->StopAllTasks();

	// Release Http download object
	Destroy_HttpDownload(m_pHttpDownloader);
	m_pHttpDownloader = NULL;
}
//------------------------------------------------------------



//------------------------------------------------------------
void CTestHTTPDlg::OnBnClickedBtnSave()
{
	// Check URL is set or not
	GetDlgItemText(IDC_EDIT_URL, m_tcNetFileURL, 1024);
	if (_tcslen(m_tcNetFileURL) == 0)
	{
		MessageBox(_T("Please set net file URL string at first!"), NULL, MB_ICONWARNING|MB_OK);
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
		SetDlgItemText(IDC_EDIT_TAGERTFILE, strSaveFile);
		_tcscpy(m_tcTargetFile, (LPCTSTR)strSaveFile);
	}
}
//------------------------------------------------------------
void CTestHTTPDlg::OnBnClickedBtnDownload()
{
	TCHAR tcLogFile[MAX_PATH];
	GetDlgItemText(IDC_EDIT_LOGFILE, tcLogFile, MAX_PATH);
	m_pHttpDownloader->SetLogFile(TRUE, tcLogFile);

	m_lTaksID = m_pHttpDownloader->CreateTask(m_tcNetFileURL, m_tcTargetFile, m_dwThreadCnt);
	if (m_lTaksID <= 0)
	{
		CString strError;
		strError.Format(_T("Create new task failed!\nError code = %8x"), m_lTaksID);
		MessageBox(strError, NULL, MB_ICONWARNING|MB_OK);		
		return;
	}

	m_lFileSize = m_pHttpDownloader->GetTaskNetFileSize(m_lTaksID);
	
	LONG lRes = m_pHttpDownloader->StartTask(m_lTaksID);
	if (lRes < 0)
	{
		CString strError;
		strError.Format(_T("Try to start the task failed!\nError code = %8x"), lRes);
		MessageBox(strError, NULL, MB_ICONWARNING|MB_OK);		
		return;
	}

	UpdateSettingControls(FALSE);
}
//------------------------------------------------------------
void CTestHTTPDlg::OnBnClickedBtnSavelog()
{
	CFileDialog dlgSave(FALSE, _T("txt"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("Log file (*.txt)|*.txt||"));
	if (dlgSave.DoModal() == IDOK)
	{
		CString strSaveFile = dlgSave.GetPathName();
		SetDlgItemText(IDC_EDIT_LOGFILE, strSaveFile);
		_tcscpy(m_tcLogFile, (LPCTSTR)strSaveFile);
	}
}
//------------------------------------------------------------
void CTestHTTPDlg::OnBnClickedRadioSingle()
{
	CButton* pBtnSingle = ((CButton*)GetDlgItem(IDC_RADIO_SINGLE));
	if (pBtnSingle->GetCheck() == BST_CHECKED)
	{
		m_dwThreadCnt = 1;
		GetDlgItem(IDC_STATIC_THREADCNT)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_THREADCNT)->EnableWindow(FALSE);
	}
}
//------------------------------------------------------------
void CTestHTTPDlg::OnBnClickedRadioMulti()
{
	CButton* pBtnMulti = ((CButton*)GetDlgItem(IDC_RADIO_MULTI));
	if (pBtnMulti->GetCheck() == BST_CHECKED)
	{
		GetDlgItem(IDC_STATIC_THREADCNT)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_THREADCNT)->EnableWindow(TRUE);
		///
		CComboBox* pThreadCntCombo = (CComboBox*)GetDlgItem(IDC_COMBO_THREADCNT);
		int iIndex = pThreadCntCombo->GetCurSel();
		m_dwThreadCnt = pThreadCntCombo->GetItemData(iIndex);
	}
}
//------------------------------------------------------------
void CTestHTTPDlg::OnCbnSelchangeComboThreadcnt()
{		
	CComboBox* pThreadCntCombo = (CComboBox*)GetDlgItem(IDC_COMBO_THREADCNT);
	int iIndex = pThreadCntCombo->GetCurSel();
	m_dwThreadCnt = pThreadCntCombo->GetItemData(iIndex);
}
//------------------------------------------------------------
//	Name:OnHttpDownloadEvent
//	Introduce:A download event arrived
//------------------------------------------------------------
long CTestHTTPDlg::OnHttpDownloadEvent(	LONG lTaskID, 
										LONG lThreadID,
										DWORD dwEvent, 
										DWORD dwParam1, 
										DWORD dwParam2)
{
	switch(dwEvent)
	{
	case eHTTPCBEvent_StatusChanged:
		PostMessage(WM_MSG_TASKSTATUSCHANGED, lTaskID, dwParam1);
		break;
	case eHTTPCBEvent_ProgressChanged:
		PostMessage(WM_MSG_TASKPROGRESS, dwParam1, dwParam2);
		break;
	case eHTTPCBEvent_Error:
		PostMessage(WM_MSG_TASKSERROR, lTaskID, dwParam1);
		break;
	}

	return 0;
}
//------------------------------------------------------------
LONG CTestHTTPDlg::OnTaskStatusChange(WPARAM wParam, LPARAM lParam)
{
	if (lParam == eHttpTaskStatus_Running)
	{
		UpdateTaskProgress(0);
	}
	else if (lParam == eHttpTaskStatus_Completed)
	{			
		m_pHttpDownloader->DeleteTask(wParam);
		MessageBox(_T("File has been downloaded!"), NULL, MB_ICONINFORMATION|MB_OK);
		//
		UpdateSettingControls(TRUE);
		CProgressCtrl* pProgress = (CProgressCtrl*)GetDlgItem(IDC_PROGRESS);
		pProgress->SetPos(0);
		SetDlgItemText(IDC_STATIC_PERCENT, _T("0%"));
		SetDlgItemText(IDC_STATIC_PROGRESS, _T("0 KB"));
	}

	return 0;
}
//------------------------------------------------------------
LONG CTestHTTPDlg::OnTaskProgress(WPARAM wParam, LPARAM lParam)
{
	UpdateTaskProgress(wParam);
	return 0;
}
//------------------------------------------------------------
LONG CTestHTTPDlg::OnTaskError(WPARAM wParam, LPARAM lParam)
{
	TCHAR tcErrorMsg[256] = {_T("\0")};
	_stprintf(tcErrorMsg, _T("Task ID: %d occurred an error!\nError Code: %d"), wParam, lParam);
	MessageBox(tcErrorMsg, NULL, MB_ICONERROR|MB_OK);
	return 0;
}
//------------------------------------------------------------



//------------------------------------------------------------
void CTestHTTPDlg::UpdateSettingControls(BOOL bEnable)
{
	GetDlgItem(IDC_RADIO_SINGLE)->EnableWindow(bEnable);
	GetDlgItem(IDC_RADIO_MULTI)->EnableWindow(bEnable);
	GetDlgItem(IDC_STATIC_THREADCNT)->EnableWindow(bEnable);
	GetDlgItem(IDC_COMBO_THREADCNT)->EnableWindow(bEnable);
	GetDlgItem(IDC_STATIC_LOGFILE)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_LOGFILE)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_SAVELOG)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_DOWNLOAD)->EnableWindow(bEnable);
}
//------------------------------------------------------------
void CTestHTTPDlg::UpdateTaskProgress(DWORD dwCurFinished)
{
	if (m_lFileSize <= 0)
		return;

	float fPerFinished = ((float)dwCurFinished / m_lFileSize) * 100;
	CProgressCtrl* pProgress = (CProgressCtrl*)GetDlgItem(IDC_PROGRESS);
	pProgress->SetPos((int)(fPerFinished));

	CString strPercent;
	strPercent.Format(_T("%.2f%s"), fPerFinished, _T("%"));
	SetDlgItemText(IDC_STATIC_PERCENT, strPercent);

	CString strInfo;
	strInfo.Format(_T("%.2f KB of %.2f KB"), dwCurFinished/1024.0f, m_lFileSize/1024.0f);
	SetDlgItemText(IDC_STATIC_PROGRESS, strInfo);
}
//------------------------------------------------------------