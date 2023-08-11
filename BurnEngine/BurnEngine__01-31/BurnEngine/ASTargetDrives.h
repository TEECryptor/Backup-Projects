//-----------------------------------------------
//	ASTargetDrives.h
//	Copyright (c) 2001 - 2008, Sonic Solutions.  All rights reserved.
//-----------------------------------------------
#ifndef _ASTARGETDRIVES_H_
#define _ASTARGETDRIVES_H_
//-----------------------------------------------
#include "ASDrives.h"
//-----------------------------------------------
//	Name:CASTargetDrives
//	Introduce:This class defines target dreives list
//-----------------------------------------------
class CASTargetDrives : public CASDrives
{
public:
	CASTargetDrives(CASDriveManager* pDriveManager);
	~CASTargetDrives(void);
	//-------------------------------------------
public:
	//Add devices to this target devices object
	BOOL	AddDevices();
	//Add which drive for burning target
	LONG	AddBurnTarget(int iDriveID);
	//Add an image file as burning target
	LONG	AddBurnTarget(LPCTSTR lpszImageFile);
	//Remove a drive from selected list
	void	RemoveTarget(int iDriveID, BOOL bRemoveAll = FALSE);
	//-------------------------------------------
	//The all selected target can write DVD or not
	BOOL	CanWriteDVD();
	//Return the selected targets count
	int		GetNumSelectedTargets();
	//Get the target drive ID by index
	int		GetSelectedTargetID(int iIndex);
	//Get the image file name of current image drive
	LPCTSTR GetImageFileName();
	//-------------------------------------------
private:
	IDArrayPtr	m_lstSelTargetID;		//Current selected target drives IDs list
	CASDrive*	m_pImageTargetDrive;	//Target drive for image file burning
	TCHAR		m_szImageFile[MAX_PATH];//Image file name
	//
	int			m_iNumOfCopies;			//The number of copies
	BOOL		m_bIsCompatible;		//The selected media type are compatible or not
	//-------------------------------------------
/*
private:
	unsigned long _GetTargetDVDSize(void);
	
	RetVal  _SetTargetRecordingOptions(IProject& Project,unsigned long CopySpeed, unsigned long NumOfCopies,enumRecordMode RecordMode);
	RetVal _SetBufferUnderrun(IProject& Project,bool bEnable);
	//CAtlList<int>  m_TargetIDList;
	//wstring m_strFilename;
	//wstring m_strImageFilewithMultipleTargets;
	CBurnEngine *m_pBurnEngine;
	// gets a list of available writing speeds
	ULONG _GetWriteSpeeds(CAtlList<wstring>& ListWriteSpeedStrings,CAtlList<ULONG>& ListWriteSpeedIndex,IProject &Project);
	enumDiscType _GetDiscInfo(IDevice * pDevice, wstring& strLabel,bool &bLabelFound,bool bNoCompatibilityCheck);
public:
	CTargetDrives(CComCriticalSection &CriticalSection,CBurnEngine *pBurnEngine);
	~CTargetDrives(void);
public:
	void   RemoveTargets(IProject& Project);
	RetVal AddBurnTarget(IProject& Project,wstring strImageName);
	RetVal AddBurnTarget(IProject& Project,int id);
	// add one additional Target drive
	RetVal AddBurnTarget(int id,bool bAddtoListOnly=false);
	RetVal AddBurnTarget(wstring strImageName);
	// remove all burning targets
	void RemoveTargets(bool bDeleteIdList=true);
	// sets all recording options necessary to write disc
	RetVal SetTargetRecordingOptions(unsigned long CopySpeed, unsigned long NumOfCopies,enumRecordMode RecordMode);
	RetVal CTargetDrives::SetBufferUnderrun(bool bEnable);
	int GetNumOfCopies(void);
	bool IsTargetImage(void);
	bool GetImageFilename(wstring& strImageFilename);
	bool IsCompatibleMedia(void);
	RetVal AddVRBurnTarget(wstring strImageName);
	bool SuspendTargets(bool bSuspend);
	bool DisableMedia(ULONG lListToDisable[],ULONG ulNumItemsList);
	bool CanWriteRaw(void);
	unsigned long GetDVDSize(void);
	bool SupportBufferUnderrunProtection(void);
	bool CanTestRawmode(void);
	bool IsDVDInSelectedDrive(int id);
	enumRawModeEnable IsRawCopyPossible();
	int GetDiscType(int& nDiscType);
	ULONG GetWriteSpeeds(bool bIsSoureImage,CAtlList<wstring>& ListWriteSpeedStrings,CAtlList<ULONG>& ListWriteSpeedIndex);
	// retrieves the required space that must be available on the target location for output to the selected image file type
	int GetImageSize(unsigned _int64& ui64TargetImageSize,wstring strFolder);
	// looks up which kind of disc is inserted 
	enumDiscType GetDiscInfo(int id, wstring& strLabel,bool &bLabelFound,bool bNoCompatibilityCheck);
	// //sets recording options needed to record properly
	RetVal ReloadTargetsFromProject(IProject *pProject);
	bool IsDVDDiscdashRDualLayer();
	int ReInitBurnTarget(void);
*/
};
//-----------------------------------------------
#endif	//_ASTARGETDRIVES_H_
//-----------------------------------------------

