//-----------------------------------------------
//	ASMedia.cpp
//	The implement file for CASMedia
//-----------------------------------------------
#include "StdAfx.h"
#include ".\asmedia.h"
//-----------------------------------------------
//	Name:CASMedia
//	Introduce:Construction
//-----------------------------------------------
CASMedia::CASMedia(void)
{
}
//-----------------------------------------------
//	Name:~CASMedia
//	Introduce:Distruction
//-----------------------------------------------
CASMedia::~CASMedia(void)
{
}
//-----------------------------------------------


//-----------------------------------------------
//	Name:Open
//	Introduce:Open the volume
//-----------------------------------------------
SInt32 CASMedia::Open(const AS_StorageDevice &device, 
	AS_Volume::InfoCallback Callback, LPVOID lpUserData)
{
	AS_Volume::FS_Type volType;
	AS_StorageError err = AS_StorageError_None;

	err = AS_StorageDevice_OpenVolume(device, 
					volType, 
					Callback, 
					lpUserData, 
					m_ASVolume);
	if(AS_StorageError_None != err)
		return err;

	return AS_StorageError_None;
}
//-----------------------------------------------
//	Name:Format
//	Introduce:Format this volume
//-----------------------------------------------
SInt32 CASMedia::Format(AS_Volume::FS_Type FormatType, LPCTSTR lpszLabel,
	AS_Volume::AccessFlag Access, AS_Volume::InfoCallback Callback, LPVOID lpUserData)
{
	//Get the label string
	char* pszLabel = NULL;
	int iLabelLen = lstrlen(lpszLabel);
	if(iLabelLen > 0)
	{
#ifdef UNICODE
		pszLabel = new char[2*iLabelLen+1];
		WideCharToMultiByte(CP_ACP, 0, lpszLabel, -1, pszLabel, 2*iLabelLen, NULL, NULL);
		pszLabel[2*iLabelLen] = '\0';
#else
		pszLabel = new char[iLabelLen+1];
		strcpy(pszLabel, lpszLabel);
		pszLabel[iLabelLen] = '\0';
#endif	//UNICODE
	}

	//Format the volume by creating
	AS_StorageError err = AS_StorageError_None;	
	err = AS_Volume_Create(	m_ASVolume, 
							FormatType,				//File format type
							AS_StringA(pszLabel),	//Volume label string
							Access,					//Access type
							Callback,				//Information callback
							lpUserData);			//Callback user data
	//Delete the buff
	if(NULL != pszLabel)
	{
		delete []pszLabel;
		pszLabel = NULL;
	}

	return err;
}
//-----------------------------------------------
//	Name:Mount
//	Introduce:Mount this volume
//-----------------------------------------------
SInt32 CASMedia::Mount(AS_Volume::AccessFlag Access, AS_Volume::InfoCallback Callback, LPVOID lpUserData)
{
	AS_StorageError err = AS_StorageError_None;

	err = AS_Volume_Mount(	m_ASVolume, 
							Access,					//Access type
							Callback,				//Information callback
							lpUserData);			//Callback user data
	if (AS_StorageError_None != err)
		return err;

	return AS_StorageError_None;
}
//-----------------------------------------------	
//	Name:CopyFile
//	Introduce:Copy a file from hard disc to this volume
//-----------------------------------------------	
SInt32 CASMedia::CopyFile(LPCTSTR lpszSrcEntry, LPCTSTR lpszDesEntry)
{
	if(NULL == lpszSrcEntry || lstrlen(lpszSrcEntry) == 0)
		return AS_StorageError_FolderNotFound;

	TCHAR szDesEntry[MAX_PATH] = {_T("\0")};
	lstrcpy(szDesEntry, lpszDesEntry);

	//In case no target path supplied - make one from src path without drive letter
	AS_StorageError err = AS_StorageError_None;
	if(lstrcmp(szDesEntry, _T("/")) == 0 || lstrlen(szDesEntry) == 0)
	{
		LPCTSTR lpPos = lpszSrcEntry;
		while(*lpPos != _T('\\') && *lpPos != _T('\0')) lpPos++;
		if(*lpPos == _T('\0'))
			lstrcpy(szDesEntry, lpszSrcEntry);
		else
			lstrcpy(szDesEntry, lpPos);
	}

	//Get the file name strings
	char cSrcFile[MAX_PATH] = {"\0"};
	char cDesFile[MAX_PATH] = {"\0"};
#ifdef UNICODE
	WideCharToMultiByte(CP_ACP, 0, lpszSrcEntry, -1, cSrcFile, MAX_PATH, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, szDesEntry, -1, cDesFile, MAX_PATH, NULL, NULL);
#else
	strcpy(cSrcFile, lpszSrcEntry);
	strcpy(cDesFile, szDesEntry);
#endif	//UNICODE

	AS_File::CreateFlags flags = AS_File::Create_Parents | AS_File::Create_Overwrite;
	err = AS_Volume_CreateEntryFromHD(m_ASVolume, AS_StringA(cDesFile), AS_StringA(cSrcFile), flags);
	if (AS_StorageError_None != err)
		return err;

	return AS_StorageError_None;
}
//-----------------------------------------------		
//	Name:Flush
//	Introduce:Flush this volume, it just to say we will commit the changes
//-----------------------------------------------	
SInt32 CASMedia::Flush(AS_Volume::Flush_Border_Options Options,
		AS_Volume::InfoCallback Callback, LPVOID lpUserData)
{
	AS_StorageError err = AS_StorageError_None;

	// Flush / commit changes to disc;
	err = AS_Volume_Flush(	m_ASVolume, 
							Callback, 
							lpUserData, 
							Options);
	if (AS_StorageError_None != err)
		return err;

	return AS_StorageError_None;
}
//-----------------------------------------------		
//	Name:Close
//	Introduce:Close this volume
//-----------------------------------------------		
SInt32 CASMedia::Close()
{
	AS_StorageError err = AS_StorageError_None;

	err = AS_Volume_Close(m_ASVolume);
	if (AS_StorageError_None != err)
		return err;

	return AS_StorageError_None;
}
//-----------------------------------------------		


