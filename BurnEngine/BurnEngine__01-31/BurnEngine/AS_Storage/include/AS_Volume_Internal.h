///////////////////////////////////////////////////////////////////////////////
// AS_Volume_Internal.h - EmbC++ Version
// Copyright (c) 2001 - 2007, Sonic Solutions.  All rights reserved.
///////////////////////////////////////////////////////////////////////////////

#ifndef __AS_Volume_Internal_h__
#define __AS_Volume_Internal_h__

// Additional AS_Storage functionality only for internal Sonic use. Not to be
// exposed through public header files.

#include "AS_Volume.h"

#ifdef __cplusplus
extern "C" {
#endif

// For AS_Volume_SetProperty():
//    IN: AS_Volume_FS_Type
extern const enum AS_Volume::Property kAS_Volume_Prop_FileSystem;

// Create a file on a volume at a specified block. From a stream of data provided by callback.
// This kind of file is created empty but allocated and data requested when its Volume is Flushed
AS_StorageError AS_API AS_Volume_CreateFileAtBlock(		// RETURNS: error number
   const AS_Volume & volume,				// IN: volume reference
   const AS_File::Path& fullPath,					// IN: Name of file to create in directory
   UInt32 blockNumber,                  // IN: start block of file
   AS_File::Size streamLength,				// IN: Size of stream (and ultimately file on media)
   AS_Volume::StreamCallback callback,		// IN: stream callback function for file data
   void * userdata);  						// IN: stream callback userdata

AS_StorageError AS_API AS_Volume_CreateFileAtBlockFromPath(   // RETURNS: error number
   const AS_Volume & volume,				// IN: volume reference
   const AS_File::Path& fullPath,					// IN: Name of file to create in directory
   UInt32 blockNumber,                  // IN: start block of file
   const AS_File::Path & fullPathOnHD, // IN: path to hard disc file from which to take all information - not copied to media until Volume is Flushed.
   void * userdata);							// IN: stream callback userdata

// Special Flush just for D2D
// writes file system and if media is write-once, then preserve the old VAT and write it in
// the last sector
// allows D2D to still mount their old UDF session
AS_StorageError AS_API AS_Volume_FlushAndCloseBridgeSession( // RETURNS: error number
   const AS_Volume & volume,                 // IN: volume to flush the FS to disc
   AS_Volume::InfoCallback callback,  // IN: progress callback function
   void* userdata,                           // IN: progress callback userdata
   AS_Volume::Flush_Border_Options options);                // IN: optional flush parameters

// Mount only a specific folder in a volume.
// Do not traverse any other folders on volume.
// Mounts as read-only.
// If folder doesn't exist, volume will be empty.
AS_StorageError AS_API AS_Volume_MountFolder(       // RETURNS: error number
   const AS_Volume & volume,                 // IN: volume reference
   const AS_File::Path& folderPath,           // IN: folder to mount
   AS_Volume::InfoCallback callback,  // IN: progress callback function
   void* userdata);                          // IN: progress callback userdata

#ifdef __cplusplus
}
#endif
#endif // __AS_Volume_Internal_h__
