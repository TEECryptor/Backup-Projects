//-----------------------------------------------
//	ASMedia.h
//	Copyright (c) 2001 - 2008, Sonic Solutions.  All rights reserved.
//-----------------------------------------------
#ifndef _ASMEDIA_H_
#define _ASMEDIA_H_
//-----------------------------------------------
#include "AS_Volume.h"
#include "AS_StorageError.h"
#include "AS_StringHelper.h"
//-----------------------------------------------
//	Name:CASMedia
//	Introduce:This class define a AS_Volume object for media
//-----------------------------------------------
class CASMedia
{
public:
	CASMedia(void);
	~CASMedia(void);
	//-------------------------------------------
public:
	//Open this volume
	SInt32		Open(const AS_StorageDevice & device, AS_Volume::InfoCallback Callback, LPVOID lpUserData);
	//Format this volume
	SInt32		Format(AS_Volume::FS_Type FormatType, LPCTSTR lpszLabel, AS_Volume::AccessFlag Access, AS_Volume::InfoCallback Callback, LPVOID lpUserData);
	//Mount this volume
	SInt32		Mount(AS_Volume::AccessFlag Access, AS_Volume::InfoCallback Callback, LPVOID lpUserData);
	//Copy a file from hard disc to this volume
	SInt32		CopyFile(LPCTSTR lpszSrcEntry, LPCTSTR lpszDesEntry);
	//Flush this volume, it just to say we will commit the changes
	SInt32		Flush(AS_Volume::Flush_Border_Options Options, AS_Volume::InfoCallback Callback, LPVOID lpUserData);
	//Close this volume
	SInt32		Close();
	//-------------------------------------------
private:
	AS_Volume	m_ASVolume;
	//-------------------------------------------
};
//-----------------------------------------------
#endif //_ASMEDIA_H_
//-----------------------------------------------
