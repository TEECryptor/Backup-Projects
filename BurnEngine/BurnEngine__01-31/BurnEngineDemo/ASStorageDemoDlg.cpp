//--------------------------------------------------
// ASStorageDemoDlg.cpp : implementation file
//
//--------------------------------------------------
#include "stdafx.h"
#include "ASStorageDemo.h"
#include "ASStorageDemoDlg.h"
#include ".\asstoragedemodlg.h"
//--------------------------------------------------
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//--------------------------------------------------
DWORD WINAPI BurnThreadProc(LPVOID lpParameter)
{
	if(NULL != lpParameter)
		return ((CASStorageDemoDlg*)lpParameter)->ThreadBurnFiles();
	
	return 0;
}
//--------------------------------------------------
DWORD WINAPI EraseThreadProc(LPVOID lpParameter)
{
	if(NULL != lpParameter)
		return ((CASStorageDemoDlg*)lpParameter)->ThreadEraseDisc();

	return 0;
}
//--------------------------------------------------
LONG Volume_InfoCallback(DWORD dwInfoType, DWORD dwInfoData, LPVOID lpUserData)
{
	if(NULL != lpUserData)
		return ((CASStorageDemoDlg*)lpUserData)->VolumeInfoCallback(dwInfoType, dwInfoData);

	return 0;
}
//--------------------------------------------------
LONG Device_InfoCallback(DWORD dwInfoData, LPVOID lpUserData)
{
	if(NULL != lpUserData)
		return ((CASStorageDemoDlg*)lpUserData)->DeviceInfoCallback(dwInfoData);

	return 0;
}
//--------------------------------------------------
LONG BurnProgressInfoCallback(DWORD dwInfoType, DWORD dwInfoData, LPVOID lpInfoPtr, LPVOID lpUserData)
{
	if(NULL != lpUserData)
		return ((CASStorageDemoDlg*)lpUserData)->ProgressInfoCallback(dwInfoType, dwInfoData, lpInfoPtr);

	return 0;
}
//--------------------------------------------------



//--------------------------------------------------
// CAboutDlg dialog used for App About
//--------------------------------------------------
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
//--------------------------------------------------
CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}
//--------------------------------------------------
void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}
//--------------------------------------------------
BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()
//--------------------------------------------------


//--------------------------------------------------
// CASStorageDemoDlg dialog
//--------------------------------------------------
CASStorageDemoDlg::CASStorageDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CASStorageDemoDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	
	m_hThread = NULL;
	m_pBurnEngine = NULL;
	m_pDriveManager = NULL;
	m_bBurnToImage = FALSE;
	m_eBurnState = BS_INIT;
}
//--------------------------------------------------
void CASStorageDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS, m_ctlProgress);
}
//--------------------------------------------------
BEGIN_MESSAGE_MAP(CASStorageDemoDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_OPEN, OnBnClickedBtnOpen)
	ON_BN_CLICKED(IDC_BTN_BURN, OnBnClickedBtnBurn)
	ON_BN_CLICKED(IDC_CHECK_BURNIMAGE, OnBnClickedCheckBurnimage)
	ON_NOTIFY(NM_CLICK, IDC_LIST_DRIVES, OnNMClickListDrives)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_ERASE, OnBnClickedBtnErase)
END_MESSAGE_MAP()
//--------------------------------------------------
// CASStorageDemoDlg message handlers
//--------------------------------------------------
BOOL CASStorageDemoDlg::OnInitDialog()
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


	//Create the Burn Engine
	m_pBurnEngine = BurnEngine_Create();
	_ASSERTE(NULL != m_pBurnEngine);
	m_pBurnEngine->InitEngine(_T("BurnEngineDemo"));

	//Get the Drives Manager from Burn Engine
	m_pDriveManager = m_pBurnEngine->GetDriveManager();

	//Fill the device selection list
	FillDevicesList();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}
