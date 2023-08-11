//-----------------------------------------------
//	ASDrives.h
//	Copyright (c) 2001 - 2008, Sonic Solutions.  All rights reserved.
//-----------------------------------------------
#ifndef _ASDRIVES_H_
#define _ASDRIVES_H_
//-----------------------------------------------
#include <list>
#include "ASDriveManager.h"
#include "ASDrive.h"
//-----------------------------------------------
typedef std::list<int>			IDArrayPtr;
typedef std::list<CASDrive*>	ASDriveArrayPtr;
//-----------------------------------------------
//	Name:CASDrives
//	Introduce:This class define a AS_Storage drives array
//-----------------------------------------------
class CASDrives
{
public:
	CASDrives(CASDriveManager* pDriveManager);
	~CASDrives(void);
	//-------------------------------------------
public:
	//Add drives object pointer from m_pDriveManager
	BOOL		AddDrives();
	//Remove all drives object pointer in drives list
	void		RemoveDrives();
	//Return all drives count
	DWORD		GetDriveCnt();
	//Return the drive object pointer by index in list
	CASDrive*	GetDriveByIndex(DWORD dwIndex);
	//Return the drive object pointer by ID in the system
	CASDrive*	GetDriveByID(DWORD dwID);
	//Create an image drive object
	CASDrive*	AddImageDrive(LPCTSTR lpImageFile);
	//Release an image drive object
	void		ReleaseImageDrive(LPCTSTR lpImageFile);
	//-------------------------------------------
protected:
	CASDriveManager*	m_pDriveManager;
	ASDriveArrayPtr		m_lstASDrive;
	ASDriveArrayPtr		m_lstImageDrive;
	//-------------------------------------------
};
//-----------------------------------------------
#endif	//_ASDRIVES_H_
//-----------------------------------------------
