///////////////////////////////////////////////////////////////////////////////
// AS_StorageDevice_Internal.h - EmbC++ Version
// Copyright (c) 2001 - 2007, Sonic Solutions.  All rights reserved.
//
//  APIs to provide more detailed device functions
//
///////////////////////////////////////////////////////////////////////////////

#ifndef __AS_StorageDevice_Internal_h__
#define __AS_StorageDevice_Internal_h__

#include "AS_StorageDevice.h"

#ifdef __cplusplus
extern "C" 
{
#endif

struct AS_StorageDevice_Internal
{

	typedef UInt32 CloseTracksMode;
	static const CloseTracksMode CloseTracksMode_Specific			= 0;
	static const CloseTracksMode CloseTracksMode_All				= 1;
	static const CloseTracksMode CloseTracksMode_ExcludeFirst		= 2;
	static const CloseTracksMode CloseTracksMode_ExcludeFirstLast	= 3;

	typedef UInt32 BorderFlags;
	static const BorderFlags BorderFlags_Open	= 1;		// open border
	static const BorderFlags BorderFlags_Closed	= 2;		// closed border
	static const BorderFlags BorderFlags_Final	= 3;		// closed (finalized) disc
};

//-----------------------------------------------------------------------------
// Reserve a track w/ number and size
//
AS_StorageError AS_API AS_StorageDevice_ReserveTrack (
	const AS_StorageDevice	&		device,					// IN: device reference
	UInt32							trackNum,				// IN: track # to reserve
	UInt32							trackLen);				// IN: length track to reserve (in sectors)

//-----------------------------------------------------------------------------
// Close tracks on media
//
AS_StorageError AS_API AS_StorageDevice_CloseTracks (
	const AS_StorageDevice	&		device,					// IN: device reference
	AS_StorageDevice_Internal::CloseTracksMode flag,		// IN: flags for close tracks
	UInt32							trackNum,				// IN: track num (only used if closing specific track)
	AS_StorageDevice::InfoCallback callback,				// IN: progress callback function
	void* callerUse);										// IN: progress callback userdata

//-----------------------------------------------------------------------------
// Flush (commit) device - block until cache is emptied and sync cache is called
//
AS_StorageError AS_API AS_StorageDevice_Flush (
	const AS_StorageDevice	&		device);				// IN: device reference


//-----------------------------------------------------------------------------
// Close Border on media.
//
AS_StorageError AS_API AS_StorageDevice_CloseBorder (
	const AS_StorageDevice	&		device,					// IN: device reference
	AS_StorageDevice_Internal::BorderFlags			flag,	// IN: flags for border params
	AS_StorageDevice::InfoCallback callback,				// IN: progress callback function
	void* callerUse);										// IN: progress callback userdata


#ifdef __cplusplus
}
#endif

#endif //__AS_StorageDevice_Internal_h__
