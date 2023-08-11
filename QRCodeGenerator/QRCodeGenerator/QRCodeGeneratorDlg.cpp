// QRCodeGeneratorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "QRCodeGenerator.h"
#include "QRCodeGeneratorDlg.h"
#include "QR_Encode.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#define IMAGE_WIDTH		480
#define LOGO_WIDTH		128
#define QR_MODE_SIZE	8

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


// CQRCodeGeneratorDlg dialog




CQRCodeGeneratorDlg::CQRCodeGeneratorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CQRCodeGeneratorDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pLogoImage = NULL;
	m_pQRImage = NULL;
}

void CQRCodeGeneratorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CQRCodeGeneratorDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_OPENLOGO, &CQRCodeGeneratorDlg::OnBnClickedBtnOpenlogo)
	ON_BN_CLICKED(IDC_BTN_GENCODE, &CQRCodeGeneratorDlg::OnBnClickedBtnGencode)
	ON_BN_CLICKED(IDC_BTN_SAVETOFILE, &CQRCodeGeneratorDlg::OnBnClickedBtnSavetofile)
END_MESSAGE_MAP()


// CQRCodeGeneratorDlg message handlers

BOOL CQRCodeGeneratorDlg::OnInitDialog()
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


	m_pLogoImage = new CImage();
	m_pLogoImage->Attach(LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_BITMAP_LOGO)));

	SetDlgItemText(IDC_EDIT_CONTENT, _T(""));
	GetDlgItem(IDC_EDIT_CONTENT)->SetFocus();

	CRect rcQRCode;
	CStatic* pQRStatic = (CStatic*)GetDlgItem(IDC_STATIC_QRCODE);
	pQRStatic->GetWindowRect(&rcQRCode);
	ScreenToClient(&rcQRCode);
	rcQRCode.right = rcQRCode.left + 160;
	rcQRCode.bottom = rcQRCode.top + 160;
	pQRStatic->MoveWindow(&rcQRCode);


	return FALSE;  // return TRUE  unless you set the focus to a control
}

void CQRCodeGeneratorDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CQRCodeGeneratorDlg::OnPaint()
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
		if (!m_pQRImage)
		{
			CDialog::OnPaint();
		}
		else
		{		
			CPaintDC dc(this);
			CRect rcQRCode;
			CStatic* pQRStatic = (CStatic*)GetDlgItem(IDC_STATIC_QRCODE);
			pQRStatic->GetWindowRect(&rcQRCode);
			ScreenToClient(&rcQRCode);
			m_pQRImage->Draw(dc.m_hDC, rcQRCode.left, rcQRCode.top, rcQRCode.Width(), rcQRCode.Height(),0, 0, IMAGE_WIDTH, IMAGE_WIDTH);
		}
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CQRCodeGeneratorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CQRCodeGeneratorDlg::OnBnClickedBtnOpenlogo()
{
	CString strLogoFile;
	CFileDialog dlgOpen(TRUE, "", "", OFN_CREATEPROMPT|OFN_PATHMUSTEXIST , "位图文件 (*.bmp)|*.bmp|PNG文件 (*.png)|*.png|All files (*.*)|*.*||");

	if (dlgOpen.DoModal() == IDOK)
	{
		strLogoFile = dlgOpen.GetPathName();
	}
	if (strLogoFile.IsEmpty()) return;

	if (m_pLogoImage)
	{
		m_pLogoImage->Destroy();
		delete m_pLogoImage;
	}
	m_pLogoImage = new CImage();
	m_pLogoImage->Load(strLogoFile);


}