//--------------------------------------------------
void CASStorageDemoDlg::OnDestroy()
{
	CDialog::OnDestroy();

	if(NULL != m_pBurnEngine)
	{
		BurnEngine_Destroy(m_pBurnEngine);
		m_pBurnEngine = NULL;
	}
}
//--------------------------------------------------
void CASStorageDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//--------------------------------------------------
// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.
//--------------------------------------------------
void CASStorageDemoDlg::OnPaint() 
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
//--------------------------------------------------
// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
//--------------------------------------------------
HCURSOR CASStorageDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
//--------------------------------------------------




//--------------------------------------------------
//	Name:FillDevicesList
//	Introduce:Fill avaible devices to list
//--------------------------------------------------
void CASStorageDemoDlg::FillDevicesList()
{
	if(NULL == m_pBurnEngine)
		return;

	CListCtrl* pDrivesLstCtrl = (CListCtrl*)GetDlgItem(IDC_LIST_DRIVES);
	pDrivesLstCtrl->SetExtendedStyle(pDrivesLstCtrl->GetStyle() | LVS_EX_CHECKBOXES);
	pDrivesLstCtrl->InsertColumn(0, _T(""), LVCFMT_LEFT, 250);
	pDrivesLstCtrl->DeleteAllItems();	 

	DWORD dwDriveCnt = m_pBurnEngine->GetTargetDriveCnt();

	if(dwDriveCnt > 0)
	{
		pDrivesLstCtrl->DeleteAllItems();	 
		for(DWORD iCnt = 0; iCnt < dwDriveCnt; iCnt++)
		{
			CDrive* pDrive = m_pBurnEngine->GetTargetDriveByIndex(iCnt);
			if(NULL != pDrive)
			{
				TCHAR szDeviceDesc[520] = {_T("\0")};
				TCHAR szDevicePath[520] = {_T("\0")};
				pDrive->GetDescription(szDeviceDesc, 520);
				pDrive->GetDrivePath(szDevicePath, 520);

				TCHAR szDeviceStr[520] = {_T("\0")};
				_stprintf(szDeviceStr, _T("%s [%s:]"), szDeviceDesc, szDevicePath);
				int iIndex = pDrivesLstCtrl->InsertItem(iCnt, szDeviceStr);
				pDrivesLstCtrl->SetItemData(iIndex, pDrive->GetID());
			}
		}
	}
}
//--------------------------------------------------	
void CASStorageDemoDlg::OnNMClickListDrives(NMHDR *pNMHDR, LRESULT *pResult)
{
	static int iOldSelItem = -1;

	CListCtrl* pDrivesLstCtrl = (CListCtrl*)GetDlgItem(IDC_LIST_DRIVES);
	POSITION pos = pDrivesLstCtrl->GetFirstSelectedItemPosition();
	int iSelItem = pDrivesLstCtrl->GetNextSelectedItem(pos);
	if(iSelItem != iOldSelItem && -1 != iSelItem)
	{
		int iSelDriveID = (int)pDrivesLstCtrl->GetItemData(iSelItem);
		UpdateSelDriveInfo(iSelDriveID);
		iOldSelItem = iSelItem;
	}

	*pResult = 0;
}
//--------------------------------------------------
//	Name:OnBnClickedBtnOpen
//	Introduce:Open a file
//--------------------------------------------------
void CASStorageDemoDlg::OnBnClickedBtnOpen()
{
	CFileDialog fileDlg (TRUE, NULL, NULL,
						OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT, 
						_T("All files(*.*)|*.*||"));

	if(fileDlg.DoModal() == IDOK)
	{
		POSITION pos =  fileDlg.GetStartPosition();
		while(pos)
		{
			BurnFileMap fileNames;
			CString szFileName = fileDlg.GetNextPathName(pos);
			fileNames.strSrcFileName = szFileName;
			//
			wstring strDesFile = szFileName;
			UINT uPos = (UINT)strDesFile.rfind(_T("\\"));
			strDesFile = strDesFile.substr(uPos+1);

			fileNames.strDesFileName = _T("/");
			fileNames.strDesFileName += strDesFile;

			m_lstBurnFiles.push_back(fileNames);

			CString strFileNames;
			TCHAR szFileNames[1024] = {_T("\0")};
			GetDlgItemText(IDC_EDIT_SOURCE, szFileNames, 1024);

			strFileNames = szFileNames;
			strFileNames += ";";
			strFileNames += szFileName;
			SetDlgItemText(IDC_EDIT_SOURCE, strFileNames);
		}
	}
}
//--------------------------------------------------
//	Name:OnBnClickedBtnBurn
//	Introduce:Burn the selected file to device
//--------------------------------------------------
void CASStorageDemoDlg::OnBnClickedBtnBurn()
{
	//To check there are files or not
	if(m_lstBurnFiles.size() == 0)
		return;

	if(NULL == m_pBurnEngine)
		return;

	//Get the selected target devices
	m_lstSelDrivesID.clear();
	CListCtrl* pDrivesLstCtrl = (CListCtrl*)GetDlgItem(IDC_LIST_DRIVES);
	for(int iItem = 0; iItem < pDrivesLstCtrl->GetItemCount(); iItem++)
	{
		if(TRUE == pDrivesLstCtrl->GetCheck(iItem))
			m_pBurnEngine->AddBurnTarget((int)pDrivesLstCtrl->GetItemData(iItem));
	}
	if(m_pBurnEngine->GetTargetDriveCnt() == 0 && FALSE == m_bBurnToImage)
		return;	

	m_ctlProgress.SetRange(0, 100);
	m_ctlProgress.SetPos(0);

	DWORD dwThreadID = 0;
	if(NULL != m_hThread) 
		CloseHandle(m_hThread);
	m_hThread = CreateThread(NULL, 0, BurnThreadProc, this, 0, &dwThreadID);
}
//--------------------------------------------------
void CASStorageDemoDlg::OnBnClickedBtnErase()
{
	if(NULL == m_pBurnEngine)
		return;

	DWORD dwThreadID = 0;
	if(NULL != m_hThread) 
		CloseHandle(m_hThread);
	m_hThread = CreateThread(NULL, 0, EraseThreadProc, this, 0, &dwThreadID);
}
//--------------------------------------------------




