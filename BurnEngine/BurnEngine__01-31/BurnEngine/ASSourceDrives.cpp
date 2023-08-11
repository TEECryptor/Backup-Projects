//-----------------------------------------------
//	ASSourceDrives.cpp
//	The implement file for CASSourceDrives
//-----------------------------------------------
#include "StdAfx.h"
#include "ASSourceDrives.h"
//#include "CommonDef.h"
//#include "FileServerDefs.h"
//-----------------------------------------------
//	Name:CASSourceDrives
//	Introduce:Construction
//-----------------------------------------------
CASSourceDrives::CASSourceDrives(CASDriveManager* pDriveManager)
: CASDrives(pDriveManager)
, m_iActiveID(-1)
{	
	lstrcpy(m_szImageFile, _T("\0"));
}
//-----------------------------------------------
//	Name:~CASSourceDrives
//	Introduce:Distruction
//-----------------------------------------------
CASSourceDrives::~CASSourceDrives(void)
{
}
//-----------------------------------------------




//-----------------------------------------------
//	Name:AddBurnSource
//	Introduce:Add which drive for burning source
//-----------------------------------------------
LONG CASSourceDrives::AddBurnSource(int iDriveID)
{
	LONG lRes = AS_StorageError_NotFound;
	if(m_iActiveID == iDriveID)
	{
		lRes = S_OK;
	}
	else
	{
		lRes = AS_StorageError_NotFound;
		for(ASDriveArrayPtr::iterator it = m_lstASDrive.begin(); it != m_lstASDrive.end(); it++)
		{
			CASDrive* pDrive = reinterpret_cast<CASDrive*>(*it);
			if(NULL != pDrive && (iDriveID == pDrive->GetID()))
			{
				lRes = S_OK;
				m_iActiveID = iDriveID;
				lstrcpy(m_szImageFile, _T("\0"));
				break;
			}
		}
	}

	return lRes;
}
//-----------------------------------------------
//	Name:AddBurnSource
//	Introduce:Add an image file as burning source
//-----------------------------------------------
LONG CASSourceDrives::AddBurnSource(LPCTSTR lpszImageFile)
{	
	if(NULL == lpszImageFile || lstrlen(lpszImageFile) == 0)
		return S_FALSE;

	lstrcpy(m_szImageFile, lpszImageFile);
	m_iActiveID = -1;

	return S_OK;
}
//-----------------------------------------------


//-----------------------------------------------
//	Name:GetActiveDevice
//	Introduce:Return current active source device
//-----------------------------------------------
CASDrive* CASSourceDrives::GetActiveDevice()
{
	CASDrive* pActiveDirve = NULL;
	if(-1 == m_iActiveID)
		pActiveDirve = m_pDriveManager->AddImageDrive(m_szImageFile);
	else
		pActiveDirve = GetDriveByID(m_iActiveID);

	return pActiveDirve;
}
//-----------------------------------------------


