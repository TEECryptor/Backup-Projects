//----------------------------------------------------------------
// Sun3D360 ToolDlg.cpp : implementation file
//
//----------------------------------------------------------------
#include "stdafx.h"
#include "Sun3D360 Tool.h"
#include "Sun3D360 ToolDlg.h"
//----------------------------------------------------------------
#include <math.h>
#include "JpgManager.h"
#include "zlib.h"
//----------------------------------------------------------------
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
//----------------------------------------------------------------
CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}
//----------------------------------------------------------------
void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}
//----------------------------------------------------------------
BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//----------------------------------------------------------------
CSun3D360ToolDlg::CSun3D360ToolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSun3D360ToolDlg::IDD, pParent)
{
	m_iDivType = DIS4_4;
	//{{AFX_DATA_INIT(CSun3D360ToolDlg)
	m_iTotalFileCnt = 0;
	m_strFileName = _T("");
	m_strFloder = _T("");
	m_iHeight = 1024;
	m_iWidth = 1024;
	m_strCurNum = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}
//----------------------------------------------------------------
void CSun3D360ToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSun3D360ToolDlg)
	DDX_Text(pDX, IDC_EDIT_CNT, m_iTotalFileCnt);
	DDX_Text(pDX, IDC_EDIT_FILENAME, m_strFileName);
	DDX_Text(pDX, IDC_EDIT_FLODER, m_strFloder);
	DDX_Text(pDX, IDC_EDIT_HEIGHT, m_iHeight);
	DDX_Text(pDX, IDC_EDIT_WIDTH, m_iWidth);
	DDX_Text(pDX, IDC_STATIC_NUM, m_strCurNum);
	//}}AFX_DATA_MAP
}
//----------------------------------------------------------------
BEGIN_MESSAGE_MAP(CSun3D360ToolDlg, CDialog)
	//{{AFX_MSG_MAP(CSun3D360ToolDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_FLODER, OnBtnFloder)
	ON_BN_CLICKED(IDC_BTN_SAVE, OnBtnSave)
	ON_BN_CLICKED(IDC_BTN_OK, OnBtnOk)
	ON_CBN_EDITCHANGE(IDC_COMBO, OnEditchangeCombo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//----------------------------------------------------------------
BOOL CSun3D360ToolDlg::OnInitDialog()
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

	GetModuleFileName(NULL, m_strExePath, MAX_PATH);
	char* p = m_strExePath;
	while(*p != '\0') p++;
	while(*p != '\\') {*p = '\0'; p--;}
	
	CComboBox* pBox = (CComboBox*)GetDlgItem(IDC_COMBO);
	pBox->SetCurSel(0);
	m_iFormat = 0;

	pBox = (CComboBox*)GetDlgItem(IDC_COMBO_DIS);
	pBox->SetCurSel(DIS4_4);
	m_iDivType = DIS4_4;	

	CProgressCtrl* pProgress = (CProgressCtrl*)GetDlgItem(IDC_PROGRESS);
	pProgress->SetRange(0, 100);
	pProgress->SetPos(0);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}
//----------------------------------------------------------------
void CSun3D360ToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//----------------------------------------------------------------
void CSun3D360ToolDlg::OnPaint() 
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
//----------------------------------------------------------------
HCURSOR CSun3D360ToolDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}
//----------------------------------------------------------------

