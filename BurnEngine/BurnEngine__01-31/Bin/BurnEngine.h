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
		Introduce:Get the media current state (writable, write-protected, etc)
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
		Param:OUT:lpszDescStr:The device description string buffer
			 IN:dwStrLen:The string buffer size
		Return:Return 0 if function succeeds, otherwise return an AS_Storage error code
	*/
	virtual LONG GetDescription(LPTSTR lpszDescStr, DWORD dwStrLen) = 0;

	/*	Name:GetDrivePath
		Introduce:Get the device path string
		Param:OUT:lpszPathStr:The device path string buffer
			  IN:dwStrLen:The string buffer size
		Return:Return 0 if function succeeds, otherwise return an AS_Storage error code
	*/
	virtual LONG GetDrivePath(LPTSTR lpszPathStr, DWORD dwStrLen) = 0;

	/*	Name:GetDrivePath
		Introduce:Get manufacturer name for this device
		Param:OUT:lpszManufacturerStr:The manufacturer name string buffer
			  IN:dwStrLen:The string buffer size
		Return:Return 0 if function succeeds, otherwise return an AS_Storage error code
	*/
	virtual LONG GetManufacturer(LPTSTR lpszManufacturerStr, DWORD dwStrLen) = 0;
	
	/*	Name:GetModelName
		Introduce:Get the model name for this device
		Param:OUT:lpszModelName:The model name string buffer
			 IN:dwStrLen:The string buffer size
		Return:Return 0 if function succeeds, otherwise return an AS_Storage error code
	*/
	virtual LONG GetModelName(LPTSTR lpszModelName, DWORD dwStrLen) = 0;

	/*	Name:GetVersion
		Introduce:Get the version string for this device
		Param:OUT:lpszModelName:The version string buffer
			  IN:dwStrLen:The string buffer size
		Return:Return 0 if function succeeds, otherwise return an AS_Storage error code
	*/
	virtual LONG GetVersion(LPTSTR lpszVersion, DWORD dwStrLen) = 0;
	
	/*	Name:GetCapableTypes
		Introduce:Get the device capable type flag
		Param:OUT:dwCapableTypes:The media capable types flag of this device, it is a combination of eMediaKind
		Return:Return 0 if function succeeds, otherwise return an AS_Storage error code
	*/
	virtual LONG GetCapableTypes(DWORD &dwCapableTypes) = 0;	

	/*	Name:GetDataRates
		Introduce:Get data rates for this device
		Param:OUT:dwDataRates:Date rates array buffer
			  IN:dwArraySize:The size of dwDataRates array
		Return:Return 0 if function succeeds, otherwise return an AS_Storage error code
	*/
	virtual LONG GetDataRates(DWORD dwDataRates[], DWORD dwArraySize) = 0;
	//-------------------------------------------

	/*	Name:GetMediaKind
		Introduce:Get current media kind in this device
		Param:N/A
		Return:The media kind, it is a combination of eMediaKind
	*/
	virtual DWORD GetMediaKind() = 0;
	
	/*	Name:GetMedia
		Introduce:Get current media in this device
		Param:N/A
		Return:Return a CMedia object pointer
	*/
	virtual CMedia* GetMedia() = 0;
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