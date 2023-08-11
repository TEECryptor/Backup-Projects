// SynoTFKeySDKTestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SynoTFKeySDKTest.h"
#include "SynoTFKeySDKTestDlg.h"
#include "LoginDlg.h"
#include "ChangePinDlg.h"
#include "UnlockPinDlg.h"
#include "NewDataFileDlg.h"
#include "PlainTextDlg.h"
#include "InputFileNameDlg.h"
#include "DevInitDlg.h"
#include "./TFKeySDKTester/SkfSDKTester.h"
#include "./TFKeySDKTester/Pkcs11SDKTester.h"
#include "../Base64/Base64.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


ULONG g_ulErrMsgLen = 512;
CHAR g_csErrMsg[512] = {0};

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


// CSynoTFKeySDKTestDlg dialog


ULONG CALLBACK MsgCallback(LPCTSTR lpcsResultMsg, LPVOID lpCaller)
{
	CSynoTFKeySDKTestDlg* pDlg = (CSynoTFKeySDKTestDlg*)lpCaller;
	if (pDlg)
	{
		return pDlg->SDKFuncMsgCallback(lpcsResultMsg);
	}

	return 0;
}

DWORD WINAPI EventThreadProc(LPVOID lpParam)
{
	CSynoTFKeySDKTestDlg *pCall = (CSynoTFKeySDKTestDlg*)lpParam;
	return pCall->ListenKeyEvents();
}

CSynoTFKeySDKTestDlg::CSynoTFKeySDKTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSynoTFKeySDKTestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pSDKTester = NULL;
	m_hSession = NULL;
	m_hSessionKey = NULL;
	m_bExitThread = NULL;
	m_bListenEvent = FALSE;
	m_hEventThread = NULL;
	m_ulEventThreadID = 0;
}

void CSynoTFKeySDKTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSynoTFKeySDKTestDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_MAINDLG_BTN_REFRESHLIST, &CSynoTFKeySDKTestDlg::OnBnClickedMaindlgBtnRefreshlist)
	ON_NOTIFY(TVN_SELCHANGED, IDC_MAINDLG_TREE_LIST, &CSynoTFKeySDKTestDlg::OnTvnSelchangedMaindlgTreeList)
	ON_BN_CLICKED(IDC_MAINDLG_BTN_DEVINFO, &CSynoTFKeySDKTestDlg::OnBnClickedMaindlgBtnDevinfo)
	ON_BN_CLICKED(IDC_MAINDLG_BTN_DEVINIT, &CSynoTFKeySDKTestDlg::OnBnClickedMaindlgBtnDevinit)
	ON_BN_CLICKED(IDC_MAINDLG_BTN_SETLABEL, &CSynoTFKeySDKTestDlg::OnBnClickedMaindlgBtnSetlabel)
	ON_BN_CLICKED(IDC_MAINDLG_BTN_DEVAUTH, &CSynoTFKeySDKTestDlg::OnBnClickedMaindlgBtnDevauth)
	ON_BN_CLICKED(IDC_MAINDLG_BTN_CHANGEAUTHKEY, &CSynoTFKeySDKTestDlg::OnBnClickedMaindlgBtnChangeauthkey)
	ON_BN_CLICKED(IDC_MAINDLG_CHECK_LISTENEVENT, &CSynoTFKeySDKTestDlg::OnBnClickedMaindlgCheckListenevent)
	ON_BN_CLICKED(IDC_MAINDLG_BTN_OPENSESSION, &CSynoTFKeySDKTestDlg::OnBnClickedMaindlgBtnOpensession)
	ON_BN_CLICKED(IDC_MAINDLG_BTN_CLOSESESSION, &CSynoTFKeySDKTestDlg::OnBnClickedMaindlgBtnClosesession)
	ON_BN_CLICKED(IDC_MAINDLG_BTN_SESSIONINFO, &CSynoTFKeySDKTestDlg::OnBnClickedMaindlgBtnSessioninfo)
	ON_BN_CLICKED(IDC_MAINDLG_BTN_LOGINSESSION, &CSynoTFKeySDKTestDlg::OnBnClickedMaindlgBtnLoginsession)
	ON_BN_CLICKED(IDC_MAINDLG_BTN_LOGOUTSESSION, &CSynoTFKeySDKTestDlg::OnBnClickedMaindlgBtnLogoutsession)
	ON_BN_CLICKED(IDC_MAINDLG_BTN_CHANGEPIN, &CSynoTFKeySDKTestDlg::OnBnClickedMaindlgBtnChangepin)
	ON_BN_CLICKED(IDC_MAINDLG_BTN_UNLOCKPIN, &CSynoTFKeySDKTestDlg::OnBnClickedMaindlgBtnUnlockpin)
	ON_BN_CLICKED(IDC_MAINDLG_BTN_GENKEYPAIR, &CSynoTFKeySDKTestDlg::OnBnClickedMaindlgBtnGenkeypair)
	ON_BN_CLICKED(IDC_MAINDLG_BTN_IMPORTKEYPAIR, &CSynoTFKeySDKTestDlg::OnBnClickedMaindlgBtnImportkeypair)
	ON_BN_CLICKED(IDC_MAINDLG_EXPORTSIGNPUBKEY, &CSynoTFKeySDKTestDlg::OnBnClickedMaindlgExportsignpubkey)
	ON_BN_CLICKED(IDC_MAINDLG_EXPORTEXCHPUBKEY, &CSynoTFKeySDKTestDlg::OnBnClickedMaindlgExportexchpubkey)
	ON_BN_CLICKED(IDC_MAINDLG_BTN_GENERATEKEY, &CSynoTFKeySDKTestDlg::OnBnClickedMaindlgBtnGeneratekey)
	ON_BN_CLICKED(IDC_MAINDLG_BTN_RELEASEKEY, &CSynoTFKeySDKTestDlg::OnBnClickedMaindlgBtnReleasekey)
	ON_BN_CLICKED(IDC_MAINDLG_BTN_CLEARINFO, &CSynoTFKeySDKTestDlg::OnBnClickedMaindlgBtnClearinfo)
	ON_BN_CLICKED(IDC_MAINDLG_BTN_SIGNVERIFY, &CSynoTFKeySDKTestDlg::OnBnClickedMaindlgBtnSignverify)
	ON_BN_CLICKED(IDC_MAINDLG_BTN_CRYPTO, &CSynoTFKeySDKTestDlg::OnBnClickedMaindlgBtnCrypto)
	ON_BN_CLICKED(IDC_MAINDLG_BTN_ENVENLOP, &CSynoTFKeySDKTestDlg::OnBnClickedMaindlgBtnEnvenlop)
	ON_BN_CLICKED(IDC_MAINDLG_BTN_IMPORTCERT, &CSynoTFKeySDKTestDlg::OnBnClickedMaindlgBtnImportcert)
	ON_BN_CLICKED(IDC_MAINDLG_BTN_EXPORTCERT, &CSynoTFKeySDKTestDlg::OnBnClickedMaindlgBtnExportcert)
	ON_BN_CLICKED(IDC_MAINDLG_BTN_ENUMFILES, &CSynoTFKeySDKTestDlg::OnBnClickedMaindlgBtnEnumfiles)
	ON_BN_CLICKED(IDC_MAINDLG_BTN_CREATEFILE, &CSynoTFKeySDKTestDlg::OnBnClickedMaindlgBtnCreatefile)
	ON_BN_CLICKED(IDC_MAINDLG_BTN_READFILE, &CSynoTFKeySDKTestDlg::OnBnClickedMaindlgBtnReadfile)
	ON_BN_CLICKED(IDC_MAINDLG_BTN_FILEINFO, &CSynoTFKeySDKTestDlg::OnBnClickedMaindlgBtnFileinfo)
