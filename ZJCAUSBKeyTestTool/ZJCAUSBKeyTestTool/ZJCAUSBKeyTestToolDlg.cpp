
// ZJCAUSBKeyTestToolDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ZJCAUSBKeyTestTool.h"
#include "ZJCAUSBKeyTestToolDlg.h"
#include "LoginDlg.h"
#include "afxdialogex.h"
#include "../Include/CommonDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CZJCAUSBKeyTestToolDlg dialog

CZJCAUSBKeyTestToolDlg::CZJCAUSBKeyTestToolDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CZJCAUSBKeyTestToolDlg::IDD, pParent)
{
	m_pCSP = NULL;
	m_pP11 = NULL;
	m_pSKF = NULL;
	m_bTestCSP = FALSE;
	m_bTestP11 = FALSE;
	m_bTestSKF = FALSE;
	m_bTestCspCryptP7 = FALSE;
	m_bTestCspCryptP1 = FALSE;
	m_bTestCspSignP7 = FALSE;
	m_bTestCspSignP1 = FALSE;
	m_bTestP11Event = FALSE;
	m_bTestP11CryptP1 = FALSE;
	m_bTestP11SignP1 = FALSE;
	m_bTestSkfEvent = FALSE;
	m_bTestSkfCryptP1 = FALSE;
	m_bTestSkfSignP1 = FALSE;
	m_strCSPName = _T("");
	m_strP11File = _T("");
	m_strSkfFile = _T("");
	m_ulCspAsymmAlg = USBKEY_ASYMMALG_RSA;
	m_ulCspSymmAlg = USBKEY_SYMMALG_3DES;
	m_ulCspHashAlg = USBKEY_HASHALG_SHA1;
	m_ulP11AsymmAlg = USBKEY_ASYMMALG_RSA;
	m_ulP11SymmAlg = USBKEY_SYMMALG_3DES;
	m_ulP11HashAlg = USBKEY_HASHALG_SHA1;
	m_ulSkfAsymmAlg = USBKEY_ASYMMALG_SM2;
	m_ulSkfSymmAlg = USBKEY_SYMMALG_SM1_ECB;
	m_ulSkfHashAlg = USBKEY_HASHALG_SM3;
	m_pP11EventThread = NULL;
	m_pSkfEventThread = NULL;
	m_pUsbKeyTestThread = NULL;
	m_ulTestDataLen = TEST_DATA_LEN;
	memset(&s_csDevEvent, 0, sizeof(s_csDevEvent));
	memset(m_btTestData, 0, TEST_DATA_LEN);

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CZJCAUSBKeyTestToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CZJCAUSBKeyTestToolDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_CHECK_TESTCSP, &CZJCAUSBKeyTestToolDlg::OnBnClickedCheckTestcsp)
	ON_CBN_SELCHANGE(IDC_COMBO_CSPNAME, &CZJCAUSBKeyTestToolDlg::OnCbnSelchangeComboCspname)
	ON_CBN_SELCHANGE(IDC_COMBO_CSP_ASYMMALG, &CZJCAUSBKeyTestToolDlg::OnCbnSelchangeComboCspAsymmalg)
	ON_CBN_SELCHANGE(IDC_COMBO_CSP_SYMMALG, &CZJCAUSBKeyTestToolDlg::OnCbnSelchangeComboCspSymmalg)
	ON_CBN_SELCHANGE(IDC_COMBO_CSP_HASHALG, &CZJCAUSBKeyTestToolDlg::OnCbnSelchangeComboCspHashalg)
	ON_BN_CLICKED(IDC_CHECK_CSPCRYPT_P7, &CZJCAUSBKeyTestToolDlg::OnBnClickedCheckCspcryptP7)
	ON_BN_CLICKED(IDC_CHECK_CSPCRYPT_P1, &CZJCAUSBKeyTestToolDlg::OnBnClickedCheckCspcryptP1)
	ON_BN_CLICKED(IDC_CHECK_CSPSIGN_P7, &CZJCAUSBKeyTestToolDlg::OnBnClickedCheckCspsignP7)
	ON_BN_CLICKED(IDC_CHECK_CSPSIGN_P1, &CZJCAUSBKeyTestToolDlg::OnBnClickedCheckCspsignP1)
	ON_BN_CLICKED(IDC_CHECK_TESTP11, &CZJCAUSBKeyTestToolDlg::OnBnClickedCheckTestp11)
	ON_BN_CLICKED(IDC_BTB_OPENP11, &CZJCAUSBKeyTestToolDlg::OnBnClickedBtbOpenp11)
	ON_CBN_SELCHANGE(IDC_COMBO_P11_ASYMMALG, &CZJCAUSBKeyTestToolDlg::OnCbnSelchangeComboP11Asymmalg)
	ON_CBN_SELCHANGE(IDC_COMBO_P11_SYMMALG, &CZJCAUSBKeyTestToolDlg::OnCbnSelchangeComboP11Symmalg)
	ON_CBN_SELCHANGE(IDC_COMBO_P11_HASHALG, &CZJCAUSBKeyTestToolDlg::OnCbnSelchangeComboP11Hashalg)
	ON_BN_CLICKED(IDC_CHECK_TESTP11_EVENT, &CZJCAUSBKeyTestToolDlg::OnBnClickedCheckTestp11Event)
	ON_BN_CLICKED(IDC_CHECK_P11CRYPT_P1, &CZJCAUSBKeyTestToolDlg::OnBnClickedCheckP11cryptP1)
	ON_BN_CLICKED(IDC_CHECK_P11SIGN_P1, &CZJCAUSBKeyTestToolDlg::OnBnClickedCheckP11signP1)
	ON_BN_CLICKED(IDC_CHECK_TESTSKF, &CZJCAUSBKeyTestToolDlg::OnBnClickedCheckTestskf)
	ON_BN_CLICKED(IDC_BTN_OPENSKF, &CZJCAUSBKeyTestToolDlg::OnBnClickedBtnOpenskf)
	ON_CBN_SELCHANGE(IDC_COMBO_SKF_ASYMMALG, &CZJCAUSBKeyTestToolDlg::OnCbnSelchangeComboSkfAsymmalg)
	ON_CBN_SELCHANGE(IDC_COMBO_SKF_SYMMALG, &CZJCAUSBKeyTestToolDlg::OnCbnSelchangeComboSkfSymmalg)
	ON_CBN_SELCHANGE(IDC_COMBO_SKF_HASHALG, &CZJCAUSBKeyTestToolDlg::OnCbnSelchangeComboSkfHashalg)
	ON_BN_CLICKED(IDC_CHECK_TESTSKF_EVENT, &CZJCAUSBKeyTestToolDlg::OnBnClickedCheckTestskfEvent)
	ON_BN_CLICKED(IDC_CHECK_SKFCRYPT_P1, &CZJCAUSBKeyTestToolDlg::OnBnClickedCheckSkfcryptP1)
	ON_BN_CLICKED(IDC_CHECK_SKFSIGN_P1, &CZJCAUSBKeyTestToolDlg::OnBnClickedCheckSkfsignP1)
	ON_BN_CLICKED(IDC_CHECK_LOGFILE, &CZJCAUSBKeyTestToolDlg::OnBnClickedCheckLogfile)
	ON_BN_CLICKED(IDC_BTN_LOGFILE, &CZJCAUSBKeyTestToolDlg::OnBnClickedBtnLogfile)
	ON_BN_CLICKED(IDC_BTN_START, &CZJCAUSBKeyTestToolDlg::OnBnClickedBtnStart)
	ON_BN_CLICKED(IDC_BTN_SAVELOG, &CZJCAUSBKeyTestToolDlg::OnBnClickedBtnSavelog)
	ON_BN_CLICKED(IDC_BTN_CLEAN, &CZJCAUSBKeyTestToolDlg::OnBnClickedBtnClean)
END_MESSAGE_MAP()


// CZJCAUSBKeyTestToolDlg message handlers

BOOL CZJCAUSBKeyTestToolDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	InitCSPNameComboBox();
	InitAlgNameComboBox();
	//
	OnBnClickedCheckTestcsp();
	OnBnClickedCheckTestp11();
	OnBnClickedCheckTestskf();
	GetDlgItem(IDC_BTN_LOGFILE)->EnableWindow(FALSE);
	//	
	InitializeCriticalSection(&s_csDevEvent);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CZJCAUSBKeyTestToolDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CZJCAUSBKeyTestToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CZJCAUSBKeyTestToolDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	
	DestoryUsbKeyApi();
	DeleteCriticalSection(&s_csDevEvent);
}

void CZJCAUSBKeyTestToolDlg::OnBnClickedCheckTestcsp()
{
	if (BST_CHECKED == ((CButton*)GetDlgItem(IDC_CHECK_TESTCSP))->GetCheck())
	{
		m_bTestCSP = TRUE;
		GetDlgItem(IDC_COMBO_CSPNAME)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_CSP_ASYMMALG)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_CSP_SYMMALG)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_CSP_HASHALG)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_CSPCRYPT_P7)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_CSPCRYPT_P1)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_CSPSIGN_P7)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_CSPSIGN_P1)->EnableWindow(TRUE);
		//
		CreateCSPApi();
	}
	else
	{
		m_bTestCSP = FALSE;
		GetDlgItem(IDC_COMBO_CSPNAME)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_CSP_ASYMMALG)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_CSP_SYMMALG)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_CSP_HASHALG)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_CSPCRYPT_P7)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_CSPCRYPT_P1)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_CSPSIGN_P7)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_CSPSIGN_P1)->EnableWindow(FALSE);
		//
		if (m_pCSP)
		{
			USBKey_Destroy(m_pCSP);
			m_pCSP = NULL;
		}
	}
}

void CZJCAUSBKeyTestToolDlg::OnCbnSelchangeComboCspname()
{
	CreateCSPApi();
}

void CZJCAUSBKeyTestToolDlg::OnCbnSelchangeComboCspAsymmalg()
{
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_CSP_ASYMMALG);
	ULONG ulIndex = pComboBox->GetCurSel();
	m_ulCspAsymmAlg = pComboBox->GetItemData(ulIndex);
}

void CZJCAUSBKeyTestToolDlg::OnCbnSelchangeComboCspSymmalg()
{
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_CSP_SYMMALG);
	ULONG ulIndex = pComboBox->GetCurSel();
	m_ulCspSymmAlg = pComboBox->GetItemData(ulIndex);
}

void CZJCAUSBKeyTestToolDlg::OnCbnSelchangeComboCspHashalg()
{
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_CSP_HASHALG);
	ULONG ulIndex = pComboBox->GetCurSel();
	m_ulCspHashAlg = pComboBox->GetItemData(ulIndex);
}

void CZJCAUSBKeyTestToolDlg::OnBnClickedCheckCspcryptP7()
{
	if (BST_CHECKED == ((CButton*)GetDlgItem(IDC_CHECK_CSPCRYPT_P7))->GetCheck())
	{
		m_bTestCspCryptP7 = TRUE;
	}
	else
	{
		m_bTestCspCryptP7 = FALSE;
	}
}

void CZJCAUSBKeyTestToolDlg::OnBnClickedCheckCspcryptP1()
{
	if (BST_CHECKED == ((CButton*)GetDlgItem(IDC_CHECK_CSPCRYPT_P1))->GetCheck())
	{
		m_bTestCspCryptP1 = TRUE;
	}
	else
	{
		m_bTestCspCryptP1 = FALSE;
	}
}

void CZJCAUSBKeyTestToolDlg::OnBnClickedCheckCspsignP7()
{
	if (BST_CHECKED == ((CButton*)GetDlgItem(IDC_CHECK_CSPSIGN_P7))->GetCheck())
	{
		m_bTestCspSignP7 = TRUE;
	}
	else
	{
		m_bTestCspSignP7 = FALSE;
	}
}

void CZJCAUSBKeyTestToolDlg::OnBnClickedCheckCspsignP1()
{
	if (BST_CHECKED == ((CButton*)GetDlgItem(IDC_CHECK_CSPSIGN_P1))->GetCheck())
	{
		m_bTestCspSignP1 = TRUE;
	}
	else
	{
		m_bTestCspSignP1 = FALSE;
	}
}

void CZJCAUSBKeyTestToolDlg::OnBnClickedCheckTestp11()
{
	if (BST_CHECKED == ((CButton*)GetDlgItem(IDC_CHECK_TESTP11))->GetCheck())
	{
		m_bTestP11 = TRUE;
		GetDlgItem(IDC_BTB_OPENP11)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_P11_ASYMMALG)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_P11_SYMMALG)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_P11_HASHALG)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_TESTP11_EVENT)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_P11CRYPT_P1)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_P11SIGN_P1)->EnableWindow(TRUE);
		//
		CreateP11Api();
	}
	else
	{
		m_bTestP11 = FALSE;
		GetDlgItem(IDC_BTB_OPENP11)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_P11_ASYMMALG)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_P11_SYMMALG)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_P11_HASHALG)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_TESTP11_EVENT)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_P11CRYPT_P1)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_P11SIGN_P1)->EnableWindow(FALSE);
		//		
		if (m_pP11)
		{
			if (m_pP11EventThread) m_pP11EventThread->Stop();
			USBKey_Destroy(m_pP11);
			m_pP11 = NULL;
		}
	}
}

void CZJCAUSBKeyTestToolDlg::OnBnClickedBtbOpenp11()
{
	CFileDialog dlgOpen(TRUE, _T("*.dll"), NULL, OPEN_EXISTING, _T("DLL file|*.dll|All files|*.*||"), NULL);
	
	if (dlgOpen.DoModal() == IDOK)
	{		
		m_strP11File = dlgOpen.GetPathName();
		SetDlgItemText(IDC_EDIT_P11DLL, m_strP11File);		
		CreateP11Api();
	}
}

void CZJCAUSBKeyTestToolDlg::OnCbnSelchangeComboP11Asymmalg()
{
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_P11_ASYMMALG);
	ULONG ulIndex = pComboBox->GetCurSel();
	m_ulP11AsymmAlg = pComboBox->GetItemData(ulIndex);
}

void CZJCAUSBKeyTestToolDlg::OnCbnSelchangeComboP11Symmalg()
{
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_P11_SYMMALG);
	ULONG ulIndex = pComboBox->GetCurSel();
	m_ulP11SymmAlg = pComboBox->GetItemData(ulIndex);
}

void CZJCAUSBKeyTestToolDlg::OnCbnSelchangeComboP11Hashalg()
{
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_P11_HASHALG);
	ULONG ulIndex = pComboBox->GetCurSel();
	m_ulP11HashAlg = pComboBox->GetItemData(ulIndex);
}

void CZJCAUSBKeyTestToolDlg::OnBnClickedCheckTestp11Event()
{
	if (BST_CHECKED == ((CButton*)GetDlgItem(IDC_CHECK_TESTP11_EVENT))->GetCheck())
	{
		m_bTestP11Event = TRUE;	
		if (m_pP11)
		{
			m_pP11EventThread = new CZJCAUSBKeyEventThread();
			m_pP11EventThread->Start(m_pP11, FALSE, USBKeyEventFunc, this);
		}
	}
	else
	{
		m_bTestP11Event = FALSE;
		if (m_pP11EventThread)
		{
			m_pP11EventThread->Stop();
			delete m_pP11EventThread;
			m_pP11EventThread = NULL;
		}
	}
}

void CZJCAUSBKeyTestToolDlg::OnBnClickedCheckP11cryptP1()
{
	if (BST_CHECKED == ((CButton*)GetDlgItem(IDC_CHECK_P11CRYPT_P1))->GetCheck())
	{
		m_bTestP11CryptP1 = TRUE;
	}
	else
	{
		m_bTestP11CryptP1 = FALSE;
	}
}

void CZJCAUSBKeyTestToolDlg::OnBnClickedCheckP11signP1()
{
	if (BST_CHECKED == ((CButton*)GetDlgItem(IDC_CHECK_P11SIGN_P1))->GetCheck())
	{
		m_bTestP11SignP1 = TRUE;
	}
	else
	{
		m_bTestP11SignP1 = FALSE;
	}
}