/*
// set the source to be copied from
RetVal CSourceDrives::AddBurnSource(int id)
{
    // Dragon removing
 //   m_CriticalSection.Lock();
	RetVal Ret=0;
	//if (id!=m_ActID) 
	//{
	//	m_strSourceFile.clear();
	//	m_ActID=id;
	//	
	//	if ( m_ActID != -1)
	//	{
	//		Ret=_AddBurnSource(m_CopierProject,id);
	//	}
	//}

	//m_CriticalSection.Unlock();
    // End Dragon removing
    return Ret; 
}

// set the source to be copied from
RetVal CSourceDrives::AddBurnSource(wstring& strFileName)
{
    // Dragon removing
 //   m_CriticalSection.Lock();
	RetVal Ret;
	//Ret=m_ImageProject.SetAttributeString(AttrStringImageFileName, (UniChar*)strFileName.data());
	//m_strSourceFile=strFileName;
	//m_ActID = -1;
	//m_CriticalSection.Unlock();
    // End Dragon removing
    return Ret;
}

// set the source to be copied from
RetVal CSourceDrives::_AddBurnSource(IProject& Project,int id)
{
	RetVal Ret;
    // Dragon removing
 //   IWriteDeviceHandler* pWriteDeviceHandler=NULL;
	//Project.GetRelatedInterface(IID_IWriteDeviceHandler,0,(void**)&pWriteDeviceHandler);
	//_ASSERTE(pWriteDeviceHandler!=NULL);

	//IDevice *pDevice=GetDevicesAt(id);
	//Ret=pWriteDeviceHandler->SetSourceDevice(pDevice);
	//Ret=pWriteDeviceHandler->SetSourceDevice(pDevice);

	//pWriteDeviceHandler->Release();
    // End Dragon removing
    return Ret;
}

// //true if source is an image file
bool CSourceDrives::IsSourceImage(void)
{
	if ( (m_ActID == -1) && (m_strSourceFile.length()>0))
	{
		return true;
	}
	return false;
}

bool CSourceDrives::GetImageName(wstring& strImageFileName)
{
	if(m_strSourceFile.length()>0)
	{
		strImageFileName=m_strSourceFile;
		return true;
	}
	return false;
}

bool CSourceDrives::SetCacheMode(long Cachmode,wstring &strCachePath)
{
    // Dragon removing
 //   _SetCacheMode(m_DataProject,Cachmode,strCachePath);
	//_SetCacheMode(m_CopierProject,Cachmode,strCachePath);
    // End Dragon removing
    return true;
}
bool CSourceDrives::_SetCacheMode(IProject& Project,long Cachmode,wstring &strCachePath)
{
    // Dragon removing
    //// Set cache settings
	//switch (Cachmode)
	//{
	//case 0:
	//	Project.SetAttributeLong(AttrULongCacheSettings, CacheAlways);
	//	break;

	//case 2:
	//	Project.SetAttributeLong(AttrULongCacheSettings, CacheAdjustWriteSpeedToMatchReadSpeed);
	//	break;

	//case 3:
	//	Project.SetAttributeLong(AttrULongCacheSettings, CacheNever);
	//	break;

	//case 1:
	//default:
	//	Project.SetAttributeLong(AttrULongCacheSettings, CacheIfReadSpeedIsSlow);
	//	break;
	//}
	//if (strCachePath.length()) 
	//{
	//	if(strCachePath[strCachePath.length()-1]!='\\')
	//	{
	//		strCachePath+=_T("\\");
	//	}
	//	Project.SetAttributeString(AttrStringTempPath, (UniChar*)strCachePath.c_str());
	//}
    // End Dragon removing
    return true;
}

long CSourceDrives::GetCacheMode(IProject& Project,LPTSTR strCachePath)
{
	long iRet=0;
	
    // Dragon removing
 //   // Set cache settings
	//switch (Project.GetAttributeLong(AttrULongCacheSettings))
	//{
	//case CacheAlways:
	//	iRet=0;
	//	break;

	//case CacheAdjustWriteSpeedToMatchReadSpeed:
	//	iRet=2;
	//	break;

	//case CacheNever:
	//	iRet=3;
	//	break;

	//case CacheIfReadSpeedIsSlow:
	//default:
	//	iRet=1;
	//	break;
	//}
	//UniChar* pS = NULL;
	//Project.GetAttributeString(AttrStringTempPath,&pS);
	//assert(pS);
	//_tcscpy(strCachePath,pS);
	//Project.Free ( pS ); 
    // End Dragon removing
    return iRet;
}
bool CSourceDrives::GetDeviceName(int id,wstring& strDriveLetter, wstring& strVendorName, wstring& strProductName)
{
    // Dragon removing
    return CDrives::GetDeviceName(id,strDriveLetter,strVendorName, strProductName);
    // End Dragon removing
}

bool CSourceDrives::GetDeviceName(wstring& strDriveLetter, wstring& strVendorName, wstring& strProductName)
{
	if ( m_ActID == -1)
	{
		strDriveLetter = strVendorName = strProductName = L"";
		return S_OK;
	}
    // Dragon removing
    return  CDrives::GetDeviceName(m_ActID,strDriveLetter,strVendorName, strProductName);
    // End Dragon removing
}
IDevice * CSourceDrives::GetActDevice(void)
{
  if(m_ActID==-1)
		return NULL;
  // Dragon removing
  return GetDevicesAt(m_ActID);
  // End Dragon removing
}
bool CSourceDrives::GetActDevice(wstring & strPath)
{
	wstring strVendorName,strProductName;
	if(!GetImageName(strPath))
	{
		return GetDeviceName(strPath,strVendorName,strProductName);
	}
	return true;
}


ULONG CSourceDrives::GetReadSpeeds(CAtlList<wstring>& ListReadSpeedStrings,CAtlList<ULONG>& ListReadSpeedIndex)
{
    // Dragon removing
 //   m_CriticalSection.Lock();
	//ULONG Ret=0;
	//// Get the supported read speeds:
	//int				nItemIndex = 0;
	//unsigned long	speedToReturn = 0;
	//ULONG			*pSpeeds = NULL;
	ULONG			nCount=0;
	//ULONG			lKBperXFactor = 0;

	//IDevice *pDevice=GetActDevice();

	//if(pDevice)
	//{
	//	lKBperXFactor = pDevice->GetAttributeLong(AttrULongSpeedKBPerXFactor);
	//	pDevice->GetAttributeLongArray(AttrULongArraySupportedDataReadSpeedsInKBPS,&pSpeeds, &Ret);
	//}
	//
	//if (pSpeeds)
	//{
	//	for (nCount = 0; nCount < Ret; ++nCount)
	//	{
	//		// Get the speed from the Dragon Resources DLL.
	//		WCHAR *pBuffer;
	//		m_StringLookup.SetAttributeLong		(AttrULongSpeedKBPerXFactor, lKBperXFactor);
	//		m_StringLookup.SetAttributeLong(AttrULongDataReadSpeedInKBPS, pSpeeds[nCount]);
	//		m_StringLookup.GetAttributeString	(AttrStringLookedUp, (UniChar**)&pBuffer);				
	//		ListReadSpeedStrings.AddTail(pBuffer);
	//		ListReadSpeedIndex.AddTail(pSpeeds[nCount]);
	//		m_StringLookup.Free(pBuffer);
	//	}
	//	m_DeviceDriverMain.Free(pSpeeds);
	//}

	//m_CriticalSection.Unlock();
    // End Dragon removing
    return nCount;
}

bool CSourceDrives::ApplyReadSpeed(int ReadSpeedMode, ULONG ReadSpeed)
{
    // Dragon removing
 //   m_CriticalSection.Lock();
	//if(_ApplyReadSpeed(m_DataProject,ReadSpeedMode,ReadSpeed))
	//	_ApplyReadSpeed(m_CopierProject,ReadSpeedMode,ReadSpeed);
	//m_CriticalSection.Unlock();
    // End Dragon removing
    return true;
}
bool CSourceDrives::_ApplyReadSpeed(IProject& Project,int ReadSpeedMode, ULONG ReadSpeed)
{
    // Dragon removing
 //   switch(ReadSpeedMode) 
	//{
	//case SET_READ_SPEED:
	//	{
	//		Project.SetAttributeLong(AttrULongDataReadSpeedInKBPS, ReadSpeed);
	//		Project.SetAttributeLong(AttrULongAudioReadSpeedInKBPS, ReadSpeed);
	//		Project.SetAttributeBool(AttrBoolPerformReadSpeedTest, false);
	//	}
	//	break;

	//case PERFORM_READ_SPEED_TEST:
	//	
	//	Project.SetAttributeLong(AttrULongDetectionType, DetectReadableLengthOnTheFly);
	//	Project.SetAttributeLong(AttrULongDetectionType, DetectReadableLengthOnTheFly);
	//	Project.SetAttributeLong(AttrULongDataReadSpeedInKBPS, MAX_SPEED_VALUE);
	//	Project.SetAttributeLong(AttrULongAudioReadSpeedInKBPS, MAX_SPEED_VALUE);
	//	Project.SetAttributeBool(AttrBoolPerformReadSpeedTest, true);
	//	break;

	//case DEFAULT_READ_SPEED_BEHAVIOR:
	//default:
	//	Project.SetAttributeLong(AttrULongDataReadSpeedInKBPS, MAX_SPEED_VALUE);
	//	Project.SetAttributeLong(AttrULongAudioReadSpeedInKBPS, MAX_SPEED_VALUE);
	//	Project.SetAttributeBool(AttrBoolPerformReadSpeedTest, false);
	//	break;
	//}
    // End Dragon removing
    return true;
}

int CSourceDrives::GetActDeviceID(void)
{
	return m_ActID;
}

wstring CSourceDrives::GetVolumeLabel(void)
{
	IDevice * spSourceDevice = (IDevice *)GetActDevice();
	// Set the volume's label:
	WCHAR wszVolLabel[MAX_PATH] = L"image";
	// Begin Adding files:
    // Dragon removing
 //   CStringAttr		strDriveLetter(spSourceDevice, AttrStringDriveLetter);
	//if (strDriveLetter.size() == 0)
	//{
	//	printf("Unable to get drive letter from source device. Exiting.\n");
	//	return wstring(_T(""));
	//}

	//WCHAR driveString[10];
	//wcscpy(driveString, strDriveLetter.c_str());
	//wcscat(driveString, L":");

	//WCHAR *volLabel = NULL;
	//IDisc *pDisc = NULL;
	//spSourceDevice->GetDiscInfo(&pDisc);
	//if (pDisc)
	//{
	//	pDisc->GetAttributeString(AttrStringTitle, (UniChar**)&volLabel);
	//	if ( volLabel != NULL)
	//	{
	//		wcscpy( wszVolLabel, volLabel);
	//		pDisc->Free(volLabel);
	//	}
	//	pDisc->Release();
	//}
	//else
	//{
	//	wcscat (driveString, L"\\");
	//	GetVolumeInformation( driveString, wszVolLabel,  MAX_PATH, NULL, NULL, NULL, NULL, 0);
	//}
	//PathCleanupSpec(_T("C:\\"),wszVolLabel);
    // End Dragon removing
    return wstring(wszVolLabel);
}

bool CSourceDrives::IsImageRawmode(void)
{
	bool bRet=false;
	if(IsSourceImage())
	{
        // Dragon removing
        //bRet=m_ImageProject.GetAttributeBool(AttrBoolIsImageInRawMode);
        // End Dragon removing
    }
	return bRet;
}

DWORD CSourceDrives::GetSizeInBlocks(wstring strFolder)
{
	DWORD BlockSize=0;
	IDevice*		pDevice = GetActDevice();
	if (IsSourceImage()) 
	{
		wstring strFilename;
		GetImageName(strFilename);
		m_pBurnEngine->SourceDrives->GetDiscImageSize( strFilename, BlockSize, NULL);
	}
	// if source is a folder
	else if ( strFolder.length() > 0)
	{
		HANDLE SourceDir=NULL;
		WIN32_FIND_DATA FileData;
		wstring StartFolder=strFolder;
		StartFolder += _T("\\*.*");
		BOOL res = TRUE;
		SourceDir=FindFirstFile(StartFolder.c_str(),&FileData);
		if(SourceDir!=INVALID_HANDLE_VALUE)
		{
			while(res)
			{
				ULONGLONG FileSize=FileData.nFileSizeHigh<<32;
				FileSize+=FileData.nFileSizeLow;
				BlockSize+=(DWORD)(FileSize/2048);
				if(_tcscmp(FileData.cFileName,_T("."))!=0 && _tcscmp(FileData.cFileName,_T(".."))!=0)
				{
					if(FileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
					{
						wstring NextDir;
						NextDir=strFolder;
						NextDir+=_T("\\");
						NextDir+=FileData.cFileName;
						BlockSize+=GetSizeInBlocks(NextDir);
					}
				}
				res = FindNextFile(SourceDir,&FileData);
			}
			FindClose( SourceDir);
		}
	}
	// if a drive is selected
	else if(pDevice)
	{
        // Dragon removing
  //      IDiscPtr spDisc = NULL;
		//pDevice->GetDiscInfo(&spDisc);
		//if (spDisc)
		//{
		//	BlockSize=spDisc->GetAttributeLong(AttrULongUsedBlocksOnDisc);
		//}
        // End Dragon removing
    }
	
	
	return BlockSize;

}

void CSourceDrives::ForceNoCache(void)
{
	wstring strCachePath;
	SetCacheMode(3,strCachePath);
}


bool CSourceDrives::IsMultiBorderDVD(void)
{
	// Check for the various DVD options:
	unsigned long		SourceMediumType = 0;
	unsigned long		BorderCount = 0;
    // Dragon removing
 //   IDiscPtr		spDiscInfo;
	//IDevice*		pDevice = GetActDevice();

	////	bool				bStrictMediaCheck = false;
	//bool				bIsSourceDVD = IsDVDDisc(pDevice);

	//if(!bIsSourceDVD)
	//	return false;

	//pDevice->GetDiscInfo(&spDiscInfo);
	//_ASSERTE(NULL != spDiscInfo);

	//SourceMediumType = spDiscInfo->GetAttributeLong(AttrULongDiscMediumType);

	//// get the border/session count:
	//BorderCount = spDiscInfo->GetAttributeLong(AttrULongSessionCount);
	//ISessionPtr	spSession;
	//spDiscInfo->GetSessionAt(BorderCount - 1, &spSession);
	//if (NULL != spSession)
	//{
	//	int		NumberOfTracks = 0;

	//	NumberOfTracks = spSession->GetAttributeLong(AttrULongTrackCount);

	//	if (0 == NumberOfTracks)
	//	{
	//		BorderCount--;
	//	}
	//}
	//if(BorderCount>1)
	//	return true;
    // End Dragon removing
    return false;
}
int CSourceDrives::GetDVDTrackCount(void)
{
	// Check for the various DVD options:
	int		NumberOfTracks = 0;
	unsigned long		SourceMediumType = 0;
	unsigned long		BorderCount = 0;
    // Dragon removing
 //   IDiscPtr		spDiscInfo;
	//IDevice*		pDevice = GetActDevice();

	////	bool				bStrictMediaCheck = false;
	//bool				bIsSourceDVD = IsDVDDisc(pDevice);

	//if(!bIsSourceDVD)
	//	return false;

	//pDevice->GetDiscInfo(&spDiscInfo);
	//_ASSERTE(NULL != spDiscInfo);

	//SourceMediumType = spDiscInfo->GetAttributeLong(AttrULongDiscMediumType);

	//// get the border/session count:
	//BorderCount = spDiscInfo->GetAttributeLong(AttrULongSessionCount);
	//ISessionPtr	spSession;
	//spDiscInfo->GetSessionAt(BorderCount - 1, &spSession);
	//if (NULL != spSession)
	//{
	//	NumberOfTracks = spSession->GetAttributeLong(AttrULongTrackCount);
	//}
    // End Dragon removing
    return NumberOfTracks;
}
bool CSourceDrives::IsDVDInSelectedDrive(void)
{
    // Dragon removing
 //   m_CriticalSection.Lock();
	//IDevice*		pDevice = GetActDevice();
	//bool bRet=IsDVDDisc(pDevice);
	//m_CriticalSection.Unlock();
    return bRet;
    // End Dragon removing
}

bool CSourceDrives::DoesSupportAudioRawRead()
{
	bool		bReturnValue = false;
	bool		bIsDVD = false;
	IDevice		*pSourceDevice = NULL;
    // Dragon removing
 //   IDiscPtr	spDiscInfo;

	//if (!IsSourceImage())
	//{
	//	pSourceDevice=GetActDevice();
	//	if(pSourceDevice)
	//	{
	//		bIsDVD = IsDVDDisc(pSourceDevice);
	//		if (true != bIsDVD)
	//		{
	//			bReturnValue = pSourceDevice->GetAttributeBool(AttrBoolCanReadAudioRawPW);
	//		}
	//	}
	//}
    // End Dragon removing
    return bReturnValue;
}

bool CSourceDrives::DoesSupportDataRawRead()
{
	bool		bReturnValue = false;
	bool		bIsDVD = false;
	IDevice		*pSourceDevice = NULL;
    // Dragon removing
 //   IDiscPtr	spDiscInfo;

	//if (!IsSourceImage())
	//{
	//	pSourceDevice=GetActDevice();
	//	if(pSourceDevice)
	//	{
	//		bIsDVD = IsDVDDisc(pSourceDevice);
	//		if (true != bIsDVD)
	//		{
	//			bReturnValue = pSourceDevice->GetAttributeBool(AttrBoolCanReadDataRawPW);
	//		}
	//	}
	//}
    // End Dragon removing
    return bReturnValue;
}

bool CSourceDrives::IsDirectCDInSelectedDrive(void)
{
	bool			bReturnValue = false;
	IDevice*		pDevice = GetActDevice();
    // Dragon removing
 //   IDiscPtr		spDisc = NULL;
	//ISession		*pLastRecordedSession;
	//unsigned long	SessionStart = 0;
	//ITrackPtr		spTrack;
	//const int		BufferSize = 2048 * 1;
	//BYTE			pBuffer[BufferSize];
	//RetVal			Result = ERR_OK;;
	//DWORD			NamedBlockOffset = 0;
	//m_bDiscPartiallyWritten = false;

	//if(!pDevice)
	//{
	//	return false;
	//}
	//pDevice->GetDiscInfo(&spDisc);

	//if (NULL == spDisc)
	//{
	//	DeviceReserveType	ReserveType;
	//	bool				bReservedByThis;
	//	WCHAR				*pszOwnerName;

	//	RetVal	Result = 0;

	//	pDevice->GetReserveStatus(&ReserveType, (UniChar**)&pszOwnerName, &bReservedByThis);

	//	if (DeviceReservedLongTerm == ReserveType && !_tcscmp(pszOwnerName,D2D_NAME))
	//		return true;
	//	else
	//		return false;
	//}

	//if ( spDisc->GetAttributeLong ( AttrULongAudioTrackCount) ||
	//	( 0==spDisc->GetAttributeLong ( AttrULongMode2XATrackCount ) &&
	//	0==spDisc->GetAttributeLong ( AttrULongMode1TrackCount) ) ){
	//		//
	//		//	Direct CD discs have no Audio and are always Mode1.
	//		return bReturnValue;
	//	}

	//	long MediumType = spDisc->GetAttributeLong(AttrULongDiscMediumType);
	//	if ( (MediumType == DiscMediumDVDdashRW) || (MediumType == DiscMediumDVDplusRW) )
	//	{
	//		return bReturnValue;
	//	}

	//	if ( (MediumType == DiscMediumDVDdashR) || (MediumType == DiscMediumDVDplusR) )
	//	{
	//		// if the last Session with tracks is closed, then just ignore the whole thing:
	//		if (_IsDiscOrLastSessionClosed(spDisc) == true)
	//		{
	//			return bReturnValue;
	//		}
	//	}

	//	pLastRecordedSession = GetLastRecordedDataSession(spDisc);

	//	if (NULL != pLastRecordedSession)
	//	{
	//		SessionStart = pLastRecordedSession->GetAttributeLong(AttrULongAddress);

	//		Result = pLastRecordedSession->GetTrackAt(0, &spTrack);

	//		if ( IS_OK(Result) )
	//		{
	//			DataMode mode = (DataMode)spTrack->GetAttributeLong(AttrULongDataMode);
	//			_ASSERTE(DataModeAudio!=mode);

	//			TrackStatus Status = (TrackStatus)spTrack->GetAttributeLong(AttrULongTrackStatus);

	//			// if the first track in the last recorded session is reserved or partially written, this is
	//			// most likely an open packet written disc, so reading blocks does not make sense here
	//			if ( Status == TrackPartiallyWritten || Status == TrackEmptyReserved)
	//			{
	//				m_bDiscPartiallyWritten = true;

	//				CStringAttr		strDriveLetter(pDevice, AttrStringDriveLetter);
	//				if (strDriveLetter.size() > 0)
	//				{
	//					wstring rootPath = strDriveLetter + L":\\";

	//					DWORD dwFlags = 0;
	//					if ( GetVolumeInformation( rootPath.c_str(), NULL, 0, NULL, NULL, &dwFlags, NULL, 0))
	//					{
	//						// accessing the volume information succeeded. This shall suffice as a criteria
	//						// to copy the disc via file system
	//						bReturnValue = true;
	//					}
	//				}
	//			}
	//			else
	//			{
	//				Result = pDevice->BeginRead ( mode, DataAccess );

	//				if ( IS_OK(Result) )
	//				{
	//					// Read block 256 to get the offset.
	//					Result = pDevice->Read(SessionStart + 256, 1, pBuffer);

	//					if ( IS_OK(Result) ) 
	//					{

	//						// Get the offset at 0x14 (20), then read the block it indicates:
	//						NamedBlockOffset = *((DWORD*)&pBuffer[0x14]);

	//						if ( NamedBlockOffset < spDisc->GetAttributeLong ( AttrULongUsedBlocksOnDisc ) ){

	//							Result = pDevice->Read(SessionStart + NamedBlockOffset, 1, pBuffer);

	//							if ( IS_OK(Result) ) {

	//								// Now check the block to see if "Adaptec DirectCD" is at offset 0x186 (390).
	//								if (0 == strcmp("Adaptec DirectCD", (char *)&pBuffer[0x186]) )
	//								{
	//									bReturnValue = true;
	//								}
	//								else
	//								{
	//									bReturnValue = false;
	//								}
	//							}
	//						}
	//					}
	//					pDevice->EndRead();
	//				}
	//			}
	//		}

	//		pLastRecordedSession->Release();
	//	}

        // End Dragon removing
        return bReturnValue;
}

bool CSourceDrives::IsDiscPartiallyWritten(void)
{
	return m_bDiscPartiallyWritten;
}
int CSourceDrives::GetDiscType(int& nDiscType)
{
	IDevice *pDevice;
	pDevice = GetActDevice();
    // Dragon removing
    return CDrives::GetDiscType(pDevice,nDiscType);
    // End Dragon removing
}

bool CSourceDrives::IsReservedByD2D(void)
{
	IDevice*		pDevice = GetActDevice();
    // Dragon removing
 //   IDiscPtr		spDisc = NULL;

	//if(!pDevice)
	//{
	//	return false;
	//}
	//DeviceReserveType	ReserveType;
	//bool				bReservedByThis;
	//WCHAR				*pszOwnerName;

	//RetVal	Result = 0;

	//pDevice->GetReserveStatus(&ReserveType, (UniChar**)&pszOwnerName, &bReservedByThis);

	//if (DeviceReservedLongTerm == ReserveType && !_tcscmp(pszOwnerName,D2D_NAME))
	//	return true;

    // End Dragon removing
    return false;
}

RetVal CSourceDrives::GetDiscParameters(wstring & strLabel , int & nNumTracks, int & nDiscType, DWORD & dwTotalBlocks, wstring & wstrDescription, DWORD& dwDataSizeInMB, DWORD& dwAudioPlaytime, int& nNumAudioTracks)
{
	RetVal rRet;
	if(IsSourceImage())
	{
		wstring strImageFileName;
		GetImageName(strImageFileName);
		rRet=_GetDiscImageParameters(strImageFileName,strLabel ,nNumTracks,nDiscType,dwTotalBlocks,
			wstrDescription,dwDataSizeInMB,dwAudioPlaytime, nNumAudioTracks);
	}
	else if ( GetActDeviceID() != -1)
	{
		rRet=_GetDiscParameters(strLabel ,nNumTracks,nDiscType,dwTotalBlocks,
			wstrDescription,dwDataSizeInMB,dwAudioPlaytime, nNumAudioTracks);
	}
	else
		rRet = S_FALSE;

	return rRet;
}

// retrieves basic disc information parameters for display in summary text field
RetVal CSourceDrives::_GetDiscParameters(wstring & strLabel , int & nNumTracks, int & nDiscType, DWORD & dwTotalBlocks,
									 wstring& strDescription, DWORD& dwDataSizeInMB, DWORD& dwAudioPlaytime, int& nNumAudioTracks)
{
	// M = dwPlaytime /(75*60)
	// S = dwPlaytime - (M*60) % 75
	// (F = dwPlaytime % 75) xx min yy sec

	IDevice *pDevice = GetActDevice();
	if ( pDevice == NULL)
		return S_FALSE;

	_bstr_t strTypeStringRef;

	RetVal ret = GetDiscType( nDiscType);
	if ( !SUCCEEDED(ret ))
		return ret;

    // Dragon removing
 //   m_CriticalSection.Lock();
	//_ASSERTE(pDevice!=NULL);

	//IDisc *pIDisc = NULL;
	//pDevice->GetDiscInfo( &pIDisc);
	//nNumTracks = nNumAudioTracks = 0;

	//dwTotalBlocks = dwAudioPlaytime = dwDataSizeInMB = 0;
	//DWORD dwAudioStart = 0;
	//strLabel.empty();
	//wstring csLetter;

	//if ( pIDisc == NULL)
	//{
	//	ret = S_FALSE;
	//}
	//else if ( pIDisc->GetAttributeLong ( AttrULongDiscLogicalStructureType ) == DiscLogicalStructureBlank)
	//{
	//	// logical structure type empty, so we can spare evaluating the rest
	//	ret = S_OK;
	//}
	//else
	//{
	//	WCHAR *volLabel = NULL;
	//	pIDisc->GetAttributeString(AttrStringTitle, (UniChar**)&volLabel);
	//	if ( volLabel)
	//	{
	//		strLabel = volLabel;
	//		pIDisc->Free(volLabel);
	//	}

	//	WCHAR *pS = NULL;
	//	pDevice->GetAttributeString(AttrStringDriveLetter,(UniChar**)&pS);
	//	csLetter = pS;
	//	if ( pS ) 
	//		pDevice->Free ( pS ); 
	//	pS = NULL;

	//	nNumTracks = pIDisc->GetAttributeLong ( AttrULongTrackCount );
	//	dwTotalBlocks = pIDisc->GetAttributeLong ( AttrULongUsedBlocksOnDisc );

	//	unsigned char*	pData = NULL;
	//	unsigned long	lDataLength = 0;
	//	boolean fHasData = false;			// has a data track (mode 1 or XA)
	//	boolean fHasAudio = false;			// has an audio track
	//	boolean fHasDataInLastSession = false;	// has a data track in the last session
	//	boolean fDataIsFirst = false;		// the data track is the first in the last session

	//	unsigned long lSessionCount	= pIDisc->GetAttributeLong ( AttrULongSessionCount );

	//	pS = NULL;
	//	pIDisc->GetAttributeString( AttrStringTitle, (UniChar**)&pS);
	//	if ( pS != NULL)
	//	{
	//		strDescription = pS;
	//		pIDisc->Free( pS);
	//	}

	//	for ( unsigned long lSession = 0; lSession < lSessionCount; lSession++)
	//	{
	//		ISession *pISession = NULL;
	//		pIDisc->GetSessionAt( lSession, &pISession);
	//		boolean fNewSession = true;

	//		if ( pISession != NULL)
	//		{
	//			unsigned long lTrackCount	= pISession->GetAttributeLong ( AttrULongTrackCount );
	//			if ( lTrackCount)				// only reset this flag if the session is not empty
	//				fHasDataInLastSession = false;

	//			for ( unsigned long lTrack = 0; lTrack < lTrackCount; lTrack++)
	//			{
	//				ITrack *pITrack = NULL;
	//				pISession->GetTrackAt( lTrack, &pITrack);
	//				if ( pITrack != NULL)
	//				{
	//					switch ( pITrack->GetAttributeLong ( AttrULongDataMode ) )
	//					{
	//					case	DataModeAudio:
	//						// if returning info about an audio disc, only count the audio tracks
	//						if ( !fHasAudio)	// first occurence of an audio track, set track count to zero
	//						{
	//							nNumAudioTracks = 0;
	//							dwAudioStart = pITrack->GetAttributeLong( AttrULongAddress);
	//						}

	//						nNumAudioTracks++;		// increment audio track count

	//						fHasAudio = true;
	//						// always save the track end position of the last track
	//						dwAudioPlaytime = pITrack->GetAttributeLong( AttrULongAddress)
	//							+ pITrack->GetAttributeLong( AttrULongLength);
	//						break;

	//					case	DataMode2XA:
	//					case	DataMode1:
	//						fHasData = fHasDataInLastSession = true;
	//						if ( fNewSession)
	//							fDataIsFirst = true;
	//						break;

	//					default:
	//						break;

	//					}
	//					pITrack->Release();
	//				}

	//				fNewSession = false;
	//			}
	//			pISession->Release();
	//		}
	//	}

	//	if ( fHasDataInLastSession) 
	//	{
	//		wstring filename;
	//		filename = csLetter + _T(":\\");
	//		ULARGE_INTEGER uliNumBytes = {0};
	//		if ( GetDiskFreeSpaceEx( filename.c_str(), NULL, &uliNumBytes, NULL))
	//			dwDataSizeInMB = (DWORD)((uliNumBytes.QuadPart / (1024i64*1024i64))) + 1;
	//	}
	//	pIDisc->Release();
	//	dwAudioPlaytime -= dwAudioStart;
	//}

	//m_CriticalSection.Unlock();
    // End Dragon removing
    return ret;
}

// retrieves size of disc image in blocks
RetVal CSourceDrives::GetDiscImageSize( wstring filename , unsigned long & dwTotalBlocks, unsigned _int64 * pnEstimatedFileSize)
{
	RetVal ret = S_OK;

	if( m_pFileServer == NULL)
	{
		CoCreateInstance(FileServerLib::CLSID_FileServer, NULL, CLSCTX_INPROC_SERVER, FileServerLib::IID_IFileServer, (void **)&m_pFileServer);
	}
	dwTotalBlocks = 0;

	if ( pnEstimatedFileSize != NULL)
		*pnEstimatedFileSize = 0;

	if( m_pFileServer == NULL)
		return S_FALSE;

	FileServerLib::IDiscImage *pDiscImage = NULL;
	m_pFileServer->OpenDiscImage( filename.c_str(), &pDiscImage);

	if ( pDiscImage == NULL)
		return S_FALSE;

	unsigned long val;
	if ( SUCCEEDED( pDiscImage->GetNumberOfBlocks( &val)))
		dwTotalBlocks = val;

	if ( pnEstimatedFileSize != NULL)
	{
		int nLoopTarget = 99;

		if ( pDiscImage->GetNumberOfTracks( &val))
			nLoopTarget = val;	

		for ( int i = 1; i <= nLoopTarget; i++)
		{
			FileServerLib::IDiscImageTrack *pTrack = NULL;
			if ( SUCCEEDED( pDiscImage->GetTrack( i, &pTrack)) && pTrack != NULL)
			{
				DWORD dwBlockSize = 0, dwNumBlocks = 0;
				pTrack->GetLength( &dwNumBlocks);
				pTrack->GetBlockSize( &dwBlockSize);

				*pnEstimatedFileSize += ((unsigned _int64) dwNumBlocks * (unsigned _int64) dwBlockSize);

				pTrack->Release();
			}
		}
	}


	pDiscImage->Release();

	return ret;
}

// retrieves basic disc image information parameters for display in summary text field
RetVal CSourceDrives::_GetDiscImageParameters( wstring filename , wstring & strLabel, int & nNumTracks, int & nDiscType, unsigned long & dwTotalBlocks,
										  wstring& strDescription, DWORD& dwDataSizeInMB, DWORD& dwAudioPlaytime,
										  int& nNumAudioTracks)
{
	RetVal ret = S_OK;

	if( m_pFileServer == NULL)
	{
		CoCreateInstance(FileServerLib::CLSID_FileServer, NULL, CLSCTX_INPROC_SERVER, FileServerLib::IID_IFileServer, (void **)&m_pFileServer);
	}
	nNumTracks = nNumAudioTracks = 0;
	nDiscType = DI_TYPE_UNKNOWN; 
	dwTotalBlocks = dwAudioPlaytime = dwDataSizeInMB = 0;
	DWORD dwAudioStart = 0;
	strLabel.empty();

	if( m_pFileServer == NULL)
		return S_FALSE;

	FileServerLib::IDiscImage *pDiscImage = NULL;
	m_pFileServer->OpenDiscImage( filename.c_str(), &pDiscImage);

	if ( pDiscImage == NULL)
		return S_FALSE;

	unsigned long val;
	if ( SUCCEEDED(pDiscImage->GetNumberOfTracks( &val)))
		nNumTracks = val;

	boolean fHasAudio = false;

	int nLoopTarget = nNumTracks;
	for ( int i = 1; i <= nLoopTarget; i++)
	{
		FileServerLib::IDiscImageTrack *pTrack = NULL;
		if ( SUCCEEDED( pDiscImage->GetTrack( i, &pTrack)) && pTrack != NULL)
		{
			DWORD dwFSSize = 0, dwBlockSize = 0, dwFlags = 0;
			if ( SUCCEEDED( pTrack->GetInfoFlags( &dwFlags)))
			{
				switch ( dwFlags & DI_TI_TM_MASK)
				{
				case DI_TI_TM_CDROM:
				case DI_TI_TM_CDXA:
					{
						DWORD dwFS = (dwFlags & DI_TI_FS_MASK);
						if ( dwFS					// file system available?
							&& SUCCEEDED( pTrack->GetFileSystemSize( &dwFSSize)))
						{
							if (dwFS & (DI_TI_FS_VCD|DI_TI_FS_SVCD))
								dwBlockSize = 2336;
							else
								dwBlockSize = 2048;

							ULARGE_INTEGER uliNumBytes;
							uliNumBytes.QuadPart = (ULONGLONG)dwFSSize * (ULONGLONG)dwBlockSize;
							dwDataSizeInMB = (DWORD)((uliNumBytes.QuadPart / (1024i64*1024i64))) + 1;
						}
						break;
					}
				case DI_TI_TM_CDDA:
					DWORD dwStart = 0, dwLen = 0;
					pTrack->GetLength( &dwLen);
					pTrack->GetStartAddress( &dwStart);
					dwAudioPlaytime = dwStart + dwLen;
					if ( !fHasAudio)
					{
						nNumAudioTracks = 0;
						dwAudioStart = dwStart;
					}
					nNumAudioTracks++;
					fHasAudio = true;
					break;
				}
			}
			pTrack->Release();
		}
		dwAudioPlaytime -= dwAudioStart;
	}

	if ( SUCCEEDED( pDiscImage->GetNumberOfBlocks( &val)))
		dwTotalBlocks = val;

	if ( SUCCEEDED( pDiscImage->GetDiscType( &val)))
		nDiscType = val;

	BSTR bstrTemp = NULL;
	if ( SUCCEEDED( pDiscImage->GetDescription( &bstrTemp)) && bstrTemp != NULL)
	{
		strDescription = bstrTemp;
		SysFreeString( bstrTemp);
	}

	bstrTemp = NULL;
	if ( SUCCEEDED( pDiscImage->GetVolumeName( &bstrTemp)) && bstrTemp != NULL)
	{
		strLabel = bstrTemp;
		SysFreeString( bstrTemp);
	}
	pDiscImage->Release();

	return ret;
}


// looks up which kind of disc is inserted 
enumDiscType CSourceDrives::GetDiscInfo(int id, wstring& strLabel,bool &bLabelFound)
{
    // End Dragon removing
 //   IDevice *pDevice = GetDevicesAt(id);
	//m_CriticalSection.Lock();
	//_ASSERTE(pDevice!=NULL);
	//enumDiscType eRet=_GetDiscInfo(pDevice,strLabel,bLabelFound);
	//m_CriticalSection.Unlock();
    return (enumDiscType)eRet;
    // End Dragon removing
}

enumDiscType CSourceDrives::_GetDiscInfo(IDevice * pDevice, wstring& strLabel,bool &bLabelFound)
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
	//WCHAR				*pszOwnerName;

	//strLabel=_T("");
	//bLabelFound=false;

	//RetVal	Result = 0;

	//Result = pDevice->TestUnitReady();

	//pDevice->GetReserveStatus(&ReserveType, (UniChar**)&pszOwnerName, &bReservedByThis);

	//if ( IS_OK(Result) )
	//{
	//	pDevice->GetDiscInfo(&spDiscInfo);

	//	if ( (NULL != spDiscInfo) && (IsProtectedDVD(spDiscInfo) == true) )
	//	{
	//		Ret=DVDProtected;
	//	}
	//	else if (NULL != spDiscInfo)
	//	{
	//		bool bHasAudio, bHasData;
	//		Result = _GetDiscTitleToDisplay(spDiscInfo, strTitleString, bHasAudio, bHasData);

	//		if (DiscContentBlank == Result)
	//		{
	//			Ret=DiscBlank;
	//		}
	//		else
	//		{
	//			if ( bHasData)
	//				Ret=DiscRom;
	//			else if ( bHasAudio)
	//				Ret=DiscAudio;
	//		}
	//		DiscMediumType		MediumType = (DiscMediumType)0;
	//		CStringAttr			MediumTypeString;

	//		MediumType = (DiscMediumType)spDiscInfo->GetAttributeLong(AttrULongDiscMediumType);
	//		if (MediumType != 0)
	//		{
	//			m_StringLookup.SetAttributeLong(AttrULongDiscMediumType, MediumType);
	//			MediumTypeString.GetAttributeString(&m_StringLookup, AttrStringLookedUp);
	//			if(strTitleString.length())
	//				bLabelFound = true;
	//			else
	//				strTitleString = MediumTypeString;
	//		}
	//	}
	//}
	//else if (DeviceReservedLongTerm == ReserveType)
	//{
	//	strTitleString=pszOwnerName;
	//	Ret=DeviceReserved;
	//}
	//else
	//{
	//	Ret=NoDisc;
	//}

	//if (NULL != pszOwnerName)
	//{
	//	pDevice->Free(pszOwnerName);
	//	pszOwnerName = NULL;
	//}

	//strLabel=strTitleString;
    // End Dragon removing
    return Ret;
}

bool CSourceDrives::ForceCache(void)
{
	wstring strCachePath;
	SetCacheMode(0,strCachePath);
	return true;
}
*/