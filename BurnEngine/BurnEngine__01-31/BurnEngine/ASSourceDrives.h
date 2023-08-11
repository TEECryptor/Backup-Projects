//-----------------------------------------------
//	ASSourceDrives.h
//	Copyright (c) 2001 - 2008, Sonic Solutions.  All rights reserved.
//-----------------------------------------------
#ifndef _ASSOURCEDRIVES_H_
#define _ASSOURCEDRIVES_H_
//-----------------------------------------------
#include "ASDrives.h"
//-----------------------------------------------
//	Name:CASSourceDrives
//	Introduce:This class defines source dreives list
//-----------------------------------------------
class CASSourceDrives: public CASDrives
{
public:
	CASSourceDrives(CASDriveManager* pDriveManager);
	~CASSourceDrives(void);
	//-------------------------------------------
public:
	//Add which drive for burning source
	LONG	AddBurnSource(int iDriveID);
	//Add an image file as burning source
	LONG	AddBurnSource(LPCTSTR lpszImageFile);
	//-------------------------------------------
	//Return current active source device
	CASDrive*	GetActiveDevice();
	//-------------------------------------------
private:
	int			m_iActiveID;			//The active source device ID
	TCHAR		m_szImageFile[MAX_PATH];//Image file name
	//-------------------------------------------
/*
private:
	wstring m_strSourceFile;
	int			m_ActID;
	bool		m_bDiscPartiallyWritten;	// set to true by _IsDirectCD if the disc has a reserved or partially written track
	RetVal _AddBurnSource(IProject& Project,int id);
	bool _SetCacheMode(IProject& Project,long Cachmode,wstring &strCachePath);
	enumDiscType _GetDiscInfo(IDevice * pDevice, wstring& strLabel,bool &bLabelFound);
	
	// retrieves basic disc information parameters for display in summary text field
	RetVal _GetDiscParameters(wstring & strLabel , int & nNumTracks, int & nDiscType, DWORD & dwTotalBlocks, wstring & wstrDescription, DWORD& dwDataSizeInMB, DWORD& dwAudioPlaytime, int& nNumAudioTracks);
	FileServerLib::IFileServer *m_pFileServer;	
	CBurnEngine *m_pBurnEngine;
public:
	RetVal GetDiscImageSize(wstring filename , unsigned long & dwTotalBlocks, unsigned _int64 *pnEstimatedFileSize);;
	// // set the source to be copied from
	RetVal AddBurnSource(wstring& strFileName);
	RetVal AddBurnSource(int id);
	// //true if source is an image file
	bool IsSourceImage(void);
	bool GetImageName(wstring& strImageFileName);
	bool SetCacheMode(long Cachmode,wstring &strCachePath);
	bool GetDeviceName(int id,wstring& strDriveLetter, wstring& strVendorName, wstring& strProductName);
	bool GetDeviceName(wstring& strDriveLetter, wstring& strVendorName, wstring& strProductName);
	IDevice * GetActDevice(void);
	ULONG GetReadSpeeds(CAtlList<wstring>& ListReadSpeedStrings,CAtlList<ULONG>& ListReadSpeedIndex);
	bool ApplyReadSpeed(int ReadSpeedMode, ULONG ReadSpeed);
	bool _ApplyReadSpeed(IProject& Project,int ReadSpeedMode, ULONG ReadSpeed);
	int GetActDeviceID(void);
	bool GetActDevice(wstring & strPath);
	wstring GetVolumeLabel(void);
	bool IsImageRawmode(void);
	DWORD GetSizeInBlocks(wstring strFolder=_T(""));
	void ForceNoCache(void);
	bool IsDVDInSelectedDrive(void);
	bool IsMultiBorderDVD(void);
	int GetDVDTrackCount(void);
	bool DoesSupportAudioRawRead();
	bool DoesSupportDataRawRead();
	bool IsDirectCDInSelectedDrive(void);
	bool IsDiscPartiallyWritten(void);
	int GetDiscType(int& nDiscType);
	bool IsReservedByD2D(void);
	// retrieves basic disc information parameters for display in summary text field
	RetVal GetDiscParameters(wstring & strLabel , int & nNumTracks, int & nDiscType, DWORD & dwTotalBlocks, wstring & wstrDescription, DWORD& dwDataSizeInMB, DWORD& dwAudioPlaytime, int& nNumAudioTracks);
	// retrieves basic disc image information parameters for display in summary text field
	RetVal _GetDiscImageParameters(wstring filename , wstring & strLabel, int & nNumTracks, int & nDiscType, unsigned long & dwTotalBlocks, wstring & wstrDescription, DWORD& dwDataSizeInMB, DWORD& dwAudioPlaytime, int& nNumAudioTracks);
	// looks up which kind of disc is inserted 
	enumDiscType GetDiscInfo(int id, wstring& strLabel,bool &bLabelFound);
	long GetCacheMode(IProject& Project,LPTSTR strCachePath);
	bool ForceCache(void);
*/
};
//-----------------------------------------------
#endif	//_ASSOURCEDRIVES_H_
//-----------------------------------------------