//----------------------------------------------------------------
//	选择原图片总文件夹
//----------------------------------------------------------------
void CSun3D360ToolDlg::OnBtnFloder() 
{
	BROWSEINFO bBinfo;
	memset(&bBinfo,0,sizeof(BROWSEINFO));  
	bBinfo.hwndOwner = m_hWnd;

    bBinfo.lpszTitle = "请选择原图片的总路径：";
    bBinfo.ulFlags = BIF_RETURNONLYFSDIRS; 
	 
    LPITEMIDLIST lpDlist;
	lpDlist = SHBrowseForFolder(&bBinfo); 
	if(lpDlist != NULL)
	{
		char strTmp[255];
		SHGetPathFromIDList(lpDlist, strTmp);
		m_strFloder = strTmp;
		UpdateData(false);
	}
}
//----------------------------------------------------------------
//	指定生成文件的文件名
//----------------------------------------------------------------
void CSun3D360ToolDlg::OnBtnSave() 
{
	CFileDialog saveDialog(	FALSE,
							"VGS",
							NULL,
							OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
							"VGS文件(*.VGS)|*.VGS|",
							this);
	if(saveDialog.DoModal() == IDOK)
	{
		m_strFileName = saveDialog.GetPathName();
		UpdateData(false);
	}
	else
	{
		return;
	}		
}
//----------------------------------------------------------------
//	选择了纹理格式
//----------------------------------------------------------------
void CSun3D360ToolDlg::OnEditchangeCombo() 
{
	CComboBox* pBox = (CComboBox*)GetDlgItem(IDC_COMBO);
	m_iFormat = pBox->GetCurSel();	
}
//----------------------------------------------------------------
//	开始压缩
//----------------------------------------------------------------
void CSun3D360ToolDlg::OnBtnOk() 
{
	//检查数据是否正确
	UpdateData();
	if(m_iTotalFileCnt <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		MessageBox("文件总个数或图片大小指定错误!", "错误", MB_OK);
		return;
	}
	
	//The dest file name is NULL
	if(m_strFloder.IsEmpty() || m_strFileName.IsEmpty()) 
	{
		MessageBox("文件路径或目标文件指定为空!", "错误", MB_OK);
		return;
	}

	//Create the VGS file
	HANDLE hFile = CreateFile(m_strFileName, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile == NULL)
	{
		MessageBox("创建文件错误！");
		return;
	}

	//The file header
	DWORD dwWriteSize = 0;
	VGSFileHeader header;
	strcpy(header.strType, "JPG");
	header.iImageCnt = m_iTotalFileCnt;
	header.iImageWidth = m_iWidth;
	header.iImageHeight = m_iHeight;
	header.iDivType = m_iDivType;
	WriteFile(hFile, &header, sizeof(header), &dwWriteSize, NULL);

	//The small images BMP data buffer of the whole image
	int iSmallCnt = 0;
	int iSmallWidth = 0;
	int iSmallHeight = 0;
	int iSmallSize = 0;
	GetSmallImageInfo(iSmallCnt, iSmallWidth, iSmallHeight, iSmallSize);

	//The path string
	char path[255] = {0};
	memcpy(path, m_strFloder.GetBuffer(m_strFloder.GetLength()), m_strFloder.GetLength());
	m_strFloder.ReleaseBuffer();

	//The offset of every frame
	__int64 dwCurOffset = sizeof(header);
	LPFRAMEDATAOFFSET pOffset = new FrameDataOffset[m_iTotalFileCnt];
	memset(pOffset, 0, sizeof(FrameDataOffset)*m_iTotalFileCnt);

	//To process the images, division, write to file and get the size
	BYTE* pBmpData = NULL;
	SetDlgItemText(IDC_STATIC_PROSS, "正压缩文件");
	for(int iFrame = 0; iFrame < m_iTotalFileCnt; iFrame++)
	{
		pOffset[iFrame].bId = iFrame;
		pOffset[iFrame].bOffset = dwCurOffset;	
		
		//Write the frame id
		WriteFile(hFile, &iFrame, sizeof(int), &dwWriteSize, NULL);
		dwCurOffset += sizeof(int);
	
		//Decode one face image to BMP and write to VGS file
		for(int iFace = IMAGE_F;iFace < IMAGE_CNT;iFace++)
		{
			//Decode jpg to bmp
			if(DecodeOneFaceToBmp(iFrame, iFace, path, pBmpData))
			{
				int iImageTotalOffset = 0;
				int iOneSmallImageSize[MAX_DIV_CNT];
				BYTE* pSmallImage[MAX_DIV_CNT]; 
				for(int iSmall = 0;iSmall < iSmallCnt;iSmall++)
				{
					pSmallImage[iSmall] = NULL;
					pSmallImage[iSmall] = new BYTE[iSmallSize];

					//To get the small image BMP data
					GetSmallImageData(pBmpData, m_iWidth, m_iHeight, iSmall, iSmallWidth, iSmallHeight, pSmallImage[iSmall]);

					//Encode the small image data from BMP to JPG
					iOneSmallImageSize[iSmall] = 0;
					EncodeBmpToJpgBuff(pSmallImage[iSmall], iSmallWidth, iSmallHeight, iOneSmallImageSize[iSmall]);

					iImageTotalOffset += iOneSmallImageSize[iSmall];
				}					
			
				//Write the total image size of this face
				iImageTotalOffset += iSmallCnt * sizeof(int);
				WriteFile(hFile, &iImageTotalOffset, sizeof(int), &dwWriteSize, NULL);
				dwCurOffset += sizeof(int);
					
				//Write the small image jpg data to VGS file
				for(iSmall = 0;iSmall < iSmallCnt;iSmall++)
				{
					//The size
					WriteFile(hFile, &iOneSmallImageSize[iSmall], sizeof(int), &dwWriteSize, NULL);
					dwCurOffset += sizeof(int);

					//The data
					WriteFile(hFile, pSmallImage[iSmall], iOneSmallImageSize[iSmall], &dwWriteSize, NULL);
					dwCurOffset += iOneSmallImageSize[iSmall];

					delete []pSmallImage[iSmall];
					pSmallImage[iSmall] = NULL;
				}
			}
		}
		
		ShowProgressInfo(m_iTotalFileCnt, iFrame+1);
	}
	if(pBmpData)
	{
		delete []pBmpData;
		pBmpData = NULL;
	}

	//To write the offset list at the file end
	WriteFile(hFile, pOffset, sizeof(FrameDataOffset)*m_iTotalFileCnt, &dwWriteSize, NULL);

	CloseHandle(hFile);

	MessageBox("图片压缩完毕!", "完成", MB_OK);
	ShowProgressInfo(m_iTotalFileCnt, 0);
}
//----------------------------------------------------------------


