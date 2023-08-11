///////////////////////////////////////////////////////////////////////////////
// AS_Storage_Option.h - EmbC++ Version
// Copyright (c) 2001 - 2005, Sonic Solutions.  All rights reserved.
///////////////////////////////////////////////////////////////////////////////
#ifndef		__AS_Storage_Option_h__
#define		__AS_Storage_Option_h__

#include "AS_StorageDevice.h"
#include "AS_StorageDevice_Authentication.h"
#include "AS_StorageDevice_Internal.h"
#include "AS_StorageDevice_Raw.h"
#include "AS_Volume_Image.h"
#include "AS_Volume_Internal.h"
#include "AS_Format.h"
#include "AS_Audio.h"
#include "AS_AudioFile.h"

//-----------------------------------------------------------------------------
// AS_Audio.h implementation functions
//-----------------------------------------------------------------------------
AS_StorageError Format_AddAudioTrackFromHD(  // RETURNS: error number
   const AS_Format & format,              // IN: layout reference
   const AS_File::Path & fullPathOnHD);   // IN: path to hard disc file from which to copy track data - not copied to media until Volume is Flushed.

AS_StorageError Format_AddAudioTrackFromStream(  // RETURNS: error number
   const AS_Format & format,              // IN: layout reference
   const AS_File::Size & streamLength,    // IN: Size of stream (and ultimately file on media)
   AS_Volume::StreamCallback callback,    // IN: stream callback function for track data
   void * callerUse);                     // IN: stream callback userdata

AS_StorageError Format_AddAudioTrackFromFile(  // RETURNS: error number
   const AS_Format & format,             // IN: layout reference
   const AS_File::Path & fullPath,       // IN: full path
   const AS_Audio::FileType & fileType,  // IN: source (file) format
   UInt32 & track);                      // OUT: track index reported back, 1 = first track

AS_StorageError Format_AddAudioTrackFromStream2(  // RETURNS: error number
   const AS_Format & format,             // IN: layout reference
   const AS_File::Size & streamLength,   // IN: size of stream (and ultimately file on media)
   AS_Volume::StreamCallback callback,   // IN: stream callback function for track data
   void * callerUse,                     // IN: stream callback userdata
   const AS_Audio::FileType & dataType,  // IN: source (stream data) format
   UInt32 & track);                      // OUT: track index reported back, 1 = first track

AS_StorageError Format_SetAudioTrackProperty(
   const AS_Format & format,               // IN: layout reference
   const AS_Audio::Track_Property & prop,  // IN: audio property to set
   UInt32 trackNumber,                     // IN: track number, 1 = first track
   UInt32 propSize,                        // IN: size of data in propData buffer
   void * propData,                        // IN: property data
   UInt32 extPropIndex = 0,                // IN: reserved - index of extended property to return
   UInt32 extPropSize = 0,                 // IN: reserved - size available in extPropData buffer
   void * extPropData = 0);                // OUT: reserved - buffer to copy extended property value into

AS_StorageError Format_AddAudioTrackEffect(
   const AS_Format & format,           // IN: layout reference
   UInt32 trackNumber,                 // IN: track number to add the effect to, 1 = first track
   const AS_Audio::EffectType & type,  // IN: type of effect, i.e. Normalize
   UInt32 parametersSize,              // IN: size of the 'parameters' buffer in bytes
   void * parameters);                 // IN: optional parameters for the specified effect type 

AS_StorageError Format_ExtractAudioTrackToFile( 
   const AS_Format & format,                        // IN: layout reference
   UInt32 trackNumber,                              // IN: track number, 1 = first track
   const AS_File::Path & fullPath,                  // IN: the full path of the file
   UInt32 flags,                                    // IN: reserved (must be zero) - extraction flags
   SInt32 startOffset,                              // IN: 0 = start address, not including pregap
   SInt32 length,                                   // IN: -1 for entire track
   const AS_Audio::Track_Tags & tags,               // IN: optional file tags, ignored for WAV output
   const AS_Audio::Track_EncoderSettings & encode,  // IN: encoding settings
   AS_Volume::InfoCallback callback,                // IN: progress callback function
   void * callerUse,                                // IN: client callback parameter
   UInt32 * reserved = 0);                          // OUT: reserved, must be NULL