void CQRCodeGeneratorDlg::OnBnClickedBtnGencode()
{
	CString strContent;
	CQR_Encode encoder;
	int iQRCodeWidth = 4*QR_MODE_SIZE + 17;
	int iWidth = (iQRCodeWidth/4 +1) * 4;
	int iHeight = iWidth;

	GetDlgItemText(IDC_EDIT_CONTENT, strContent);
	if (strContent.IsEmpty())
	{
		MessageBox(_T("请输入二维码内容"), _T("提示"), MB_ICONHAND);
		GetDlgItem(IDC_EDIT_CONTENT)->SetFocus();
		return;
	}

	// 二维码编码
	if (!encoder.EncodeData(3, QR_MODE_SIZE, FALSE, 0, strContent))
	{
		MessageBox(_T("生成二维码内容失败！"), _T("提示"), MB_ICONHAND);
		return;
	}

	// 将二维码数据转化为bitmap数据
	BYTE *pbtPixel = new BYTE[4*iWidth*iHeight];
	int iStart = 0 + (iWidth - iQRCodeWidth) / 2;
	int iEnd = iStart + iQRCodeWidth;
	memset(pbtPixel, 255, 4*iWidth*iHeight);
	for (int j = 0; j < iQRCodeWidth; j++)
	{
		for (int i = 0; i < iQRCodeWidth; i++)
		{
			if (encoder.m_byModuleData[i][j] == 0)
			{
				pbtPixel[4*((j+iStart)*iWidth + (i+iStart)) + 0] = 255;
				pbtPixel[4*((j+iStart)*iWidth + (i+iStart)) + 1] = 255;
				pbtPixel[4*((j+iStart)*iWidth + (i+iStart)) + 2] = 255;
			}
			else
			{
				pbtPixel[4*((j+iStart)*iWidth + (i+iStart)) + 0] = 0;
				pbtPixel[4*((j+iStart)*iWidth + (i+iStart)) + 1] = 0;
				pbtPixel[4*((j+iStart)*iWidth + (i+iStart)) + 2] = 0;
			}
			pbtPixel[4*((j+iStart)*iWidth + (i+iStart)) + 3] = 0;
		}
	}

	// 创建二维码图片
	CBitmap* pCodeImage = new CBitmap();
	if (!pCodeImage->CreateBitmap(iWidth, iHeight, 1, 32, pbtPixel))
	{
		delete []pbtPixel;
		MessageBox(_T("生成二维码图片失败！"), _T("提示"), MB_ICONHAND);
		return;
	}
	delete []pbtPixel;
	pbtPixel = NULL;
	

	// 创建画布图片
	if (m_pQRImage) 
	{
		m_pQRImage->Destroy();
		delete m_pQRImage;
	}
	m_pQRImage = new CImage();
	m_pQRImage->Create(IMAGE_WIDTH, IMAGE_WIDTH, 32);

	// 将二维码图片贴至画布
	HDC dc = m_pQRImage->GetDC();
	HDC hDCMem = CreateCompatibleDC(dc);
	int iOldDcBkMode = SetBkMode(dc, TRANSPARENT);
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(hDCMem, (HBITMAP)pCodeImage->m_hObject);		
	HBRUSH hRedBrush = CreateSolidBrush(RGB(255, 255, 255));

	//SelectObject(dc, hRedBrush);
	FillRect(dc, CRect(0, 0, IMAGE_WIDTH, IMAGE_WIDTH), hRedBrush);
	StretchBlt(dc, 10, 10, (IMAGE_WIDTH-20), (IMAGE_WIDTH-20), hDCMem, iStart, iStart, iQRCodeWidth, iQRCodeWidth, SRCCOPY);			
	
	// 将logo图片贴至画布
	//if (m_pLogoImage)
	//{
	//	m_pLogoImage->Draw(dc, (IMAGE_WIDTH-LOGO_WIDTH) / 2, (IMAGE_WIDTH-LOGO_WIDTH) / 2, LOGO_WIDTH, LOGO_WIDTH, 0, 0, m_pLogoImage->GetWidth(), m_pLogoImage->GetHeight());
	//}

	SelectObject(hDCMem, hOldBitmap);
	SetBkMode(hDCMem, iOldDcBkMode);
	DeleteDC(hDCMem);
	DeleteObject(hRedBrush);
	m_pQRImage->ReleaseDC();
	delete pCodeImage;

	Invalidate();
	UpdateWindow();
}

void CQRCodeGeneratorDlg::OnBnClickedBtnSavetofile()
{
	CString strSaveFile;
	CFileDialog dlgSave(FALSE, ".bmp", "", OFN_CREATEPROMPT|OFN_PATHMUSTEXIST , "位图文件 (*.bmp)|*.bmp|All files (*.*)|*.*||");

	if (!m_pQRImage)
	{
		MessageBox(_T("请先生成二维码图片！"), _T("提示"), MB_ICONHAND);
		return;
	}

	if (dlgSave.DoModal() == IDOK)
	{
		strSaveFile = dlgSave.GetPathName();
	}
	if (strSaveFile.IsEmpty()) return;
	

	m_pQRImage->Save(strSaveFile);
}
