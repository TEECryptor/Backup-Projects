//-----------------------------------------------
//	BurnEngine.h
//	Copyright (c) 2001 - 2008, Sonic Solutions.  All rights reserved.
//-----------------------------------------------
#ifndef _BURNENGINE_H_
#define _BURNENGINE_H_
//-----------------------------------------------
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the BURNENGINE_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// BURNENGINE_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
//-----------------------------------------------
#ifdef BURNENGINE_EXPORTS
#define BURNENGINE_API __declspec(dllexport)
#else
#define BURNENGINE_API __declspec(dllimport)
#endif
//-----------------------------------------------


//-----------------------------------------------
//	Name:eMediaState
//	Introduce:Media State Information is valid only if Device state reports ready
//			  Incompatible media is represented by the following states: Readable = false, Writable = false
//-----------------------------------------------
enum eMediaState
{
	MediaIsReadable    = (1 << 0),
	MediaIsWritable    = (1 << 1),
	MediaIsInvisible   = (1 << 2),
	MediaIsAppendable  = (1 << 3),
	MediaIsRewritable  = (1 << 5),
	MediaIsErasable    = (1 << 7),
	MediaIsProtected   = (1 << 10),
	MediaIsBlank       = (1 << 12),		
	LayerJumpSettable  = (1 << 13)		// report whether setting jump address is allowed
};
//-----------------------------------------------


//-----------------------------------------------
//	Name:eMediaKind
//	Introduce:Media kind type definition
//-----------------------------------------------
enum eMediaKind
{
	MediaIsCdRom    = (1 << 0),
	MediaIsCdr      = (1 << 1),
	MediaIsCdrw     = (1 << 2),
	MediaIsDdCdrom  = (1 << 3),
	MediaIsDdCdr    = (1 << 4),
	MediaIsDdCdrw   = (1 << 5),
	MediaIsDvdRom   = (1 << 6),
	MediaIsDvdr     = (1 << 7),
	MediaIsDvdrw    = (1 << 8),
	MediaIsDvdpr    = (1 << 9),
	MediaIsDvdprw   = (1 << 10),
	MediaIsDvdRam   = (1 << 11),
	MediaIsDvdpr9   = (1 << 12),
	MediaIsDvdr9    = (1 << 13),
	MediaIsBdr		= (1 << 14),
	MediaIsBdre		= (1 << 15),
	MediaIsHdDvdRom = (1 << 16),
	MediaIsHdDvdr   = (1 << 17),
	MediaIsHdDvdrw  = (1 << 18),
	MediaIsBdrom	= (1 << 19),
	MediaIsSeqTape  = ((ULONGLONG)1 << 31)
};
//-----------------------------------------------
/*
// Defines parameters returned through the InfoCallback method (below).
enum Info
{
	Info_ProgressUpdate         = 1, // infoDword = percent complete, infoPtr = Progress structure
	Info_PromptForMediaInDevice = 2, // infoDword = unused, infoPtr = unused
	Info_PromptForNextMedia     = 3  // infoDword = unused, infoPtr = unused
};
*/
typedef LONG (*DeviceInfoCallback)(DWORD dwInfoData, LPVOID lpUserData);

typedef LONG (*MediaInfoCallback)(DWORD dwInfoType, DWORD dwInfoData, LPVOID lpUserData);
typedef LONG (*ProgressInfoCallback)(DWORD dwInfoType, DWORD dwInfoData, LPVOID lpInfoPtr, LPVOID lpUserData);


//-----------------------------------------------
//	Name:CMedia
//	Introduce:This class define the interface for Media
//-----------------------------------------------
class CMedia
{
public:
	CMedia(){};
	virtual ~CMedia(){};
	//-------------------------------------------
public:	
	/*	Name:GetMediaState
		Introduce:Get the media current state (File_IsMounted, File_IsDirty, etc)
		Param:N/A
		Return:Return the state value, which will be a combination of eMediaState
	*/
	virtual DWORD GetMediaState() = 0;

	/*	Name:GetBlockSize
		Introduce:Get current media block size, in 4 byte
		Param:N/A
		Return:The current media block size
	*/
	virtual DWORD GetBlockSize() = 0;

	/*	Name:GetFreeBlocks
		Introduce:Get current media free block, in 4 byte
		Param:N/A
		Return:The current media free block 
	*/
	virtual DWORD GetFreeBlocks() = 0;
	//-------------------------------------------
};
//-----------------------------------------------


