///////////////////////////////////////////////////////////////////////////////
// AS_Audio.h - ATGS Team
// Copyright (c) 2001 - 2007, Sonic Solutions.  All rights reserved.
//
// Notes: This API requires AS_StorageDevice_OpenFormat to be called first to
//        get a format handle and that AS_Format_Close be called when finished.
//
//        Track numbers (as also indicated in the comments) are one-based
//        (i.e. 1 = first track).
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _AS_AudioH
#define _AS_AudioH

#include "AS_Format.h"
#include "AS_StorageTypes.h"


struct AS_Audio
{
   // Audio effects that may be applied for creation or extraction
   enum EffectType
   {
      Effect_FadeIn    = 0,
      Effect_FadeOut   = 1,
      Effect_Normalize = 2
   };

   // Supported audio file types (used also for streaming). The name of each
   // type also describes its expected three-letter file extension. Please note
   // that customers without full audio support may only use File_PCM.
   enum FileType
   {
      File_AutoDetect = 0,  // Auto-detect the file type based on the file extension. Not available for streamed data.
      File_PCM        = 1,  // Raw PCM audio (CD-DA). Audio CD creation only.
      File_WAV        = 2,  // Audio CD creation. Extract to file.
      File_MP3        = 3,  // Audio CD creation. Extract to file (when enabled).
      File_WMA        = 4,  // Audio CD creation only.
      File_M4A        = 5,  // Audio CD creation only.
   };

   // Audio-specific properties that may be set for creation. CD-Text strings
   // are of type wchar_t unless otherwise specified. The 'extPropIndex'
   // parameter of AS_Format_SetAudioTrackProperty must be set to the CD-Text
   // language type (AS_StorageDevice::CDTextLanguageType).
   enum Track_Property
   {
      Track_PregapSilenceSize = 0,  // Not available at this time
      Track_Indices           = 1,  // Not available at this time
      Track_ISRC              = 2,  // IN/OUT: 12 byte ISRC (L to R: 0-1 Country, 2-4 Registrant, 5-6 Year, 7-11 Designation)
      Track_Title             = 3,  // IN/OUT: String CD-Text Title
      Track_Performer         = 4,  // IN/OUT: String CD-Text Performer
      Track_Songwriter        = 5,  // IN/OUT: String CD-Text Songwriter
      Track_Composer          = 6,  // IN/OUT: String CD-Text Composer
      Track_Arranger          = 7,  // IN/OUT: String CD-Text Arranger
      Track_Message           = 8,  // IN/OUT: String CD-Text Message
      Track_Genre             = 9,  // IN/OUT: String CD-Text Genre
      Track_GenreCode         = 10, // IN/OUT: String CD-Text Genre Code
   };

   // Tag information that may be included when extracting CD-Audio to a file
   // format that supports such data (MP3, etc.)
   struct Track_Tags
   {
      UInt32 structSizeBytes;  // 0 = do not care or use default
      AS_String * Track;
      AS_String * Title; 
      AS_String * Artist;
      AS_String * Album;
      AS_String * Year;
      AS_String * Genre; 
      AS_String * Comment; 
      AS_String * Type;
   };

   // Audio encoder information used for extraction to file
   struct Track_EncoderSettings
   {
      UInt32 structSizeBytes;  // size of structure
      FileType type;           // file type to encode to
   };

   // DRM status values used in AS_Format_CheckAudioPlaylist
   enum DRMStatus
   {
      DRM_OK         = 0,  // DRM status OK
      DRM_NO_LICENSE = 1,  // No license
      DRM_BURN_COUNT = 2,  // Burn count exceeded
      DRM_PLAY_COUNT = 3,  // Play count exceeded
      DRM_PLAYLIST   = 4,  // Playlist restriction
      DRM_GENERIC    = 5   // Generic DRM error
   };

