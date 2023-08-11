//-----------------------------------------------
//	ASDriveManager.cpp
//	The implement file for CASDriveManager
//-----------------------------------------------
#include "StdAfx.h"
#include "ASBurnEngine.h"
//-----------------------------------------------
//	Name:DeviceInfo_Callback
//	Introduce:Device information callback function for AS_Stroge, 
//		      Used in AS_StorageDevice_EraseMedia and other device functions
//-----------------------------------------------
AS_StorageError AS_CALLBACK DeviceInfo_Callback(const AS_StorageDevice::Info& info, 
							UInt32 infoDword, void* infoPtr, void* callerUse)
{
	if(NULL != callerUse)
		return ((CASBurnEngine*)callerUse)->DeviceInfoCallback(info, infoDword, infoPtr);

	return AS_StorageError_None;
}
//-----------------------------------------------
//	Name:VolumeInfo_Callback
//	Introduce:Volume information callback function for AS_Stroge, 
//		      Used in AS_StorageDevice_OpenVolume, AS_Volume_Create, AS_Volume_CreateTDF, AS_Volume_Mount and AS_Volume_Flush
//-----------------------------------------------
AS_StorageError AS_CALLBACK VolumeInfo_Callback(const AS_Volume::Info& info, 
							UInt32 infoDword, void* infoPtr, void* callerUse)
{
	if(NULL != callerUse)
		return ((CASBurnEngine*)callerUse)->VolumeInfoCallback(info, infoDword, infoPtr);

	return AS_StorageError_None;
}
//-----------------------------------------------



//-----------------------------------------------
//	Name:CASBurnEngine
//	Introduce:Construction
//-----------------------------------------------
CASBurnEngine::CASBurnEngine()
 : m_pDriveManager(NULL)
 , m_pSourceDrives(NULL)
 , m_pTargetDrives(NULL)
 , m_pProgressCallback(NULL)
 , m_pCallbackUserDatae(NULL)
{
	lstrcpy(m_szVolumeLabel, _T(" \0"));
}
//-----------------------------------------------
//	Name:~CASBurnEngine
//	Introduce:Distruction
//-----------------------------------------------
CASBurnEngine::~CASBurnEngine()
{
	Release();
}
//-----------------------------------------------
//	Name:DeviceInfoCallback
//	Introduce:Device information callback function
//-----------------------------------------------
AS_StorageError CASBurnEngine::DeviceInfoCallback(const AS_StorageDevice::Info& info, UInt32 infoDword, void* infoPtr)
{
	switch(info)
	{
	case AS_StorageDevice::Info_PromptForMediaInDevice: // infoDword = unused, infoPtr = unused
		break;
	case AS_StorageDevice::Info_ProgressUpdate:			// infoDword = percent complete, infoPtr = Progress structure
		if(NULL != m_pProgressCallback) m_pProgressCallback(info, infoDword, infoPtr, m_pCallbackUserDatae);
		break;
	}

	return AS_StorageError_None;
}
//-----------------------------------------------
//	Name:VolumeInfoCallback
//	Introduce:Volume information callback function
//-----------------------------------------------
AS_StorageError CASBurnEngine::VolumeInfoCallback(const AS_Volume::Info& info, UInt32 infoDword, void* infoPtr)
{
	switch(info)
	{
	case AS_Volume::Info_PromptForMediaInDevice:   // infoDword = unused, infoPtr = unused
		//std::cout << "Prompt For Media In Device" << std::endl << std::flush;
		// TODO: wait for user to put media in and press key
		break;

	case AS_Volume::Info_ProgressUpdate:   // infoDword = percent complete, infoPtr = Progress structure
		//std::cout << "Progress:" << infoDword << "% complete" << std::endl << std::flush;
		// TODO: output infoPtr's Progress structure information
		if(NULL != m_pProgressCallback) m_pProgressCallback(info, infoDword, infoPtr, m_pCallbackUserDatae);
		break;

	case AS_Volume::Info_PromptForNextMedia: // infoDword = unused, infoPtr = unused
		//std::cout << "Insert next media into the device, then press <enter>" << std::endl << std::flush;
		//std::string s;
		//std::cin >> s;
		break;
	}

	return AS_StorageError_None;
}
//-----------------------------------------------



