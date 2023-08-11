///////////////////////////////////////////////////////////////////////////////
// AS_StorageDevice_Raw.h - EmbC++ Version
// Copyright (c) 2001 - 2007, Sonic Solutions.  All rights reserved.
//
//  APIs to read & write raw blocks on a media
//
///////////////////////////////////////////////////////////////////////////////

#ifndef __AS_StorageDevice_Raw_h__
#define __AS_StorageDevice_Raw_h__

#include "AS_StorageDevice.h"

#ifdef __cplusplus
extern "C" 
{
#endif

struct AS_StorageDevice_Raw
{

   //---------------------------------------------------------------------------------------------------------------
   // Image Type properties for use in writing an image (AS_StorageDevice_WriteImage and AS_StorageDevice_WriteImage_WCS)
   enum Image_Type    // Type                          Description
	{								//----------------------------------------------------------------------
		Image_ISO = 1,				// ISO							Standard 1 track 2048-byte-per-block image.
		Image_MovieDisc = 2,		// MovieDisc					Can be 2048-bytes-per-block or 2054, XML specifies parameters
		Image_CopyBlock = 4,		// CopyBlock					2048 bytes-per-block, bad blocks marked according to CopyBlock spec.
	};

	//---------------------------------------------------------------------------------------------------------------
	// Optional flags for (AS_StorageDevice_WriteImage and AS_StorageDevice_WriteImage_WCS)
	static const UInt32 WriteImageFlags_None = 0;
	static const UInt32 WriteImageFlags_Verify = 1;

    typedef UInt32 RawCopyFlags;
	static const RawCopyFlags RawCopyFlags_None		=  (1 << 0);
	static const RawCopyFlags RawCopyFlags_ISO		=  (1 << 1);
	static const RawCopyFlags RawCopyFlags_GI			=  (1 << 2);
	static const RawCopyFlags RawCopyFlags_MovieDisc	=  (1 << 8);
	static const RawCopyFlags RawCopyFlags_CopyBlock	=  (1 << 9);

	enum PrepareMode
	{
		PrepareMode_None				= 0,	// disable streaming bit for writes
		PrepareMode_Write_Streaming	= 1		// enable Streaming bit for writes
	};

	typedef UInt32 OpenRawMode;
	static const OpenRawMode OpenRawMode_Exclusive = 1;			// exclusive access to media - prevent others from access
	static const OpenRawMode OpenRawMode_ReadWrite = 2;			// open media in read/write mode
	static const OpenRawMode OpenRawMode_Read      = 3;			// open media in read only mode
	static const OpenRawMode OpenRawMode_ForceRefresh = 0x80000000; // force refresh (used mostly for copy operations)


	enum WriteMode
	{
		WriteMode_RestrictedOverwrite     = 1,
		WriteMode_Packet                  = 2
	};

	typedef UInt32 Access_Type;
	static const Access_Type Access_Sync	= 0;  // access device for synchronous direct access
	static const Access_Type Access_Async	= 1;  // access device for asynchronous job access

    typedef UInt32 SendCommand_Flags;
    static const SendCommand_Flags SendCommand_DataIn  = 1;
    static const SendCommand_Flags SendCommand_DataOut = 2;


};


//-----------------------------------------------------------------------------
// Open device for raw access
//
AS_StorageError AS_API AS_StorageDevice_OpenRawAccess(	// RETURNS: error number
    AS_StorageDevice &					device,			// IN: device reference
	AS_StorageDevice_Raw::OpenRawMode	OpenMode,		// IN: mode to open device (read/readwrite/exclusive)
	AS_StorageDevice_Raw::Access_Type	AccessType);	// IN: access type for device (sync/async)

//-----------------------------------------------------------------------------
// Close device for raw access
//
AS_StorageError AS_API AS_StorageDevice_CloseRawAccess(	// RETURNS: error number
    const AS_StorageDevice &			device,			// IN: device reference
	AS_StorageDevice::InfoCallback		callback,		// IN: progress callback proc
	void*								userdata);		// IN: progress callback userdata

//-----------------------------------------------------------------------------
// Set mode
//
AS_StorageError AS_API AS_StorageDevice_SetMode(		// RETURNS: error number
    const AS_StorageDevice &			device,			// IN: device reference
    AS_StorageDevice_Raw::WriteMode     mode);			// IN: set mode for raw access

//-----------------------------------------------------------------------------
// Get mode
//
AS_StorageError AS_API AS_StorageDevice_GetMode(		// RETURNS: error number
    const AS_StorageDevice &			device,			// IN: device reference
	AS_StorageDevice_Raw::WriteMode &   mode);			// OUT: get currently set mode 

//-----------------------------------------------------------------------------
// Raw device read
//
AS_StorageError AS_API AS_StorageDevice_Read(			// RETURNS: error number
    const AS_StorageDevice &			device,			// IN: device reference
    UInt32								blkNum,			// IN: block offset on media
    UInt32								bufLen,			// IN: length of data to read (in sectors)
    void *								buf,			// OUT: Data read
	UInt32*								numRead);		// OUT: length of data read (in sectors)

//-----------------------------------------------------------------------------
// Raw device write
//
AS_StorageError AS_API AS_StorageDevice_Write(			// RETURNS: error number
    const AS_StorageDevice &			device,			// IN: device reference
    UInt32								blkNum,			// IN: block offset on media
    UInt32								bufLen,			// IN: length of data to read (in sectors)
    void *								buf,			// IN: Data to write to offset
	UInt32*								numWritten);	// OUT: length of data written (in sectors)


//-----------------------------------------------------------------------------
// Write an image file to device.  The device state must be ready.
//
AS_StorageError AS_API AS_StorageDevice_WriteImage(		// RETURNS: error number
    const AS_StorageDevice &			device,			// IN: device reference
	enum AS_StorageDevice_Raw::Image_Type image_type,	// IN: type of image (ie AS_StorageDevice_Raw::Image_ISO)
    UInt32								flags,			// IN: flags (0)
    AS_String							filepath,		// IN: path to image file
    AS_StorageDevice::InfoCallback		callback,		// IN: progress callback proc
    void*								userdata);		// IN: progress callback userdata

//-----------------------------------------------------------------------------
// Get buffer from engine.  If none available, will block
AS_StorageError AS_API AS_StorageDevice_GetBuffer(		// RETURNS: error number
    const AS_StorageDevice &			device,			// IN: device reference
	void**								buf);			// OUT: Pointer to the buffer

//-----------------------------------------------------------------------------
// Prepare to write - enable/disable streaming bit for writing
AS_StorageError AS_API AS_StorageDevice_PrepareWrite(	// RETURNS: error number
    const AS_StorageDevice &			device,			// IN: device reference
    AS_StorageDevice_Raw::PrepareMode	mode);			// IN: flags for prepare params

//-----------------------------------------------------------------------------
// Async read blocks
//
AS_StorageError AS_API AS_StorageDevice_ReadAsync(
	const AS_StorageDevice	&			device,			// IN: source device
    UInt32								blkNum,			// IN: block offset on media
	UInt32								bufLen,			// IN:  length of data to read (in sectors)
	void*								buf,			// OUT: Pointer to the data read
	UInt32*								numRead);		// OUT: length of data read 

//-----------------------------------------------------------------------------
// Async write blocks
//
AS_StorageError AS_API AS_StorageDevice_WriteAsync(
	const AS_StorageDevice	&			device,			// IN: dest device
    UInt32								blkNum,			// IN: block offset on media
	UInt32								bufLen,			// IN:  length of data to write (in sectors)
	void*								buf,			// OUT: Pointer to the data to write
	UInt32*								numWritten,		// OUT: length of data written
	UInt32*								complete);		// OUT: completed write		

//-----------------------------------------------------------------------------
// Send raw cdbs
//
// returns: AS_StorageError_DeviceError on a check condition
//          AS_StorageError_None on success
//
AS_StorageError AS_API AS_StorageDevice_SendCommand(
	const AS_StorageDevice &			device,			// IN: dest device
	UInt32								dwCmdLen,       // IN: length of the command bytes below
	UInt8*								pCmd,           // IN: pointer to the buffer of the cdb
	UInt32								dwDataLen,      // IN: length of the data buffer
	UInt8*								pData,          // IN: pointer to the data buffer
	AS_StorageDevice_Raw::SendCommand_Flags Flags,		// IN: Flags for the command
	UInt32								dwTimeOut,      // IN: timeout value for the command
	UInt32								dwSenseBufSize, // IN:  size of the buffer for sense info.
	UInt8*								pdwSense,       // OUT: filled sense error when there's an error
	UInt32*								pdwASC,         // OUT: sense code
	UInt32*								pdwASCQ);       // OUT: sense qualifier

#ifdef __cplusplus
}
#endif

#endif //__AS_StorageDevice_Raw_h__
