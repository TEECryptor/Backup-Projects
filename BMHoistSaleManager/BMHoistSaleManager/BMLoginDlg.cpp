//-----------------------------------------------------------------------
//	File Name:BMLoginDlg.cpp
//	Introduce:Class CBMLoginDlg implementation file
//-----------------------------------------------------------------------
#include "stdafx.h"
#include "BMHoistSaleManager.h"
#include "BMLoginDlg.h"
#include "CommonDefine.h"
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
// CBMLoginDlg dialog
//-----------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CBMLoginDlg, CDialog)
CBMLoginDlg::CBMLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBMLoginDlg::IDD, pParent)
{
	m_pKImage = NULL;
}
//-----------------------------------------------------------------------
CBMLoginDlg::~CBMLoginDlg()
{
}
//-----------------------------------------------------------------------
void CBMLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}
//-----------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CBMLoginDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDOK, &CBMLoginDlg::OnBnClickedOk)	
END_MESSAGE_MAP()


//-----------------------------------------------------------------------
// CBMLoginDlg message handlers
//-----------------------------------------------------------------------
BOOL CBMLoginDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CRect rcDlg(0, 0, 606, 450);
	MoveWindow(&rcDlg);
	CenterWindow();
	InitControlsPos();

	m_pKImage = new CBitmap();
	m_pKImage->LoadBitmap(IDB_BITMAP_LOGINBK);

	GetDlgItem(IDC_EDIT_NAME)->SetFocus();

	return FALSE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
//-----------------------------------------------------------------------
void CBMLoginDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	RECT rc;
	GetClientRect(&rc);
	int iWidth = rc.right - rc.left;
	int iHeight = rc.bottom - rc.top;
	HDC hDCMem = CreateCompatibleDC(dc.m_hDC);
	int iOldDcBkMode = SetBkMode(dc.m_hDC, TRANSPARENT);
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(hDCMem, (HBITMAP)m_pKImage->m_hObject);	
	
	BitBlt(	dc.m_hDC, 
			0, 0, iWidth, iHeight,
			hDCMem,
			0, 0,
			SRCCOPY);
			
	SelectObject(hDCMem, hOldBitmap);
	SetBkMode(hDCMem, iOldDcBkMode);
	DeleteDC(hDCMem);
}
//-----------------------------------------------------------------------
void CBMLoginDlg::OnDestroy()
{
	CDialog::OnDestroy();

	if (NULL != m_pKImage)
	{
		delete m_pKImage;
		m_pKImage = NULL;
	}
}
//-----------------------------------------------------------------------
void CBMLoginDlg::OnBnClickedOk()
{
	CString strUserID;
	CString strPassword;
	CString strUserName;
	GetDlgItemText(IDC_EDIT_NAME, strUserID);
	GetDlgItemText(IDC_EDIT_PASSWORD, strPassword);

	CADOTable tbUsersInfo;
	CString strOperations;
	strOperations.Format(_T("SELECT * FROM %s WHERE ID='%s'"), g_tcTableName[eTableID_EmployeeInfo], strUserID);
	if (FALSE == tbUsersInfo.OpenTable(g_pSettingDatabase->GetConnectionPtr(), strOperations))
	{
		MessageBox(_T("The inputed User ID is not exist!"), _T("Error"), MB_OK | MB_ICONERROR);
		return;
	}

	DWORD dwUserCnt = tbUsersInfo.GetRecordCnt();
	if (0 == dwUserCnt)
	{
		MessageBox(_T("The inputed User ID is not exist!"), _T("Error"), MB_OK | MB_ICONERROR);
		GetDlgItem(IDC_EDIT_NAME)->SetFocus();
		tbUsersInfo.CloseTable();
		return;
	}

	CString strStdPassword;
	tbUsersInfo.GetRecordCell(0, L"UserName", strUserName.GetBuffer(32), 32);
	tbUsersInfo.GetRecordCell(0, L"Password", strStdPassword.GetBuffer(32), 32);
	if (strStdPassword != strPassword)
	{
		MessageBox(_T("The inputed User ID/Password are incorrect, please input again!"), _T("Error"), MB_OK | MB_ICONERROR);
		GetDlgItem(IDC_EDIT_PASSWORD)->SetFocus();
		tbUsersInfo.CloseTable();
		return;
	}

	strUserID.MakeUpper();
	g_LoginUserInfo.C30 = strUserID;
	g_LoginUserInfo.C31 = strUserName;	
	tbUsersInfo.GetRecordCell(0, L"Phone", g_LoginUserInfo.C32.GetBuffer(32), 32);
	tbUsersInfo.GetRecordCell(0, L"Fax", g_LoginUserInfo.C33.GetBuffer(32), 32);
	tbUsersInfo.GetRecordCell(0, L"Email", g_LoginUserInfo.C34.GetBuffer(32), 32);
	tbUsersInfo.GetRecordCell(0, L"Mobile", g_LoginUserInfo.C35.GetBuffer(32), 32);
	tbUsersInfo.GetRecordCell(0, L"Group", (DWORD&)g_LoginUserInfo.C36);
	tbUsersInfo.GetRecordCell(0, L"Company", g_LoginUserInfo.C37.GetBuffer(32), 32);
	tbUsersInfo.GetRecordCell(0, L"Brand", g_LoginUserInfo.C38.GetBuffer(32), 32);

	tbUsersInfo.CloseTable();

	CDialog::OnOK();
}
//-----------------------------------------------------------------------



//-----------------------------------------------------------------------
void CBMLoginDlg::InitControlsPos()
{
	CRect rcClient;
	GetClientRect(&rcClient);

	int iOffsetY = 0;
	CRect rcStaticWnd;
	CWnd* pStaticWnd = GetDlgItem(IDC_STATIC_LOGIN);
	if (NULL != pStaticWnd)
	{
		pStaticWnd->GetWindowRect(&rcStaticWnd);
		ScreenToClient(&rcStaticWnd);
		int iHeight = rcStaticWnd.Height();
		int iOldBottom = rcStaticWnd.bottom;
		rcStaticWnd.left = 10;
		rcStaticWnd.right = rcClient.right - 10;
		rcStaticWnd.bottom = rcClient.bottom - 10;
		rcStaticWnd.top = rcStaticWnd.bottom - iHeight;
		iOffsetY = rcStaticWnd.bottom - iOldBottom;
		pStaticWnd->MoveWindow(&rcStaticWnd);
	}
	
	// Options 	
	UINT nBtnIDs[] = {IDC_STATIC_NAME, IDC_EDIT_NAME, IDOK, IDC_STATIC_PASSWORD, IDC_EDIT_PASSWORD, IDCANCEL};
	for (int i = 0; i < sizeof(nBtnIDs) / sizeof(nBtnIDs[0]); i++)
	{
		CWnd* pBtn = GetDlgItem(nBtnIDs[i]);
		if(NULL != pBtn) 
		{	
			CRect rc;
			pBtn->GetWindowRect(&rc);
			ScreenToClient(&rc);
			rc.OffsetRect(0, iOffsetY);
			pBtn->MoveWindow(&rc);
		}
	}

}
//-----------------------------------------------------------------------
