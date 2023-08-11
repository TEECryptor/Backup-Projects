//--------------------------------------------
//	File Name:TestHTTPDownloadDlg.cpp
//	Introduce:The implement file of class CTestHTTPDownloadDlg
//	Date:2010-05-10
//--------------------------------------------
#include "stdafx.h"
#include "TestHTTPDownload.h"
#include "TestHTTPDownloadDlg.h"
#include "NewTaskDlg.h"
#include "OptionsDlg.h"
#include "TaskSettingDlg.h"
//--------------------------------------------
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//--------------------------------------------
//	Name:HttpCallbackFunc
//	Introduce:The download callback function
//--------------------------------------------
long HttpCallbackFunc(LONG lTaskID, LONG lThreadID, DWORD dwEvent, DWORD dwParam1, DWORD dwParam2, LPVOID lpUser)
{
	if (NULL != lpUser)
		return ((CTestHTTPDownloadDlg*)lpUser)->OnHttpDownloadEvent(lTaskID, lThreadID, dwEvent, dwParam1, dwParam2);

	return 0;
}
//--------------------------------------------



//--------------------------------------------
// CAboutDlg dialog used for App About
//--------------------------------------------
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
//--------------------------------------------
CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}
//--------------------------------------------
void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}
//--------------------------------------------
BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()
//--------------------------------------------




//--------------------------------------------
CTestHTTPDownloadDlg::CTestHTTPDownloadDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestHTTPDownloadDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_dwMaxThreadCnt = 5;
}
//--------------------------------------------
void CTestHTTPDownloadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}
//--------------------------------------------
BEGIN_MESSAGE_MAP(CTestHTTPDownloadDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_NEW, OnBnClickedBtnNew)
	ON_BN_CLICKED(IDC_BTN_START, OnBnClickedBtnStart)
	ON_BN_CLICKED(IDC_BTN_STOP, OnBnClickedBtnStop)
	ON_BN_CLICKED(IDC_BTN_DELETE, OnBnClickedBtnDelete)
	ON_BN_CLICKED(IDC_BTN_OPTION, OnBnClickedBtnOption)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_READFILE, OnBnClickedBtnReadfile)
	ON_BN_CLICKED(IDC_BTN_SETTING, OnBnClickedBtnSetting)
END_MESSAGE_MAP()
//--------------------------------------------
BOOL CTestHTTPDownloadDlg::OnInitDialog()
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

	// Intialize UI
	CListCtrl* pTaskList = (CListCtrl*)GetDlgItem(IDC_LIST_TASKS);
	pTaskList->SetExtendedStyle(/*pTaskList->GetExtendedStyle() | */LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_SHOWSELALWAYS);
	pTaskList->InsertColumn(0, _T("Task ID"),			LVCFMT_LEFT, 50);
	pTaskList->InsertColumn(1, _T("Thread ID"),			LVCFMT_LEFT, 50);
	pTaskList->InsertColumn(2, _T("Status"),			LVCFMT_LEFT, 60);
	pTaskList->InsertColumn(3, _T("URL"),				LVCFMT_LEFT, 180);
	pTaskList->InsertColumn(4, _T("Target File"),		LVCFMT_LEFT, 150);
	pTaskList->InsertColumn(5, _T("Total Size"),		LVCFMT_LEFT, 80);
	pTaskList->InsertColumn(6, _T("Downloaded Size"),	LVCFMT_LEFT, 100);
	pTaskList->InsertColumn(7, _T("%"),					LVCFMT_LEFT, 80);
	pTaskList->InsertColumn(8, _T("Thread Cnt"),		LVCFMT_LEFT, 80);
	pTaskList->InsertColumn(9, _T("Speed (Sec)"),		LVCFMT_LEFT, 80);

	// Create Http downloader object
	LONG lRes = HTTP_E_NOERROR;
	m_pHttpDownloader = Create_HttpDownload();
	ASSERT(NULL != m_pHttpDownloader);

	// Call intialize function at first!
	lRes = m_pHttpDownloader->Initialize();
	if (lRes != HTTP_E_NOERROR)
	{
		CString strError;
		strError.Format(_T("Initialize http download failed!\nError code = %8x"), lRes);
		MessageBox(strError);
		return TRUE;
	}

	// Set configure data to downloader
	HttpDownloadCfg cfg;
	memset(&cfg, 0, sizeof(cfg));
	cfg.dwMaxTaskCnt = 5;
	cfg.dwMaxThreadCnt = 5;
	cfg.bForceUseSingleThread = TRUE;
	cfg.lpHttpCallbackFunc = HttpCallbackFunc;
	cfg.lpUser = this;
	lRes = m_pHttpDownloader->Configure(cfg);
	m_pHttpDownloader->SetLogFile(TRUE, _T("C:\\HttpLog.txt"));
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}
//--------------------------------------------
void CTestHTTPDownloadDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//--------------------------------------------
// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.
//--------------------------------------------
void CTestHTTPDownloadDlg::OnPaint() 
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
//--------------------------------------------
// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
//--------------------------------------------
HCURSOR CTestHTTPDownloadDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
//--------------------------------------------
void CTestHTTPDownloadDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// Stop all task before windows close
	m_pHttpDownloader->StopAllTasks();

	// Release Http download object
	Destroy_HttpDownload(m_pHttpDownloader);
	m_pHttpDownloader = NULL;
}
//--------------------------------------------
void CTestHTTPDownloadDlg::OnBnClickedOk()
{
	OnOK();
}
//--------------------------------------------