//-----------------------------------------------
//	Name:CDrive
//	Introduce:This class define the interface for Drive
//-----------------------------------------------
class CDrive
{
public:
	CDrive(){};
	virtual ~CDrive(){};
	//-------------------------------------------
public:	
	/*	Name:GetID
		Introduce:Get this driver ID in system
		Param:N/A
		Return:Return this driver ID in system
	*/
	virtual DWORD GetID() = 0;

	/*	Name:GetDescription
		Introduce:Get the device description string
		Param:OUT:lpwzDescStr:The device description string buffer
			 IN:dwStrLen:The string buffer size
		Return:Return 0 if function succeeds, otherwise return an AS_Storage error code
	*/
	virtual LONG GetDescription(LPWSTR lpwzDescStr, DWORD dwStrLen) = 0;

	/*	Name:GetDrivePath
		Introduce:Get the device path string
		Param:OUT:lpwzPathStr:The device path string buffer
			  IN:dwStrLen:The string buffer size
		Return:Return 0 if function succeeds, otherwise return an AS_Storage error code
	*/
	virtual LONG GetDrivePath(LPWSTR lpwzPathStr, DWORD dwStrLen) = 0;

	/*	Name:GetDrivePath
		Introduce:Get manufacturer name for this device
		Param:OUT:lpwzManufacturerStr:The manufacturer name string buffer
			  IN:dwStrLen:The string buffer size
		Return:Return 0 if function succeeds, otherwise return an AS_Storage error code
	*/
	virtual LONG GetManufacturer(LPWSTR lpwzManufacturerStr, DWORD dwStrLen) = 0;
	
	/*	Name:GetModelName
		Introduce:Get the model name for this device
		Param:OUT:lpwzModelName:The model name string buffer
			 IN:dwStrLen:The string buffer size
		Return:Return 0 if function succeeds, otherwise return an AS_Storage error code
	*/
	virtual LONG GetModelName(LPWSTR lpwzModelName, DWORD dwStrLen) = 0;

	/*	Name:GetVersion
		Introduce:Get the version string for this device
		Param:OUT:lpwzVersion:The version string buffer
			  IN:dwStrLen:The string buffer size
		Return:Return 0 if function succeeds, otherwise return an AS_Storage error code
	*/
	virtual LONG GetVersion(LPWSTR lpwzVersion, DWORD dwStrLen) = 0;
	
	/*	Name:GetCapableTypes
		Introduce:Get the device capable type flag
		Param:N/AOUT:dwCapableTypes:The media capable types flag of this device, it is a combination of eMediaKind
		Return:The media capable types flag of this device, it is a combination of eMediaKind. And return 0 if function fail.
	*/
	virtual DWORD GetCapableTypes() = 0;	

	/*	Name:GetDataRates
		Introduce:Get data rates for this device
		Param:OUT:dwDataRates:Date rates array buffer
			  IN:dwArraySize:The size of dwDataRates array
		Return:Return 0 if function succeeds, otherwise return an AS_Storage error code
	*/
	virtual LONG GetDataRates(DWORD dwDataRates[], DWORD dwArraySize) = 0;

	/*	Name:IsReady
		Introduce:Return device is ready or not
		Param:N/A
		Return:Return TRUE if device is ready, otherwise return FALSE
	*/
	virtual BOOL IsReady() = 0;

	/*	Name:Eject
		Introduce:Eject this device
		Param:IN:bSybc:Eject this device by synchronization or not
		Return:Return 0 if function succeeds, otherwise return an AS_Storage error code
	*/
	virtual LONG Eject(BOOL bSybc = FALSE) = 0;

	/*	Name:Close
		Introduce:Close this device
		Param:IN:bSybc:Close this device by synchronization or not
		Return:Return 0 if function succeeds, otherwise return an AS_Storage error code
	*/
	virtual LONG Close(BOOL bSybc = FALSE) = 0;
	//-------------------------------------------
	/*	Name:GetMediaKind
		Introduce:Get current media kind in this device.(MediaIsCdRom, MediaIsDvdRom, etc)
		Param:N/A
		Return:The media kind, it is a combination of eMediaKind
	*/
	virtual DWORD GetMediaKind() = 0;

	/*	Name:GetMediaState
		Introduce:Get current media state in this device.(MediaIsReadable, MediaIsWritable,etc)
		Param:N/A
		Return:The media state, it is a combination of eMediaState
	*/
	virtual DWORD GetMediaState() = 0;
	