END_MESSAGE_MAP()


// CSynoTFKeySDKTestDlg message handlers

BOOL CSynoTFKeySDKTestDlg::OnInitDialog()
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

	// ����SDK���Զ���ʵ��
	switch(m_eSelSDKType)
	{
	case SDK_TYPE_SKF:
		m_pSDKTester = new CSkfSDKTester();
		break;
	case SDK_TYPE_PKCS11:
		m_pSDKTester = new CPkcs11SDKTester();
		break;
	default:
		MessageBox(_T("SDK����ѡ�����"), _T("����"));
		PostQuitMessage(-1);
	}
		
	// ����SDK
	ULONG ulRes = m_pSDKTester->LoadDll(m_strDllFile, MsgCallback, this);
	if (USBKEY_ERR_OK != ulRes)
	{
		MessageBox(_T("����SDKʧ�ܣ�"), _T("����"));
		PostQuitMessage(-1);
	}

	// ���ÿؼ���ʼ״̬
	_SetControlInitStatus();

	// ö���豸	
	_EnumDevices();	

	// ����Event�����߳�
	m_hEventThread = CreateThread(NULL, NULL, EventThreadProc, this, NULL, &m_ulEventThreadID);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSynoTFKeySDKTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSynoTFKeySDKTestDlg::OnPaint()
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

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSynoTFKeySDKTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

ULONG CSynoTFKeySDKTestDlg::SDKFuncMsgCallback(LPCTSTR lpcsResultMsg)
{
	CListBox* pInfoMsgListBox = (CListBox*)GetDlgItem(IDC_MAINDLG_LIST_INFO);

	pInfoMsgListBox->AddString(lpcsResultMsg);	

	return 0;
}

ULONG CSynoTFKeySDKTestDlg::ListenKeyEvents()
{
	ULONG ulRes = 0;
	ULONG ulEvenType = 0;
	ULONG ulSlotID = 0;
	ULONG ulDevNameLen = 128;
	TCHAR tcDevName[128] = {0};
	CString strEvenInfo;

	while (1)
	{
		if (m_bExitThread)
			break;

		if (m_bListenEvent)
		{
			if (m_pSDKTester)
			{
				ulDevNameLen = 128;
				memset(tcDevName, 0, 128);
				ulRes = m_pSDKTester->WaitForDevEvent(tcDevName, &ulDevNameLen, &ulSlotID, &ulEvenType);
				if (USBKEY_ERR_OK == ulRes)
				{
					if (_tcslen(tcDevName) > 0)
					{
						strEvenInfo.Format(_T("Key %s is %s!\n"), tcDevName, (1==ulEvenType) ? _T("inserted") : _T("removed"));
					}
					else
					{
						strEvenInfo.Format(_T("Slot %d is %s!\n"), ulSlotID, (1==ulEvenType) ? _T("inserted") : _T("removed"));
					}
					SDKFuncMsgCallback(strEvenInfo);
					_EnumDevices();
				}
			}
		}
		else
		{
			Sleep(200);
		}
	}

	return 0;
}

void CSynoTFKeySDKTestDlg::OnDestroy()
{
	CDialog::OnDestroy();

	m_bExitThread = TRUE;
	CloseHandle(m_hEventThread);
	WaitForSingleObject(m_hEventThread, -1);
	m_hEventThread = NULL;

	if (m_pSDKTester)
	{
		delete m_pSDKTester;
		m_pSDKTester = NULL;
	}
}

void CSynoTFKeySDKTestDlg::OnBnClickedMaindlgBtnRefreshlist()
{
	_EnumDevices();	
}

void CSynoTFKeySDKTestDlg::OnTvnSelchangedMaindlgTreeList(NMHDR *pNMHDR, LRESULT *pResult)
{
	HTREEITEM hSelectedItem = NULL;
	CTreeCtrl* pDevTree = (CTreeCtrl*)GetDlgItem(IDC_MAINDLG_TREE_LIST);

	hSelectedItem = pDevTree->GetSelectedItem();
	if (hSelectedItem)
	{
		m_strSelDevName = pDevTree->GetItemText(hSelectedItem);
	}
	else
	{
		m_strSelDevName = _T("");
	}

	*pResult = 0;
}

void CSynoTFKeySDKTestDlg::OnBnClickedMaindlgBtnDevinfo()
{
	if (m_strSelDevName.GetLength() == 0)
	{
		MessageBox(_T("����ѡ���豸!"), _T("��ʾ"));
		return;
	}

	m_pSDKTester->GetDevInfo(m_strSelDevName);
}

void CSynoTFKeySDKTestDlg::OnBnClickedMaindlgBtnDevinit()
{	
	CDevInitDlg dlg;

	if (m_strSelDevName.GetLength() == 0)
	{
		MessageBox(_T("����ѡ���豸!"), _T("��ʾ"));
		return;
	}
	if (dlg.DoModal() != IDOK)
	{
		return;
	}

	ULONG ulRes = m_pSDKTester->Initialize(m_strSelDevName, dlg.m_strLabel, _T("SynoApp"), 
											dlg.m_strSOPin, dlg.m_ulSOPinRetryTimes, 
											dlg.m_strUserPin, dlg.m_ulUserPinRetryTimes);
	if (USBKEY_ERR_OK == ulRes)
	{
		MessageBox(_T("�豸��ʼ���ɹ�!"), _T("��ʾ"));
	}
	else
	{
		MessageBox(_T("�豸��ʼ��ʧ��!"), _T("����"));
	}
}