//--------------------------------------------
//	Name:OnHttpDownloadEvent
//	Introduce:A download event arrived
//--------------------------------------------
long CTestHTTPDownloadDlg::OnHttpDownloadEvent(LONG lTaskID, 
											   LONG lThreadID,
											   DWORD dwEvent, 
											   DWORD dwParam1, 
											   DWORD dwParam2)
{
	switch(dwEvent)
	{
	case eHTTPCBEvent_StatusChanged:
		UpdateTaskStatus(lTaskID, lThreadID, dwParam1);
		break;
	case eHTTPCBEvent_ProgressChanged:
		UpdateTaskProgress(lTaskID, lThreadID, dwParam1, dwParam2);
		break;
	case eHTTPCBEvent_Error:
		{
			TCHAR tcErrorMsg[256] = {_T("\0")};
			_stprintf(tcErrorMsg, _T("Task ID: %d occurred an error!\nError Code: %d"), lTaskID, dwParam1);
			MessageBox(tcErrorMsg);
		}
		break;
	}

	return 0;
}
//--------------------------------------------
void CTestHTTPDownloadDlg::OnBnClickedBtnOption()
{
	COptionsDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		HttpDownloadCfg cfg;
		memset(&cfg, 0, sizeof(cfg));
		cfg.dwMaxTaskCnt = dlg.m_dwMaxTaskCnt;
		cfg.dwMaxThreadCnt = dlg.m_dwMaxThreadCnt;
		cfg.lpHttpCallbackFunc = HttpCallbackFunc;
		cfg.lpUser = this;
		m_pHttpDownloader->Configure(cfg);

		m_dwMaxThreadCnt = dlg.m_dwMaxThreadCnt;
	}
}
//--------------------------------------------
void CTestHTTPDownloadDlg::OnBnClickedBtnNew()
{
	CNewTaskDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		LONG lRes = 0;

		if (TRUE == dlg.m_bIsStreamTask)
		{
			// Create a stream task
			lRes = m_pHttpDownloader->CreateStreamTask(	dlg.m_tcNetFileURL, 
														dlg.m_tcTargetFile, 
														dlg.m_bForceDownload);
		}
		else
		{
			// Create a normal task
			lRes = m_pHttpDownloader->CreateTask(	dlg.m_tcNetFileURL, 
													dlg.m_tcTargetFile, 
													dlg.m_dwThreadCnt,
													dlg.m_bForceDownload);
		}

		if (lRes > 0)
		{
			// If result > 0, create successfuly and the result is the new task ID
			LONG lTaskID = lRes;
			AddNewTaskToList(lTaskID, dlg.m_bIsStreamTask);

			// For stream task, add download thread in application
			if (TRUE == dlg.m_bIsStreamTask)
			{
				for (ThreadInfoList::iterator it = dlg.m_lstThreadInfo.begin();
					it != dlg.m_lstThreadInfo.end();
					it++)
				{
					StreamTaskThreadInfo ThreadInfo = *it;
					lRes = m_pHttpDownloader->AddStreamTaskThread(lTaskID, ThreadInfo.dwThreadID, ThreadInfo.dwStartPos, ThreadInfo.dwDataLength);
					if (HTTP_E_NOERROR == lRes)
					{						
						AddNewThreadToList(lTaskID, ThreadInfo.dwThreadID);
					}
					else
					{
						CString strError;
						strError.Format(_T("Add thread %d:[%d-%d] to steam task failed!\nError code = %8x"), ThreadInfo.dwThreadID, ThreadInfo.dwStartPos, ThreadInfo.dwStartPos+ThreadInfo.dwDataLength, lRes);
						MessageBox(strError);			
					}
				}
			}
		}
		else
		{
			// Otherwise, it is failed and the result is an error code
			CString strError;
			strError.Format(_T("Create new task failed!\nError code = %8x"), lRes);
			MessageBox(strError);			
		}
	}
}
//--------------------------------------------
void CTestHTTPDownloadDlg::OnBnClickedBtnSetting()
{
	BOOL bIsStreamTask = FALSE;
	DWORD dwSelTaskID = 0;

	// Check which task item is selected
	CListCtrl* pTaskList = (CListCtrl*)GetDlgItem(IDC_LIST_TASKS);
	DWORD dwItemCnt = pTaskList->GetItemCount();
	for (DWORD dwIndex = 0; dwIndex < dwItemCnt; dwIndex++)
	{
		if (LVIS_SELECTED == pTaskList->GetItemState(dwIndex, LVIS_SELECTED))
		{
			bIsStreamTask = pTaskList->GetItemData(dwIndex) == 1 ? TRUE : FALSE;

			TCHAR tcTaskID[32] = {_T("\0")};
			pTaskList->GetItemText(dwIndex, 0, tcTaskID, 32);
			dwSelTaskID = (DWORD)_ttol(tcTaskID);
			break;
		}
	}

	// No any task is selected in list
	if (0 == dwSelTaskID)
	{
		MessageBox(_T("Please select a task you want to setting!"));
		return;
	}

	// Can't do a setting for a stream task
	if (TRUE == bIsStreamTask)
	{
		MessageBox(_T("Can't do setting for a stream task!"));
		return;
	}

	// Get current task status
	LONG lStatus = m_pHttpDownloader->GetTaskStatus(dwSelTaskID);
	if (lStatus < 0)
	{
		CString strError;
		strError.Format(_T("Get task (%d) status failed!\nError code = %8x"), dwSelTaskID, lStatus);
		MessageBox(strError);			
	}

	// Can't do a setting for a running task
	if (eHttpTaskStatus_Running == lStatus)
	{
		MessageBox(_T("Can't do setting for a running task! Please stop it at first."));
		return;
	}

	// Get current thread count in this task
	DWORD dwThreadCnt = 0;
	TCHAR tcNetFileURL[1024] = {_T("\0")};
	TCHAR tcTargetFile[MAX_PATH] = {_T("\0")};
	m_pHttpDownloader->GetTaskInfo(dwSelTaskID, &dwThreadCnt, tcNetFileURL, tcTargetFile);

	CTaskSettingDlg dlg;
	dlg.m_dwMaxThreadCnt = m_dwMaxThreadCnt;
	dlg.m_dwSettingThreadCnt = dwThreadCnt;
	if (dlg.DoModal() == IDOK)
	{
		LONG lRes = m_pHttpDownloader->SetTaskThreadCnt(dwSelTaskID, dlg.m_dwSettingThreadCnt);
		if (lRes != HTTP_E_NOERROR)
		{
			CString strError;
			strError.Format(_T("Set task (%d) thread count failed!\nError code = %8x"), dwSelTaskID, lRes);
			MessageBox(strError);	
		}
		else
		{
			UpdateTaskThreadCnt(dwSelTaskID, dlg.m_dwSettingThreadCnt);
		}
	}
}
//--------------------------------------------
void CTestHTTPDownloadDlg::OnBnClickedBtnStart()
{
	BOOL bIsStreamTask = FALSE;
	DWORD dwSelTaskID = 0;
	DWORD dwThreadID = 0;

	// Check which task item is selected
	CListCtrl* pTaskList = (CListCtrl*)GetDlgItem(IDC_LIST_TASKS);
	DWORD dwItemCnt = pTaskList->GetItemCount();
	for (DWORD dwIndex = 0; dwIndex < dwItemCnt; dwIndex++)
	{
		if (LVIS_SELECTED == pTaskList->GetItemState(dwIndex, LVIS_SELECTED))
		{
			bIsStreamTask = pTaskList->GetItemData(dwIndex) == 1 ? TRUE : FALSE;

			TCHAR tcTaskID[32] = {_T("\0")};
			pTaskList->GetItemText(dwIndex, 0, tcTaskID, 32);
			dwSelTaskID = (DWORD)_ttol(tcTaskID);
			if (TRUE == bIsStreamTask)
			{
				TCHAR tcThreadID[32] = {_T("\0")};
				pTaskList->GetItemText(dwIndex, 1, tcThreadID, 32);
				dwThreadID = (DWORD)_ttol(tcThreadID);
			}
			break;
		}
	}

	// No any task is selected in list
	if (0 == dwSelTaskID)
	{
		MessageBox(_T("Please select a task you want to start!"));
		return;
	}

	// Try to start the select task
	LONG lRes = HTTP_E_NOERROR;
	if (FALSE == bIsStreamTask || 
		(TRUE == bIsStreamTask && 0 == dwThreadID))
		lRes = m_pHttpDownloader->StartTask(dwSelTaskID);
	else
		lRes = m_pHttpDownloader->StartStreamTaskThread(dwSelTaskID, dwThreadID);
	if (HTTP_E_NOERROR != lRes)
	{
		CString strError;
		strError.Format(_T("Start task: %d failed!\nError code = %8x"), dwSelTaskID, lRes);
		MessageBox(strError);	
	}
}
//--------------------------------------------
void CTestHTTPDownloadDlg::OnBnClickedBtnStop()
{
	BOOL bIsStreamTask = FALSE;
	DWORD dwSelTaskID = 0;
	DWORD dwThreadID = 0;

	// Check which task item is selected
	CListCtrl* pTaskList = (CListCtrl*)GetDlgItem(IDC_LIST_TASKS);
	DWORD dwItemCnt = pTaskList->GetItemCount();
	for (DWORD dwIndex = 0; dwIndex < dwItemCnt; dwIndex++)
	{
		if (LVIS_SELECTED == pTaskList->GetItemState(dwIndex, LVIS_SELECTED))
		{
			bIsStreamTask = pTaskList->GetItemData(dwIndex) == 1 ? TRUE : FALSE;

			TCHAR tcTaskID[32] = {_T("\0")};
			pTaskList->GetItemText(dwIndex, 0, tcTaskID, 32);
			dwSelTaskID = (DWORD)_ttol(tcTaskID);
			if (TRUE == bIsStreamTask)
			{
				TCHAR tcThreadID[32] = {_T("\0")};
				pTaskList->GetItemText(dwIndex, 1, tcThreadID, 32);
				dwThreadID = (DWORD)_ttol(tcThreadID);
			}
			break;
		}
	}

	// No any task is selected in list
	if (0 == dwSelTaskID)
	{
		MessageBox(_T("Please select a task you want to stop!"));
		return;
	}

	LONG lRes = HTTP_E_NOERROR;

	// Try to stop the select task
	if (FALSE == bIsStreamTask || 0 == dwThreadID)
	{
		lRes = m_pHttpDownloader->StopTask(dwSelTaskID);
	}
	// Try to stop the select thread
	else
	{
		lRes = m_pHttpDownloader->StopStreamTaskThread(dwSelTaskID, dwThreadID);
	}
	if (HTTP_E_NOERROR != lRes)
	{
		CString strError;
		strError.Format(_T("Stop task: %d failed!\nError code = %8x"), dwSelTaskID, lRes);
		MessageBox(strError);	
	}
}
//--------------------------------------------
void CTestHTTPDownloadDlg::OnBnClickedBtnDelete()
{
	BOOL bIsStreamTask = FALSE;
	DWORD dwSelTaskID = 0;
	DWORD dwSelThreadID = 0;

	// Check which task item is selected
	CListCtrl* pTaskList = (CListCtrl*)GetDlgItem(IDC_LIST_TASKS);
	DWORD dwItemCnt = pTaskList->GetItemCount();
	for (DWORD dwIndex = 0; dwIndex < dwItemCnt; dwIndex++)
	{
		if (LVIS_SELECTED == pTaskList->GetItemState(dwIndex, LVIS_SELECTED))
		{
			bIsStreamTask = pTaskList->GetItemData(dwIndex) == 1 ? TRUE : FALSE;

			TCHAR tcTaskID[32] = {_T("\0")};
			pTaskList->GetItemText(dwIndex, 0, tcTaskID, 32);
			dwSelTaskID = (DWORD)_ttol(tcTaskID);
			if (TRUE == bIsStreamTask)
			{			
				TCHAR tcThreadID[32] = {_T("\0")};
				pTaskList->GetItemText(dwIndex, 1, tcThreadID, 32);
				dwSelThreadID = (DWORD)_ttol(tcThreadID);
			}
			break;
		}
	}

	// No any task is selected in list
	if (0 == dwSelTaskID)
	{
		MessageBox(_T("Please select a task you want to delete!"));
		return;
	}

	LONG lRes = HTTP_E_NOERROR;

	// Try to delte the select task
	if (FALSE == bIsStreamTask || 0 == dwSelThreadID)
	{
		// Ask user to delete the downloaded file or not
		lRes = MessageBox(_T("The selected task will be deleted!\nWould you also want to delete the downloaded file?"), NULL, MB_YESNO);

		if (FALSE == bIsStreamTask)
			lRes = m_pHttpDownloader->DeleteTask(dwSelTaskID, lRes == IDYES ? TRUE : FALSE);
		else 
			lRes = m_pHttpDownloader->DeleteStreamTask(dwSelTaskID, lRes == IDYES ? TRUE : FALSE);
	}
	// Try to delte the select thread
	else
	{
		lRes = m_pHttpDownloader->DelStreamTaskThread(dwSelTaskID, dwSelThreadID);
	}

	if (HTTP_E_NOERROR != lRes)
	{
		CString strError;
		strError.Format(_T("Stop task: %d failed!\nError code = %8x"), dwSelTaskID, lRes);
		MessageBox(strError);
	}

	// Delete task from list
	DelTaskFromList(dwSelTaskID, bIsStreamTask ? dwSelThreadID : -1);
}
//--------------------------------------------
/* 
  Test read the downloading file of stream task
 */
