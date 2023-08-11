//-----------------------------------------------
//	ASDrive.h
//	Copyright (c) 2001 - 2008, Sonic Solutions.  All rights reserved.
//-----------------------------------------------
#ifndef _ASDRIVE_H_
#define _ASDRIVE_H_
//-----------------------------------------------
#include "AS_StorageDevice.h"
#include "AS_StorageError.h"
#include "AS_StringHelper.h"
#include "ASMedia.h"
//-----------------------------------------------
#define	AS_STRING_LEN		80
//-----------------------------------------------
//	Name:CASDrive
//	Introduce:This class define a AS_Storage drive
//-----------------------------------------------
class CASDrive
{
public:
	CASDrive(DWORD dwIndex, LPCTSTR lpAppName = NULL);
	CASDrive(LPCTSTR lpszImageFile, LPCTSTR lpAppName = NULL);
	~CASDrive(void);
	//-------------------------------------------
public:
	//Reserve this device
	SInt32		Reserve(DWORD dwAccessType = AS_StorageDevice::ExclusiveAccess_Obtain);
	//Relese this device
	SInt32		Release();
	//Return device is ready or not
	BOOL		IsReady();
	//Return this device can write or not
	BOOL		IsWriteable();
	//Close this device
	SInt32		Close(BOOL bSybc = FALSE);
	//Eject this device
	SInt32		Eject(BOOL bSybc = FALSE);
	//Lock this device
	SInt32		Lock();
	//Unlock this device
	SInt32		Unlock();
	//-------------------------------------------
	//Get this driver ID in system
	UInt32		GetID();
	//Get the image file name of this drive.Only for image drive
	LPCTSTR		GetImageFile();
	//Get the handle of AS_Storage device
	PULONG		GetHandle();
	//Return the device description string
	SInt32		GetDescription(LPWSTR lpwString, DWORD dwStrLen);
	//Return the device path string
	SInt32		GetDrivePath(LPWSTR lpwString, DWORD dwStrLen);
	//Return manufacturer name for this device
	SInt32		GetManufacturer(LPWSTR lpwString, DWORD dwStrLen);
	//Return the model name for this device
	SInt32		GetModelName(LPWSTR lpwString, DWORD dwStrLen);
	//Return the version for this device
	SInt32		GetVersion(LPWSTR lpwString, DWORD dwStrLen);
	//Return the device capable type string
	SInt32		GetCapableTypes(LPWSTR lpwString, DWORD dwStrLen);
	//Return data rates for this device
	SInt32		GetDataRates(DWORD dwDataRates[]);
	//-------------------------------------------
	//Return the media kind in this driver
	UInt64		GetMediaKind();
	//Return the media state in this driver
	UInt32		GetMediaState();
	//Erase the media in this driver
	SInt32		EraseMedia(AS_StorageDevice::InfoCallback Callback, LPVOID lpUserData);
	//Open current media object in this driver
	CASMedia*	OpenMedia(AS_Volume::InfoCallback Callback, LPVOID lpUserData);
	//-------------------------------------------
public:
	AS_StorageDevice	m_ASDevice;		//AS_Storage device handle
	//-------------------------------------------
private:
	UInt32				m_uIndex;		//This device index in system
	TCHAR				m_szImageFile[MAX_PATH];
	char				m_scAppName[AS_STRING_LEN];	
	//
	CASMedia			m_Media;		//The media object in this drive
	//-------------------------------------------
};
//-----------------------------------------------
#endif	//_ASDRIVE_H_
//-----------------------------------------------