void CSynoTFKeySDKTestDlg::OnBnClickedMaindlgBtnSetlabel()
{
	MessageBox(_T("�ù�����δʵ��!"), _T("��ʾ"));
}

void CSynoTFKeySDKTestDlg::OnBnClickedMaindlgBtnDevauth()
{
	MessageBox(_T("�ù�����δʵ��!"), _T("��ʾ"));
}

void CSynoTFKeySDKTestDlg::OnBnClickedMaindlgBtnChangeauthkey()
{
	MessageBox(_T("�ù�����δʵ��!"), _T("��ʾ"));
}


void CSynoTFKeySDKTestDlg::OnBnClickedMaindlgCheckListenevent()
{
	CButton* pCheckBtn = (CButton*)GetDlgItem(IDC_MAINDLG_CHECK_LISTENEVENT);
	if (pCheckBtn->GetCheck() == BST_CHECKED)
	{
		m_bListenEvent = TRUE;
	}
	else
	{
		m_bListenEvent = FALSE;
		m_pSDKTester->CancelWaitForDevEvent();
	}
}


void CSynoTFKeySDKTestDlg::OnBnClickedMaindlgBtnOpensession()
{
	ULONG ulRes = 0;

	if (m_strSelDevName.GetLength() == 0)
	{
		MessageBox(_T("����ѡ���豸!"), _T("��ʾ"));
		return;
	}

	ulRes = m_pSDKTester->OpenSession(m_strSelDevName, &m_hSession);
	if (USBKEY_ERR_OK == ulRes)
	{
		MessageBox(_T("�򿪻Ự�ɹ�!"), _T("��ʾ"));
	}
	else
	{
		MessageBox(_T("�򿪻Ựʧ��!"), _T("����"));
	}
}


void CSynoTFKeySDKTestDlg::OnBnClickedMaindlgBtnClosesession()
{
	ULONG ulRes = 0;

	if (m_strSelDevName.GetLength() == 0)
	{
		MessageBox(_T("����ѡ���豸!"), _T("��ʾ"));
		return;
	}
	if (NULL == m_hSession)
	{
		MessageBox(_T("���ȴ򿪻Ự!"), _T("��ʾ"));
		return;
	}

	ulRes = m_pSDKTester->CloseSession(m_hSession);
	if (USBKEY_ERR_OK == ulRes)
	{
		MessageBox(_T("�رջỰ�ɹ�!"), _T("��ʾ"));
	}
	else
	{
		MessageBox(_T("�رջỰʧ��!"), _T("����"));
	}
}


void CSynoTFKeySDKTestDlg::OnBnClickedMaindlgBtnSessioninfo()
{
	ULONG ulRes = 0;

	if (m_strSelDevName.GetLength() == 0)
	{
		MessageBox(_T("����ѡ���豸!"), _T("��ʾ"));
		return;
	}
	if (NULL == m_hSession)
	{
		MessageBox(_T("���ȴ򿪻Ự!"), _T("��ʾ"));
		return;
	}

	ulRes = m_pSDKTester->GetSessionInfo(m_hSession);
	if (USBKEY_ERR_OK == ulRes)
	{
		MessageBox(_T("��ȡ�Ự��Ϣ�ɹ�!"), _T("��ʾ"));
	}
	else
	{
		MessageBox(_T("��ȡ�Ự��Ϣʧ��!"), _T("����"));
	}
}


void CSynoTFKeySDKTestDlg::OnBnClickedMaindlgBtnLoginsession()
{
	CLoginDlg dlg;
	ULONG ulRes = 0;
	ULONG ulLeftTimes = 0;

	if (m_strSelDevName.GetLength() == 0)
	{
		MessageBox(_T("����ѡ���豸!"), _T("��ʾ"));
		return;
	}
	if (NULL == m_hSession)
	{
		MessageBox(_T("���ȴ򿪻Ự!"), _T("��ʾ"));
		return;
	}

	if (dlg.DoModal() == IDOK)
	{
		ulRes = m_pSDKTester->LoginSession(m_hSession, dlg.m_ulUserType, dlg.m_tcPin, &ulLeftTimes);
		if (USBKEY_ERR_OK == ulRes)
		{
			m_ulCurLoginType = dlg.m_ulUserType;
			MessageBox(_T("��¼�Ự�ɹ�!"), _T("��ʾ"));
		}
		else
		{
			CString strError;
			if (ulLeftTimes > 0) strError.Format(_T("��¼�Ựʧ�ܣ�ʣ�����Դ�����%d"), ulLeftTimes);
			else strError.Format(_T("��¼�Ựʧ�ܣ�ʣ�����Դ�����δ֪"));
			MessageBox(strError);
		}
	}
}


void CSynoTFKeySDKTestDlg::OnBnClickedMaindlgBtnLogoutsession()
{
	ULONG ulRes = 0;

	if (m_strSelDevName.GetLength() == 0)
	{
		MessageBox(_T("����ѡ���豸!"), _T("��ʾ"));
		return;
	}
	if (NULL == m_hSession)
	{
		MessageBox(_T("���ȴ򿪻Ự!"), _T("��ʾ"));
		return;
	}

	ulRes = m_pSDKTester->LogOutSession(m_hSession);
	if (USBKEY_ERR_OK == ulRes)
	{
		MessageBox(_T("�ǳ��Ự�ɹ�!"), _T("��ʾ"));
	}
	else
	{
		MessageBox(_T("�ǳ��Ựʧ��!"), _T("����"));
	}
}