void CZJCAUSBKeyTestToolDlg::OnBnClickedCheckTestskf()
{
	if (BST_CHECKED == ((CButton*)GetDlgItem(IDC_CHECK_TESTSKF))->GetCheck())
	{
		m_bTestSKF = TRUE;
		GetDlgItem(IDC_BTN_OPENSKF)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_SKF_ASYMMALG)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_SKF_SYMMALG)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_SKF_HASHALG)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_TESTSKF_EVENT)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_SKFCRYPT_P1)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_SKFSIGN_P1)->EnableWindow(TRUE);
		//
		CreateSkfApi();
	}
	else
	{
		m_bTestSKF = FALSE;
		GetDlgItem(IDC_BTN_OPENSKF)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_SKF_ASYMMALG)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_SKF_SYMMALG)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_SKF_HASHALG)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_TESTSKF_EVENT)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_SKFCRYPT_P1)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_SKFSIGN_P1)->EnableWindow(FALSE);
		//
		if (m_pSKF)
		{
			if (m_pSkfEventThread) m_pSkfEventThread->Stop();
			USBKey_Destroy(m_pSKF);
			m_pSKF = NULL;
		}
	}
}

void CZJCAUSBKeyTestToolDlg::OnBnClickedBtnOpenskf()
{
	CFileDialog dlgOpen(TRUE, _T("*.dll"), NULL, OPEN_EXISTING, _T("DLL file|*.dll|All files|*.*||"), NULL);
	
	if (dlgOpen.DoModal() == IDOK)
	{		
		m_strSkfFile = dlgOpen.GetPathName();
		SetDlgItemText(IDC_EDIT_SKFDLL, m_strSkfFile);
		CreateSkfApi();
	}
}

void CZJCAUSBKeyTestToolDlg::OnCbnSelchangeComboSkfAsymmalg()
{
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_SKF_ASYMMALG);
	ULONG ulIndex = pComboBox->GetCurSel();
	m_ulSkfAsymmAlg = pComboBox->GetItemData(ulIndex);
}

void CZJCAUSBKeyTestToolDlg::OnCbnSelchangeComboSkfSymmalg()
{
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_SKF_SYMMALG);
	ULONG ulIndex = pComboBox->GetCurSel();
	m_ulSkfSymmAlg = pComboBox->GetItemData(ulIndex);
}

void CZJCAUSBKeyTestToolDlg::OnCbnSelchangeComboSkfHashalg()
{
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_SKF_HASHALG);
	ULONG ulIndex = pComboBox->GetCurSel();
	m_ulSkfHashAlg = pComboBox->GetItemData(ulIndex);
}

void CZJCAUSBKeyTestToolDlg::OnBnClickedCheckTestskfEvent()
{
	if (BST_CHECKED == ((CButton*)GetDlgItem(IDC_CHECK_TESTSKF_EVENT))->GetCheck())
	{
		m_bTestSkfEvent = TRUE;		
		if (m_pSKF)
		{
			m_pSkfEventThread = new CZJCAUSBKeyEventThread();
			m_pSkfEventThread->Start(m_pSKF, FALSE, USBKeyEventFunc, this);
		}
	}
	else
	{
		m_bTestSkfEvent = FALSE;		
		if (m_pSkfEventThread)
		{
			m_pSkfEventThread->Stop();
			delete m_pSkfEventThread;
			m_pSkfEventThread = NULL;
		}
	}
}

void CZJCAUSBKeyTestToolDlg::OnBnClickedCheckSkfcryptP1()
{
	if (BST_CHECKED == ((CButton*)GetDlgItem(IDC_CHECK_SKFCRYPT_P1))->GetCheck())
	{
		m_bTestSkfCryptP1 = TRUE;
	}
	else
	{
		m_bTestSkfCryptP1 = FALSE;
	}
}

void CZJCAUSBKeyTestToolDlg::OnBnClickedCheckSkfsignP1()
{
	if (BST_CHECKED == ((CButton*)GetDlgItem(IDC_CHECK_SKFSIGN_P1))->GetCheck())
	{
		m_bTestSkfSignP1 = TRUE;
	}
	else
	{
		m_bTestSkfSignP1 = FALSE;
	}
}

void CZJCAUSBKeyTestToolDlg::OnBnClickedCheckLogfile()
{
	CHAR csLogFile[MAX_PATH] = {0};

	if (BST_CHECKED == ((CButton*)GetDlgItem(IDC_CHECK_LOGFILE))->GetCheck())
	{
		GetDlgItem(IDC_BTN_LOGFILE)->EnableWindow(TRUE);
		GetDlgItemTextA(m_hWnd, IDC_EDIT_LOGFILE, csLogFile, MAX_PATH);
		if (strlen(csLogFile) > 0)
		{
			USBKey_LogEnabled(TRUE, csLogFile);
		}
	}
	else
	{
		USBKey_LogEnabled(FALSE, "");
		GetDlgItem(IDC_BTN_LOGFILE)->EnableWindow(FALSE);
	}
}

void CZJCAUSBKeyTestToolDlg::OnBnClickedBtnLogfile()
{
	CString strLogFile;
	CFileDialog dlgSave(FALSE, _T(".log"), NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, _T("log文件|*.log|所有文件|*.*||"), NULL);

	USES_CONVERSION;

	if (dlgSave.DoModal() == IDOK)
	{
		HANDLE hFile = NULL;
		ULONG ulWrittenSize = 0;
		TCHAR tcText[256] = {0};
		
		strLogFile = dlgSave.GetPathName();
		SetDlgItemText(IDC_EDIT_LOGFILE, strLogFile);
		USBKey_LogEnabled(TRUE, W2A(strLogFile));
	}
}

void CZJCAUSBKeyTestToolDlg::OnBnClickedBtnStart()
{
	ULONG ulRes = 0;
	ULONG ulErrMsgLen = 512;
	CHAR csErrMsg[512] = {0};

	USES_CONVERSION;

	GetDlgItem(IDC_BTN_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_CLEAN)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_SAVELOG)->EnableWindow(FALSE);
	GetDlgItem(IDOK)->EnableWindow(FALSE);

	if (m_pP11 || m_pSKF)
	{
		CLoginDlg dlg;
		if (dlg.DoModal() == IDOK)
		{
#ifdef UNICODE
			strcpy_s(m_csUserPIN, 64, W2A(dlg.m_strUserPIN));
#else
			strcpy_s(m_csUserPIN, 64, dlg.m_strUserPIN);
#endif	//
		}
		else
		{
			return;
		}
	}
		
	for (ULONG i = 0; i < m_ulTestDataLen; i++)
	{
		m_btTestData[i] = Random(255);
	}

	if (m_pUsbKeyTestThread)
	{
		m_pUsbKeyTestThread->Stop();
		delete m_pUsbKeyTestThread;
		m_pUsbKeyTestThread = NULL;
	}
	m_pUsbKeyTestThread = new CZJCAUSBKeyTestThread();
	ulRes = m_pUsbKeyTestThread->Start();
	if (USBKEY_ERR_OK != ulRes)
	{
		USBKey_GetErrorLog(csErrMsg, &ulErrMsgLen);
		PrintLogMessage(csErrMsg, TRUE);
		goto FUNC_END;
	}

	if (m_pCSP)
	{
		TestCSP();
	}
	if (m_pP11)
	{
		TestP11();
	}
	if (m_pSKF)
	{
		TestSkf();
	}

FUNC_END:
	GetDlgItem(IDC_BTN_START)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_CLEAN)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_SAVELOG)->EnableWindow(TRUE);
	GetDlgItem(IDOK)->EnableWindow(TRUE);
}

void CZJCAUSBKeyTestToolDlg::OnBnClickedBtnClean()
{
	CListBox* pLogList = (CListBox*)GetDlgItem(IDC_LIST_LOG);
	pLogList->ResetContent();
}