//----------------------------------------------------------------
//	To get the small images size and width, height
//----------------------------------------------------------------
void CSun3D360ToolDlg::GetSmallImageInfo(int &iCnt, int &width, int &height, int &size)
{
	switch(m_iDivType)
	{
	case DIS1_1:
		width = m_iWidth;
		height = m_iHeight;
		iCnt = 1;
		break;
	case DIS2_2:
		width = m_iWidth/2;
		height = m_iHeight/2;
		iCnt = 4;
		break;
	case DIS4_4:
		width = m_iWidth/4;
		height = m_iHeight/4;
		iCnt = 16;
		break;
	case DIS8_8:
		width = m_iWidth/8;
		height = m_iHeight/8;
		iCnt = 64;
		break;
	default:
		width = 0;
		height = 0;
		iCnt = 0;
		break;
	}
	size = 3 * width * height;
}
//----------------------------------------------------------------
//	To decode one face image to BMP
//----------------------------------------------------------------
bool CSun3D360ToolDlg::DecodeOneFaceToBmp(int iFrame, int iFace, const char* path, BYTE*& pBMPData)
{
	if(iFrame < 0 || iFrame > m_iTotalFileCnt) return false;
	if(iFace < IMAGE_F || iFace >= IMAGE_CNT) return false;
	if(path == NULL) return false;

	//The file name
	char file[255] = {0};
	strcpy(file, path);

	switch(iFace)
	{
	case IMAGE_F:
		strcat(file, "/cam_F/cam_F");
		break;
	case IMAGE_B:
		strcat(file, "/cam_B/cam_B");
		break;
	case IMAGE_L:
		strcat(file, "/cam_L/cam_L");
		break;
	case IMAGE_R:
		strcat(file, "/cam_R/cam_R");
		break;
	case IMAGE_T:
		strcat(file, "/cam_T/cam_T");
		break;
	case IMAGE_U:
		strcat(file, "/cam_U/cam_U");
		break;
	}

	char tmp[10] = {0};
	GetCharFormInt(iFrame, tmp);
	strcat(file, tmp);
	strcat(file, ".jpg");

	//Decode the JPG file to BMP buffer
	CJpgManager jpg;
	if(!jpg.jpgOpenFile(file))
	{
		return false;
	}
	if(m_iWidth != jpg.jpgGetImageParams(_JPGIMAGEWIDTH))
	{
		return false;
	}
	if(m_iHeight != jpg.jpgGetImageParams(_JPGIMAGEHEIGHT))
	{
		return false;
	}
	long lSize = 3 * m_iWidth * m_iHeight;
	if(lSize != jpg.jpgGetImageParams(_JPGIMAGESIZE))
	{
		return false;
	}

	//The BMP data buffer
	if(pBMPData) delete[]pBMPData;
	pBMPData = new BYTE[lSize];
	BYTE* pTmpData = jpg.jpgGetImageData();
	memcpy(pBMPData, pTmpData, m_iWidth*m_iHeight*3);
/*
	for(int i = 0;i < m_iHeight;i++)
		memcpy(&pBMPData[i*m_iWidth*3], &pTmpData[(m_iHeight-i-1)*m_iWidth*3], m_iWidth*3);
*/
	return true;
}
//----------------------------------------------------------------
//	To get the small image BMP data
//----------------------------------------------------------------
void CSun3D360ToolDlg::GetSmallImageData(BYTE* pBmpData, int iBigWidth, int iBigHeight, 
			int iID, int iSmallWidth, int iSmallHeight, BYTE*& pSmallData)
{
	if(pBmpData == NULL) return;

	int iPos = 0;
	for(int i = 0;i < iSmallHeight;i++)
	{
		iPos = (iID/((int)pow(2,m_iDivType))) * iBigWidth * iSmallHeight * 3;
		iPos += (i*iBigWidth*3 + (iID%((int)pow(2,m_iDivType))) * iSmallWidth * 3);

		memcpy(&pSmallData[i * iSmallWidth * 3], &pBmpData[iPos], iSmallWidth*3);
	}

}
//----------------------------------------------------------------	
//	Encode the small image data from BMP to JPG
//----------------------------------------------------------------	
void CSun3D360ToolDlg::EncodeBmpToJpgBuff(BYTE*& pImage, int iWidth, int iHeight, int &iSize)
{
	if(pImage == NULL) return;

	CJpgManager jpg;
	jpg.jpgSetImageParams(_JPGIMAGEWIDTH, iWidth);
	jpg.jpgSetImageParams(_JPGIMAGEHEIGHT, iHeight);
	jpg.jpgSetImageParams(_JPGIMAGECHANNELS, 3);

	if(jpg.jpgSaveBuff(pImage, iSize))
	{
		iSize = jpg.jpgGetImageParams(_JPGIMAGESIZE);

		if(pImage) delete []pImage;
		pImage =  new BYTE[iSize];
		memcpy(pImage, jpg.jpgGetImageData(), iSize);

		/*
		if(pImage == NULL) pImage = new BYTE[iSize];
		memcpy(pImage, jpg.jpgGetImageData(), iSize);
		*/
	}
	else
	{
		iSize = 0;
	}
}
//----------------------------------------------------------------	