AS_StorageError Format_SetWMAReaderCallback(
   UInt32 flags,                                 // IN: Reserved flags (must be zero)
   AS_Audio::CreateWMAReaderCallback callback);  // IN: AS_Audio::CreateWMAReaderCallback

AS_StorageError Format_CreateAudioPlaylist(
   const AS_Format & format);  // IN: layout reference

AS_StorageError Format_CheckAudioPlaylist(
   const AS_Format & format,  // IN: layout reference
   UInt32 bufferSizeBytes,    // IN: Size of result buffer in bytes
   UInt32 * results);         // OUT: Array of results in playlist order

AS_StorageError Format_CloseAudioPlaylist(
   const AS_Format & format);  // IN: layout reference


//-----------------------------------------------------------------------------
// AS_AudioFile.h implementation functions
//-----------------------------------------------------------------------------
AS_StorageError Open_AudioFile(
   const AS_String &path,        // IN: full path to file
   AS_AudioFile_Handle &handle,  // OUT: file handle
   UInt32 reserved = 0,          // IN: reserved
   UInt32 reserved1 = 0);        // IN: reserved

AS_StorageError AudioFile_GetProperty(
   const AS_AudioFile_Handle &handle,  // IN: file handle to get property from
   AS_AudioFile_Property property,     // IN: property requested
   UInt32 bufferSize,                  // IN: size of the property buffer in bytes
   void * propertyBuffer,              // OUT: buffer to hold the requested property data.
   UInt32 * retDataSize,               // OUT: actual amount (size) used of the output buffer
   UInt32 extPropertyIndex = 0,        // IN (Optional): index of extended property - only used for properties which are lists
   UInt32 extBufferSize = 0,           // IN (Optional): size in bytes of the extended buffer
   void * extPropertyBuffer = 0,       // IN/OUT (Optional): extended buffer to hold the requested extended property data
   UInt32 * extRetDataSize = 0);       // OUT (Optional): actual amount (size) used of the extended buffer

AS_StorageError AudioFile_Close(AS_AudioFile_Handle &handle);

//-----------------------------------------------------------------------------
// AS_StorageDevice_Raw.h implementation functions
//-----------------------------------------------------------------------------

AS_StorageError StorageDevice_OpenRawAccess(				// RETURNS: error number
    AS_StorageDevice &  device,					// IN: device reference
	AS_StorageDevice_Raw::OpenRawMode OpenMode,
	AS_StorageDevice_Raw::Access_Type AccessType);

AS_StorageError StorageDevice_CloseRawAccess(				// RETURNS: error number
    const AS_StorageDevice &  device,              // IN: device reference
	AS_StorageDevice::InfoCallback callback,				// IN: progress callback function
	void* callerUse);										// IN: progress callback userdata

//-----------------------------------------------------------------------------
//
AS_StorageError StorageDevice_SetMode(					// RETURNS: error number
    const AS_StorageDevice &   device,						// IN: device reference
	AS_StorageDevice_Raw::WriteMode		mode);

//-----------------------------------------------------------------------------
//
AS_StorageError StorageDevice_GetMode(					// RETURNS: error number
    const AS_StorageDevice &    device,					// IN: device reference
	AS_StorageDevice_Raw::WriteMode &     mode);

//-----------------------------------------------------------------------------
//
AS_StorageError StorageDevice_Read(						// RETURNS: error number
    const AS_StorageDevice &  device,						// IN: device reference
    UInt32             blkNum,						// IN: block offset on media
    UInt32             bufLen,						// IN: length of data to read
    void *             buf,							// OUT: Data read
	UInt32*	           numRead);					// OUT: length of data read (in sectors)

//-----------------------------------------------------------------------------
// Get device state - recognize state of the device.
//
AS_StorageError StorageDevice_Write(						// RETURNS: error number
    const AS_StorageDevice &    device,					// IN: device reference
    UInt32               blkNum,					// IN: block offset on media
    UInt32               bufLen,					// IN: length of data to read
    void *               buf,						// IN: Data to write to offset
	UInt32*	             numWritten);				// OUT: length of data written (in sectors)

//-----------------------------------------------------------------------------
// Write an image file to device.  The device state must be ready.
//
AS_StorageError StorageDevice_WriteImage(					// RETURNS: error number
    const AS_StorageDevice &			device,		// IN: device reference
	enum AS_StorageDevice_Raw::Image_Type	image_type,	// IN: type of image (ie AS_StorageDevice_Raw::Image_ISO)
    UInt32						flags,				// IN: flags (0)
    AS_String					filepath,			// IN: path to image file
	AS_StorageDevice::InfoCallback callback,				// IN: progress callback function
	void* callerUse);										// IN: progress callback userdata

