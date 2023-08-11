///////////////////////////////////////////////////////////////////////////////
// AS_AudioFile.h
// Copyright (c) 2001 - 2007, Sonic Solutions.  All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _AS_AudioFile
#define _AS_AudioFile

#include "AS_StorageError.h"
#include "AS_StorageTypes.h"


typedef UInt32 AS_AudioFile_Handle;

enum AS_AudioFile_Property
{
   // RawSize is the size in bytes of the uncompressed audio data expressed as
   // PCM CD-Audio. It is *not* rounded up to the nearest CD-DA sector size (2352).
   AudioFile_Property_RawSize = 0,   // Estimated size depending on source format (VBR, etc.).
   AudioFile_Property_RawSize_Exact  // Exact size (may take longer for VBR-type files).
};


#ifdef __cplusplus
extern "C" {
#endif

//-----------------------------------------------------------------------------
// AudioFile API Functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// AS_Open_AudioFile
//
// DESCRIPTION:
// Open an AS_AudioFile_Handle for the file specified by the given path.
//
// RETURN:
//  AS_StorageError_None             - ok
//  AS_StorageError_NotInitialized   - if the audio API could not be initialized
//  AS_StorageError_FileNotFound     - if the file could not be found
//  AS_StorageError_FileOpenFailed   - if the file could not be opened
//
AS_StorageError AS_API AS_Open_AudioFile(
   const AS_String &path,        // IN: full path to file
   AS_AudioFile_Handle &handle,  // OUT: file handle
   UInt32 reserved = 0,          // IN: reserved
   UInt32 reserved1 = 0);        // IN: reserved

//-----------------------------------------------------------------------------
// AS_AudioFile_GetProperty
//
// DESCRIPTION:
// Get properties from the file represented by the given AS_AudioFile_Handle.
//
// RETURN:
//  AS_StorageError_None             - ok
//  AS_StorageError_NotInitialized   - if the audio API could not be initialized
//  AS_StorageError_InvalidParameter - if one or more of the parameters are incorrect
//
AS_StorageError AS_API AS_AudioFile_GetProperty(
   const AS_AudioFile_Handle &handle,  // IN: file handle to get property from
   AS_AudioFile_Property property,     // IN: property requested
   UInt32 bufferSize,                  // IN: size of the property buffer in bytes
   void * propertyBuffer,              // OUT: buffer to hold the requested property data.
   UInt32 * retDataSize,               // OUT: actual amount (size) used of the output buffer
   UInt32 extPropertyIndex = 0,        // IN (Optional): index of extended property - only used for properties which are lists
   UInt32 extBufferSize = 0,           // IN (Optional): size in bytes of the extended buffer
   void * extPropertyBuffer = 0,       // IN/OUT (Optional): extended buffer to hold the requested extended property data
   UInt32 * extRetDataSize = 0);       // OUT (Optional): actual amount (size) used of the extended buffer

//-----------------------------------------------------------------------------
// AS_AudioFile_Close
//
// DESCRIPTION:
// Close an AS_AudioFile_Handle opened by AS_Open_AudioFile.
//
// RETURN:
//  AS_StorageError_None             - ok
//  AS_StorageError_NotInitialized   - if the audio API could not be initialized
//  AS_StorageError_InvalidParameter - if the handle is invalid
//
AS_StorageError AS_API AS_AudioFile_Close(AS_AudioFile_Handle &handle);

#ifdef __cplusplus
}
#endif
#endif  // _AS_AudioFile