//-----------------------------------------------
//	Name:InitEngine
//	Introduce:Initialize the burn engine
//-----------------------------------------------
BOOL CASBurnEngine::InitEngine(LPCTSTR lpszAppName)
{
	//Initliaze drives manager
	if(NULL == m_pDriveManager)
	{
		m_pDriveManager = new CASDriveManager(lpszAppName);
		m_pDriveManager->EnumDrives();
	}

	//Create the source drives list
	if(NULL == m_pSourceDrives)
	{
		m_pSourceDrives = new CASSourceDrives(m_pDriveManager);
		m_pSourceDrives->AddDrives();
	}

	//Create the target drives list
	if(NULL == m_pTargetDrives)
	{
		m_pTargetDrives = new CASTargetDrives(m_pDriveManager);
		m_pTargetDrives->AddDevices();
	}

	return TRUE;
}
//-----------------------------------------------
//	Name:Release
//	Introduce:Release the burn engine
//-----------------------------------------------
void CASBurnEngine::Release()
{
	//Release devices manager
	if(NULL != m_pDriveManager)
	{
		m_pDriveManager->ReleaseDrives();
		delete m_pDriveManager;
		m_pDriveManager = NULL;
	}

	//Release source drives list
	if(NULL != m_pSourceDrives)
	{
		delete m_pSourceDrives;
		m_pSourceDrives = NULL;
	}

	//Release target drives list
	if(NULL != m_pTargetDrives)
	{
		delete m_pTargetDrives;
		m_pTargetDrives = NULL;
	}
}
//-----------------------------------------------
//	Name:GetDriveManager
//	Introduce:Get drive manager in burn engine
//-----------------------------------------------
CDriveManager* CASBurnEngine::GetDriveManager()
{
	return m_pDriveManager;
}
//-----------------------------------------------
//	Name:GetSourceDriveCnt
//	Introduce:Return the number of source drivers in this Burn Engine
//-----------------------------------------------
DWORD CASBurnEngine::GetSourceDriveCnt()
{
	DWORD dwSourceDriveCnt = 0;
	if(NULL != m_pSourceDrives)
	{
		dwSourceDriveCnt = m_pSourceDrives->GetDriveCnt();
	}

	return dwSourceDriveCnt;
}
//-----------------------------------------------
//	Name:GetSourceDriveByIndex
//	Introduce:Return the source drive pointer by specific index in this Burn Engine
//-----------------------------------------------
CDrive* CASBurnEngine::GetSourceDriveByIndex(int iIndex)
{
	CDrive* pSourceDrive = NULL;
	if(NULL != m_pSourceDrives)
	{
		pSourceDrive = m_pSourceDrives->GetDriveByIndex(iIndex);
	}

	return pSourceDrive;
}
//-----------------------------------------------
//	Name:GetTargetDriveCnt
//	Introduce:Return the number of target drivers in this Burn Engine
//-----------------------------------------------
DWORD CASBurnEngine::GetTargetDriveCnt()
{
	DWORD dwTargetDriveCnt = 0;
	if(NULL != m_pTargetDrives)
	{
		dwTargetDriveCnt = m_pTargetDrives->GetDriveCnt();
	}

	return dwTargetDriveCnt;
}
//-----------------------------------------------
//	Name:GetTargetDriveByIndex
//	Introduce:Return the target drive pointer by specific index in this Burn Engine
//-----------------------------------------------
CDrive* CASBurnEngine::GetTargetDriveByIndex(int iIndex)
{
	CDrive* pTargetDrive = NULL;
	if(NULL != m_pTargetDrives)
	{
		pTargetDrive = m_pTargetDrives->GetDriveByIndex(iIndex);
	}

	return pTargetDrive;
}
//-----------------------------------------------