//--------------------------------------------------	
//Burn files thread
//--------------------------------------------------	
DWORD CASStorageDemoDlg::ThreadBurnFiles()
{
	LONG lRes = 0;
	if(FALSE == m_bBurnToImage)
	{
		lRes = BurnToDisc();
	}
	else
	{
		lRes = BurnToImage();
	}

	return lRes;
}
//--------------------------------------------------	
//Erase discs trhead
//--------------------------------------------------	
DWORD CASStorageDemoDlg::ThreadEraseDisc()
{
	if(NULL == m_pBurnEngine)
		return 1;

	int iSelDriveID = -1;
	CListCtrl* pDrivesLstCtrl = (CListCtrl*)GetDlgItem(IDC_LIST_DRIVES);
	for(int iItem = 0; iItem < pDrivesLstCtrl->GetItemCount(); iItem++)
	{
		if(TRUE == pDrivesLstCtrl->GetCheck(iItem))
		{
			iSelDriveID = (int)pDrivesLstCtrl->GetItemData(iItem);
			break;
		}
	}

	if(-1 != iSelDriveID)
		m_pBurnEngine->EraseMedia(iSelDriveID, BurnProgressInfoCallback, this, TRUE);

	return 0;
}
//--------------------------------------------------	
//AS infomation callback function
//--------------------------------------------------	
LONG CASStorageDemoDlg::VolumeInfoCallback(DWORD dwInfoType, DWORD dwInfoData)
{
	TCHAR szCurInfo[MAX_PATH] = {_T("\0")};
	switch(dwInfoType)
	{
	case 2://AS_Volume::Info_PromptForMediaInDevice:   // infoDword = unused, infoPtr = unused
		//std::cout << "Prompt For Media In Device" << std::endl << std::flush;
		// TODO: wait for user to put media in and press key
		break;

	case 1://AS_Volume::Info_ProgressUpdate:   // infoDword = percent complete, infoPtr = Progress structure
		//std::cout << "Progress:" << infoDword << "% complete" << std::endl << std::flush;
		// TODO: output infoPtr's Progress structure information
		m_ctlProgress.SetPos(dwInfoData);

		_stprintf(szCurInfo, _T("%d%s"), dwInfoData, _T("%"));
		SetDlgItemText(IDC_STATIC_CURPOS, szCurInfo);
		break;

	case 3://AS_Volume::Info_PromptForNextMedia: // infoDword = unused, infoPtr = unused
	//	std::cout << "Insert next media into the device, then press <enter>" << std::endl << std::flush;
	//	std::string s;
	//	std::cin >> s;
		break;
	}

	return 0;
}
//--------------------------------------------------	
//AS device infomation callback function
//--------------------------------------------------	
LONG CASStorageDemoDlg::DeviceInfoCallback(DWORD dwInfoData)
{
/*
	TCHAR szCurInfo[MAX_PATH] = {_T("\0")};
	switch(info)
	{
	case AS_StorageDevice::Info_PromptForMediaInDevice:   // infoDword = unused, infoPtr = unused
		break;
	case AS_StorageDevice::Info_ProgressUpdate:   // infoDword = percent complete, infoPtr = Progress structure
		m_ctlProgress.SetPos(infoDword);
		_stprintf(szCurInfo, _T("%d%s"), dwInfoData, _T("%"));
		SetDlgItemText(IDC_STATIC_CURPOS, szCurInfo);
		break;
	}
*/

	TCHAR szCurInfo[MAX_PATH] = {_T("\0")};
	m_ctlProgress.SetPos(dwInfoData);
	_stprintf(szCurInfo, _T("%d%s"), dwInfoData, _T("%"));
	SetDlgItemText(IDC_STATIC_CURPOS, szCurInfo);

	return 0;
}
//--------------------------------------------------
//	Name:ProgressInfoCallback
//	Introduce:Progress information callbcak function
//--------------------------------------------------
LONG CASStorageDemoDlg::ProgressInfoCallback(DWORD dwInfoType, DWORD dwInfoData, LPVOID lpInfoPtr)
{
	TCHAR szCurInfo[MAX_PATH] = {_T("\0")};
	m_ctlProgress.SetPos(dwInfoData);
	_stprintf(szCurInfo, _T("%d%s"), dwInfoData, _T("%"));
	SetDlgItemText(IDC_STATIC_CURPOS, szCurInfo);

	return 0;
}
//--------------------------------------------------