//-----------------------------------------------------------------------------
// Prepare to write
AS_StorageError StorageDevice_PrepareWrite(				// RETURNS: error number
    const AS_StorageDevice &				device,	// IN: device reference
    AS_StorageDevice_Raw::PrepareMode 			mode);	// IN: flags for prepare params

//-----------------------------------------------------------------------------
// Async read blocks
//
AS_StorageError StorageDevice_ReadAsync(					// RETURNS: error number
	const AS_StorageDevice	&		device,			// IN: source device
    UInt32							blkNum,			// IN: block offset on media
	UInt32							bufLen,			// IN:  length of data to read  (in sectors)
	void*							buf,			// OUT: Pointer to the data read
	UInt32*							numRead);		// OUT: length of data read  (in sectors)

//-----------------------------------------------------------------------------
// Get buffer from engine.  If none available, will block
AS_StorageError StorageDevice_GetBuffer(			// RETURNS: error number
    const AS_StorageDevice &		device,			// IN: device reference
	void**							buf);			// OUT: Pointer to the buffer

//-----------------------------------------------------------------------------
// Async write blocks
//
AS_StorageError StorageDevice_WriteAsync(
	const AS_StorageDevice	&		device,			// IN: dest device
    UInt32							blkNum,			// IN: block offset on media
	UInt32							bufLen,			// IN:  length of data to write (in sectors)
	void*							buf,			// OUT: Pointer to the data to write
	UInt32*							numWritten,		// OUT: length of data written  (in sectors)
	UInt32*							complete);

//-----------------------------------------------------------------------------
// AS_StorageDevice_Internal.h implementation functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Reserve a track w/ number and size
//
AS_StorageError StorageDevice_ReserveTrack (
	const AS_StorageDevice	&		device,				// IN: device reference
	UInt32							trackNum,			// IN: track # to reserve
	UInt32							trackLen);			// IN: length track to reserve (in sectors)

//-----------------------------------------------------------------------------
// Close tracks on media
//
AS_StorageError StorageDevice_CloseTracks (
	const AS_StorageDevice	&				device,				// IN: device reference
	AS_StorageDevice_Internal::CloseTracksMode		flag,				// IN: flags for close tracks
	UInt32									trackNum,			// IN: track num
	AS_StorageDevice::InfoCallback callback,				// IN: progress callback function
	void* callerUse);										// IN: progress callback userdata

//-----------------------------------------------------------------------------
// Flush (commit) device - block until cache is emptied and sync cache is called
//
AS_StorageError StorageDevice_Flush (
	const AS_StorageDevice	&				device);			// IN: device reference

//-----------------------------------------------------------------------------
// Close Border on media.
//
AS_StorageError StorageDevice_CloseBorder (
	const AS_StorageDevice	&				device,				// IN: device reference
	AS_StorageDevice_Internal::BorderFlags			flag,				// IN: flags for border params
	AS_StorageDevice::InfoCallback callback,				// IN: progress callback function
	void* callerUse);										// IN: progress callback userdata


//-----------------------------------------------------------------------------
// AS_Volume_Image.h implementation functions
//-----------------------------------------------------------------------------

// Open a volume on a device and determine the logical format
AS_StorageError OpenVolumeImage(					// RETURNS: error number
   const AS_File::Path& pathToImageFile,		// IN: device reference
   AS_Volume::FS_Type& kind,					// OUT: kind of volume detected
   AS_Volume & volume);						// OUT: volume reference



//-----------------------------------------------------------------------------
// AS_Volume_Internal.h implementation functions
//-----------------------------------------------------------------------------

// Create a file on a volume at a specified block. From a stream of data provided by callback.
// This kind of file is created empty but allocated and data requested when its Volume is Flushed
AS_StorageError Volume_CreateFileAtBlock(			// RETURNS: error number
   const AS_Volume& volume,					// IN: volume reference
   const AS_File::Path& fullPath,			// IN: Name of file to create in directory
   UInt32 blockNumber,						// IN: start block of file
   AS_File::Size streamLength,				// IN: Size of stream (and ultimately file on media)
   AS_Volume::StreamCallback callback,			// IN: stream callback function for file data
   void * userdata);  						// IN: stream callback userdata

