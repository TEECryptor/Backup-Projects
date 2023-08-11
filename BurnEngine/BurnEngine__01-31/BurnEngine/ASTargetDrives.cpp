//-----------------------------------------------
//	ASTargetDrives.cpp
//	The implement file for CASTargetDrives
//-----------------------------------------------
#include "StdAfx.h"
//#include "CommonDef.h"
#include "ASTargetdrives.h"
//-----------------------------------------------
//	Name:CASTargetDrives
//	Introduce:Construction
//-----------------------------------------------
CASTargetDrives::CASTargetDrives(CASDriveManager* pDriveManager)
 : CASDrives(pDriveManager)
 , m_pImageTargetDrive(NULL)
 , m_iNumOfCopies(0)
 , m_bIsCompatible(FALSE)
{
	lstrcpy(m_szImageFile, _T("\0"));
}
//-----------------------------------------------
//	Name:~CASTargetDrives
//	Introduce:Distruction
//-----------------------------------------------
CASTargetDrives::~CASTargetDrives()
{
}
//-----------------------------------------------


//-----------------------------------------------
//	Name:AddDevices
//	Introduce:Add devices to this target devices object
//-----------------------------------------------
BOOL CASTargetDrives::AddDevices(void)
{
	if(NULL == m_pDriveManager)
		return FALSE;

	DWORD dwDriveCnt = m_pDriveManager->GetDrivesCnt();
	if(0 == dwDriveCnt)
		return FALSE;

	for(DWORD dwCnt = 0; dwCnt < dwDriveCnt; dwCnt++)
	{
		CASDrive* pDrive = m_pDriveManager->GetDriveByIndex(dwCnt);
		if(NULL != pDrive && pDrive->IsWriteable())
		{
			m_lstASDrive.push_back(pDrive);
		}
	}

	return TRUE;
}
//-----------------------------------------------
//	Name:AddBurnTarget
//	Introduce:Add which drive for burning target
//-----------------------------------------------
LONG CASTargetDrives::AddBurnTarget(int iDriveID)
{
	//To check this drive ID is exist in the selected list or not
	for(IDArrayPtr::iterator it = m_lstSelTargetID.begin(); it != m_lstSelTargetID.end(); it++)
	{
		if(iDriveID == (*it))
		{
			return S_OK;
		}
	}

	//Add the new selected drive ID to selected list
	LONG lRes = AS_StorageError_NotFound;
	for(ASDriveArrayPtr::iterator it = m_lstASDrive.begin(); it != m_lstASDrive.end(); it++)
	{
		CASDrive* pDrive = reinterpret_cast<CASDrive*>(*it);
		if(NULL != pDrive && (iDriveID == pDrive->GetID()))
		{
			lRes = S_OK;
			lstrcpy(m_szImageFile, _T("\0"));
			m_lstSelTargetID.push_back(iDriveID);
			break;
		}
	}

	return lRes;
}
//-----------------------------------------------
//	Name:AddBurnTarget
//	Introduce:Add an image file as burning target
//-----------------------------------------------
LONG CASTargetDrives::AddBurnTarget(LPCTSTR lpszImageFile)
{
	if(NULL == lpszImageFile || lstrlen(lpszImageFile) == 0)
		return AS_StorageError_InvalidFile;

	LONG lRes = AS_StorageError_NotFound;
	if(NULL != m_pDriveManager)
	{
		if(NULL != m_pImageTargetDrive)
			m_pDriveManager->ReleaseImageDrive(m_szImageFile); 

		m_pImageTargetDrive = m_pDriveManager->AddImageDrive(lpszImageFile);
		if(NULL != m_pImageTargetDrive)
		{
			lRes = S_OK;
			lstrcpy(m_szImageFile, lpszImageFile);
		}
	}

	return lRes;	
}
//-----------------------------------------------
//	Name:RemoveTarget
//	Introduce:Remove a drive from selected list
//-----------------------------------------------
void CASTargetDrives::RemoveTarget(int iDriveID, BOOL bRemoveAll)
{
	//Remove all selected target drive ID and image drive
	if(TRUE == bRemoveAll)
	{
		m_lstSelTargetID.clear();

		if(NULL != m_pDriveManager) 
			m_pDriveManager->ReleaseImageDrive(m_szImageFile);
	}
	//Only remove the image drive
	else if(-1 == iDriveID)
	{
		if(NULL != m_pDriveManager) 
		{
			m_pDriveManager->ReleaseImageDrive(m_szImageFile);
		}
	}
	//Remove the specific drive ID from selected list
	else
	{
		for(IDArrayPtr::iterator it = m_lstSelTargetID.begin(); it != m_lstSelTargetID.end(); it++)
		{
			if(iDriveID == (*it))
			{
				m_lstSelTargetID.erase(it);
				break;
			}
		}
	}
}
//-----------------------------------------------
//	Name:CanWriteDVD
//	Introduce:The all selected target can write DVD or not
//-----------------------------------------------
BOOL CASTargetDrives::CanWriteDVD()
{
	BOOL bCanwriteDVD = FALSE;
	DWORD dwSelDriveCnt = GetNumSelectedTargets();

	for(DWORD dwIndex = 0;dwIndex < dwSelDriveCnt; dwIndex++)
	{
		bCanwriteDVD = FALSE;
		int iDriveID = GetSelectedTargetID(dwIndex);
		CASDrive* pTargetDrive = GetDriveByID(iDriveID);
		if(NULL != pTargetDrive)
		{
			bCanwriteDVD = pTargetDrive->CanWriteDVD();
			if(FALSE == bCanwriteDVD)
				break;
		}
	}

	return bCanwriteDVD;
}
//-----------------------------------------------
//	Name:GetNumSelectedTargets
//	Introduce:Return the selected targets count
//-----------------------------------------------
int CASTargetDrives::GetNumSelectedTargets()
{
	int iSelDriveCnt = 0;
	iSelDriveCnt = (int)m_lstSelTargetID.size();

	return iSelDriveCnt;
}
//-----------------------------------------------
//	Name:GetSelectedTargetID
//	Introduce:Get the target drive ID by index
//-----------------------------------------------
int CASTargetDrives::GetSelectedTargetID(int iIndex)
{
	int i = 0;
	int iDriveID = 0;
	for(IDArrayPtr::iterator it = m_lstSelTargetID.begin(); it != m_lstSelTargetID.end(); it++, i++)
	{
		if(iIndex == i)
		{
			iDriveID = (*it);
			break;
		}
	}

	return iDriveID;
}
//-----------------------------------------------
//	Name:GetImageFileName
//	Introduce:Get the image file name of current image drive
//-----------------------------------------------
LPCTSTR CASTargetDrives::GetImageFileName()
{
	return m_szImageFile;
}
//-----------------------------------------------



