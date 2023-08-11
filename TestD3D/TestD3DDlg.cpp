// TestD3DDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TestD3D.h"
#include "TestD3DDlg.h"
#include ".\testd3ddlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//---------------------------------------------
const TCHAR g_szRenderWndClassName[]	= {_T("D3DRenderWnd")};
//---------------------------------------------
LRESULT CALLBACK RenderWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
//---------------------------------------------
// CAboutDlg dialog used for App About
//---------------------------------------------
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
//---------------------------------------------
CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}
//---------------------------------------------
void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}
//---------------------------------------------
BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//---------------------------------------------
// CTestD3DDlg dialog
//---------------------------------------------
CTestD3DDlg::CTestD3DDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestD3DDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTestD3DDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_hRenderWnd = NULL;
	m_pD3DEngine = NULL;
	m_pD3D = NULL;
	m_pd3dDevice = NULL;
	//
	m_fCameraX = 0.0f;
	m_fCameraY = 0.0f;
	m_fCameraZ = 0.0f;
}
//---------------------------------------------
void CTestD3DDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTestD3DDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}
//---------------------------------------------
BEGIN_MESSAGE_MAP(CTestD3DDlg, CDialog)
	//{{AFX_MSG_MAP(CTestD3DDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
END_MESSAGE_MAP()
//---------------------------------------------
// CTestD3DDlg message handlers
//---------------------------------------------
BOOL CTestD3DDlg::OnInitDialog()
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
	SetWindowPos(NULL, 0, 0, 800, 680, SWP_SHOWWINDOW);
	CenterWindow(NULL);

	m_hRenderWnd = CreateRenderWnd(0, 0, 800, 600);
	if(InitD3D(m_hRenderWnd) == S_OK)
	{
		SetTimer(0, 33, NULL);
	}
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}
//---------------------------------------------
void CTestD3DDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//---------------------------------------------
// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.
//---------------------------------------------
void CTestD3DDlg::OnPaint() 
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
//---------------------------------------------
// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTestD3DDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}
//---------------------------------------------
BOOL CTestD3DDlg::PreTranslateMessage(MSG* pMsg)
{
	switch(pMsg->message)
	{
	case WM_KEYDOWN:
		{
			if(pMsg->wParam == VK_ESCAPE && NULL != m_pD3DEngine)
				m_pD3DEngine->SetFullScreen(0, 0, m_hRenderWnd, FALSE);
		}
		return 0;
	default:
		break;
	}
	return CDialog::PreTranslateMessage(pMsg);
}
//---------------------------------------------
void CTestD3DDlg::OnDestroy()
{
	CDialog::OnDestroy();

	if(NULL != m_pD3DEngine)
	{
		delete m_pD3DEngine;
		m_pD3DEngine = NULL;
	}

	if(NULL != m_hRenderWnd)
	{
		::DestroyWindow(m_hRenderWnd);
		m_hRenderWnd = NULL;
	}
}
//---------------------------------------------