// Create a file on a volume at a specified block. From an existing file on local storage.
// This kind of file is created empty but allocated and data requested when its Volume is Flushed
AS_StorageError Volume_CreateFileAtBlockFromPath(   // RETURNS: error number
   const AS_Volume & volume,				// IN: volume reference
   const AS_File::Path& fullPath,					// IN: Name of file to create in directory
   UInt32 blockNumber,                  // IN: start block of file
   const AS_File::Path & fullPathOnHD, // IN: path to hard disc file from which to take all information - not copied to media until Volume is Flushed.
   void * userdata);							// IN: stream callback userdata

// Special Flush just for D2D
// writes file system and if media is write-once, then preserve the old VAT and write it in
// the last sector
// allows D2D to still mount their old UDF session
AS_StorageError Volume_FlushAndCloseBridgeSession( // RETURNS: error number
   const AS_Volume& volume,                 // IN: volume to flush the FS to disc
   AS_Volume::InfoCallback callback, // IN: progress callback function
   void* userdata,                          // IN: progress callback userdata
   const AS_Volume::Flush_Border_Options& options);        // IN: optional flush parameters

// Mount only a specific folder in a volume.
// Do not traverse any other folders on volume.
// Mounts as read-only.
// If folder doesn't exist, volume will be empty.
AS_StorageError Volume_MountFolder(				// RETURNS: error number
   const AS_Volume& volume,                 // IN: volume reference
   const AS_File::Path& folderPath,          // IN: folder to mount
   AS_Volume::InfoCallback callback, // IN: progress callback function
   void* userdata);                         // IN: progress callback userdata

AS_StorageError Volume_Copy(						// RETURNS: error number
   const AS_Volume & volume, 				// IN: volume to flush the FS to disc
   const AS_File::Path& srcPath,				// IN: Name of path to copy from Media
   const AS_File::Path& destPath,			// IN: Name of destinationPath
   AS_Volume::InfoCallback callback, // IN: progress callback function
   void* userdata);                         // IN: progress callback userdata

//-----------------------------------------------------------------------------
// AS_Storage_Authentication.h implementation functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Send Key - Authentication purposes
//
AS_StorageError StorageDevice_SendKey(					// RETURNS: error number
    const AS_StorageDevice & device,                      // IN: device reference
	UInt32 SendKeyDataSize,                         // IN: size of SendKeyData
	UInt8* SendKeyData,                             // IN: your Key Data for the drive.
	UInt8  KeyFormat,                               // IN: proprietary to Authentication
	UInt8  AGID,                                    // IN: proprietary to Authentication
	UInt8  KeyClass                                 // IN: proprietary to Authentication
	);

//-----------------------------------------------------------------------------
// Report Key - Authentication purposes
//
AS_StorageError StorageDevice_ReportKey(					// RETURNS: error number
    const AS_StorageDevice & device,                       // IN: device reference
	UInt32 ReportKeyDataSize,                       // IN: size of SendKeyData
	UInt8* ReportKeyData,                           // OUT: your Key Data for the drive.
	UInt32 Lba,                                     // IN: Logical Block Address for report key.
	UInt8  KeyFormat,                               // IN: proprietary to Authentication
	UInt8  AGID,                                    // IN: proprietary to Authentication
	UInt8  KeyClass,                                // IN: proprietary to Authentication
	UInt8  BlockCount                               // IN: proprietary to Authentication
	);


//-----------------------------------------------------------------------------
AS_StorageError StorageDevice_GetKeyInfo (				// RETURNS: error number
	const AS_StorageDevice        &device,                 // IN: device reference
	UInt32                 KeyDataSize,             // IN: size of "KeyData"
	void                  *KeyData,                 // OUT: key info read from drive.
	AS_StorageDevice_KeyInfo_Type Type,                    // IN: type of read dvd struct.
	UInt32                 Address,                 // IN: address or LBA, if any 
	UInt32                 AGID = 0,                // IN: AGID you used for authentication, if any
	UInt32                 LayerNum = 0,            // layer number to get info about.
	UInt32                 Reserved1 = 0, 
	UInt32                 Reserved2 = 0,
	UInt32                 Reserved3 = 0);


#endif //__AS_Storage_Option_h__
