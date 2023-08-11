//-----------------------------------------------
//	ASBurnEngine.h
//	Copyright (c) 2001 - 2008, Sonic Solutions.  All rights reserved.
//-----------------------------------------------
#ifndef _ASBURNENGINE_H_
#define _ASBURNENGINE_H_
//-----------------------------------------------
#include <list>
//-----------------------------------------------
#include "ASDriveManager.h"
#include "ASSourceDrives.h"
#include "ASTargetdrives.h"
//-----------------------------------------------
//	Name:BurnFileInfo
//	Introduce:The file name string for which should be burned
//-----------------------------------------------
typedef struct tagBurnFileInfo
{
	TCHAR	szSrcFileName[MAX_PATH];	//File name in hard disc
	TCHAR	szDesFileName[MAX_PATH];	//File name in target disc
}BurnFileInfo, *LPBURNFILEINFO;
//-----------------------------------------------
typedef std::list<BurnFileInfo> BurnFileArrayPtr;
//-----------------------------------------------


//-----------------------------------------------
//	Name:CASBurnEngine
//	Introduce:The definition for CASBurnEngine
//-----------------------------------------------
class CASBurnEngine : public CBurnEngine
{
public:
	CASBurnEngine();
	~CASBurnEngine();
	//-------------------------------------------
public:
	//Device information callback function
	AS_StorageError DeviceInfoCallback(const AS_StorageDevice::Info& info, UInt32 infoDword, void* infoPtr);
	//Volume information callback function
	AS_StorageError VolumeInfoCallback(const AS_Volume::Info& info, UInt32 infoDword, void* infoPtr);
	//-------------------------------------------
public:
	//Initialize the burn engine
	BOOL InitEngine(LPCTSTR lpszAppName);
	//Release the burn engine
	void Release();
	//Get drive manager in burn engine
	CDriveManager* GetDriveManager();
	//Return the number of source drivers in this Burn Engine
	DWORD GetSourceDriveCnt();
	//Return the source drive pointer by specific index in this Burn Engine
	CDrive* GetSourceDriveByIndex(int iIndex);
	//Return the number of target drivers in this Burn Engine
	DWORD GetTargetDriveCnt();
	//Return the target drive pointer by specific index in this Burn Engine
	CDrive* GetTargetDriveByIndex(int iIndex);
	//Add a file to list for burning
	void AddFileForBurn(LPCTSTR lpszSrcFileName, LPCTSTR lpszDesFileName);
	//Clear all files which are in the list for burning
	void ClearFileForBurn();
	//Set the source device for burning
	void SetBurnSource(int iDeviceID);
	//Set a image file as burn source
	void SetBurnSource(LPCTSTR lpszImageFile);
	//Add a target device ID for burning
	LONG AddBurnTarget(int iDeviceID);
	//Add a image file as burn target
	LONG AddBurnTarget(LPCTSTR lpszImageFile);
	//Remove target device from burning list	
	void RemoveTarget(int iDriveID, BOOL bRemoveAll = FALSE); 
	//-------------------------------------------
	///Set the target disc/image label
	void SetTargetVolumeLabel(LPCTSTR lpszVolumeLabel);
	//Erase the media in specific device
	LONG EraseMedia(int iDeviceID, ProgressInfoCallback pInfoCallback, LPVOID lpUserCallbackData, BOOL bEject = FALSE);
	//Burn all files(Add by calling function AddFileForBurn()) to an image
	LONG BurnToImage(ProgressInfoCallback pInfoCallback, LPVOID lpUserCallbackData);
	//Burn all files(Add by calling function AddFileForBurn()) to discs, the target devices are selected by calling function AddBurnTarget()
	LONG BurnToDisc(ProgressInfoCallback pInfoCallback, LPVOID lpUserCallbackData, BOOL bEject = TRUE);
	//Copy a disc context from source to target. The source device is selected by calling function SetBurnSource(),	the target devices are selected by calling function AddBurnTarget()
	LONG BurnCopy(ProgressInfoCallback pInfoCallback, LPVOID lpUserCallbackData, BOOL bEject = TRUE);
	//-------------------------------------------
private:
	//Burn all files to the image device
	LONG BurnToImageDevice(CASDrive* pImageDrive);
	//-------------------------------------------
private:
	CASDriveManager*	m_pDriveManager;	//Devices manager
	CASSourceDrives*	m_pSourceDrives;	//Source drives list
	CASTargetDrives*	m_pTargetDrives;	//Target drives list
	//-------------------------------------------
	TCHAR					m_szVolumeLabel[AS_STRING_LEN];
	BurnFileArrayPtr		m_lstBurnFileInfo;		//The file name which should be burned
	ProgressInfoCallback	m_pProgressCallback;
	LPVOID					m_pCallbackUserDatae;
	//-------------------------------------------
};
//-----------------------------------------------
#endif	//_ASBURNENGINE_H_
//-----------------------------------------------