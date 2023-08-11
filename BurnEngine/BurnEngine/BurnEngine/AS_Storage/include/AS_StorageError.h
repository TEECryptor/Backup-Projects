///////////////////////////////////////////////////////////////////////////////
// AS_StorageError.h
// Copyright (c) 2003 - 2007, Sonic Solutions.  All rights reserved.
///////////////////////////////////////////////////////////////////////////////


#ifndef __AS_StorageError_h__
#define __AS_StorageError_h__

#include "AS_StorageTypes.h"


typedef SInt32 AS_StorageError;
static const AS_StorageError AS_StorageError_Base					= -97000;
static const AS_StorageError AS_StorageError_None					= 0;

// General API errors
static const AS_StorageError AS_StorageError_BufferOverflow			= AS_StorageError_Base - 10;
static const AS_StorageError AS_StorageError_Fatal					= AS_StorageError_Base - 11;
static const AS_StorageError AS_StorageError_NotFound				= AS_StorageError_Base - 12;
static const AS_StorageError AS_StorageError_NotImplemented			= AS_StorageError_Base - 13;
static const AS_StorageError AS_StorageError_NotInitialized			= AS_StorageError_Base - 14;
static const AS_StorageError AS_StorageError_NotSupported			= AS_StorageError_Base - 15;
static const AS_StorageError AS_StorageError_UnsupportedVersion		= AS_StorageError_Base - 16;
static const AS_StorageError AS_StorageError_InvalidAccess			= AS_StorageError_Base - 17;
static const AS_StorageError AS_StorageError_ReentrantCall			= AS_StorageError_Base - 18;

// API verification errors
static const AS_StorageError AS_StorageError_InvalidFile			= AS_StorageError_Base - 30;
static const AS_StorageError AS_StorageError_InvalidFolder			= AS_StorageError_Base - 31;
static const AS_StorageError AS_StorageError_InvalidIndex			= AS_StorageError_Base - 32;
static const AS_StorageError AS_StorageError_InvalidLanguageCode	= AS_StorageError_Base - 33;
static const AS_StorageError AS_StorageError_InvalidMediaType		= AS_StorageError_Base - 34;
static const AS_StorageError AS_StorageError_InvalidMetadata		= AS_StorageError_Base - 35;
static const AS_StorageError AS_StorageError_InvalidMetadataName	= AS_StorageError_Base - 36;
static const AS_StorageError AS_StorageError_InvalidOperation		= AS_StorageError_Base - 37;
static const AS_StorageError AS_StorageError_InvalidParameter		= AS_StorageError_Base - 38;
static const AS_StorageError AS_StorageError_InvalidProjectType		= AS_StorageError_Base - 39;
static const AS_StorageError AS_StorageError_InvalidStringType		= AS_StorageError_Base - 40;
static const AS_StorageError AS_StorageError_InvalidTime			= AS_StorageError_Base - 41;
static const AS_StorageError AS_StorageError_InvalidTimeUnit		= AS_StorageError_Base - 42;
static const AS_StorageError AS_StorageError_InvalidTrackType		= AS_StorageError_Base - 43;
static const AS_StorageError AS_StorageError_InvalidHandle			= AS_StorageError_Base - 44;
static const AS_StorageError AS_StorageError_InvalidSequence		= AS_StorageError_Base - 45;
static const AS_StorageError AS_StorageError_InvalidLayerBreak    = AS_StorageError_Base - 46;

// Device, Media Errors
static const AS_StorageError AS_StorageError_DeviceError			= AS_StorageError_Base - 60;
static const AS_StorageError AS_StorageError_DeviceNotReady			= AS_StorageError_Base - 61;
static const AS_StorageError AS_StorageError_DeviceSelected			= AS_StorageError_Base - 62;
static const AS_StorageError AS_StorageError_MediaFull				= AS_StorageError_Base - 63;
static const AS_StorageError AS_StorageError_NotEnoughStorage		= AS_StorageError_Base - 64;
static const AS_StorageError AS_StorageError_DeviceInUse            = AS_StorageError_Base - 65;
static const AS_StorageError AS_StorageError_DeviceInQuickUse       = AS_StorageError_Base - 66;
static const AS_StorageError AS_StorageError_TrackCountOverflow		= AS_StorageError_Base - 70;
static const AS_StorageError AS_StorageError_DeviceSenseError		= AS_StorageError_Base - 71;
static const AS_StorageError AS_StorageError_FullEraseNeeded        = AS_StorageError_Base - 72;

// File Handling errors
static const AS_StorageError AS_StorageError_FileExists				= AS_StorageError_Base - 80;
static const AS_StorageError AS_StorageError_FileNotFound			= AS_StorageError_Base - 81;
static const AS_StorageError AS_StorageError_FolderExists			= AS_StorageError_Base - 82;
static const AS_StorageError AS_StorageError_FolderNotFound			= AS_StorageError_Base - 83;
static const AS_StorageError AS_StorageError_FileOpenFailed			= AS_StorageError_Base - 84;

// Metadata verification errors
static const AS_StorageError AS_StorageError_MetadataNotFound		= AS_StorageError_Base - 100;
static const AS_StorageError AS_StorageError_MetadataTypeMismatch	= AS_StorageError_Base - 101;

// User callback related error codes
static const AS_StorageError AS_StorageError_UserCancelled			= AS_StorageError_Base - 120;
static const AS_StorageError AS_StorageError_StreamCallbackError  = AS_StorageError_Base - 121;

// Verify errors
static const AS_StorageError AS_StorageError_DataMiscompare			= AS_StorageError_Base - 140;
static const AS_StorageError AS_StorageError_WriteError				= AS_StorageError_Base - 141;
static const AS_StorageError AS_StorageError_ReadError				= AS_StorageError_Base - 142;


#endif //__AS_StorageError_h__