//-----------------------------------------------
//	Name:AddFileForBurn
//	Introduce:Add a file to list for burning
//-----------------------------------------------
void CASBurnEngine::AddFileForBurn(LPCTSTR lpszSrcFileName, LPCTSTR lpszDesFileName)
{
	BurnFileInfo fileInfo;
	lstrcpy(fileInfo.szSrcFileName, lpszSrcFileName);
	lstrcpy(fileInfo.szDesFileName, lpszDesFileName);
	
	m_lstBurnFileInfo.push_back(fileInfo);
}
//-----------------------------------------------
//	Name:ClearFileForBurn
//	Introduce:Clear all files which are in the list for burning
//-----------------------------------------------
void CASBurnEngine::ClearFileForBurn()
{
	m_lstBurnFileInfo.clear();
}
//-----------------------------------------------
//	Name:SetBurnSource
//	Introduce:Set the source device for burning
//-----------------------------------------------
void CASBurnEngine::SetBurnSource(int iDeviceID)
{
	assert(NULL != m_pSourceDrives);

	m_pSourceDrives->AddBurnSource(iDeviceID);
}
//-----------------------------------------------
//	Name:SetBurnSource
//	Introduce:Set a image file as burn source
//-----------------------------------------------
void CASBurnEngine::SetBurnSource(LPCTSTR lpszImageFile)
{
	assert(NULL != m_pSourceDrives);

	m_pSourceDrives->AddBurnSource(lpszImageFile);
}
//-----------------------------------------------
//	Name:AddBurnTarget
//	Introduce:Add a target device ID for burning
//-----------------------------------------------
LONG CASBurnEngine::AddBurnTarget(int iDeviceID)
{
	assert(NULL != m_pTargetDrives);

	return m_pTargetDrives->AddBurnTarget(iDeviceID);
}
//-----------------------------------------------
//	Name:AddBurnTarget
//	Introduce:Add a image file as burn target
//-----------------------------------------------
LONG CASBurnEngine::AddBurnTarget(LPCTSTR lpszImageFile)
{
	assert(NULL != m_pTargetDrives);

	return m_pTargetDrives->AddBurnTarget(lpszImageFile);
}
//-----------------------------------------------
//	Name:RemoveTarget
//	Introduce:Remove target device from burning list	
//-----------------------------------------------
void CASBurnEngine::RemoveTarget(int iDriveID, BOOL bRemoveAll)
{
	assert(NULL != m_pTargetDrives);

	m_pTargetDrives->RemoveTarget(iDriveID, bRemoveAll);
}
//-----------------------------------------------


//-----------------------------------------------
//	Name:EraseMedia
//	Introduce:Erase the media in specific device
//-----------------------------------------------
LONG CASBurnEngine::EraseMedia(int iDeviceID, ProgressInfoCallback pInfoCallback, LPVOID lpUserCallbackData, BOOL bEject)
{
	assert(NULL != m_pTargetDrives);

	m_pProgressCallback = pInfoCallback;
	m_pCallbackUserDatae = lpUserCallbackData;

	//Get the drive object pointer
	CASDrive* pDrive = m_pTargetDrives->GetDriveByID(iDeviceID);
	if(NULL == pDrive)
		return -1;

	//Reserve this drive handle
	LONG lRes = AS_StorageError_None;
	lRes = pDrive->Reserve();
	if(AS_StorageError_None != lRes)
		return lRes;

	//Lock this drive
	lRes = pDrive->Lock();
	if(AS_StorageError_None != lRes)
	{
		pDrive->Release();
		return lRes;
	}

	//Close this drive
	lRes = pDrive->Close(TRUE);
	if(AS_StorageError_None != lRes)
	{
		pDrive->Unlock();
		pDrive->Release();
		return lRes;
	}

	//To check this drive is ready or not.(If no media in drive, we'll get FALSE)
	BOOL bIsReady = pDrive->IsReady();
	if(FALSE == bIsReady)
	{
		pDrive->Unlock();
		pDrive->Eject();
		pDrive->Release();
		return -1;
	}

	//Get the media state
	DWORD dwMediaState = pDrive->GetMediaState();
	if((dwMediaState & AS_StorageDevice::MediaIsRewritable) == 0)
	{
		pDrive->Unlock();
		pDrive->Eject();
		pDrive->Release();
		return -1;
	}

	//Erase the media in this drive
	lRes = pDrive->EraseMedia(DeviceInfo_Callback, this);
	if(AS_StorageError_None != lRes)
	{
		pDrive->Unlock();
		pDrive->Eject();
		pDrive->Release();
		return lRes;
	}

	//Unlock this drive
	pDrive->Unlock();

	//Eject this drive or not
	if(TRUE == bEject)
	{
		pDrive->Eject();
	}

	//Release this drive
	pDrive->Release();

	return AS_StorageError_None;
}
//-----------------------------------------------	
//	Name:SetTargetVolumeLabel
//	Introduce:Set the target disc/image label
//-----------------------------------------------	
void CASBurnEngine::SetTargetVolumeLabel(LPCTSTR lpszVolumeLabel)
{
	lstrcpy(m_szVolumeLabel, lpszVolumeLabel);
}
//-----------------------------------------------	
//	Name:BurnToImage
//	Introduce:Burn all files(Add by calling function AddFileForBurn()) to an image
//-----------------------------------------------	
LONG CASBurnEngine::BurnToImage(ProgressInfoCallback pInfoCallback, LPVOID lpUserCallbackData)
{
	assert(NULL != m_pTargetDrives);
	assert(NULL != m_pDriveManager);

	m_pProgressCallback = pInfoCallback;
	m_pCallbackUserDatae = lpUserCallbackData;

	//Get the target image drive
	CASDrive* pTargetImageDrive = NULL;
	LPCTSTR lpTargetImageFile = m_pTargetDrives->GetImageFileName();
	pTargetImageDrive = m_pDriveManager->GetDriveByImageName(lpTargetImageFile);
	if(NULL == pTargetImageDrive)
		return -1;

	//Burn all burn files to the image device
	LONG lRes = 0;
	lRes = BurnToImageDevice(pTargetImageDrive);

	return lRes;
}
//-----------------------------------------------
//	Name:BurnToDisc
//	Introduce:Burn all files(Add by calling function AddFileForBurn()) to discs, 
//			  the target devices are selected by calling function AddBurnTarget()
//-----------------------------------------------	
LONG CASBurnEngine::BurnToDisc(ProgressInfoCallback pInfoCallback, LPVOID lpUserCallbackData, BOOL bEject)
{
	assert(NULL != m_pTargetDrives);
	assert(NULL != m_pDriveManager);

	//To check the number of selected targets
	int iSelTargetCnt = m_pTargetDrives->GetNumSelectedTargets();
	if(iSelTargetCnt <= 0)
		return -1;

	m_pProgressCallback = pInfoCallback;
	m_pCallbackUserDatae = lpUserCallbackData;

	//Create the temporary image file for target discs
	const TCHAR szImageFile[] = {_T("C:\\Tmp.iso")};
	CASDrive* pTmpImageDrive = m_pDriveManager->AddImageDrive(szImageFile);
	if(NULL == pTmpImageDrive)
		return -1;

	//Burn all burn files to the temporary image device
	LONG lRes = AS_StorageError_None;
	lRes = BurnToImageDevice(pTmpImageDrive);
	if(AS_StorageError_None != lRes)
	{
		m_pDriveManager->ReleaseImageDrive(szImageFile);
		return lRes;
	}

	//Open the temporary image device as source
	lRes = pTmpImageDrive->Reserve();
	if(AS_StorageError_None != lRes)
	{
		m_pDriveManager->ReleaseImageDrive(szImageFile);
		return lRes;
	}

	//Currnt target devices
	AS_StorageDevice* pTargetDevices = new AS_StorageDevice[iSelTargetCnt];
	for(int iIndex = 0; iIndex < iSelTargetCnt; iIndex++)
	{
		int iDriveID = m_pTargetDrives->GetSelectedTargetID(iIndex);
		CASDrive* pTargetDrive = m_pTargetDrives->GetDriveByID(iDriveID);
		if(NULL != pTargetDrive)
		{
		}
	}

	return AS_StorageError_None;
}
//-----------------------------------------------
//	Name:EraseMedia
//	Introduce:Copy a disc context from source to target. The source device is selected by calling 
//		      function SetBurnSource(),	the target devices are selected by calling function AddBurnTarget()
//-----------------------------------------------	
LONG CASBurnEngine::BurnCopy(ProgressInfoCallback pInfoCallback, LPVOID lpUserCallbackData, BOOL bEject)
{
	return AS_StorageError_None;
}
//-----------------------------------------------	