void CTestHTTPDownloadDlg::OnBnClickedBtnReadfile()
{
	BOOL bIsStreamTask = FALSE;
	DWORD dwSelTaskID = 0;
	DWORD dwSelThreadID = 0;

	// Check which task item is selected
	CListCtrl* pTaskList = (CListCtrl*)GetDlgItem(IDC_LIST_TASKS);
	DWORD dwItemCnt = pTaskList->GetItemCount();
	for (DWORD dwIndex = 0; dwIndex < dwItemCnt; dwIndex++)
	{
		if (LVIS_SELECTED == pTaskList->GetItemState(dwIndex, LVIS_SELECTED))
		{
			bIsStreamTask = pTaskList->GetItemData(dwIndex) == 1 ? TRUE : FALSE;

			TCHAR tcTaskID[32] = {_T("\0")};
			pTaskList->GetItemText(dwIndex, 0, tcTaskID, 32);
			dwSelTaskID = (DWORD)_ttol(tcTaskID);
			if (TRUE == bIsStreamTask)
			{			
				TCHAR tcThreadID[32] = {_T("\0")};
				pTaskList->GetItemText(dwIndex, 1, tcThreadID, 32);
				dwSelThreadID = (DWORD)_ttol(tcThreadID);
			}
			break;
		}
	}

	// No any task is selected in list
	if (0 == dwSelTaskID || FALSE == bIsStreamTask)
	{
		MessageBox(_T("Please select a stream task to test reading file during downloading!"));
		return;
	}

	// Get stream task target file name
	DWORD dwThreadCnt = 0;
	TCHAR tcNetFileURL[1024] = {_T("\0")};
	TCHAR tcTargetFile[MAX_PATH] = {_T("\0")};
	m_pHttpDownloader->GetTaskInfo(dwSelTaskID, &dwThreadCnt, tcNetFileURL, tcTargetFile);

	// If stream task is not completed, use the project file name
	LONG lStatus = m_pHttpDownloader->GetTaskStatus(dwSelTaskID);
	if (eHttpTaskStatus_Completed != lStatus)
		_tcscat(tcTargetFile, HTTP_DOWNLOADING_EXT);

	// Try open file
	HANDLE hFile = CreateFile(	tcTargetFile,
								GENERIC_READ,
								FILE_SHARE_READ | FILE_SHARE_WRITE,
								NULL,
								OPEN_ALWAYS,
								FILE_ATTRIBUTE_NORMAL,
								NULL);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		DWORD dwError = 0;
		dwError = GetLastError();
		return;
	}

	// Try to read some data to buffer
	DWORD dwReadBytes = 0;
	BYTE byDataBuff[1024] = {_T("\0")};
	ReadFile(hFile, byDataBuff, 1024, &dwReadBytes, NULL);

	// Here, add your code to check the data and use it.

	// After read file data to buffer, close it
	CloseHandle(hFile);
	hFile = NULL;
}
//--------------------------------------------



