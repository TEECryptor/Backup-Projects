// TestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Test.h"
#include "TestDlg.h"
#include "comdef.h" 
//#include <atlcomcli.h>
#include "msword.h"
#include "mapi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//---------------------------------------------------

#import "c:\\Program Files\\Common Files\\Microsoft Shared\\Office10\\mso.dll" named_guids
#import "C:\Program Files\Common Files\Microsoft Shared\VBA\VBA6\VBE6EXT.OLB" named_guids
//#import "C:\\Program Files\\Microsoft Office\\Office10\\MSWORD.OLB" rename_namespace("Word") 
#import "C:\\Program Files\\Microsoft Office\\Office10\\MSOUTL.OLB" rename_namespace("Outlook") no_dual_interfaces rename("CopyFile","_CopyFile") //exclude("_IRecipientControl", "_DRecipientControl")

#import "C:\\Program Files\\Common Files\\System\\Mapi\\1033\\cdo.DLL" rename_namespace("MAPI") //raw_interfaces_only
//---------------------------------------------------
using namespace Outlook;
//using namespace Word;
using namespace MAPI;
//---------------------------------------------------
typedef _com_ptr_t<_com_IIID<Range, 0x0> > RangePtr;
typedef _com_ptr_t<_com_IIID<_Document, 0x0> > _DocumentPtr;

LPMAPISENDMAIL pfnSendMail = NULL;
LPMAPILOGOFF   pfnLogoff = NULL;
LPMAPILOGON    pfnLogon = NULL;
LPMAPIRESOLVENAME lpfnMAPIResolveName = NULL;
//---------------------------------------------------
// CAboutDlg dialog used for App About
//---------------------------------------------------
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestDlg dialog

CTestDlg::CTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTestDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTestDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTestDlg, CDialog)
	//{{AFX_MSG_MAP(CTestDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestDlg message handlers

BOOL CTestDlg::OnInitDialog()
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

	if (CoInitialize(NULL) != 0) 
	{ 
		AfxMessageBox("初始化COM支持库失败!"); 
		exit(1); 
	} 
	
	//CoUninitialize();
	
	InitMAPIFunc();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTestDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

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

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTestDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}
//--------------------------------------
BOOL CTestDlg::InitMAPIFunc()
{ 
	HMODULE hMAPIDll = NULL;
	hMAPIDll = LoadLibrary("MAPI32.DLL");
	if(hMAPIDll == NULL)
	{
	   return FALSE;
	}

	// Die Funktionen aus der DLL holen
	pfnLogon    = (LPMAPILOGON)GetProcAddress(hMAPIDll, "MAPILogon");
	pfnLogoff   = (LPMAPILOGOFF)GetProcAddress(hMAPIDll, "MAPILogoff");
	pfnSendMail = (LPMAPISENDMAIL)GetProcAddress(hMAPIDll, "MAPISendMail");
	lpfnMAPIResolveName =(LPMAPIRESOLVENAME) GetProcAddress(hMAPIDll, "MAPIResolveName");

	if(pfnLogon == NULL || pfnLogoff == NULL || pfnSendMail == NULL || lpfnMAPIResolveName == NULL)
		return FALSE;

   return TRUE;
}
//--------------------------------------
void CTestDlg::OnButton1() 
{
	Outlook::_ApplicationPtr m_pOutlookApplication = NULL;
    Outlook::_MailItemPtr m_pOutlookMailItem = NULL;
	Outlook::AttachmentsPtr pOulookAttachments = NULL;
	Outlook::AttachmentPtr pOulookAttachment = NULL;
    _variant_t comEntryID;

	m_pOutlookApplication.CreateInstance("Outlook.Application");
    m_pOutlookMailItem = m_pOutlookApplication->CreateItem(olMailItem); 
    pOulookAttachments = m_pOutlookMailItem->GetAttachments();
    pOulookAttachment = pOulookAttachments->Add("D:/VC Projects/Test OutLook/Test/Debug/1.jpg");
    m_pOutlookMailItem->PutSubject(_T("Test Message"));
    m_pOutlookMailItem->Close(olSave);

    comEntryID.bstrVal = m_pOutlookMailItem->GetEntryID();

    m_pOutlookMailItem = NULL;
    pOulookAttachment = NULL;
    pOulookAttachments = NULL;

	MAPI::_SessionPtr pMapiSession = NULL;
    MAPI::MessagePtr pMapiMessage = NULL;
    MAPI::AttachmentsPtr pMapiAttactments = NULL;
    MAPI::AttachmentPtr pMapiAttactment = NULL;
    MAPI::FieldsPtr pMapiFields = NULL;
	
    pMapiSession.CreateInstance("MAPI.Session");
	DWORD dwError = GetLastError();

    pMapiSession->Logon("Outlook");
    pMapiMessage = pMapiSession->GetMessage(comEntryID.bstrVal);
    pMapiAttactments = pMapiMessage->GetAttachments();
    pMapiAttactment = pMapiAttactments->GetItem(_variant_t((long)1, VT_I4));
    pMapiFields = pMapiAttactment->GetFields();
    pMapiFields->Add(_variant_t((long)CdoPR_ATTACH_MIME_TAG, VT_ERROR), "image/jpeg");
    pMapiFields->Add(_variant_t((long)0x3712001E, VT_ERROR), "myindent");
    pMapiFields = pMapiMessage->GetFields();
    pMapiFields->Add("{0820060000000000C000000000000046}0x8514", _variant_t((long)11, VT_I4), true);
    
	pMapiMessage->PutSubject(_T("Test Message from Outlook/VC++7"));
    pMapiMessage->Update();

	m_pOutlookMailItem = m_pOutlookApplication->GetNamespace("MAPI")->GetItemFromID(comEntryID.bstrVal);
	m_pOutlookMailItem->PutHTMLBody(_T("<IMG align=baseline border=0 hspace=0 src=cid:myident>"));

	m_pOutlookMailItem->PutTo(_T("Click here to reveal e-mail address"));
    m_pOutlookMailItem->Close(olSave);
    m_pOutlookMailItem->Display();

    pMapiSession = NULL;
    pMapiMessage = NULL;
    pMapiAttactments = NULL;
    pMapiAttactment = NULL;
    pMapiFields = NULL;
}

