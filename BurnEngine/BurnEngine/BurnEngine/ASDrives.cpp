//-----------------------------------------------
//	ASDrives.cpp
//	The implement file for CASDrives
//-----------------------------------------------
#include "StdAfx.h"
#include ".\asdrives.h"
//-----------------------------------------------
//	Name:CASDrives
//	Introduce:Construction
//-----------------------------------------------
CASDrives::CASDrives(CASDriveManager* pDriveManager)
 : m_pDriveManager(pDriveManager)
{
}
//-----------------------------------------------
//	Name:~CASDrives
//	Introduce:Distruction
//-----------------------------------------------
CASDrives::~CASDrives(void)
{
	RemoveDrives();
}
//-----------------------------------------------


//-----------------------------------------------
//	Name:AddDrives
//	Introduce:Add drives object pointer from m_pDriveManager
//-----------------------------------------------
BOOL CASDrives::AddDrives()
{
	if(NULL == m_pDriveManager)
		return FALSE;

	DWORD dwDriveCnt = m_pDriveManager->GetDrivesCnt();
	if(0 == dwDriveCnt)
		return FALSE;

	for(DWORD dwCnt = 0; dwCnt < dwDriveCnt; dwCnt++)
	{
		CASDrive* pDrive = m_pDriveManager->GetDriveByIndex(dwCnt);
		if(NULL != pDrive)
		{
			m_lstASDrive.push_back(pDrive);
		}
	}

	return TRUE;
}
//-----------------------------------------------
//	Name:RemoveDrives
//	Introduce:Remove all drives object pointer in drives list
//-----------------------------------------------
void CASDrives::RemoveDrives()
{
	m_lstASDrive.clear();
}
//-----------------------------------------------
//	Name:GetDriveCnt
//	Introduce:Return all drives count
//-----------------------------------------------
DWORD CASDrives::GetDriveCnt()
{
	DWORD dwCnt = (DWORD)m_lstASDrive.size();

	return dwCnt;
}
//-----------------------------------------------
//	Name:GetDriveByIndex
//	Introduce:Return the drive object pointer by index
//-----------------------------------------------
CASDrive* CASDrives::GetDriveByIndex(DWORD dwIndex)
{
	int iIndex = 0;
	CASDrive* pDrive = NULL;
	for(ASDriveArrayPtr::iterator it = m_lstASDrive.begin(); it != m_lstASDrive.end(); it++, iIndex++)
	{
		if(dwIndex == iIndex)
		{
			pDrive = reinterpret_cast<CASDrive*>(*it);
			break;
		}
	}

	return pDrive;
}
//-----------------------------------------------
//	Name:GetDriveByID
//	Introduce:Return the drive object pointer by ID in the system
//-----------------------------------------------
CASDrive* CASDrives::GetDriveByID(DWORD dwID)
{
	int iIndex = 0;
	CASDrive* pDrive = NULL;
	for(ASDriveArrayPtr::iterator it = m_lstASDrive.begin(); it != m_lstASDrive.end(); it++, iIndex++)
	{
		pDrive = reinterpret_cast<CASDrive*>(*it);
		if((NULL != pDrive) && (dwID == pDrive->GetID()))
		{
			break;
		}
	}

	return pDrive;
}
//-----------------------------------------------
//	Name:AddImageDrive
//	Introduce:Create an image drive object
//-----------------------------------------------
CASDrive* CASDrives::AddImageDrive(LPCTSTR lpImageFile)
{
	if(NULL == m_pDriveManager)
		return NULL;

	if(NULL == lpImageFile || lstrlen(lpImageFile) == 0)
		return NULL;

	CASDrive* pImageDrive = m_pDriveManager->AddImageDrive(lpImageFile);

	return pImageDrive;
}
//-----------------------------------------------
//	Name:AddImageDrive
//	Introduce:Release an image drive object
//-----------------------------------------------
void CASDrives::ReleaseImageDrive(LPCTSTR lpImageFile)
{
	if(NULL == m_pDriveManager)
		return;

	if(NULL == lpImageFile || lstrlen(lpImageFile) == 0)
		return;

	m_pDriveManager->ReleaseImageDrive(lpImageFile);
}
//-----------------------------------------------