	/*	Name:GetMedia
		Introduce:Get current media in this device
		Param:N/A
		Return:Return a CMedia object pointer
	*/
	virtual CMedia* GetMedia() = 0;
	//-------------------------------------------

	/*	Name:SetNumberOfCopies
		Introduce:Set the number of copies for this device in burning project
		Param:IN:iCopies:The number of copy
		Return:N/A
	*/
	virtual void SetNumberOfCopies(int iCopies) = 0;

	/*	Name:GetNumberOfCopies
		Introduce:Get the number of copies for this device in burning project
		Param:N/A
		Return:The number of copy
	*/
	virtual int GetNumberOfCopies() = 0;
	//-------------------------------------------
};
//-----------------------------------------------


//-----------------------------------------------
//	Name:CDriveManager
//	Introduce:This class define the interface for Drive Manager
//-----------------------------------------------
class CDriveManager
{
public:
	CDriveManager(){};
	virtual ~CDriveManager(){};
	//-------------------------------------------
public:		
	/*	Name:GetDrivesCnt
		Introduce:Get the total count of devices
		Param:N/A
		Return:Return the total count of devices int the system
	*/
	virtual DWORD GetDrivesCnt() = 0;

	/*	Name:GetDriveByIndex
		Introduce:Get drive object pointer by index in manager
		Param:N/A
		Return:Return the drive object pointer specific by iIndex
	*/
	virtual CDrive* GetDriveByIndex(int iIndex) = 0;

	/*	Name:GetDriveByID
		Introduce:Get drive object pointer by ID in current system
		Param:N/A
		Return:Return the drive object pointer specific by ID in current system
	*/
	virtual CDrive* GetDriveByID(int iID) = 0;
	//-------------------------------------------
};
//-----------------------------------------------


//-----------------------------------------------
//	Name:CBurnEngine
//	Introduce:This class define the interface for Burn Engine
//-----------------------------------------------
class CBurnEngine 
{
public:
	CBurnEngine(){};
	virtual ~CBurnEngine(){};
	//-------------------------------------------
public:
	/*	Name:InitEngine
		Introduce:Initialize the burn engine
		Param:IN:lpszAppName:The host application name string
		Return:Return TRUE if function succeeds, otherwise return FALSE
	*/
	virtual BOOL InitEngine(LPCTSTR lpszAppName) = 0;

	/*	Name:Release
		Introduce:Release the burn engine
		Param:N/A
		Return:N/A
	*/
	virtual void Release() = 0;

	/*	Name:GetDriveManager
		Introduce:Get drive manager in burn engine
		Param:N/A
		Return:The drive manager pointer in burn engine
	*/
	virtual CDriveManager* GetDriveManager() = 0;

	/*	Name:GetSourceDriveCnt
		Introduce:Return the number of source drivers in this Burn Engine
		Param:N/A
		Return:The number of source drivers in this Burn Engine
	*/
	virtual DWORD GetSourceDriveCnt() = 0;

	/*	Name:GetSourceDriveByIndex
		Introduce:Return the source drive pointer by specific index in this Burn Engine
		Param:N/A
		Return:The source drive pointer by specific index in this Burn Engine
	*/
	virtual CDrive* GetSourceDriveByIndex(int iIndex) = 0;

	/*	Name:GetTargetDriveCnt
		Introduce:Return the number of target drivers in this Burn Engine
		Param:N/A
		Return:The number of target drivers in this Burn Engine
	*/
	virtual DWORD GetTargetDriveCnt() = 0;

	/*	Name:GetTargetDriveByIndex
		Introduce:Return the target drive pointer by specific index in this Burn Engine
		Param:N/A
		Return:The target drive pointer by specific index in this Burn Engine
	*/
	virtual CDrive* GetTargetDriveByIndex(int iIndex) = 0;

	/*	Name:AddFileForBurn
		Introduce:Add a file to list for burning
		Param:IN:lpszSrcFileName:The full file name string on the hard disc
			  IN:lpszDesFileName:The file name string on the disc
		Return:N/A
	*/
	virtual void AddFileForBurn(LPCTSTR lpszSrcFileName, LPCTSTR lpszDesFileName) = 0;

	/*	Name:ClearFileForBurn
		Introduce:Clear all files which are in the list for burning
		Param:N/A
		Return:N/A
	*/
	virtual void ClearFileForBurn() = 0;