void CSynoTFKeySDKTestDlg::OnBnClickedMaindlgBtnChangepin()
{
	CChangePinDlg dlg;
	ULONG ulRes = 0;
	ULONG ulLeftTimes = 0;

	if (m_strSelDevName.GetLength() == 0)
	{
		MessageBox(_T("����ѡ���豸!"), _T("��ʾ"));
		return;
	}
	if (NULL == m_hSession)
	{
		MessageBox(_T("���ȴ򿪻Ự!"), _T("��ʾ"));
		return;
	}

	if (dlg.DoModal() == IDOK)
	{
		ulRes = m_pSDKTester->ChangePin(m_hSession, m_ulCurLoginType, dlg.m_tcOldPin, dlg.m_tcNewPin, &ulLeftTimes);
		if (USBKEY_ERR_OK == ulRes)
		{
			MessageBox(_T("�޸�PIN�ɹ�!"), _T("��ʾ"));
		}
		else
		{
			CString strError;
			if (ulLeftTimes > 0) strError.Format(_T("�޸�PINʧ�ܣ�ʣ�����Դ�����%d"), ulLeftTimes);
			else strError.Format(_T("�޸�PINʧ�ܣ�ʣ�����Դ�����δ֪"));
			MessageBox(strError);
		}
	}
}


void CSynoTFKeySDKTestDlg::OnBnClickedMaindlgBtnUnlockpin()
{
	CUnlockPinDlg dlg;
	ULONG ulRes = 0;
	ULONG ulLeftTimes = 0;

	if (m_strSelDevName.GetLength() == 0)
	{
		MessageBox(_T("����ѡ���豸!"), _T("��ʾ"));
		return;
	}
	if (NULL == m_hSession)
	{
		MessageBox(_T("���ȴ򿪻Ự!"), _T("��ʾ"));
		return;
	}

	if (dlg.DoModal() == IDOK)
	{
		ulRes = m_pSDKTester->UnlockPin(m_hSession, dlg.m_tcSOPin, dlg.m_tcNewUserPin, &ulLeftTimes);
		if (USBKEY_ERR_OK == ulRes)
		{
			MessageBox(_T("����PIN�ɹ�!"), _T("��ʾ"));
		}
		else
		{
			CString strError;
			if (ulLeftTimes > 0) strError.Format(_T("����PINʧ�ܣ�SO PINʣ�����Դ�����%d"), ulLeftTimes);
			else strError.Format(_T("����PINʧ�ܣ�SO PINʣ�����Դ�����δ֪"));
			MessageBox(strError);
		}
	}
}


void CSynoTFKeySDKTestDlg::OnBnClickedMaindlgBtnGenkeypair()
{
	ULONG ulRes = 0;

	if (m_strSelDevName.GetLength() == 0)
	{
		MessageBox(_T("����ѡ���豸!"), _T("��ʾ"));
		return;
	}
	if (NULL == m_hSession)
	{
		MessageBox(_T("���ȴ򿪻Ự!"), _T("��ʾ"));
		return;
	}

	ulRes = m_pSDKTester->GenerateKeyPair(m_hSession, USBKEY_ASYMMALG_RSA, 1024);
	if (USBKEY_ERR_OK == ulRes)
	{
		MessageBox(_T("������Կ�Գɹ�!"), _T("��ʾ"));
	}
	else
	{
		MessageBox(_T("������Կ��ʧ��!"), _T("����"));
	}
}


void CSynoTFKeySDKTestDlg::OnBnClickedMaindlgBtnImportkeypair()
{
	ULONG ulRes = 0;
	ULONG ulKeyPairLen = 2048;
	BYTE btKeyPairData[2048] = {0};
	CString strKeypairFile;
	FILE* pKeypairFile = NULL;
	CFileDialog dlgOpen(TRUE, _T("*.dat"), NULL, OPEN_EXISTING, _T("RSA ��Կ�ļ�|*.dat|All files|*.*||"), NULL);
	
	if (m_strSelDevName.GetLength() == 0)
	{
		MessageBox(_T("����ѡ���豸!"), _T("��ʾ"));
		return;
	}
	if (NULL == m_hSession)
	{
		MessageBox(_T("���ȴ򿪻Ự!"), _T("��ʾ"));
		return;
	}

	if (dlgOpen.DoModal() == IDOK)
	{		
		strKeypairFile = dlgOpen.GetPathName();
	}
	if (strKeypairFile.GetLength() == 0)
	{
		return;
	}

	USES_CONVERSION;
	
	//	������Կ�������ļ�
#ifdef UNICODE
	ulRes = fopen_s(&pKeypairFile, W2A(strKeypairFile), "r");
#else
	ulRes = fopen_s(&pKeypairFile, strKeypairFile, "r");
#endif	//UNICODE
	if (!pKeypairFile)
	{
		MessageBox(_T("����Կ���ļ�����!"), _T("��ʾ"));
		return;
	}	
	
	//	��ȡ�ļ�����
	if (!_ReadFileData(pKeypairFile, btKeyPairData, &ulKeyPairLen))
	{
		MessageBox(_T("��ȡ��Կ���ļ�����!"), _T("��ʾ"));
		goto ERR;
	}

	// ���������Կ��
	ulRes = m_pSDKTester->ImportKeyPair(m_hSession, USBKEY_ASYMMALG_RSA, btKeyPairData, ulKeyPairLen);
	if (USBKEY_ERR_OK != ulRes)
	{
		MessageBox(_T("������Կ�����ݴ���!"), _T("��ʾ"));
		goto ERR;
	}
	
	MessageBox(_T("������Կ�����ݳɹ�!"), _T("��ʾ"));

ERR:
	if (pKeypairFile)
	{
		fclose(pKeypairFile);
		pKeypairFile = NULL;
	}
}


void CSynoTFKeySDKTestDlg::OnBnClickedMaindlgExportsignpubkey()
{
	ULONG ulRes = 0;
	ULONG ulPubKeyLen = 512;
	BYTE btPubKey[512] = {0};

	if (m_strSelDevName.GetLength() == 0)
	{
		MessageBox(_T("����ѡ���豸!"), _T("��ʾ"));
		return;
	}
	if (NULL == m_hSession)
	{
		MessageBox(_T("���ȴ򿪻Ự!"), _T("��ʾ"));
		return;
	}

	ulRes = m_pSDKTester->ExportPublicKey(m_hSession, USBKEY_ASYMMALG_RSA, TRUE, btPubKey, &ulPubKeyLen);
	if (USBKEY_ERR_OK == ulRes)
	{
		CString strPublicKey;
		CString strTemp;
		strPublicKey = _T("  ");
		for (ULONG i = 0; i < ulPubKeyLen; i++)
		{
			strTemp.Format(_T("0x%02x "), btPubKey[i]);
			strPublicKey += strTemp;
		}
		SDKFuncMsgCallback(_T("Public Key data:"));
		SDKFuncMsgCallback(strPublicKey);

		MessageBox(_T("����ǩ����Կ�ɹ�!"), _T("��ʾ"));
	}
	else
	{
		MessageBox(_T("����ǩ����Կʧ��!"), _T("����"));
	}
}