void CTestDlg::OnButton2() 
{
	Outlook::_ApplicationPtr m_pOutlookApplication = NULL;
    Outlook::_MailItemPtr m_pOutlookMailItem = NULL;
	Outlook::AttachmentsPtr pOulookAttachments = NULL;
	Outlook::AttachmentPtr pOulookAttachment = NULL;

	m_pOutlookApplication.CreateInstance("Outlook.Application");
    m_pOutlookMailItem = m_pOutlookApplication->CreateItem(olMailItem); 

	_NameSpacePtr pNameSpace = NULL;
	pNameSpace = m_pOutlookApplication->GetNamespace(L"MAPI");
    pOulookAttachments = m_pOutlookMailItem->GetAttachments();

	_variant_t covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
	pNameSpace->Logon(covOptional, covOptional, covOptional, covOptional);

    pOulookAttachment = pOulookAttachments->Add("C:\\Documents and Settings\\yyf\\My Documents\\Personal Card\\Images\\Logo1.jpg");
    pOulookAttachment = pOulookAttachments->Add("c:/2.jpg");
    pOulookAttachment = pOulookAttachments->Add("c:/3.jpg");
	m_pOutlookMailItem->PutHTMLBody(_T("<HTML><img src='cid:Logo1.jpg' height=200 width=200><img src='cid:2.JPG' height=200 width=200><img src='cid:3.jpg' height=200 width=200></HTML>"));
	m_pOutlookMailItem->Display();
}


