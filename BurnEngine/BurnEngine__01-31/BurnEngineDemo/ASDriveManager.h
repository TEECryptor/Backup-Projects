//-----------------------------------------------
//	ASDriveManager.h
//	Copyright (c) 2001 - 2008, Sonic Solutions.  All rights reserved.
//-----------------------------------------------
#ifndef _ASDRIVE_MANAGER_H_
#define _ASDRIVE_MANAGER_H_
//-----------------------------------------------
#include <list>
#include "ASDrive.h"
//-----------------------------------------------
typedef std::list<CASDrive*>	ASDriveArrayPtr;
//-----------------------------------------------
class CASDriveManager
{
public:
	CASDriveManager(LPCTSTR lpszAppName = NULL);
	~CASDriveManager();
	//-------------------------------------------
public:
	//Enum all drives
	void		EnumDrives();
	//Release all drives
	void		ReleaseDrives();	
	//Get drives count
	DWORD		GetDrivesCnt();
	//Get drive by index
	CASDrive*	GetDriveByIndex(int iIndex);
	//Add a new image drive object
	CASDrive*	AddImageDrive(LPCTSTR lpImageFile);
	//Release an image drive object by image file name
	void		ReleaseImageDrive(LPCTSTR lpImageFile);
	//-------------------------------------------
private:
	//Release all image file drives
	void		ReleaseImageDrives();
	//-------------------------------------------
private:
	static int				m_iRef;
	static TCHAR			m_scAppName[AS_STRING_LEN];
	static ASDriveArrayPtr	m_lstDrives;
	//-------------------------------------------
	 ASDriveArrayPtr		m_lstImageDrive;
	//-------------------------------------------
};
//-----------------------------------------------
#endif	//_ASDRIVE_MANAGER_H_
//-----------------------------------------------