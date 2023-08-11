//-----------------------------------------------
//	ASDriveManager.cpp
//	The implement file for CASDriveManager
//-----------------------------------------------
#include "StdAfx.h"
#include "ASDriveManager.h"
//-----------------------------------------------
int CASDriveManager::m_iRef = 0;
TCHAR CASDriveManager::m_scAppName[AS_STRING_LEN];
ASDriveArrayPtr CASDriveManager::m_lstDrives;
//-----------------------------------------------


//-----------------------------------------------
//	Name:CASDriveManager
//	Introduce:Construction
//-----------------------------------------------
CASDriveManager::CASDriveManager(LPCTSTR lpszAppName)
{
	if(NULL == lpszAppName || lstrlen(lpszAppName))
	{
		lstrcpy(m_scAppName, _T("\0"));
	}
	else
	{
		lstrcpy(m_scAppName, lpszAppName);
	}
}
//-----------------------------------------------
//	Name:~CASDriveManager
//	Introduce:Distruction
//-----------------------------------------------
CASDriveManager::~CASDriveManager()
{
	ReleaseImageDrives();
}
//-----------------------------------------------


//-----------------------------------------------
//	Name:GetDrivesCnt
//	Introduce:Get drives count
//-----------------------------------------------
DWORD CASDriveManager::GetDrivesCnt()
{
	DWORD dwCnt = (DWORD)m_lstDrives.size();

	return dwCnt;
}
//-----------------------------------------------
//	Name:GetDriveByIndex
//	Introduce:Get drive by index
//-----------------------------------------------
CASDrive* CASDriveManager::GetDriveByIndex(int iIndex)
{
	if(iIndex < 0 || iIndex >= (int)m_lstDrives.size())
		return NULL;

	int i = 0;
	CASDrive* pDrive = NULL;
	for(ASDriveArrayPtr::iterator it = m_lstDrives.begin(); it != m_lstDrives.end(); it++, i++)
	{
		if(i == iIndex)
		{
			pDrive = reinterpret_cast<CASDrive*>(*it);
			break;
		}
	}

	return pDrive;
}
//-----------------------------------------------
//	Name:GetDriveByID
//	Introduce:Get drive object pointer by ID in current system
//-----------------------------------------------
CDrive* CASDriveManager::GetDriveByID(int iID)
{
	CASDrive* pDrive = NULL;
	for(ASDriveArrayPtr::iterator it = m_lstDrives.begin(); it != m_lstDrives.end(); it++)
	{
		pDrive = reinterpret_cast<CASDrive*>(*it);
		if(NULL != pDrive && iID == pDrive->GetID())
		{
			break;
		}
	}

	return pDrive;
}
//-----------------------------------------------