//--------------------------------------------
void CTestHTTPDownloadDlg::AddNewTaskToList(LONG lTaskID, BOOL bIsStreamTask)
{
	DWORD dwTotalSize = 0;
	DWORD dwThreadCnt = 0;
	DWORD dwFinisedBytes = 0;
	DWORD dwCurrentSpeed = 0;
	TCHAR tcNetFileURL[1024] = {_T("\0")};
	TCHAR tcTargetFile[MAX_PATH] = {_T("\0")};
	dwTotalSize = m_pHttpDownloader->GetTaskNetFileSize(lTaskID);
	m_pHttpDownloader->GetTaskInfo(lTaskID, &dwThreadCnt, tcNetFileURL, tcTargetFile);
	m_pHttpDownloader->GetTaskProgress(lTaskID, dwFinisedBytes, dwCurrentSpeed);

	TCHAR tcTempStr[32] = {_T("\0")};
	CListCtrl* pTaskList = (CListCtrl*)GetDlgItem(IDC_LIST_TASKS);
	DWORD dwCurCount = pTaskList->GetItemCount();

	TCHAR tcPercent[8] = {_T("\0")};
	float fPercent = (dwFinisedBytes * 100.0f) / dwTotalSize;
	_stprintf(tcPercent, _T("%.2f%s"), fPercent, _T("%"));

	// Task ID
	_stprintf(tcTempStr, _T("%d"), lTaskID);
	int iIndex = pTaskList->InsertItem(dwCurCount, tcTempStr);
	pTaskList->SetItemData(iIndex, bIsStreamTask ? 1 : 0);	// Flag this item is a stream task item or not

	// Task thread ID, only for stream task
	pTaskList->SetItemText(dwCurCount, 1, _T("All"));

	// Task status, set READY for all new task
	pTaskList->SetItemText(dwCurCount, 2, GetStatusText(eHttpTaskStatus_Ready));

	// URL string
	pTaskList->SetItemText(dwCurCount, 3, tcNetFileURL);

	// Target file name
	pTaskList->SetItemText(dwCurCount, 4, tcTargetFile);

	// File total size
	pTaskList->SetItemText(dwCurCount, 5, GetBytesSizeText(dwTotalSize));

	// The downloaded size, 0 for new task
	pTaskList->SetItemText(dwCurCount, 6, GetBytesSizeText(dwFinisedBytes));

	// Current percent, 0 for new task
	pTaskList->SetItemText(dwCurCount, 7, tcPercent);

	// Thread count of this task
	_stprintf(tcTempStr, _T("%d"), dwThreadCnt);
	pTaskList->SetItemText(dwCurCount, 8, tcTempStr);

	// Current download speed, 0 for new task
	pTaskList->SetItemText(dwCurCount, 9, GetBytesSizeText(dwCurrentSpeed));
}
//--------------------------------------------
void CTestHTTPDownloadDlg::AddNewThreadToList(LONG lTaskID, LONG lThreadID)
{
	DWORD dwThreadCnt = 0;
	TCHAR tcNetFileURL[1024] = {_T("\0")};
	TCHAR tcTargetFile[MAX_PATH] = {_T("\0")};
	m_pHttpDownloader->GetTaskInfo(lTaskID, &dwThreadCnt, tcNetFileURL, tcTargetFile);

	DWORD dwStartPos = 0;
	DWORD dwDataSize = 0;
	DWORD dwFinished = 0;
	m_pHttpDownloader->GetStreamTaskThreadInfo(lTaskID, lThreadID, dwStartPos, dwDataSize, dwFinished);

	TCHAR tcTempStr[32] = {_T("\0")};
	CListCtrl* pTaskList = (CListCtrl*)GetDlgItem(IDC_LIST_TASKS);
	DWORD dwCurCount = pTaskList->GetItemCount();

	TCHAR tcPercent[8] = {_T("\0")};
	float fPercent = (dwFinished * 100.0f) / dwDataSize;
	_stprintf(tcPercent, _T("%.2f%s"), fPercent, _T("%"));

	// Task ID
	_stprintf(tcTempStr, _T("%d"), lTaskID);
	int iIndex = pTaskList->InsertItem(dwCurCount, tcTempStr);
	pTaskList->SetItemData(iIndex, 1);	// Flag this item is a stream task item

	// Task thread ID, only for stream task
	_stprintf(tcTempStr, _T("%d"), lThreadID);
	pTaskList->SetItemText(dwCurCount, 1, tcTempStr);

	// Task status, set READY for all new task
	pTaskList->SetItemText(dwCurCount, 2, GetStatusText(eHttpTaskStatus_Ready));

	// URL string
	pTaskList->SetItemText(dwCurCount, 3, tcNetFileURL);

	// Target file name
	pTaskList->SetItemText(dwCurCount, 4, tcTargetFile);

	// Thread total size
	pTaskList->SetItemText(dwCurCount, 5, GetBytesSizeText(dwDataSize));

	// The downloaded size
	pTaskList->SetItemText(dwCurCount, 6, GetBytesSizeText(dwFinished));

	// Current percent, 0 for new task
	pTaskList->SetItemText(dwCurCount, 7, tcPercent);

	// Thread count of this task
	_stprintf(tcTempStr, _T("%d"), 1);
	pTaskList->SetItemText(dwCurCount, 8, tcTempStr);

	// Current download speed, 0 for new task
	pTaskList->SetItemText(dwCurCount, 9, GetBytesSizeText(0));

	/* Update parent task thread count*/	
	DWORD dwItemCnt = pTaskList->GetItemCount();
	for (DWORD dwIndex = 0; dwIndex < dwItemCnt; dwIndex++)
	{
		BOOL bIsStreamTask = pTaskList->GetItemData(dwIndex) == 1 ? TRUE : FALSE;
		if (FALSE == bIsStreamTask)
			continue;

		TCHAR tcTaskID[32] = {_T("\0")};
		TCHAR tcThreadID[32] = {_T("\0")};
		pTaskList->GetItemText(dwIndex, 0, tcTaskID, 32);
		pTaskList->GetItemText(dwIndex, 1, tcThreadID, 32);
		DWORD dwParentTaskID = (DWORD)_ttol(tcTaskID);
		DWORD dwThreadID = (DWORD)_ttol(tcThreadID);
		if (dwParentTaskID == lTaskID && 0 == dwThreadID)
		{
			_stprintf(tcTempStr, _T("%d"), dwThreadCnt);
			pTaskList->SetItemText(dwIndex, 8, tcTempStr);
		}
	}
}
//--------------------------------------------
void CTestHTTPDownloadDlg::DelTaskFromList(LONG lTaskID, LONG lThreadID)
{
	CListCtrl* pTaskList = (CListCtrl*)GetDlgItem(IDC_LIST_TASKS);
	DWORD dwCurCount = pTaskList->GetItemCount();
	for (DWORD dwIndex = 0; dwIndex < dwCurCount; dwIndex++)
	{
		TCHAR tcItemTaskID[32] = {_T("\0")};
		pTaskList->GetItemText(dwIndex, 0, tcItemTaskID, 32);
		DWORD dwItemTaskID = (DWORD)_ttol(tcItemTaskID);
		// Is a normal task event
		if (-1 == lThreadID)
		{
			if (dwItemTaskID == lTaskID)
			{
				pTaskList->DeleteItem(dwIndex);
				break;
			}
		}
		// Is a stream task event
		else
		{
			TCHAR tcItemThreadID[32] = {_T("\0")};
			pTaskList->GetItemText(dwIndex, 1, tcItemThreadID, 32);
			DWORD dwItemThreadID = (DWORD)_ttol(tcItemThreadID);
			if (dwItemTaskID == lTaskID &&
				dwItemThreadID == lThreadID)
			{
				pTaskList->DeleteItem(dwIndex);
				break;
			}
		}
	}
}
//--------------------------------------------
void CTestHTTPDownloadDlg::UpdateTaskStatus(LONG lTaskID, 
											LONG lThreadID, 
											DWORD dwNewStatus)
{
	int iTaskItem = -1;
	CListCtrl* pTaskList = (CListCtrl*)GetDlgItem(IDC_LIST_TASKS);
	DWORD dwItemCount = pTaskList->GetItemCount();
	for (DWORD dwIndex = 0; dwIndex < dwItemCount; dwIndex++)
	{
		TCHAR tcItemTaskID[32] = {_T("\0")};
		pTaskList->GetItemText(dwIndex, 0, tcItemTaskID, 32);
		DWORD dwItemTaskID = (DWORD)_ttol(tcItemTaskID);
		// Is a normal task event
		if (-1 == lThreadID)
		{
			if (dwItemTaskID == lTaskID)
			{
				iTaskItem = dwIndex;
				break;
			}
		}
		// Is a stream task event
		else
		{
			TCHAR tcItemThreadID[32] = {_T("\0")};
			pTaskList->GetItemText(dwIndex, 1, tcItemThreadID, 32);
			DWORD dwItemThreadID = (DWORD)_ttol(tcItemThreadID);
			if (dwItemTaskID == lTaskID &&
				dwItemThreadID == lThreadID)
			{
				iTaskItem = dwIndex;
				break;
			}
		}
	}

	if (-1 == iTaskItem)
		return;

	pTaskList->SetItemText(iTaskItem, 2, GetStatusText(dwNewStatus));
	pTaskList->SetItemText(iTaskItem, 9, _T("0"));
	pTaskList->UpdateWindow();
}
//--------------------------------------------
void CTestHTTPDownloadDlg::UpdateTaskProgress(LONG lTaskID, 
											  LONG lThreadID, 
											  DWORD dwFinishedBytes, 
											  DWORD dwCurrentSpeed)
{	
	int iTaskItem = -1;
	CListCtrl* pTaskList = (CListCtrl*)GetDlgItem(IDC_LIST_TASKS);
	DWORD dwItemCount = pTaskList->GetItemCount();
	for (DWORD dwIndex = 0; dwIndex < dwItemCount; dwIndex++)
	{
		TCHAR tcItemTaskID[32] = {_T("\0")};
		pTaskList->GetItemText(dwIndex, 0, tcItemTaskID, 32);
		DWORD dwItemTaskID = (DWORD)_ttol(tcItemTaskID);
		// Is a normal task event
		if (-1 == lThreadID)
		{
			if (dwItemTaskID == lTaskID)
			{
				iTaskItem = dwIndex;
				break;
			}
		}
		// Is a stream task event
		else
		{
			TCHAR tcItemThreadID[32] = {_T("\0")};
			pTaskList->GetItemText(dwIndex, 1, tcItemThreadID, 32);
			DWORD dwItemThreadID = (DWORD)_ttol(tcItemThreadID);
			if (dwItemTaskID == lTaskID &&
				dwItemThreadID == lThreadID)
			{
				iTaskItem = dwIndex;
				break;
			}
		}
	}

	if (-1 == iTaskItem)
		return;

	DWORD dwTotalSize = 0;
	if (-1 == lThreadID)
	{
		dwTotalSize = m_pHttpDownloader->GetTaskNetFileSize(lTaskID);
	}
	else
	{
		DWORD dwStartPos = 0;
		DWORD dwFinished = 0;
		m_pHttpDownloader->GetStreamTaskThreadInfo(lTaskID, lThreadID, dwStartPos, dwTotalSize, dwFinished);
	}
	//
	TCHAR tcPercent[8] = {_T("\0")};
	dwTotalSize = dwTotalSize > 0 ? dwTotalSize : dwFinishedBytes;
	float fPercent = (dwFinishedBytes * 100.0f) / dwTotalSize;
	_stprintf(tcPercent, _T("%.2f%s"), fPercent, _T("%"));

	pTaskList->SetItemText(iTaskItem, 6, GetBytesSizeText(dwFinishedBytes));
	pTaskList->SetItemText(iTaskItem, 7, tcPercent);
	pTaskList->SetItemText(iTaskItem, 9, GetBytesSizeText(dwCurrentSpeed));
	pTaskList->UpdateWindow();
}	
//--------------------------------------------
void CTestHTTPDownloadDlg::UpdateTaskThreadCnt(LONG lTaskID, 
											   DWORD dwNewThreadCnt)
{	
	int iTaskItem = -1;
	CListCtrl* pTaskList = (CListCtrl*)GetDlgItem(IDC_LIST_TASKS);
	DWORD dwItemCount = pTaskList->GetItemCount();
	for (DWORD dwIndex = 0; dwIndex < dwItemCount; dwIndex++)
	{
		TCHAR tcItemTaskID[32] = {_T("\0")};
		pTaskList->GetItemText(dwIndex, 0, tcItemTaskID, 32);
		DWORD dwItemTaskID = (DWORD)_ttol(tcItemTaskID);
		if (dwItemTaskID == lTaskID)
		{
			iTaskItem = dwIndex;
			break;
		}
	}

	if (-1 == iTaskItem)
		return;

	// Thread count of this task
	CString strText;
	strText.Format(_T("%d"), dwNewThreadCnt);
	pTaskList->SetItemText(iTaskItem, 8, strText); 
	pTaskList->UpdateWindow();
}
//--------------------------------------------
LPCTSTR CTestHTTPDownloadDlg::GetStatusText(LONG lStatus)
{
	static TCHAR stcStatusText[64] = {_T("\0")};
	_tcscpy(stcStatusText, _T("\0"));

	switch(lStatus)
	{
	case eHttpTaskStatus_Ready:
		_tcscpy(stcStatusText, _T("Ready"));
		break;
	case eHttpTaskStatus_Waitting:
		_tcscpy(stcStatusText, _T("Waitting"));
		break;
	case eHttpTaskStatus_Running:
		_tcscpy(stcStatusText, _T("Running"));
		break;	
	case eHttpTaskStatus_Stopped:
		_tcscpy(stcStatusText, _T("Stopped"));
		break;	
	case eHttpTaskStatus_Completed:
		_tcscpy(stcStatusText, _T("Completed"));
		break;
	}

	return stcStatusText;
}
//--------------------------------------------
LPCTSTR CTestHTTPDownloadDlg::GetBytesSizeText(DWORD dwBytesSize)
{
	static TCHAR stcSizeText[64] = {_T("\0")};
	_tcscpy(stcSizeText, _T("\0"));

	int iChangeUnitCnt = 0;
	float fSize = (float)dwBytesSize;
	while(fSize >= 1024 && iChangeUnitCnt < 3) 
	{
		fSize /= 1024.0f;
		iChangeUnitCnt++;
	}
	switch(iChangeUnitCnt)
	{
	case 0:
		_stprintf(stcSizeText, _T("%d bytes"), (DWORD)fSize);
		break;
	case 1:
		_stprintf(stcSizeText, _T("%.02f KB"), fSize);
		break;
	case 2:
		_stprintf(stcSizeText, _T("%.02f MB"), fSize);
		break;
	case 3:
		_stprintf(stcSizeText, _T("%.02f GB"), fSize);
		break;
	}
	return stcSizeText;
}
//--------------------------------------------