	/*	Name:SetBurnSource
		Introduce:Set the source device for burning
		Param:IN:iDeviceID:The device ID which will be burn source
		Return:N/A
	*/
	virtual void SetBurnSource(int iDeviceID) = 0;

	/*	Name:SetBurnSource
		Introduce:Set a image file as burn source
		Param:IN:lpszImageFile:The image file name which will be burn source
		Return:N/A
	*/
	virtual void SetBurnSource(LPCTSTR lpszImageFile) = 0;

	/*	Name:AddBurnTarget
		Introduce:Add a target device ID for burning
		Param:IN:iDeviceID:The device ID which will be burn target
		Return:N/A
	*/
	virtual LONG AddBurnTarget(int iDeviceID) = 0;

	/*	Name:AddBurnTarget
		Introduce:Add a image file as burn target
		Param:IN:lpszImageFile:The image file name which will be burn target
		Return:N/A
	*/
	virtual LONG AddBurnTarget(LPCTSTR lpszImageFile) = 0;

	/*	Name:RemoveTarget
		Introduce:Remove target device from burning list
		Param:IN:iDriveID:The device ID which should be remove, set it is -1 if want to remove the target image device
			  IN:bRemoveAll:Remove all selected target devices, include image target device
		Return:N/A
	*/
	virtual void RemoveTarget(int iDriveID, BOOL bRemoveAll = FALSE) = 0; 

	/*	Name:SetTargetVolumeLabel
		Introduce:Set the target disc/image label
		Param:IN:lpszVolumeLabel:The target disc/image label
		Return:N/A
	*/
	virtual void SetTargetVolumeLabel(LPCTSTR lpszVolumeLabel) = 0;

	/*	Name:EraseMedia
		Introduce:Erase the media in specific device
		Param:IN:iDeviceID:The deive ID which will erase media
		      IN:pInfoCallback:Device progress information callback function
			  IN:lpUserCallbackData:User define data in callback function
			  IN:bEject:Eject the device or not after erasing
		Return:Return 0 if function succeeds, otherwise return an AS_Storage error code
	*/
	virtual LONG EraseMedia(int iDeviceID, ProgressInfoCallback pInfoCallback, LPVOID lpUserCallbackData, BOOL bEject = FALSE) = 0;

	/*	Name:BurnToImage
		Introduce:Burn all files(Add by calling function AddFileForBurn()) to an image
		Param:IN:pInfoCallback:Device progress information callback function
			  IN:lpUserCallbackData:User define data in callback function
		Return:Return 0 if function succeeds, otherwise return an AS_Storage error code
	*/
	virtual LONG BurnToImage(ProgressInfoCallback pInfoCallback, LPVOID lpUserCallbackData) = 0;

	/*	Name:BurnToDisc
		Introduce:Burn all files(Add by calling function AddFileForBurn()) to discs,
				  the target devices are selected by calling function AddBurnTarget()
		Param:IN:pInfoCallback:Device progress information callback function
			  IN:lpUserCallbackData:User define data in callback function
			  IN:bEject:Eject the device or not after burning
		Return:Return 0 if function succeeds, otherwise return an AS_Storage error code
	*/
	virtual LONG BurnToDisc(ProgressInfoCallback pInfoCallback, LPVOID lpUserCallbackData, BOOL bEject = TRUE) = 0;
	
	/*	Name:BurnToDisc
		Introduce:Copy a disc context from source to target. The source device is selected by calling function SetBurnSource(),
				  the target devices are selected by calling function AddBurnTarget()
		Param:IN:pInfoCallback:Device progress information callback function
			  IN:lpUserCallbackData:User define data in callback function
			  IN:bEject:Eject the device or not after burning
		Return:Return 0 if function succeeds, otherwise return an AS_Storage error code
	*/
	virtual LONG BurnCopy(ProgressInfoCallback pInfoCallback, LPVOID lpUserCallbackData, BOOL bEject = TRUE) = 0;
	//-------------------------------------------
};
//-----------------------------------------------


//-----------------------------------------------
/*
 * The following functions are the exportors of this DLL
 */
//-----------------------------------------------
//To create a CBurnEngine object pointer
BURNENGINE_API CBurnEngine* BurnEngine_Create();
//To destroy a CBurnEngine object pointer
BURNENGINE_API void BurnEngine_Destroy(CBurnEngine* pBurnEngine);
//-----------------------------------------------
#endif	//_BURNENGINE_H_
//-----------------------------------------------