//-----------------------------------------------
//	Name:BurnToImageDevice
//	Introduce:Burn all files to the image device
//-----------------------------------------------
LONG CASBurnEngine::BurnToImageDevice(CASDrive* pImageDrive)
{
	if(NULL == pImageDrive)
		return -1;

	LONG lRes = 0;
	lRes = pImageDrive->Reserve();

	//Get the volume object
	CASMedia* pMedia = pImageDrive->OpenMedia(NULL, NULL);
	if(NULL == pMedia)
	{
		pImageDrive->Release();
		return -1;
	}

	//Create the volume
	lRes = pMedia->Format(AS_Volume::FS_ISO, m_szVolumeLabel, AS_Volume::Mastering, VolumeInfo_Callback, this);
	if(0 != lRes)
	{
		pImageDrive->Release();
		return -1;
	}	

	//Copy file from hard disc to the volume
	for(BurnFileArrayPtr::iterator it = m_lstBurnFileInfo.begin(); it != m_lstBurnFileInfo.end(); it++)
	{
		lRes = pMedia->CopyFile((*it).szSrcFileName, (*it).szDesFileName);
	}

	//Commit the change
	pMedia->Flush(AS_Volume::Border_Final, VolumeInfo_Callback, this);
	if(0 != lRes)
	{
		pMedia->Close();
		pImageDrive->Release();
		return -1;
	}

	//Close volume
	lRes = pMedia->Close();

	//Release drive
	lRes = pImageDrive->Release();

	return AS_StorageError_None;
}
//-----------------------------------------------	