//--------------------------------------------------
//	Name:BurnToDisc
//	Introduce:Burn files to disc
//--------------------------------------------------
LONG CASStorageDemoDlg::BurnToDisc()
{
	LONG lRes = 0;

	if(NULL == m_pBurnEngine)
		return -1;

	//To check there are files or not
	if(m_lstBurnFiles.size() == 0)
		return -1;

	//Add the files to Burn Engine for burning
	for(BurnFileArrayPtr::iterator it = m_lstBurnFiles.begin(); 
		it != m_lstBurnFiles.end(); it++)
	{
		m_pBurnEngine->AddFileForBurn(it->strSrcFileName.c_str(), it->strDesFileName.c_str());
	}

	//Set the target disc volume label
	m_pBurnEngine->SetTargetVolumeLabel(_T("Video"));

	//Start to burn
	lRes = m_pBurnEngine->BurnToDisc(BurnProgressInfoCallback, this);

	return lRes;

#if 0
	const TCHAR szImageFile[] = {_T("C:\\Test.iso")};


	//At first, we create a tmp image file
	lRes = BurnToImage();
	if(0 != lRes)
		return lRes;

	//Open the image file as the source drive
	CASDrive* pSrcDrive = m_pTargetDrives->AddImageDrive(szImageFile);
	if(NULL == pSrcDrive)
		return -1;

	//Prepare the target devices
	int iTargetCnt = m_lstSelDrivesID.size();
	if(iTargetCnt <= 0)
		return -1;

	int iIndex = 0;
	AS_StorageDevice* pTargetDevices = new AS_StorageDevice[iTargetCnt];
	for(IDArrayPtr::iterator it = m_lstSelDrivesID.begin(); 
		it != m_lstSelDrivesID.end(); 
		it++, iIndex++)
	{
		CASDrive* pDrive = m_pTargetDrives->GetDriveByID(*it);
		if(NULL != pDrive)
		{
			lRes = pDrive->Reserve();
			lRes = pDrive->Close(TRUE);
			BOOL bIsReady = pDrive->IsReady();

			m_eBurnState = BS_ERASE;
			UpdateBurnStateInfo();
			lRes = pDrive->EraseMedia(Device_InfoCallback, this);

			pTargetDevices[iIndex] = *((AS_StorageDevice*)pDrive->GetHandle());
		}
	}

	//Copy from source device to targets
	m_eBurnState = BS_WRITE;
	UpdateBurnStateInfo();
	lRes = AS_StorageDevice_Copy(	*((AS_StorageDevice*)pSrcDrive->GetHandle()), 
									AS_StorageDevice::FormatType_OpticalDetect, 
									AS_StorageDevice::CopyFlags_Write, 
									0, 
									iTargetCnt, 
									pTargetDevices, 
									Device_InfoCallback, 
									this);

	m_eBurnState = BS_FINISH;
	UpdateBurnStateInfo();
	for(IDArrayPtr::iterator it = m_lstSelDrivesID.begin();	it != m_lstSelDrivesID.end();it++, iIndex++)
	{
		CASDrive* pDrive = m_pTargetDrives->GetDriveByID(*it);
		if(NULL != pDrive)
		{
			pDrive->Eject();
			pDrive->Release();
		}
	}
	delete []pTargetDevices;
	pTargetDevices = NULL;

	m_pTargetDrives->ReleaseImageDrive(szImageFile);

#endif //0

#if 0
	//To check there are files or not
	if(m_lstBurnFiles.size() == 0)
		return -1;

	//Get the current driver
	CComboBox* pDevicesList = (CComboBox*)GetDlgItem(IDC_COMBO_DEVICES);
	int iSelIndex = pDevicesList->GetCurSel();
	int iSelDeviceID = (int)pDevicesList->GetItemData(iSelIndex);
	CASDrive* pSelDrive = m_ASDrives.GetDriveByIndex(iSelDeviceID);
	if(NULL == pSelDrive)
		return -1;

	m_eBurnState = BS_INIT;
	UpdateBurnStateInfo();

	//Access this drive
	lRes = pSelDrive->Reserve();
	if(0 != lRes)
		return lRes;

	//Close this drive
	lRes == pSelDrive->Close(TRUE);
	if(0 != lRes)
		return lRes;

	//To check this drive is ready or not
	BOOL bIsReady = pSelDrive->IsReady();
	if(FALSE == bIsReady)
	{
		pSelDrive->Eject();
		pSelDrive->Release();
		MessageBox(_T("There are no any disc in the drive!"));
		return -1;
	}

	//To get the media state in this drive
	DWORD dwMediaState;
	dwMediaState = pSelDrive->GetMediaState();

	//If the disc is not blank
	if(!(dwMediaState & AS_StorageDevice::MediaIsBlank))
	{
		//The disc can't be rewrited
		if(!(dwMediaState & AS_StorageDevice::MediaIsRewritable))
		{
			pSelDrive->Eject();
			pSelDrive->Release();
			MessageBox(_T("The disc can't be written again!"));
			return -1;
		}
		//The disc can't be erased
		if(!(dwMediaState & AS_StorageDevice::MediaIsErasable))
		{
			pSelDrive->Eject();
			pSelDrive->Release();
			MessageBox(_T("The disc isn't blank, and it can't be erased!"));
			return -1;
		}

		//Erase this disc
		m_eBurnState = BS_ERASE;
		UpdateBurnStateInfo();
		lRes = pSelDrive->EraseMedia(Device_InfoCallback, this);
		if(0 != lRes)
		{
			pSelDrive->Eject();
			pSelDrive->Release();
			MessageBox(_T("An error occured when erasing the disc!"));
			return -1;
		}
	}
	else
	{
		//The disc can't be written
		if(!(dwMediaState & AS_StorageDevice::MediaIsWritable))
		{
			pSelDrive->Eject();
			pSelDrive->Release();
			MessageBox(_T("The disc can't be written!"));
			return -1;
		}

	}

	//Get the volume kind
	DWORD dwFormatType = 0;
	DWORD dwMedKind = pSelDrive->GetMediaKind();
	switch (dwMedKind)
	{
	case AS_StorageDevice::MediaIsCdRom:
	case AS_StorageDevice::MediaIsCdr:
	case AS_StorageDevice::MediaIsCdrw:
		dwFormatType = AS_Volume::FS_ISO_Level_2_Long;
		break;
	case AS_StorageDevice::MediaIsDvdr:
	case AS_StorageDevice::MediaIsDvdpr:
	case AS_StorageDevice::MediaIsDvdrw:
	case AS_StorageDevice::MediaIsDvdprw:
	case AS_StorageDevice::MediaIsDvdRam:
		dwFormatType = AS_Volume::FS_UDF15;
		break;
	default:
		dwFormatType = AS_Volume::FS_UDF15;
		break;
	};

	//Get the volume in this drive
	m_eBurnState = BS_WRITE;
	UpdateBurnStateInfo();
	CASMedia* pMedia = pSelDrive->OpenMedia(NULL, NULL);
	if(NULL == pMedia)
	{
		pSelDrive->Eject();
		pSelDrive->Release();
		MessageBox(_T("The disc is error! Unkown error."));
		return -1;
	}

	//Create the volume
	lRes = pMedia->Format(dwFormatType, _T("TestAS"), AS_Volume::Mastering, Volume_InfoCallback, this);
	if(0 != lRes)
	{
		pSelDrive->Release();
		MessageBox(_T("Create volume error!"));
		return -1;
	}	

	//Copy file from hard disc to the volume
	for(BurnFileArrayPtr::iterator it = m_lstBurnFiles.begin(); it != m_lstBurnFiles.end(); it++)
	{
		lRes = pMedia->CopyFile((*it).strSrcFileName.c_str(), (*it).strDesFileName.c_str());
	}

	//Commit the change
	pMedia->Flush(AS_Volume::Border_Final, Volume_InfoCallback, this);
	if(0 != lRes)
	{
		pMedia->Close();
		pSelDrive->Release();
		MessageBox(_T("Flush volume error!"));
		return -1;
	}

	m_eBurnState = BS_CLOSE;
	UpdateBurnStateInfo();

	//Close volume
	lRes = pMedia->Close();

	//Eject the drive
	lRes = pSelDrive->Eject();

	//Release drive
	lRes = pSelDrive->Release();

	m_eBurnState = BS_FINISH;
	UpdateBurnStateInfo();
#endif //0

	return 0;
}
//--------------------------------------------------
//	Name:BurnToImage
//	Introduce:Burn files to image file
//--------------------------------------------------
LONG CASStorageDemoDlg::BurnToImage()
{
	LONG lRes = 0;

	if(NULL == m_pBurnEngine)
		return -1;

	//To check there are files or not
	if(m_lstBurnFiles.size() == 0)
		return -1;

	//Add the files to Burn Engine for burning
	for(BurnFileArrayPtr::iterator it = m_lstBurnFiles.begin(); 
		it != m_lstBurnFiles.end(); it++)
	{
		m_pBurnEngine->AddFileForBurn(it->strSrcFileName.c_str(), it->strDesFileName.c_str());
	}

	//Add the target image file name
	const TCHAR szImageFile[] = {_T("C:\\Test.iso")};
	lRes = m_pBurnEngine->AddBurnTarget(szImageFile);
	if(0 != lRes)
	{
		m_pBurnEngine->ClearFileForBurn();
		m_pBurnEngine->RemoveTarget(-1);
	}

	//Start to burn
	lRes = m_pBurnEngine->BurnToImage(BurnProgressInfoCallback, this);

	m_pBurnEngine->ClearFileForBurn();
	m_pBurnEngine->RemoveTarget(-1);
	return lRes;

	//m_eBurnState = BS_INIT;
	///UpdateBurnStateInfo();


#if 0
	//To check there are files or not
	if(m_lstBurnFiles.size() == 0)
		return -1;

	const TCHAR szImageFile[] = {_T("C:\\Test.iso")};

	//Get the current driver
	CASDrive* pSelDrive = m_pTargetDrives->AddImageDrive(szImageFile);
	if(NULL == pSelDrive)
		return -1;

	LONG lRes = 0;
	m_eBurnState = BS_INIT;
	UpdateBurnStateInfo();

	lRes = pSelDrive->Reserve();

	CASMedia* pMedia = pSelDrive->OpenMedia(NULL, NULL);
	if(NULL == pMedia)
	{
		pSelDrive->Release();
		MessageBox(_T("The disc is error! Unkown error."));
		return -1;
	}


	//Create the volume
	lRes = pMedia->Format(AS_Volume::FS_UDF15, _T("TestAS"), AS_Volume::Mastering, Volume_InfoCallback, this);
	if(0 != lRes)
	{
		pSelDrive->Release();
		MessageBox(_T("Create volume error!"));
		return -1;
	}	

	//Copy file from hard disc to the volume
	for(BurnFileArrayPtr::iterator it = m_lstBurnFiles.begin(); it != m_lstBurnFiles.end(); it++)
	{
		lRes = pMedia->CopyFile((*it).strSrcFileName.c_str(), (*it).strDesFileName.c_str());
	}

	//Commit the change
	pMedia->Flush(AS_Volume::Border_Final, Volume_InfoCallback, this);
	if(0 != lRes)
	{
		pMedia->Close();
		pSelDrive->Release();
		MessageBox(_T("Flush volume error!"));
		return -1;
	}

	//Close volume
	lRes = pMedia->Close();

	//Release drive
	lRes = pSelDrive->Release();

	//Release the image file drive from list
	m_pTargetDrives->ReleaseImageDrive(szImageFile);
#endif //0

	return 0;
}
//--------------------------------------------------
//	Name:UpdateSelDriveInfo
//	Introduce:Update the selected drive information
//--------------------------------------------------	
void CASStorageDemoDlg::UpdateSelDriveInfo(int iSelDriveID)
{
	if(NULL == m_pDriveManager)
		return;

	CDrive* pDrive = NULL;
	pDrive = m_pDriveManager->GetDriveByID(iSelDriveID);	

	if(NULL != pDrive)
	{
		TCHAR scDeviceCapaleStr[256]; 
		DWORD dwDeviceCapable = pDrive->GetCapableTypes();
		GetCapableTypeStr(dwDeviceCapable, scDeviceCapaleStr);
		SetDlgItemText(IDC_STATIC_CAPABLE, scDeviceCapaleStr);
		//
		TCHAR scManufacturer[64];
		pDrive->GetManufacturer(scManufacturer, 64);
		SetDlgItemText(IDC_STATIC_MAKENAME, scManufacturer);
		//
		TCHAR scModelName[64];
		pDrive->GetModelName(scModelName, 64);
		SetDlgItemText(IDC_STATIC_MODELNAME, scModelName);
		//
		TCHAR scVersion[64];
		pDrive->GetVersion(scVersion, 64);
		SetDlgItemText(IDC_STATIC_VERSION, scVersion);
		//
		int i = 0;
		TCHAR scDataRate[128] = _T("N/A\0");
		DWORD dwDataRates[128] = {0};
		//
		BOOL bIsReady = pDrive->IsReady();
		if(bIsReady)
		{
			lstrcpy(scDataRate, _T("\0"));
			pDrive->GetDataRates(dwDataRates, 128);
			while(0 != dwDataRates[i])
			{
				TCHAR scRate[32];
				_stprintf(scRate, _T("%.1fx"), dwDataRates[i]/1024.0f);
				lstrcat(scDataRate, scRate);
				i++;
			}
		}
		SetDlgItemText(IDC_STATIC_DATARATE, scDataRate);
	}
}
//--------------------------------------------------
//	Name:UpdateBurnStateInfo
//	Introduce:Update current burn information
//--------------------------------------------------	
void CASStorageDemoDlg::UpdateBurnStateInfo()
{
	TCHAR szInfoStr[256] = {_T("\0")};
	switch(m_eBurnState)
	{
	case BS_INIT:
		lstrcpy(szInfoStr, _T("Initliazing..."));
		break;
	case BS_ERASE:
		lstrcpy(szInfoStr, _T("Erasing Disc..."));
		break;
	case BS_WRITE:
		lstrcpy(szInfoStr, _T("Writting Files To Disc..."));
		break;
	case BS_CLOSE:
		lstrcpy(szInfoStr, _T("Closing Disc..."));
		break;
	case BS_FINISH:
		lstrcpy(szInfoStr, _T("Burn Finished"));
		break;
	}

	SetDlgItemText(IDC_STATIC_INFO, szInfoStr);	
}
//--------------------------------------------------	
//Get device capable type strings
//--------------------------------------------------	
void CASStorageDemoDlg::GetCapableTypeStr(DWORD dwDeviceCapable, LPTSTR lpszDeviceCapaleStr)
{
	if(NULL == lpszDeviceCapaleStr)
		return;

	lstrcpy(lpszDeviceCapaleStr, _T("\0"));

	//Construct the capable types string
	if (dwDeviceCapable & MediaIsCdRom)
		lstrcat(lpszDeviceCapaleStr, _T("CD-Rom"));
	if (dwDeviceCapable & MediaIsCdr)
		lstrcat(lpszDeviceCapaleStr, _T(", CD-R"));
	if (dwDeviceCapable & MediaIsCdrw)
		lstrcat(lpszDeviceCapaleStr, _T(", CD-RW"));
	if (dwDeviceCapable & MediaIsDdCdrom)
		lstrcat(lpszDeviceCapaleStr, _T(", DD-CDRom"));
	if (dwDeviceCapable & MediaIsDdCdr)
		lstrcat(lpszDeviceCapaleStr, _T(", DD-CDR"));
	if (dwDeviceCapable & MediaIsDdCdrw)
		lstrcat(lpszDeviceCapaleStr, _T(", DDCD-RW"));
	if (dwDeviceCapable & MediaIsDvdRom)
		lstrcat(lpszDeviceCapaleStr, _T(", DVD-Rom"));
	if (dwDeviceCapable & MediaIsDvdr)
		lstrcat(lpszDeviceCapaleStr, _T(", DVD-R"));
	if (dwDeviceCapable & MediaIsDvdrw)
		lstrcat(lpszDeviceCapaleStr, _T(", DVD-RW"));
	if (dwDeviceCapable & MediaIsDvdpr)
		lstrcat(lpszDeviceCapaleStr, _T(", DVD+R"));
	if (dwDeviceCapable & MediaIsDvdprw)
		lstrcat(lpszDeviceCapaleStr, _T(", DVD+RW"));
	if (dwDeviceCapable & MediaIsDvdRam)
		lstrcat(lpszDeviceCapaleStr, _T(", DVD+RAM"));
	if (dwDeviceCapable & MediaIsDvdpr9)
		lstrcat(lpszDeviceCapaleStr, _T(", DVD+R Dual layer"));
	if (dwDeviceCapable & MediaIsDvdr9)
		lstrcat(lpszDeviceCapaleStr, _T(", DVD-R Dual Layer"));
	if (dwDeviceCapable & MediaIsBdr)
		lstrcat(lpszDeviceCapaleStr, _T(", BD-R"));
	if (dwDeviceCapable & MediaIsBdre)
		lstrcat(lpszDeviceCapaleStr, _T(", BD-RE"));
	if (dwDeviceCapable & MediaIsSeqTape)
		lstrcat(lpszDeviceCapaleStr, _T(", Sequential Tape"));
}
//--------------------------------------------------	
void CASStorageDemoDlg::OnBnClickedCheckBurnimage()
{
	m_bBurnToImage = (BST_CHECKED == ((CButton*)GetDlgItem(IDC_CHECK_BURNIMAGE))->GetCheck()) ? TRUE : FALSE;
}
//--------------------------------------------------	