BOOL CTestDlg::UseMAPI()
{
/*	LHANDLE        lHnd = NULL;
	int iRest = pfnLogon(0, NULL, NULL, MAPI_LOGON_UI , 0, &lHnd);

	MapiRecipDesc  rdOriginator;
	MapiRecipDesc  rdRecipient[1];
	MapiFileDesc   mapifiledesc[2];

	MapiMessage    mapimsg;
	mapimsg.ulReserved   = 0;
	mapimsg.lpszSubject  = "Subject";
	mapimsg.lpszNoteText = "Hi!\n\nAnbei ist eine Datei!\n\nGru?\nIch\n"; // EMailtext
	mapimsg.lpszMessageType    = NULL;
	mapimsg.lpszDateReceived   = NULL;
	mapimsg.lpszConversationID = NULL;
	mapimsg.flFlags      = MAPI_SENT;

//	lpfnMAPIResolveName(lHnd, 0,"sender_test@sohu.com", 0, 0, &mapimsg.lpOriginator);
	//mapimsg.lpOriginator->ulRecipClass=MAPI_ORIG;

	mapimsg.nRecipCount  = 1;
	mapimsg.lpRecips     = rdRecipient;
	mapimsg.nFileCount   = 0; //2 Dateien anh?ngen
	mapimsg.lpFiles      = &mapifiledesc[0];

	rdRecipient[0].ulReserved   = 0;
	rdRecipient[0].ulRecipClass = MAPI_TO;
	rdRecipient[0].lpszName     = ""; // Empf?ngername
	rdRecipient[0].lpszAddress  = "SMTP:youfang_yao@sonic.com"; // Empf?ngeradresse
	rdRecipient[0].ulEIDSize    = 0;
	rdRecipient[0].lpEntryID    = NULL;

	mapifiledesc[0].ulReserved   = 0;
	mapifiledesc[0].flFlags      = 0;
	mapifiledesc[0].nPosition    = -1;
	mapifiledesc[0].lpszPathName = NULL;//"C:\\zoom.bmp"; // Die erste Datei
	// so heisst die Datei im Anhang des EMails
	mapifiledesc[0].lpszFileName = NULL;//"Datei.txt"; 
	mapifiledesc[0].lpFileType   = 0;

	// Für die 2te Datei kann man sich dann hierauf beschr?nken:
	mapifiledesc[1].ulReserved   = 0;
	mapifiledesc[1].flFlags      = 0;
	mapifiledesc[1].nPosition    = -1;
	mapifiledesc[1].lpszPathName = NULL;//"C:\\response.txt"; // Die zweite Datei
	mapifiledesc[1].lpszFileName = NULL;//"Log.txt";
	mapifiledesc[1].lpFileType   = 0;

	iRest = pfnSendMail(lHnd, NULL, &mapimsg, MAPI_LOGON_UI|MAPI_DIALOG, 0); // Das eigentliche Versenden
*/

/*
	HMODULE   hMod   =   LoadLibrary("MAPI32.DLL");   

	if   (hMod   ==   NULL)   
	{   
		AfxMessageBox(AFX_IDP_FAILED_MAPI_LOAD);   
		return;   
	}   

	ULONG   (PASCAL   *lpfnSendMail)(ULONG,   ULONG,   MapiMessage*,   FLAGS,   ULONG);   
	(FARPROC&)lpfnSendMail   =   GetProcAddress(hMod,   "MAPISendMail");   

	if   (lpfnSendMail   ==   NULL)   
	{   
		AfxMessageBox(AFX_IDP_INVALID_MAPI_DLL);   
		return;   
	}   

	ASSERT(lpfnSendMail   !=   NULL);   

	TCHAR   szPath[_MAX_PATH]   =   "F:\\MySelf\\VC Projects\\Test\\Debug\\test.txt";   
	TCHAR szTitle[_MAX_PATH]   =   "test.txt";   

	//   prepare   the   file   description   (for   the   attachment)   
	MapiFileDesc   fileDesc;   
	memset(&fileDesc,   0,   sizeof(fileDesc));   
	fileDesc.nPosition   =   (ULONG)-1;   

	fileDesc.lpszPathName   =   szPath;   
	fileDesc.lpszFileName   =   szTitle;   

	//   prepare   the   message   (empty   with   1   attachment)   
	MapiMessage   message;   
	memset(&message,   0,   sizeof(message));   
	message.nFileCount   =   1;   
	message.lpFiles   =   &fileDesc;   

	int   nError   =   lpfnSendMail(0,   0,   
	&message,   MAPI_LOGON_UI|MAPI_DIALOG,   0);   

	//   after   returning   from   the   MAPISendMail   call,   the   window   must   
	//   be   re-enabled   and   focus   returned   to   the   frame   to   undo   the   workaround   
	//   done   before   the   MAPI   call.   
	if   (nError   !=   SUCCESS_SUCCESS   &&   
	nError   !=   MAPI_USER_ABORT   &&   nError   !=   MAPI_E_LOGIN_FAILURE)   
	{   
		AfxMessageBox(AFX_IDP_FAILED_MAPI_SEND);   
	} */  

	return FALSE;
}
