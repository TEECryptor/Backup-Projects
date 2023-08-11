// TestImageButtonDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TestImageButton.h"
#include "TestImageButtonDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//--------------------------------------------------------
// CAboutDlg dialog used for App About
//--------------------------------------------------------
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
//--------------------------------------------------------
CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}
//--------------------------------------------------------
void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}
//--------------------------------------------------------
BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//--------------------------------------------------------
// CTestImageButtonDlg dialog
//--------------------------------------------------------
CTestImageButtonDlg::CTestImageButtonDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestImageButtonDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTestImageButtonDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}
//--------------------------------------------------------
void CTestImageButtonDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTestImageButtonDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}
//--------------------------------------------------------
BEGIN_MESSAGE_MAP(CTestImageButtonDlg, CDialog)
	//{{AFX_MSG_MAP(CTestImageButtonDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//--------------------------------------------------------
// CTestImageButtonDlg message handlers
//--------------------------------------------------------
BOOL CTestImageButtonDlg::OnInitDialog()
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
	
/*	// TODO: Add extra initialization here
	RECT rc;
	rc.left = 10;
	rc.right = 110;
	rc.top = 10;
	rc.bottom = 60;
	ButtonParam bp;
	bp.uID = 1;
	bp.fValue = 0.5f;
	CImageButton* pBtn = new CImageButton(m_hWnd, rc, bp);
	pBtn->Create(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_BITMAP_OUT), MAKEINTRESOURCE(IDB_BITMAP_IN));
	m_lstBtn.push_back(pBtn);
	
	rc.left = 120;
	rc.right = 220;
	rc.top = 10;
	rc.bottom = 60;
	bp.uID = 2;
	bp.fValue = 0.0f;
	pBtn = new CImageButton(m_hWnd, rc, bp);
	pBtn->Create(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_BITMAP_OUT), MAKEINTRESOURCE(IDB_BITMAP_IN));
	m_lstBtn.push_back(pBtn);
*/
	
	char strPath[MAX_PATH];
	GetModuleFileName(NULL, strPath, MAX_PATH);
	CString strExePath = strPath;
	strExePath = strExePath.Left(strExePath.ReverseFind('\\') + 1);
	
	CString strIniFile = strExePath + "Buttons.ini";
	LoadButtonIniFile(strIniFile.GetBuffer(strIniFile.GetLength()));

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTestImageButtonDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//--------------------------------------------------------
// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.
//--------------------------------------------------------
void CTestImageButtonDlg::OnPaint() 
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
		HDC hdc = ::GetDC(m_hWnd);
		for(int i = 0;i < m_lstBtn.size();i++)
		{
			m_lstBtn[i]->DrawButton(hdc);
		}	

		CDialog::OnPaint();
	}
}
//--------------------------------------------------------
// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
//--------------------------------------------------------
HCURSOR CTestImageButtonDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}
//--------------------------------------------------------
void CTestImageButtonDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	POINT pnt;
	pnt.x = point.x;
	pnt.y = point.y;

	for(int i = 0;i < m_lstBtn.size();i++)
	{
		if(m_lstBtn[i]->PntInButton(pnt))
		{
			if(m_lstBtn[i]->GetButtonState() != BS_IN)
			{
				m_lstBtn[i]->SetButtonState(BS_IN);
				//InvalidateRect(&m_lstBtn[i]->GetButtonRect());
				//InvalidateRect(NULL);
				Invalidate(false);
			}
		}
		else
		{
			if(m_lstBtn[i]->GetButtonState() != BS_OUT)
			{
				m_lstBtn[i]->SetButtonState(BS_OUT);
				//InvalidateRect(&m_lstBtn[i]->GetButtonRect());
				Invalidate(false);
			}
		}
	}

	CDialog::OnMouseMove(nFlags, point);
}
//--------------------------------------------------------
void CTestImageButtonDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	POINT pnt;
	pnt.x = point.x;
	pnt.y = point.y;

	ButtonParam bp;
	BOOL bClickButton = false;
	for(int i = 0;i < m_lstBtn.size();i++)
	{
		if(m_lstBtn[i]->PntInButton(pnt))
		{
			m_lstBtn[i]->GetButtonParam(bp);
			bClickButton = true;
			break;
		}
	}
	if(bClickButton)
	{
		char tmp[256] = {0};
		switch(bp.uID)
		{
		case 1:
			sprintf(tmp, "Button click id is %d, The value is %.2f", bp.uID, bp.fValue);
			MessageBox(tmp);
			break;
		case 2:
			sprintf(tmp, "Button click id is %d, The value is %.2f", bp.uID, bp.fValue);
			MessageBox(tmp);
			break;
		default:
			break;
		}
	}
	
	CDialog::OnLButtonDown(nFlags, point);
}
//--------------------------------------------------------
//	To load the button ini file
//--------------------------------------------------------
void CTestImageButtonDlg::LoadButtonIniFile(char* file)
{
	//int iCnt = GetPrivateProfileInt("Button", "Count", 0, file);
	int iCnt = MyGetPrivateProfileInt("Button", "Count", 0, file);

	for(int i = 0;i < iCnt;i++)
	{
		char strID[32] = {0};
		sprintf(strID, "ID%d", i);

		char strValue[32] = {0};
		sprintf(strValue, "Value%d", i);

		char strLeft[32] = {0};
		sprintf(strLeft, "Left%d", i);

		char strTop[32] = {0};
		sprintf(strTop, "Top%d", i);

		char strRight[32] = {0};
		sprintf(strRight, "Right%d", i);

		char strBottom[32] = {0};
		sprintf(strBottom, "Bottom%d", i);

		char strImageOut[32] = {0};
		sprintf(strImageOut, "ImageOut%d", i);

		char strImageIn[32] = {0};
		sprintf(strImageIn, "ImageIn%d", i);

		char tmp[MAX_PATH] = {0};

		//The button click param
		ButtonParam bp;
		bp.uID = MyGetPrivateProfileInt("Button", strID, 0, file);
		
		CString strTmp;
		strTmp = MyGetPrivateProfileString("Button", strValue, "", file);
		bp.fValue = atof(strTmp.GetBuffer(strTmp.GetLength()));

		//The button rect
		RECT rc;
		rc.left = MyGetPrivateProfileInt("Button", strLeft, 0, file);
		rc.top = MyGetPrivateProfileInt("Button", strTop, 0, file);
		rc.right = MyGetPrivateProfileInt("Button", strRight, 0, file);
		rc.bottom = MyGetPrivateProfileInt("Button", strBottom, 0, file);

		//The button image files name
		char strOut[MAX_PATH] = {0};
		strTmp = MyGetPrivateProfileString("Button", strImageOut, "", file);
		strcpy(strOut, strTmp.GetBuffer(strTmp.GetLength()));

		char strIn[MAX_PATH] = {0};
		strTmp = MyGetPrivateProfileString("Button", strImageIn, "", file);
		strcpy(strIn, strTmp.GetBuffer(strTmp.GetLength()));

		//To create the button
		CImageButton* pBtn = new CImageButton(m_hWnd, rc, bp);
		//pBtn->Create(AfxGetApp()->m_hInstance, strOut, strIn);
		pBtn->Create(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_BITMAP_OUT), MAKEINTRESOURCE(IDB_BITMAP_IN));

		m_lstBtn.push_back(pBtn);
	}
}
//--------------------------------------------------------
//	To load the button text file
//--------------------------------------------------------
void CTestImageButtonDlg::LoadButtonTxtFile(char* file)
{
}
//--------------------------------------------------------
//Get the string value from the ini file
//--------------------------------------------------------
CString	CTestImageButtonDlg::MyGetPrivateProfileString(const CString strSection, const CString strEntry, const CString strDefault, const CString strIniFile)
{
	//At first, cheek the section and entry
	if(strSection == "" || strEntry == "")
	{
		return strDefault;   
	}

	CFile    IniFile;
	CString  strCombine;
	TRY
	{
		//Open the ini file   
		CFileException ex;
		if(!IniFile.Open(strIniFile, CFile::modeRead, &ex))
		{     
			TCHAR szError[1024];
			ex.GetErrorMessage(szError, 1024);

			return strDefault;
		}

		//If the ini file is empty
		if(IniFile.GetLength() == 0)
		{
			IniFile.Close();
			return strDefault;
		}

		//Read the all data to buffer
		char  *pBuf;
		pBuf = new char[IniFile.GetLength() + 1];  
		if(pBuf == NULL)
		{
			IniFile.Close();
			return strDefault;
		}
		if(IniFile.Read(pBuf, IniFile.GetLength()) != IniFile.GetLength())
		{
			delete[]  pBuf;
			IniFile.Close();
			return strDefault;
		}

		//pBuf[IniFile.GetLength() / 2] = NULL;
		strCombine.GetBuffer(MAX_LEN);  
		strCombine = pBuf;
		delete[]   pBuf;

		//Begin look up
		int iIndex1, iIndex2, iIndex3, iIndexT;    
		iIndex1 = strCombine.Find("[" + strSection + "]\r\n");
		//The section is not in the ini file
		if(iIndex1 == -1)  
		{
			IniFile.Close();
			return strDefault;
		}

		iIndexT = iIndex1 + 4 + strSection.GetLength();
		iIndex2 = strCombine.Find(strEntry + "=", iIndexT);
		//The entry is not in the ini file
		if(iIndex2 == -1)  
		{
			IniFile.Close();
			return strDefault;
		}
		else
		{
			iIndex3 = strCombine.Find("\r\n", iIndex2 + 1);
			if(iIndex3 == -1)
			{
				IniFile.Close();
				return strDefault;
			}
			iIndexT = iIndex2 + 1 + strEntry.GetLength();
			IniFile.Close();
			return  strCombine.Mid(iIndexT, iIndex3 - iIndexT);
		}
	}
	CATCH(CFileException, e)
	{		
	}
	END_CATCH     
	
	IniFile.Close();  
	return strDefault;
}
//--------------------------------------------------------
//Get the int value from the ini file
//--------------------------------------------------------
int	CTestImageButtonDlg::MyGetPrivateProfileInt(const CString strSection, const CString strEntry, const int iDefault, const CString strIniFile)
{
	CString strValue = MyGetPrivateProfileString(strSection, strEntry, "", strIniFile);
	
	if(strValue == "") 
		return iDefault;
	else
		return atoi( (const char*)strValue.GetBuffer(strValue.GetLength()));
}
//--------------------------------------------------------