//----------------------------------------------------------------
//	将图片id转换为字符串
//----------------------------------------------------------------
void CSun3D360ToolDlg::GetCharFormInt(int num, char* pstring)
{	
	if(num < 0  || !pstring) return;

	if(num < 10)
	{
		char tmp[10] = {0};
		itoa(num, tmp, 10);
		strcpy(pstring, "000");
		strcat(pstring, tmp);
	}
	else if(num < 100)
	{
		char tmp[10] = {0};
		itoa(num, tmp, 10);
		strcpy(pstring, "00");
		strcat(pstring, tmp);
	}
	else if(num < 1000)
	{
		char tmp[10] = {0};
		itoa(num, tmp, 10);
		strcpy(pstring, "0");
		strcat(pstring, tmp);
	}
	else if(num < 10000)
	{
		char tmp[10] = {0};
		itoa(num, tmp, 10);
		strcat(pstring, tmp);
	}
	else
	{
		pstring = "";
	}
}
//----------------------------------------------------------------


//----------------------------------------------------------------
//	显示进度
//----------------------------------------------------------------
void CSun3D360ToolDlg::ShowProgressInfo(int iTotal, int iCurCnt)
{
	float tmp = 100.0f * iCurCnt/iTotal;

	CProgressCtrl* pProgress = (CProgressCtrl*)GetDlgItem(IDC_PROGRESS);
	pProgress->SetPos((int)tmp);

	CString strNum;
	strNum.Format("%.2f%c", tmp, '%');
	SetDlgItemText(IDC_STATIC_NUM, strNum);
}
//----------------------------------------------------------------