void CSynoTFKeySDKTestDlg::OnBnClickedMaindlgExportexchpubkey()
{
	ULONG ulRes = 0;
	ULONG ulPubKeyLen = 512;
	BYTE btPubKey[512] = {0};
	
	if (m_strSelDevName.GetLength() == 0)
	{
		MessageBox(_T("����ѡ���豸!"), _T("��ʾ"));
		return;
	}
	if (NULL == m_hSession)
	{
		MessageBox(_T("���ȴ򿪻Ự!"), _T("��ʾ"));
		return;
	}

	ulRes = m_pSDKTester->ExportPublicKey(m_hSession, USBKEY_ASYMMALG_RSA, FALSE, btPubKey, &ulPubKeyLen);
	if (USBKEY_ERR_OK == ulRes)
	{
		CString strPublicKey;
		CString strTemp;
		strPublicKey = _T("  ");
		for (ULONG i = 0; i < ulPubKeyLen; i++)
		{
			strTemp.Format(_T("0x%02x "), btPubKey[i]);
			strPublicKey += strTemp;
		}
		SDKFuncMsgCallback(_T("Public Key data:"));
		SDKFuncMsgCallback(strPublicKey);

		MessageBox(_T("�������ܹ�Կ�ɹ�!"), _T("��ʾ"));
	}
	else
	{
		MessageBox(_T("�������ܹ�Կʧ��!"), _T("����"));
	}
}

void CSynoTFKeySDKTestDlg::OnBnClickedMaindlgBtnGeneratekey()
{
	ULONG ulRes = 0;

	if (m_strSelDevName.GetLength() == 0)
	{
		MessageBox(_T("����ѡ���豸!"), _T("��ʾ"));
		return;
	}
	if (NULL == m_hSession)
	{
		MessageBox(_T("���ȴ򿪻Ự!"), _T("��ʾ"));
		return;
	}

	//	�����ϵĻỰ��Կ
	if (m_hSessionKey)
	{
		m_pSDKTester->CloseSessionKey(m_hSession, m_hSessionKey);	
		m_hSessionKey = NULL;
	}

	//	�����µĻỰ��Կ
	ulRes = m_pSDKTester->GenerateSessionKey(m_hSession, USBKEY_SYMMALG_3DES, &m_hSessionKey);
	if (USBKEY_ERR_OK == ulRes)
	{
		MessageBox(_T("�Ự��Կ���ɳɹ�!"), _T("��ʾ"));
	}
	else
	{
		MessageBox(_T("�Ự��Կ����ʧ��!"), _T("����"));
	}
}


void CSynoTFKeySDKTestDlg::OnBnClickedMaindlgBtnReleasekey()
{
	ULONG ulRes = 0;

	if (m_strSelDevName.GetLength() == 0)
	{
		MessageBox(_T("����ѡ���豸!"), _T("��ʾ"));
		return;
	}
	if (NULL == m_hSession)
	{
		MessageBox(_T("���ȴ򿪻Ự!"), _T("��ʾ"));
		return;
	}
	if (NULL == m_hSessionKey)
	{
		MessageBox(_T("��ǰ�޻Ự��Կ!"), _T("��ʾ"));
		return;
	}

	//	�����ϵĻỰ��Կ
	ulRes = m_pSDKTester->CloseSessionKey(m_hSession, m_hSessionKey);	
	if (USBKEY_ERR_OK == ulRes)
	{
		m_hSessionKey = NULL;
		MessageBox(_T("�Ự��Կ���ٳɹ�!"), _T("��ʾ"));
	}
	else
	{
		MessageBox(_T("�Ự��Կ����ʧ��!"), _T("����"));
	}
}


void CSynoTFKeySDKTestDlg::OnBnClickedMaindlgBtnSignverify()
{
	CPlainTextDlg dlg;
	CString strPlainText;
	ULONG ulTestAsymmAlgId = 0;
	ULONG ulTestHashAlgId = 0;
	
	if (m_strSelDevName.GetLength() == 0)
	{
		MessageBox(_T("����ѡ���豸!"), _T("��ʾ"));
		return;
	}
	if (NULL == m_hSession)
	{
		MessageBox(_T("���ȴ򿪻Ự!"), _T("��ʾ"));
		return;
	}

	dlg.m_ulTestType = 1;
	if (dlg.DoModal() == IDOK)
	{
		strPlainText = dlg.m_strPlainText;
		switch(dlg.m_ulAlgId)
		{
		case USBKEY_SIGNALG_RSA:
			ulTestAsymmAlgId = USBKEY_ASYMMALG_RSA;
			ulTestHashAlgId = 0;
			break;
		case USBKEY_SIGNALG_RSA_MD5:
			ulTestAsymmAlgId = USBKEY_ASYMMALG_RSA;
			ulTestHashAlgId = USBKEY_HASHALG_MD5;
			break;
		case USBKEY_SIGNALG_RSA_SHA1:
			ulTestAsymmAlgId = USBKEY_ASYMMALG_RSA;
			ulTestHashAlgId = USBKEY_HASHALG_SHA1;
			break;
		case USBKEY_SIGNALG_RSA_SHA256:
			ulTestAsymmAlgId = USBKEY_ASYMMALG_RSA;
			ulTestHashAlgId = USBKEY_HASHALG_SHA256;
			break;
		}
		if (USBKEY_ERR_OK == m_pSDKTester->TestSign(m_hSession, ulTestAsymmAlgId, ulTestHashAlgId, strPlainText))
		{
			MessageBox(_T("����ͨ��!"), _T("��ʾ"));
		}
		else
		{
			MessageBox(_T("����ʧ��!"), _T("��ʾ"));
		}
	}
}


void CSynoTFKeySDKTestDlg::OnBnClickedMaindlgBtnCrypto()
{
	CPlainTextDlg dlg;
	CString strPlainText;
	ULONG ulTestAlgId = 0;
	
	if (m_strSelDevName.GetLength() == 0)
	{
		MessageBox(_T("����ѡ���豸!"), _T("��ʾ"));
		return;
	}
	if (NULL == m_hSession)
	{
		MessageBox(_T("���ȴ򿪻Ự!"), _T("��ʾ"));
		return;
	}

	dlg.m_ulTestType = 2;
	if (dlg.DoModal() == IDOK)
	{
		strPlainText = dlg.m_strPlainText;
		ulTestAlgId = dlg.m_ulAlgId;
		if (USBKEY_ERR_OK == m_pSDKTester->TestEncrypt(m_hSession, ulTestAlgId, strPlainText))
		{
			MessageBox(_T("����ͨ��!"), _T("��ʾ"));
		}
		else
		{
			MessageBox(_T("����ʧ��!"), _T("��ʾ"));
		}
	}
}