   // Callback method used by AS_Format_SetWMAReaderCallback to set up the
   // Microsoft Windows Media Audio (WMA) Digital Rights Management (DRM)
   // library. See the Windows Media Format SDK for further details.
   //
   // Example callback implementation:
   //
   // SInt32 SetupWMAReaderCallback(void *reserved, UInt32 rights, void **iwmReader)
   // {
   //    return WMCreateReader((IUnknown*)reserved, rights, (IWMReader**)iwmReader);
   // }
   typedef SInt32 (AS_CALLBACK *CreateWMAReaderCallback)(
      void *reserved,     // Reserved IUnknown pointer (must be zero)
      UInt32 wmtRights,   // WMT_RIGHTS flags
      void **iwmReader);  // Double-Pointer to IWMReader object
};


#ifdef __cplusplus
extern "C" {
#endif

//-----------------------------------------------------------------------------
// Audio API Functions
//-----------------------------------------------------------------------------

// Typical sequence for creating an audio CD:
//
// 1) AS_StorageDevice_OpenFormat
// 2) AS_Format_Create (audio type)
// 3) AS_Format_AddAudioTrackFromFile or AS_Format_AddAudioTrackFromStream2
// 4) AS_Format_AddAudioTrackEffect (optional)
// 5) AS_Format_SetAudioTrackProperty (optional)
// 6) AS_Volume_Flush (flush to disc or GI - based on target specified)
// 7) AS_Format_Close
//
// Note: CD-Text can be created via AS_Format_SetAudioTrackProperty by using
//       the CD-Text properties.


// Typical sequence to extract PCM tracks of an audio CD to file (WAV, MP3, etc):
//
// 1) AS_StorageDevice_OpenFormat
// 2) AS_Volume_Mount
// 3) AS_Format_AddAudioTrackEffect (optional)
// 4) for tracks 1..n AS_Format_ExtractAudioTrackToFile(tracks 1..n)
// 5) AS_Format_Close


//-----------------------------------------------------------------------------
// AS_Format_AddAudioTrackFromHD
//
// DEPRECATED:
// Please use AddAudioTrackFromFile instead. Supports AS_Audio::File_PCM only.
//
// Use of this method requires the host application to maintain and augment its
// own track index list to match that kept internally by the API. For this
// reason it is suggested that AddAudioTrackFromFile be used instead.
//
// DESCRIPTION:
// Add an audio track to media from a raw PCM audio file on hard drive.
//
// RETURN:
//  AS_StorageError_None             - ok
//  AS_StorageError_InvalidParameter - if one or more of the parameters are invalid
//
AS_StorageError AS_API AS_Format_AddAudioTrackFromHD(  // RETURNS: error number
   const AS_Format & format,             // IN: layout reference
   const AS_File::Path & fullPathOnHD);  // IN: path to hard disc file from which to copy track data - not copied to media until Volume is flushed.

//-----------------------------------------------------------------------------
// AS_Format_AddAudioTrackFromStream
//
// DEPRECATED:
// Please use AddAudioTrackFromStream2 instead. Supports AS_Audio::File_PCM only.
//
// Use of this method requires the host application to maintain and augment its
// own track index list to match that kept internally by the API. For this
// reason it is suggested that AddAudioTrackFromStream2 be used instead.
//
// DESCRIPTION:
// Add an audio track to media from a raw PCM audio stream.
//
// RETURN:
//  AS_StorageError_None             - ok
//  AS_StorageError_InvalidParameter - if one or more of the parameters are invalid
//
AS_StorageError AS_API AS_Format_AddAudioTrackFromStream(  // RETURNS: error number
   const AS_Format & format,            // IN: layout reference
   const AS_File::Size & streamLength,  // IN: size of stream (and ultimately file on media)
   AS_Volume::StreamCallback callback,  // IN: stream callback function for track data
   void * callerUse);                   // IN: stream callback userdata

//-----------------------------------------------------------------------------
// AS_Format_AddAudioTrackFromFile
//
// DESCRIPTION:
// Add an audio track to media from a file. Returns the one-based track number
// representing the file in the 'track' parameter.
//
// RETURN:
//  AS_StorageError_None             - ok
//  AS_StorageError_NotInitialized   - if the AS_Audio library could not start for non-PCM sources
//  AS_StorageError_InvalidParameter - if one or more of the parameters are invalid
//
AS_StorageError AS_API AS_Format_AddAudioTrackFromFile(  // RETURNS: error number
   const AS_Format & format,             // IN: layout reference
   const AS_File::Path & fullPath,       // IN: full path
   const AS_Audio::FileType & fileType,  // IN: source (file) format
   UInt32 & track);                      // OUT: track index reported back, 1 = first track

//-----------------------------------------------------------------------------
// AS_Format_AddAudioTrackFromStream2
//
// DESCRIPTION:
// Add an audio track to media from a stream. Returns the one-based track number
// representing the file in the 'track' parameter.
//
// The AS_Audio::FileType must be specified for stream data. Currently only
// AS_Audio::File_PCM is supported.
//
// RETURN:
//  AS_StorageError_None             - ok
//  AS_StorageError_InvalidParameter - if one or more of the parameters are invalid
//
AS_StorageError AS_API AS_Format_AddAudioTrackFromStream2(  // RETURNS: error number
   const AS_Format & format,             // IN: layout reference
   const AS_File::Size & streamLength,   // IN: size of stream (and ultimately file on media)
   AS_Volume::StreamCallback callback,   // IN: stream callback function for track data
   void * callerUse,                     // IN: stream callback userdata
   const AS_Audio::FileType & dataType,  // IN: source (stream data) format
   UInt32 & track);                      // OUT: track index reported back, 1 = first track

//-----------------------------------------------------------------------------
// AS_Format_SetAudioTrackProperty
//
// DESCRIPTION:
// Set an AS_Audio::Track_Property for the given track number.
//
// RETURN:
//  AS_StorageError_None             - ok
//  AS_StorageError_InvalidAccess    - if the given AS_Format is mounted ReadOnly
//  AS_StorageError_InvalidOperation - if called out of sequence or invalid for the given context
//  AS_StorageError_InvalidParameter - if one or more of the parameters are invalid
//
AS_StorageError AS_API AS_Format_SetAudioTrackProperty(
   const AS_Format & format,               // IN: layout reference
   const AS_Audio::Track_Property & prop,  // IN: audio property to set
   UInt32 trackNumber,                     // IN: track number, 1 = first track
   UInt32 propSize,                        // IN: size of data in propData buffer
   void * propData,                        // IN: property data
   UInt32 extPropIndex = 0,                // IN: reserved - index of extended property to return
   UInt32 extPropSize = 0,                 // IN: reserved - size available in extPropData buffer
   void * extPropData = 0);                // OUT: reserved - buffer to copy extended property value into

//-----------------------------------------------------------------------------
// AS_Format_AddAudioTrackEffect
//
// UNIMPLEMENTED:
// This method is currently unimplemented and will return
// AS_StorageError_NotImplemented if given valid parameters.
//
// DESCRIPTION:
// Add an AS_Audio::EffectType effect to the specified track number.
//
// RETURN:
//  AS_StorageError_None             - ok
//  AS_StorageError_InvalidOperation - if called out of sequence or invalid for the given context
//  AS_StorageError_InvalidParameter - if one or more of the parameters are invalid
//
AS_StorageError AS_API AS_Format_AddAudioTrackEffect(
   const AS_Format & format,           // IN: layout reference
   UInt32 trackNumber,                 // IN: track number to add the effect to, 1 = first track
   const AS_Audio::EffectType & type,  // IN: type of effect, i.e. Normalize
   UInt32 parametersSize,              // IN: size of the 'parameters' buffer in bytes
   void * parameters);                 // IN: optional parameters for the specified effect type 

//-----------------------------------------------------------------------------
// AS_Format_ExtractAudioTrackToFile
//
// DESCRIPTION:
// Extracts a track from the audio CD to a file with the given path.
//
// RETURN:
//  AS_StorageError_None             - ok
//  AS_StorageError_NotInitialized   - if the AS_Audio library could not start
//  AS_StorageError_InvalidOperation - if called out of sequence or invalid for the given context
//  AS_StorageError_InvalidParameter - if track number does not exist
//                                   - if the specified path is not valid
//                                   - if the flags are incorrect
//                                   - if the startOffset and/or length exceed the length of the track
//
AS_StorageError AS_API AS_Format_ExtractAudioTrackToFile( 
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

//-----------------------------------------------------------------------------
// AS_Format_SetWMAReaderCallback
//
// DESCRIPTION:
// Sets up the Microsoft Windows Media Audio (WMA) Digital Rights Management
// (DRM) library. See the notes above for AS_Audio::CreateWMAReaderCallback and
// the Windows Media Format SDK for further details. This method must only be
// called once and before any AS_Audio or AS_AudioFile methods are used. If
// this method is not called a default reader will be provided that only
// supports WMA decoding.
//
// RETURN:
//  AS_StorageError_None           - ok
//  AS_StorageError_NotInitialized - if the AS_Audio library could not start
//
AS_StorageError AS_API AS_Format_SetWMAReaderCallback(
   UInt32 flags,                                 // IN: Reserved flags (must be zero)
   AS_Audio::CreateWMAReaderCallback callback);  // IN: AS_Audio::CreateWMAReaderCallback

//-----------------------------------------------------------------------------
// AS_Format_CreateAudioPlaylist
//
// UNIMPLEMENTED:
// This method is currently unimplemented and will return
// AS_StorageError_NotImplemented if given valid parameters.
//
// DESCRIPTION:
// Creates a new audio playlist to support Windows Media DRM. The playlist is
// populated by making calls to AS_Format_AddAudioTrackFromFile. Adding tracks
// from streamed data is not supported.
//
// RETURN:
//  AS_StorageError_None             - ok
//  AS_StorageError_NotInitialized   - if the AS_Audio library could not start
//  AS_StorageError_InvalidOperation - if a playlist already exists
//  AS_StorageError_InvalidParameter - if the AS_Format object is invalid
//
AS_StorageError AS_API AS_Format_CreateAudioPlaylist(
   const AS_Format & format);  // IN: layout reference

//-----------------------------------------------------------------------------
// AS_Format_CheckAudioPlaylist
//
// UNIMPLEMENTED:
// This method is currently unimplemented and will return
// AS_StorageError_NotImplemented if given valid parameters.
//
// DESCRIPTION:
// Checks the existing playlist created by AS_Format_CreateAudioPlaylist to
// determine the AS_Audio::DRMStatus of each track that has been added to it
// with AS_Format_AddAudioTrackFromFile.
//
// RETURN:
//  AS_StorageError_None             - ok
//  AS_StorageError_NotInitialized   - if the AS_Audio library could not start
//  AS_StorageError_InvalidOperation - if there is no current playlist
//  AS_StorageError_InvalidParameter - if one or more parameters are invalid
//
AS_StorageError AS_API AS_Format_CheckAudioPlaylist(
   const AS_Format & format,  // IN: layout reference
   UInt32 bufferSizeBytes,    // IN: Size of result buffer in bytes
   UInt32 * results);         // OUT: Array of results in playlist order

//-----------------------------------------------------------------------------
// AS_Format_CloseAudioPlaylist
//
// UNIMPLEMENTED:
// This method is currently unimplemented and will return
// AS_StorageError_NotImplemented if given valid parameters.
//
// DESCRIPTION:
// Closes the existing audio playlist. Subsequent calls to
// AS_Format_AddAudioTrackFromFile will not make use of the Windows Media DRM
// mechanism. This method must be called before AS_Format_CreateAudioPlaylist
// may be used again.
//
// RETURN:
//  AS_StorageError_None             - ok
//  AS_StorageError_NotInitialized   - if the AS_Audio library could not start
//  AS_StorageError_InvalidOperation - if there is no current playlist
//  AS_StorageError_InvalidParameter - if the AS_Format object is invalid
//
AS_StorageError AS_API AS_Format_CloseAudioPlaylist(
   const AS_Format & format);  // IN: layout reference

#ifdef __cplusplus
}
#endif
#endif //_AS_AudioH
