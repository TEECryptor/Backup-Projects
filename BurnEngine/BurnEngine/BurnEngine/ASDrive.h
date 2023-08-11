//-----------------------------------------------
//	ASDrive.h
//	Copyright (c) 2001 - 2008, Sonic Solutions.  All rights reserved.
//-----------------------------------------------
#ifndef _ASDRIVE_H_
#define _ASDRIVE_H_
//-----------------------------------------------
#include "BurnEngine.h"
#include "AS_StorageDevice.h"
#include "AS_StorageError.h"
#include "AS_StringHelper.h"
#include "ASMedia.h"
//-----------------------------------------------
#define	AS_STRING_LEN		128
//-----------------------------------------------
//	Name:CASDrive
//	Introduce:This class define a AS_Storage drive
//-----------------------------------------------
class CASDrive : public CDrive
{
public:
	CASDrive(DWORD dwIndex, LPCTSTR lpAppName = NULL);
	CASDrive(LPCTSTR lpszImageFile, LPCTSTR lpAppName = NULL);
	~CASDrive(void);
	//-------------------------------------------
public:
	//Get this driver ID in system
	DWORD		GetID();
	//Return the device description string
	LONG		GetDescription(LPWSTR lpwzDescStr, DWORD dwStrLen);
	//Return the device path string
	LONG		GetDrivePath(LPWSTR lpwzPathStr, DWORD dwStrLen);
	//Return manufacturer name for this device
	LONG		GetManufacturer(LPWSTR lpwzManufacturerStr, DWORD dwStrLen);
	//Return the model name for this device
	LONG		GetModelName(LPWSTR lpwzModelName, DWORD dwStrLen);
	//Return the version for this device
	LONG		GetVersion(LPWSTR lpwzVersion, DWORD dwStrLen);
	//Return the device capable type flag
	DWORD		GetCapableTypes();
	//Return data rates for this device
	LONG		GetDataRates(DWORD dwDataRates[], DWORD dwArraySize);
	//Return device is ready or not
	BOOL		IsReady();
	//Close this device
	LONG		Close(BOOL bSybc = FALSE);
	//Eject this device
	LONG		Eject(BOOL bSybc = FALSE);
	//Get current media kind in this device.(MediaIsCdRom, MediaIsDvdRom, etc)
	DWORD		GetMediaKind();
	//Get current media state in this device.(MediaIsReadable, MediaIsWritable,etc)
	DWORD		GetMediaState();
	//Get current media in this device
	CMedia*		GetMedia();
	//Set the number of copies for this device in burning project
	void		SetNumberOfCopies(int iCopies);
	//Get the number of copies for this device in burning project
	int			GetNumberOfCopies();
	//-------------------------------------------
	//Reserve this device
	SInt32		Reserve(DWORD dwAccessType = AS_StorageDevice::ExclusiveAccess_Obtain);
	//Relese this device
	SInt32		Release();
	//Return this device can write or not
	BOOL		IsWriteable();
	//Lock this device
	SInt32		Lock();
	//Unlock this device
	SInt32		Unlock();
	//-------------------------------------------
	//Get the image file name of this drive.Only for image drive
	LPCTSTR		GetImageFile();
	//Get the handle of AS_Storage device
	PULONG		GetHandle();
	//Return the device capable type string
	SInt32		GetCapableTypes(LPWSTR lpwString, DWORD dwStrLen);
	//Return this drive can write DVD or not
	BOOL		CanWriteDVD();
	//-------------------------------------------
	//Erase the media in this driver
	SInt32		EraseMedia(AS_StorageDevice::InfoCallback Callback, LPVOID lpUserData);
	//Open current media object in this driver
	CASMedia*	OpenMedia(AS_Volume::InfoCallback Callback, LPVOID lpUserData);
	//-------------------------------------------
public:
	AS_StorageDevice	m_ASDevice;			//AS_Storage device handle
	//-------------------------------------------
private:
	UInt32				m_uIndex;			//This device index in system
	TCHAR				m_szImageFile[MAX_PATH];
	char				m_scAppName[AS_STRING_LEN];	
	int					m_iNumberOfCopies;	//The number of copy for this device	
	//
	CASMedia*			m_pASMedia;			//The media object in this drive
	//-------------------------------------------
};
//-----------------------------------------------
#endif	//_ASDRIVE_H_
//-----------------------------------------------