void CSynoTFKeySDKTestDlg::OnBnClickedMaindlgBtnEnvenlop()
{
	CPlainTextDlg dlg;
	CString strPlainText;
	ULONG ulTestAlgId = 0;
	
	if (m_strSelDevName.GetLength() == 0)
	{
		MessageBox(_T("����ѡ���豸!"), _T("��ʾ"));
		return;
	}
	if (NULL == m_hSession)
	{
		MessageBox(_T("���ȴ򿪻Ự!"), _T("��ʾ"));
		return;
	}

	dlg.m_ulTestType = 3;
	if (dlg.DoModal() == IDOK)
	{
		strPlainText = dlg.m_strPlainText;
		ulTestAlgId = dlg.m_ulAlgId;
		if (USBKEY_ERR_OK == m_pSDKTester->TestEnvelop(m_hSession, ulTestAlgId, strPlainText))
		{
			MessageBox(_T("����ͨ��!"), _T("��ʾ"));
		}
		else
		{
			MessageBox(_T("����ʧ��!"), _T("��ʾ"));
		}
	}
}


void CSynoTFKeySDKTestDlg::OnBnClickedMaindlgBtnImportcert()
{
	ULONG ulRes = 0;
	ULONG ulFileSize = 0;
	ULONG ulReadSize = 0;
	LPBYTE lpbtCertData = NULL;
	CString strCertFile;
	HANDLE hCertFile = NULL;
	CFileDialog dlgOpen(TRUE, _T("*.cer"), NULL, OPEN_EXISTING, _T("X509֤�� file|*.cer||"), NULL);

	if (m_strSelDevName.GetLength() == 0)
	{
		MessageBox(_T("����ѡ���豸!"), _T("��ʾ"));
		return;
	}
	if (NULL == m_hSession)
	{
		MessageBox(_T("���ȴ򿪻Ự!"), _T("��ʾ"));
		return;
	}	
	
	if (dlgOpen.DoModal() != IDOK)
	{
		return;
	}	
	strCertFile = dlgOpen.GetPathName();

	// ��֤���ļ�
	hCertFile = CreateFile(strCertFile, GENERIC_WRITE|GENERIC_READ, 
		FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (INVALID_HANDLE_VALUE == hCertFile)
	{
		MessageBox(_T("��֤���ļ�ʧ��!"), _T("��ʾ"));
		return;
	}

	// ��ȡ�ļ���С
	ulFileSize = ::GetFileSize(hCertFile, &ulFileSize);
	if (ulFileSize == 0)
	{
		MessageBox(_T("֤���ļ���С����!"), _T("��ʾ"));
		goto FREE_MEMORY;
	}

	lpbtCertData = new BYTE[ulFileSize];
	memset(lpbtCertData, 0, ulFileSize);
	if (!ReadFile(hCertFile, lpbtCertData, ulFileSize, &ulReadSize, NULL))
	{
		MessageBox(_T("��֤���ļ�ʧ��!"), _T("��ʾ"));
		goto FREE_MEMORY;
	}

	//	PEM����(Base64)
	if (lpbtCertData[0] != 0x30 || lpbtCertData[1] != 0x82)
	{
		ULONG ulDataLen = 0;
		LPBYTE lpbtTemp = new BYTE[ulReadSize];

		memcpy_s(lpbtTemp, ulReadSize, lpbtCertData, ulReadSize);
		delete []lpbtCertData;
		lpbtCertData = NULL;

		// ��Base64��ʽת��Ϊ������
		ulDataLen = Base64ToBinary((CHAR*)lpbtTemp, ulReadSize, NULL);
		if (ulDataLen == 0)
		{
			MessageBox(_T("Base64ת��ʧ��!"), _T("��ʾ"));
			delete []lpbtTemp;
			lpbtTemp = NULL;
			goto FREE_MEMORY;
		}
		lpbtCertData = new BYTE[ulDataLen];
		ulDataLen = Base64ToBinary((CHAR*)lpbtTemp, ulReadSize, lpbtCertData);
		if (ulDataLen == 0)
		{
			MessageBox(_T("Base64ת��ʧ��!"), _T("��ʾ"));
			delete []lpbtTemp;
			lpbtTemp = NULL;
			goto FREE_MEMORY;
		}
		ulReadSize = ulDataLen;
	}

	// ����֤��
	ulRes = m_pSDKTester->ImportCert(m_hSession, _T("P11_Con"), USBKEY_ASYMMALG_RSA, USAGE_TYPE_SIGN, lpbtCertData, ulReadSize);
	if (USBKEY_ERR_OK != ulRes)
	{
		MessageBox(_T("����֤��ʧ��!"), _T("��ʾ"));
		goto FREE_MEMORY;
	}

	MessageBox(_T("����֤��ɹ�!"), _T("��ʾ"));

FREE_MEMORY:
	if (lpbtCertData)
	{
		delete []lpbtCertData;
		lpbtCertData = NULL;
	}
	if (hCertFile)
	{
		CloseHandle(hCertFile);
		hCertFile = NULL;
	}
}


void CSynoTFKeySDKTestDlg::OnBnClickedMaindlgBtnExportcert()
{
	ULONG ulRes = 0;
	ULONG ulCertSize = 0;
	ULONG ulWrittenLen = 0;
	LPBYTE lpbtCertData = NULL;
	HANDLE	hCertFile = NULL;
	CString strCertFile;
	CFileDialog dlgOpen(FALSE, _T("*.cer"), NULL, OPEN_EXISTING, _T("X509֤�� file|*.cer||"), NULL);

	if (m_strSelDevName.GetLength() == 0)
	{
		MessageBox(_T("����ѡ���豸!"), _T("��ʾ"));
		return;
	}
	if (NULL == m_hSession)
	{
		MessageBox(_T("���ȴ򿪻Ự!"), _T("��ʾ"));
		return;
	}	
				
	if (dlgOpen.DoModal() != IDOK)
	{
		return;
	}	
	strCertFile = dlgOpen.GetPathName();

	// ֤�鳤��
	ulRes = m_pSDKTester->ExportCert(m_hSession, USBKEY_ASYMMALG_RSA, USAGE_TYPE_SIGN, NULL, &ulCertSize);
	if (USBKEY_ERR_OK != ulRes)
	{
		MessageBox(_T("����֤��ʧ��!"), _T("��ʾ"));
		return;
	}

	// ֤������
	lpbtCertData = new BYTE[ulCertSize];
	ulRes = m_pSDKTester->ExportCert(m_hSession, USBKEY_ASYMMALG_RSA, USAGE_TYPE_SIGN, lpbtCertData, &ulCertSize);
	if (USBKEY_ERR_OK != ulRes)
	{
		MessageBox(_T("����֤��ʧ��!"), _T("��ʾ"));
		goto FREE_MEMORY;
	}

	// �����ļ�
	hCertFile = CreateFile(strCertFile, GENERIC_WRITE|GENERIC_READ, 
		FILE_SHARE_READ, NULL, CREATE_ALWAYS, 0, NULL);
	if (INVALID_HANDLE_VALUE == hCertFile)
	{
		MessageBox(_T("����֤���ļ�ʧ��!"), _T("��ʾ"));
		goto FREE_MEMORY;
	}
	if (!WriteFile(hCertFile, lpbtCertData, ulCertSize, &ulWrittenLen, NULL) || 0 == ulWrittenLen)
	{
		MessageBox(_T("д֤���ļ�ʧ��!"), _T("��ʾ"));
		goto FREE_MEMORY;
	}
	
	MessageBox(_T("����֤��ɹ�!"), _T("��ʾ"));

FREE_MEMORY:
	if (lpbtCertData)
	{
		delete []lpbtCertData;
		lpbtCertData = NULL;
	}
	if (hCertFile)
	{
		CloseHandle(hCertFile);
		hCertFile = NULL;
	}
}


void CSynoTFKeySDKTestDlg::OnBnClickedMaindlgBtnEnumfiles()
{
	ULONG ulRes = 0;
	ULONG ulFileNameLen = 0;
	ULONG ulFileCount = 0;
	LPTSTR lpFileNameList = NULL;
	LPCTSTR lpcsFileName = NULL;
	CString strInfo;
	
	if (m_strSelDevName.GetLength() == 0)
	{
		MessageBox(_T("����ѡ���豸!"), _T("��ʾ"));
		return;
	}
	if (NULL == m_hSession)
	{
		MessageBox(_T("���ȴ򿪻Ự!"), _T("��ʾ"));
		return;
	}

	// ��ȡ����
	ulRes = m_pSDKTester->EnumFiles(m_hSession, NULL, &ulFileNameLen);
	if (USBKEY_ERR_OK != ulRes)
	{
		MessageBox(_T("ö���û��ļ�ʧ��!"), _T("��ʾ"));
		return;
	}
	if (ulFileNameLen <= 2)
	{
		MessageBox(_T("û���ҵ��û��ļ�!"), _T("��ʾ"));
		return;
	}

	// ��ȡ����
	lpFileNameList = new TCHAR[ulFileNameLen];
	ulRes = m_pSDKTester->EnumFiles(m_hSession, lpFileNameList, &ulFileNameLen);
	if (USBKEY_ERR_OK != ulRes)
	{
		MessageBox(_T("ö���û��ļ�ʧ��!"), _T("��ʾ"));
		goto FREE;
	}

	// ��ʾ�ļ��б�
	SDKFuncMsgCallback(_T("Data file list:\n"));
	lpcsFileName = lpFileNameList;
	while (lpcsFileName && _tcslen(lpcsFileName) > 0)
	{
		strInfo.Format(_T("  %d: %s\n"), ++ulFileCount, lpcsFileName);
		SDKFuncMsgCallback(strInfo);
		lpcsFileName += (_tcslen(lpcsFileName) + 1);
	}

	MessageBox(_T("ö���û��ļ��ɹ�!"), _T("��ʾ"));

FREE:
	if (lpFileNameList)
	{
		delete []lpFileNameList;
		lpFileNameList = NULL;
	}
}


void CSynoTFKeySDKTestDlg::OnBnClickedMaindlgBtnCreatefile()
{
	ULONG ulRes = 0;
	ULONG ulFileSize = 0;
	TCHAR tcFileName[64] = {0};
	LPBYTE lpFileData = NULL;
	CNewDataFileDlg dlg;
	
	USES_CONVERSION;

	if (m_strSelDevName.GetLength() == 0)
	{
		MessageBox(_T("����ѡ���豸!"), _T("��ʾ"));
		return;
	}
	if (NULL == m_hSession)
	{
		MessageBox(_T("���ȴ򿪻Ự!"), _T("��ʾ"));
		return;
	}

	if (dlg.DoModal() != IDOK)
		return;

	_tcscpy_s(tcFileName, 64, dlg.m_strFileName);
#ifdef UNICODE
	ulFileSize = strlen(W2A(dlg.m_strFileData));
	lpFileData = new BYTE[ulFileSize];
	memcpy_s(lpFileData, ulFileSize, W2A(dlg.m_strFileData), ulFileSize);
#else
	ulFileSize = strlen(dlg.m_strFileData);
	lpFileData = new BYTE[ulFileSize];
	memcpy_s(lpFileData, ulFileSize, dlg.m_strFileData, ulFileSize);
#endif	//UNICODE

	//	�����ļ�
	ulRes = m_pSDKTester->CreateFile(m_hSession, tcFileName, ulFileSize);
	if (USBKEY_ERR_OK != ulRes)
	{
		MessageBox(_T("�����û��ļ�ʧ��!"), _T("��ʾ"));
		goto FREE;
	}
	
	//	д�ļ�
	m_pSDKTester->WriteFile(m_hSession, tcFileName, lpFileData, ulFileSize);
	if (USBKEY_ERR_OK != ulRes)
	{
		MessageBox(_T("д�û��ļ�ʧ��!"), _T("��ʾ"));
		goto FREE;
	}

	MessageBox(_T("�����û��ļ��ɹ�!"), _T("��ʾ"));

FREE:
	if (lpFileData)
	{
		delete []lpFileData;
		lpFileData = NULL;
	}
}

void CSynoTFKeySDKTestDlg::OnBnClickedMaindlgBtnReadfile()
{
	ULONG ulRes = 0;
	ULONG ulFileSize = 0;
	TCHAR tcFileName[64] = {0};
	LPBYTE lpFileData = NULL;
	LPCTSTR lpcsFileData = NULL;
	CInputFileNameDlg dlg;
	CString strFileContent;
	
	USES_CONVERSION;

	if (m_strSelDevName.GetLength() == 0)
	{
		MessageBox(_T("����ѡ���豸!"), _T("��ʾ"));
		return;
	}
	if (NULL == m_hSession)
	{
		MessageBox(_T("���ȴ򿪻Ự!"), _T("��ʾ"));
		return;
	}

	if (dlg.DoModal() != IDOK)
		return;

	_tcscpy_s(tcFileName, 64, dlg.m_strFileName);

	//	��ȡ�ļ���С
	ulRes = m_pSDKTester->ReadFile(m_hSession, tcFileName, NULL, &ulFileSize);
	if (USBKEY_ERR_OK != ulRes)
	{
		MessageBox(_T("��ȡ�û��ļ�ʧ��!"), _T("��ʾ"));
		return;
	}
	if (0 == ulFileSize)
	{
		MessageBox(_T("�û��ļ���СΪ0!"), _T("��ʾ"));
		return;
	}
	
	//	��ȡ�ļ�����
	lpFileData = new BYTE[ulFileSize + 1];
	memset(lpFileData, 0, ulFileSize + 1);
	ulRes = m_pSDKTester->ReadFile(m_hSession, tcFileName, lpFileData, &ulFileSize);
	if (USBKEY_ERR_OK != ulRes)
	{
		MessageBox(_T("��ȡ�û��ļ�ʧ��!"), _T("��ʾ"));
		goto FREE;
	}

#ifdef UNICODE
	lpcsFileData = A2W((LPSTR)lpFileData);
#else
	lpcsFileData = (LPSTR)lpFileData;
#endif	//UNICODE
	strFileContent.Format(_T("File Data:%s"), lpcsFileData);
	SDKFuncMsgCallback(strFileContent);
	
	MessageBox(_T("��ȡ�û��ļ��ɹ�!"), _T("��ʾ"));

FREE:
	if (lpFileData)
	{
		delete []lpFileData;
		lpFileData = NULL;
	}
}


void CSynoTFKeySDKTestDlg::OnBnClickedMaindlgBtnFileinfo()
{
	ULONG ulRes = 0;
	ULONG ulFileSize = 0;
	TCHAR tcFileName[64] = {0};
	CString strFileInfo;
	CInputFileNameDlg dlg;
	
	if (m_strSelDevName.GetLength() == 0)
	{
		MessageBox(_T("����ѡ���豸!"), _T("��ʾ"));
		return;
	}
	if (NULL == m_hSession)
	{
		MessageBox(_T("���ȴ򿪻Ự!"), _T("��ʾ"));
		return;
	}

	if (dlg.DoModal() != IDOK)
		return;

	_tcscpy_s(tcFileName, 64, dlg.m_strFileName);
		
	//	��ȡ�ļ���С
	ulRes = m_pSDKTester->ReadFile(m_hSession, tcFileName, NULL, &ulFileSize);
	if (USBKEY_ERR_OK != ulRes)
	{
		MessageBox(_T("��ȡ�û��ļ�ʧ��!"), _T("��ʾ"));
		return;
	}

	strFileInfo.Format(_T("File size: 0x%x\n"), ulFileSize);
	SDKFuncMsgCallback(strFileInfo);	
	
	MessageBox(_T("��ȡ�û��ļ���Ϣ�ɹ�!"), _T("��ʾ"));
}


void CSynoTFKeySDKTestDlg::OnBnClickedMaindlgBtnClearinfo()
{
	CListBox* pInfoMsgListBox = (CListBox*)GetDlgItem(IDC_MAINDLG_LIST_INFO);
	pInfoMsgListBox->ResetContent();
}


void CSynoTFKeySDKTestDlg::_SetControlInitStatus()
{
	switch(m_eSelSDKType)
	{
	case SDK_TYPE_SKF:
		break;
	case SDK_TYPE_PKCS11:
		GetDlgItem(IDC_MAINDLG_BTN_SETLABEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_MAINDLG_BTN_DEVAUTH)->EnableWindow(FALSE);
		GetDlgItem(IDC_MAINDLG_BTN_CHANGEAUTHKEY)->EnableWindow(FALSE);
		break;
	}
}

void CSynoTFKeySDKTestDlg::_EnumDevices()
{
	ULONG ulRes = 0;
	ULONG ulDevNameSize = 0;
	LPTSTR lptsDevNames = NULL;
	LPCTSTR lpctDevName = NULL;
	CTreeCtrl* pDevNameTree = NULL;
	
	pDevNameTree = (CTreeCtrl*)GetDlgItem(IDC_MAINDLG_TREE_LIST);
	pDevNameTree->DeleteAllItems();

	ulRes = m_pSDKTester->EnumDev(NULL, &ulDevNameSize);
	if (USBKEY_ERR_OK != ulRes || ulDevNameSize <= 2)
	{
		return;
	}

	lptsDevNames = new TCHAR[ulDevNameSize + 1];
	memset(lptsDevNames, 0, sizeof(TCHAR)*(ulDevNameSize + 1));
	ulRes = m_pSDKTester->EnumDev(lptsDevNames, &ulDevNameSize);
	if (USBKEY_ERR_OK != ulRes)
	{
		goto END;
	}

	lpctDevName = lptsDevNames;
	while (_tcslen(lpctDevName) > 0)
	{
		pDevNameTree->InsertItem(lpctDevName);
		lpctDevName += _tcslen(lpctDevName) + 1;
	}

END:
	if (lptsDevNames)
	{
		delete []lptsDevNames;
		lptsDevNames = NULL;
	}
}

BOOL CSynoTFKeySDKTestDlg::_ReadFileData(FILE* file, BYTE* lpData, ULONG *pulDataLen)
{	
	ULONG ulIndex = 0;
	ULONG ulWritten = 0;
	ULONG ulData = 0;

	if (!file || !lpData || !pulDataLen || 0 == *pulDataLen)
	{
		return FALSE;
	}

	while (!feof(file))
	{
		if (ulIndex == *pulDataLen)
		{
			break;
		}
		int n = fscanf_s(file, "%02X ", &ulData, 4);
		if (1 == n)
		{
			lpData[ulIndex++] = (BYTE)ulData;
		}
		else
		{
			return FALSE;
		}
	}
	*pulDataLen = ulIndex;
	
	return TRUE;
}