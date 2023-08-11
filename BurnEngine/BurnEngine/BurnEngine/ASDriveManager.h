//-----------------------------------------------
//	ASDriveManager.h
//	Copyright (c) 2001 - 2008, Sonic Solutions.  All rights reserved.
//-----------------------------------------------
#ifndef _ASDRIVE_MANAGER_H_
#define _ASDRIVE_MANAGER_H_
//-----------------------------------------------
#include <list>
#include "BurnEngine.h"
#include "ASDrive.h"
//-----------------------------------------------
typedef std::list<CASDrive*>	ASDriveArrayPtr;
//-----------------------------------------------
class CASDriveManager : public CDriveManager
{
public:
	CASDriveManager(LPCTSTR lpszAppName = NULL);
	~CASDriveManager();
	//-------------------------------------------
public:
	//Get drives count
	DWORD		GetDrivesCnt();
	//Get drive by index
	CASDrive*	GetDriveByIndex(int iIndex);
	//Get drive object pointer by ID in current system
	CDrive*		GetDriveByID(int iID);
	//-------------------------------------------
public:
	//Enum all drives
	void		EnumDrives();
	//Release all drives
	void		ReleaseDrives();	
	//Add a new image drive object
	CASDrive*	AddImageDrive(LPCTSTR lpImageFile);
	//Get the image drive by image file name
	CASDrive*	GetDriveByImageName(LPCTSTR lpImageFile);
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