/*
// // add one additional Target drive
RetVal CTargetDrives::AddBurnTarget(int id,bool bAddtoListOnly)
{
    // Dragon removing
    RetVal Ret=ERR_CMD_NO_DEVICE;
 //   m_CriticalSection.Lock();
	//if(m_pDriveList->GetCount()>=1)
	//{
	//
	//	m_strFilename=_T("");
	//	if (!bAddtoListOnly) 
	//	{
	//		Ret=AddBurnTarget(m_DataProject,id);
	//		if(IS_OK(Ret))
	//			Ret=AddBurnTarget((IProject&)m_VRProject,id);
	//		if(IS_OK(Ret))
	//			Ret=AddBurnTarget(m_CopierProject,id);
	//		if(IS_OK(Ret))
	//			Ret=AddBurnTarget(m_ImageProject,id);
	//	}

	//	IDevice *pDevice=GetDevicesAt(id);
	//	BusType busType;
	//	if ( pDevice)
	//	{
	//		busType= (BusType)pDevice->GetAttributeLong(AttrULongBusType);
	//	}
	//	if ( (BusUsb_10 == busType) )
	//	{
	//		Ret=DTR_ERR_USB1;
	//	}
	//	m_TargetIDList.AddTail(id);
	//}
	//m_CriticalSection.Unlock();
    // End Dragon removing
    return Ret;
}

// // remove all burning target
void CTargetDrives::RemoveTargets(bool bDeleteIdList)
{
    // Dragon removing
    //if(m_pDriveList->GetCount()<1)
	//	return;

	//m_CriticalSection.Lock();
	//m_strFilename=_T("");
	//RemoveTargets(m_DataProject);
	//RemoveTargets((IProject&)m_VRProject);
	//RemoveTargets(m_CopierProject);
	//RemoveTargets(m_ImageProject);

	//if(bDeleteIdList)
	//{
	//	m_TargetIDList.RemoveAll();
	//}
	//m_CriticalSection.Unlock();
    // End Dragon removing
}

RetVal CTargetDrives::SetTargetRecordingOptions(unsigned long CopySpeed, unsigned long NumOfCopies,enumRecordMode RecordMode)
{
	RetVal Ret=ERR_CMD_NO_DEVICE;
    // Dragon removing
 //   m_CriticalSection.Lock();
	//if(m_pDriveList->GetCount()>=1)
	//{
	//	m_NumOfCopies=NumOfCopies;
	//	RetVal Ret;
	//	Ret=_SetTargetRecordingOptions(m_DataProject,CopySpeed,NumOfCopies,RecordMode);
	//	if(IS_OK(Ret))
	//		_SetTargetRecordingOptions((IProject&)m_VRProject,CopySpeed,NumOfCopies,RecordMode);
	//	if(IS_OK(Ret))
	//		_SetTargetRecordingOptions(m_CopierProject,CopySpeed,NumOfCopies,RecordMode);
	//	if(IS_OK(Ret))
	//		_SetTargetRecordingOptions(m_ImageProject,CopySpeed,NumOfCopies,RecordMode);
	//}
	//m_CriticalSection.Unlock();
    // End Dragon removing
    return Ret;
}

RetVal CTargetDrives::_SetTargetRecordingOptions(IProject& Project,unsigned long CopySpeed, unsigned long NumOfCopies,enumRecordMode RecordMode)
{

    // Dragon removing
 //   if (RecordMode == DCTestOnly)
	//{
	//	NumOfCopies = 0;
	//	// Make sure that we eject the discs after the copy:
	//	Project.SetAttributeBool(AttrBoolEjectTargetDrivesAfterWriteRelease, true);
	//	Project.SetAttributeBool(AttrBoolPerformWriteTest, true);

	//}
	//else if( RecordMode == DCTestThenCopy)
	//{
	//	// Make sure that we eject the discs after the copy:
	//	Project.SetAttributeBool(AttrBoolEjectTargetDrivesAfterWriteRelease, false);
	//	Project.SetAttributeBool(AttrBoolPerformWriteTest, true);
	//}
	//else
	//{
	//	// Make sure that we eject the discs after the copy:
	//	Project.SetAttributeBool(AttrBoolEjectTargetDrivesAfterWriteRelease, true);
	//	// Tell the Copier Project that we want to do an actual write:
	//	Project.SetAttributeBool(AttrBoolPerformWriteTest, false);
	//}

	//if (NumOfCopies > 1 && RecordMode != DCTestThenCopy)
	//{
	//	// Make sure that we eject the discs after each copy in the
	//	// case of multiple copies;
	//	Project.SetAttributeBool(AttrBoolEjectTargetDrivesAfterWriteOperations, true );
	//}
	//else
	//{
	//	Project.SetAttributeBool(AttrBoolEjectTargetDrivesAfterWriteOperations, false );
	//}
	//if (CopySpeed!=0xFFFFFFFF)
	//{
	//	Project.SetAttributeLong(AttrULongWriteSpeedInKBPS, CopySpeed);
	//}
	//Project.SetAttributeLong(AttrULongNumberOfCopies,NumOfCopies);

    // End Dragon removing
    return S_OK;
}
RetVal CTargetDrives::AddBurnTarget(IProject& Project,int id)
{
	RetVal Ret;	
    // Dragon removing
 //   IWriteDeviceHandler* pWriteDeviceHandler=NULL;
	//if(!&Project)
	//	return ERR_FAIL;
	//Project.GetRelatedInterface(IID_IWriteDeviceHandler,0,(void**)&pWriteDeviceHandler);
	//_ASSERTE(pWriteDeviceHandler!=NULL);
	//Ret=pWriteDeviceHandler->AddTargetDevice(GetDevicesAt(id),NULL);
	//pWriteDeviceHandler->Release();
    // End Dragon removing
    return Ret;
}

//remove all burning target
void CTargetDrives::RemoveTargets(IProject& Project)
{
    // Dragon removing
 //   RetVal Result;
	//IWriteDeviceHandler* pWriteDeviceHandler=NULL;
	//Project.GetRelatedInterface(IID_IWriteDeviceHandler,0,(void**)&pWriteDeviceHandler);
	//_ASSERTE(pWriteDeviceHandler!=NULL);
	//do
	//{
	//	Result = pWriteDeviceHandler->DeleteTargetDeviceAt(0);
	//} while (ERR_OK == Result);
	//pWriteDeviceHandler->Release();
    // End Dragon removing
}

ULONG CTargetDrives::_GetWriteSpeeds(CAtlList<wstring>& ListWriteSpeedStrings,CAtlList<ULONG>& ListWriteSpeedIndex,IProject &Project)
{
    // Dragon removing
 //   if(m_pDriveList->GetCount()<1)
	//	return 0;

	//// Get the supported write speeds:
	//int				nItemIndex = 0;
	//unsigned long	speedToReturn = 0;
	//ULONG			*pSpeeds = NULL;
	ULONG			nCount=0;
	//unsigned long	lSize = 0;
	//ULONG			lKBperXFactor = 0;

	//RetVal lRet=Project.GetAttributeLongArray(AttrULongArraySupportedWriteSpeedsInKBPS, &pSpeeds, &lSize);
	//if ( IS_OK(lRet) ) 
	//{
	//	m_bIsCompatible=true;
	//	lKBperXFactor = Project.GetAttributeLong ( AttrULongSpeedKBPerXFactor );

	//	if (pSpeeds)
	//	{
	//		for (nCount = 0; nCount < lSize; ++nCount)
	//		{
	//			// Get the speed from the Dragon Resources DLL.
	//			WCHAR *pBuffer;
	//			m_StringLookup.SetAttributeLong		(AttrULongSpeedKBPerXFactor, lKBperXFactor);
	//			m_StringLookup.SetAttributeLong(AttrULongDataReadSpeedInKBPS, pSpeeds[nCount]);
	//			m_StringLookup.GetAttributeString	(AttrStringLookedUp, (UniChar**)&pBuffer);				
	//			ListWriteSpeedStrings.AddTail(pBuffer);
	//			ListWriteSpeedIndex.AddTail(pSpeeds[nCount]);
	//			m_StringLookup.Free(pBuffer);
	//		}
	//		m_DeviceDriverMain.Free(pSpeeds);
	//	}
	//}
	//else
	//{
	//	m_bIsCompatible=false;
	//}

    // End Dragon removing

	return nCount;
}

// // add one additional Target drive
RetVal CTargetDrives::AddBurnTarget(wstring strImageName)
{
    // Dragon removing
 //   m_CriticalSection.Lock();
	RetVal Ret=0;
	//RemoveTargets();
	//Ret=AddBurnTarget(m_ImageProject,strImageName);
	//if(IS_OK(Ret))
	//	Ret=AddBurnTarget(m_DataProject,strImageName);	
	//if(IS_OK(Ret))
	//	Ret=AddBurnTarget(m_CopierProject,strImageName);
	//if(IS_OK(Ret))
	//	Ret=AddBurnTarget((IProject&)m_VRProject,strImageName);
	//if(IS_OK(Ret))
	//	m_strFilename=strImageName;
	//m_CriticalSection.Unlock();
    // End Dragon removing
    return Ret;
}

RetVal CTargetDrives::AddBurnTarget(IProject& Project,wstring strImageName)
{
	RetVal Ret=ERR_OK;	
	
    // Dragon removing
 //   IAttributeContainer64	*pImageFileDevice;

	//IWriteDeviceHandler* pWriteDeviceHandler=NULL;
	//Project.GetRelatedInterface(IID_IWriteDeviceHandler,0,(void**)&pWriteDeviceHandler);
	//_ASSERTE(pWriteDeviceHandler!=NULL);
	//pWriteDeviceHandler->CreateGenericTargetDevice(&pImageFileDevice);
	//
	//size_t iPos=strImageName.find_last_of(_T("."));
	//if(iPos!=-1)
	//{
	//	WCHAR strExtW[4];
	//	wstring strExt=strImageName.substr(iPos+1);
	//	_tcscpy(strExtW,strExt.data());
	//	_tcslwr(strExtW);
	//	strExt=strExtW;
	//	pImageFileDevice->SetAttributeLong(AttrULongDeviceType, DeviceImageFile);
	//	//workaround for dragon 
	//	if(strExt==_T("cue"))
	//	{
	//		wstring strTemp;
	//		strExt=_T("bin");
	//		strTemp=strImageName.substr(0,iPos+1);
	//		strImageName=strTemp;
	//		strImageName+=strExt;
	//	}

	//	if(strExt==_T("iso"))
	//	{
	//		pImageFileDevice->SetAttributeLong(AttrULongImageFileType, ImageISO);
	//	}
	//	else if(strExt==_T("c2d"))
	//	{
	//		pImageFileDevice->SetAttributeLong(AttrULongImageFileType, ImageWinOnCD);
	//	}
	//	else if(strExt==_T("bin"))
	//	{
	//		pImageFileDevice->SetAttributeLong(AttrULongImageFileType, ImageBINCUE);
	//	}
	//	else
	//	{
	//		Ret=ERR_FAIL;
	//	}
	//	if(Ret==ERR_OK)
	//	{
	//		pImageFileDevice->SetAttributeString(AttrStringImageFileName,(UniChar*)strImageName.c_str());
	//		Ret=pWriteDeviceHandler->AddTargetDevice(pImageFileDevice,NULL);
	//	}
	//}
	//else
	//{
	//	Ret=ERR_FAIL;
	//}
	//pWriteDeviceHandler->Release();
	//pImageFileDevice->Release();
    // End Dragon removing
    return Ret;
}

RetVal CTargetDrives::_SetBufferUnderrun(IProject& Project,bool bEnable)
{
    // Dragon removing
    //if(!&Project)
		return ERR_FAIL;
	//return Project.SetAttributeBool(AttrBoolPerformBUFE, bEnable);
    // End Dragon removing
}
RetVal CTargetDrives::SetBufferUnderrun(bool bEnable)
{
    // Dragon removing
 //   if(m_pDriveList->GetCount()<1)
	//	return ERR_CMD_NO_DEVICE;

	//RetVal Ret=_SetBufferUnderrun(m_DataProject,bEnable);
	//if(IS_OK(Ret))
	//	_SetBufferUnderrun(m_CopierProject,bEnable);
	//if(IS_OK(Ret))
	//	_SetBufferUnderrun(m_ImageProject,bEnable);
    return ERR_OK;
    // End Dragon removing
}

RetVal CTargetDrives::ReloadTargetsFromProject(IProject *pProject)
{
    // Dragon removing
 //   int iCount=-1;
 //   IWriteDeviceHandler* pWriteDeviceHandler=NULL;
	//if (pProject==NULL) 
	//{
	//	pProject = m_pBurnEngine->GetProjectPtr();
	//}

	//m_TargetIDList.RemoveAll();

	//pProject->GetRelatedInterface(IID_IWriteDeviceHandler,0,(void**)&pWriteDeviceHandler);
	//int iNumDevices=GetNumDevices();
	//IAttributeContainer64 *pProjectDevice=NULL;
	//IAttributeContainer64 *pStoredDevice=NULL;
	//long lTargetDevicesCount = pWriteDeviceHandler->GetAttributeLong ( AttrULongDeviceCount );
	//for(int i=0;i<lTargetDevicesCount;i++)
	//{
	//	iCount++;
	//	pWriteDeviceHandler->GetTargetDeviceAt(iCount,&pProjectDevice);
	//	for (int i=0;i<iNumDevices;i++) 
	//	{
	//		pStoredDevice=GetDevicesAt(i);
	//		if (pStoredDevice==pProjectDevice) 
	//		{
	//			AddBurnTarget(i,true);
	//			break;
	//		}
	//		else
	//		{
	//			UniChar* pS=NULL;
	//			if (pProjectDevice) 
	//				pProjectDevice->GetAttributeString(AttrStringImageFileName,&pS);
	//			if (pS) 
	//			{
	//				m_strFilename=pS;
	//				pProjectDevice->Free ( pS );  
	//			}
	//		}
	//	}	
	//	pProjectDevice->Release();
	//} 
	//pWriteDeviceHandler->Release();
    // End Dragon removing
    return S_OK;
}

bool CTargetDrives::IsTargetImage(void)
{
	return m_strFilename.length()!=0;
}

bool CTargetDrives::GetImageFilename(wstring& strImageFilename)
{
	if(m_strFilename.length())
	{
		strImageFilename=m_strFilename;
		return true;
	}
	return false;
}
bool CTargetDrives::IsCompatibleMedia(void)
{
	return m_bIsCompatible;
}
int CTargetDrives::GetNumOfCopies(void)
{
    // Dragon removing
    return 0;
    // End Dragon removing
}
RetVal CTargetDrives::AddVRBurnTarget(wstring strImageName)
{
    // Dragon removing
 //   RemoveTargets((IProject&)m_VRProject);
	//RetVal Ret=AddBurnTarget((IProject&)m_VRProject,strImageName);
	//m_strFilename=strImageName;
	return ERR_OK;
    // End Dragon removing
}

bool CTargetDrives::SuspendTargets(bool bSuspend)
{
    // Dragon removing
 //   m_CriticalSection.Lock();
	//if(bSuspend)
	//{
	//	RemoveTargets(false);
	//}
	//else
	//{
	//	int nNum=GetNumSelectedTargets();
	//	for (int i=0;i<nNum;i++)
	//	{
	//		AddBurnTarget(GetSelectedTargetID(i));
	//	}
	//}
	//m_CriticalSection.Unlock();
    // End Dragon removing
    return false;
}

bool CTargetDrives::DisableMedia(ULONG lListToDisable[],ULONG ulNumItemsList)
{
    // Dragon removing
 //   m_DataProject.SetAttributeLongArray ( AttrULongArrayTargetMediaTypesToDisable, lListToDisable, ulNumItemsList);
	//m_VRProject.SetAttributeLongArray ( AttrULongArrayTargetMediaTypesToDisable, lListToDisable, ulNumItemsList);
    // End Dragon removing
    return true;
}
bool CTargetDrives::CanWriteRaw(void)
{
    // Dragon removing
    return true;
    // End Dragon removing
}

unsigned long CTargetDrives::GetDVDSize(void)
{
	unsigned long targetSize = _GetTargetDVDSize();
	if ( 0 == targetSize)
		targetSize = SINGLE_LAYER_DVD_SIZE_IN_BLOCKS;

	return targetSize;
}

unsigned long CTargetDrives::_GetTargetDVDSize(void)
{
	DWORD nBlockSize=0;
	DWORD iCount=GetNumSelectedTargets();
    // Dragon removing
 //   for(DWORD i=0;i<iCount;i++)
	//{
	//	IDevice *pIDevice=GetDevicesAt(GetSelectedTargetID(i));
	//	if(pIDevice)
	//	{
	//		IDiscPtr spDisc;
	//		pIDevice->GetDiscInfo(&spDisc);
	//		if (spDisc!=NULL) 
	//		{
	//			ULONG ulFreeSpaceOnDisk=0;
	//			DiscMediumType MediumType = (DiscMediumType)spDisc->GetAttributeLong(AttrULongDiscMediumType);

	//			if((MediumType == DiscMediumDVDdashR) ||
	//				(MediumType == DiscMediumDVDplusRDualLayer) ||
	//				(MediumType == DiscMediumDVDplusR))
	//			{
	//				if ( spDisc->GetAttributeLong(AttrULongUsedBlocksOnDisc) == 0)
	//					ulFreeSpaceOnDisk=spDisc->GetAttributeLong(AttrULongAvailableBlocksOnDisc);
	//			}
	//			else if ((MediumType == DiscMediumDVDRAM)			||
	//				(MediumType == DiscMediumDVDdashRW)	||
	//				(MediumType == DiscMediumDVDplusRW))
	//			{
	//				ulFreeSpaceOnDisk=spDisc->GetAttributeLong(AttrULongAvailableBlocksOnDisc);
	//				ulFreeSpaceOnDisk+=spDisc->GetAttributeLong(AttrULongUsedBlocksOnDisc);
	//			}

	//			if (ulFreeSpaceOnDisk && (ulFreeSpaceOnDisk < nBlockSize || nBlockSize==0)) 
	//			{
	//				nBlockSize=ulFreeSpaceOnDisk;
	//			}
	//		}
	//	}
	//}
    // End Dragon removing
    return nBlockSize;
}


bool CTargetDrives::SupportBufferUnderrunProtection(void)
{
	DWORD iCount=GetNumSelectedTargets();
	bool bCanPerformBufferUnderrun=false;
    // Dragon removing
 //   for(DWORD i=0;i<iCount;i++)
	//{
	//	bCanPerformBufferUnderrun=false;
	//	IDevice *pIDevice=GetDevicesAt(GetSelectedTargetID(i));
	//	if(pIDevice)
	//	{
	//		if ( pIDevice->GetAttributeBool ( AttrBoolCanPerformBUFE ) ) bCanPerformBufferUnderrun=true;
	//		// all targets have to support this feature
	//		if(!bCanPerformBufferUnderrun)
	//			break;
	//	}
	//}
    // End Dragon removing
    return bCanPerformBufferUnderrun;
}
bool CTargetDrives::CanTestRawmode(void)
{
	DWORD iCount=GetNumSelectedTargets();
	bool bCanTestRawmode=false;
    // Dragon removing
 //   for(DWORD i=0;i<iCount;i++)
	//{
	//	bCanTestRawmode=true;
	//	IDevice *pIDevice=GetDevicesAt(GetSelectedTargetID(i));
	//	if(pIDevice)
	//	{
	//		if (!pIDevice->GetAttributeBool ( AttrBoolCanWriteDiscAtOnceRawInTest ) ) bCanTestRawmode=false;
	//		// all targets have to support dvds
	//		if(!bCanTestRawmode)
	//			break;
	//	}
	//}
    // End Dragon removing
    return bCanTestRawmode;
}

bool CTargetDrives::IsDVDInSelectedDrive(int id)
{
    // Dragon removing
 //   m_CriticalSection.Lock();
	//IDevice*		pDevice = GetDevicesAt(id);
	//bool bRet=IsDVDDisc(pDevice);
	//m_CriticalSection.Unlock();
    // End Dragon removing
    return true;
}
// retrieves the disc type of the current source or target _disc_ (not image)
int CTargetDrives::GetDiscType(int& nDiscType)
{
    // Dragon removing
 //   IDevice *pDevice;
	//pDevice = GetDevicesAt(GetSelectedTargetID(0));
    // End Dragon removing
    return 0;
}

// // deliver write speeds
ULONG CTargetDrives::GetWriteSpeeds(bool bIsSoureImage,CAtlList<wstring>& ListWriteSpeedStrings,CAtlList<ULONG>& ListWriteSpeedIndex)
{
    // Dragon removing
 //   m_CriticalSection.Lock();
	ULONG lRet;
	//if(bIsSoureImage)
	//{
	//	lRet=_GetWriteSpeeds(ListWriteSpeedStrings,ListWriteSpeedIndex,m_ImageProject);
	//}
	//else
	//{
	//	lRet=_GetWriteSpeeds(ListWriteSpeedStrings,ListWriteSpeedIndex,m_CopierProject);
	//	if (lRet==0)
	//	{
	//		lRet=_GetWriteSpeeds(ListWriteSpeedStrings,ListWriteSpeedIndex,m_DataProject);
	//	}

	//}

	//m_CriticalSection.Unlock();
    // End Dragon removing
    return lRet;
}

static unsigned long GetBlockSize ( unsigned long lReadWriteMode, unsigned long lDefaultBlockSize )
{
    // Dragon removing
 //   switch ( lReadWriteMode ){
	//case	DataAccess:				// Mode-1 and Mode-2/Form-1 data with blocksize of 2048 
	//	return 2048L;
	//	break;
	//case	DataAccessMode2MixTAO:	// Mode-2/Mix-Form data with blocksize of 2332 (write Track At Once only!! No Read)
	//	return 2332L;
	//	break;
	//case	DataAccessMode2Mix:		// Mode-2/Mix-Form data with blocksize of 2336 (generic read  and  write-Disc-At-Once-only)
	//	return 2336L;
	//	break;
	//case	DataAccessRaw:			// Audio and data with blocksize of 2352 
	//	return 2352L;
	//	break;
	//case	DataAccessRawPQ:		// Audio and data with blocksize of 2368 
	//	return 2368L;
	//	break;
	//case	DataAccessRawPW:		// Audio and data with blocksize of 2448 
	//	return 2448L;
	//	break;
	//case	DataAccessPW:
	//	return 96;
	//	break;
	//default:
	//	assert(NULL);
		return 0L;
	//}
    // End Dragon removing
}

// retrieves the required space that must be available on the target location for output to the selected image file type
int CTargetDrives::GetImageSize(unsigned _int64& ui64TargetImageSize,wstring strFolder)
{
	bool ret = false;
	wstring strFilename;
	unsigned long lBlockCount=m_pBurnEngine->SourceDrives->GetSizeInBlocks(strFolder);

	// if we have to transcode we will already have an targetimage size here
	if (ui64TargetImageSize && (ui64TargetImageSize/2048)<lBlockCount)
	{
		return true;
	}
	if(m_pBurnEngine->SourceDrives->IsSourceImage())
	{
		wstring strImageName;
		DWORD dwNumBlocks = 0;
		m_pBurnEngine->SourceDrives->GetImageName(strImageName);
		m_pBurnEngine->SourceDrives->GetDiscImageSize( strImageName, dwNumBlocks, &ui64TargetImageSize);
		return true;
	}

    // Dragon removing
 //   IWriteDeviceHandler* pWriteDeviceHandler=NULL;
	//IProject* pProject = m_pBurnEngine->GetProjectPtr();
	//pProject->GetRelatedInterface(IID_IWriteDeviceHandler,0,(void**)&pWriteDeviceHandler);

	//_ASSERTE(pWriteDeviceHandler!=NULL);
	//IAttributeContainer64 *pDevice = NULL;
	//unsigned long count, index;
	//count = pWriteDeviceHandler->GetAttributeLong( AttrULongDeviceCount);



	//for ( index = 0; !ret && (index < count); index++)
	//{
	//	if ( IS_OK(pWriteDeviceHandler->GetTargetDeviceAt( index, &pDevice)))
	//	{
	//		_ASSERTE(pDevice!=NULL);
	//		DeviceType devicetype = (DeviceType)pDevice->GetAttributeLong( AttrULongDeviceType);
	//		if ( DeviceImageFile == devicetype)
	//		{
	//			DataAccessType	dataAccessType	= (DataAccessType)(pDevice->GetAttributeLong ( AttrULongDataAccessType ));
	//			unsigned long	lBlockSize		= GetBlockSize ( dataAccessType, 2048 );
	//			ui64TargetImageSize = lBlockCount;
	//			ui64TargetImageSize *= lBlockSize;
	//			ret = true;
	//		}
	//		else
	//		{
	//			unsigned long	lBlockSize		= 2048;
	//			ui64TargetImageSize = lBlockCount;
	//			ui64TargetImageSize *= lBlockSize;
	//		}
	//		pDevice->Release();
	//	}
	//}

	//// approximately an Image is about 10 MB bigger then source
	//ui64TargetImageSize += 1024i64*1024i64*10;	
	//pWriteDeviceHandler->Release();
    // End Dragon removing
    return ret;
}

bool CTargetDrives::IsDVDDiscdashRDualLayer()
{
	DWORD iCount=GetNumSelectedTargets();
	bool bIsDVDdashRDualLayer=false;
    // Dragon removing
 //   for(DWORD i=0;i<iCount;i++)
	//{
	//	IDevice *pIDevice=GetDevicesAt(GetSelectedTargetID(i));
	//	if(pIDevice)
	//	{
	//		if(CDrives::IsDVDDiscdashRDualLayer(pIDevice))
	//		{
	//			bIsDVDdashRDualLayer=true;
	//			break;
	//		}
	//	}
	//}
    // End Dragon removing
    return bIsDVDdashRDualLayer;
}
enumDiscType CTargetDrives::_GetDiscInfo(IDevice * pDevice, wstring& strLabel,bool &bLabelFound,bool bNoCompatibilityCheck)
{

	enumDiscType Ret=DiscUnknown;
	int			nSelectionIndex = 0;
	void		*pSelectedItem = NULL;
    // Dragon removing
 //   IDiscPtr	spDiscInfo;
	//wstring		strTitleString;
	//bool		bIsProtected = false;
	//DeviceReserveType	ReserveType;
	//bool				bReservedByThis;
	//WCHAR				*pszOwnerName=NULL;

	//strLabel=_T("");
	//bLabelFound=false;

	//RetVal	Result = 0;


	//Result = pDevice->TestUnitReady();
	//if ( IS_OK(Result) )
	//{

	//	pDevice->GetReserveStatus(&ReserveType, (UniChar**)&pszOwnerName, &bReservedByThis);
	//	pDevice->SetAttributeBool(AttrBoolSingleDestWasReserved, false);
	//	if(ReserveType!=DeviceAvailable)
	//	{
	//		if (pszOwnerName)
	//		{
	//			bLabelFound=true;
	//			strLabel=pszOwnerName;
	//		}

	//	}
	//}
	//else
	//{
	//	pDevice->GetReserveStatus(&ReserveType, (UniChar**)&pszOwnerName, &bReservedByThis);
	//	pDevice->SetAttributeBool(AttrBoolSingleDestWasReserved, true);
	//	if (pszOwnerName)
	//	{
	//		strLabel=pszOwnerName;
	//		bLabelFound=true;
	//	}
	//}

	//pDevice->Reserve(DeviceReservedShortTerm, NULL, 100, NULL, NULL, NULL);

	//if ( IS_OK(Result) ){
	//	Result = pDevice->GetDiscInfo(&spDiscInfo);
	//}

	//if (IS_OK(Result) && (NULL != spDiscInfo)) 
	//{
	//	unsigned long		*pTypeArray = NULL;
	//	unsigned long		ArraySize = 0;
	//	DiscMediumType		DiscType = (DiscMediumType)0;
	//	IProject *pProject=m_pBurnEngine->GetProjectPtr();

	//	pProject->GetAttributeLongArray(AttrULongArraySupportedTargetMediaTypes, &pTypeArray, &ArraySize);

	//	DiscType = (DiscMediumType)(spDiscInfo->GetAttributeLong(AttrULongDiscMediumType));
	//	// let's check if source has media or not.
	//	// if not, then we can display the destination as compatible as we don't know yet what the source
	//	// will be.

	//	IDevice		*pSourceDevice = NULL;
	//	bool bSourceIsDVD = false, bDestIsDVD = false;
	//	bool bSrcHasMedia = false;
	//	bool bIsValidDestMedia = true;
	//	IDisc *pSrcDisc = NULL;
	//	ULONG nIndex = 0;

	//	if(!m_pBurnEngine->SourceDrives->IsSourceImage())
	//	{
	//		pSourceDevice=m_pBurnEngine->SourceDrives->GetActDevice();
	//		if(pSourceDevice)
	//			pSourceDevice->GetDiscInfo(&pSrcDisc);
	//		if (pSrcDisc != NULL)
	//		{
	//			bSrcHasMedia = true;
	//			pSrcDisc->Release();
	//		}
	//	}
	//	if(pSourceDevice)
	//		bSourceIsDVD = m_pBurnEngine->SourceDrives->IsDVDDisc(pSourceDevice);

	//	bDestIsDVD = m_pBurnEngine->TargetDrives->IsDVDDisc(pDevice);

	//	if ( (bSourceIsDVD != bDestIsDVD) ||
	//		(true == bSourceIsDVD) && !bNoCompatibilityCheck)
	//	{
	//		bIsValidDestMedia = false;
	//		while ( (NULL != pTypeArray) && (nIndex < ArraySize) )
	//		{
	//			if (pTypeArray[nIndex] == DiscType)
	//			{
	//				bIsValidDestMedia = true;
	//				break;
	//			}
	//			nIndex++;
	//		}
	//	}

	//	bool m_bDestIsDVDFolder=false,m_bDestIsImageFile=false;
	//	if (bSrcHasMedia == false ||  (true == bIsValidDestMedia))
	//	{
	//		if ( (true == m_bDestIsDVDFolder) || (true == m_bDestIsImageFile) )
	//		{
	//			//							strTitle = m_strDestinationFileName;
	//			//mb2wc( strTitle, m_strDestinationFileName.c_str() );
	//		}
	//		else
	//		{
	//			bool bAudio, bData;
	//			Result = _GetDiscTitleToDisplay( spDiscInfo, strLabel, bAudio, bData);

	//			if (DiscContentBlank == Result)
	//			{
	//				Ret=DiscBlank;
	//			}
	//			else
	//			{
	//				if ( bData)
	//					Ret = DiscRom;
	//				else if ( bAudio)
	//					Ret = DiscAudio;
	//			}
	//		}

	//		DiscMediumType	MediumType = (DiscMediumType)0;
	//		CStringAttr			MediumTypeString;

	//		if(strLabel.length())
	//		{
	//			bLabelFound=true;
	//		}
	//		else
	//		{
	//			MediumType = (DiscMediumType)spDiscInfo->GetAttributeLong(AttrULongDiscMediumType);
	//			if (MediumType != 0)
	//			{
	//				m_pBurnEngine->m_pStringLookup->SetAttributeLong(AttrULongDiscMediumType, MediumType);
	//				MediumTypeString.GetAttributeString(m_pBurnEngine->m_pStringLookup, AttrStringLookedUp);
	//				strLabel += MediumTypeString.c_str();
	//			}
	//		}

	//	}
	//	else
	//	{
	//		Ret=DiscIncompatible;
	//	}
	//}
	//else if (DeviceReservedLongTerm == ReserveType)
	//{
	//	Ret=DeviceReserved;
	//}
	//else
	//{
	//	Ret=NoDisc;
	//}

	//pDevice->Reserve(DeviceAvailable, NULL, 0, NULL, NULL, NULL);
    // End Dragon removing
    return Ret;
}
// looks up which kind of disc is inserted 
enumDiscType CTargetDrives::GetDiscInfo(int id, wstring& strLabel,bool &bLabelFound,bool bNoCompatibilityCheck)
{
    // Dragon removing
 //   IDevice *pDevice = m_pBurnEngine->TargetDrives->GetDevicesAt(id);
	//_ASSERTE(pDevice!=NULL);
	//m_CriticalSection.Lock();
	//enumDiscType eRet=_GetDiscInfo(pDevice,strLabel,bLabelFound,bNoCompatibilityCheck);
	//m_CriticalSection.Unlock();
    return (enumDiscType)0;
    // End Dragon removing
}


int CTargetDrives::ReInitBurnTarget(void)
{
    // Dragon removing
 //   IWriteDeviceHandler* pWriteDeviceHandler=NULL;
	//m_CopierProject.GetRelatedInterface(IID_IWriteDeviceHandler,0,(void**)&pWriteDeviceHandler);
	//if ( pWriteDeviceHandler )
	//{
	//	// Need to use NULL
	//	pWriteDeviceHandler->GetSourceDevice (NULL);
	//}
	//pWriteDeviceHandler->Release();
    // End Dragon removing
    return 0;
}*/