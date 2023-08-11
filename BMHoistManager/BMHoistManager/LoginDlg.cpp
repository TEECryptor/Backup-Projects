// LoginDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BMHoistManager.h"
#include "LoginDlg.h"


// CLoginDlg dialog

IMPLEMENT_DYNAMIC(CLoginDlg, CDialog)

CLoginDlg::CLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLoginDlg::IDD, pParent)
{
	m_pKImage = NULL;
}

CLoginDlg::~CLoginDlg()
{
}

void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CLoginDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDOK, &CLoginDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CLoginDlg message handlers

BOOL CLoginDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_pKImage = new CBitmap();
	m_pKImage->LoadBitmap(IDB_BITMAP_LOGINBK);

	GetDlgItem(IDC_EDIT_LOGINDLG_USERNAME)->SetFocus();

	return FALSE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CLoginDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rcOptionStatic;
	GetDlgItem(IDC_STATIC_LOGIN)->GetWindowRect(&rcOptionStatic);
	ScreenToClient(&rcOptionStatic);

	CRect rcClient;
	GetClientRect(&rcClient);
	int iWidth = rcClient.Width();
	int iHeight = rcClient.Height();
	HDC hDCMem = CreateCompatibleDC(dc.m_hDC);
	int iOldDcBkMode = SetBkMode(dc.m_hDC, TRANSPARENT);
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(hDCMem, (HBITMAP)m_pKImage->m_hObject);	
	
	BITMAP bm;
	m_pKImage->GetObject(sizeof(BITMAP), &bm);  
	StretchBlt(	dc.m_hDC, 
				0, 0, iWidth, rcOptionStatic.top - 10,
				hDCMem,
				0, 0, bm.bmWidth, bm.bmHeight,
				SRCCOPY);

	//BitBlt(	dc.m_hDC, 
	//		0, 0, iWidth, rcOptionStatic.top - 10,
	//		hDCMem,
	//		0, 0,
	//		SRCCOPY);
			
	SelectObject(hDCMem, hOldBitmap);
	SetBkMode(hDCMem, iOldDcBkMode);
	DeleteDC(hDCMem);
}

void CLoginDlg::OnDestroy()
{
	CDialog::OnDestroy();

	if (NULL != m_pKImage)
	{
		delete m_pKImage;
		m_pKImage = NULL;
	}
}

void CLoginDlg::OnBnClickedOk()
{
	CString strInputUserName;
	CString strInputPassword;

	GetDlgItemText(IDC_EDIT_LOGINDLG_USERNAME, strInputUserName);
	GetDlgItemText(IDC_EDIT_LOGINDLG_PSW, strInputPassword);

	CADOTable tmEmployee;
	CString strOpenOperation;
	strOpenOperation.Format(L"SELECT * FROM %s", _T("S001"));
	if (tmEmployee.OpenTable(m_pDatabase->GetConnectionPtr(), strOpenOperation))
	{
		CString strUserName;
		CString strPassword;
		BOOL bIsLogin = FALSE;
		DWORD dwCount = tmEmployee.GetRecordCnt();

		do
		{
			
			BOOL bHasThisUser = FALSE;
			if (!strInputUserName.IsEmpty())
			{
				for (DWORD dwIndex = 0; dwIndex < dwCount; dwIndex++)
				{
					tmEmployee.GetRecordCell(dwIndex, L"LoginName", strUserName.GetBuffer(256), 256);
					tmEmployee.GetRecordCell(dwIndex, L"Password", strPassword.GetBuffer(256), 256);
					if (_tcsicmp(strUserName, strInputUserName) == 0)
					{
						bHasThisUser = TRUE;
						if (strInputPassword == strPassword/*DecodePasswordString(strPassword)*/)
						{
							TCHAR tcUserName[64] = {0};
							TCHAR tcUserGroup[64] = {0};

							bIsLogin = TRUE;
							m_strLoginName = strUserName;
							tmEmployee.GetRecordCell(dwIndex, L"Group", tcUserGroup, 64);
							tmEmployee.GetRecordCell(dwIndex, L"UserName", tcUserName, 64);
							m_strUserName = tcUserName;
							m_strUserGroup = tcUserGroup;
							break;
						}
						else
						{
							tmEmployee.CloseTable();
							MessageBox(_T("输入的用户名、密码不一致，请重新输入！"), _T("错误"), MB_OK | MB_ICONERROR);
							return;
						}
					}
				}
			}
			if (FALSE == bHasThisUser)
			{
				tmEmployee.CloseTable();
				MessageBox(_T("输入的用户名不存在，请重新输入！"), _T("错误"), MB_OK | MB_ICONERROR);
				return;
			}
		}while(!bIsLogin);

		tmEmployee.CloseTable();
	}
	else
	{
		OnCancel();
	}

	OnOK();
}
