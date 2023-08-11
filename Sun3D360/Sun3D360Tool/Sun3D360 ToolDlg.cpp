//----------------------------------------------------------------
// Sun3D360 ToolDlg.cpp : implementation file
//
//----------------------------------------------------------------
#include "stdafx.h"
#include "Sun3D360 Tool.h"
#include "Sun3D360 ToolDlg.h"
//----------------------------------------------------------------
#include "JpgManager.h"
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
	//{{AFX_DATA_INIT(CSun3D360ToolDlg)
	m_iTotalFileCnt = 0;
	m_strFileName = _T("");
	m_strFloder = _T("");
	m_iHeight = 512;
	m_iWidth = 512;
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
	
	CComboBox* pBox = (CComboBox*)GetDlgItem(IDC_COMBO);
	pBox->SetCurSel(0);
	m_iFormat = 0;

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

	//open the dest file
	fstream outf(m_strFileName.GetBuffer(m_strFileName.GetLength()), ios::out | ios::binary);
	if(outf.is_open() == 0) 	return;
	
	//write the file header
	WriteS3DFileHeader(outf);

	char path[255] = {0};
	memcpy(path, m_strFloder.GetBuffer(m_strFloder.GetLength()), m_strFloder.GetLength());
	m_strFloder.ReleaseBuffer();

	//read the files size, and write the offset to dest file
	SetDlgItemText(IDC_STATIC_PROSS, "正获取文件信息");
	DWORD dOffset = sizeof(VGSFileHeader) + m_iTotalFileCnt*sizeof(FrameDataOffset);
	LPFRAMEDATAOFFSET pOffset = new FrameDataOffset;
	for(int iCnt = 0;iCnt < m_iTotalFileCnt;iCnt++)
	{
		pOffset->bId = iCnt;

		//该帧数据的起始位置
		pOffset->bTotalOffset = dOffset;

		//“前面”图片文件数据的偏移量
		dOffset += sizeof(DWORD);
		pOffset->bOffset[0] = dOffset;
		//“后面”图片文件数据的偏移量
		dOffset += (sizeof(DWORD) + GetImageFileSize(iCnt, IMAGE_F, path));
		pOffset->bOffset[1] = dOffset;
		//“左面”图片文件数据的偏移量
		dOffset += (sizeof(DWORD) + GetImageFileSize(iCnt, IMAGE_B, path));
		pOffset->bOffset[2] = dOffset;
		//“右面”图片文件数据的偏移量
		dOffset += (sizeof(DWORD) + GetImageFileSize(iCnt, IMAGE_L, path));
		pOffset->bOffset[3] = dOffset;
		//“顶面”图片文件数据的偏移量
		dOffset += (sizeof(DWORD) + GetImageFileSize(iCnt, IMAGE_R, path));
		pOffset->bOffset[4] = dOffset;
		//“底面”图片文件数据的偏移量
		dOffset += (sizeof(DWORD) + GetImageFileSize(iCnt, IMAGE_T, path));
		pOffset->bOffset[5] = dOffset;
		//
		dOffset += (sizeof(DWORD) + GetImageFileSize(iCnt, IMAGE_U, path));

		//写入文件
		outf.write(reinterpret_cast<const char*>(pOffset), sizeof(FrameDataOffset));

		//显示进度
		ShowProgressInfo(m_iTotalFileCnt, iCnt+1);
	}
	delete pOffset;
	pOffset = NULL;

	//write image data to dest file
	SetDlgItemText(IDC_STATIC_PROSS, "正压缩文件");

	long lSize = 0;
	char* pData = NULL;
	for(iCnt = 0;iCnt < m_iTotalFileCnt;iCnt++)
	{
		//write the id
		outf.write(reinterpret_cast<const char*>(&iCnt), sizeof(int));
		for(int iFace = 0;iFace < IMAGE_CNT;iFace++)
		{
			//load file data
			if(LoadImageFileData(iCnt, iFace, path, pData, lSize))
			{
				//write to the data to dest file
				WriteToVGSFile(outf, lSize, pData);
			}
		}

		//显示进度
		ShowProgressInfo(m_iTotalFileCnt, iCnt+1);
	}

	outf.close();
	if(pData)
	{
		delete []pData;
		pData = NULL;
	}

	MessageBox("图片压缩完毕!", "完成", MB_OK);
	ShowProgressInfo(m_iTotalFileCnt, 0);
}
//----------------------------------------------------------------


//----------------------------------------------------------------
//	写S3D文件头
//----------------------------------------------------------------
BOOL CSun3D360ToolDlg::WriteS3DFileHeader(fstream &out)
{
	if(!out.is_open()) return false;

	VGSFileHeader header;
	header.iImageCnt = m_iTotalFileCnt;
	header.iImageWidth = m_iWidth;
	header.iImageHeight = m_iHeight;
	
	out.write(reinterpret_cast<const char*>(&header), sizeof(VGSFileHeader));
	return true;
}
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
	int tmp = (int)(100.0f * iCurCnt/iTotal);

	CProgressCtrl* pProgress = (CProgressCtrl*)GetDlgItem(IDC_PROGRESS);
	pProgress->SetPos(tmp);

	CString strNum;
	strNum.Format("%d%c", tmp, '%');
	SetDlgItemText(IDC_STATIC_NUM, strNum);
}
//----------------------------------------------------------------
//	To get the file size 
//----------------------------------------------------------------
int CSun3D360ToolDlg::GetImageFileSize(int id, int face, const char* path)
{
	char file[255] = {0};
	strcpy(file, path);

	switch(face)
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
	GetCharFormInt(id, tmp);
	strcat(file, tmp);
	strcat(file, ".jpg");
	
	//open the file
	fstream inf(file, ios::in | ios::binary);
	if(inf.is_open() == 0) 	return 0;

	//get the size
	int iSize = 0;
	inf.seekg(0, ios::beg);
	inf.seekg(0, ios::end);
	iSize = inf.tellg();
	inf.close();	

	return iSize;
}
//----------------------------------------------------------------
//To get the file data
//----------------------------------------------------------------
bool CSun3D360ToolDlg::LoadImageFileData(int id, int face, const char* path, char*& data, long &cnt)
{
	//The file name
	char file[255] = {0};
	strcpy(file, path);

	switch(face)
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
	GetCharFormInt(id, tmp);
	strcat(file, tmp);
	strcat(file, ".jpg");
	
	//open the file
	fstream inf(file, ios::in | ios::binary);
	if(inf.is_open() == 0) 	return false;

	//get the file size
	inf.seekg(0, ios::beg);
	inf.seekg(0, ios::end);
	cnt = inf.tellg();
	inf.seekg(0, ios::beg);	

	//get the file data
	if(data) delete[] data;
	data = new char[cnt];
	inf.read(data, cnt);

	inf.close();
	return true;
}
//----------------------------------------------------------------	
//	Write to one VGS file
//----------------------------------------------------------------	
void CSun3D360ToolDlg::WriteToVGSFile(fstream& out, long size, char* data)
{
	if(!out.is_open()) return;
	if(size <= 0 || data == NULL) return;

	out.write(reinterpret_cast<const char*>(&size), sizeof(long));
	out.write(reinterpret_cast<const char*>(data), size);
}
//----------------------------------------------------------------	