void CZJCAUSBKeyTestToolDlg::OnBnClickedBtnSavelog()
{
	CString strSaveFile;
	CListBox* pLogList = (CListBox*)GetDlgItem(IDC_LIST_LOG);
	CFileDialog dlgSave(FALSE, _T(".txt"), NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, _T("文本文件|*.txt|所有文件|*.*||"), NULL);

	if (dlgSave.DoModal() == IDOK)
	{
		HANDLE hFile = NULL;
		ULONG ulWrittenSize = 0;
		TCHAR tcText[256] = {0};
		
		strSaveFile = dlgSave.GetPathName();
		hFile = CreateFile(strSaveFile, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (INVALID_HANDLE_VALUE == hFile)
		{
			MessageBox(_T("打开文件失败！"));
			return;
		}

		for (int iIndex = 0; iIndex < pLogList->GetCount(); iIndex++)
		{
			pLogList->GetText(iIndex, tcText);
			_tcscat_s(tcText, _T("\r\n"));
			WriteFile(hFile, tcText, _tcslen(tcText)*sizeof(TCHAR), &ulWrittenSize, NULL);
		}

		CloseHandle(hFile);
		hFile = NULL;
		MessageBox(_T("文件保存成功！"));
	}
}

ULONG CALLBACK CZJCAUSBKeyTestToolDlg::USBKeyEventFunc(ULONG ulSoltID, 
													   LPTSTR lpDevName, 
													   ULONG ulEventType, 
													   LPVOID lpCaller)
{
	TCHAR tcEventMsg[512] = {0};

	CZJCAUSBKeyTestToolDlg* pDlg = (CZJCAUSBKeyTestToolDlg*)lpCaller;
	if (NULL == pDlg)
	{
		return 0;
	}

	USES_CONVERSION;
	EnterCriticalSection(&pDlg->s_csDevEvent);

	if (_tcslen(lpDevName) > 0 && (1 == ulEventType || 2 == ulEventType))
	{
		_stprintf_s(tcEventMsg, 512, _T("SKF Event: %s is %s!"), lpDevName, (1==ulEventType) ? _T("inserted") : _T("removed"));
	}
	else if (ulSoltID >= 0)
	{
		_stprintf_s(tcEventMsg, 512, _T("P11 Event: %d token is inserted or removed!"), ulSoltID);
	}
	else
	{
		_stprintf_s(tcEventMsg, 512, _T("Unknown event!"));
	}

#ifdef UNICODE
	pDlg->PrintLogMessage(W2A(tcEventMsg), FALSE);
#else
	pDlg->PrintLogMessage(tcEventMsg, FALSE);
#endif	//UNICODE
	
	LeaveCriticalSection(&pDlg->s_csDevEvent);

	return 0;
}

void CZJCAUSBKeyTestToolDlg::InitCSPNameComboBox()
{
	ULONG ulIndex = 0;
	ULONG ulType = 0;
	ULONG ulNameLen = 0;
	CComboBox* pCSPList = (CComboBox*)GetDlgItem(IDC_COMBO_CSPNAME);
	pCSPList->ResetContent();

	while (CryptEnumProviders(ulIndex, NULL, 0, &ulType, NULL, &ulNameLen))
	{	
		if (PROV_RSA_FULL == ulType)
		{
			TCHAR * pName = new TCHAR[ulNameLen + 1 ];
			if (CryptEnumProviders(ulIndex, NULL, 0, &ulType, pName, &ulNameLen))
			{
				 pCSPList->AddString(pName);
			}
			delete []pName;
		}
		ulIndex++;
	}
	pCSPList->SetCurSel(0);
}

void CZJCAUSBKeyTestToolDlg::InitAlgNameComboBox()
{	
	ULONG ulIndex = 0;
	CComboBox* pComboBox = NULL;
	pComboBox = ((CComboBox*)GetDlgItem(IDC_COMBO_CSP_ASYMMALG));
	pComboBox->ResetContent();
	ulIndex = pComboBox->AddString(_T("RSA"));
	pComboBox->SetItemData(ulIndex, USBKEY_ASYMMALG_RSA);
	pComboBox->SetCurSel(0);
	//
	pComboBox = ((CComboBox*)GetDlgItem(IDC_COMBO_CSP_SYMMALG));
	pComboBox->ResetContent();
	ulIndex = pComboBox->AddString(_T("3DES"));
	pComboBox->SetItemData(ulIndex, USBKEY_SYMMALG_3DES);
	pComboBox->SetCurSel(0);
	//
	pComboBox = ((CComboBox*)GetDlgItem(IDC_COMBO_CSP_HASHALG));
	pComboBox->ResetContent();
	ulIndex = pComboBox->AddString(_T("SHA1"));
	pComboBox->SetItemData(ulIndex, USBKEY_HASHALG_SHA1);
	ulIndex = pComboBox->AddString(_T("SHA256"));
	pComboBox->SetItemData(ulIndex, USBKEY_HASHALG_SHA256);
	pComboBox->SetCurSel(0);
		
	pComboBox = ((CComboBox*)GetDlgItem(IDC_COMBO_P11_ASYMMALG));
	pComboBox->ResetContent();
	ulIndex = pComboBox->AddString(_T("RSA"));
	pComboBox->SetItemData(ulIndex, USBKEY_ASYMMALG_RSA);
	pComboBox->SetCurSel(0);
	//
	pComboBox = ((CComboBox*)GetDlgItem(IDC_COMBO_P11_SYMMALG));
	pComboBox->ResetContent();
	ulIndex = pComboBox->AddString(_T("3DES"));
	pComboBox->SetItemData(ulIndex, USBKEY_SYMMALG_3DES);
	pComboBox->SetCurSel(0);
	//
	pComboBox = ((CComboBox*)GetDlgItem(IDC_COMBO_P11_HASHALG));
	pComboBox->ResetContent();
	ulIndex = pComboBox->AddString(_T("SHA1"));
	pComboBox->SetItemData(ulIndex, USBKEY_HASHALG_SHA1);
	ulIndex = pComboBox->AddString(_T("SHA256"));
	pComboBox->SetItemData(ulIndex, USBKEY_HASHALG_SHA256);
	pComboBox->SetCurSel(0);
	
	pComboBox = ((CComboBox*)GetDlgItem(IDC_COMBO_SKF_ASYMMALG));
	pComboBox->ResetContent();
	ulIndex = pComboBox->AddString(_T("SM2"));
	pComboBox->SetItemData(ulIndex, USBKEY_ASYMMALG_SM2);
	pComboBox->SetCurSel(0);
	//
	pComboBox = ((CComboBox*)GetDlgItem(IDC_COMBO_SKF_SYMMALG));
	pComboBox->ResetContent();
	ulIndex = pComboBox->AddString(_T("SM1_ECB"));
	pComboBox->SetItemData(ulIndex, USBKEY_SYMMALG_SM1_ECB);
	ulIndex = pComboBox->AddString(_T("SMS4_ECB"));
	pComboBox->SetItemData(ulIndex, USBKEY_SYMMALG_SMS4_ECB);
	ulIndex = pComboBox->AddString(_T("SSF33_ECB"));
	pComboBox->SetItemData(ulIndex, USBKEY_SYMMALG_SSF33_ECB);
	pComboBox->SetCurSel(0);
	//
	pComboBox = ((CComboBox*)GetDlgItem(IDC_COMBO_SKF_HASHALG));
	pComboBox->ResetContent();
	ulIndex = pComboBox->AddString(_T("SM3"));
	pComboBox->SetItemData(ulIndex, USBKEY_HASHALG_SM3);
	ulIndex = pComboBox->AddString(_T("SHA1"));
	pComboBox->SetItemData(ulIndex, USBKEY_HASHALG_SHA1);
	ulIndex = pComboBox->AddString(_T("SHA256"));
	pComboBox->SetItemData(ulIndex, USBKEY_HASHALG_SHA256);
	pComboBox->SetCurSel(0);
}

void CZJCAUSBKeyTestToolDlg::PrintLogMessage(LPCSTR lpMsg, 
											 BOOL bIsError)
{
	TCHAR tcMsgText[512] = {0};
	CListBox* pLogList = (CListBox*)GetDlgItem(IDC_LIST_LOG);

	USES_CONVERSION;
	
	if (lpMsg && strlen(lpMsg) > 0)
	{
		if (bIsError)
		{
			_tcscpy_s(tcMsgText, 512, _T("[ERROR]:"));
		}
#ifdef UNICODE
		_tcscat_s(tcMsgText, 512, A2W(lpMsg));
#else
		strcat_s(tcMsgText, 512, lpMsg);
#endif	//
		pLogList->AddString(tcMsgText);
	}
}

void CZJCAUSBKeyTestToolDlg::CreateCSPApi()
{
	ULONG ulRes = 0;
	ULONG ulErrMsgLen = 256;
	CHAR csCSPName[256] = {0};
	CHAR csMsgTxt[256] = {0};
	CComboBox* pCSPList = NULL; 
	
	GetDlgItemTextA(m_hWnd, IDC_COMBO_CSPNAME, csCSPName, 256);
	if (strlen(csCSPName) <= 0)
	{
		return;
	}

	if (m_pCSP)
	{
		USBKey_Destroy(m_pCSP);
		m_pCSP = NULL;
	}

	m_pCSP = USBKey_CreateCSP();
	ulRes = m_pCSP->Create(csCSPName);
	if (USBKEY_ERR_OK != ulRes)
	{
		USBKey_GetErrorLog(csMsgTxt, &ulErrMsgLen);		
		PrintLogMessage(csMsgTxt, TRUE);
	}
	else
	{
		sprintf_s(csMsgTxt, 256, "Open CSP: %s successuful!", csCSPName);
		PrintLogMessage(csMsgTxt, FALSE);
	}
}

void CZJCAUSBKeyTestToolDlg::CreateP11Api()
{	
	ULONG ulRes = 0;
	ULONG ulErrMsgLen = 256;
	CHAR csP11File[256] = {0};
	CHAR csMsgTxt[256] = {0};
	
	GetDlgItemTextA(m_hWnd, IDC_EDIT_P11DLL, csP11File, 255);
	if (strlen(csP11File) <= 0)
	{
		return;
	}

	if (m_pP11)
	{
		USBKey_Destroy(m_pP11);
		m_pP11 = NULL;
	}

	m_pP11 = USBKey_CreateP11();
	ulRes = m_pP11->Create(csP11File);
	if (USBKEY_ERR_OK != ulRes)
	{
		USBKey_GetErrorLog(csMsgTxt, &ulErrMsgLen);	
		PrintLogMessage(csMsgTxt, TRUE);
	}
	else
	{
		sprintf_s(csMsgTxt, 256, "Load P11 Dll: %s successuful!", csP11File);
		PrintLogMessage(csMsgTxt, FALSE);
	}
}

void CZJCAUSBKeyTestToolDlg::CreateSkfApi()
{	
	ULONG ulRes = 0;
	ULONG ulErrMsgLen = 256;
	CHAR csSKFFile[256] = {0};
	CHAR csMsgTxt[256] = {0};
	
	GetDlgItemTextA(m_hWnd, IDC_EDIT_SKFDLL, csSKFFile, 255);
	if (strlen(csSKFFile) <= 0)
	{
		return;
	}

	if (m_pSKF)
	{
		USBKey_Destroy(m_pSKF);
		m_pSKF = NULL;
	}

	m_pSKF = USBKey_CreateSKF();
	ulRes = m_pSKF->Create(csSKFFile);
	if (USBKEY_ERR_OK != ulRes)
	{
		USBKey_GetErrorLog(csMsgTxt, &ulErrMsgLen);	
		PrintLogMessage(csMsgTxt, TRUE);
	}
	else
	{
		sprintf_s(csMsgTxt, 256, "Load SFK Dll: %s successuful!", csSKFFile);
		PrintLogMessage(csMsgTxt, FALSE);
	}
}

void CZJCAUSBKeyTestToolDlg::DestoryUsbKeyApi()
{
	if (m_pP11EventThread)
	{
		m_pP11EventThread->Stop();
		delete m_pP11EventThread;
		m_pP11EventThread = NULL;
	}
	if (m_pSkfEventThread)
	{
		m_pSkfEventThread->Stop();
		delete m_pSkfEventThread;
		m_pSkfEventThread = NULL;
	}
	if (m_pUsbKeyTestThread)
	{
		m_pUsbKeyTestThread->Stop();
		delete m_pUsbKeyTestThread;
		m_pUsbKeyTestThread = NULL;
	}
	if (m_pCSP)
	{
		USBKey_Destroy(m_pCSP);
		m_pCSP = NULL;
	}
	if (m_pP11)
	{
		USBKey_Destroy(m_pP11);
		m_pP11 = NULL;
	}
	if (m_pSKF)
	{
		USBKey_Destroy(m_pSKF);
		m_pSKF = NULL;
	}
	if (s_csDevEvent.OwningThread)
	{
		DeleteCriticalSection(&s_csDevEvent);
		memset(&s_csDevEvent, 0, sizeof(s_csDevEvent));
	}
}

void CZJCAUSBKeyTestToolDlg::TestCSP()
{
	ULONG ulRes = 0;
	BOOL bTestPass = FALSE;
	ULONG ulErrMsgLen = 512;
	CHAR csErrMsg[512] = {0};
	ULONG ulEncryptedLen = 0;
	ULONG ulDecryptedLen = 0;
	ULONG ulSignatureLen = 0;
	LPBYTE lpbtEncryptedData = NULL;
	LPBYTE lpbtDecryptedData = NULL;
	LPBYTE lpbtSignature = NULL;
	time_t tStart = 0;
	time_t tEnd = 0;

	if (!m_pCSP || !m_pUsbKeyTestThread)
	{
		return;
	}
	
	PrintLogMessage("******************CSP Test Started******************", FALSE);

	ulRes = m_pCSP->Initialize(m_ulCspAsymmAlg, m_ulCspSymmAlg, m_ulCspHashAlg, "");
	if (USBKEY_ERR_OK != ulRes)
	{
		USBKey_GetErrorLog(csErrMsg, &ulErrMsgLen);
		PrintLogMessage(csErrMsg, TRUE);
		goto FUNC_END;
	}

	ulRes = m_pCSP->ConnectDev();
	if (USBKEY_ERR_OK != ulRes)
	{
		USBKey_GetErrorLog(csErrMsg, &ulErrMsgLen);
		PrintLogMessage(csErrMsg, TRUE);
		goto FUNC_END;
	}
	
	lpbtEncryptedData = new BYTE[m_ulTestDataLen + 2048];
	lpbtDecryptedData = new BYTE[m_ulTestDataLen + 2048];
	lpbtSignature = new BYTE[m_ulTestDataLen + 2048];
	
	if (m_bTestCspCryptP7)
	{
		bTestPass = FALSE;
		PrintLogMessage("CSP crypto P7 test is started!", FALSE);

		PrintLogMessage("Envelop data is starting...", FALSE);
		tStart = GetCurrentTime();
		ulEncryptedLen = m_ulTestDataLen + 2048;
		ulRes = m_pUsbKeyTestThread->TestEnvelop(m_pCSP, m_btTestData, m_ulTestDataLen, lpbtEncryptedData, &ulEncryptedLen);
		if (USBKEY_ERR_OK != ulRes)
		{
			USBKey_GetErrorLog(csErrMsg, &ulErrMsgLen);
			PrintLogMessage(csErrMsg, TRUE);
			goto CRYPT_P7_END;
		}
		tEnd = GetCurrentTime();
		sprintf_s(csErrMsg, 512, "Envelop data is end! Data size:%d KBytes, time:%.2f s, speed:%.2f KBytes/s", 
				  m_ulTestDataLen/1024, (tEnd-tStart)/1000.0f, (m_ulTestDataLen/1024) / ((tEnd-tStart)/1000.0f));
		PrintLogMessage(csErrMsg, FALSE);
		
		PrintLogMessage("Develop data is starting...", FALSE);
		tStart = GetCurrentTime();
		ulDecryptedLen = m_ulTestDataLen + 2048;
		ulRes = m_pUsbKeyTestThread->TestDevelop(m_pCSP, lpbtEncryptedData, ulEncryptedLen, lpbtDecryptedData, &ulDecryptedLen);
		if (USBKEY_ERR_OK != ulRes)
		{
			USBKey_GetErrorLog(csErrMsg, &ulErrMsgLen);
			PrintLogMessage(csErrMsg, TRUE);
			goto CRYPT_P7_END;
		}
		tEnd = GetCurrentTime();
		sprintf_s(csErrMsg, 512, "Develop data is end! Data size:%d KBytes, time:%.2f s, speed:%.2f KBytes/s", 
				  m_ulTestDataLen/1024, (tEnd-tStart)/1000.0f, (m_ulTestDataLen/1024) / ((tEnd-tStart)/1000.0f));
		PrintLogMessage(csErrMsg, FALSE);

		if ((ulDecryptedLen == m_ulTestDataLen) && memcmp(m_btTestData, lpbtDecryptedData, ulDecryptedLen) == 0)
		{
			bTestPass = TRUE;
		}
		else
		{
			PrintLogMessage("The decrypted data is not same as source!", FALSE);
			bTestPass = FALSE;
		}

CRYPT_P7_END:
		if (bTestPass)
		{
			PrintLogMessage("CSP crypto P7 test pass!", FALSE);
		}
		else
		{
			PrintLogMessage("CSP crypto P7 test failed!", FALSE);
		}
	}
	
	ulErrMsgLen = 512;
	if (m_bTestCspCryptP1)
	{
		bTestPass = FALSE;
		PrintLogMessage("CSP crypto P1 test is started!", FALSE);

		PrintLogMessage("Encrypt data is starting...", FALSE);
		tStart = GetCurrentTime();
		ulEncryptedLen = m_ulTestDataLen + 2048;
		ulRes = m_pUsbKeyTestThread->TestEncrypt(m_pCSP, m_btTestData, m_ulTestDataLen, lpbtEncryptedData, &ulEncryptedLen);
		if (USBKEY_ERR_OK != ulRes)
		{
			USBKey_GetErrorLog(csErrMsg, &ulErrMsgLen);
			PrintLogMessage(csErrMsg, TRUE);
			goto CRYPT_P1_END;
		}
		tEnd = GetCurrentTime();
		sprintf_s(csErrMsg, 512, "Encrypt data is end! Data size:%d KBytes, time:%.2f s, speed:%.2f KBytes/s", 
				  m_ulTestDataLen/1024, (tEnd-tStart)/1000.0f, (m_ulTestDataLen/1024) / ((tEnd-tStart)/1000.0f));
		PrintLogMessage(csErrMsg, FALSE);
		
		PrintLogMessage("Decrypt data is starting...", FALSE);
		tStart = GetCurrentTime();
		ulDecryptedLen = m_ulTestDataLen + 2048;
		ulRes = m_pUsbKeyTestThread->TestDecrypt(m_pCSP, lpbtEncryptedData, ulEncryptedLen, lpbtDecryptedData, &ulDecryptedLen);
		if (USBKEY_ERR_OK != ulRes)
		{
			USBKey_GetErrorLog(csErrMsg, &ulErrMsgLen);
			PrintLogMessage(csErrMsg, TRUE);
			goto CRYPT_P1_END;
		}
		tEnd = GetCurrentTime();
		sprintf_s(csErrMsg, 512, "Decrypt data is end! Data size:%d KBytes, time:%.2f s, speed:%.2f KBytes/s", 
				  m_ulTestDataLen/1024, (tEnd-tStart)/1000.0f, (m_ulTestDataLen/1024) / ((tEnd-tStart)/1000.0f));
		PrintLogMessage(csErrMsg, FALSE);
		
		if ((ulDecryptedLen == m_ulTestDataLen) && memcmp(m_btTestData, lpbtDecryptedData, ulDecryptedLen) == 0)
		{
			bTestPass = TRUE;
		}
		else
		{
			PrintLogMessage("The decrypted data is not same as source!", FALSE);
			bTestPass = FALSE;
		}

CRYPT_P1_END:
		if (bTestPass)
		{
			PrintLogMessage("CSP crypto P1 test pass!", FALSE);
		}
		else
		{
			PrintLogMessage("CSP crypto P1 test failed!", FALSE);
		}
	}
	
	ulErrMsgLen = 512;
	if (m_bTestCspSignP7)
	{
		bTestPass = FALSE;
		PrintLogMessage("CSP sign/verify P7 test is started!", FALSE);
		
		PrintLogMessage("Sign data to P7 is starting...", FALSE);
		tStart = GetCurrentTime();
		ulSignatureLen = m_ulTestDataLen + 2048;
		ulRes = m_pUsbKeyTestThread->TestSignP7(m_pCSP, m_btTestData, m_ulTestDataLen, lpbtSignature, &ulSignatureLen);
		if (USBKEY_ERR_OK != ulRes)
		{
			USBKey_GetErrorLog(csErrMsg, &ulErrMsgLen);
			PrintLogMessage(csErrMsg, TRUE);
			goto SIGN_P7_END;
		}
		tEnd = GetCurrentTime();
		sprintf_s(csErrMsg, 512, "Sign data to P7 is end! Data size:%d KBytes, time:%.2f s, speed:%.2f KBytes/s", 
				  m_ulTestDataLen/1024, (tEnd-tStart)/1000.0f, (m_ulTestDataLen/1024) / ((tEnd-tStart)/1000.0f));
		PrintLogMessage(csErrMsg, FALSE);
		
		PrintLogMessage("Verify P7 signature is starting...", FALSE);
		tStart = GetCurrentTime();
		ulRes = m_pUsbKeyTestThread->TestVerifyP7(m_pCSP, m_btTestData, m_ulTestDataLen, lpbtSignature, ulSignatureLen);
		if (USBKEY_ERR_OK != ulRes)
		{
			USBKey_GetErrorLog(csErrMsg, &ulErrMsgLen);
			PrintLogMessage(csErrMsg, TRUE);
			goto SIGN_P7_END;
		}
		else
		{
			bTestPass = TRUE;
		}
		tEnd = GetCurrentTime();
		sprintf_s(csErrMsg, 512, "Verify P7 signature is end! Data size:%d KBytes, time:%.2f s, speed:%.2f KBytes/s", 
				  m_ulTestDataLen/1024, (tEnd-tStart)/1000.0f, (m_ulTestDataLen/1024) / ((tEnd-tStart)/1000.0f));
		PrintLogMessage(csErrMsg, FALSE);

SIGN_P7_END:		
		if (bTestPass)
		{
			PrintLogMessage("CSP sign/verify P7 test pass!", FALSE);
		}
		else
		{
			PrintLogMessage("CSP sign/verify P7 test failed!", FALSE);
		}
	}
	
	ulErrMsgLen = 512;
	if (m_bTestCspSignP1)
	{
		bTestPass = FALSE;
		PrintLogMessage("CSP sign/verify P1 test is started!", FALSE);
		
		PrintLogMessage("Sign data to P1 is starting...", FALSE);
		tStart = GetCurrentTime();
		ulSignatureLen = m_ulTestDataLen + 2048;
		ulRes = m_pUsbKeyTestThread->TestSignP1(m_pCSP,m_btTestData, m_ulTestDataLen, NULL, lpbtSignature, &ulSignatureLen);
		if (USBKEY_ERR_OK != ulRes)
		{
			USBKey_GetErrorLog(csErrMsg, &ulErrMsgLen);
			PrintLogMessage(csErrMsg, TRUE);
			goto SIGN_P1_END;
		}
		tEnd = GetCurrentTime();
		sprintf_s(csErrMsg, 512, "Sign data to P1 is end! Data size:%d KBytes, time:%.2f s, speed:%.2f KBytes/s", 
				  m_ulTestDataLen/1024, (tEnd-tStart)/1000.0f, (m_ulTestDataLen/1024) / ((tEnd-tStart)/1000.0f));
		PrintLogMessage(csErrMsg, FALSE);
		
		PrintLogMessage("Verify P1 signature is starting...", FALSE);
		tStart = GetCurrentTime();
		ulRes = m_pUsbKeyTestThread->TestVerifyP1(m_pCSP,m_btTestData, m_ulTestDataLen, NULL, lpbtSignature, ulSignatureLen);
		if (USBKEY_ERR_OK != ulRes)
		{
			USBKey_GetErrorLog(csErrMsg, &ulErrMsgLen);
			PrintLogMessage(csErrMsg, TRUE);
			goto SIGN_P1_END;
		}
		else
		{
			bTestPass = TRUE;
		}
		tEnd = GetCurrentTime();
		sprintf_s(csErrMsg, 512, "Verify P1 signature is end! Data size:%d KBytes, time:%.2f s, speed:%.2f KBytes/s", 
				  m_ulTestDataLen/1024, (tEnd-tStart)/1000.0f, (m_ulTestDataLen/1024) / ((tEnd-tStart)/1000.0f));
		PrintLogMessage(csErrMsg, FALSE);

SIGN_P1_END:		
		if (bTestPass)
		{
			PrintLogMessage("CSP sign/verify P1 test pass!", FALSE);
		}
		else
		{
			PrintLogMessage("CSP sign/verify P1 test failed!", FALSE);
		}
	}

FUNC_END:
	if (lpbtEncryptedData)
	{
		delete []lpbtEncryptedData;
		lpbtEncryptedData = NULL;
	}
	if (lpbtDecryptedData)
	{
		delete []lpbtDecryptedData;
		lpbtDecryptedData = NULL;
	}
	if (lpbtSignature)
	{
		delete []lpbtSignature;
		lpbtSignature = NULL;
	}
	if (m_pCSP)
	{
		m_pCSP->DisconnectDev();
	}
	PrintLogMessage("******************CSP Test Finished******************", FALSE);
	PrintLogMessage(" ", FALSE);

	return;
}

void CZJCAUSBKeyTestToolDlg::TestP11()
{
	ULONG ulRes = 0;
	BOOL bTestPass = FALSE;
	ULONG ulErrMsgLen = 512;
	CHAR csErrMsg[512] = {0};
	ULONG ulEncryptedLen = 0;
	ULONG ulDecryptedLen = 0;
	ULONG ulSignatureLen = 0;
	LPBYTE lpbtEncryptedData = NULL;
	LPBYTE lpbtDecryptedData = NULL;
	LPBYTE lpbtSignature = NULL;
	time_t tStart = 0;
	time_t tEnd = 0;

	if (!m_pP11)
	{
		return;
	}
	
	PrintLogMessage("******************P11 Test Started******************", FALSE);

	ulRes = m_pP11->Initialize(m_ulP11AsymmAlg, m_ulP11SymmAlg, m_ulP11HashAlg, m_csUserPIN);
	if (USBKEY_ERR_OK != ulRes)
	{
		USBKey_GetErrorLog(csErrMsg, &ulErrMsgLen);
		PrintLogMessage(csErrMsg, TRUE);
		goto FUNC_END;
	}

	ulRes = m_pP11->ConnectDev();
	if (USBKEY_ERR_OK != ulRes)
	{
		USBKey_GetErrorLog(csErrMsg, &ulErrMsgLen);
		PrintLogMessage(csErrMsg, TRUE);
		goto FUNC_END;
	}
	
	lpbtEncryptedData = new BYTE[m_ulTestDataLen + 512];
	lpbtDecryptedData = new BYTE[m_ulTestDataLen + 512];
	lpbtSignature = new BYTE[m_ulTestDataLen + 2048];

	if (m_bTestP11CryptP1)
	{
		bTestPass = FALSE;
		PrintLogMessage("P11 crypto P1 test is started!", FALSE);
		
		PrintLogMessage("Encrypt data is starting...", FALSE);
		tStart = GetCurrentTime();
		ulEncryptedLen = m_ulTestDataLen + 512;
		ulRes = m_pUsbKeyTestThread->TestEncrypt(m_pP11, m_btTestData, m_ulTestDataLen, lpbtEncryptedData, &ulEncryptedLen);
		if (USBKEY_ERR_OK != ulRes)
		{
			USBKey_GetErrorLog(csErrMsg, &ulErrMsgLen);
			PrintLogMessage(csErrMsg, TRUE);
			goto CRYPT_P1_END;
		}
		tEnd = GetCurrentTime();
		sprintf_s(csErrMsg, 512, "Encrypt data is end! Data size:%d KBytes, time:%.2f s, speed:%.2f KBytes/s", 
				  m_ulTestDataLen/1024, (tEnd-tStart)/1000.0f, (m_ulTestDataLen/1024) / ((tEnd-tStart)/1000.0f));
		PrintLogMessage(csErrMsg, FALSE);
		
		PrintLogMessage("Decrypt data is starting...", FALSE);
		tStart = GetCurrentTime();
		ulDecryptedLen = m_ulTestDataLen + 512;
		ulRes = m_pUsbKeyTestThread->TestDecrypt(m_pP11, lpbtEncryptedData, ulEncryptedLen, lpbtDecryptedData, &ulDecryptedLen);
		if (USBKEY_ERR_OK != ulRes)
		{
			USBKey_GetErrorLog(csErrMsg, &ulErrMsgLen);
			PrintLogMessage(csErrMsg, TRUE);
			goto CRYPT_P1_END;
		}
		tEnd = GetCurrentTime();
		sprintf_s(csErrMsg, 512, "Decrypt data is end! Data size:%d KBytes, time:%.2f s, speed:%.2f KBytes/s", 
				  m_ulTestDataLen/1024, (tEnd-tStart)/1000.0f, (m_ulTestDataLen/1024) / ((tEnd-tStart)/1000.0f));
		PrintLogMessage(csErrMsg, FALSE);
		
		if ((ulDecryptedLen == m_ulTestDataLen) && memcmp(m_btTestData, lpbtDecryptedData, ulDecryptedLen) == 0)
		{
			bTestPass = TRUE;
		}
		else
		{
			PrintLogMessage("The decrypted data is not same as source!", FALSE);
			bTestPass = FALSE;
		}

CRYPT_P1_END:
		if (bTestPass)
		{
			PrintLogMessage("P11 crypto P1 test pass!", FALSE);
		}
		else
		{
			PrintLogMessage("P11 crypto P1 test failed!", FALSE);
		}
	}
	
	ulErrMsgLen = 512;
	if (m_bTestP11SignP1)
	{
		bTestPass = FALSE;
		PrintLogMessage("P11 sign/verify P1 test is started!", FALSE);
		
		PrintLogMessage("Sign data to P1 is starting...", FALSE);
		tStart = GetCurrentTime();
		ulSignatureLen = m_ulTestDataLen + 2048;
		ulRes = m_pUsbKeyTestThread->TestSignP1(m_pP11, m_btTestData, m_ulTestDataLen, NULL, lpbtSignature, &ulSignatureLen);
		if (USBKEY_ERR_OK != ulRes)
		{
			USBKey_GetErrorLog(csErrMsg, &ulErrMsgLen);
			PrintLogMessage(csErrMsg, TRUE);
			goto SIGN_P1_END;
		}
		tEnd = GetCurrentTime();
		sprintf_s(csErrMsg, 512, "Sign data to P1 is end! Data size:%d KBytes, time:%.2f s, speed:%.2f KBytes/s", 
				  m_ulTestDataLen/1024, (tEnd-tStart)/1000.0f, (m_ulTestDataLen/1024) / ((tEnd-tStart)/1000.0f));
		PrintLogMessage(csErrMsg, FALSE);
		
		PrintLogMessage("Verify P1 signature is starting...", FALSE);
		tStart = GetCurrentTime();
		ulRes = m_pUsbKeyTestThread->TestVerifyP1(m_pP11, m_btTestData, m_ulTestDataLen, NULL, lpbtSignature, ulSignatureLen);
		if (USBKEY_ERR_OK != ulRes)
		{
			USBKey_GetErrorLog(csErrMsg, &ulErrMsgLen);
			PrintLogMessage(csErrMsg, TRUE);
			goto SIGN_P1_END;
		}
		else
		{
			bTestPass = TRUE;
		}
		tEnd = GetCurrentTime();
		sprintf_s(csErrMsg, 512, "Verify P1 signature is end! Data size:%d KBytes, time:%.2f s, speed:%.2f KBytes/s", 
				  m_ulTestDataLen/1024, (tEnd-tStart)/1000.0f, (m_ulTestDataLen/1024) / ((tEnd-tStart)/1000.0f));
		PrintLogMessage(csErrMsg, FALSE);

SIGN_P1_END:		
		if (bTestPass)
		{
			PrintLogMessage("P11 sign/verify P1 test pass!", FALSE);
		}
		else
		{
			PrintLogMessage("P11 sign/verify P1 test failed!", FALSE);
		}
	}

FUNC_END:
	if (lpbtEncryptedData)
	{
		delete []lpbtEncryptedData;
		lpbtEncryptedData = NULL;
	}
	if (lpbtDecryptedData)
	{
		delete []lpbtDecryptedData;
		lpbtDecryptedData = NULL;
	}
	if (lpbtSignature)
	{
		delete []lpbtSignature;
		lpbtSignature = NULL;
	}
	if (m_pP11)
	{
		m_pP11->DisconnectDev();
	}
	PrintLogMessage("******************P11 Test Finished******************", FALSE);
	PrintLogMessage(" ", FALSE);

	return;
}

void CZJCAUSBKeyTestToolDlg::TestSkf()
{
	ULONG ulRes = 0;
	BOOL bTestPass = FALSE;
	ULONG ulErrMsgLen = 512;
	CHAR csErrMsg[512] = {0};
	ULONG ulEncryptedLen = 0;
	ULONG ulDecryptedLen = 0;
	ULONG ulSignatureLen = 0;
	LPBYTE lpbtEncryptedData = NULL;
	LPBYTE lpbtDecryptedData = NULL;
	LPBYTE lpbtSignature = NULL;
	CHAR csUserID[16] = {0};
	time_t tStart = 0;
	time_t tEnd = 0;

	if (!m_pSKF)
	{
		return;
	}
	if (USBKEY_ASYMMALG_SM2 == m_ulSkfAsymmAlg)
	{
		strcpy_s(csUserID, 16, "12345678");
	}
	
	PrintLogMessage("******************SKF Test Started******************", FALSE);

	ulRes = m_pSKF->Initialize(m_ulSkfAsymmAlg, m_ulSkfSymmAlg, m_ulSkfHashAlg, m_csUserPIN);
	if (USBKEY_ERR_OK != ulRes)
	{
		USBKey_GetErrorLog(csErrMsg, &ulErrMsgLen);
		PrintLogMessage(csErrMsg, TRUE);
		goto FUNC_END;
	}

	ulRes = m_pSKF->ConnectDev();
	if (USBKEY_ERR_OK != ulRes)
	{
		USBKey_GetErrorLog(csErrMsg, &ulErrMsgLen);
		PrintLogMessage(csErrMsg, TRUE);
		goto FUNC_END;
	}
	
	lpbtEncryptedData = new BYTE[m_ulTestDataLen + 512];
	lpbtDecryptedData = new BYTE[m_ulTestDataLen + 512];
	lpbtSignature = new BYTE[m_ulTestDataLen + 2048];

	if (m_bTestSkfCryptP1)
	{
		bTestPass = FALSE;
		PrintLogMessage("SKF crypto P1 test is started!", FALSE);
		
		PrintLogMessage("Encrypt data is starting...", FALSE);
		tStart = GetCurrentTime();
		ulEncryptedLen = m_ulTestDataLen + 512;
		ulRes = m_pUsbKeyTestThread->TestEncrypt(m_pSKF, m_btTestData, m_ulTestDataLen, lpbtEncryptedData, &ulEncryptedLen);
		if (USBKEY_ERR_OK != ulRes)
		{
			USBKey_GetErrorLog(csErrMsg, &ulErrMsgLen);
			PrintLogMessage(csErrMsg, TRUE);
			goto CRYPT_P1_END;
		}
		tEnd = GetCurrentTime();
		sprintf_s(csErrMsg, 512, "Encrypt data is end! Data size:%d KBytes, time:%.2f s, speed:%.2f KBytes/s", 
				  m_ulTestDataLen/1024, (tEnd-tStart)/1000.0f, (m_ulTestDataLen/1024) / ((tEnd-tStart)/1000.0f));
		PrintLogMessage(csErrMsg, FALSE);
		
		PrintLogMessage("Decrypt data is starting...", FALSE);
		tStart = GetCurrentTime();
		ulDecryptedLen = m_ulTestDataLen + 512;
		ulRes = m_pUsbKeyTestThread->TestDecrypt(m_pSKF, lpbtEncryptedData, ulEncryptedLen, lpbtDecryptedData, &ulDecryptedLen);
		if (USBKEY_ERR_OK != ulRes)
		{
			USBKey_GetErrorLog(csErrMsg, &ulErrMsgLen);
			PrintLogMessage(csErrMsg, TRUE);
			goto CRYPT_P1_END;
		}
		tEnd = GetCurrentTime();
		sprintf_s(csErrMsg, 512, "Decrypt data is end! Data size:%d KBytes, time:%.2f s, speed:%.2f KBytes/s", 
				  m_ulTestDataLen/1024, (tEnd-tStart)/1000.0f, (m_ulTestDataLen/1024) / ((tEnd-tStart)/1000.0f));
		PrintLogMessage(csErrMsg, FALSE);
		
		if ((ulDecryptedLen == m_ulTestDataLen) && memcmp(m_btTestData, lpbtDecryptedData, ulDecryptedLen) == 0)
		{
			bTestPass = TRUE;
		}
		else
		{
			PrintLogMessage("The decrypted data is not same as source!", FALSE);
			bTestPass = FALSE;
		}

CRYPT_P1_END:
		if (bTestPass)
		{
			PrintLogMessage("SKF crypto P1 test pass!", FALSE);
		}
		else
		{
			PrintLogMessage("SKF crypto P1 test failed!", FALSE);
		}
	}
	
	ulErrMsgLen = 512;
	if (m_bTestSkfSignP1)
	{
		bTestPass = FALSE;
		PrintLogMessage("SKF sign/verify P1 test is started!", FALSE);
		
		PrintLogMessage("Sign data to P1 is starting...", FALSE);
		tStart = GetCurrentTime();
		ulSignatureLen = m_ulTestDataLen + 2048;
		ulRes = m_pUsbKeyTestThread->TestSignP1(m_pSKF, m_btTestData, m_ulTestDataLen, csUserID, lpbtSignature, &ulSignatureLen);
		if (USBKEY_ERR_OK != ulRes)
		{
			USBKey_GetErrorLog(csErrMsg, &ulErrMsgLen);
			PrintLogMessage(csErrMsg, TRUE);
			goto SIGN_P1_END;
		}
		tEnd = GetCurrentTime();
		sprintf_s(csErrMsg, 512, "Sign data to P1 is end! Data size:%d KBytes, time:%.2f s, speed:%.2f KBytes/s", 
				  m_ulTestDataLen/1024, (tEnd-tStart)/1000.0f, (m_ulTestDataLen/1024) / ((tEnd-tStart)/1000.0f));
		PrintLogMessage(csErrMsg, FALSE);
		
		PrintLogMessage("Verify P1 signature is starting...", FALSE);
		tStart = GetCurrentTime();
		ulRes = m_pUsbKeyTestThread->TestVerifyP1(m_pSKF, m_btTestData, m_ulTestDataLen, csUserID, lpbtSignature, ulSignatureLen);
		if (USBKEY_ERR_OK != ulRes)
		{
			USBKey_GetErrorLog(csErrMsg, &ulErrMsgLen);
			PrintLogMessage(csErrMsg, TRUE);
			goto SIGN_P1_END;
		}
		else
		{
			bTestPass = TRUE;
		}
		tEnd = GetCurrentTime();
		sprintf_s(csErrMsg, 512, "Verify P1 signature is end! Data size:%d KBytes, time:%.2f s, speed:%.2f KBytes/s", 
				  m_ulTestDataLen/1024, (tEnd-tStart)/1000.0f, (m_ulTestDataLen/1024) / ((tEnd-tStart)/1000.0f));
		PrintLogMessage(csErrMsg, FALSE);

SIGN_P1_END:		
		if (bTestPass)
		{
			PrintLogMessage("SKF sign/verify P1 test pass!", FALSE);
		}
		else
		{
			PrintLogMessage("SKF sign/verify P1 test failed!", FALSE);
		}
	}

FUNC_END:
	if (lpbtEncryptedData)
	{
		delete []lpbtEncryptedData;
		lpbtEncryptedData = NULL;
	}
	if (lpbtDecryptedData)
	{
		delete []lpbtDecryptedData;
		lpbtDecryptedData = NULL;
	}
	if (lpbtSignature)
	{
		delete []lpbtSignature;
		lpbtSignature = NULL;
	}
	if (m_pSKF)
	{
		m_pSKF->DisconnectDev();
	}
	PrintLogMessage("******************SKF Test Finished******************", FALSE);
	PrintLogMessage(" ", FALSE);

	return;
}