//-----------------------------------------------
//	Name:EnumDrives
//	Introduce:Enum all drives
//-----------------------------------------------
void CASDriveManager::EnumDrives()
{
	if(m_iRef <= 0)
	{
		UInt32 uCount = 0;
		AS_Error err = AS_StorageError_None;
		err = AS_GetStorageDeviceCount(AS_StorageDevice::Scan_Optical, uCount);
		if(AS_StorageError_None == err && uCount != 0)
		{
			for (UInt32 i = 1; i <= uCount; ++i)
			{
				CASDrive* pASDrive = new CASDrive(i, m_scAppName);
				err = AS_OpenStorageDevice(i, pASDrive->m_ASDevice);
				if(AS_StorageError_None == err)
					m_lstDrives.push_back(pASDrive);
				else
					delete pASDrive;
			}
		}	

		m_iRef = 1;
	}
	else
		m_iRef++;
}
//-----------------------------------------------
//	Name:ReleaseDrives
//	Introduce:Release all drives
//-----------------------------------------------
void CASDriveManager::ReleaseDrives()
{
	m_iRef--;

	if(m_iRef <= 0)
	{
		CASDrive* pASDrive = NULL;
		for(ASDriveArrayPtr::iterator it = m_lstDrives.begin(); it != m_lstDrives.end(); it++)
		{
			pASDrive = reinterpret_cast<CASDrive*>(*it);
			if(NULL != pASDrive)
			{
				AS_StorageDevice_Close(pASDrive->m_ASDevice);
				pASDrive->Release();
				delete pASDrive;
				pASDrive = NULL;
			}
		}

		m_lstDrives.clear();
	}
}
//-----------------------------------------------
//	Name:AddImageDrive
//	Introduce:Add a new image drive object
//-----------------------------------------------
CASDrive* CASDriveManager::AddImageDrive(LPCTSTR lpImageFile)
{
	if(NULL == lpImageFile || lstrlen(lpImageFile) == 0)
		return NULL;	

	//To check this image file drive is exist or not
	CASDrive* pImageDrive = NULL;
	for(ASDriveArrayPtr::iterator it = m_lstImageDrive.begin(); it != m_lstImageDrive.end(); it++)
	{
		pImageDrive = reinterpret_cast<CASDrive*>(*it);
		if((NULL != pImageDrive) && lstrcmp(lpImageFile, pImageDrive->GetImageFile()) == 0)
		{
			return pImageDrive;
		}
	}

	//We will create a new image file drive
	char szImageFile[MAX_PATH] = {"\0"};
#ifdef UNICODE	
	WideCharToMultiByte(CP_ACP, 0, lpImageFile, -1, szImageFile, MAX_PATH, NULL, NULL);
#else    
	strcpy(szImageFile, lpImageFile);
#endif	//UNICODE

	AS_Error err = AS_StorageError_None;
	pImageDrive = new CASDrive(lpImageFile, m_scAppName);
	err = AS_OpenStorageDevice(0, pImageDrive->m_ASDevice, AS_StringA(szImageFile));
	if(AS_StorageError_None != err)
	{
		delete pImageDrive;
		pImageDrive = NULL;
	}

	m_lstImageDrive.push_back(pImageDrive);
	
	return pImageDrive;
}
//-----------------------------------------------
//	Name:GetDriveByImageName
//	Introduce:Get the image drive by image file name
//-----------------------------------------------
CASDrive* CASDriveManager::GetDriveByImageName(LPCTSTR lpImageFile)
{
	if(NULL == lpImageFile || lstrlen(lpImageFile) == 0)
		return NULL;	

	CASDrive* pImageDrive = NULL;
	for(ASDriveArrayPtr::iterator it = m_lstImageDrive.begin(); it != m_lstImageDrive.end(); it++)
	{
		pImageDrive = reinterpret_cast<CASDrive*>(*it);
		if((NULL != pImageDrive) && lstrcmp(lpImageFile, pImageDrive->GetImageFile()) == 0)
		{
			return pImageDrive;
		}
	}

	return NULL;
}
//-----------------------------------------------
//	Name:ReleaseImageDrive
//	Introduce:Release an image drive object by image file name
//-----------------------------------------------
void CASDriveManager::ReleaseImageDrive(LPCTSTR lpImageFile)
{
	if(NULL == lpImageFile || lstrlen(lpImageFile) == 0)
		return;

	CASDrive* pImageDrive = NULL;
	for(ASDriveArrayPtr::iterator it = m_lstImageDrive.begin(); it != m_lstImageDrive.end(); it++)
	{
		pImageDrive = reinterpret_cast<CASDrive*>(*it);
		if((NULL != pImageDrive) && lstrcmp(lpImageFile, pImageDrive->GetImageFile()) == 0)
		{
			m_lstImageDrive.erase(it);

			AS_StorageDevice_Close(pImageDrive->m_ASDevice);
			pImageDrive->Release();
			delete pImageDrive;
			pImageDrive = NULL;
			break;
		}
	}
}
//-----------------------------------------------



//-----------------------------------------------
//	Name:ReleaseImageDrives
//	Introduce:Release all image file drives
//-----------------------------------------------
void CASDriveManager::ReleaseImageDrives()
{
	CASDrive* pImageDrive = NULL;
	for(ASDriveArrayPtr::iterator it = m_lstImageDrive.begin(); it != m_lstImageDrive.end(); it++)
	{
		pImageDrive = reinterpret_cast<CASDrive*>(*it);
		if(NULL != pImageDrive)
		{
			pImageDrive->Release();
			delete pImageDrive;
			pImageDrive = NULL;
		}
	}
	m_lstImageDrive.clear();
}
//-----------------------------------------------