//---------------------------------------------
HWND CTestD3DDlg::CreateRenderWnd(int iLeft, int iTop, int iWidth, int iHeight)
{
	WNDCLASS wc;
	wc.style         = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = (WNDPROC)RenderWndProc; 
	wc.cbClsExtra    = NULL;
	wc.cbWndExtra    = NULL;
	wc.hInstance     = AfxGetApp()->m_hInstance;
	wc.hIcon         = NULL;
	wc.hCursor       = NULL;
	wc.hbrBackground = (HBRUSH)COLOR_HIGHLIGHTTEXT;//NULL;
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = g_szRenderWndClassName;
	if( !RegisterClass(&wc) ) 
	{
		::MessageBox(0, "RegisterClass() - FAILED", 0, 0);
		return NULL;
	}

	HWND hRenderWnd = ::CreateWindow(	g_szRenderWndClassName, g_szRenderWndClassName, 
										WS_CHILD | WS_VISIBLE,
										iLeft, iTop, iWidth, iHeight,
										m_hWnd, NULL /* menu */, NULL, NULL /*extra*/); 	
	return hRenderWnd;
}
//---------------------------------------------
HRESULT CTestD3DDlg::InitD3D(HWND hRenderWnd)
{
	if(NULL != m_pD3DEngine)
	{
		delete m_pD3DEngine;
		m_pD3DEngine = NULL;
	}

	m_pD3DEngine = new CD3DEngine();
	if(FALSE == m_pD3DEngine->CreateD3DDevice(40, 30, hRenderWnd, FALSE))
		return S_FALSE;

//	if(FALSE == m_pD3DEngine->CreateD3DDevice(1024, 768, m_hWnd, TRUE))
//		return S_FALSE;

	return S_OK;
}
//---------------------------------------------
VOID CTestD3DDlg::Render()
{
	if( NULL == m_pD3DEngine )
		return;

	m_pD3DEngine->Render();
}
//---------------------------------------------
void CTestD3DDlg::OnTimer(UINT nIDEvent) 
{
	Render();
	
	CDialog::OnTimer(nIDEvent);
}
//---------------------------------------------
void CTestD3DDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
/*
	if( NULL != m_pD3DEngine )
		m_pD3DEngine->SetViewport(10, 10, cx-10, cy-10, 0.0f, 1.0f);
*/	
}
//---------------------------------------------
HRESULT	CTestD3DDlg::SetProjectType(UINT uType, float fWidth, float fHeight)
{
	//To set the world matrix
    D3DXMATRIXA16 matWorld;
    D3DXMatrixIdentity( &matWorld );
    m_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );

	//For perspective
	if(uType == 0)
	{	
		//Look at
		D3DXVECTOR3 vEyePt( m_fCameraX, m_fCameraY,m_fCameraZ );
		D3DXVECTOR3 vLookatPt( m_fCameraX,m_fCameraY, 0.0f );
		D3DXVECTOR3 vUpVec( 0.0f, 1.0f, 0.0f );
		D3DXMATRIXA16 matView;
		D3DXMatrixLookAtLH( &matView, &vEyePt, &vLookatPt, &vUpVec );
		m_pd3dDevice->SetTransform( D3DTS_VIEW, &matView );

		//To set the project matrix
		D3DXMATRIXA16 matProj;
		D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI/4, 1.0f, 1.0f, 100.0f );
		m_pd3dDevice->SetTransform( D3DTS_PROJECTION, &matProj);
	}
	//For Ortho
	else
	{
		D3DXMATRIXA16 matView;
		D3DXMatrixIdentity( &matView );
		m_pd3dDevice->SetTransform( D3DTS_VIEW, &matView );
		//
		//To set the project matrix
		D3DXMATRIXA16 matProj;
		D3DXMatrixOrthoLH(&matProj, fWidth, fHeight, 0, 1.0f);
		m_pd3dDevice->SetTransform( D3DTS_PROJECTION, &matProj);
	}

	return S_OK;
}
//-----------------------------------------	
HRESULT	CTestD3DDlg::MoveCamera(float x, float y, float z)
{
	m_fCameraX += x;
	m_fCameraY += y;
	m_fCameraZ += z;

	return S_OK;
}
//-----------------------------------------	
VOID CTestD3DDlg::CalcCameraPos()
{
	BYTE btState[256] = {0};
	GetKeyboardState(btState);

	if((btState['A'] & 0x80) || (btState['a'] & 0x80))
	{
		m_fCameraX -= 0.1f;
	}
	else if((btState['D'] & 0x80) || (btState['d'] & 0x80))
	{
		m_fCameraX += 0.1f;
	}

	if((btState['W'] & 0x80) || (btState['w'] & 0x80))
	{
		m_fCameraZ += 0.1f;
	}
	else if((btState['S'] & 0x80) || (btState['s'] & 0x80))
	{
		m_fCameraZ -= 0.1f;
	}

	if((btState['Z'] & 0x80) || (btState['z'] & 0x80))
	{
		m_fCameraY += 0.1f;
	}
	else if((btState['X'] & 0x80) || (btState['x'] & 0x80))
	{
		m_fCameraY -= 0.1f;
	}

}
//-----------------------------------------	

void CTestD3DDlg::OnBnClickedButton1()
{
	if(NULL != m_pD3DEngine)
		m_pD3DEngine->SetFullScreen(1024, 768, m_hWnd, TRUE);
}
//-----------------------------------------	
